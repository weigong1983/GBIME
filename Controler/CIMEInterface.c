/***************************************************************************
* File Name    :  CMessage.c
* Corporation  :  Guobi Technology Ltd. GuangZhou China
* Copyright    :  Copyright Guobi Technology Ltd. And Subject to Confidentiality 
*                 Restrictions. This document is the confidential and proprietary 
*                 property of Guobi Technology Ltd.It is neither the document                  
*                 itself nor any part of it can be shared with any party  without 
*                 the written  permission. 
* Author       :  fanbo
* Date         :  2010-06-08
* Purpose      :  消息处理实现文件
* Version      :  V1.0
* Last Changed List
* Name         |   date   |  Discription
****************************************************************************/
#include "PlatformTools.h"
#include "PlatformAdapter.h"
#include "PlatformInputBox.h"
#include "CIMEInterface.h"
#include "gbft_def.h"
#include "GB_Global.h"
#include "CSwitchLayout.h"
#include "CSwitchInputMethod.h"
#include "GBIMEUtils.h"
#include "CHWEngine.h"
#include "CConfigMenu.h"

/*!
 * \brief 框架内部各个子模块的消息处理函数
 */
GBIMELOCAL GBIMEReturn Framework_HandleMessage(PGBIMEEvent pIMEEvent);
GBIMELOCAL GBIMEReturn Interface__DispatchEvent(GBIMEEventGroup * pIMEEvtGroup);
GBIMELOCAL GBIMEReturn Interface_PreHandleEvent(GBINT nPlatformEvent, GBINT wParam, GBINT lParam);
GBIMELOCAL GBIMEReturn Interface_PostHandleEvent(GBINT nPlatformEvent, GBINT wParam, GBINT lParam);

/*!
 * \brief 初始化框架接口对象
 * \param void 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-8-11 9:33:50
*/
GBAPI void Interface_InitIME(void)
{
	Global_InitIMEInterface();
}

/*!
 * \brief 判断框架接口对象是否已经初始化
 * \param void 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-8-11 9:33:50
*/
GBAPI GBBOOL Interface_IsIMEInited(void)
{
	return Global_IMEInterfaceIsInited();
}

/*!
 * \brief 获取IME模块连接状态
 * \param void 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-8-11 9:33:50
*/
GBAPI GBBOOL Interface_IsIMEConnected(void)
{
	return Global_IMEIsConnected();
}

/*!
 * \brief 设置IME模块连接状态
 * \param GBBOOL 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-8-11 9:33:50
*/
GBAPI void Interface_SetIMEConnectedStatus(GBBOOL status)
{
	Global_SetIMEConnectedStatus(status);
}

/*!
 * \brief 消息处理函数(由具体Layout实例对象负责将平台消息转换为IME框架消息)
 * \param nPlatformEvent 
 * \param wParam 
 * \param lParam 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-6-22 14:58:51
 */
GBAPI GBINT32 Handle_GBIMEInterface(GBINT nPlatformEvent, GBINT wParam, GBINT lParam)
{
	PCLayoutAbstract pltObj = Global_GetCurrentLayoutObject();
	GBIMEReturn ret = GBIME_IGNORE; 
	PEngineOutputInfo pEngineOutputInfo = CLayoutControl_GetEngineOutputInfo();

	if (pltObj == NULL)
	{
		return GBIME_LAYOUT_NOEXIST;
	}

	if (!Global_IMEInterfaceIsInited())
	{
		return GBIME_UNINITIALIZED;
	}

	// 非连接状态不做处理
	if (!Interface_IsIMEConnected())
	{
		return GBIME_IGNORE;
	}

	// 清空事件组，避免由于未重新填充处理了历史事件
	Global_ResetEventGroup();
	
	// 框架层消息预处理未结束则继续交给Layout对象处理
	if ((ret = Interface_PreHandleEvent(nPlatformEvent, wParam, lParam)) == GBIME_IGNORE)
	{
		/*! Layout消息预处理: 解析为一组相互关联的框架事件集、
		设置是否需要发送相应的按键消息给国笔引擎处理的标记并记录按键值(实际处理消息时用到)
		 */
		if (pltObj->m_pfcPreHandleEvent)
		{
			CLayoutControl_UpdateOutputInfo();
			if ((ret = pltObj->m_pfcPreHandleEvent(pltObj, nPlatformEvent, wParam, lParam, pEngineOutputInfo)) != GBIME_OK)
			{
				return ret;
			}
		}
	}

	// 框架事件组消息分发(循环调用Handle_GBIMEMessage处理每一个事件)
	if ((ret = Interface__DispatchEvent(Global_GetIMEEventGroup())) != GBIME_OK)
	{
		return ret;
	}

	// Note: 如果跳转到设置菜单不要重新设置功能键回调(此时输入法面板处于非激活状态)
	//if (pltObj->currentLayoutKeyValue != GBKEY_VK_SETTING)
	if (Global_GetLayoutWinActiveStatus())
	{
		// GBIME框架处理消息完毕并且国笔输入框打开则注册平台的功能按键为框架自己的处理函数
		if (ret == GBIME_OK && pEngineOutputInfo->bCandWindOpen)
		{
			GBInputBox_SetAllIMEFuncKey();			
		}
		else
		{
			GBInputBox_SetAllPlatformFuncKey();
		}

		// 更新平台Softkey标签
		GBInputBox_UpdateSoftKeyLabel();
	}

	// 框架消息后处理
	if ((ret = Interface_PostHandleEvent(nPlatformEvent, wParam, lParam)) != GBIME_IGNORE)
	{
		return ret;
	}

	return ret;
}


/*!
 * \brief IME 框架消息处理函数(注意在恰当时机调用按钮的btpfcResponse响应函数)
 * \param pIMEEvent 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-6-22 14:58:27
 */
GBAPI GBIMEReturn Handle_GBIMEMessage(PGBIMEEvent pIMEEvent)
{
	GBIMEReturn ret = GBIME_IGNORE;

	if (!Global_IMEInterfaceIsInited())
	{
		return GBIME_UNINITIALIZED;
	}

	switch (pIMEEvent->nType)
	{
	case GBIME_EVENT_TYPE_FRAMEWORK:
		if ((ret = Framework_HandleMessage(pIMEEvent)) != GBIME_IGNORE)
		{
			break; // 该模块已处理
		}
	case GBIME_EVENT_TYPE_ENGINE: ///<国笔引擎事件		
		if ((ret = GBEngine_HandleMessage(pIMEEvent)) != GBIME_IGNORE)
		{
			break; // 该模块已处理
		}
#if defined(__GBIME_HANDWRITE__)
	case GBIME_EVENT_TYPE_HANDWRITE:		
		if ((ret = GBHW_HandleMessage(pIMEEvent)) != GBIME_IGNORE)
		{
			break; // 该模块已处理
		}
#endif
	case GBIME_EVENT_TYPE_LAYOUT: ///<Layout事件		
		if ((ret = CLayoutControl_HandleMessage(pIMEEvent)) != GBIME_IGNORE)
		{
			break; // 该模块已处理
		}
	case GBIME_EVENT_TYPE_INPUT_METHOD: // 输入法模式管理模块
		if ((ret = CSWitchInputMethod_HandleMessage(pIMEEvent)) != GBIME_IGNORE)
		{
			break; // 该模块已处理
		}
	case GBIME_EVENT_TYPE_CONFIG: ///<配置事件	
		if ((ret = GBConfig_HandleMessage(pIMEEvent)) != GBIME_IGNORE)
		{
			break; // 该模块已处理
		}
	default:
		break;
	}

	return ret;
}

/*!
 * \brief 分发事件组事件
 * \param pIMEEvtGroup 事件组
 * \return 
 * \autor huanjin
 * \data 2010-8-26
 */
static GBIMEReturn Interface__DispatchEvent(GBIMEEventGroup * pIMEEvtGroup)
{
	GBIMEReturn ret;
	GBU8 i;
	GBU8 eventNum = 0;

	PT_Assert(pIMEEvtGroup != NULL);

	eventNum = pIMEEvtGroup->eventNum;
	
	if (0 == eventNum)
	{
		return GBIME_EVENT_GROUP_IS_NULL;
	}

	for (i=0; i < eventNum; i++)
	{
		ret = Handle_GBIMEMessage(&pIMEEvtGroup->eventGroup[i]);
		if (ret != GBIME_OK)
		{
			//return ret; // fail!!
			continue;
		}
	}

	return GBIME_OK;
}

/*!
 * \brief 获取国笔引擎实例对象指针
 * \param void 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-26 15:31:24
 */
PCGBEngine Interface_GetEnineInstance(void)
{
	return Global_GetEnineInstance();
}

/*!
 * \brief 设置输入法Layout窗口的起始显示位置(即左下角相对于屏幕的坐标，因为左上角会随着候选框高度变化)
 * \param pWinPos 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-21 11:28:23
 */
GBIMEReturn Interface_SetWinPos(GBIMEPoint *pWinPos)
{
	return Global_SetWinPos(pWinPos);
}

/*!
 * \brief 获取输入法Layout窗口的左上角位置(左下角起始显示位置 + Layout窗口显示高度)
 * \param pWinPos 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-21 11:28:37
 */
GBIMEReturn Interface_GetWinPos(GBIMEPoint *pWinPos)
{
	GBIMEReturn ret;

	ret = Global_GetWinPos(pWinPos);
	if (ret == GBIME_OK)
	{
		pWinPos->y -= Interface_GetWinHeight(); // 平台需要获取左上角位置
	}
	return ret;
}

/*!
 * \brief 设置输入法窗口的显示高度
 * \param newHeight 
 * \note 高度发生变化时需要通知MTK平台编辑器进行刷新，否则会导致内容区和输入法窗口刷新不完整
 * \return 
 */
void Interface_SetWinHeight(GBINT newHeight)
{
	Global_SetLayoutWinWidth(newHeight);
}

/*!
 * \brief 获取输入法窗口的显示高度
 * \param void 
 * \note 高度发生变化时需要通知MTK平台编辑器进行刷新，否则会导致内容区和输入法窗口刷新不完整
 * \return 
 */
GBINT Interface_GetWinHeight(void)
{
	return Global_GetLayoutWinHeight();
}

/*!
 * \brief 获取输入法窗口的显示宽度
 * \param newWidth 
 * \note 宽度发生变化时需要通知MTK平台编辑器进行刷新，否则会导致内容区和输入法窗口刷新不完整
 * \return 
 */
void Interface_SetWinWidth(GBINT newWidth)
{
	Global_SetLayoutWinWidth(newWidth);
}

/*!
 * \brief 获取输入法窗口的显示宽度
 * \param void 
 * \note 宽度发生变化时需要通知MTK平台编辑器进行刷新，否则会导致内容区和输入法窗口刷新不完整
 * \return 
 */
GBINT Interface_GetWinWidth(void)
{
	return Global_GetLayoutWinWidth();
}

/*!
 * \brief 获取Layout窗口激活状态
 * \param void 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-8-11 11:45:39
*/
GBBOOL Interface_GetLayoutWinActiveStatus(void)
{
	return Global_GetLayoutWinActiveStatus();
}

/*!
 * \brief 设置Layout窗口激活状态
 * \param status 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-8-11 11:45:39
*/
void Interface_SetLayoutWinActiveStatus(GBBOOL status)
{
	Global_SetLayoutWinActiveStatus(status);
}

/*!
 * \brief 框架内部消息处理函数
 * \param pIMEEvent 
 * \return 
 */
GBIMELOCAL GBIMEReturn Framework_HandleMessage(PGBIMEEvent pIMEEvent)
{
	GBIMEReturn ret = GBIME_OK;

	if (pIMEEvent->nType != GBIME_EVENT_TYPE_FRAMEWORK)
	{
		return GBIME_IGNORE; // 非框架事件，忽略
	}

	switch (pIMEEvent->nEvent)
	{
	case GBIME_EVENT_FRAMEWORK_INIT:
		//GBInputBox_Init(); // 推迟到GBIME_Connect
		break;
	case GBIME_EVENT_FRAMEWORK_EXIT:
		Global_ReleaseIMEInterface();
		break;
	default:
		PT_Assert(0); // 非法事件
	}

	return ret;
}

/*!
 * \brief 框架层的消息预处理函数(实现一些Layout对象层面之上的全局性处理，如虚拟Layout和物理Layout之间的切换)
 * \param nPlatformEvent 
 * \param wParam 
 * \param lParam 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-10-29 14:28:57
 */
GBIMELOCAL GBIMEReturn Interface_PreHandleEvent(GBINT nPlatformEvent, GBINT wParam, GBINT lParam)
{
	PCLayoutAbstract pltAbstractObj = Global_GetCurrentLayoutObject();
	KEYBOARD_TYPE keyboardType = CLayoutInfo_GetKeyboardTypeByLayoutID(GET_LAYOUT_ID(pltAbstractObj));
	GBIMEReturn ret = GBIME_IGNORE;
	GBUINT16 targetLayoutID;
	GBIMEPoint point;
	GBRECT layoutRect = {0};
	GBIMEPoint layoutWinStartPos;
	
	point.x = wParam;
	point.y = lParam;

	// 获取Layout矩形区域
	Global_GetWinPos(&layoutWinStartPos); // note: 左下角位置
	FILL_RECT(&layoutRect, layoutWinStartPos.x,	(GBINT16)(layoutWinStartPos.y - Interface_GetWinHeight()), Interface_GetWinWidth(), layoutWinStartPos.y);

	switch(nPlatformEvent)
	{
	case GBPEVT_PEN_UP: // 物理键盘切换到虚拟键盘[必须点击Layout区域切换VK，不然影响平台光标定位功能]
	#if defined(__GBIME_VK__) // 国笔虚拟键盘
		if(point.y < layoutRect.top) // 物理Layout点击候选框以上区域需要切换到虚拟Layout
		{
			switch(keyboardType)
			{
			case KEYBOARD_KB_NUMPAD: // 切换到虚拟九键对应Layout
				GBConfig_SetKeyboardType(Global_GetConfigInstance(), KEYBOARD_VK_NUMPAD);
				if ((ret = GBPA_GetLayoutIDByPlatformInputMode(GBInputBox_GetCurrentInputMode(), &targetLayoutID)) != GBIME_OK)
				{
					return GBIME_LAYOUT_NOEXIST; // 找不到对应的虚拟键盘LayoutID
				}			
				Global_FillEventGroup(GBIME_CT_KB_2_VK_SWITCH, targetLayoutID, lParam);
				return GBIME_OK;
			case KEYBOARD_KB_QWERTY: // 切换到虚拟26键对应Layout
				GBConfig_SetKeyboardType(Global_GetConfigInstance(), KEYBOARD_VK_QWERTY);
				if ((ret = GBPA_GetLayoutIDByPlatformInputMode(GBInputBox_GetCurrentInputMode(), &targetLayoutID)) != GBIME_OK)
				{
					return GBIME_LAYOUT_NOEXIST; // 找不到对应的虚拟键盘LayoutID
				}
				Global_FillEventGroup(GBIME_CT_KB_2_VK_SWITCH, targetLayoutID, lParam);
				return GBIME_OK;
			default:
				break;
			}
		}
	#endif // 国笔虚拟键盘
		break;
	case GBPEVT_KEY_DOWN: // 虚拟键盘触发物理按键
	#if defined(__GBIME_KB__) // 国笔物理键盘
		switch(keyboardType)
		{
		case KEYBOARD_VK_NUMPAD:
		case KEYBOARD_VK_QWERTY:
			if (IS_VK_SHOULD_HANDLE_PHYSIC_FUNC_KEY(wParam))
			{
				//return GBIME_OK; // 还需要同步Layout状态，暂时不调试，先屏蔽
				return GBIME_IGNORE; // 虚拟键盘下需要特殊响应的物理按键继续交给m_pfcPhysicFuncKeyHandler处理
			}
			else // 虚拟键盘切换到物理键盘
			{
				GBConfig_SetKeyboardType(Global_GetConfigInstance(), KEYBOARD_KB_NUMPAD);
				if ((ret = GBPA_GetLayoutIDByPlatformInputMode(GBInputBox_GetCurrentInputMode(), &targetLayoutID)) != GBIME_OK)
				{
					return GBIME_LAYOUT_NOEXIST; // 找不到对应的虚拟键盘LayoutID
				}
				Global_FillEventGroup(GBIME_CT_VK_2_KB_SWITCH, targetLayoutID, wParam);
				Global_SetVk2KbSwitchFlag(GBTrue);
				Global_SetVk2KbSwitchEngineKeyValue(wParam);
			}
			return GBIME_OK;
		default:
			break;
		}
	#endif // 国笔物理键盘
		break;
	default:
		break;
	}

	return ret;
}

/*!
 * \brief 框架消息后处理(有些情况下Layout内部会要求框架重新处理新的一组事件)
 * \param nPlatformEvent 
 * \param wParam 
 * \param lParam 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-12-9 8:41:24
*/
GBIMELOCAL GBIMEReturn Interface_PostHandleEvent(GBINT nPlatformEvent, GBINT wParam, GBINT lParam)
{
	GBIMEReturn ret = GBIME_IGNORE; 

	// 框架事件组消息分发(循环调用Handle_GBIMEMessage处理每一个事件)
	if ((ret = Interface__DispatchEvent(Global_GetIMEPostEventGroup())) != GBIME_OK)
	{
		return ret;
	}

	return ret;
}
