/***************************************************************************
* File Name    :  CSwitchInputMethod.c
* Corporation  :  Guobi Technology Ltd. GuangZhou China
* Copyright    :  Copyright Guobi Technology Ltd. And Subject to Confidentiality 
*                 Restrictions. This document is the confidential and proprietary 
*                 property of Guobi Technology Ltd.It is neither the document                  
*                 itself nor any part of it can be shared with any party  without 
*                 the written  permission. 
* Author       :  fanbo
* Date         :  2010-06-08
* Purpose      :  切换输入法实现文件
* Version      :  V1.0
* Last Changed List
* Name         |   date   |  Discription
****************************************************************************/
#include "CSwitchInputMethod.h"
#include "PlatformAdapter.h"
#include "PlatformInputBox.h"
#include "GB_Global.h"

/*!
* \brief 切换到指定输入法模式
* \param tartLayoutID 
* \return 
* \note 
* \example
* \author weizhiping
* \date 2010-08-05
*/
GBIMELOCAL GBIMEReturn CSWitchInputMethod_Switch(GBUINT16 tartLayoutID)
{
	GBIMEReturn ret;
	GBINT platformInputMode;
	GBUINT16 nGBInputMode;
	GBUINT16 nLangID;
	GBUINT16 nSubLangID;
	PCLayoutControl pLtCtl = Global_GetLayoutCtl();
	PCLayoutAbstract pltCurrentObject = Global_GetCurrentLayoutObject();
	PCLayoutAbstract pltPreObject = Global_GetPrevLayoutObject();

	// 检查LayoutID是否有效
	PT_Assert(IS_VALID_LAYOUT_ID(tartLayoutID));

	// 目标Layout就是当前Layout对象(从选项菜单返回时出现此状况)
// 	if (pltCurrentObject != NULL && tartLayoutID == GET_LAYOUT_ID(pltCurrentObject))
// 	{
// 		return GBIME_OK;
// 	}

    // 前置处理
	switch (tartLayoutID)
	{
	case LayoutIdSwitchPreObj: // 返回上一个Layout
		if (pltPreObject == NULL)
		{
			return GBIME_LAYOUT_NOEXIST;
		}
		tartLayoutID = CLayoutInfo_GetLayoutID(&pltPreObject->m_ltData);
		break;
	case LayoutIdSwitchLatestObj: // 返回最近Layout
		tartLayoutID = Global_GetLatestMasterLayoutID();
		break;
	case LayoutIdSwitchLatestChnObj: // 返回最近中文Layout
		tartLayoutID = Global_GetLatestChnLayoutID();
		break;
	case LayoutIdSwitchLatestAlpObj: // 返回最近拉丁Layout
		tartLayoutID = Global_GetLatestAlpLayoutID();
		break;
	default:
		break;
	}

	// 切换到符号Layout不需要更新输入模式[这样就可以根据上一次的语言类型决定默认的符号是全角还是半角]
	if (LayoutVKSymbol == CLayoutInfo_GetLayoutTypeByID(Global_GetSkinData(), tartLayoutID))
	{
		return GBIME_OK;
	}

	// 根据LayoutID获取平台输入模式
	if ((ret = GBPA_GetPlatformInputModeByLayoutID(tartLayoutID, &platformInputMode)) != GBIME_OK)
	{
		return ret;
	}

	// 根据LayoutID获取国笔引擎输入模式信息
	if ((ret = CLayoutInfo_GetGBInputModeInfoByLayoutID(Global_GetSkinData(), 
		 tartLayoutID, &nGBInputMode, &nLangID, &nSubLangID)) != GBIME_OK)
	{
		return ret;
	}

	// 国笔引擎输入模式切换
	if ((ret = GBEngine_SwitchInputMode(Global_GetEnineInstance(), nGBInputMode, nLangID, nSubLangID)) != GBOK)
	{
		return ret;
	}

	// Abc Multitap输入模式
	if (LayoutIdSP9Jabc == tartLayoutID	|| LayoutIdSP26Jabc == tartLayoutID
		|| LayoutIdSP9JABC == tartLayoutID	|| LayoutIdSP26JABC == tartLayoutID
// 		|| LayoutIdWL9Jabc == tartLayoutID || LayoutIdWL26Jabc == tartLayoutID
// 		|| LayoutIdWL9JABC == tartLayoutID || LayoutIdWL26JABC == tartLayoutID
		)
	{
		pLtCtl->engineOutputInfo.isMultiTapInputMode = GBTrue;
		GBEngine_SetCandType(Global_GetEnineInstance(), GBCT_MultiTap_Lower_Cap);
	}
	else if(LayoutIdWL9Jabc == tartLayoutID || LayoutIdWL26Jabc == tartLayoutID)
	{
		pLtCtl->engineOutputInfo.isMultiTapInputMode = GBTrue;
		GBEngine_AttachTimerFunction(Global_GetEnineInstance()
			, GBEngine_GBRequestTimer_t, GBEngine_GBRequestKillTimer_t
			, (GBLPVOID)pltCurrentObject->m_pfcKBMultiTapTimerCallBack);
		GBEngine_SetCandType(Global_GetEnineInstance(), GBCT_MultiTap_Lower);
	}
	else if(LayoutIdWL9JABC == tartLayoutID || LayoutIdWL26JABC == tartLayoutID)
	{
		pLtCtl->engineOutputInfo.isMultiTapInputMode = GBTrue;
		GBEngine_AttachTimerFunction(Global_GetEnineInstance()
			, GBEngine_GBRequestTimer_t, GBEngine_GBRequestKillTimer_t
			, (GBLPVOID)pltCurrentObject->m_pfcKBMultiTapTimerCallBack);
		GBEngine_SetCandType(Global_GetEnineInstance(), GBCT_MultiTap_Cap);			
	}
	else
	{
		pLtCtl->engineOutputInfo.isMultiTapInputMode = GBFalse;
		GBEngine_SetCandType(Global_GetEnineInstance(), GBCT_Normal);
	}

	return GBIME_OK;
}

/*!
* \brief 通知目标平台切换到指定输入法模式
* \param tartLayoutID 
* \return 
* \note 
* \example
* \author weizhiping
* \date 2010-08-05
*/
GBIMELOCAL GBIMEReturn CSWitchInputMethod_Notify(GBUINT16 tartLayoutID)
{
	GBIMEReturn ret = GBIME_IGNORE;
	GBINT platformInputMode;
	PCLayoutAbstract pltPreObject = Global_GetPrevLayoutObject();
	KEYBOARD_TYPE keyboardType;

	// 检查LayoutID是否有效
	PT_Assert(IS_VALID_LAYOUT_ID(tartLayoutID));

	// 前置处理
	switch (tartLayoutID)
	{
	case LayoutIdSwitchPreObj: // 返回上一个Layout
		if (pltPreObject == NULL)
		{
			return GBIME_LAYOUT_NOEXIST;
		}
		tartLayoutID = CLayoutInfo_GetLayoutID(&pltPreObject->m_ltData);
		break;
	case LayoutIdSwitchLatestObj: // 返回最近Layout
		tartLayoutID = Global_GetLatestMasterLayoutID();
		break;
	case LayoutIdSwitchLatestChnObj: // 返回最近中文Layout
		tartLayoutID = Global_GetLatestChnLayoutID();
		break;
	case LayoutIdSwitchLatestAlpObj: // 返回最近拉丁Layout
		tartLayoutID = Global_GetLatestAlpLayoutID();
		break;
	default:
		break;
	}

	// 如果ShiftCap状态有效，需要转换成对应的大写Layout
	if (Global_GetShiftCapStatus())
	{
		tartLayoutID = CLayoutInfo_GetUpperLayoutID(tartLayoutID);
	}

	// 必须先根据目标LayoutID判断键盘类型
	keyboardType = CLayoutInfo_GetKeyboardTypeByLayoutID(tartLayoutID);
	if (keyboardType != GBConfig_GetKeyboardType(Global_GetConfigInstance())) // 发生键盘类型切换
	{
		GBConfig_SetKeyboardType(Global_GetConfigInstance(), keyboardType);
	}

	// 根据LayoutID获取平台输入模式
	if ((ret = GBPA_GetPlatformInputModeByLayoutID(tartLayoutID, &platformInputMode)) != GBIME_OK)
	{
		return ret;
	}

	// 通知目标平台切换到指定输入法模式
	GBInputBox_ChangeInputMode(platformInputMode, 0);
	return ret;
}

/*!
 * \brief 输入模式消息处理函数
 * \param pIMEEvent 
 * \return 
 */
GBIMEReturn CSWitchInputMethod_HandleMessage(PGBIMEEvent pIMEEvent)
{
	GBIMEReturn ret = GBIME_OK;

	if (pIMEEvent->nType != GBIME_EVENT_TYPE_INPUT_METHOD)
	{
		return GBIME_IGNORE; // 非输入模式事件，忽略
	}

	switch (pIMEEvent->nEvent)
	{
	case GBIME_EVENT_INPUT_METHOD_SWITCH:
		ret = CSWitchInputMethod_Switch((GBUINT16)pIMEEvent->wParam); ///<切换输入法事件[传入参数为LayoutID]	
		break;
	case GBIME_EVENT_INPUT_METHOD_SWITCH_NOTIFY:
		ret = CSWitchInputMethod_Notify((GBUINT16)pIMEEvent->wParam); ///<切换输入法事件[传入参数为LayoutID]	
		break;
	default:
		PT_Assert(0); // 非法事件
	}

	return ret;
}