/***************************************************************************
* File Name    :  CLayoutVK.c
* Corporation  :  Guobi Technology Ltd. GuangZhou China
* Copyright    :  Copyright Guobi Technology Ltd. And Subject to Confidentiality 
*                 Restrictions. This document is the confidential and proprietary 
*                 property of Guobi Technology Ltd.It is neither the document                  
*                 itself nor any part of it can be shared with any party  without 
*                 the written  permission. 
* Author       :  haj
* Date         :  2010-06-08
* Purpose      :  虚拟键盘Layout抽象类
* Version      :  V1.0
* Last Changed List
* Name         |   date   |  Discription
****************************************************************************/
#if defined(__GBIME_VK__) // 国笔虚拟键盘
#include "CLayoutVK.h"
#include "CLayoutViewVK.h"
#include "PlatformAdapter.h"
#include "CLayoutInfo.h"
#include "GB_Global.h"
#include "CSwitchInputMethod.h"
#include "CSwitchLayout.h"
#include "PlatformTools.h"

// extern const GBWCHAR * symbolsTable_1_Chn[];
// extern const GBWCHAR * symbolsTable_1_En[];
// extern const GBWCHAR * facesTable_0[];
// #define GBIME_SYMBOL_0_ROW_NUM	2
// #define GBIME_SYMBOL_1_ROW_NUM	2

/*!
 * \brief 成员函数声明
 */
GBIMELOCAL GBIMEReturn CLayoutVK__Init(PCLayoutVK pltVK);
//GBIMELOCAL GBIMEReturn CLayoutVK__SetInterpunctionKey(GBLPVOID pltObj);
GBIMELOCAL GBBOOL CLayoutVK__ShouldSendMsgToEngine(GBLPVOID pltObj, ButtonStatusEnum buttonStatus);
GBIMELOCAL GBIMEReturn CLayoutVK__PreHandleEvent(GBLPVOID pltObj, GBINT nPlatformEvent, GBINT wParam, GBINT lParam, PEngineOutputInfo pEngineOutputInfo);
GBIMELOCAL GBIMEReturn CLayoutVK__PhysicFuncKeyHandler(GBLPVOID pltObj, GBINT nPlatformEvent, GBINT wParam, GBINT lParam, PEngineOutputInfo pEngineOutputInfo);
GBIMELOCAL void CLayoutVK__SetSyllableTouchRegion(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo); // 音节触摸区
GBIMELOCAL void CLayoutVK__SetCandidateTouchRegion(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo); // 候选触摸区
GBIMELOCAL void CLayoutVK__SetSyllablePageButtonTouchRegion(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo); // 音节翻页按钮触摸区
GBIMELOCAL void CLayoutVK__SetCandidatePageButtonTouchRegion(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo);// 候选翻页按钮触摸区
GBIMELOCAL PenPosition CLayoutVK__PenPositionIsInPinYin(GBLPVOID pltObj, 
											 GBIMEPoint hitPoint, 
											 ButtonStatusEnum buttonStatus, 
											 PEngineOutputInfo pEngineOutputInfo); // 检测是否选中音节
GBIMELOCAL PenPosition CLayoutVK__PenPositionIsInCandidate(GBLPVOID pltObj, 
												GBIMEPoint hitPoint, 
														ButtonStatusEnum buttonStatus,
														PEngineOutputInfo pEngineOutputInfo); // 检测是否选中候选
GBIMELOCAL PenPosition CLayoutVK__PenPositionIsInSyllablePageButton(GBLPVOID pltObj, 
														 GBIMEPoint hitPoint, 
														 ButtonStatusEnum buttonStatus); // 检测是否选中音节翻页按钮
GBIMELOCAL PenPosition CLayoutVK__PenPositionIsInCandidatePageButton(GBLPVOID pltObj, 
														  GBIMEPoint hitPoint,
														  ButtonStatusEnum buttonStatus); // 检测是否选中候选翻页按钮

/*!
 * \brief 本地函数声明
 */
// GBIMELOCAL void ICLayoutVK__ResetInterpunctionKey(GBLPVOID pltObj);
// GBIMELOCAL GBUINT16 ICLayoutVK_GetStaticArraySize_symbolsTable_1_En(void);
// GBIMELOCAL GBUINT16 ICLayoutVK_GetStaticArraySize_symbolsTable_1_Chn(void);
// GBIMELOCAL GBUINT16 ICLayoutVK_GetStaticArraySize_facesTable(void);

CLayoutAbstract * CLayoutVK__Construct(PCLayoutVK pltVK, 
											  GBLPCVOID pltUIData,
											  GBLPCVOID pltIMData,
											  GBLPCVOID pltConfigData)
{
	CLayoutAbstract__Construct(&pltVK->m_ltParent,
		pltUIData,
		pltIMData,
		pltConfigData);
	CLayoutVK__Init(pltVK);

	return (CLayoutAbstract *)pltVK;
}

GBIMELOCAL GBIMEReturn CLayoutVK__Init(PCLayoutVK pltVK)
{
//	pltVK->m_ltParent.m_pfcSetInterpunctionKey = CLayoutVK__SetInterpunctionKey;
	pltVK->m_ltParent.m_pfcShouldSendMsgToEngine = CLayoutVK__ShouldSendMsgToEngine;

	pltVK->m_ltParent.m_pfcPreHandleEvent = CLayoutVK__PreHandleEvent;
	pltVK->m_ltParent.m_pfcPhysicFuncKeyHandler = CLayoutVK__PhysicFuncKeyHandler;
	pltVK->m_pfcSetSyllableTouchRegion = CLayoutVK__SetSyllableTouchRegion; // 设置音节触摸区
	pltVK->m_pfcSetCandidateTouchRegion = CLayoutVK__SetCandidateTouchRegion; // 设置候选触摸区
	pltVK->m_pfcSetSyllablePageButtonTouchRegion = CLayoutVK__SetSyllablePageButtonTouchRegion; // 设置音节翻页按钮触摸区
	pltVK->m_pfcSetCandidatePageButtonTouchRegion = CLayoutVK__SetCandidatePageButtonTouchRegion;// 设置候选翻页按钮触摸区
	pltVK->m_pfcPenPositionIsInPinYin = CLayoutVK__PenPositionIsInPinYin; // 检测是否选中音节
	pltVK->m_pfcPenPositionIsInCandidate = CLayoutVK__PenPositionIsInCandidate; // 检测是否选中候选
	pltVK->m_pfcPenPositionIsInSyllablePageButton = CLayoutVK__PenPositionIsInSyllablePageButton; // 检测是否选中音节翻页按钮
	pltVK->m_pfcPenPositionIsInCandidatePageButton = CLayoutVK__PenPositionIsInCandidatePageButton; // 检测是否选中候选翻页按钮

	return GBIME_OK;
}

/*!
 * \brief 判断是否应该向引擎发送信息
 * \param pltObj 
 * \param buttonStatus 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-9 14:28:54
*/
GBIMELOCAL GBBOOL CLayoutVK__ShouldSendMsgToEngine(GBLPVOID pltObj, ButtonStatusEnum buttonStatus)
{
	CLASE_THIS_POITER(CLayoutVK, pltObj)

	GBUINT16 iCurrentLayoutId;
	GBBOOL bShouldSendMsgToEngine = GBFalse;
	PenPosition penPosition = PenPositionInNothing;
	GBUINT16 iCurrentLayoutKeyValue = GBEvent_None;

	if (!pltBase || !pltBase->m_ltData.pltcData)
	{
		return GBFalse;
	}

	penPosition = pltBase->penInPostion;
	iCurrentLayoutId = pltBase->m_ltData.pltcData->ltID;
	iCurrentLayoutKeyValue = pltBase->currentLayoutKeyValue;
	switch(iCurrentLayoutId)
	{
	case LayoutIdSP9JPinYin:		// 9键拼音
	case LayoutIdSP9JStroke:		// 9键笔划
	case LayoutIdSP9JLowerEnglish:	// 9键英文小写
	case LayoutIdSP9JUpEnglish:		// 9键英文大写
	case LayoutIdSP9Jabc:			// 9键abc
	case LayoutIdSP9JABC:			// 9键ABC
	case LayoutIdSP26JPinYin:		// 26键拼音
	case LayoutIdSP26JLowerEnglish:	// 26键英文小写
	case LayoutIdSP26JUpEnglish:	// 26键英文大写
	case LayoutIdSP26Jabc:			// 26键abc	
	case LayoutIdSP26JABC:			// 26键ABC
		if (buttonStatus == ButtonStatusUp
			&& penPosition != PenPositionInNothing)
		{
			bShouldSendMsgToEngine = GBTrue;
		}
		break;
	case LayoutIdSP9JNumber:		// 9键数字
	case LayoutIdSP26JStroke:		// 26键笔划
	case LayoutIdSP26JNumber:		// 26键数字
		if (buttonStatus == ButtonStatusUp
			&& penPosition != PenPositionInNothing			 
			&& !LAYOUTVK_IS_NUM_KEY(iCurrentLayoutKeyValue))
		{
			bShouldSendMsgToEngine = GBTrue;
		}
		break;
	default:
		break;
	}

	return bShouldSendMsgToEngine;
}

/*!
 * \brief 该函数完成对平台原始消息的预处理
 * \param pltObj 
 * \param nPlatformEvent 
 * \param wParam 
 * \param lParam 
 * \param pEngineOutputInfo 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-10 10:11:42
*/
GBIMELOCAL GBIMEReturn CLayoutVK__PreHandleEvent(GBLPVOID pltObj, GBINT nPlatformEvent, GBINT wParam, GBINT lParam, PEngineOutputInfo pEngineOutputInfo)
{
	CLASE_THIS_POITER(CLayoutVK, pltObj)
	PLayoutInfo pLayoutInfo = (PLayoutInfo)(&pltBase->m_ltData); // Layout UI data point
	GBIMEPoint hitPoint;
	GBINT32 hitButtonIndex = -1;
	PenPosition penPositionInWhere = PenPositionInNothing;
	GBUINT16 targetLayoutID = LayoutIdSP9JPinYin;
	GBIMEReturn ret;

	// 将平台触摸屏事件转换成IME框架需要处理的事件集	
	hitPoint.x = wParam;
	hitPoint.y = lParam;
	pltBase->bSendEngineKeyEvent = GBFalse;
	pltBase->engineKeyEvent = wParam; // 这个参数需要Layout解析成具体的引擎按键值
	pltBase->engineEventParam = lParam; // 这个参数需要Layout根据具体功能按钮进行解析

	switch(nPlatformEvent)
	{
	case GBPEVT_PEN_DOWN:
		pltBase->m_pfcSlidingPenDownHandler(pltObj, hitPoint);
		penPositionInWhere = pltBase->m_pfcHitTest(pltObj, hitPoint, ButtonStatusDown, pEngineOutputInfo);
		pltBase->m_pfcLongPressKeyHandler(pltObj, pEngineOutputInfo); // 长按处理
		Global_FillEventGroup(GBIME_CT_KEY_DOWN_REFRESH, wParam, lParam);
		break;
	case GBPEVT_PEN_UP:
		// 如果是滑屏切换Layout则直接结束处理
		if ((ret = pltBase->m_pfcSlidingPenUpHandler(pltObj, hitPoint)) != GBIME_IGNORE)
		{
			pltBase->m_pfcOnMultitapTimer();
			return ret;
		}

		penPositionInWhere = pltBase->m_pfcHitTest(pltObj, hitPoint, ButtonStatusUp, pEngineOutputInfo);
	
		// 长按处理
		//pltAbstractObj->m_pfcLongPressKeyHandler(pltObj, pEngineOutputInfo);

		// 实现点击显示、隐藏Layout面板
		if (0)//penPositionInWhere == PenPositionInNothing && Global_GetLayoutWinActiveStatus())
		{
			Global_FillEventGroup(GBIME_CT_HIDE, wParam, lParam);
			break; // 直接退出switch
		}
		else if (/*penPositionInWhere != PenPositionInNothing && */!Global_GetLayoutWinActiveStatus()) // 当前Layout处于隐藏状态，需要先激活
		{
			Global_FillEventGroup(GBIME_CT_SHOW, wParam, lParam);
			break; // 直接退出switch
		}
		
		// 长按键处理
		if ((ret = pltBase->m_pfcLongPressKeyHandler(pltObj, pEngineOutputInfo)) != GBIME_IGNORE)
		{
			break; // 结束处理
		}

		// 点击键盘区域时按下和弹起键值一致才作响应
		if (penPositionInWhere != PenPositionInNothing 
			&& pltBase->pressLayoutKeyValue == pltBase->currentLayoutKeyValue)
		{
			// 同步国笔引擎的ShiftCap状态
			pltBase->m_pfcSyncEngineShiftCapStatus(pltObj);

			// 检查是否取消MultiTap符号定时器(MultiTap状态下按到了其他按键)
			if ((pltBase->bMultitapSymbolStartFlag && pltBase->currentLayoutKeyValue != GBKEY_VK_V9_SYMBOL_KEY)
				|| (pltBase->bMultitapFaceStartFlag && pltBase->currentLayoutKeyValue != GBKEY_VK_V9_SYMBOL_FACE_MULTIP_KEY))
			{
				pltBase->m_pfcOnMultitapTimer();
			}

			if(pltBase->m_pfcShouldSendMsgToEngine(pltObj, ButtonStatusUp))
			{
				pltBase->bSendEngineKeyEvent = GBTrue;
				pltBase->engineKeyEvent = CLayoutInfo_ConversionLayoutKeyToEngineKey(pltBase->currentLayoutKeyValue);
			}

			if ((ret = pltBase->m_pfcSwitchHandler(pltObj)) != GBIME_IGNORE) // Layout切换处理
			{
				break;
			}
			else if ((ret = pltBase->m_pfcFuncKeyHandler(pltObj, pEngineOutputInfo)) != GBIME_IGNORE) // 功能键处理
			{
				break;
			}
			else
			{
				if (pltBase->bSendEngineKeyEvent)
				{
					Global_FillEventGroup(GBIME_CT_NORMAL_KEY_UP, wParam, lParam);					
				}				
			}
		}
		else
		{
			Global_FillEventGroup(GBIME_CT_KEY_UP_REFRESH, wParam, lParam);
		}
	    break;
	case GBPEVT_PEN_MOVE:
		pltBase->m_pfcSlidingPenMoveHandler(pltObj, hitPoint);
		// 处理触摸笔Move消息，更新Layout按钮状态
		penPositionInWhere = pltBase->m_pfcHitTest(pltObj, hitPoint, ButtonStatusMove, pEngineOutputInfo);
		Global_FillEventGroup(GBIME_CT_MOVE, wParam, lParam);	
		break;
	case GBPEVT_KEY_DOWN:
	case GBPEVT_KEY_UP:
		// 按键处理
		pltBase->m_pfcPhysicFuncKeyHandler(pltObj, nPlatformEvent, wParam, lParam, pEngineOutputInfo);
		break;
	default:
	    break;
	}

	return GBIME_OK;
}

/*!
 * \brief 物理功能按键处理函数
 * \param pltObj 
 * \param pEngineOutputInfo 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-8-25 11:22:00
*/
GBIMELOCAL GBIMEReturn CLayoutVK__PhysicFuncKeyHandler(GBLPVOID pltObj, GBINT nPlatformEvent, GBINT wParam, GBINT lParam, PEngineOutputInfo pEngineOutputInfo)
{
	CLASE_THIS_POITER(CLayoutVK, pltObj)

	if (!pEngineOutputInfo->bCandWindOpen || nPlatformEvent != GBPEVT_KEY_DOWN 
		|| !IS_VK_SHOULD_HANDLE_PHYSIC_FUNC_KEY(wParam))
	{
		return GBIME_IGNORE;
	}
	
	pltBase->buttonStatus = ButtonStatusUp; // 记录按钮状态
	pltBase->penDownPostion = pltBase->penInPostion = PenPositionInKeyboardButton;

	switch (wParam)
	{
// 	case GBKEY_LEFT:
// 		Global_FillEventGroup(GBIME_CT_KEY_DOWN, GBKEY_LEFT, 0);
// 		break;
// 	case GBKEY_RIGHT:
// 		Global_FillEventGroup(GBIME_CT_KEY_DOWN, GBKEY_RIGHT, 0);
// 		break;
// 	case GBKEY_UP:
// 		Global_FillEventGroup(GBIME_CT_KEY_DOWN, GBKEY_UP, 0);
// 		break;
// 	case GBKEY_DOWN:
// 		Global_FillEventGroup(GBIME_CT_KEY_DOWN, GBKEY_DOWN, 0);
// 		break;
	case GBKEY_OK:
		// 转换为对应的Layout功能按钮消息
		pltBase->pressLayoutKeyValue = pltBase->currentLayoutKeyValue = GBKEY_SPACE;
		// 更新触摸区域
		pltThis->m_pfcSetTouchRegion(pltObj, pEngineOutputInfo);
				
		pltBase->bSendEngineKeyEvent = GBTrue;
		pltBase->engineKeyEvent = GBEVENT_SELECT_CANDIDATE;
		pltBase->engineEventParam = pltBase->currentCandidateIndex;
		pltBase->bUpToInputKeySequence = GBTrue;
		Global_FillEventGroup(GBIME_CT_NORMAL_KEY_UP, 0, 0);
		break;
	case GBKEY_BACK:
		// 转换为对应的Layout功能按钮消息
		pltBase->pressLayoutKeyValue = pltBase->currentLayoutKeyValue = GBKEY_BACK;
		// 更新触摸区域
 		pltThis->m_pfcSetTouchRegion(pltObj, pEngineOutputInfo);

		if (pEngineOutputInfo->isMultiTapInputMode)
		{
			pltBase->bSendEngineKeyEvent = GBTrue;
			pltBase->engineKeyEvent = GBKEY_OK;
			Global_FillEventGroup(GBIME_CT_NORMAL_KEY_UP, GBKEY_OK, 0);
		}
		else
		{
			Global_FillEventGroup(GBIME_CT_NORMAL_KEY_UP, GBKEY_BACK, 0);
		}	
		break;
	default:
		return GBIME_IGNORE;
		break;
	}
	
	return GBIME_OK;
}

/*!
 * \brief 设置拼音音节触摸区
 * \param pltObj 
 * \param pEngineOutputInfo 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-8-24 17:38:42
*/
static void CLayoutVK__SetSyllableTouchRegion(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo)
{
	CLASE_THIS_POITER(CLayoutVK, pltObj)
	GBPCWCHAR *ppCandidates;
	GBUINT16 iCandidateCount = 0;
	PT_TextAlign textAlign = PT_TEXT_ALIGN_CENTER;
	
	if (pEngineOutputInfo->isSymbolCandType/*GBEngine_IsSymbolCandType(Global_GetEnineInstance())*/)
	{
		ppCandidates = &pEngineOutputInfo->pOutputString;
		iCandidateCount = 1;		
	}
	else
	{
		ppCandidates = pEngineOutputInfo->ppSyllables;
		iCandidateCount = (GBUINT16)pEngineOutputInfo->nSyllableNum;
	}

	// 单个候选居左显示，保持产品风格一致
	if (iCandidateCount == 1)
	{
		textAlign = PT_TEXT_ALIGN_LEFT;
	}
	PT_CreateTouchRegion(&pltThis->syllableTouchRegion,			// 音节栏区域
						 pltBase->syllableDisplayStartPos,		// 音节栏显示的开始位置
						 (GBUINT16)GBIME_SYLLABLE_LINE_WIDTH,	// 音节栏宽度
						 (GBUINT16)GBIME_SYLLABLE_LINE_HEIGHT,	// 音节栏高度
						 ppCandidates,							// 音节数组
						 iCandidateCount,						// 音节个数
						 GBIME_CANDIDATE_FONT_SIZE,				// 字体
						 textAlign);							// 文本对齐
}

/*!
 * \brief 设置候选触摸区
 * \param pltObj 
 * \param pEngineOutputInfo 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-8-24 17:39:07
*/
static void CLayoutVK__SetCandidateTouchRegion(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo)
{
	CLASE_THIS_POITER(CLayoutVK, pltObj)

	PT_CreateTouchRegion(&pltThis->candidateTouchRegion,		// 候选汉字触摸区域
						 pltBase->candidateDisplayStartPos,		// 候选汉字显示的开始位置
						 (GBUINT16)GBIME_CANDIDATE_LINE_WIDTH,	// 候选栏宽度
						 (GBUINT16)GBIME_CANDIDATE_LINE_HEIGHT,	// 候选栏高度
						 pEngineOutputInfo->ppCandidates,		// 候选数组
						 (GBUINT16)pEngineOutputInfo->nCandNum,	// 候选个数
						 GBIME_CANDIDATE_FONT_SIZE,				// 字体
						 PT_TEXT_ALIGN_CENTER);					// 文本对齐
}

/*!
 * \brief 设置音节翻页按钮触摸区
 * \param pltObj 
 * \param pEngineOutputInfo 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-10 11:44:11
*/
static void CLayoutVK__SetSyllablePageButtonTouchRegion(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo)
{
	CLASE_THIS_POITER(CLayoutVK, pltObj)

	GBRECT buttonRect;
	PGBRECT pTargetRect;
	GBUINT16 iCandidateBoxHeight = 0;
	GBUINT16 iKeyBoardHeight = 0;
	GBUINT16 iButtonHeight = 0;
	GBIMEPoint winPos;
	GBUINT16 iCurrentLayoutId, iCurrentLayerNumber;
	GBUINT16 iButtonLayoutId, iButtonLayerId;

	iCurrentLayerNumber = GET_LAYOUT_CURRENT_LAYER(pltBase);
	iButtonLayoutId = iCurrentLayoutId = GET_LAYOUT_ID(pltBase);
	iButtonLayerId = (iCurrentLayerNumber == ThirdLayer) ? ThirdLayer : SecondLayer;
	
	// 取得键显示的开始位置[左下角坐标]
	Global_GetWinPos(&winPos);
	iKeyBoardHeight = pltBase->m_pfcGetLayoutHeight(pltObj, iCurrentLayoutId);
	// 键盘显示左上角Y坐标
	winPos.y -=  iKeyBoardHeight;
	// 取候选框高度
	iCandidateBoxHeight = pltBase->m_pfcGetCandidateBoxHeight(pltObj, iCurrentLayoutId, iCurrentLayerNumber);

	// 计算候选[向下]翻页的区域坐标
	pTargetRect = &pltThis->syllablePageDownOrUpRectArray[PAGE_DOWN_BUTTON_INDEX];
	pltBase->m_pfcGetBottonRectByKey(pltObj, iButtonLayoutId, iButtonLayerId, GBKEY_NEXT_PAGE_SYLLABLE, &buttonRect);
	iButtonHeight = buttonRect.bottom - buttonRect.top;
	pTargetRect->left = buttonRect.left + (GBUINT16)winPos.x;
	pTargetRect->top = (GBUINT16)winPos.y - iCandidateBoxHeight;
	pTargetRect->right = buttonRect.right + (GBUINT16)winPos.x;
	pTargetRect->bottom = (GBUINT16)winPos.y - (iCandidateBoxHeight - iButtonHeight);
	
	// 计算候选[向上]翻页的区域坐标
	pTargetRect = &pltThis->syllablePageDownOrUpRectArray[PAGE_Up_BUTTON_INDEX];
	pltBase->m_pfcGetBottonRectByKey(pltObj, iButtonLayoutId, iButtonLayerId, GBKEY_PREV_PAGE_SYLLABLE, &buttonRect);
	iButtonHeight = buttonRect.bottom - buttonRect.top;
	pTargetRect->left = buttonRect.left + (GBUINT16)winPos.x;
	pTargetRect->top = (GBUINT16)winPos.y - iCandidateBoxHeight;
	pTargetRect->right = buttonRect.right + (GBUINT16)winPos.x;
	pTargetRect->bottom = (GBUINT16)winPos.y - (iCandidateBoxHeight - iButtonHeight);
}

/*!
 * \brief 设置候选翻页按钮触摸区
 * \param pltObj 
 * \param pEngineOutputInfo 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-10 11:46:08
*/
static void CLayoutVK__SetCandidatePageButtonTouchRegion(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo)
{
	CLASE_THIS_POITER(CLayoutVK, pltObj)

	GBRECT buttonRect;
	PGBRECT pTargetRect;
	GBUINT16 iCandidateBoxHeight = 0;
	GBUINT16 iKeyBoardHeight = 0;
	GBUINT16 iButtonHeight = 0;
	GBIMEPoint winPos;
	GBUINT16 iCurrentLayoutId, iCurrentLayerNumber;
	GBUINT16 iButtonLayoutId, iButtonLayerId;
	
	iCurrentLayerNumber = GET_LAYOUT_CURRENT_LAYER(pltBase);
	iButtonLayoutId = iCurrentLayoutId = GET_LAYOUT_ID(pltBase);
	iButtonLayerId = (iCurrentLayerNumber == ThirdLayer) ? ThirdLayer : SecondLayer;
	
	// 取得键显示的开始位置[左下角坐标]
	Global_GetWinPos(&winPos);
	iKeyBoardHeight = pltBase->m_pfcGetLayoutHeight(pltObj, iCurrentLayoutId);
	// 键盘显示左上角Y坐标
	winPos.y -=  iKeyBoardHeight;
	// 取候选框高度
	iCandidateBoxHeight = pltBase->m_pfcGetCandidateBoxHeight(pltObj, iCurrentLayoutId, iCurrentLayerNumber);

	// 计算候选[向下]翻页的区域坐标
	pTargetRect = &pltThis->candidatePageDownOrUpRectArray[PAGE_DOWN_BUTTON_INDEX];
	pltBase->m_pfcGetBottonRectByKey(pltObj, iButtonLayoutId, iButtonLayerId, GBKEY_NEXT_PAGE_CANDIATE, &buttonRect);
	pTargetRect->left = buttonRect.left + (GBUINT16)winPos.x;
	pTargetRect->top = (GBUINT16)winPos.y - (iCandidateBoxHeight - buttonRect.top);
	pTargetRect->right = buttonRect.right + (GBUINT16)winPos.x;
	pTargetRect->bottom = (GBUINT16)winPos.y;

	// 计算候选[向上]翻页的区域坐标
	pTargetRect = &pltThis->candidatePageDownOrUpRectArray[PAGE_Up_BUTTON_INDEX];
	pltBase->m_pfcGetBottonRectByKey(pltObj, iButtonLayoutId, iButtonLayerId, GBKEY_PREV_PAGE_CANDIATE, &buttonRect);
	pTargetRect->left = buttonRect.left + (GBUINT16)winPos.x;
	pTargetRect->top = (GBUINT16)winPos.y - (iCandidateBoxHeight - buttonRect.top);
	pTargetRect->right = buttonRect.right + (GBUINT16)winPos.x;
	pTargetRect->bottom = (GBUINT16)winPos.y;
}

/*!
 * \brief 判断触摸笔是否点中了拼音
 * \param pltObj 
 * \param hitPoint 
 * \param buttonStatus 
 * \param pEngineOutputInfo 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-10 12:44:48
*/
static PenPosition CLayoutVK__PenPositionIsInPinYin(GBLPVOID pltObj, 
													GBIMEPoint hitPoint, 
													ButtonStatusEnum buttonStatus,
													PEngineOutputInfo pEngineOutputInfo)
{
	CLASE_THIS_POITER(CLayoutVK, pltObj)
	PenPosition penPositionInWhere = PenPositionInNothing;
	GBINT hittedIndex = -1;

	// 候选栏不处理Move消息
	if (buttonStatus == ButtonStatusMove
		|| (pltBase->penDownPostion != PenPositionInSyllable && buttonStatus == ButtonStatusUp)) // 从其他区域挪过来
	{
		return PenPositionInNothing;
	}
	
	if ((hittedIndex = PT_TouchRegionHitTest(&pltThis->syllableTouchRegion, hitPoint)) != -1) // 如果点中了拼音
	{
		// 记录点中的位置
		pltBase->penInPostion = PenPositionInSyllable;
		penPositionInWhere = PenPositionInSyllable;
		pltBase->buttonStatus = buttonStatus; // 记录按钮状态

		// 记录按下时的Layout按键值(只有当按下和弹起键值一致时才执行按键操作)
		if (buttonStatus == ButtonStatusDown)
		{
			pltBase->penDownPostion = PenPositionInSyllable;
			pltBase->pressLayoutKeyValue = GBEVENT_SELECT_SYLLABLE_PAGED;
		}

		// 记录点中的拼音索引值
		pltBase->currentLayoutKeyValue = GBEVENT_SELECT_SYLLABLE_PAGED;
		pltBase->engineEventParam = hittedIndex;
		pltBase->currentSyllableIndex = hittedIndex;
		return penPositionInWhere;
	}
	else
	{
		if (pltBase->pressLayoutKeyValue == GBEVENT_SELECT_SYLLABLE_PAGED)
		{
			// 记录按钮状态
			pltBase->buttonStatus = buttonStatus;
			// 记录点中的位置
			penPositionInWhere = pltBase->penInPostion = PenPositionLeave;			
			// 记录点中的候选索引值
			pltBase->pressLayoutKeyValue = pltBase->currentLayoutKeyValue = GBEvent_None;
			// 默认音节索引
			pltBase->currentSyllableIndex = 0;
		}
	}

	return penPositionInWhere;
}

/*!
 * \brief 判断触摸笔是否点中了候选
 * \param pltObj 
 * \param hitPoint 
 * \param buttonStatus 
 * \param pEngineOutputInfo 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-10 12:46:31
*/
static PenPosition CLayoutVK__PenPositionIsInCandidate(GBLPVOID pltObj, 
													   GBIMEPoint hitPoint, 
													   ButtonStatusEnum buttonStatus,
													   PEngineOutputInfo pEngineOutputInfo)
{
	CLASE_THIS_POITER(CLayoutVK, pltObj)
	PenPosition penPositionInWhere = PenPositionInNothing;
	GBINT hittedIndex = -1;

	// 候选栏不处理Move消息
	if (buttonStatus == ButtonStatusMove
		|| (pltBase->penDownPostion != PenPositionInCandidate && buttonStatus == ButtonStatusUp)) // 从其他区域挪过来
	{
		return PenPositionInNothing;
	}
	
	if ((hittedIndex = PT_TouchRegionHitTest(&pltThis->candidateTouchRegion, hitPoint)) != -1) // 如果点中了汉字
	{
		GBUINT8 tempAnsiBuf[50];

		pltBase->buttonStatus = buttonStatus; // 记录按钮状态
		penPositionInWhere = PenPositionInCandidate;
		// 记录点中的位置
		pltBase->penInPostion = PenPositionInCandidate;

		// 记录按下时的Layout按键值(只有当按下和弹起键值一致时才执行按键操作)
		if (buttonStatus == ButtonStatusDown)
		{
			pltBase->penDownPostion = PenPositionInCandidate;
			pltBase->pressLayoutKeyValue = GBEVENT_SELECT_CANDIDATE;
		}

		// 记录点中的候选索引值
		pltBase->currentLayoutKeyValue = GBEVENT_SELECT_CANDIDATE;
		pltBase->engineEventParam = hittedIndex;
		pltBase->currentCandidateIndex = hittedIndex;
		// 打印Log信息
		PT_PrintLogInfo(GBLOG_LAYOUTINFO, ("****CLayoutVK__PenPositionIsInCandidate() Pen position in candidate[SecondLayer]\n"));
		//PT_UnicodeToAnsi((GBUINT16*)pEngineOutputInfo->ppCandidates[hittedIndex],(GBUINT8*)tempAnsiBuf);
		PT_wcstombs((GBPACHAR)tempAnsiBuf, (GBPCWCHAR)pEngineOutputInfo->ppCandidates[hittedIndex], sizeof(tempAnsiBuf));
		PT_PrintLogInfo(GBLOG_LAYOUTINFO, ("****CLayoutVK__PenPositionIsInCandidate() candidate is %s[%d]\n",tempAnsiBuf,hittedIndex));
		return penPositionInWhere;
	}
	else
	{
		if (pltBase->pressLayoutKeyValue == GBEVENT_SELECT_CANDIDATE)
		{
			// 记录按钮状态
			pltBase->buttonStatus = buttonStatus;
			// 记录点中的位置
			penPositionInWhere = pltBase->penInPostion = PenPositionLeave;			
			// 记录点中的候选索引值
			pltBase->pressLayoutKeyValue = pltBase->currentLayoutKeyValue = GBEvent_None;
			// 默认候选索引
			pltBase->currentCandidateIndex = 0;
		}
	}

	return penPositionInWhere;
}

/*!
 * \brief 判断触摸笔是否点中了拼音翻页按钮
 * \param pltObj 
 * \param hitPoint 
 * \param buttonStatus 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-10 12:48:49
*/
static PenPosition CLayoutVK__PenPositionIsInSyllablePageButton(GBLPVOID pltObj, 
																GBIMEPoint hitPoint, 
																ButtonStatusEnum buttonStatus)
{
	CLASE_THIS_POITER(CLayoutVK, pltObj)

	PenPosition penPositionInWhere = PenPositionInNothing;

	// 候选栏不处理Move消息
	if (buttonStatus == ButtonStatusMove)
	{
		return PenPositionInNothing;
	}

	// 查看是否是音节翻页
	if (PT_PointIsInRect(hitPoint, pltThis->syllablePageDownOrUpRectArray[PAGE_DOWN_BUTTON_INDEX])) // 向下翻页
	{
		pltBase->buttonStatus = buttonStatus; // 记录按钮状态
		penPositionInWhere = PenPositionInSyllablePageDown;
		pltBase->penInPostion = PenPositionInSyllablePageDown;

		// 记录按下时的Layout按键值(只有当按下和弹起键值一致时才执行按键操作)
		if (buttonStatus == ButtonStatusDown)
		{
			pltBase->penDownPostion = PenPositionInSyllablePageDown;
			pltBase->pressLayoutKeyValue = GBKEY_NEXT_PAGE_SYLLABLE;
		}

		pltBase->currentLayoutKeyValue = GBKEY_NEXT_PAGE_SYLLABLE;
		pltBase->currentSyllableIndex = 0;
	}
	else if (PT_PointIsInRect(hitPoint, pltThis->syllablePageDownOrUpRectArray[PAGE_Up_BUTTON_INDEX])) // 向上翻页
	{
		pltBase->buttonStatus = buttonStatus; // 记录按钮状态
		penPositionInWhere = PenPositionInSyllablePageUp;
		pltBase->penInPostion = PenPositionInSyllablePageUp;

		// 记录按下时的Layout按键值(只有当按下和弹起键值一致时才执行按键操作)
		if (buttonStatus == ButtonStatusDown)
		{
			pltBase->penDownPostion = PenPositionInSyllablePageUp;
			pltBase->pressLayoutKeyValue = GBKEY_PREV_PAGE_SYLLABLE;
		}

		pltBase->currentLayoutKeyValue = GBKEY_PREV_PAGE_SYLLABLE;
		pltBase->currentSyllableIndex = 0;
	}
	else
	{
		if (pltBase->pressLayoutKeyValue == GBKEY_PREV_PAGE_SYLLABLE
			|| pltBase->pressLayoutKeyValue == GBKEY_NEXT_PAGE_SYLLABLE)
		{
			// 记录按钮状态
			pltBase->buttonStatus = buttonStatus;
			// 记录点中的位置
			penPositionInWhere = pltBase->penInPostion = PenPositionLeave;
			// 记录点中的候选索引值
			pltBase->pressLayoutKeyValue = pltBase->currentLayoutKeyValue = GBEvent_None;
		}
	}

	return penPositionInWhere;
}

/*!
 * \brief 判断触摸笔是否点中了候选翻页按键
 * \param pltObj 
 * \param hitPoint 
 * \param buttonStatus 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-10 12:53:59
*/
static PenPosition CLayoutVK__PenPositionIsInCandidatePageButton(GBLPVOID pltObj, 
																 GBIMEPoint hitPoint,  
																 ButtonStatusEnum buttonStatus)
{
	CLASE_THIS_POITER(CLayoutVK, pltObj)

	PenPosition penPositionInWhere = PenPositionInNothing;

	// 候选栏不处理Move消息
	if (buttonStatus == ButtonStatusMove)
	{
		return PenPositionInNothing;
	}

	// 查看是否是候选翻页
	if (PT_PointIsInRect(hitPoint, pltThis->candidatePageDownOrUpRectArray[PAGE_DOWN_BUTTON_INDEX])) // 向下翻页
	{
		pltBase->buttonStatus = buttonStatus; // 记录按钮状态
		penPositionInWhere = PenPositionInCandidatePageDown;
		pltBase->penInPostion = PenPositionInCandidatePageDown;

		// 记录按下时的Layout按键值(只有当按下和弹起键值一致时才执行按键操作)
		if (buttonStatus == ButtonStatusDown)
		{
			pltBase->penDownPostion = PenPositionInCandidatePageDown;
			pltBase->pressLayoutKeyValue = GBKEY_NEXT_PAGE_CANDIATE;
		}

		pltBase->currentLayoutKeyValue = GBKEY_NEXT_PAGE_CANDIATE;
	}
	else if (PT_PointIsInRect(hitPoint, pltThis->candidatePageDownOrUpRectArray[PAGE_Up_BUTTON_INDEX])) // 向上翻页
	{
		pltBase->buttonStatus = buttonStatus; // 记录按钮状态
		penPositionInWhere = PenPositionInCandidatePageUp;
		pltBase->penInPostion = PenPositionInCandidatePageUp;
	
		// 记录按下时的Layout按键值(只有当按下和弹起键值一致时才执行按键操作)
		if (buttonStatus == ButtonStatusDown)
		{
			pltBase->penDownPostion = PenPositionInCandidatePageUp;
			pltBase->pressLayoutKeyValue = GBKEY_PREV_PAGE_CANDIATE;
		}

		pltBase->currentLayoutKeyValue = GBKEY_PREV_PAGE_CANDIATE;
	}
	else
	{
		if (pltBase->pressLayoutKeyValue == GBKEY_PREV_PAGE_CANDIATE
			|| pltBase->pressLayoutKeyValue == GBKEY_NEXT_PAGE_CANDIATE)
		{
			// 记录按钮状态
			pltBase->buttonStatus = buttonStatus;
			// 记录点中的位置
			penPositionInWhere = pltBase->penInPostion = PenPositionLeave;
			// 记录点中的候选索引值
			pltBase->pressLayoutKeyValue = pltBase->currentLayoutKeyValue = GBEvent_None;
		}
	}

	return penPositionInWhere;
}

#endif // 国笔虚拟键盘