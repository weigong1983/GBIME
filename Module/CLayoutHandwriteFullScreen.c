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
* Purpose      :  ȫ����д����
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

// ���غ�������
GBIMELOCAL PenPosition ICLayoutHandwriteFullScreen_HitTest(GBLPVOID pltObj, 
											GBIMEPoint hitPoint, 
											ButtonStatusEnum buttonStatus, 
											PEngineOutputInfo pEngineOutputInfo);
// ��ѡ����
GBIMELOCAL void ICLayoutHandwriteFullScreen__UpdateCandidateRegion(GBLPVOID pltObj);
GBIMELOCAL PenPosition ICLayoutHandwriteFullScreen_PenPositionIsInCandidate(GBLPVOID pltObj, 
															  GBIMEPoint hitPoint, 
											                  ButtonStatusEnum buttonStatus);
// ��������
GBIMELOCAL void ICLayoutHandwriteFullScreen__UpdateAssociateRegion(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo);
GBIMELOCAL PenPosition ICLayoutHandwriteFullScreen_PenPositionIsInAssociateWord(GBLPVOID pltObj, 
																  GBIMEPoint hitPoint, 
																  ButtonStatusEnum buttonStatus,
																  PEngineOutputInfo pEngineOutputInfo);
// ����ʶ��Χ
GBIMELOCAL void ICLayoutHandwriteFullScreen__SetRecognizeRange(GBLPVOID pltObj);

// ��ʼ����ر���
GBIMELOCAL GBIMEReturn CLayoutHandwriteFullScreen__InitVariable(GBLPVOID pltObj);

// ��дMultiTap���Ź��ܶ�ʱ���������ʱ����Ӧ����
GBIMELOCAL void CLayoutHandwriteFullScreen_OnMultitapTimer(void);

// Layout����(������ʷ��¼�ָ�����״̬)
GBIMELOCAL GBIMEReturn CLayoutHandwriteFullScreen__Reset(GBLPVOID pltObj);

// ���ú�ѡ����ʼ��ʾλ��
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

	// �����Ӧ����ͼ����
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

	// ���׳�ʼ��
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
	
	// ����ͬ����״̬���
	GBHW_SetHomonymStatus(Global_GetHWEnineInstance(), GBFalse);
	// ����Ӣ��ʶ����
	GBHW_SetEnglishStatus(Global_GetHWEnineInstance(), GBFalse);
	// ��������ʶ����
	GBHW_SetNumberStatus(Global_GetHWEnineInstance(), GBFalse);
	// ���MultiTap����ѡ���ѡ״̬
	//pltHandwrite->m_ltParent.m_ltParent.bMultitapTimerStartFlag = GBFalse;
	//pltHandwrite->m_ltParent.m_ltParent.multitapReplaceCandLength = 0;

	return GBIME_OK;
}

GBIMELOCAL GBIMEReturn CLayoutHandwriteFullScreen__Paint(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo)
{
	CLASE_THIS_POITER(CLayoutHandwriteFullScreen, pltObj)

	if (pltRoot->m_hltView != NULL 
		&& pltRoot->m_hltView->m_pfcPaint != NULL
		&& Global_GetLayoutWinActiveStatus()) // ����״̬����ʾ
	{
		pltRoot->m_hltView->m_pfcPaint(pltObj, pEngineOutputInfo);
	}

	return GBIME_OK;
}

/*!
 *-brief �ú�����ɶ�ƽ̨ԭʼ��Ϣ��Ԥ����
 1. Layout��������, �����Ƿ���Ҫ������Ӧ�İ�����Ϣ���������洦��ı�ǲ���¼����ֵ(ʵ�ʴ�����Ϣʱ�õ�)
 2. ת���ɿ�ܶ�Ӧ��һ����Ϣ
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

	// ���ñ���
	pltThis->bUpdateCandidate = GBFalse;
	pltThis->bUpdateAssocWord = GBFalse;
	pltRoot->bSendEngineKeyEvent = GBFalse;
	pltRoot->m_refreshFlag = GBIME_LAYOUT_REFRESH_NONE;

	// ��ƽ̨�����¼�ת����IME�����Ҫ������¼�
	switch (nPlatformEvent)
	{
	case GBPEVT_PEN_DOWN:
		penPosition = ICLayoutHandwriteFullScreen_HitTest(pltObj, hitPoint, ButtonStatusDown, pEngineOutputInfo);
		if (penPosition != PenPositionInNothing && penPosition != PenPositionLeave)
		{
			pltThis->bDisableHandwriting = GBTrue; // ��ʱ��ֹ��д����
		}

		// ��������
		pltRoot->m_pfcLongPressKeyHandler(pltObj, pEngineOutputInfo);

		if(pltThis->bDisableHandwriting)
		{
			Global_FillEventGroup(GBIME_CT_KEY_DOWN_REFRESH, wParam, lParam);
		}
		else // �켣��������Ϣ����
		{
			// ���ֺ�ѡ��������дʶ��
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
		if(!pltThis->bDisableHandwriting) // �켣��������Ϣ����
		{
			// ���ֺ�ѡ��������дʶ��
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

		pltThis->bDisableHandwriting = GBFalse; // ����ʹ����д����
		penPosition = ICLayoutHandwriteFullScreen_HitTest(pltObj, hitPoint, ButtonStatusUp, pEngineOutputInfo);

		// ����������
		if ((ret = pltRoot->m_pfcLongPressKeyHandler(pltObj, pEngineOutputInfo)) != GBIME_IGNORE)
		{
			break; // ��������
		}

		// �����������ʱ���º͵����ֵһ�²�����Ӧ
		if (penPosition != PenPositionInNothing 
			&& pltRoot->pressLayoutKeyValue == pltRoot->currentLayoutKeyValue)
		{
			switch (pltRoot->currentLayoutKeyValue)
			{
			case GBKEY_VK_V_HANDWRITE_CANDIDATE_AREA: ///<������д��ѡ��[ѡ��֮�������ǰ��ѡ����������]
				{					
					GBU16 candNum = GBHW_GetCandCount(Global_GetHWEnineInstance());
					GBPCWCHAR pCand = GBHW_GetCandByIndex(Global_GetHWEnineInstance(), pltThis->nSelectedCandidateIndex);

					// �Ƿ����ֺ�ѡ��
					if (GBHW_GetNumberStatus(Global_GetHWEnineInstance()))
					{
						Global_FillEventGroup(GBIME_CT_KEY_UP_REFRESH, wParam, lParam);
						break;
					}					

					if (candNum > 0 && pCand != NULL)
					{
						// �滻������ѡ
						PT_LayerLock();
						GBInputBox_DeleteChar();
						GBInputBox_InsertChar(pCand[0]);
						/*pltRoot->multitapReplaceCandLength = GBInputBox_ReplaceHighlightString(pltRoot->multitapReplaceCandLength, pCand);
						PT_GUIStartTimer(GBIME_MULTITAP_TIMER_INTERVAL, pltRoot->m_pfcOnMultitapTimer);
						pltRoot->bMultitapTimerStartFlag = GBTrue;*/
						PT_LayerUnLock();
						PT_Invalidate(0, 0, PT_GetLcdWidth(), PT_GetLcdHeight());
						// ���»�ȡ�����
						Global_FillEventGroup(GBIME_CT_HANDWRITE_DRAW_ASSOCWORD, (GBINT)pCand, lParam);
					}
				}
				break;
			case GBKEY_VK_V_HANDWRITE_ASSOCIATE_AREA: ///<������д������
				ICLayoutHandwriteFullScreen__UpdateAssociateRegion(pltObj, pEngineOutputInfo);
				pltThis->bUpdateAssocWord = GBTrue;
				pltRoot->m_refreshFlag |= (GBIME_LAYOUT_REFRESH_CANDIDATE_LINE | GBIME_LAYOUT_REFRESH_KEYBOARD);
				Global_FillEventGroup(GBIME_CT_KEY_UP_REFRESH, wParam, lParam);
				break;
			case GBKEY_VK_V9_NUM: ///<����9������
			case GBKEY_VK_V_HANDWRITE_ALL: ///<������дȫ��
			case GBKEY_VK_v_HANDWRITE_ENG: ///<������дӢ��
			case GBKEY_VK_v_HANDWRITE_TONGYIN: ///<������дͬ��
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

			// Layout�л�����
			if ((ret = pltRoot->m_pfcSwitchHandler(pltObj)) != GBIME_IGNORE)
			{
				break;
			}
			else if ((ret = pltRoot->m_pfcFuncKeyHandler(pltObj, pEngineOutputInfo)) != GBIME_IGNORE) // ���ܼ�����
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
		if(!pltThis->bDisableHandwriting) // �켣��������Ϣ����
		{
			// ���ֺ�ѡ��������дʶ��
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
			// ��������Move��Ϣ������Layout��ť״̬
			penPosition = ICLayoutHandwriteFullScreen_HitTest(pltObj, hitPoint, ButtonStatusMove, pEngineOutputInfo);
			Global_FillEventGroup(GBIME_CT_MOVE, wParam, lParam);
		}		
		break;
	case GBPEVT_KEY_DOWN:
	case GBPEVT_KEY_UP:
		// ��������
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

	// ������Ϣ����
// 	if ((ret = pltRoot->m_pfcHandleCommonEvent(pltObj, pIMEEvent, pEngineOutputInfo)) != GBIME_IGNORE)
// 	{
// 		return ret;
// 	}

	switch(pIMEEvent->nEvent)
	{
	case GBIME_EVENT_LAYOUT_PEN_DOWN:	
	case GBIME_EVENT_LAYOUT_PEN_MOVE: // ȫ��ˢ��
		pltRoot->m_refreshFlag |= (GBIME_LAYOUT_REFRESH_CANDIDATE_LINE | GBIME_LAYOUT_REFRESH_KEYBOARD);
		break;
	case GBIME_EVENT_LAYOUT_PEN_UP:
		// �����ʷ����ֵ
		pltRoot->pressLayoutKeyValue = pltRoot->currentLayoutKeyValue = GBEvent_None;
		pltRoot->penDownPostion = pltRoot->penInPostion = PenPositionInNothing;
		pltRoot->m_refreshFlag |= (GBIME_LAYOUT_REFRESH_CANDIDATE_LINE | GBIME_LAYOUT_REFRESH_KEYBOARD);
		break;
	case GBIME_EVENT_KEY_DOWN: // ������ֱ��ˢ��	
	case GBIME_EVENT_KEY_UP:
	case GBIME_EVENT_KEY_LONG_PRESS:
		pltRoot->m_refreshFlag = (GBIME_LAYOUT_REFRESH_CANDIDATE_LINE | GBIME_LAYOUT_REFRESH_SYLLABLE_LINE);
		break;
	case GBIME_EVENT_HANDWRITE_DRAW_CANDIDATE: ///<������дʶ������ĺ�ѡ
		// ���ú�ѡ����ʼ��ʾλ��
		ICLayoutHandwriteFullScreen_SetCandidateDisplayStartPos(pltObj);

		// ������һ����ѡ
		if (GBHW_GetCandCount(Global_GetHWEnineInstance()) > 0)
		{
			GBPCWCHAR pCand = GBHW_GetCandByIndex(Global_GetHWEnineInstance(), 0);
			GBInputBox_InsertChar(pCand[0]);
			/*pltRoot->multitapReplaceCandLength = GBInputBox_ReplaceHighlightString(pltRoot->multitapReplaceCandLength, pCand);
			PT_GUIStartTimer(GBIME_MULTITAP_TIMER_INTERVAL, pltRoot->m_pfcOnMultitapTimer);
			pltRoot->bMultitapTimerStartFlag = GBTrue;*/
		}

		// ���ø�����ѡ������
		ICLayoutHandwriteFullScreen__UpdateCandidateRegion(pltObj);
		pltThis->bUpdateCandidate = GBTrue;
		pltRoot->m_refreshFlag |= (GBIME_LAYOUT_REFRESH_CANDIDATE_LINE | GBIME_LAYOUT_REFRESH_KEYBOARD);
		break;
	case GBIME_EVENT_HANDWRITE_DRAW_ASSOCWORD: ///<���������ַ���	
		// ���ø�����ѡ������
		ICLayoutHandwriteFullScreen__UpdateAssociateRegion(pltObj, pEngineOutputInfo);
		pltThis->bUpdateAssocWord = GBTrue;
		pltRoot->m_refreshFlag |= (GBIME_LAYOUT_REFRESH_CANDIDATE_LINE | GBIME_LAYOUT_REFRESH_KEYBOARD);
		break;
	case GBIME_EVENT_LAYOUT_ACTIVATE: ///<Layout ����
		pltRoot->m_ltData.ltCurLayer = FirstLayer;
		Global_SetLayoutWinActiveStatus(GBTrue);
		Global_SetLayoutWinHeight(pltRoot->m_pfcGetLayoutHeight(pltObj, pltRoot->m_ltData.pltcData->ltID));
		pltRoot->m_refreshFlag = GBIME_LAYOUT_REFRESH_ALL;
		break;
	case GBIME_EVENT_LAYOUT_DEACTIVATE: ///<Layout ע��
		pltRoot->m_ltData.ltCurLayer = FirstLayer;
		Global_SetLayoutWinActiveStatus(GBFalse);
		Global_SetLayoutWinHeight(0);
		// ���³�ʼ��Layout����״̬����
		pltRoot->m_pfcInitVariable(pltObj);
		break;
	case GBIME_EVENT_LAYOUT_HIDE: ///<Layout ����
		pltRoot->m_ltData.ltCurLayer = FirstLayer;
		Global_SetLayoutWinActiveStatus(GBFalse);
		pltRoot->m_pfcChangeHeight(pltObj, 0);
		// ���³�ʼ��Layout����״̬����
		pltRoot->m_pfcInitVariable(pltObj);
		break;
	case GBIME_EVENT_LAYOUT_SHOW: ///<Layout ��ʾ
		pltRoot->m_ltData.ltCurLayer = FirstLayer;
		Global_SetLayoutWinActiveStatus(GBTrue);
		pltRoot->m_pfcChangeHeight(pltObj, pltRoot->m_pfcGetLayoutHeight(pltObj, pltRoot->m_ltData.pltcData->ltID));
		pltRoot->m_refreshFlag = GBIME_LAYOUT_REFRESH_ALL;
		break;
	case GBIME_EVENT_LAYOUT_ENTER:
		//CLayoutHandwriteFullScreen__Reset(pltObj);
		pltRoot->m_ltData.ltCurLayer = FirstLayer;
		//Global_SetLayoutWinHeight(0); // �߶����㣬���Layout���л����ڸ߶�һ��û��ˢ��VK
		pltRoot->m_pfcChangeHeight(pltObj, pltRoot->m_pfcGetLayoutHeight(pltObj, pltRoot->m_ltData.pltcData->ltID)/* - 2 ����Ҫƫ��2�����ص�*/); // ��д����ͼû����ô�� - weizhiping-2010/10/30
		pltRoot->m_refreshFlag = GBIME_LAYOUT_REFRESH_ALL;
		//pltRoot->m_pfcInitVariable(pltObj);
		break;	
	case GBIME_EVENT_LAYOUT_EXIT:
		CLayoutHandwriteFullScreen__Reset(pltObj);
		Global_SetLayoutWinHeight(0); // �߶����㣬���Layout���л����ڸ߶�һ��û��ˢ��VK
		pltRoot->m_pfcInitVariable(pltObj);
		break;
	default:
		break;
	}

	return GBIME_OK;
}

/*!
 * \brief ������ԣ��жϻ�ȡ��ǰ�����ʰ���ʱ��λ�ã��ں�ѡ�ϣ��ڼ��̰�ť�ϣ������ط�
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

	// ���ú�ѡ����ʼ��ʾλ��
	ICLayoutHandwriteFullScreen_SetCandidateDisplayStartPos(pltObj);

	// ��һ����������ʱ�������ѯ����
	iButtonTotalCount = pltRoot->m_ltData.pltLayerData[FirstLayer].lyBtCount;

	// �жϴ������Ƿ����ڼ��̰�ť��
	penPosition = pltRoot->m_pfcPenPositionInKeyboardButton(pltObj, hitPoint, buttonStatus, pEngineOutputInfo);

	// �ɿ�ʱ�鿴�Ƿ�����˺�ѡ
	tempPenPosition = ICLayoutHandwriteFullScreen_PenPositionIsInCandidate(pltObj, hitPoint, buttonStatus);
	if (tempPenPosition == PenPositionInCandidate)
	{
		return tempPenPosition;
	}

	// �ɿ�ʱ�鿴�Ƿ������������
	tempPenPosition = ICLayoutHandwriteFullScreen_PenPositionIsInAssociateWord(pltObj, hitPoint, buttonStatus, pEngineOutputInfo);
	if (tempPenPosition == PenPositionInCandidate)
	{
		return tempPenPosition;
	}	
	
	return penPosition;
}

/*!
 * \brief ���ú�ѡ�����ֻ�ƴ����ѡ����ռ����������
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
	GBUINT16 iCandidateCount; // ��ѡ����
	GBIMEPoint startPos;
	GBRECT candButtonRect;
	
	// �Ƿ����ֺ�ѡ��
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

	// �����ر���
	pltThis->nSelectedCandidateIndex = 0;
	pltThis->nSelectedAssociateWordIndex = -1;

	// ���ø�����ѡ������
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
 * \brief �жϴ������Ƿ�����˺�ѡ
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
			// �Ƿ����ֺ�ѡ��
			if (GBHW_GetNumberStatus(Global_GetHWEnineInstance()))
			{
				pltRoot->m_pfcUpScreen(pltObj, pltThis->pNumberCandidates[hittedIndex]);
			}
			pltRoot->currentLayoutKeyValue = GBKEY_VK_V_HANDWRITE_CANDIDATE_AREA;
		}
		// ��¼���е�λ��
		pltRoot->buttonStatus = buttonStatus; // ��¼��ť״̬
		penPositionInWhere = pltRoot->penInPostion = PenPositionInCandidate;
		// ���к�ѡ����
		pltThis->nSelectedCandidateIndex = hittedIndex;
		pltThis->nSelectedAssociateWordIndex = -1;
		pltThis->bUpdateCandidate = GBTrue;
		return penPositionInWhere;
	}

	return penPositionInWhere;
}

/*!
 * \brief ������������ռ����������
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
	GBUINT16 iCandidateCount; // ��ѡ����
	GBIMEPoint startPos;
	GBRECT candidateButtonRect;
	GBRECT associateButtonRect;

	// �Ƿ����ֺ�ѡ��
	if (GBHW_GetNumberStatus(Global_GetHWEnineInstance()))
	{
		return ;
	}

	pltRoot->m_pfcGetBottonRectByKey(pltObj, LayoutIdSPQuanPingShouXiue, FirstLayer, GBKEY_VK_V_HANDWRITE_CANDIDATE_AREA, &candidateButtonRect);
	pltRoot->m_pfcGetBottonRectByKey(pltObj, LayoutIdSPQuanPingShouXiue, FirstLayer, GBKEY_VK_V_HANDWRITE_ASSOCIATE_AREA, &associateButtonRect);
	iButtonWidth = (GBUINT16)(PT_GetLcdWidth());//(GBUINT16)(pButtonConstData[1].btPos.right - pButtonConstData[1].btPos.left);
	iButtonHeight = (GBUINT16)(associateButtonRect.bottom - associateButtonRect.top);
	
	startPos.x = pltRoot->candidateDisplayStartPos.x;
	startPos.y = pltRoot->candidateDisplayStartPos.y + (candidateButtonRect.bottom - candidateButtonRect.top); // + ��ѡ��ť�߶�
	ppCandidates = pEngineOutputInfo->ppCandidates;
	iCandidateCount = pEngineOutputInfo->nCandNum;
	
	// ���ø�����ѡ������
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
 * \brief �жϴ������Ƿ��������д��������
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
		pltRoot->buttonStatus = buttonStatus; // ��¼��ť״̬
		penPositionInWhere = pltRoot->penInPostion = PenPositionInCandidate; // ��¼���е�λ��	
		pltThis->nSelectedAssociateWordIndex = hittedIndex; // ���к�ѡ����
		pltThis->bUpdateAssocWord = GBTrue;

		// ������ֺ�ѡ���ڶ��к�ѡʱ���õ�һ�к�ѡ����
		if (GBHW_GetNumberStatus(Global_GetHWEnineInstance()))
		{
			pltThis->nSelectedCandidateIndex = -1;
		}

		if (ButtonStatusUp == buttonStatus)
		{
			pltRoot->currentLayoutKeyValue = GBKEY_VK_V_HANDWRITE_ASSOCIATE_AREA;
		
			// �Ƿ����ֺ�ѡ��
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
				
				// ���MultiTap����ѡ���ѡ״̬
				//pltRoot->m_pfcOnMultitapTimer();
				pltRoot->m_pfcUpScreen(pltObj, pEngineOutputInfo->ppCandidates[hittedIndex]);

				// Ӣ��ʶ��ģʽ���޷��������룬������ǰ�����ѡ����
				if (!GBHW_GetEnglishStatus(Global_GetHWEnineInstance()))
				{
					GBEngine_GetAssocWord(Global_GetEnineInstance(), pEngineOutputInfo->ppCandidates[hittedIndex]);
				}
				// �����дʶ������ĺ�ѡ
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
 * \brief �û������En������ȫ����ťʱ������дʶ��Χ
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
	case GBKEY_VK_V9_NUM: ///<����9������
		hwType = GBIME_HANDWRITE_TYPE_NUMBER;
		// ȡ��һ��
		pltThis->hwRecognizeRangeFlag = ~pltThis->hwRecognizeRangeFlag;
		// ���������ֱ��
		pltThis->hwRecognizeRangeFlag = pltThis->hwRecognizeRangeFlag & GBIME_HW_RECOGNIZE_RANGE_NUMBER;		
		break;
	case GBKEY_VK_v_HANDWRITE_ENG: ///<������дӢ��
		hwType = GBIME_HANDWRITE_TYPE_LOWER_CASE_ENGLISH;
		// ȡ��һ��
		pltThis->hwRecognizeRangeFlag = ~pltThis->hwRecognizeRangeFlag;
		// ������Ӣ�ı��
		pltThis->hwRecognizeRangeFlag = pltThis->hwRecognizeRangeFlag & GBIME_HW_RECOGNIZE_RANGE_ENGLISH;	
		break;
	case GBKEY_VK_v_HANDWRITE_TONGYIN: ///<������дͬ��
		hwType = GBIME_HANDWRITE_TYPE_ALL;
		// ����All���
		status = pltThis->hwRecognizeRangeFlag & GBIME_HW_RECOGNIZE_RANGE_ALL;
		// ȡ��һ��
		pltThis->hwRecognizeRangeFlag = ~pltThis->hwRecognizeRangeFlag;
		// ������ͬ�����
		pltThis->hwRecognizeRangeFlag = pltThis->hwRecognizeRangeFlag & GBIME_HW_RECOGNIZE_RANGE_HOMONYM;
		// �ָ�All���
		pltThis->hwRecognizeRangeFlag |= status;
		break;
	case GBKEY_VK_V_HANDWRITE_ALL: ///<������дȫ��
		hwType = GBIME_HANDWRITE_TYPE_ALL;
		// ����ͬ�����
		status = pltThis->hwRecognizeRangeFlag & GBIME_HW_RECOGNIZE_RANGE_HOMONYM;
		// ȡ��һ��
		pltThis->hwRecognizeRangeFlag = ~pltThis->hwRecognizeRangeFlag;
		// ������ͬ�����
		pltThis->hwRecognizeRangeFlag = pltThis->hwRecognizeRangeFlag & GBIME_HW_RECOGNIZE_RANGE_ALL;
		// �ָ�ͬ�����
		pltThis->hwRecognizeRangeFlag |= status;
		break;
	default:
		hwType = GBIME_HANDWRITE_TYPE_DEFAULT;
		pltThis->hwRecognizeRangeFlag = GBIME_HW_RECOGNIZE_RANGE_NONE;
		//PT_Assert(0);
		break;
	}

	// ����ͬ����״̬���
	status = (GBBOOL) ((pltThis->hwRecognizeRangeFlag & GBIME_HW_RECOGNIZE_RANGE_HOMONYM) 
						== GBIME_HW_RECOGNIZE_RANGE_HOMONYM);
	GBHW_SetHomonymStatus(Global_GetHWEnineInstance(), status);

	// ����Ӣ��ʶ����
	status = (GBBOOL) ((pltThis->hwRecognizeRangeFlag & GBIME_HW_RECOGNIZE_RANGE_ENGLISH) 
						== GBIME_HW_RECOGNIZE_RANGE_ENGLISH);
	GBHW_SetEnglishStatus(Global_GetHWEnineInstance(), status);

	// ��������ʶ����
	status = (GBBOOL) ((pltThis->hwRecognizeRangeFlag & GBIME_HW_RECOGNIZE_RANGE_NUMBER) 
		== GBIME_HW_RECOGNIZE_RANGE_NUMBER);
	GBHW_SetNumberStatus(Global_GetHWEnineInstance(), status);

	// ����ģʽ������д
	if (GBHW_GetNumberStatus(Global_GetHWEnineInstance()))
	{		
		PT_DisableHandwriting();
	}
	else // ʹ����д
	{		
		PT_EnableHandwriting();
	}

	// ���ʶ��Χ��Ǳ���գ�����Ĭ�ϵ�hwType
	if (pltThis->hwRecognizeRangeFlag == GBIME_HW_RECOGNIZE_RANGE_NONE)
	{
		hwType = GBIME_HANDWRITE_TYPE_DEFAULT;
	}

	// ������д�����ʶ��Χ
	Global_FillEventGroup(GBIME_CT_HANDWRITE_SET_RECOGNIZE_RANGE, hwType, 0);

	// �������ã��������
	memset(&pltThis->candidateTouchRegion, 0, sizeof(GBIMETouchRegion));
	memset(&pltThis->associateTouchRegion, 0, sizeof(GBIMETouchRegion));
	GBHW_Reset(Global_GetHWEnineInstance());
	GBEngine_Reset(Global_GetEnineInstance());
}

/*!
 * \brief ���ܼ�������
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
	case GBKEY_RETURN: // ������ʱ��������س�����������
		// ���MultiTap����ѡ���ѡ״̬
		pltRoot->m_pfcOnMultitapTimer();
		tempStr[0] = GBKEY_RETURN;
		tempStr[1] = 0;
		pltRoot->m_pfcUpScreen(pltObj, tempStr);
		break;
	case GBKEY_SPACE: // ������ʱ��������ո񡿼�������ո�
		// ���MultiTap����ѡ���ѡ״̬
		pltRoot->m_pfcOnMultitapTimer();
		tempStr[0] = GBKEY_SPACE;
		tempStr[1] = 0;
		pltRoot->m_pfcUpScreen(pltObj, tempStr);
		break;
	case GBKEY_BACK:
		// ��������ģʽ��Ҫ�����ѡ�֣��������³�ʼ��[weizhiping/2010/12/04]
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
			// ���MultiTap����ѡ���ѡ״̬
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
 * \brief ������ֺ�ѡ
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
	GBUINT16 iCandidateCount; // ��ѡ����
	GBIMEPoint startPos;
	GBRECT candidateButtonRect;
	GBRECT associateButtonRect;
	
	pltRoot->m_pfcGetBottonRectByKey(pltObj, LayoutIdSPQuanPingShouXiue, FirstLayer, GBKEY_VK_V_HANDWRITE_CANDIDATE_AREA, &candidateButtonRect);
	pltRoot->m_pfcGetBottonRectByKey(pltObj, LayoutIdSPQuanPingShouXiue, FirstLayer, GBKEY_VK_V_HANDWRITE_ASSOCIATE_AREA, &associateButtonRect);

	// �����ر���
	pltThis->nSelectedCandidateIndex = 0;
	pltThis->nSelectedAssociateWordIndex = -1;

	// ��һ����ʾ1~5
	iButtonWidth = (GBUINT16)(PT_GetLcdWidth());
	iButtonHeight = (GBUINT16)(candidateButtonRect.bottom - candidateButtonRect.top);	
	startPos.x = pltRoot->candidateDisplayStartPos.x;
	startPos.y = pltRoot->candidateDisplayStartPos.y;
	ppCandidates = (GBPCWCHAR *)pltThis->pNumberCandidates;
	iCandidateCount = 5;
	// ���ø�����ѡ������
	PT_CreateTouchRegion(&pltThis->candidateTouchRegion, 
						 startPos,
						 (GBUINT16)(iButtonWidth),
						 (GBUINT16)(iButtonHeight),
						 ppCandidates,
						 iCandidateCount,
						 GBIME_CANDIDATE_FONT_SIZE,
						 PT_TEXT_ALIGN_CENTER);
	
	// �ڶ�����ʾ6~0
	iButtonWidth = (GBUINT16)(PT_GetLcdWidth());
	iButtonHeight = (GBUINT16)(associateButtonRect.bottom - associateButtonRect.top);	
	startPos.x = pltRoot->candidateDisplayStartPos.x;
	startPos.y = pltRoot->candidateDisplayStartPos.y + (candidateButtonRect.bottom - candidateButtonRect.top); // + ��ѡ��ť�߶�
	ppCandidates = (GBPCWCHAR *)&pltThis->pNumberCandidates[5];
	iCandidateCount = 5;	
	// ���ø�����ѡ������
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
 * \brief ��ʼ����ر���
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
	
	// ����ͬ����״̬���
	//GBHW_SetHomonymStatus(Global_GetHWEnineInstance(), GBFalse);
	// ����Ӣ��ʶ����
	//GBHW_SetEnglishStatus(Global_GetHWEnineInstance(), GBFalse);
	// ��������ʶ����
	//GBHW_SetNumberStatus(Global_GetHWEnineInstance(), GBFalse);
	// ���MultiTap����ѡ���ѡ״̬
	//pltRoot->bMultitapTimerStartFlag = GBFalse;
	//pltRoot->multitapReplaceCandLength = 0;
	return GBIME_OK;
}

/*!
 * \brief ��дMultiTap���Ź��ܶ�ʱ���������ʱ����Ӧ����
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
		// ����Layout��ʾ���״̬
		pltThis->bUpdateCandidate = GBFalse;
		pltThis->nSelectedCandidateIndex = -1;
		memset(&pltThis->candidateTouchRegion, 0, sizeof(GBIMETouchRegion));
		GBHW_Reset(Global_GetHWEnineInstance());
		// ���MultiTap״̬
		pltAbstractObj->bMultitapTimerStartFlag = GBFalse;
		pltAbstractObj->multitapReplaceCandLength = 0;
		PT_GUICancelTimer(pltAbstractObj->m_pfcOnMultitapTimer);
		GBInputBox_MultitapInputConfirm();
	}
}

/*!
 * \brief Layout����(������ʷ��¼�ָ�����״̬)
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
	
	// ��������ģʽ��Ҫ�����ѡ�֣��������³�ʼ��[weizhiping/2010/12/04]
	if (GBHW_GetNumberStatus(Global_GetHWEnineInstance()))
	{
		PT_DisableHandwriting(); // ��ֹ��д
	}
	else
	{		
		CLayoutHandwriteFullScreen__InitVariable(pltObj);		
		PT_EnableHandwriting(); // ʹ����д
	}

	// ������ʷ״̬����������д�����ʶ��Χ
	if (GBHW_GetEnglishStatus(Global_GetHWEnineInstance())) // Ӣ��
	{
		hwType = GBIME_HANDWRITE_TYPE_LOWER_CASE_ENGLISH;
	}
	else if (GBHW_GetNumberStatus(Global_GetHWEnineInstance())) // ����
	{
		hwType = GBIME_HANDWRITE_TYPE_NUMBER;
	}
	else
	{
		hwType = GBIME_HANDWRITE_TYPE_ALL;
	}

	// ������д�����ʶ��Χ
	GBHW_ConvertHandwriteTypeToRange(hwType, &dwRange);
	GBHW_SetRange(Global_GetHWEnineInstance(), dwRange);
	return GBIME_OK;
}

/*!
 * \brief ���ú�ѡ����ʼ��ʾλ��
 * \param pltObj 
 * \return 
 * \note ��д��̧��ʶ�����ѡ��Ҫ����һ�Σ�������ʾλ�ô���
 * \example
 * \author weizhiping
 * \date 2010-12-10 14:57:48
*/
GBIMELOCAL void ICLayoutHandwriteFullScreen_SetCandidateDisplayStartPos(GBLPVOID pltObj)
{
	CLASE_THIS_POITER(CLayoutHandwriteFullScreen, pltObj)
	GBUINT16 iKeyboardHeight = 0;
	GBIMEPoint winPos;

	// ȡ�ü�����ʾ�Ŀ�ʼλ��[���½�����]
	Global_GetWinPos(&winPos);
	// ���̸߶�
	iKeyboardHeight = pltRoot->m_pfcGetLayoutHeight(pltRoot, GET_LAYOUT_ID(pltRoot));
	// ��������ʼ��ʾλ�õ��������Ͻ�
	winPos.y -=  iKeyboardHeight;
	// ���ú�ѡ��ʾ�Ŀ�ʼλ��
	//pltRoot->candidateDisplayStartPos.x = winPos.x;
	//pltRoot->candidateDisplayStartPos.y = winPos.y;
	pltRoot->m_pfcSetCandidateDisplayStartPos(pltObj, winPos.x, winPos.y);
}

#endif // defined(__GBIME_HANDWRITE__)