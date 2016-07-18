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
* Purpose      :  �������Layout������
* Version      :  V1.0
* Last Changed List
* Name         |   date   |  Discription
****************************************************************************/
#if defined(__GBIME_VK__) // �����������
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
 * \brief ��Ա��������
 */
GBIMELOCAL GBIMEReturn CLayoutVK__Init(PCLayoutVK pltVK);
//GBIMELOCAL GBIMEReturn CLayoutVK__SetInterpunctionKey(GBLPVOID pltObj);
GBIMELOCAL GBBOOL CLayoutVK__ShouldSendMsgToEngine(GBLPVOID pltObj, ButtonStatusEnum buttonStatus);
GBIMELOCAL GBIMEReturn CLayoutVK__PreHandleEvent(GBLPVOID pltObj, GBINT nPlatformEvent, GBINT wParam, GBINT lParam, PEngineOutputInfo pEngineOutputInfo);
GBIMELOCAL GBIMEReturn CLayoutVK__PhysicFuncKeyHandler(GBLPVOID pltObj, GBINT nPlatformEvent, GBINT wParam, GBINT lParam, PEngineOutputInfo pEngineOutputInfo);
GBIMELOCAL void CLayoutVK__SetSyllableTouchRegion(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo); // ���ڴ�����
GBIMELOCAL void CLayoutVK__SetCandidateTouchRegion(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo); // ��ѡ������
GBIMELOCAL void CLayoutVK__SetSyllablePageButtonTouchRegion(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo); // ���ڷ�ҳ��ť������
GBIMELOCAL void CLayoutVK__SetCandidatePageButtonTouchRegion(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo);// ��ѡ��ҳ��ť������
GBIMELOCAL PenPosition CLayoutVK__PenPositionIsInPinYin(GBLPVOID pltObj, 
											 GBIMEPoint hitPoint, 
											 ButtonStatusEnum buttonStatus, 
											 PEngineOutputInfo pEngineOutputInfo); // ����Ƿ�ѡ������
GBIMELOCAL PenPosition CLayoutVK__PenPositionIsInCandidate(GBLPVOID pltObj, 
												GBIMEPoint hitPoint, 
														ButtonStatusEnum buttonStatus,
														PEngineOutputInfo pEngineOutputInfo); // ����Ƿ�ѡ�к�ѡ
GBIMELOCAL PenPosition CLayoutVK__PenPositionIsInSyllablePageButton(GBLPVOID pltObj, 
														 GBIMEPoint hitPoint, 
														 ButtonStatusEnum buttonStatus); // ����Ƿ�ѡ�����ڷ�ҳ��ť
GBIMELOCAL PenPosition CLayoutVK__PenPositionIsInCandidatePageButton(GBLPVOID pltObj, 
														  GBIMEPoint hitPoint,
														  ButtonStatusEnum buttonStatus); // ����Ƿ�ѡ�к�ѡ��ҳ��ť

/*!
 * \brief ���غ�������
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
	pltVK->m_pfcSetSyllableTouchRegion = CLayoutVK__SetSyllableTouchRegion; // �������ڴ�����
	pltVK->m_pfcSetCandidateTouchRegion = CLayoutVK__SetCandidateTouchRegion; // ���ú�ѡ������
	pltVK->m_pfcSetSyllablePageButtonTouchRegion = CLayoutVK__SetSyllablePageButtonTouchRegion; // �������ڷ�ҳ��ť������
	pltVK->m_pfcSetCandidatePageButtonTouchRegion = CLayoutVK__SetCandidatePageButtonTouchRegion;// ���ú�ѡ��ҳ��ť������
	pltVK->m_pfcPenPositionIsInPinYin = CLayoutVK__PenPositionIsInPinYin; // ����Ƿ�ѡ������
	pltVK->m_pfcPenPositionIsInCandidate = CLayoutVK__PenPositionIsInCandidate; // ����Ƿ�ѡ�к�ѡ
	pltVK->m_pfcPenPositionIsInSyllablePageButton = CLayoutVK__PenPositionIsInSyllablePageButton; // ����Ƿ�ѡ�����ڷ�ҳ��ť
	pltVK->m_pfcPenPositionIsInCandidatePageButton = CLayoutVK__PenPositionIsInCandidatePageButton; // ����Ƿ�ѡ�к�ѡ��ҳ��ť

	return GBIME_OK;
}

/*!
 * \brief �ж��Ƿ�Ӧ�������淢����Ϣ
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
	case LayoutIdSP9JPinYin:		// 9��ƴ��
	case LayoutIdSP9JStroke:		// 9���ʻ�
	case LayoutIdSP9JLowerEnglish:	// 9��Ӣ��Сд
	case LayoutIdSP9JUpEnglish:		// 9��Ӣ�Ĵ�д
	case LayoutIdSP9Jabc:			// 9��abc
	case LayoutIdSP9JABC:			// 9��ABC
	case LayoutIdSP26JPinYin:		// 26��ƴ��
	case LayoutIdSP26JLowerEnglish:	// 26��Ӣ��Сд
	case LayoutIdSP26JUpEnglish:	// 26��Ӣ�Ĵ�д
	case LayoutIdSP26Jabc:			// 26��abc	
	case LayoutIdSP26JABC:			// 26��ABC
		if (buttonStatus == ButtonStatusUp
			&& penPosition != PenPositionInNothing)
		{
			bShouldSendMsgToEngine = GBTrue;
		}
		break;
	case LayoutIdSP9JNumber:		// 9������
	case LayoutIdSP26JStroke:		// 26���ʻ�
	case LayoutIdSP26JNumber:		// 26������
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
 * \brief �ú�����ɶ�ƽ̨ԭʼ��Ϣ��Ԥ����
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

	// ��ƽ̨�������¼�ת����IME�����Ҫ������¼���	
	hitPoint.x = wParam;
	hitPoint.y = lParam;
	pltBase->bSendEngineKeyEvent = GBFalse;
	pltBase->engineKeyEvent = wParam; // ���������ҪLayout�����ɾ�������水��ֵ
	pltBase->engineEventParam = lParam; // ���������ҪLayout���ݾ��幦�ܰ�ť���н���

	switch(nPlatformEvent)
	{
	case GBPEVT_PEN_DOWN:
		pltBase->m_pfcSlidingPenDownHandler(pltObj, hitPoint);
		penPositionInWhere = pltBase->m_pfcHitTest(pltObj, hitPoint, ButtonStatusDown, pEngineOutputInfo);
		pltBase->m_pfcLongPressKeyHandler(pltObj, pEngineOutputInfo); // ��������
		Global_FillEventGroup(GBIME_CT_KEY_DOWN_REFRESH, wParam, lParam);
		break;
	case GBPEVT_PEN_UP:
		// ����ǻ����л�Layout��ֱ�ӽ�������
		if ((ret = pltBase->m_pfcSlidingPenUpHandler(pltObj, hitPoint)) != GBIME_IGNORE)
		{
			pltBase->m_pfcOnMultitapTimer();
			return ret;
		}

		penPositionInWhere = pltBase->m_pfcHitTest(pltObj, hitPoint, ButtonStatusUp, pEngineOutputInfo);
	
		// ��������
		//pltAbstractObj->m_pfcLongPressKeyHandler(pltObj, pEngineOutputInfo);

		// ʵ�ֵ����ʾ������Layout���
		if (0)//penPositionInWhere == PenPositionInNothing && Global_GetLayoutWinActiveStatus())
		{
			Global_FillEventGroup(GBIME_CT_HIDE, wParam, lParam);
			break; // ֱ���˳�switch
		}
		else if (/*penPositionInWhere != PenPositionInNothing && */!Global_GetLayoutWinActiveStatus()) // ��ǰLayout��������״̬����Ҫ�ȼ���
		{
			Global_FillEventGroup(GBIME_CT_SHOW, wParam, lParam);
			break; // ֱ���˳�switch
		}
		
		// ����������
		if ((ret = pltBase->m_pfcLongPressKeyHandler(pltObj, pEngineOutputInfo)) != GBIME_IGNORE)
		{
			break; // ��������
		}

		// �����������ʱ���º͵����ֵһ�²�����Ӧ
		if (penPositionInWhere != PenPositionInNothing 
			&& pltBase->pressLayoutKeyValue == pltBase->currentLayoutKeyValue)
		{
			// ͬ�����������ShiftCap״̬
			pltBase->m_pfcSyncEngineShiftCapStatus(pltObj);

			// ����Ƿ�ȡ��MultiTap���Ŷ�ʱ��(MultiTap״̬�°�������������)
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

			if ((ret = pltBase->m_pfcSwitchHandler(pltObj)) != GBIME_IGNORE) // Layout�л�����
			{
				break;
			}
			else if ((ret = pltBase->m_pfcFuncKeyHandler(pltObj, pEngineOutputInfo)) != GBIME_IGNORE) // ���ܼ�����
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
		// ��������Move��Ϣ������Layout��ť״̬
		penPositionInWhere = pltBase->m_pfcHitTest(pltObj, hitPoint, ButtonStatusMove, pEngineOutputInfo);
		Global_FillEventGroup(GBIME_CT_MOVE, wParam, lParam);	
		break;
	case GBPEVT_KEY_DOWN:
	case GBPEVT_KEY_UP:
		// ��������
		pltBase->m_pfcPhysicFuncKeyHandler(pltObj, nPlatformEvent, wParam, lParam, pEngineOutputInfo);
		break;
	default:
	    break;
	}

	return GBIME_OK;
}

/*!
 * \brief �����ܰ���������
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
	
	pltBase->buttonStatus = ButtonStatusUp; // ��¼��ť״̬
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
		// ת��Ϊ��Ӧ��Layout���ܰ�ť��Ϣ
		pltBase->pressLayoutKeyValue = pltBase->currentLayoutKeyValue = GBKEY_SPACE;
		// ���´�������
		pltThis->m_pfcSetTouchRegion(pltObj, pEngineOutputInfo);
				
		pltBase->bSendEngineKeyEvent = GBTrue;
		pltBase->engineKeyEvent = GBEVENT_SELECT_CANDIDATE;
		pltBase->engineEventParam = pltBase->currentCandidateIndex;
		pltBase->bUpToInputKeySequence = GBTrue;
		Global_FillEventGroup(GBIME_CT_NORMAL_KEY_UP, 0, 0);
		break;
	case GBKEY_BACK:
		// ת��Ϊ��Ӧ��Layout���ܰ�ť��Ϣ
		pltBase->pressLayoutKeyValue = pltBase->currentLayoutKeyValue = GBKEY_BACK;
		// ���´�������
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
 * \brief ����ƴ�����ڴ�����
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

	// ������ѡ������ʾ�����ֲ�Ʒ���һ��
	if (iCandidateCount == 1)
	{
		textAlign = PT_TEXT_ALIGN_LEFT;
	}
	PT_CreateTouchRegion(&pltThis->syllableTouchRegion,			// ����������
						 pltBase->syllableDisplayStartPos,		// ��������ʾ�Ŀ�ʼλ��
						 (GBUINT16)GBIME_SYLLABLE_LINE_WIDTH,	// ���������
						 (GBUINT16)GBIME_SYLLABLE_LINE_HEIGHT,	// �������߶�
						 ppCandidates,							// ��������
						 iCandidateCount,						// ���ڸ���
						 GBIME_CANDIDATE_FONT_SIZE,				// ����
						 textAlign);							// �ı�����
}

/*!
 * \brief ���ú�ѡ������
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

	PT_CreateTouchRegion(&pltThis->candidateTouchRegion,		// ��ѡ���ִ�������
						 pltBase->candidateDisplayStartPos,		// ��ѡ������ʾ�Ŀ�ʼλ��
						 (GBUINT16)GBIME_CANDIDATE_LINE_WIDTH,	// ��ѡ�����
						 (GBUINT16)GBIME_CANDIDATE_LINE_HEIGHT,	// ��ѡ���߶�
						 pEngineOutputInfo->ppCandidates,		// ��ѡ����
						 (GBUINT16)pEngineOutputInfo->nCandNum,	// ��ѡ����
						 GBIME_CANDIDATE_FONT_SIZE,				// ����
						 PT_TEXT_ALIGN_CENTER);					// �ı�����
}

/*!
 * \brief �������ڷ�ҳ��ť������
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
	
	// ȡ�ü���ʾ�Ŀ�ʼλ��[���½�����]
	Global_GetWinPos(&winPos);
	iKeyBoardHeight = pltBase->m_pfcGetLayoutHeight(pltObj, iCurrentLayoutId);
	// ������ʾ���Ͻ�Y����
	winPos.y -=  iKeyBoardHeight;
	// ȡ��ѡ��߶�
	iCandidateBoxHeight = pltBase->m_pfcGetCandidateBoxHeight(pltObj, iCurrentLayoutId, iCurrentLayerNumber);

	// �����ѡ[����]��ҳ����������
	pTargetRect = &pltThis->syllablePageDownOrUpRectArray[PAGE_DOWN_BUTTON_INDEX];
	pltBase->m_pfcGetBottonRectByKey(pltObj, iButtonLayoutId, iButtonLayerId, GBKEY_NEXT_PAGE_SYLLABLE, &buttonRect);
	iButtonHeight = buttonRect.bottom - buttonRect.top;
	pTargetRect->left = buttonRect.left + (GBUINT16)winPos.x;
	pTargetRect->top = (GBUINT16)winPos.y - iCandidateBoxHeight;
	pTargetRect->right = buttonRect.right + (GBUINT16)winPos.x;
	pTargetRect->bottom = (GBUINT16)winPos.y - (iCandidateBoxHeight - iButtonHeight);
	
	// �����ѡ[����]��ҳ����������
	pTargetRect = &pltThis->syllablePageDownOrUpRectArray[PAGE_Up_BUTTON_INDEX];
	pltBase->m_pfcGetBottonRectByKey(pltObj, iButtonLayoutId, iButtonLayerId, GBKEY_PREV_PAGE_SYLLABLE, &buttonRect);
	iButtonHeight = buttonRect.bottom - buttonRect.top;
	pTargetRect->left = buttonRect.left + (GBUINT16)winPos.x;
	pTargetRect->top = (GBUINT16)winPos.y - iCandidateBoxHeight;
	pTargetRect->right = buttonRect.right + (GBUINT16)winPos.x;
	pTargetRect->bottom = (GBUINT16)winPos.y - (iCandidateBoxHeight - iButtonHeight);
}

/*!
 * \brief ���ú�ѡ��ҳ��ť������
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
	
	// ȡ�ü���ʾ�Ŀ�ʼλ��[���½�����]
	Global_GetWinPos(&winPos);
	iKeyBoardHeight = pltBase->m_pfcGetLayoutHeight(pltObj, iCurrentLayoutId);
	// ������ʾ���Ͻ�Y����
	winPos.y -=  iKeyBoardHeight;
	// ȡ��ѡ��߶�
	iCandidateBoxHeight = pltBase->m_pfcGetCandidateBoxHeight(pltObj, iCurrentLayoutId, iCurrentLayerNumber);

	// �����ѡ[����]��ҳ����������
	pTargetRect = &pltThis->candidatePageDownOrUpRectArray[PAGE_DOWN_BUTTON_INDEX];
	pltBase->m_pfcGetBottonRectByKey(pltObj, iButtonLayoutId, iButtonLayerId, GBKEY_NEXT_PAGE_CANDIATE, &buttonRect);
	pTargetRect->left = buttonRect.left + (GBUINT16)winPos.x;
	pTargetRect->top = (GBUINT16)winPos.y - (iCandidateBoxHeight - buttonRect.top);
	pTargetRect->right = buttonRect.right + (GBUINT16)winPos.x;
	pTargetRect->bottom = (GBUINT16)winPos.y;

	// �����ѡ[����]��ҳ����������
	pTargetRect = &pltThis->candidatePageDownOrUpRectArray[PAGE_Up_BUTTON_INDEX];
	pltBase->m_pfcGetBottonRectByKey(pltObj, iButtonLayoutId, iButtonLayerId, GBKEY_PREV_PAGE_CANDIATE, &buttonRect);
	pTargetRect->left = buttonRect.left + (GBUINT16)winPos.x;
	pTargetRect->top = (GBUINT16)winPos.y - (iCandidateBoxHeight - buttonRect.top);
	pTargetRect->right = buttonRect.right + (GBUINT16)winPos.x;
	pTargetRect->bottom = (GBUINT16)winPos.y;
}

/*!
 * \brief �жϴ������Ƿ������ƴ��
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

	// ��ѡ��������Move��Ϣ
	if (buttonStatus == ButtonStatusMove
		|| (pltBase->penDownPostion != PenPositionInSyllable && buttonStatus == ButtonStatusUp)) // ����������Ų����
	{
		return PenPositionInNothing;
	}
	
	if ((hittedIndex = PT_TouchRegionHitTest(&pltThis->syllableTouchRegion, hitPoint)) != -1) // ���������ƴ��
	{
		// ��¼���е�λ��
		pltBase->penInPostion = PenPositionInSyllable;
		penPositionInWhere = PenPositionInSyllable;
		pltBase->buttonStatus = buttonStatus; // ��¼��ť״̬

		// ��¼����ʱ��Layout����ֵ(ֻ�е����º͵����ֵһ��ʱ��ִ�а�������)
		if (buttonStatus == ButtonStatusDown)
		{
			pltBase->penDownPostion = PenPositionInSyllable;
			pltBase->pressLayoutKeyValue = GBEVENT_SELECT_SYLLABLE_PAGED;
		}

		// ��¼���е�ƴ������ֵ
		pltBase->currentLayoutKeyValue = GBEVENT_SELECT_SYLLABLE_PAGED;
		pltBase->engineEventParam = hittedIndex;
		pltBase->currentSyllableIndex = hittedIndex;
		return penPositionInWhere;
	}
	else
	{
		if (pltBase->pressLayoutKeyValue == GBEVENT_SELECT_SYLLABLE_PAGED)
		{
			// ��¼��ť״̬
			pltBase->buttonStatus = buttonStatus;
			// ��¼���е�λ��
			penPositionInWhere = pltBase->penInPostion = PenPositionLeave;			
			// ��¼���еĺ�ѡ����ֵ
			pltBase->pressLayoutKeyValue = pltBase->currentLayoutKeyValue = GBEvent_None;
			// Ĭ����������
			pltBase->currentSyllableIndex = 0;
		}
	}

	return penPositionInWhere;
}

/*!
 * \brief �жϴ������Ƿ�����˺�ѡ
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

	// ��ѡ��������Move��Ϣ
	if (buttonStatus == ButtonStatusMove
		|| (pltBase->penDownPostion != PenPositionInCandidate && buttonStatus == ButtonStatusUp)) // ����������Ų����
	{
		return PenPositionInNothing;
	}
	
	if ((hittedIndex = PT_TouchRegionHitTest(&pltThis->candidateTouchRegion, hitPoint)) != -1) // ��������˺���
	{
		GBUINT8 tempAnsiBuf[50];

		pltBase->buttonStatus = buttonStatus; // ��¼��ť״̬
		penPositionInWhere = PenPositionInCandidate;
		// ��¼���е�λ��
		pltBase->penInPostion = PenPositionInCandidate;

		// ��¼����ʱ��Layout����ֵ(ֻ�е����º͵����ֵһ��ʱ��ִ�а�������)
		if (buttonStatus == ButtonStatusDown)
		{
			pltBase->penDownPostion = PenPositionInCandidate;
			pltBase->pressLayoutKeyValue = GBEVENT_SELECT_CANDIDATE;
		}

		// ��¼���еĺ�ѡ����ֵ
		pltBase->currentLayoutKeyValue = GBEVENT_SELECT_CANDIDATE;
		pltBase->engineEventParam = hittedIndex;
		pltBase->currentCandidateIndex = hittedIndex;
		// ��ӡLog��Ϣ
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
			// ��¼��ť״̬
			pltBase->buttonStatus = buttonStatus;
			// ��¼���е�λ��
			penPositionInWhere = pltBase->penInPostion = PenPositionLeave;			
			// ��¼���еĺ�ѡ����ֵ
			pltBase->pressLayoutKeyValue = pltBase->currentLayoutKeyValue = GBEvent_None;
			// Ĭ�Ϻ�ѡ����
			pltBase->currentCandidateIndex = 0;
		}
	}

	return penPositionInWhere;
}

/*!
 * \brief �жϴ������Ƿ������ƴ����ҳ��ť
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

	// ��ѡ��������Move��Ϣ
	if (buttonStatus == ButtonStatusMove)
	{
		return PenPositionInNothing;
	}

	// �鿴�Ƿ������ڷ�ҳ
	if (PT_PointIsInRect(hitPoint, pltThis->syllablePageDownOrUpRectArray[PAGE_DOWN_BUTTON_INDEX])) // ���·�ҳ
	{
		pltBase->buttonStatus = buttonStatus; // ��¼��ť״̬
		penPositionInWhere = PenPositionInSyllablePageDown;
		pltBase->penInPostion = PenPositionInSyllablePageDown;

		// ��¼����ʱ��Layout����ֵ(ֻ�е����º͵����ֵһ��ʱ��ִ�а�������)
		if (buttonStatus == ButtonStatusDown)
		{
			pltBase->penDownPostion = PenPositionInSyllablePageDown;
			pltBase->pressLayoutKeyValue = GBKEY_NEXT_PAGE_SYLLABLE;
		}

		pltBase->currentLayoutKeyValue = GBKEY_NEXT_PAGE_SYLLABLE;
		pltBase->currentSyllableIndex = 0;
	}
	else if (PT_PointIsInRect(hitPoint, pltThis->syllablePageDownOrUpRectArray[PAGE_Up_BUTTON_INDEX])) // ���Ϸ�ҳ
	{
		pltBase->buttonStatus = buttonStatus; // ��¼��ť״̬
		penPositionInWhere = PenPositionInSyllablePageUp;
		pltBase->penInPostion = PenPositionInSyllablePageUp;

		// ��¼����ʱ��Layout����ֵ(ֻ�е����º͵����ֵһ��ʱ��ִ�а�������)
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
			// ��¼��ť״̬
			pltBase->buttonStatus = buttonStatus;
			// ��¼���е�λ��
			penPositionInWhere = pltBase->penInPostion = PenPositionLeave;
			// ��¼���еĺ�ѡ����ֵ
			pltBase->pressLayoutKeyValue = pltBase->currentLayoutKeyValue = GBEvent_None;
		}
	}

	return penPositionInWhere;
}

/*!
 * \brief �жϴ������Ƿ�����˺�ѡ��ҳ����
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

	// ��ѡ��������Move��Ϣ
	if (buttonStatus == ButtonStatusMove)
	{
		return PenPositionInNothing;
	}

	// �鿴�Ƿ��Ǻ�ѡ��ҳ
	if (PT_PointIsInRect(hitPoint, pltThis->candidatePageDownOrUpRectArray[PAGE_DOWN_BUTTON_INDEX])) // ���·�ҳ
	{
		pltBase->buttonStatus = buttonStatus; // ��¼��ť״̬
		penPositionInWhere = PenPositionInCandidatePageDown;
		pltBase->penInPostion = PenPositionInCandidatePageDown;

		// ��¼����ʱ��Layout����ֵ(ֻ�е����º͵����ֵһ��ʱ��ִ�а�������)
		if (buttonStatus == ButtonStatusDown)
		{
			pltBase->penDownPostion = PenPositionInCandidatePageDown;
			pltBase->pressLayoutKeyValue = GBKEY_NEXT_PAGE_CANDIATE;
		}

		pltBase->currentLayoutKeyValue = GBKEY_NEXT_PAGE_CANDIATE;
	}
	else if (PT_PointIsInRect(hitPoint, pltThis->candidatePageDownOrUpRectArray[PAGE_Up_BUTTON_INDEX])) // ���Ϸ�ҳ
	{
		pltBase->buttonStatus = buttonStatus; // ��¼��ť״̬
		penPositionInWhere = PenPositionInCandidatePageUp;
		pltBase->penInPostion = PenPositionInCandidatePageUp;
	
		// ��¼����ʱ��Layout����ֵ(ֻ�е����º͵����ֵһ��ʱ��ִ�а�������)
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
			// ��¼��ť״̬
			pltBase->buttonStatus = buttonStatus;
			// ��¼���е�λ��
			penPositionInWhere = pltBase->penInPostion = PenPositionLeave;
			// ��¼���еĺ�ѡ����ֵ
			pltBase->pressLayoutKeyValue = pltBase->currentLayoutKeyValue = GBEvent_None;
		}
	}

	return penPositionInWhere;
}

#endif // �����������