/***************************************************************************
* File Name    :  GBIME.c
* Corporation  :  Guobi Technology Ltd. GuangZhou China
* Copyright    :  Copyright Guobi Technology Ltd. And Subject to Confidentiality 
*                 Restrictions. This document is the confidential and proprietary 
*                 property of Guobi Technology Ltd.It is neither the document
*                 itself nor any part of it can be shared with any party  without 
*                 the written  permission. 
* Author       :  fanbo
* Date         :  2010-06-08
* Purpose      :  输入法框架适配层（适配框架，提供API供平台调用，平台无需知道框架实现细节）
* Version      :  V1.0
* Last Changed List
* Name         |   date   |  Discription
****************************************************************************/
#include "GBIME.h"
#include "gbft_def.h"
#include "CIMEInterface.h"
#include "PlatformTools.h"
#include "PlatformAdapter.h"
#include "CIMEInterface.h"
#include "CLayoutInfo.h"
#include "GB_Global.h"
#include "CGBEngine.h"

/*!
* \brief 国笔输入法框架创建
* \param void
* \return 
* \note 
* \example
* \author fanbo
* \date 2010-06-08
*/
GBIMEReturn GBIME_Create(void)
{
	GBIMEReturn ret;
	GBIMEEvent imeEvent;

	// 防止重复初始化
	if (Interface_IsIMEInited())
	{
		return GBIME_OK;
	}

	// 初始化框架接口对象
	Interface_InitIME();

	// Init Framework
	imeEvent.nType = GBIME_EVENT_TYPE_FRAMEWORK;
	imeEvent.nEvent = GBIME_EVENT_FRAMEWORK_INIT;
	if ((ret = Handle_GBIMEMessage(&imeEvent)) != GBIME_OK)
	{
		return ret;
	}

	// Init Engine
	imeEvent.nType = GBIME_EVENT_TYPE_ENGINE;
	imeEvent.nEvent = GBIME_EVENT_ENGINE_INIT;
	if ((ret = Handle_GBIMEMessage(&imeEvent)) != GBIME_OK)
	{
		return ret;
	}

	// Init Handwrite Engine
#if defined(__GBIME_HANDWRITE__)
	imeEvent.nType = GBIME_EVENT_TYPE_HANDWRITE;
	imeEvent.nEvent = GBIME_EVENT_HANDWRITE_ENGINE_INIT;
	if ((ret = Handle_GBIMEMessage(&imeEvent)) != GBIME_OK)
	{
		return ret;
	}
#endif

	// 加载配置
	imeEvent.nType = GBIME_EVENT_TYPE_CONFIG;
	imeEvent.nEvent = GBIME_EVENT_CONFIG_LOAD;
	if ((ret = Handle_GBIMEMessage(&imeEvent)) != GBIME_OK)
	{
		return ret;
	}

	return ret;
}

/*!
 * \brief 国笔输入法框架和平台输入界面连接(切换到首选默认输入模式并创建对应Layout显示面板)
 * \param platformInputMode 平台启动输入法时的默认输入法模式
 * \return 
 * \note 必须先调用GBIME_Create
 * \example
 * \author weizhiping
 * \date 2010-8-20 10:20:58
*/
GBIMEReturn GBIME_Connect(GBINT platformInputMode)
{
	GBInputBox_Init();
	// 设置IME模块连接状态
	Interface_SetIMEConnectedStatus(GBTrue);
	return GBIME_OK;
}

/*!
 * \brief 国笔输入法框架切换输入模式
 * \param platformInputMode
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-8-20 10:20:58
*/
GBIMEReturn GBIME_SwitchInputMode(GBINT platformInputMode)
{
	GBIMEReturn ret;
	GBIMEEvent imeEvent;
	GBUINT16 nInitialLayoutID = 0;

	PT_Assert(Interface_IsIMEInited());

	if ((ret = GBPA_GetLayoutIDByPlatformInputMode(platformInputMode, &nInitialLayoutID)) != GBIME_OK)
	{
		PT_Assert(ret != GBIME_OK); // 不支持的输入法模式
		return ret;
	}
	
	// 切换到初始Layout 输入法面板
	imeEvent.nType = GBIME_EVENT_TYPE_LAYOUT;
	imeEvent.nEvent = GBIME_EVENT_LAYOUT_SWITCH;
	imeEvent.wParam = nInitialLayoutID;
	Handle_GBIMEMessage(&imeEvent);

	// 切换输入法模式
	imeEvent.nType = GBIME_EVENT_TYPE_INPUT_METHOD;
	imeEvent.nEvent = GBIME_EVENT_INPUT_METHOD_SWITCH;
	imeEvent.wParam = nInitialLayoutID;
	Handle_GBIMEMessage(&imeEvent);

	// 每次切换Layout之后重新激活国笔输入法面板
	imeEvent.nType = GBIME_EVENT_TYPE_LAYOUT;
	imeEvent.nEvent = GBIME_EVENT_LAYOUT_ACTIVATE;
	ret = Handle_GBIMEMessage(&imeEvent);

	return GBIME_OK;
}

/*!
 * \brief 国笔输入法框架和平台输入界面断开连接
 * \param void 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-8-31 14:17:17
*/
GBIMEReturn GBIME_Disconnect(void)
{
 	GBIMEReturn ret;
	GBIMEEvent imeEvent;
// 	
// 	PT_Assert(Interface_IsIMEInited());
// 	
// 	// Reset Engine
// 	imeEvent.nType = GBIME_EVENT_TYPE_ENGINE;
// 	imeEvent.nEvent = GBIME_EVENT_ENGINE_RESET;
// 	if ((ret = Handle_GBIMEMessage(&imeEvent)) != GBIME_OK)
// 	{
// 		return ret;
// 	}

	//GBIME_Hide();
	// 注销国笔输入法面板[某些某些应用第一次进入会断开imc调到此函数，而在输入界面进入时隐藏VK会导致inline编辑框无法上屏]
	imeEvent.nType = GBIME_EVENT_TYPE_LAYOUT;
	imeEvent.nEvent = GBIME_EVENT_LAYOUT_DEACTIVATE;
	ret = Handle_GBIMEMessage(&imeEvent);

	// 清除IME模块连接状态
	Interface_SetIMEConnectedStatus(GBFalse);
	
	return GBIME_OK;
}

/*!
* \brief 国笔输入法框架销毁
* \return 
* \note 先销毁框架内部子模块，最后才销毁框架
* \example
* \author fanbo
* \date 2010-06-08
*/
GBIMEReturn GBIME_Destroy(void)
{
	GBIMEReturn ret;
	GBIMEEvent imeEvent;

	if (!Interface_IsIMEInited())
	{
		return GBIME_OK;
	}
	
	// exit Engine
	imeEvent.nType = GBIME_EVENT_TYPE_ENGINE;
	imeEvent.nEvent = GBIME_EVENT_ENGINE_EXIT;
	if ((ret = Handle_GBIMEMessage(&imeEvent)) != GBIME_OK)
	{
		return ret;
	}

	// exit Handwrite Engine
#if defined(__GBIME_HANDWRITE__)
	imeEvent.nType = GBIME_EVENT_TYPE_HANDWRITE;
	imeEvent.nEvent = GBIME_EVENT_HANDWRITE_ENGINE_EXIT;
	if ((ret = Handle_GBIMEMessage(&imeEvent)) != GBIME_OK)
	{
		return ret;
	}
#endif

	// exit Config
	imeEvent.nType = GBIME_EVENT_TYPE_CONFIG;
	imeEvent.nEvent = GBIME_EVENT_CONFIG_EXIT;
	if ((ret = Handle_GBIMEMessage(&imeEvent)) != GBIME_OK)
	{
		return ret;
	}

	// exit Framework
	imeEvent.nType = GBIME_EVENT_TYPE_FRAMEWORK;
	imeEvent.nEvent = GBIME_EVENT_FRAMEWORK_EXIT;
	if ((ret = Handle_GBIMEMessage(&imeEvent)) != GBIME_OK)
	{
		return ret;
	}
	
	return ret;
}

/*!
 * \brief 平台输入法框架物理按键按下处理函数
 * \param key_code 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-22 14:09:23
 */
GBIMEReturn GBIME_KeyDownHandler(GBINT key_code)
{
	return Handle_GBIMEInterface(GBPEVT_KEY_DOWN, key_code, 0);
}

/*!
 * \brief 平台输入法框架物理按键弹起处理函数
 * \param key_code 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-22 14:09:23
 */
GBIMEReturn GBIME_KeyUpHandler(GBINT key_code)
{
	return Handle_GBIMEInterface(GBPEVT_KEY_UP, key_code, 0);
}

/*!
 * \brief 平台输入法框架物理按键长按处理函数
 * \param key_code 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-22 14:09:23
 */
GBIMEReturn GBIME_KeyLongPressHandler(GBINT key_code)
{
	return Handle_GBIMEInterface(GBPEVT_KEY_LONG_PRESS, key_code, 0);
}

/*!
 * \brief 平台输入法框架物理按键快速短按处理函数
 * \param key_code 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-22 14:09:23
 */
GBIMEReturn GBIME_KeyRepeatHandler(GBINT key_code)
{
	return Handle_GBIMEInterface(GBPEVT_KEY_REPEAT, key_code, 0);
}

/*!
 * \brief 平台输入法框架触摸屏按下处理函数
 * \param x
 * \param y
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-22 14:11:58
 */
GBIMEReturn GBIME_PenDownHandler(GBINT x, GBINT y)
{
	return Handle_GBIMEInterface(GBPEVT_PEN_DOWN, x, y);
}
/*!
 * \brief 平台输入法框架触摸屏弹起处理函数
 * \param x
 * \param y
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-22 14:11:58
*/
GBIMEReturn GBIME_PenUpHandler(GBINT x, GBINT y)
{
	return Handle_GBIMEInterface(GBPEVT_PEN_UP, x, y);
}

/*!
 * \brief 平台输入法框架触摸屏Move处理函数
 * \param x
 * \param y
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-22 14:11:58
*/
GBIMEReturn GBIME_PenMoveHandler(GBINT x, GBINT y)
{
	return Handle_GBIMEInterface(GBPEVT_PEN_MOVE, x, y);
}

/*!
 * \brief 国笔输入法面板弹出显示
 * \return 
 * \note 
 * \example
 * \author fanbo
 * \date 2010-06-08
 */
GBIMEReturn GBIME_Show(void)
{
	GBIMEReturn ret;
	GBIMEEvent imeEvent;

	imeEvent.nType = GBIME_EVENT_TYPE_LAYOUT;
	imeEvent.nEvent = GBIME_EVENT_LAYOUT_SHOW;
	if ((ret = Handle_GBIMEMessage(&imeEvent)) != GBIME_OK)
	{
		return ret;
	}

	return ret;
}

/*!
 * \brief 国笔输入法面板隐藏
 * \return 
 * \note 
 * \example
 * \author fanbo
 * \date 2010-06-08
 */
GBIMEReturn GBIME_Hide(void)
{
	GBIMEReturn ret;
	GBIMEEvent imeEvent;
	
	imeEvent.nType = GBIME_EVENT_TYPE_LAYOUT;
	imeEvent.nEvent = GBIME_EVENT_LAYOUT_HIDE;
	if ((ret = Handle_GBIMEMessage(&imeEvent)) != GBIME_OK)
	{
		return ret;
	}

	return ret;
}

/*!
 * \brief 国笔输入法面板强制刷新显示
 * \param void 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-17 11:17:16
*/
GBIMEReturn GBIME_Paint(void)
{
	GBIMEReturn ret;
	GBIMEEvent imeEvent;

	// Layout 窗口处于隐藏状态不重绘
	if (!Interface_GetLayoutWinActiveStatus())
	{
		return GBIME_IGNORE;
	}

	//pidongming add
	if(mmi_imc_is_key_disabled())
	{
		return GBIME_IGNORE;
	}
	//pidongming add end

	imeEvent.nType = GBIME_EVENT_TYPE_LAYOUT;
	imeEvent.nEvent = GBIME_EVENT_LAYOUT_REDRAW;
	if ((ret = Handle_GBIMEMessage(&imeEvent)) != GBIME_OK)
	{
		return ret;
	}

	return ret;
}

/*!
 * \brief 设置输入法窗口的起始显示位置(即左下角相对于屏幕的坐标，因为左上角会随着候选框高度变化)
 * \param pWinPos 
 * \note 可在框架初始化时设置，也可以动态设置
 * \return 
 */
GBIMEReturn GBIME_SetWinPos(GBIMEPoint *pWinPos)
{
	return Interface_SetWinPos(pWinPos);
}

/*!
 * \brief 获取输入法窗口的左上角位置(左下角起始显示位置 + Layout窗口显示高度)
 * \param pWinPos 
 * \note 框架处于显示状态才有效
 * \return 
 */
GBIMEReturn GBIME_GetWinPos(GBIMEPoint *pWinPos)
{
	return Interface_GetWinPos(pWinPos);
}

/*!
 * \brief 获取输入法窗口的显示
 * \param void 
 * \note 高度发生变化时需要通知MTK平台编辑器进行刷新，否则会导致内容区和输入法窗口刷新不完整
 * \return 
 */
GBINT GBIME_GetWinHeight(void)
{
	return Interface_GetWinHeight();
}
/*!
 * \brief 获取输入法窗口的显示
 * \param void 
 * \note 宽度发生变化时需要通知MTK平台编辑器进行刷新，否则会导致内容区和输入法窗口刷新不完整
 * \return 
 */
GBINT GBIME_GetWinWidth(void)
{
	return Interface_GetWinWidth();
}

/*!
 * \brief 判断坐标点是否落在Layout窗口区域内
 * \param point 
 * \return 
 */
GBBOOL GBIME_PointIsInLayoutRect(GBIMEPoint point)
{
	GBRECT layoutRect = {0};
	GBIMEPoint layoutWinStartPos;
	PCLayoutAbstract pCurrentLayoutObject = Global_GetCurrentLayoutObject();	

	Global_GetWinPos(&layoutWinStartPos); // note: 左下角位置
	// 获取Layout矩形区域
	FILL_RECT(&layoutRect, 
		layoutWinStartPos.x, 
		(GBINT16)(layoutWinStartPos.y - Interface_GetWinHeight()), 
		Interface_GetWinWidth(), 
		layoutWinStartPos.y);

	if(PT_PointIsInRect(point, layoutRect))
	{
		return GBTrue;
	}

	return GBFalse;
}

/*!
 * \brief 判断输入法框架是否需要处理此触摸屏消息
 * \param nPenEvent 
 * \param point 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-11-22 20:07:44
 */
 GBBOOL GBIME_ShouldHandlePenEvent(GBINT nPenEvent, GBIMEPoint point)
{
	GBRECT layoutRect = {0};
	GBIMEPoint layoutWinStartPos;
	PCLayoutAbstract pCurrentLayoutObject = Global_GetCurrentLayoutObject();
	Global_GetWinPos(&layoutWinStartPos); // note: 左下角位置

	/*if(IS_HANDWRITE_LAYOUT(GET_LAYOUT_ID(pCurrentLayoutObject))
		&& point.y < layoutWinStartPos.y) // 避免左右软键无法响应
	{
		return GBTrue; // 手写Layout需要全屏接收Pen消息[需要在平台识别过程中在HitTest函数中设置好candidateDisplayStartPos->Layout内部处理]
	}
	else */if(IS_KB_LAYOUT(GET_LAYOUT_ID(pCurrentLayoutObject)) && point.y < layoutWinStartPos.y)
	{
		return GBTrue; // 物理Layout点击候选框以上区域需要切换到虚拟Layout || 点击候选框区域则直接Cosume Pen消息
	}
	else if (pCurrentLayoutObject->penDownPostion != PenPositionInNothing
		&& (nPenEvent == GBPEVT_PEN_MOVE || nPenEvent == GBPEVT_PEN_UP))
	{
		return GBTrue; // 触摸笔在Layout区域按下然后离开Layout，需要刷新Layout按钮状态
	}
	else if (!Global_GetLayoutWinActiveStatus() && point.y < layoutWinStartPos.y)
	{
		return GBTrue; // 隐藏状态下允许接受Pen消息激活国笔虚拟键盘
	}
	else
	{
		return GBIME_PointIsInLayoutRect(point);
	}

	return GBFalse;
}

/*!
 * \brief 获取当前键盘类型
 * \param void 
 * \return KEYBOARD_TYPE
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-10-28
*/
KEYBOARD_TYPE GBIME_GetKeyboardType(void)
{
	return GBConfig_GetKeyboardType(Global_GetConfigInstance());
}

#if __GBIME_LOG_TO_FILE__ > 0
GBINT GBIME_OpenFile(char* filePath)
{
	return PT_OpenFile(filePath);
}
void GBIME_CloseFile()
{
	PT_CloseFile();
}
#endif

/*!
 * \brief 设置国笔输入法框架虚拟键盘
 * \param input_mode 
 * \note 
 * \return 
 */
/*void GBIME_SetVirtualKeyboard(mmi_imm_input_mode_enum input_mode)
{
	GBIME_Connect(input_mode);
	GBIME_Show();
}*/