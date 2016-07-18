/***************************************************************************
* File Name    :  CLayoutVK26Stroke.c
* Corporation  :  Guobi Technology Ltd. GuangZhou China
* Copyright    :  Copyright Guobi Technology Ltd. And Subject to Confidentiality 
*                 Restrictions. This document is the confidential and proprietary 
*                 property of Guobi Technology Ltd.It is neither the document                  
*                 itself nor any part of it can be shared with any party  without 
*                 the written  permission. 
* Author       :  weizhiping
* Date         :  2010-08-17
* Purpose      :  虚拟Qwerty Layout实现
* Version      :  V1.0
* Last Changed List
* Name         |   date   |  Discription
****************************************************************************/
#if defined(__GBIME_VK__) // 国笔虚拟键盘

#include "CLayoutVK26Stroke.h"
#include "CLayoutViewVK26Stroke.h"

/*!
 * \brief 重载父类成员函数声明
 */
GBIMELOCAL GBIMEReturn CLayoutVK26Stroke__Release(GBLPVOID pltObj, PLayoutInfo pltUIData);
GBIMELOCAL GBIMEReturn CLayoutVK26Stroke__Init(PCLayoutVK26Stroke pltVK26);
GBIMELOCAL GBIMEReturn CLayoutVK26Stroke__Paint(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo);
GBIMELOCAL GBIMEReturn CLayoutVK26Stroke__HandleEvent(GBLPVOID pltObj, PGBIMEEvent pIMEEvent, PEngineOutputInfo pEngineOutputInfo);
GBIMELOCAL PenPosition CLayoutVK26Stroke__HitTest(GBLPVOID pltObj, 
									   GBIMEPoint hitPoint, 
									   ButtonStatusEnum buttonStatus, 
									   PEngineOutputInfo pEngineOutputInfo);
GBIMELOCAL GBIMEReturn CLayoutVK26Stroke__FuncKeyHandler(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo);

/*!
 * \brief 本地函数声明
 */
GBIMELOCAL void ICLayoutVK26Stroke__UpdateLayoutInformation(GBLPVOID pltObj, 
												ButtonStatusEnum buttonStatus,	
												PEngineOutputInfo pEngineOutputInfo);
GBIMELOCAL void ICLayoutVK26Stroke__SetTouchRegion(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo);

/*!
 * \brief 函数实现
 */
CLayoutAbstract * CLayoutVK26Stroke__Construct(PCLayoutVK26Stroke pltVK26, 
											   GBLPCVOID pltUIData,
											   GBLPCVOID pltIMData,
											   GBLPCVOID pltConfigData)
{
	CLayoutVK__Construct(&pltVK26->m_ltParent,
		pltUIData,
		pltIMData,
		pltConfigData);
	CLayoutVK26Stroke__Init(pltVK26);

	return (CLayoutAbstract *)pltVK26;
}

GBIMELOCAL GBIMEReturn CLayoutVK26Stroke__Release(GBLPVOID pltObj, PLayoutInfo pltUIData)
{
	CLASE_THIS_POITER(CLayoutVK26Stroke, pltObj)

	pltRoot->m_pfcReleaseBase(pltObj, pltUIData);
	return GBIME_OK;
}

GBIMELOCAL GBIMEReturn CLayoutVK26Stroke__Init(PCLayoutVK26Stroke pltVK26)
{
	PCLayoutViewVK26Stroke pLtView = NULL;

	// 构造对应的视图对象
	pLtView = (PCLayoutViewVK26Stroke)PT_Malloc(sizeof(CLayoutViewVK26Stroke));
	PT_Assert(pLtView != NULL);
	memset(pLtView, 0, sizeof(CLayoutViewVK26Stroke));
	CLayoutViewVK26Stroke__Construct(pLtView);

	pltVK26->m_ltParent.m_ltParent.m_hltView = (PCLayoutViewAbstract)pLtView;
	pltVK26->m_ltParent.m_ltParent.m_pfcPaint = CLayoutVK26Stroke__Paint;
	pltVK26->m_ltParent.m_ltParent.m_pfcHitTest = CLayoutVK26Stroke__HitTest;
	pltVK26->m_ltParent.m_ltParent.m_pfcUpdateLayoutInformation = ICLayoutVK26Stroke__UpdateLayoutInformation;
	pltVK26->m_ltParent.m_pfcSetTouchRegion = ICLayoutVK26Stroke__SetTouchRegion;
	pltVK26->m_ltParent.m_ltParent.m_pfcHandleEvent = CLayoutVK26Stroke__HandleEvent;
	pltVK26->m_ltParent.m_ltParent.m_pfcRelease = CLayoutVK26Stroke__Release;
	pltVK26->m_ltParent.m_ltParent.m_pfcFuncKeyHandler = CLayoutVK26Stroke__FuncKeyHandler;
	pltVK26->m_ltParent.m_ltParent.currentSyllableIndex = 0;
	pltVK26->m_ltParent.m_ltParent.currentCandidateIndex = 0;
	
	return GBIME_OK;
}

GBIMELOCAL GBIMEReturn CLayoutVK26Stroke__Paint(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo)
{
	CLASE_THIS_POITER(CLayoutVK26Stroke, pltObj)

	if (pltRoot->m_hltView != NULL 
		&& pltRoot->m_hltView->m_pfcPaint != NULL
		&& Global_GetLayoutWinActiveStatus()) // 激活状态才显示
	{
		pltRoot->m_hltView->m_pfcPaint(pltObj, pEngineOutputInfo);
	}

	return GBIME_OK;
}

GBIMELOCAL GBIMEReturn CLayoutVK26Stroke__HandleEvent(GBLPVOID pltObj, PGBIMEEvent pIMEEvent, PEngineOutputInfo pEngineOutputInfo)
{
	CLASE_THIS_POITER(CLayoutVK26Stroke, pltObj)		
	GBIMEReturn ret;

	PT_Assert(pltObj != NULL);
	PT_Assert(pIMEEvent != NULL);
	PT_Assert(pEngineOutputInfo != NULL);

	pltRoot->m_refreshFlag = GBIME_LAYOUT_REFRESH_NONE;

	// Layout窗口非激活状态必须先激活显示才能接收事件
	if (!Global_GetLayoutWinActiveStatus()
		&& !(pIMEEvent->nEvent == GBIME_EVENT_LAYOUT_ACTIVATE || pIMEEvent->nEvent == GBIME_EVENT_LAYOUT_SHOW))
	{
		return GBIME_IGNORE;
	}

	// 公共消息处理
	if ((ret = pltRoot->m_pfcHandleCommonEvent(pltObj, pIMEEvent, pEngineOutputInfo)) != GBIME_IGNORE)
	{
		return ret;
	}

	// 消息分发(完成Layout按钮状态更新、发送引擎按键事件更新引擎输出信息)
	switch (pIMEEvent->nEvent)
	{
	case GBIME_EVENT_LAYOUT_PEN_DOWN:
		ICLayoutVK26Stroke__UpdateLayoutInformation(pltObj, ButtonStatusDown, pEngineOutputInfo);
		if (pltRoot->penInPostion != PenPositionInNothing)
		{
			pltRoot->m_refreshFlag |= (GBIME_LAYOUT_REFRESH_CANDIDATE_LINE | GBIME_LAYOUT_REFRESH_KEYBOARD);
		}
		break;
	case GBIME_EVENT_LAYOUT_PEN_UP:
		ICLayoutVK26Stroke__UpdateLayoutInformation(pltObj, ButtonStatusUp, pEngineOutputInfo);
		if (pltRoot->penInPostion != PenPositionInNothing)
		{
			pltRoot->m_refreshFlag |= (GBIME_LAYOUT_REFRESH_CANDIDATE_LINE | GBIME_LAYOUT_REFRESH_KEYBOARD);
		}
		// 清除历史按键值
		pltRoot->pressLayoutKeyValue = pltRoot->currentLayoutKeyValue = GBEvent_None;
		pltRoot->penDownPostion = pltRoot->penInPostion = PenPositionInNothing;
		break;
	case GBIME_EVENT_LAYOUT_PEN_MOVE:
		if (pltRoot->penInPostion != PenPositionInNothing)
		{
			pltRoot->m_refreshFlag = (GBIME_LAYOUT_REFRESH_CANDIDATE_LINE | GBIME_LAYOUT_REFRESH_KEYBOARD);
		}
		break;
	case GBIME_EVENT_KEY_DOWN: // 物理按键直接刷新	
	case GBIME_EVENT_KEY_UP:
	case GBIME_EVENT_KEY_LONG_PRESS:
		pltRoot->m_refreshFlag = (GBIME_LAYOUT_REFRESH_CANDIDATE_LINE | GBIME_LAYOUT_REFRESH_SYLLABLE_LINE);
		break;
	default:
		break;
	}
	
	return GBIME_OK;
}

/*!
 * \brief 获取当前触摸笔按下时的位置：在候选上，在键盘按钮上，其他地方
 * \param pltObj 
 * \param hitPoint 
 * \param buttonStatus 
 * \param pEngineOutputInfo 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-8-17 16:48:11
*/
static PenPosition CLayoutVK26Stroke__HitTest(GBLPVOID pltObj, 
											  GBIMEPoint hitPoint, 
											  ButtonStatusEnum buttonStatus, 
											  PEngineOutputInfo pEngineOutputInfo)
{
	CLASE_THIS_POITER(CLayoutVK26Stroke, pltObj)

    GBUINT16 iCandidateBoxHeight = 0;
	GBUINT16 iKeyBoardHeight = 0;
	PenPosition penPositionInWhere = PenPositionInNothing;
	GBUINT16 iCurrentLayoutId;	

	//当前要显示的层
	GBUINT16 iCurrentLayerNumber;
	GBUINT16 iButtonTotalCount;
	GBIMEPoint winPos;
	PLayoutChgtLayer pLayoutChgtLayer;

	PT_PrintLogInfo(GBLOG_LAYOUTINFO, ("###################################################[x,y]=[%3d,%3d]\n", hitPoint.x, hitPoint.y));

	// 当前LayoutId
	iCurrentLayoutId = GET_LAYOUT_ID(pltRoot);
	// 获取键盘高度
	iKeyBoardHeight = pltRoot->m_pfcGetLayoutHeight(pltObj,iCurrentLayoutId);
	// 取得键盘显示的开始位置【左下角】，并调整为【左上角】
	Global_GetWinPos(&winPos);	
	winPos.y -=  iKeyBoardHeight;
	// 当前要显示的层
	iCurrentLayerNumber = GET_LAYOUT_CURRENT_LAYER(pltRoot);
	// 取候选框高度
	iCandidateBoxHeight = pltRoot->m_pfcGetCandidateBoxHeight(pltObj,iCurrentLayoutId,iCurrentLayerNumber);
	//先查第地层
	pLayoutChgtLayer = (PLayoutChgtLayer)&(pltRoot->m_ltData.pltLayerData[FirstLayer]);

	// 第一，二或三层时，都会查询键盘
	iButtonTotalCount = pLayoutChgtLayer->lyBtCount;

	// 默认为无效点击位置
	pltRoot->penInPostion = PenPositionInNothing;

	// 判断触摸笔是否落在键盘按钮上
	penPositionInWhere = pltRoot->m_pfcPenPositionInKeyboardButton(pltObj, hitPoint, buttonStatus, pEngineOutputInfo);

	// 根据当前所处的Layout层检测触摸点位置
	switch (iCurrentLayerNumber)
	{
	case FirstLayer: // 当前是第一层时
		// 设置拼笔划显示的开始位置
		//pltRoot->syllableDisplayStartPos.x = GBIME_SYLLABLE_DISPLAY_START_X;
		//pltRoot->syllableDisplayStartPos.y = winPos.y - iCandidateBoxHeight;
		pltRoot->m_pfcSetSyllableDisplayStartPos(pltObj, GBIME_SYLLABLE_DISPLAY_START_X, winPos.y - iCandidateBoxHeight);
		
		// 设置候选显示的开始位置
		//pltRoot->candidateDisplayStartPos.x = GBIME_CANDIDATE_DISPLAY_START_X;
		//pltRoot->candidateDisplayStartPos.y = winPos.y - GBIME_CANDIDATE_LINE_HEIGHT;
		pltRoot->m_pfcSetCandidateDisplayStartPos(pltObj, GBIME_CANDIDATE_DISPLAY_START_X, winPos.y - GBIME_CANDIDATE_LINE_HEIGHT);
		return penPositionInWhere;
	case SecondLayer: // 当前是第二层时
		// 第二层中按中键盘时
		if (penPositionInWhere == PenPositionInKeyboardButton
			&& pltRoot->pressLayoutKeyValue == pltRoot->currentLayoutKeyValue)
		{
			return penPositionInWhere;
		}

		// 查看是否点中了候选
		penPositionInWhere = pltBase->m_pfcPenPositionIsInCandidate(pltObj, hitPoint, buttonStatus, pEngineOutputInfo);
		if (penPositionInWhere == PenPositionInCandidate
			&& pltRoot->pressLayoutKeyValue == pltRoot->currentLayoutKeyValue)
		{
			return penPositionInWhere;
		}

		// 查看是否点中了候选翻页按钮
		penPositionInWhere = pltBase->m_pfcPenPositionIsInCandidatePageButton(pltObj, hitPoint, buttonStatus);
		if ((penPositionInWhere == PenPositionInCandidatePageDown || penPositionInWhere == PenPositionInCandidatePageUp)
			&& pltRoot->pressLayoutKeyValue == pltRoot->currentLayoutKeyValue)
		{
			return penPositionInWhere;
		}
		break;
	case ThirdLayer: //当前是三层时		
		if (penPositionInWhere == PenPositionInKeyboardButton
			&& pltRoot->pressLayoutKeyValue == pltRoot->currentLayoutKeyValue) // 第三层中按中键盘时
		{
			return penPositionInWhere;
		}
				
		// 查看是否点中了候选
		penPositionInWhere = pltBase->m_pfcPenPositionIsInCandidate(pltObj, hitPoint, buttonStatus, pEngineOutputInfo);
		if (penPositionInWhere == PenPositionInCandidate
			&& pltRoot->pressLayoutKeyValue == pltRoot->currentLayoutKeyValue)
		{
			return penPositionInWhere;
		}

		// 查看是否点中了候选翻页按钮
		penPositionInWhere = pltBase->m_pfcPenPositionIsInCandidatePageButton(pltObj, hitPoint, buttonStatus);
		if ((penPositionInWhere == PenPositionInCandidatePageDown || penPositionInWhere == PenPositionInCandidatePageUp)
			&& pltRoot->pressLayoutKeyValue == pltRoot->currentLayoutKeyValue)
		{
			return penPositionInWhere;
		}
		break;
	default:
		break;
	}

	return penPositionInWhere;
}

/*!
 * \brief 更新图层信息(控制层的切换、通知平台输入框调整高度)
 * \param pltObj 
 * \param buttonStatus 
 * \param pEngineOutputInfo 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-8-17 16:49:58
*/
static void ICLayoutVK26Stroke__UpdateLayoutInformation(GBLPVOID pltObj, ButtonStatusEnum buttonStatus, PEngineOutputInfo pEngineOutputInfo)
{
	CLASE_THIS_POITER(CLayoutVK26Stroke, pltObj)

	GBUINT16 iCandidateBoxHeight = 0;
	GBUINT16 iKeyBoardHeight = 0;
	GBUINT16 iCurrentLayoutId;

	// 当前要显示的层
	GBUINT16 iCurrentLayerNumber;
	GBIMEPoint winPos;
	GBIMEReturn ret;

	// 当前LayoutId
	iCurrentLayoutId = GET_LAYOUT_ID(pltRoot);
	// 获取键盘高度
	iKeyBoardHeight = pltRoot->m_pfcGetLayoutHeight(pltObj,iCurrentLayoutId);
	// 取得键盘显示的开始位置【左下角】，并调整为【左上角】
	Global_GetWinPos(&winPos);	
	winPos.y -=  iKeyBoardHeight;
	// 当前要显示的层
	iCurrentLayerNumber = GET_LAYOUT_CURRENT_LAYER(pltRoot);

	// 点击键盘区域如果按下和弹起键值不一致不作响应
	if (pltRoot->penInPostion == PenPositionInKeyboardButton
		&& pltRoot->pressLayoutKeyValue != pltRoot->currentLayoutKeyValue)
	{
		return ;
	}

	// 点击Layout有效区域需要更新触摸区
	if (pltRoot->penInPostion != PenPositionInNothing) 
	{
		ICLayoutVK26Stroke__SetTouchRegion(pltObj, pEngineOutputInfo);
	}
	
	if (pltRoot->penInPostion == PenPositionInKeyboardButton)
	{
		// 输入键弹起进入第二层
		if (buttonStatus == ButtonStatusUp)
		{
			if (pltRoot->m_pfcIsInputKey(pltRoot->currentLayoutKeyValue))
			{
				pltRoot->currentCandidateIndex = 0; // 清零候选索引
				pltRoot->m_ltData.ltCurLayer = SecondLayer;
				iCandidateBoxHeight = pltRoot->m_pfcGetCandidateBoxHeight(pltObj,iCurrentLayoutId, SecondLayer);
				pltRoot->m_pfcChangeHeight(pltObj, iKeyBoardHeight+iCandidateBoxHeight);
			}
		}
	}

	if (buttonStatus == ButtonStatusUp)
	{
		switch (iCurrentLayerNumber)
		{
		case FirstLayer: // 当前是第一层时	
			// 关闭候选框
			pltRoot->m_pfcCloseCandWin(pltObj, pEngineOutputInfo);
			break;
		case SecondLayer: // 当前是第二层时
			// 上屏处理
			pltRoot->m_pfcCheckUpScreen(pltObj, buttonStatus, pEngineOutputInfo);
			
			// 关闭候选框
			if ((ret = pltRoot->m_pfcCloseCandWin(pltObj, pEngineOutputInfo)) != GBIME_IGNORE)
			{
				return ;
			}				
	
			// 判断是否进入联想状态
			if (GBEngine_IsAssociateStatus(Global_GetEnineInstance()))
			{
				pltRoot->m_pfcShowAssociateWin(pltObj, buttonStatus, pEngineOutputInfo);
			}

			if ((pltRoot->penInPostion == PenPositionInCandidatePageDown)
				||(pltRoot->penInPostion == PenPositionInCandidatePageUp))
			{
				// 在第2层中，点中候选翻页时，还是进入第2层
				pltRoot->m_ltData.ltCurLayer = SecondLayer;
				return;
			}
			break;
		case ThirdLayer: // 当前是三层时
			// 上屏处理
			pltRoot->m_pfcCheckUpScreen(pltObj, buttonStatus, pEngineOutputInfo);
			
			// 关闭候选框
			if ((ret = pltRoot->m_pfcCloseCandWin(pltObj, pEngineOutputInfo)) != GBIME_IGNORE)
			{
				return ;
			}
	
			if (pltRoot->penInPostion == PenPositionInCandidate)
			{
				pltRoot->m_pfcShowAssociateWin(pltObj, buttonStatus, pEngineOutputInfo);
				return;
			}	

			if ((pltRoot->penInPostion == PenPositionInCandidatePageDown)
				||(pltRoot->penInPostion == PenPositionInCandidatePageUp))
			{
				// 在第3层中，点中候选翻页时，还是进入第3层
				pltRoot->m_ltData.ltCurLayer = ThirdLayer;
				return;
			}
			break;
		default:
			break;
		}
	}
}

/*!
 * \brief 设置触摸区域
 * \param pltObj 
 * \param pEngineOutputInfo 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-10 11:30:08
*/
static void ICLayoutVK26Stroke__SetTouchRegion(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo)
{
	CLASE_THIS_POITER(CLayoutVK26Stroke, pltObj)
	pltBase->m_pfcSetCandidateTouchRegion(pltObj, pEngineOutputInfo); // 候选栏触摸区
	pltBase->m_pfcSetCandidatePageButtonTouchRegion(pltObj, pEngineOutputInfo);// 候选翻页触摸区
}

/*!
 * \brief 功能键处理函数
 * \param pltObj 
 * \param pEngineOutputInfo 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-8-25 11:22:00
*/
GBIMELOCAL GBIMEReturn CLayoutVK26Stroke__FuncKeyHandler(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo)
{
	CLASE_THIS_POITER(CLayoutVK26Stroke, pltObj)
	GBUINT16 tempStr[2];

	pltRoot->bUpToInputKeySequence = GBFalse;

	switch (pltRoot->currentLayoutKeyValue)
	{
	case GBKEY_VK_SETTING: ///<设置
		Global_FillEventGroup(GBIME_CT_CONFIG_MENU, 0, 0);
		break;
	case GBKEY_VK_V9_PINYIN_SEPARATOR://拼音分隔符
		if (pEngineOutputInfo->bCandWindOpen) // 引擎处理过程中，输入分隔符
		{
			pltRoot->bSendEngineKeyEvent = GBTrue;
			pltRoot->engineKeyEvent = GBKEY_AMB_SPECIAL_A;
			Global_FillEventGroup(GBIME_CT_NORMAL_KEY_UP, 0, 0);
		}
		break;
	case GBKEY_VK_V9_SYMBOL_KEY: // MultiTap标点符号处理		
		pltRoot->m_pfcMultiTapSymbolHandler(pltObj);
		Global_FillEventGroup(GBIME_CT_KEY_UP_REFRESH, 0, 0); // 按钮弹起刷新
		break;
	case GBKEY_VK_V9_SYMBOL_FACE_MULTIP_KEY: // MultiTap表情符号处理		
		pltRoot->m_pfcMultiTapFaceHandler(pltObj);
		Global_FillEventGroup(GBIME_CT_KEY_UP_REFRESH, 0, 0); // 按钮弹起刷新
		break;
	case GBKEY_RETURN: // 无输入时，点击【回车】键，换行。输入时，点击【回车】键，候选首字上屏
		if (pEngineOutputInfo->bCandWindOpen)
		{
			pltRoot->bSendEngineKeyEvent = GBTrue;
			pltRoot->engineKeyEvent = GBEVENT_SELECT_CANDIDATE;
			pltRoot->engineEventParam = pltRoot->currentCandidateIndex;
			pltRoot->bUpToInputKeySequence = GBTrue;
			Global_FillEventGroup(GBIME_CT_NORMAL_KEY_UP, 0, 0);
		}
		else
		{
			tempStr[0] = GBKEY_RETURN;
			tempStr[1] = 0;
			pltRoot->m_pfcUpScreen(pltObj, tempStr);
			Global_FillEventGroup(GBIME_CT_KEY_UP_REFRESH, 0, 0); // 按钮弹起刷新
		}
		break;
	case GBKEY_SPACE: // 无输入时，点击【空格】键，输入空格。输入时，点击【空格】键，候选首字上屏
		if (pEngineOutputInfo->bCandWindOpen)
		{
			pltRoot->bSendEngineKeyEvent = GBTrue;
			pltRoot->engineKeyEvent = GBEVENT_SELECT_CANDIDATE;
			pltRoot->engineEventParam = pltRoot->currentCandidateIndex;
			pltRoot->bUpToInputKeySequence = GBTrue;
			Global_FillEventGroup(GBIME_CT_NORMAL_KEY_UP, 0, 0);
		}
		else
		{
			tempStr[0] = GBKEY_SPACE;
			tempStr[1] = 0;
			pltRoot->m_pfcUpScreen(pltObj, tempStr);
			Global_FillEventGroup(GBIME_CT_KEY_UP_REFRESH, 0, 0); // 按钮弹起刷新
		}
		break;
	case GBKEY_BACK:
		if (pEngineOutputInfo->bCandWindOpen)
		{
			if (pEngineOutputInfo->isMultiTapInputMode)
			{
				pltRoot->bSendEngineKeyEvent = GBTrue;
				pltRoot->engineKeyEvent = GBKEY_OK;
				Global_FillEventGroup(GBIME_CT_NORMAL_KEY_UP, GBKEY_OK, 0);
			}
			else
			{
				Global_FillEventGroup(GBIME_CT_NORMAL_KEY_UP, GBKEY_BACK, 0);
			}
		}
		else
		{
			GBInputBox_DeleteChar();
			Global_FillEventGroup(GBIME_CT_KEY_UP_REFRESH, GBKEY_BACK, 0);
		}
		break;
	default:
		return GBIME_IGNORE;
		break;
	}
	
	return GBIME_OK;
}

#endif // 国笔虚拟键盘