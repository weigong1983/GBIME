/***************************************************************************
* File Name    :  CLayoutHandwriteFullScreen.c
* Corporation  :  Guobi Technology Ltd. GuangZhou China
* Copyright    :  Copyright Guobi Technology Ltd. And Subject to Confidentiality 
*                 Restrictions. This document is the confidential and proprietary 
*                 property of Guobi Technology Ltd.It is neither the document                  
*                 itself nor any part of it can be shared with any party  without 
*                 the written  permission. 
* Author       :  weizhiping
* Date         :  2010-08-07
* Purpose      :  全屏手写键盘
* Version      :  V1.0
* Last Changed List
* Name         |   date   |  Discription
****************************************************************************/
#include "CLayoutHandwriteFullScreen.h"
#include "CLayoutViewHandwriteFullScreen.h"
#include "PlatformAdapter.h"
#include "GB_Global.h"

#if defined(__GBIME_HANDWRITE__)

GBIMELOCAL GBIMEReturn CLayoutHandwriteFullScreen__Init(PCLayoutHandwriteFullScreen pltHandwrite);
GBIMELOCAL GBIMEReturn CLayoutHandwriteFullScreen__Paint(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo);
GBIMELOCAL GBIMEReturn CLayoutHandwriteFullScreen__PreHandleEvent(GBLPVOID pltObj, GBINT nPlatformEvent, GBINT wParam, GBINT lParam, PEngineOutputInfo pEngineOutputInfo);
GBIMELOCAL GBIMEReturn CLayoutHandwriteFullScreen__HandleEvent(GBLPVOID pltObj, PGBIMEEvent pIMEEvent, PEngineOutputInfo pEngineOutputInfo);
GBIMELOCAL GBIMEReturn ICLayoutHandwriteFullScreen__FuncKeyHandler(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo);
GBIMELOCAL void ICLayoutHandwriteFullScreen__FillNumberCandidates(GBLPVOID pltObj);

// 本地函数声明
GBIMELOCAL PenPosition ICLayoutHandwriteFullScreen_HitTest(GBLPVOID pltObj, 
											GBIMEPoint hitPoint, 
											ButtonStatusEnum buttonStatus, 
											PEngineOutputInfo pEngineOutputInfo);
// 候选区域
GBIMELOCAL void ICLayoutHandwriteFullScreen__UpdateCandidateRegion(GBLPVOID pltObj);
GBIMELOCAL PenPosition ICLayoutHandwriteFullScreen_PenPositionIsInCandidate(GBLPVOID pltObj, 
															  GBIMEPoint hitPoint, 
											                  ButtonStatusEnum buttonStatus);
// 联想区域
GBIMELOCAL void ICLayoutHandwriteFullScreen__UpdateAssociateRegion(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo);
GBIMELOCAL PenPosition ICLayoutHandwriteFullScreen_PenPositionIsInAssociateWord(GBLPVOID pltObj, 
																  GBIMEPoint hitPoint, 
																  ButtonStatusEnum buttonStatus,
																  PEngineOutputInfo pEngineOutputInfo);
// 设置识别范围
GBIMELOCAL void ICLayoutHandwriteFullScreen__SetRecognizeRange(GBLPVOID pltObj);

// 初始化相关变量
GBIMELOCAL GBIMEReturn CLayoutHandwriteFullScreen__InitVariable(GBLPVOID pltObj);

// 手写MultiTap符号功能定时器间隔到达时的响应函数
GBIMELOCAL void CLayoutHandwriteFullScreen_OnMultitapTimer(void);

// Layout重置(根据历史记录恢复引擎状态)
GBIMELOCAL GBIMEReturn CLayoutHandwriteFullScreen__Reset(GBLPVOID pltObj);

// 设置候选字起始显示位置
GBIMELOCAL void ICLayoutHandwriteFullScreen_SetCandidateDisplayStartPos(GBLPVOID pltObj);

CLayoutAbstract * CLayoutHandwriteFullScreen__Construct(PCLayoutHandwriteFullScreen pltHandwrite, 
											  GBLPCVOID pltUIData,
											  GBLPCVOID pltIMData,
											  GBLPCVOID pltConfigData)
{	
	CLayoutHandwrite__Construct(&pltHandwrite->m_ltParent,
								pltUIData,
								pltIMData,
								pltConfigData);
	CLayoutHandwriteFullScreen__Init(pltHandwrite);
	return (CLayoutAbstract *)pltHandwrite;
}

static const GBWCHAR numberCandidatesArray[DIGIT_NUM][2] = 
{
	{'1', '\0'},
	{'2', '\0'},
	{'3', '\0'},
	{'4', '\0'},
	{'5', '\0'},
	{'6', '\0'},
	{'7', '\0'},
	{'8', '\0'},
	{'9', '\0'},
	{'0', '\0'},
};

GBIMELOCAL GBIMEReturn CLayoutHandwriteFullScreen__Init(PCLayoutHandwriteFullScreen pltHandwrite)
{
	PCLayoutViewHandwriteFullScreen pLtView = NULL;
	GBU8 i;	

	// 构造对应的视图对象
	pLtView = (PCLayoutViewHandwriteFullScreen)PT_Malloc(sizeof(CLayoutViewHandwriteFullScreen));
	PT_Assert(pLtView != NULL);
	memset(pLtView, 0, sizeof(CLayoutViewHandwriteFullScreen));
	CLayoutViewHandwrite__Construct(&pLtView->m_ltParent);
	CLayoutViewHandwriteFullScreen__Init(pLtView);

	pltHandwrite->m_ltParent.m_ltParent.m_hltView = (PCLayoutViewAbstract)pLtView;
	pltHandwrite->m_ltParent.m_ltParent.m_pfcPaint = CLayoutHandwriteFullScreen__Paint;
	pltHandwrite->m_ltParent.m_ltParent.m_pfcPreHandleEvent = CLayoutHandwriteFullScreen__PreHandleEvent;
	pltHandwrite->m_ltParent.m_ltParent.m_pfcHandleEvent = CLayoutHandwriteFullScreen__HandleEvent;
	pltHandwrite->m_ltParent.m_ltParent.m_pfcFuncKeyHandler = ICLayoutHandwriteFullScreen__FuncKeyHandler;
	pltHandwrite->m_ltParent.m_ltParent.m_pfcOnMultitapTimer = CLayoutHandwriteFullScreen_OnMultitapTimer;

	// 彻底初始化
	//CLayoutHandwriteFullScreen__InitVariable(pltHandwrite);	
	pltHandwrite->bUpdateCandidate = GBFalse;
	pltHandwrite->bUpdateAssocWord = GBFalse;
	pltHandwrite->bDisableHandwriting = GBFalse;
	pltHandwrite->nSelectedCandidateIndex = 0;
	pltHandwrite->nSelectedAssociateWordIndex = -1;
	pltHandwrite->hwRecognizeRangeFlag = GBIME_HW_RECOGNIZE_RANGE_NONE;
	GBHW_SetRange(Global_GetHWEnineInstance(), GBHW_OPT_RANGE_DEFAULT);
	memset(&pltHandwrite->candidateTouchRegion, 0, sizeof(GBIMETouchRegion));
	memset(&pltHandwrite->associateTouchRegion, 0, sizeof(GBIMETouchRegion));
	GBHW_Reset(Global_GetHWEnineInstance());
	GBEngine_Reset(Global_GetEnineInstance());
	
	for (i = 0; i < DIGIT_NUM; i++)
	{
		pltHandwrite->pNumberCandidates[i] = numberCandidatesArray[i];
	}
	
	// 设置同音字状态标记
	GBHW_SetHomonymStatus(Global_GetHWEnineInstance(), GBFalse);
	// 设置英文识别标记
	GBHW_SetEnglishStatus(Global_GetHWEnineInstance(), GBFalse);
	// 设置数字识别标记
	GBHW_SetNumberStatus(Global_GetHWEnineInstance(), GBFalse);
	// 清除MultiTap高亮选择候选状态
	//pltHandwrite->m_ltParent.m_ltParent.bMultitapTimerStartFlag = GBFalse;
	//pltHandwrite->m_ltParent.m_ltParent.multitapReplaceCandLength = 0;

	return GBIME_OK;
}

GBIMELOCAL GBIMEReturn CLayoutHandwriteFullScreen__Paint(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo)
{
	CLASE_THIS_POITER(CLayoutHandwriteFullScreen, pltObj)

	if (pltRoot->m_hltView != NULL 
		&& pltRoot->m_hltView->m_pfcPaint != NULL
		&& Global_GetLayoutWinActiveStatus()) // 激活状态才显示
	{
		pltRoot->m_hltView->m_pfcPaint(pltObj, pEngineOutputInfo);
	}

	return GBIME_OK;
}

/*!
 *-brief 该函数完成对平台原始消息的预处理：
 1. Layout解析处理, 设置是否需要发送相应的按键消息给国笔引擎处理的标记并记录按键值(实际处理消息时用到)
 2. 转换成框架对应的一组消息
 */
GBIMELOCAL GBIMEReturn CLayoutHandwriteFullScreen__PreHandleEvent(GBLPVOID pltObj, GBINT nPlatformEvent, GBINT wParam, GBINT lParam, 
									         PEngineOutputInfo pEngineOutputInfo)
{
	CLASE_THIS_POITER(CLayoutHandwriteFullScreen, pltObj)

	PLayoutInfo pLayoutInfo = (PLayoutInfo)(&pltRoot->m_ltData); // Layout UI data point
	PenPosition penPosition = PenPositionInNothing;
	GBIMEPoint hitPoint;
	//PCLayoutAbstract pltPreObject = Global_GetPrevLayoutObject();
	GBIMEReturn ret;

	hitPoint.x = (GBINT16)wParam;
	hitPoint.y = (GBINT16)lParam;

	// 重置变量
	pltThis->bUpdateCandidate = GBFalse;
	pltThis->bUpdateAssocWord = GBFalse;
	pltRoot->bSendEngineKeyEvent = GBFalse;
	pltRoot->m_refreshFlag = GBIME_LAYOUT_REFRESH_NONE;

	// 将平台按键事件转换成IME框架需要处理的事件
	switch (nPlatformEvent)
	{
	case GBPEVT_PEN_DOWN:
		penPosition = ICLayoutHandwriteFullScreen_HitTest(pltObj, hitPoint, ButtonStatusDown, pEngineOutputInfo);
		if (penPosition != PenPositionInNothing && penPosition != PenPositionLeave)
		{
			pltThis->bDisableHandwriting = GBTrue; // 此时禁止手写处理
		}

		// 长按处理
		pltRoot->m_pfcLongPressKeyHandler(pltObj, pEngineOutputInfo);

		if(pltThis->bDisableHandwriting)
		{
			Global_FillEventGroup(GBIME_CT_KEY_DOWN_REFRESH, wParam, lParam);
		}
		else // 轨迹管理器消息处理
		{
			// 数字候选串不用手写识别
			if (GBHW_GetNumberStatus(Global_GetHWEnineInstance()))
			{
				break;
			}

// 			if (pltBase->pGBHWTrace->pfHWTraceHandleEvent(pltObj, GBPEVT_PEN_DOWN, hitPoint)) 
// 			{
// 				pltRoot->m_refreshFlag = GBIME_LAYOUT_REFRESH_ALL;
// 				Global_FillEventGroup(GBIME_CT_KEY_DOWN_REFRESH, wParam, lParam);
// 			}			
		}		
		break;
	case GBPEVT_PEN_UP:
		if(!pltThis->bDisableHandwriting) // 轨迹管理器消息处理
		{
			// 数字候选串不用手写识别
			if (GBHW_GetNumberStatus(Global_GetHWEnineInstance()))
			{
				break;
			}

// 			if (pltBase->pGBHWTrace->pfHWTraceHandleEvent(pltObj, GBPEVT_PEN_UP, hitPoint)) 
// 			{
// 				pltRoot->m_refreshFlag = GBIME_LAYOUT_REFRESH_ALL;
// 				Global_FillEventGroup(GBIME_CT_KEY_UP_REFRESH, wParam, lParam);
// 				break;
// 			}			
		}

		pltThis->bDisableHandwriting = GBFalse; // 重新使能手写处理
		penPosition = ICLayoutHandwriteFullScreen_HitTest(pltObj, hitPoint, ButtonStatusUp, pEngineOutputInfo);

		// 长按键处理
		if ((ret = pltRoot->m_pfcLongPressKeyHandler(pltObj, pEngineOutputInfo)) != GBIME_IGNORE)
		{
			break; // 结束处理
		}

		// 点击键盘区域时按下和弹起键值一致才作响应
		if (penPosition != PenPositionInNothing 
			&& pltRoot->pressLayoutKeyValue == pltRoot->currentLayoutKeyValue)
		{
			switch (pltRoot->currentLayoutKeyValue)
			{
			case GBKEY_VK_V_HANDWRITE_CANDIDATE_AREA: ///<竖屏手写候选区[选中之后高亮当前候选并更新联想]
				{					
					GBU16 candNum = GBHW_GetCandCount(Global_GetHWEnineInstance());
					GBPCWCHAR pCand = GBHW_GetCandByIndex(Global_GetHWEnineInstance(), pltThis->nSelectedCandidateIndex);

					// 是否数字候选串
					if (GBHW_GetNumberStatus(Global_GetHWEnineInstance()))
					{
						Global_FillEventGroup(GBIME_CT_KEY_UP_REFRESH, wParam, lParam);
						break;
					}					

					if (candNum > 0 && pCand != NULL)
					{
						// 替换上屏候选
						PT_LayerLock();
						GBInputBox_DeleteChar();
						GBInputBox_InsertChar(pCand[0]);
						/*pltRoot->multitapReplaceCandLength = GBInputBox_ReplaceHighlightString(pltRoot->multitapReplaceCandLength, pCand);
						PT_GUIStartTimer(GBIME_MULTITAP_TIMER_INTERVAL, pltRoot->m_pfcOnMultitapTimer);
						pltRoot->bMultitapTimerStartFlag = GBTrue;*/
						PT_LayerUnLock();
						PT_Invalidate(0, 0, PT_GetLcdWidth(), PT_GetLcdHeight());
						// 重新获取联想词
						Global_FillEventGroup(GBIME_CT_HANDWRITE_DRAW_ASSOCWORD, (GBINT)pCand, lParam);
					}
				}
				break;
			case GBKEY_VK_V_HANDWRITE_ASSOCIATE_AREA: ///<竖屏手写联想区
				ICLayoutHandwriteFullScreen__UpdateAssociateRegion(pltObj, pEngineOutputInfo);
				pltThis->bUpdateAssocWord = GBTrue;
				pltRoot->m_refreshFlag |= (GBIME_LAYOUT_REFRESH_CANDIDATE_LINE | GBIME_LAYOUT_REFRESH_KEYBOARD);
				Global_FillEventGroup(GBIME_CT_KEY_UP_REFRESH, wParam, lParam);
				break;
			case GBKEY_VK_V9_NUM: ///<竖屏9键数字
			case GBKEY_VK_V_HANDWRITE_ALL: ///<竖屏手写全部
			case GBKEY_VK_v_HANDWRITE_ENG: ///<竖屏手写英文
			case GBKEY_VK_v_HANDWRITE_TONGYIN: ///<竖屏手写同音
				ICLayoutHandwriteFullScreen__SetRecognizeRange(pltObj);
				if (GBHW_GetNumberStatus(Global_GetHWEnineInstance()))
				{
					ICLayoutHandwriteFullScreen__FillNumberCandidates(pltObj);
				}
				pltRoot->m_refreshFlag |= GBIME_LAYOUT_REFRESH_ALL;
				return GBIME_OK;
			default:
				Global_FillEventGroup(GBIME_CT_KEY_UP_REFRESH, wParam, lParam);
				break;
			}

			// Layout切换处理
			if ((ret = pltRoot->m_pfcSwitchHandler(pltObj)) != GBIME_IGNORE)
			{
				break;
			}
			else if ((ret = pltRoot->m_pfcFuncKeyHandler(pltObj, pEngineOutputInfo)) != GBIME_IGNORE) // 功能键处理
			{
				break;
			}
		}
		else
		{
			Global_FillEventGroup(GBIME_CT_KEY_UP_REFRESH, wParam, lParam);
		}
	    break;
	case GBPEVT_PEN_MOVE:
		if(!pltThis->bDisableHandwriting) // 轨迹管理器消息处理
		{
			// 数字候选串不用手写识别
			if (GBHW_GetNumberStatus(Global_GetHWEnineInstance()))
			{
				break;
			}

// 			if (pltBase->pGBHWTrace->pfHWTraceHandleEvent(pltObj, GBPEVT_PEN_MOVE, hitPoint)) 
// 			{
// 				pltRoot->m_refreshFlag = GBIME_LAYOUT_REFRESH_ALL;
// 				break;
// 			}			
		}
		else
		{
			// 处理触摸笔Move消息，更新Layout按钮状态
			penPosition = ICLayoutHandwriteFullScreen_HitTest(pltObj, hitPoint, ButtonStatusMove, pEngineOutputInfo);
			Global_FillEventGroup(GBIME_CT_MOVE, wParam, lParam);
		}		
		break;
	case GBPEVT_KEY_DOWN:
	case GBPEVT_KEY_UP:
		// 按键处理
		//pltRoot->m_pfcPhysicFuncKeyHandler(pltObj, nPlatformEvent, wParam, lParam, pEngineOutputInfo);
		break;
	default:
		break;
	}

	return GBIME_OK;
}

GBIMELOCAL GBIMEReturn CLayoutHandwriteFullScreen__HandleEvent(GBLPVOID pltObj, PGBIMEEvent pIMEEvent, PEngineOutputInfo pEngineOutputInfo)
{
	CLASE_THIS_POITER(CLayoutHandwriteFullScreen, pltObj)

	PT_Assert(pIMEEvent != NULL);
	PT_Assert(pEngineOutputInfo != NULL);

	//pltRoot->m_refreshFlag = GBIME_LAYOUT_REFRESH_NONE;

	// 公共消息处理
// 	if ((ret = pltRoot->m_pfcHandleCommonEvent(pltObj, pIMEEvent, pEngineOutputInfo)) != GBIME_IGNORE)
// 	{
// 		return ret;
// 	}

	switch(pIMEEvent->nEvent)
	{
	case GBIME_EVENT_LAYOUT_PEN_DOWN:	
	case GBIME_EVENT_LAYOUT_PEN_MOVE: // 全部刷新
		pltRoot->m_refreshFlag |= (GBIME_LAYOUT_REFRESH_CANDIDATE_LINE | GBIME_LAYOUT_REFRESH_KEYBOARD);
		break;
	case GBIME_EVENT_LAYOUT_PEN_UP:
		// 清除历史按键值
		pltRoot->pressLayoutKeyValue = pltRoot->currentLayoutKeyValue = GBEvent_None;
		pltRoot->penDownPostion = pltRoot->penInPostion = PenPositionInNothing;
		pltRoot->m_refreshFlag |= (GBIME_LAYOUT_REFRESH_CANDIDATE_LINE | GBIME_LAYOUT_REFRESH_KEYBOARD);
		break;
	case GBIME_EVENT_KEY_DOWN: // 物理按键直接刷新	
	case GBIME_EVENT_KEY_UP:
	case GBIME_EVENT_KEY_LONG_PRESS:
		pltRoot->m_refreshFlag = (GBIME_LAYOUT_REFRESH_CANDIDATE_LINE | GBIME_LAYOUT_REFRESH_SYLLABLE_LINE);
		break;
	case GBIME_EVENT_HANDWRITE_DRAW_CANDIDATE: ///<绘制手写识别出来的候选
		// 设置候选字起始显示位置
		ICLayoutHandwriteFullScreen_SetCandidateDisplayStartPos(pltObj);

		// 上屏第一个候选
		if (GBHW_GetCandCount(Global_GetHWEnineInstance()) > 0)
		{
			GBPCWCHAR pCand = GBHW_GetCandByIndex(Global_GetHWEnineInstance(), 0);
			GBInputBox_InsertChar(pCand[0]);
			/*pltRoot->multitapReplaceCandLength = GBInputBox_ReplaceHighlightString(pltRoot->multitapReplaceCandLength, pCand);
			PT_GUIStartTimer(GBIME_MULTITAP_TIMER_INTERVAL, pltRoot->m_pfcOnMultitapTimer);
			pltRoot->bMultitapTimerStartFlag = GBTrue;*/
		}

		// 设置各个候选的区域
		ICLayoutHandwriteFullScreen__UpdateCandidateRegion(pltObj);
		pltThis->bUpdateCandidate = GBTrue;
		pltRoot->m_refreshFlag |= (GBIME_LAYOUT_REFRESH_CANDIDATE_LINE | GBIME_LAYOUT_REFRESH_KEYBOARD);
		break;
	case GBIME_EVENT_HANDWRITE_DRAW_ASSOCWORD: ///<绘制联想字符串	
		// 设置各个候选的区域
		ICLayoutHandwriteFullScreen__UpdateAssociateRegion(pltObj, pEngineOutputInfo);
		pltThis->bUpdateAssocWord = GBTrue;
		pltRoot->m_refreshFlag |= (GBIME_LAYOUT_REFRESH_CANDIDATE_LINE | GBIME_LAYOUT_REFRESH_KEYBOARD);
		break;
	case GBIME_EVENT_LAYOUT_ACTIVATE: ///<Layout 激活
		pltRoot->m_ltData.ltCurLayer = FirstLayer;
		Global_SetLayoutWinActiveStatus(GBTrue);
		Global_SetLayoutWinHeight(pltRoot->m_pfcGetLayoutHeight(pltObj, pltRoot->m_ltData.pltcData->ltID));
		pltRoot->m_refreshFlag = GBIME_LAYOUT_REFRESH_ALL;
		break;
	case GBIME_EVENT_LAYOUT_DEACTIVATE: ///<Layout 注销
		pltRoot->m_ltData.ltCurLayer = FirstLayer;
		Global_SetLayoutWinActiveStatus(GBFalse);
		Global_SetLayoutWinHeight(0);
		// 重新初始化Layout对象状态变量
		pltRoot->m_pfcInitVariable(pltObj);
		break;
	case GBIME_EVENT_LAYOUT_HIDE: ///<Layout 隐藏
		pltRoot->m_ltData.ltCurLayer = FirstLayer;
		Global_SetLayoutWinActiveStatus(GBFalse);
		pltRoot->m_pfcChangeHeight(pltObj, 0);
		// 重新初始化Layout对象状态变量
		pltRoot->m_pfcInitVariable(pltObj);
		break;
	case GBIME_EVENT_LAYOUT_SHOW: ///<Layout 显示
		pltRoot->m_ltData.ltCurLayer = FirstLayer;
		Global_SetLayoutWinActiveStatus(GBTrue);
		pltRoot->m_pfcChangeHeight(pltObj, pltRoot->m_pfcGetLayoutHeight(pltObj, pltRoot->m_ltData.pltcData->ltID));
		pltRoot->m_refreshFlag = GBIME_LAYOUT_REFRESH_ALL;
		break;
	case GBIME_EVENT_LAYOUT_ENTER:
		//CLayoutHandwriteFullScreen__Reset(pltObj);
		pltRoot->m_ltData.ltCurLayer = FirstLayer;
		//Global_SetLayoutWinHeight(0); // 高度清零，解决Layout间切换由于高度一样没有刷新VK
		pltRoot->m_pfcChangeHeight(pltObj, pltRoot->m_pfcGetLayoutHeight(pltObj, pltRoot->m_ltData.pltcData->ltID)/* - 2 不需要偏移2个像素点*/); // 手写背景图没有那么高 - weizhiping-2010/10/30
		pltRoot->m_refreshFlag = GBIME_LAYOUT_REFRESH_ALL;
		//pltRoot->m_pfcInitVariable(pltObj);
		break;	
	case GBIME_EVENT_LAYOUT_EXIT:
		CLayoutHandwriteFullScreen__Reset(pltObj);
		Global_SetLayoutWinHeight(0); // 高度清零，解决Layout间切换由于高度一样没有刷新VK
		pltRoot->m_pfcInitVariable(pltObj);
		break;
	default:
		break;
	}

	return GBIME_OK;
}

/*!
 * \brief 点击测试，判断获取当前触摸笔按下时的位置：在候选上，在键盘按钮上，其他地方
 * \param pltObj 
 * \param hitPoint 
 * \param buttonStatus 
 * \param pEngineOutputInfo 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-8-19 11:07:19
*/
static PenPosition ICLayoutHandwriteFullScreen_HitTest(GBLPVOID pltObj, 
											GBIMEPoint hitPoint, 
											ButtonStatusEnum buttonStatus, 
											PEngineOutputInfo pEngineOutputInfo)
{
	CLASE_THIS_POITER(CLayoutHandwriteFullScreen, pltObj)
	PenPosition penPosition = PenPositionInNothing;
	PenPosition tempPenPosition = PenPositionInNothing;
	GBUINT16 iButtonTotalCount;

	// 设置候选字起始显示位置
	ICLayoutHandwriteFullScreen_SetCandidateDisplayStartPos(pltObj);

	// 第一，二或三层时，都会查询键盘
	iButtonTotalCount = pltRoot->m_ltData.pltLayerData[FirstLayer].lyBtCount;

	// 判断触摸笔是否落在键盘按钮上
	penPosition = pltRoot->m_pfcPenPositionInKeyboardButton(pltObj, hitPoint, buttonStatus, pEngineOutputInfo);

	// 松开时查看是否点中了候选
	tempPenPosition = ICLayoutHandwriteFullScreen_PenPositionIsInCandidate(pltObj, hitPoint, buttonStatus);
	if (tempPenPosition == PenPositionInCandidate)
	{
		return tempPenPosition;
	}

	// 松开时查看是否点中了联想字
	tempPenPosition = ICLayoutHandwriteFullScreen_PenPositionIsInAssociateWord(pltObj, hitPoint, buttonStatus, pEngineOutputInfo);
	if (tempPenPosition == PenPositionInCandidate)
	{
		return tempPenPosition;
	}	
	
	return penPosition;
}

/*!
 * \brief 设置候选（汉字或拼音候选）所占的区域坐标
 * \param pltObj 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-8-24 13:52:51
*/
static void ICLayoutHandwriteFullScreen__UpdateCandidateRegion(GBLPVOID pltObj)												
{
	CLASE_THIS_POITER(CLayoutHandwriteFullScreen, pltObj)
		
	GBUINT16 iButtonWidth;
	GBUINT16 iButtonHeight;
	GBPCWCHAR *ppCandidates;
	GBUINT16 iCandidateCount; // 候选个数
	GBIMEPoint startPos;
	GBRECT candButtonRect;
	
	// 是否数字候选串
	if (GBHW_GetNumberStatus(Global_GetHWEnineInstance()))
	{
		return ;
	}

	pltRoot->m_pfcGetBottonRectByKey(pltObj, LayoutIdSPQuanPingShouXiue, FirstLayer, GBKEY_VK_V_HANDWRITE_CANDIDATE_AREA, &candButtonRect);
	iButtonWidth = (GBUINT16)(PT_GetLcdWidth());//(GBUINT16)(pButtonConstData[0].btPos.right - pButtonConstData[0].btPos.left);
	iButtonHeight = (GBUINT16)(candButtonRect.bottom - candButtonRect.top);
	
	startPos.x = pltRoot->candidateDisplayStartPos.x;
	startPos.y = pltRoot->candidateDisplayStartPos.y;
	ppCandidates = GBHW_GetCandBuffer(Global_GetHWEnineInstance());
	iCandidateCount = GBHW_GetCandCount(Global_GetHWEnineInstance());	

	// 清除相关变量
	pltThis->nSelectedCandidateIndex = 0;
	pltThis->nSelectedAssociateWordIndex = -1;

	// 设置各个候选的区域
	PT_CreateTouchRegion(&pltThis->candidateTouchRegion, 
						 startPos,
						 (GBUINT16)(iButtonWidth),
						 (GBUINT16)(iButtonHeight),
						 ppCandidates,
						 iCandidateCount,
						 GBIME_CANDIDATE_FONT_SIZE,
						 PT_TEXT_ALIGN_CENTER);
}

/*!
 * \brief 判断触摸笔是否点中了候选
 * \param pltObj 
 * \param hitPoint 
 * \param buttonStatus 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-8-23 20:37:31
*/
static PenPosition ICLayoutHandwriteFullScreen_PenPositionIsInCandidate(GBLPVOID pltObj, GBIMEPoint hitPoint, ButtonStatusEnum buttonStatus)
{
	CLASE_THIS_POITER(CLayoutHandwriteFullScreen, pltObj)

	PenPosition penPositionInWhere = PenPositionInNothing;
	GBINT hittedIndex = -1;

	if ((hittedIndex = PT_TouchRegionHitTest(&pltThis->candidateTouchRegion, hitPoint)) != -1)
	{
		if (ButtonStatusUp == buttonStatus)
		{
			// 是否数字候选串
			if (GBHW_GetNumberStatus(Global_GetHWEnineInstance()))
			{
				pltRoot->m_pfcUpScreen(pltObj, pltThis->pNumberCandidates[hittedIndex]);
			}
			pltRoot->currentLayoutKeyValue = GBKEY_VK_V_HANDWRITE_CANDIDATE_AREA;
		}
		// 记录点中的位置
		pltRoot->buttonStatus = buttonStatus; // 记录按钮状态
		penPositionInWhere = pltRoot->penInPostion = PenPositionInCandidate;
		// 点中候选索引
		pltThis->nSelectedCandidateIndex = hittedIndex;
		pltThis->nSelectedAssociateWordIndex = -1;
		pltThis->bUpdateCandidate = GBTrue;
		return penPositionInWhere;
	}

	return penPositionInWhere;
}

/*!
 * \brief 设置联想字所占的区域坐标
 * \param pltObj 
 * \param startPos 
 * \param pEngineOutputInfo 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-8-19 11:04:59
*/
static void ICLayoutHandwriteFullScreen__UpdateAssociateRegion(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo)												
{
	CLASE_THIS_POITER(CLayoutHandwriteFullScreen, pltObj)
		
	GBUINT16 iButtonWidth;
	GBUINT16 iButtonHeight;
	GBPCWCHAR *ppCandidates;
	GBUINT16 iCandidateCount; // 候选个数
	GBIMEPoint startPos;
	GBRECT candidateButtonRect;
	GBRECT associateButtonRect;

	// 是否数字候选串
	if (GBHW_GetNumberStatus(Global_GetHWEnineInstance()))
	{
		return ;
	}

	pltRoot->m_pfcGetBottonRectByKey(pltObj, LayoutIdSPQuanPingShouXiue, FirstLayer, GBKEY_VK_V_HANDWRITE_CANDIDATE_AREA, &candidateButtonRect);
	pltRoot->m_pfcGetBottonRectByKey(pltObj, LayoutIdSPQuanPingShouXiue, FirstLayer, GBKEY_VK_V_HANDWRITE_ASSOCIATE_AREA, &associateButtonRect);
	iButtonWidth = (GBUINT16)(PT_GetLcdWidth());//(GBUINT16)(pButtonConstData[1].btPos.right - pButtonConstData[1].btPos.left);
	iButtonHeight = (GBUINT16)(associateButtonRect.bottom - associateButtonRect.top);
	
	startPos.x = pltRoot->candidateDisplayStartPos.x;
	startPos.y = pltRoot->candidateDisplayStartPos.y + (candidateButtonRect.bottom - candidateButtonRect.top); // + 候选按钮高度
	ppCandidates = pEngineOutputInfo->ppCandidates;
	iCandidateCount = pEngineOutputInfo->nCandNum;
	
	// 设置各个候选的区域
	PT_CreateTouchRegion(&pltThis->associateTouchRegion, 
						 startPos,
						 (GBUINT16)(iButtonWidth),
						 (GBUINT16)(iButtonHeight),
						 ppCandidates,
						 iCandidateCount,
						 GBIME_CANDIDATE_FONT_SIZE,
						 PT_TEXT_ALIGN_CENTER);
}

/*!
 * \brief 判断触摸笔是否点中了手写联想区域
 * \param pltObj 
 * \param hitPoint 
 * \param buttonStatus 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-8-23 20:36:52
*/
static PenPosition ICLayoutHandwriteFullScreen_PenPositionIsInAssociateWord(GBLPVOID pltObj, 
																  GBIMEPoint hitPoint,
																  ButtonStatusEnum buttonStatus,
																  PEngineOutputInfo pEngineOutputInfo)
{
	CLASE_THIS_POITER(CLayoutHandwriteFullScreen, pltObj)

	PenPosition penPositionInWhere = PenPositionInNothing;
	GBINT hittedIndex = -1;
	
	if ((hittedIndex = PT_TouchRegionHitTest(&pltThis->associateTouchRegion, hitPoint)) != -1)
	{
		pltRoot->buttonStatus = buttonStatus; // 记录按钮状态
		penPositionInWhere = pltRoot->penInPostion = PenPositionInCandidate; // 记录点中的位置	
		pltThis->nSelectedAssociateWordIndex = hittedIndex; // 点中候选索引
		pltThis->bUpdateAssocWord = GBTrue;

		// 点击数字候选串第二行候选时不让第一行候选反显
		if (GBHW_GetNumberStatus(Global_GetHWEnineInstance()))
		{
			pltThis->nSelectedCandidateIndex = -1;
		}

		if (ButtonStatusUp == buttonStatus)
		{
			pltRoot->currentLayoutKeyValue = GBKEY_VK_V_HANDWRITE_ASSOCIATE_AREA;
		
			// 是否数字候选串
			if (GBHW_GetNumberStatus(Global_GetHWEnineInstance()))
			{
				pltRoot->m_pfcUpScreen(pltObj, pltThis->pNumberCandidates[hittedIndex + 5]);
				pltThis->nSelectedAssociateWordIndex = -1;
				return penPositionInWhere;
			}

			if (pEngineOutputInfo->nCandNum > 0 
				&& hittedIndex >= 0 && hittedIndex < pEngineOutputInfo->nCandNum)
			{
				if (0 != GBHW_GetCandByIndex(Global_GetHWEnineInstance(), 0) && GBHW_GetEnglishStatus(Global_GetHWEnineInstance()))//modify by john 
				{
					GBInputBox_DeleteChar();
				}
				
				// 清除MultiTap高亮选择候选状态
				//pltRoot->m_pfcOnMultitapTimer();
				pltRoot->m_pfcUpScreen(pltObj, pEngineOutputInfo->ppCandidates[hittedIndex]);

				// 英文识别模式下无法连续联想，保留当前联想候选即可
				if (!GBHW_GetEnglishStatus(Global_GetHWEnineInstance()))
				{
					GBEngine_GetAssocWord(Global_GetEnineInstance(), pEngineOutputInfo->ppCandidates[hittedIndex]);
				}
				// 清除手写识别出来的候选
				memset(&pltThis->candidateTouchRegion, 0, sizeof(GBIMETouchRegion));
				GBHW_Reset(Global_GetHWEnineInstance());
				pltThis->nSelectedAssociateWordIndex = -1;		
			}
		}

		return penPositionInWhere;
	}
	
	return penPositionInWhere;
}

/*!
 * \brief 用户点击【En、音、全】按钮时设置手写识别范围
 * \param pltObj 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-8-19 16:54:33
*/
static void ICLayoutHandwriteFullScreen__SetRecognizeRange(GBLPVOID pltObj)
{
	CLASE_THIS_POITER(CLayoutHandwriteFullScreen, pltObj)
	GBBOOL status;

	GBIMEHandwriteTypeEnum hwType = GBIME_HANDWRITE_TYPE_NONE;

	switch (pltRoot->currentLayoutKeyValue)
	{
	case GBKEY_VK_V9_NUM: ///<竖屏9键数字
		hwType = GBIME_HANDWRITE_TYPE_NUMBER;
		// 取反一次
		pltThis->hwRecognizeRangeFlag = ~pltThis->hwRecognizeRangeFlag;
		// 仅保留数字标记
		pltThis->hwRecognizeRangeFlag = pltThis->hwRecognizeRangeFlag & GBIME_HW_RECOGNIZE_RANGE_NUMBER;		
		break;
	case GBKEY_VK_v_HANDWRITE_ENG: ///<竖屏手写英文
		hwType = GBIME_HANDWRITE_TYPE_LOWER_CASE_ENGLISH;
		// 取反一次
		pltThis->hwRecognizeRangeFlag = ~pltThis->hwRecognizeRangeFlag;
		// 仅保留英文标记
		pltThis->hwRecognizeRangeFlag = pltThis->hwRecognizeRangeFlag & GBIME_HW_RECOGNIZE_RANGE_ENGLISH;	
		break;
	case GBKEY_VK_v_HANDWRITE_TONGYIN: ///<竖屏手写同音
		hwType = GBIME_HANDWRITE_TYPE_ALL;
		// 备份All标记
		status = pltThis->hwRecognizeRangeFlag & GBIME_HW_RECOGNIZE_RANGE_ALL;
		// 取反一次
		pltThis->hwRecognizeRangeFlag = ~pltThis->hwRecognizeRangeFlag;
		// 仅保留同音标记
		pltThis->hwRecognizeRangeFlag = pltThis->hwRecognizeRangeFlag & GBIME_HW_RECOGNIZE_RANGE_HOMONYM;
		// 恢复All标记
		pltThis->hwRecognizeRangeFlag |= status;
		break;
	case GBKEY_VK_V_HANDWRITE_ALL: ///<竖屏手写全部
		hwType = GBIME_HANDWRITE_TYPE_ALL;
		// 备份同音标记
		status = pltThis->hwRecognizeRangeFlag & GBIME_HW_RECOGNIZE_RANGE_HOMONYM;
		// 取反一次
		pltThis->hwRecognizeRangeFlag = ~pltThis->hwRecognizeRangeFlag;
		// 仅保留同音标记
		pltThis->hwRecognizeRangeFlag = pltThis->hwRecognizeRangeFlag & GBIME_HW_RECOGNIZE_RANGE_ALL;
		// 恢复同音标记
		pltThis->hwRecognizeRangeFlag |= status;
		break;
	default:
		hwType = GBIME_HANDWRITE_TYPE_DEFAULT;
		pltThis->hwRecognizeRangeFlag = GBIME_HW_RECOGNIZE_RANGE_NONE;
		//PT_Assert(0);
		break;
	}

	// 设置同音字状态标记
	status = (GBBOOL) ((pltThis->hwRecognizeRangeFlag & GBIME_HW_RECOGNIZE_RANGE_HOMONYM) 
						== GBIME_HW_RECOGNIZE_RANGE_HOMONYM);
	GBHW_SetHomonymStatus(Global_GetHWEnineInstance(), status);

	// 设置英文识别标记
	status = (GBBOOL) ((pltThis->hwRecognizeRangeFlag & GBIME_HW_RECOGNIZE_RANGE_ENGLISH) 
						== GBIME_HW_RECOGNIZE_RANGE_ENGLISH);
	GBHW_SetEnglishStatus(Global_GetHWEnineInstance(), status);

	// 设置数字识别标记
	status = (GBBOOL) ((pltThis->hwRecognizeRangeFlag & GBIME_HW_RECOGNIZE_RANGE_NUMBER) 
		== GBIME_HW_RECOGNIZE_RANGE_NUMBER);
	GBHW_SetNumberStatus(Global_GetHWEnineInstance(), status);

	// 数字模式禁用手写
	if (GBHW_GetNumberStatus(Global_GetHWEnineInstance()))
	{		
		PT_DisableHandwriting();
	}
	else // 使能手写
	{		
		PT_EnableHandwriting();
	}

	// 如果识别范围标记被清空，设置默认的hwType
	if (pltThis->hwRecognizeRangeFlag == GBIME_HW_RECOGNIZE_RANGE_NONE)
	{
		hwType = GBIME_HANDWRITE_TYPE_DEFAULT;
	}

	// 设置手写引擎的识别范围
	Global_FillEventGroup(GBIME_CT_HANDWRITE_SET_RECOGNIZE_RANGE, hwType, 0);

	// 引擎重置，清除变量
	memset(&pltThis->candidateTouchRegion, 0, sizeof(GBIMETouchRegion));
	memset(&pltThis->associateTouchRegion, 0, sizeof(GBIMETouchRegion));
	GBHW_Reset(Global_GetHWEnineInstance());
	GBEngine_Reset(Global_GetEnineInstance());
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
static GBIMEReturn ICLayoutHandwriteFullScreen__FuncKeyHandler(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo)
{
	CLASE_THIS_POITER(CLayoutHandwriteFullScreen, pltObj)
	GBUINT16 tempStr[2];

	switch (pltRoot->currentLayoutKeyValue)
	{
	case GBKEY_RETURN: // 无输入时，点击【回车】键，换行
		// 清除MultiTap高亮选择候选状态
		pltRoot->m_pfcOnMultitapTimer();
		tempStr[0] = GBKEY_RETURN;
		tempStr[1] = 0;
		pltRoot->m_pfcUpScreen(pltObj, tempStr);
		break;
	case GBKEY_SPACE: // 无输入时，点击【空格】键，输入空格
		// 清除MultiTap高亮选择候选状态
		pltRoot->m_pfcOnMultitapTimer();
		tempStr[0] = GBKEY_SPACE;
		tempStr[1] = 0;
		pltRoot->m_pfcUpScreen(pltObj, tempStr);
		break;
	case GBKEY_BACK:
		// 数字输入模式需要保存候选字，不做重新初始化[weizhiping/2010/12/04]
		if (GBHW_GetNumberStatus(Global_GetHWEnineInstance()))
		{
			GBInputBox_DeleteChar();
			Global_FillEventGroup(GBIME_CT_KEY_UP_REFRESH, GBKEY_BACK, 0);
		}
		else
		{
			memset(&pltThis->candidateTouchRegion, 0, sizeof(GBIMETouchRegion));
			memset(&pltThis->associateTouchRegion, 0, sizeof(GBIMETouchRegion));
			GBHW_Reset(Global_GetHWEnineInstance());
			GBEngine_Reset(Global_GetEnineInstance());
			// 清除MultiTap高亮选择候选状态
			pltRoot->m_pfcOnMultitapTimer();

			if (pltThis->bUpdateCandidate || pltThis->bUpdateAssocWord)
			{
				GBHW_Reset(Global_GetHWEnineInstance());
				Global_FillEventGroup(GBIME_CT_HIDE, 0, 0);
			}		
			else
			{
				GBInputBox_DeleteChar();
				Global_FillEventGroup(GBIME_CT_KEY_UP_REFRESH, GBKEY_BACK, 0);
			}
		}
		break;	
	default:
		return GBIME_IGNORE;
		break;
	}

	return GBIME_OK;
}

/*!
 * \brief 填充数字候选
 * \param pltObj 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-8-24 13:52:51
*/
static void ICLayoutHandwriteFullScreen__FillNumberCandidates(GBLPVOID pltObj)							
{
	CLASE_THIS_POITER(CLayoutHandwriteFullScreen, pltObj)
		
	GBUINT16 iButtonWidth;
	GBUINT16 iButtonHeight;
	GBPCWCHAR *ppCandidates;
	GBUINT16 iCandidateCount; // 候选个数
	GBIMEPoint startPos;
	GBRECT candidateButtonRect;
	GBRECT associateButtonRect;
	
	pltRoot->m_pfcGetBottonRectByKey(pltObj, LayoutIdSPQuanPingShouXiue, FirstLayer, GBKEY_VK_V_HANDWRITE_CANDIDATE_AREA, &candidateButtonRect);
	pltRoot->m_pfcGetBottonRectByKey(pltObj, LayoutIdSPQuanPingShouXiue, FirstLayer, GBKEY_VK_V_HANDWRITE_ASSOCIATE_AREA, &associateButtonRect);

	// 清除相关变量
	pltThis->nSelectedCandidateIndex = 0;
	pltThis->nSelectedAssociateWordIndex = -1;

	// 第一行显示1~5
	iButtonWidth = (GBUINT16)(PT_GetLcdWidth());
	iButtonHeight = (GBUINT16)(candidateButtonRect.bottom - candidateButtonRect.top);	
	startPos.x = pltRoot->candidateDisplayStartPos.x;
	startPos.y = pltRoot->candidateDisplayStartPos.y;
	ppCandidates = (GBPCWCHAR *)pltThis->pNumberCandidates;
	iCandidateCount = 5;
	// 设置各个候选的区域
	PT_CreateTouchRegion(&pltThis->candidateTouchRegion, 
						 startPos,
						 (GBUINT16)(iButtonWidth),
						 (GBUINT16)(iButtonHeight),
						 ppCandidates,
						 iCandidateCount,
						 GBIME_CANDIDATE_FONT_SIZE,
						 PT_TEXT_ALIGN_CENTER);
	
	// 第二行显示6~0
	iButtonWidth = (GBUINT16)(PT_GetLcdWidth());
	iButtonHeight = (GBUINT16)(associateButtonRect.bottom - associateButtonRect.top);	
	startPos.x = pltRoot->candidateDisplayStartPos.x;
	startPos.y = pltRoot->candidateDisplayStartPos.y + (candidateButtonRect.bottom - candidateButtonRect.top); // + 候选按钮高度
	ppCandidates = (GBPCWCHAR *)&pltThis->pNumberCandidates[5];
	iCandidateCount = 5;	
	// 设置各个候选的区域
	PT_CreateTouchRegion(&pltThis->associateTouchRegion, 
						 startPos,
						 (GBUINT16)(iButtonWidth),
						 (GBUINT16)(iButtonHeight),
					 	 ppCandidates,
						 iCandidateCount,
						 GBIME_CANDIDATE_FONT_SIZE,
					 	 PT_TEXT_ALIGN_CENTER);
}

/*!
 * \brief 初始化相关变量
 * \param pltObj 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-10-11 11:49:50
*/
GBIMELOCAL GBIMEReturn CLayoutHandwriteFullScreen__InitVariable(GBLPVOID pltObj)
{
	CLASE_THIS_POITER(CLayoutHandwriteFullScreen, pltObj)
	GBU8 i;	
	
	pltThis->bUpdateCandidate = GBFalse;
	pltThis->bUpdateAssocWord = GBFalse;
	pltThis->bDisableHandwriting = GBFalse;
	pltThis->nSelectedCandidateIndex = 0;
	pltThis->nSelectedAssociateWordIndex = -1;
	//pltThis->hwRecognizeRangeFlag = GBIME_HW_RECOGNIZE_RANGE_NONE;
	//GBHW_SetRange(Global_GetHWEnineInstance(), GBHW_OPT_RANGE_DEFAULT);
	memset(&pltThis->candidateTouchRegion, 0, sizeof(GBIMETouchRegion));
	memset(&pltThis->associateTouchRegion, 0, sizeof(GBIMETouchRegion));
	GBHW_Reset(Global_GetHWEnineInstance());
	GBEngine_Reset(Global_GetEnineInstance());
	
	for (i = 0; i < DIGIT_NUM; i++)
	{
		pltThis->pNumberCandidates[i] = numberCandidatesArray[i];
	}
	
	// 设置同音字状态标记
	//GBHW_SetHomonymStatus(Global_GetHWEnineInstance(), GBFalse);
	// 设置英文识别标记
	//GBHW_SetEnglishStatus(Global_GetHWEnineInstance(), GBFalse);
	// 设置数字识别标记
	//GBHW_SetNumberStatus(Global_GetHWEnineInstance(), GBFalse);
	// 清除MultiTap高亮选择候选状态
	//pltRoot->bMultitapTimerStartFlag = GBFalse;
	//pltRoot->multitapReplaceCandLength = 0;
	return GBIME_OK;
}

/*!
 * \brief 手写MultiTap符号功能定时器间隔到达时的响应函数
 * \param void 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-10-20 11:32:42
*/
GBIMELOCAL void CLayoutHandwriteFullScreen_OnMultitapTimer(void)
{
	PCLayoutAbstract pltAbstractObj = (PCLayoutAbstract)Global_GetCurrentLayoutObject();
	PCLayoutHandwriteFullScreen pltThis = (PCLayoutHandwriteFullScreen)Global_GetCurrentLayoutObject();

	if (pltAbstractObj->bMultitapTimerStartFlag)
	{
		// 更新Layout显示相关状态
		pltThis->bUpdateCandidate = GBFalse;
		pltThis->nSelectedCandidateIndex = -1;
		memset(&pltThis->candidateTouchRegion, 0, sizeof(GBIMETouchRegion));
		GBHW_Reset(Global_GetHWEnineInstance());
		// 清除MultiTap状态
		pltAbstractObj->bMultitapTimerStartFlag = GBFalse;
		pltAbstractObj->multitapReplaceCandLength = 0;
		PT_GUICancelTimer(pltAbstractObj->m_pfcOnMultitapTimer);
		GBInputBox_MultitapInputConfirm();
	}
}

/*!
 * \brief Layout重置(根据历史记录恢复引擎状态)
 * \param pltObj 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-12-7 18:33:20
*/
GBIMELOCAL GBIMEReturn CLayoutHandwriteFullScreen__Reset(GBLPVOID pltObj)
{
	CLASE_THIS_POITER(CLayoutHandwriteFullScreen, pltObj)
	GBU32 dwRange;
	GBIMEHandwriteTypeEnum hwType = GBIME_HANDWRITE_TYPE_NONE;
	
	// 数字输入模式需要保存候选字，不做重新初始化[weizhiping/2010/12/04]
	if (GBHW_GetNumberStatus(Global_GetHWEnineInstance()))
	{
		PT_DisableHandwriting(); // 禁止手写
	}
	else
	{		
		CLayoutHandwriteFullScreen__InitVariable(pltObj);		
		PT_EnableHandwriting(); // 使能手写
	}

	// 根据历史状态重新设置手写引擎的识别范围
	if (GBHW_GetEnglishStatus(Global_GetHWEnineInstance())) // 英文
	{
		hwType = GBIME_HANDWRITE_TYPE_LOWER_CASE_ENGLISH;
	}
	else if (GBHW_GetNumberStatus(Global_GetHWEnineInstance())) // 数字
	{
		hwType = GBIME_HANDWRITE_TYPE_NUMBER;
	}
	else
	{
		hwType = GBIME_HANDWRITE_TYPE_ALL;
	}

	// 设置手写引擎的识别范围
	GBHW_ConvertHandwriteTypeToRange(hwType, &dwRange);
	GBHW_SetRange(Global_GetHWEnineInstance(), dwRange);
	return GBIME_OK;
}

/*!
 * \brief 设置候选字起始显示位置
 * \param pltObj 
 * \return 
 * \note 手写笔抬起识别完候选需要调用一次，否则显示位置错乱
 * \example
 * \author weizhiping
 * \date 2010-12-10 14:57:48
*/
GBIMELOCAL void ICLayoutHandwriteFullScreen_SetCandidateDisplayStartPos(GBLPVOID pltObj)
{
	CLASE_THIS_POITER(CLayoutHandwriteFullScreen, pltObj)
	GBUINT16 iKeyboardHeight = 0;
	GBIMEPoint winPos;

	// 取得键盘显示的开始位置[左下角坐标]
	Global_GetWinPos(&winPos);
	// 键盘高度
	iKeyboardHeight = pltRoot->m_pfcGetLayoutHeight(pltRoot, GET_LAYOUT_ID(pltRoot));
	// 将键盘起始显示位置调整到左上角
	winPos.y -=  iKeyboardHeight;
	// 设置候选显示的开始位置
	//pltRoot->candidateDisplayStartPos.x = winPos.x;
	//pltRoot->candidateDisplayStartPos.y = winPos.y;
	pltRoot->m_pfcSetCandidateDisplayStartPos(pltObj, winPos.x, winPos.y);
}

#endif // defined(__GBIME_HANDWRITE__)