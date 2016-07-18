/***************************************************************************
* File Name    :  CLayoutVK9MultiLang.c
* Corporation  :  Guobi Technology Ltd. GuangZhou China
* Copyright    :  Copyright Guobi Technology Ltd. And Subject to Confidentiality 
*                 Restrictions. This document is the confidential and proprietary 
*                 property of Guobi Technology Ltd.It is neither the document                  
*                 itself nor any part of it can be shared with any party  without 
*                 the written  permission. 
* Author       :  haj
* Date         :  2010-06-08
* Purpose      :  ����ż�������Layout
* Version      :  V1.0
* Last Changed List
* Name         |   date   |  Discription
****************************************************************************/
#if defined(__GBIME_VK__) // �����������

#include "CLayoutVK9MultiLang.h"
#include "CLayoutViewVK9MultiLang.h"

/*!
 * \brief ���ظ����Ա��������
 */
GBIMELOCAL GBIMEReturn CLayoutVK9MultiLang__Release(GBLPVOID pltObj, PLayoutInfo pltUIData);
GBIMELOCAL GBIMEReturn CLayoutVK9MultiLang__Init(PCLayoutVK9MultiLang pltVK9Alp);
GBIMELOCAL GBIMEReturn CLayoutVK9MultiLang__Paint(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo);
GBIMELOCAL GBIMEReturn CLayoutVK9MultiLang__HandleEvent(GBLPVOID pltObj, PGBIMEEvent pIMEEvent, PEngineOutputInfo pEngineOutputInfo);
GBIMELOCAL PenPosition CLayoutVK9MultiLang__HitTest(GBLPVOID pltObj, 
										 GBIMEPoint hitPoint, 
										 ButtonStatusEnum buttonStatus, 
										 PEngineOutputInfo pEngineOutputInfo);
/*!
 * \brief ���غ�������
 */
GBIMELOCAL void ICLayoutVK9MultiLang__UpdateLayoutInformation(GBLPVOID pltObj, 
												   ButtonStatusEnum buttonStatus,
												   PEngineOutputInfo pEngineOutputInfo);
GBIMELOCAL void ICLayoutVK9MultiLang__SetTouchRegion(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo);

/*!
 * \brief ����ʵ��
 */
CLayoutAbstract * CLayoutVK9MultiLang__Construct(PCLayoutVK9MultiLang pltVK, 
												 GBLPCVOID pltUIData,
												 GBLPCVOID pltIMData,
												 GBLPCVOID pltConfigData)
{
	CLayoutVK__Construct(&pltVK->m_ltParent,
		pltUIData,
		pltIMData,
		pltConfigData);
	CLayoutVK9MultiLang__Init(pltVK);

	return (CLayoutAbstract *)pltVK;
}

GBIMELOCAL GBIMEReturn CLayoutVK9MultiLang__Release(GBLPVOID pltObj, PLayoutInfo pltUIData)
{
	CLASE_THIS_POITER(CLayoutVK9MultiLang, pltObj)

	pltRoot->m_pfcReleaseBase(pltObj, pltUIData);
	return GBIME_OK;
}

GBIMELOCAL GBIMEReturn CLayoutVK9MultiLang__Init(PCLayoutVK9MultiLang pltVK)
{
	PCLayoutViewVK9MultiLang pLtView = NULL;

	// �����Ӧ����ͼ����
	pLtView = (PCLayoutViewVK9MultiLang)PT_Malloc(sizeof(CLayoutViewVK9MultiLang));
	PT_Assert(pLtView != NULL);
	memset(pLtView, 0, sizeof(CLayoutViewVK9MultiLang));
	CLayoutViewVK9MultiLang__Construct(pLtView);

	pltVK->m_ltParent.m_ltParent.m_hltView = (PCLayoutViewAbstract)pLtView;
	pltVK->m_ltParent.m_ltParent.m_pfcPaint = CLayoutVK9MultiLang__Paint;
	pltVK->m_ltParent.m_ltParent.m_pfcHitTest = CLayoutVK9MultiLang__HitTest;
	pltVK->m_ltParent.m_ltParent.m_pfcUpdateLayoutInformation = ICLayoutVK9MultiLang__UpdateLayoutInformation;
	pltVK->m_ltParent.m_pfcSetTouchRegion = ICLayoutVK9MultiLang__SetTouchRegion;
	pltVK->m_ltParent.m_ltParent.m_pfcHandleEvent = CLayoutVK9MultiLang__HandleEvent;
	pltVK->m_ltParent.m_ltParent.m_pfcRelease = CLayoutVK9MultiLang__Release;
	pltVK->m_ltParent.m_ltParent.currentSyllableIndex = 0;
	pltVK->m_ltParent.m_ltParent.currentCandidateIndex = 0;
	
	return GBIME_OK;
}

GBIMELOCAL GBIMEReturn CLayoutVK9MultiLang__Paint(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo)
{
	CLASE_THIS_POITER(CLayoutVK9MultiLang, pltObj)

	if (pltRoot->m_hltView != NULL 
		&& pltRoot->m_hltView->m_pfcPaint != NULL
		&& Global_GetLayoutWinActiveStatus()) // ����״̬����ʾ
	{
		pltRoot->m_hltView->m_pfcPaint(pltObj, pEngineOutputInfo);
	}

	return GBIME_OK;
}

GBIMELOCAL GBIMEReturn CLayoutVK9MultiLang__HandleEvent(GBLPVOID pltObj, PGBIMEEvent pIMEEvent, PEngineOutputInfo pEngineOutputInfo)
{
	CLASE_THIS_POITER(CLayoutVK9MultiLang, pltObj)
	GBIMEReturn ret;
	
	PT_Assert(pltObj != NULL);
	PT_Assert(pIMEEvent != NULL);
	PT_Assert(pEngineOutputInfo != NULL);

	pltRoot->m_refreshFlag = GBIME_LAYOUT_REFRESH_NONE;

	// Layout���ڷǼ���״̬�����ȼ�����ʾ���ܽ����¼�
	if (!Global_GetLayoutWinActiveStatus()
		&& !(pIMEEvent->nEvent == GBIME_EVENT_LAYOUT_ACTIVATE || pIMEEvent->nEvent == GBIME_EVENT_LAYOUT_SHOW))
	{
		return GBIME_IGNORE;
	}

	// ������Ϣ����
	if ((ret = pltRoot->m_pfcHandleCommonEvent(pltObj, pIMEEvent, pEngineOutputInfo)) != GBIME_IGNORE)
	{
		return ret;
	}

	// ��Ϣ�ַ�(���Layout��ť״̬���¡��������水���¼��������������Ϣ)
	switch (pIMEEvent->nEvent)
	{
	case GBIME_EVENT_LAYOUT_PEN_DOWN:
		ICLayoutVK9MultiLang__UpdateLayoutInformation(pltObj, ButtonStatusDown, pEngineOutputInfo);
		if (pltRoot->penInPostion != PenPositionInNothing)
		{
			pltRoot->m_refreshFlag |= (GBIME_LAYOUT_REFRESH_CANDIDATE_LINE | GBIME_LAYOUT_REFRESH_KEYBOARD);
		}
		break;
	case GBIME_EVENT_LAYOUT_PEN_UP:
		ICLayoutVK9MultiLang__UpdateLayoutInformation(pltObj, ButtonStatusUp, pEngineOutputInfo);
		if (pltRoot->penInPostion != PenPositionInNothing)
		{
			pltRoot->m_refreshFlag |= (GBIME_LAYOUT_REFRESH_CANDIDATE_LINE | GBIME_LAYOUT_REFRESH_KEYBOARD);
		}
		// �����ʷ����ֵ
		pltRoot->pressLayoutKeyValue = pltRoot->currentLayoutKeyValue = GBEvent_None;
		pltRoot->penDownPostion = pltRoot->penInPostion = PenPositionInNothing;
		break;
	case GBIME_EVENT_LAYOUT_PEN_MOVE:
		if (pltRoot->penInPostion != PenPositionInNothing)
		{
			pltRoot->m_refreshFlag = (GBIME_LAYOUT_REFRESH_CANDIDATE_LINE | GBIME_LAYOUT_REFRESH_KEYBOARD);
		}
		break;
	case GBIME_EVENT_KEY_DOWN: // ��������ֱ��ˢ��	
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
 * \brief ��ȡ��ǰ�����ʰ���ʱ��λ�ã��ں�ѡ�ϣ��ڼ��̰�ť�ϣ������ط�
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
static PenPosition CLayoutVK9MultiLang__HitTest(GBLPVOID pltObj, 
												GBIMEPoint hitPoint, 
												ButtonStatusEnum buttonStatus, 
												PEngineOutputInfo pEngineOutputInfo)
{
	CLASE_THIS_POITER(CLayoutVK9MultiLang, pltObj)

    GBUINT16 iCandidateBoxHeight = 0;
	GBUINT16 iKeyBoardHeight = 0;
	PenPosition penPositionInWhere = PenPositionInNothing;
	GBUINT16 iCurrentLayoutId;	

	//��ǰҪ��ʾ�Ĳ�
	GBUINT16 iCurrentLayerNumber;
	GBUINT16 iButtonTotalCount;
	GBIMEPoint winPos;
	PLayoutChgtLayer pLayoutChgtLayer;

	PT_PrintLogInfo(GBLOG_LAYOUTINFO, ("###################################################[x,y]=[%3d,%3d]\n", hitPoint.x, hitPoint.y));

	// ��ǰLayoutId
	iCurrentLayoutId = GET_LAYOUT_ID(pltRoot);
	// ��ȡ���̸߶�
	iKeyBoardHeight = pltRoot->m_pfcGetLayoutHeight(pltObj,iCurrentLayoutId);
	// ȡ�ü�����ʾ�Ŀ�ʼλ�á����½ǡ���������Ϊ�����Ͻǡ�
	Global_GetWinPos(&winPos);	
	winPos.y -=  iKeyBoardHeight;
	// ��ǰҪ��ʾ�Ĳ�
	iCurrentLayerNumber = GET_LAYOUT_CURRENT_LAYER(pltRoot);
	// ȡ��ѡ��߶�
	iCandidateBoxHeight = pltRoot->m_pfcGetCandidateBoxHeight(pltObj,iCurrentLayoutId,iCurrentLayerNumber);
	//�Ȳ�ڵز�
	pLayoutChgtLayer = (PLayoutChgtLayer)&(pltRoot->m_ltData.pltLayerData[FirstLayer]);

	// ��һ����������ʱ�������ѯ����
	iButtonTotalCount = pLayoutChgtLayer->lyBtCount;

	// Ĭ��Ϊ��Ч���λ��
	pltRoot->penInPostion = PenPositionInNothing;

	// �жϴ������Ƿ����ڼ��̰�ť��
	penPositionInWhere = pltRoot->m_pfcPenPositionInKeyboardButton(pltObj, hitPoint, buttonStatus, pEngineOutputInfo);

	// ���ݵ�ǰ������Layout���ⴥ����λ��
	switch (iCurrentLayerNumber)
	{
	case FirstLayer: // ��ǰ�ǵ�һ��ʱ
		// ����ƴ�ʻ���ʾ�Ŀ�ʼλ��
		//pltRoot->syllableDisplayStartPos.x = GBIME_SYLLABLE_DISPLAY_START_X;
		//pltRoot->syllableDisplayStartPos.y = winPos.y - iCandidateBoxHeight;
		pltRoot->m_pfcSetSyllableDisplayStartPos(pltObj, GBIME_SYLLABLE_DISPLAY_START_X, winPos.y - iCandidateBoxHeight);
		
		// ���ú�ѡ��ʾ�Ŀ�ʼλ��
		//pltRoot->candidateDisplayStartPos.x = GBIME_CANDIDATE_DISPLAY_START_X;
		//pltRoot->candidateDisplayStartPos.y = winPos.y - GBIME_CANDIDATE_LINE_HEIGHT;
		pltRoot->m_pfcSetCandidateDisplayStartPos(pltObj, GBIME_CANDIDATE_DISPLAY_START_X, winPos.y - GBIME_CANDIDATE_LINE_HEIGHT);
		return penPositionInWhere;
	case SecondLayer: // ��ǰ�ǵڶ���ʱ
		// �ڶ����а��м���ʱ
		if (penPositionInWhere == PenPositionInKeyboardButton
			&& pltRoot->pressLayoutKeyValue == pltRoot->currentLayoutKeyValue)
		{
			return penPositionInWhere;
		}
		
		// ���ú�ѡ��ʾ�Ŀ�ʼλ��
		//pltRoot->candidateDisplayStartPos.x = GBIME_CANDIDATE_DISPLAY_START_X;
		//pltRoot->candidateDisplayStartPos.y = winPos.y - GBIME_CANDIDATE_LINE_HEIGHT;
		pltRoot->m_pfcSetCandidateDisplayStartPos(pltObj, GBIME_CANDIDATE_DISPLAY_START_X, winPos.y - GBIME_CANDIDATE_LINE_HEIGHT);
		// �鿴�Ƿ�����˺�ѡ
		penPositionInWhere = pltBase->m_pfcPenPositionIsInCandidate(pltObj, hitPoint, buttonStatus, pEngineOutputInfo);
		if (penPositionInWhere == PenPositionInCandidate
			&& pltRoot->pressLayoutKeyValue == pltRoot->currentLayoutKeyValue)
		{
			return penPositionInWhere;
		}

		// �鿴�Ƿ�����˺�ѡ��ҳ��ť
		penPositionInWhere = pltBase->m_pfcPenPositionIsInCandidatePageButton(pltObj, hitPoint, buttonStatus);
		if ((penPositionInWhere == PenPositionInCandidatePageDown || penPositionInWhere == PenPositionInCandidatePageUp)
			&& pltRoot->pressLayoutKeyValue == pltRoot->currentLayoutKeyValue)
		{
			return penPositionInWhere;
		}
		break;
	case ThirdLayer: //��ǰ������ʱ		
		break;
	default:
		break;
	}

	return penPositionInWhere;
}

/*!
 * \brief ����ͼ����Ϣ(���Ʋ���л���֪ͨƽ̨���������߶�)
 * \param pltObj 
 * \param buttonStatus 
 * \param pEngineOutputInfo 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-8-17 16:49:58
*/
static void ICLayoutVK9MultiLang__UpdateLayoutInformation(GBLPVOID pltObj, ButtonStatusEnum buttonStatus, PEngineOutputInfo pEngineOutputInfo)
{
	CLASE_THIS_POITER(CLayoutVK9MultiLang, pltObj)

	GBUINT16 iCandidateBoxHeight = 0;
	GBUINT16 iKeyBoardHeight = 0;
	GBUINT16 iCurrentLayoutId;

	// ��ǰҪ��ʾ�Ĳ�
	GBUINT16 iCurrentLayerNumber;
	GBIMEPoint winPos;
	GBIMEReturn ret;

	// ��ǰLayoutId
	iCurrentLayoutId = GET_LAYOUT_ID(pltRoot);
	// ��ȡ���̸߶�
	iKeyBoardHeight = pltRoot->m_pfcGetLayoutHeight(pltObj,iCurrentLayoutId);
	// ȡ�ü�����ʾ�Ŀ�ʼλ�á����½ǡ���������Ϊ�����Ͻǡ�
	Global_GetWinPos(&winPos);	
	winPos.y -=  iKeyBoardHeight;
	// ��ǰҪ��ʾ�Ĳ�
	iCurrentLayerNumber = GET_LAYOUT_CURRENT_LAYER(pltRoot);

	// �����������������º͵����ֵ��һ�²�����Ӧ
	if (pltRoot->penInPostion == PenPositionInKeyboardButton
		&& pltRoot->pressLayoutKeyValue != pltRoot->currentLayoutKeyValue)
	{
		return ;
	}

	// ���Layout��Ч������Ҫ���´�����
	if (pltRoot->penInPostion != PenPositionInNothing) 
	{
		ICLayoutVK9MultiLang__SetTouchRegion(pltObj, pEngineOutputInfo);
	}
	
	if (pltRoot->penInPostion == PenPositionInKeyboardButton)
	{
		pltRoot->m_pfcShowCandWin(pltObj, buttonStatus, pEngineOutputInfo);
	}

	if (buttonStatus == ButtonStatusUp)
	{
		switch (iCurrentLayerNumber)
		{
		case FirstLayer: // ��ǰ�ǵ�һ��ʱ	
			// �رպ�ѡ��
			pltRoot->m_pfcCloseCandWin(pltObj, pEngineOutputInfo);
			break;
		case SecondLayer: // ��ǰ�ǵڶ���ʱ
			// ��������
			pltRoot->m_pfcCheckUpScreen(pltObj, buttonStatus, pEngineOutputInfo);
			
			// �رպ�ѡ��
			if ((ret = pltRoot->m_pfcCloseCandWin(pltObj, pEngineOutputInfo)) != GBIME_IGNORE)
			{
				// ֱ���л���Ĭ��Сд״̬
				if (Global_GetShiftCapStatus() == GBStatus_Shift)
				{
					pltRoot->m_pfcSwitchShiftCapStatus(pltObj, GBStatus_Normal, GBFalse, pEngineOutputInfo);	
				}
				return ;
			}

			if ((pltRoot->penInPostion == PenPositionInCandidatePageDown)
				||(pltRoot->penInPostion == PenPositionInCandidatePageUp))
			{
				// �ڵ�2���У����к�ѡ��ҳʱ�����ǽ����2��
				pltRoot->m_ltData.ltCurLayer = SecondLayer;
				return;
			}
			break;
		case ThirdLayer: // ��ǰ������ʱ			
			break;
		default:
			break;
		}
	}	
}

/*!
 * \brief ���ô�������
 * \param pltObj 
 * \param pEngineOutputInfo 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-10 11:30:08
*/
static void ICLayoutVK9MultiLang__SetTouchRegion(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo)
{
	CLASE_THIS_POITER(CLayoutVK9MultiLang, pltObj)
	pltBase->m_pfcSetCandidateTouchRegion(pltObj, pEngineOutputInfo); // ��ѡ��������
	pltBase->m_pfcSetCandidatePageButtonTouchRegion(pltObj, pEngineOutputInfo);// ��ѡ��ҳ������
}

#endif // �����������
