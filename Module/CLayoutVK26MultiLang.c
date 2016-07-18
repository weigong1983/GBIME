/***************************************************************************
* File Name    :  CLayoutVK26MultiLang.c
* Corporation  :  Guobi Technology Ltd. GuangZhou China
* Copyright    :  Copyright Guobi Technology Ltd. And Subject to Confidentiality 
*                 Restrictions. This document is the confidential and proprietary 
*                 property of Guobi Technology Ltd.It is neither the document                  
*                 itself nor any part of it can be shared with any party  without 
*                 the written  permission. 
* Author       :  weizhiping
* Date         :  2010-08-17
* Purpose      :  ����Qwerty������Layoutʵ��
* Version      :  V1.0
* Last Changed List
* Name         |   date   |  Discription
****************************************************************************/

#if defined(__GBIME_VK__) // �����������

#include "CLayoutVK26MultiLang.h"
#include "CLayoutViewVK26MultiLang.h"

/*!
 * \brief ���ظ����Ա��������
 */
GBIMELOCAL GBIMEReturn CLayoutVK26MultiLang__Release(GBLPVOID pltObj, PLayoutInfo pltUIData);
GBIMELOCAL GBIMEReturn CLayoutVK26MultiLang__Init(PCLayoutVK26MultiLang pltVK26);
GBIMELOCAL GBIMEReturn CLayoutVK26MultiLang__Paint(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo);
GBIMELOCAL GBIMEReturn CLayoutVK26MultiLang__HandleEvent(GBLPVOID pltObj, PGBIMEEvent pIMEEvent, PEngineOutputInfo pEngineOutputInfo);
GBIMELOCAL PenPosition CLayoutVK26MultiLang__HitTest(GBLPVOID pltObj, 
										  GBIMEPoint hitPoint, 
										  ButtonStatusEnum buttonStatus, 
										  PEngineOutputInfo pEngineOutputInfo);
GBIMELOCAL GBIMEReturn CLayoutVK26MultiLang__FuncKeyHandler(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo);
GBIMELOCAL GBBOOL CLayoutVK26MultiLang__IsInputKey(GBUINT16 keyValue);

/*!
 * \brief ���غ�������
 */
GBIMELOCAL void ICLayoutVK26MultiLang__UpdateLayoutInformation(GBLPVOID pltObj, 
													ButtonStatusEnum buttonStatus,
													PEngineOutputInfo pEngineOutputInfo);
GBIMELOCAL void ICLayoutVK26MultiLang__SetTouchRegion(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo);

/*!
 * \brief ����ʵ��
 */
CLayoutAbstract * CLayoutVK26MultiLang__Construct(PCLayoutVK26MultiLang pltVK26, 
												  GBLPCVOID pltUIData,
												  GBLPCVOID pltIMData,
												  GBLPCVOID pltConfigData)
{
	CLayoutVK__Construct(&pltVK26->m_ltParent,
		pltUIData,
		pltIMData,
		pltConfigData);
	CLayoutVK26MultiLang__Init(pltVK26);

	return (CLayoutAbstract *)pltVK26;
}

GBIMELOCAL GBIMEReturn CLayoutVK26MultiLang__Release(GBLPVOID pltObj, PLayoutInfo pltUIData)
{
	CLASE_THIS_POITER(CLayoutVK26MultiLang, pltObj)

	pltRoot->m_pfcReleaseBase(pltObj, pltUIData);
	return GBIME_OK;
}

GBIMELOCAL GBIMEReturn CLayoutVK26MultiLang__Init(PCLayoutVK26MultiLang pltVK26)
{
	PCLayoutViewVK26MultiLang pLtView = NULL;

	// �����Ӧ����ͼ����
	pLtView = (PCLayoutViewVK26MultiLang)PT_Malloc(sizeof(CLayoutViewVK26MultiLang));
	PT_Assert(pLtView != NULL);
	memset(pLtView, 0, sizeof(CLayoutViewVK26MultiLang));
	CLayoutViewVK26MultiLang__Construct(pLtView);

	pltVK26->m_ltParent.m_ltParent.m_hltView = (PCLayoutViewAbstract)pLtView;
	pltVK26->m_ltParent.m_ltParent.m_pfcPaint = CLayoutVK26MultiLang__Paint;
	pltVK26->m_ltParent.m_ltParent.m_pfcHitTest = CLayoutVK26MultiLang__HitTest;
	pltVK26->m_ltParent.m_ltParent.m_pfcUpdateLayoutInformation = ICLayoutVK26MultiLang__UpdateLayoutInformation;
	pltVK26->m_ltParent.m_pfcSetTouchRegion = ICLayoutVK26MultiLang__SetTouchRegion;
	pltVK26->m_ltParent.m_ltParent.m_pfcHandleEvent = CLayoutVK26MultiLang__HandleEvent;
	pltVK26->m_ltParent.m_ltParent.m_pfcRelease = CLayoutVK26MultiLang__Release;
	pltVK26->m_ltParent.m_ltParent.m_pfcFuncKeyHandler = CLayoutVK26MultiLang__FuncKeyHandler;
	pltVK26->m_ltParent.m_ltParent.m_pfcIsInputKey = CLayoutVK26MultiLang__IsInputKey;

	pltVK26->m_ltParent.m_ltParent.currentSyllableIndex = 0;
	pltVK26->m_ltParent.m_ltParent.currentCandidateIndex = 0;
	return GBIME_OK;
}

GBIMELOCAL GBIMEReturn CLayoutVK26MultiLang__Paint(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo)
{
	CLASE_THIS_POITER(CLayoutVK26MultiLang, pltObj)

	if (pltRoot->m_hltView != NULL 
		&& pltRoot->m_hltView->m_pfcPaint != NULL
		&& Global_GetLayoutWinActiveStatus()) // ����״̬����ʾ
	{
		pltRoot->m_hltView->m_pfcPaint(pltObj, pEngineOutputInfo);
	}

	return GBIME_OK;
}

GBIMELOCAL GBIMEReturn CLayoutVK26MultiLang__HandleEvent(GBLPVOID pltObj, PGBIMEEvent pIMEEvent, PEngineOutputInfo pEngineOutputInfo)
{
	CLASE_THIS_POITER(CLayoutVK26MultiLang, pltObj)		
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
		ICLayoutVK26MultiLang__UpdateLayoutInformation(pltObj, ButtonStatusDown, pEngineOutputInfo);
		if (pltRoot->penInPostion != PenPositionInNothing)
		{
			pltRoot->m_refreshFlag |= (GBIME_LAYOUT_REFRESH_CANDIDATE_LINE | GBIME_LAYOUT_REFRESH_KEYBOARD);
		}
		break;
	case GBIME_EVENT_LAYOUT_PEN_UP:
		ICLayoutVK26MultiLang__UpdateLayoutInformation(pltObj, ButtonStatusUp, pEngineOutputInfo);
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
	case GBIME_EVENT_KEY_DOWN: // ������ֱ��ˢ��	
	case GBIME_EVENT_KEY_UP:
	case GBIME_EVENT_KEY_LONG_PRESS:
		pltRoot->m_refreshFlag = (GBIME_LAYOUT_REFRESH_CANDIDATE_LINE | GBIME_LAYOUT_REFRESH_SYLLABLE_LINE);
		break;
	default:
		break;
	}

	// Ĭ�ϴ���
	
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
static PenPosition CLayoutVK26MultiLang__HitTest(GBLPVOID pltObj, 
												 GBIMEPoint hitPoint, 
												 ButtonStatusEnum buttonStatus, 
												 PEngineOutputInfo pEngineOutputInfo)
{
	CLASE_THIS_POITER(CLayoutVK26MultiLang, pltObj)

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
		if ((penPositionInWhere == PenPositionInKeyboardButton && pltRoot->pressLayoutKeyValue == pltRoot->currentLayoutKeyValue)
			|| iCurrentLayoutId == LayoutIdSP26Jabc  // abc ֱ��������ĸ������Ҫ��ѡ��
			|| iCurrentLayoutId == LayoutIdSP26JABC) // ABC ֱ��������ĸ������Ҫ��ѡ��
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
	case ThirdLayer: // ��ǰ������ʱ	
		break;
	default:
		break;
	}

	return penPositionInWhere;
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
GBIMELOCAL GBIMEReturn CLayoutVK26MultiLang__FuncKeyHandler(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo)
{
	CLASE_THIS_POITER(CLayoutVK26MultiLang, pltObj)
	GBUINT16 tempStr[2];
	GBUINT16 iCurrentLayoutId = GET_LAYOUT_ID(pltRoot);

	pltRoot->bUpToInputKeySequence = GBFalse;

	switch (pltRoot->currentLayoutKeyValue)
	{
	case GBKEY_VK_SETTING: ///<����
		Global_FillEventGroup(GBIME_CT_CONFIG_MENU, 0, 0);
		break;
	case GBKEY_RETURN: // ������ʱ��������س����������С�����ʱ��������س���������������
		if (pEngineOutputInfo->bCandWindOpen && !pEngineOutputInfo->isMultiTapInputMode)
		{
			// ����������Ҫȷ�����ڷ�ѡ��״̬
			GBEngine_ExitSelectedStatus(Global_GetEnineInstance());
			pltRoot->bSendEngineKeyEvent = GBTrue;
			pltRoot->engineKeyEvent = GBKEY_UP;
			pltRoot->bUpToInputKeySequence = GBTrue;
			Global_FillEventGroup(GBIME_CT_NORMAL_KEY_UP, 0, 0);
		}
		else
		{
			tempStr[0] = GBKEY_RETURN;
			tempStr[1] = 0;
			pltRoot->m_pfcUpScreen(pltObj, tempStr);
			Global_FillEventGroup(GBIME_CT_KEY_UP_REFRESH, 0, 0); // ��ť����ˢ��
		}
		break;
	case GBKEY_SPACE: // ������ʱ��������ո񡿼�������ո�����ʱ��������ո񡿼�����ѡ��������
		if (pEngineOutputInfo->bCandWindOpen && !pEngineOutputInfo->isMultiTapInputMode)
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
			Global_FillEventGroup(GBIME_CT_KEY_UP_REFRESH, 0, 0); // ��ť����ˢ��
		}
		break;
	case GBKEY_BACK:
		if (pEngineOutputInfo->bCandWindOpen && !pEngineOutputInfo->isMultiTapInputMode)
		{
			/*if (pEngineOutputInfo->isMultiTapInputMode)
			{
				pltRoot->bSendEngineKeyEvent = GBTrue;
				pltRoot->engineKeyEvent = GBKEY_OK;
				Global_FillEventGroup(GBIME_CT_NORMAL_KEY_UP, GBKEY_OK, 0);
			}
			else*/
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
	case GBKEY_VK_CAP: // �л�Ӣ�������Сд	
		if (GBEngine_IsAlpInputMode(Global_GetEnineInstance()))
		{
			pltRoot->m_pfcSwitchShiftCapStatus(pltObj, 0, GBTrue, pEngineOutputInfo);
		}
		break;
	case GBKEY_VK_V9_SYMBOL_KEY: // MultiTap�����Ŵ���		
		if (GBEngine_IsInputStatus(Global_GetEnineInstance()))
		{			
			pltRoot->bSendEngineKeyEvent = GBTrue;
			pltRoot->engineKeyEvent = GBIME_EMBEDDED_INTERPUNCTION_KEY;
			Global_FillEventGroup(GBIME_CT_NORMAL_KEY_UP, 0, 0);
		} 
		else
		{
			pltRoot->m_pfcMultiTapSymbolHandler(pltObj);
			Global_FillEventGroup(GBIME_CT_KEY_UP_REFRESH, 0, 0); // ��ť����ˢ��
		}		
		break;
	case GBKEY_VK_V9_SYMBOL_FACE_MULTIP_KEY: // MultiTap������Ŵ���		
		pltRoot->m_pfcMultiTapFaceHandler(pltObj);
		Global_FillEventGroup(GBIME_CT_KEY_UP_REFRESH, 0, 0); // ��ť����ˢ��
		break;
	default:
		return GBIME_IGNORE;
		break;
	}
	
	return GBIME_OK;
}

/*!
 * \brief �ж��Ƿ������
 * \param keyValue 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-17 17:33:07
*/
GBIMELOCAL GBBOOL CLayoutVK26MultiLang__IsInputKey(GBUINT16 keyValue)
{
	if ((keyValue >= GBKEY_A && keyValue <= GBKEY_Z)
		|| (keyValue >= GBKEY_VK_A_UPPER && keyValue <= GBKEY_VK_Z_UPPER))
	{
		return GBTrue;
	}
	return GBFalse;
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
static void ICLayoutVK26MultiLang__UpdateLayoutInformation(GBLPVOID pltObj, ButtonStatusEnum buttonStatus, PEngineOutputInfo pEngineOutputInfo)
{
	CLASE_THIS_POITER(CLayoutVK26MultiLang, pltObj)

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
	
	// abcֱ��������ĸ������Ҫ��ѡ�� 
	if (iCurrentLayoutId == LayoutIdSP26Jabc || iCurrentLayoutId == LayoutIdSP26JABC)
	{
		if (pltRoot->penInPostion == PenPositionInKeyboardButton)
		{
			pltRoot->m_ltData.ltCurLayer = FirstLayer;			
			if (buttonStatus == ButtonStatusUp
				&& pltRoot->m_pfcIsInputKey(pltRoot->currentLayoutKeyValue))
			{
				GBUINT16 strNumber[2];
				
				strNumber[0] = CLayoutInfo_ConversionLayoutKeyToEngineKey(pltRoot->currentLayoutKeyValue);
				if (Global_GetShiftCapStatus() != GBStatus_Normal)
				{
					strNumber[0] -= 32; // 'A' - 'a' ת���ɴ�д
				}
				strNumber[1] = 0;
				pltRoot->m_pfcUpScreen(pltObj, strNumber);

				// ֱ���л���Ĭ��Сд״̬
				if (Global_GetShiftCapStatus() == GBStatus_Shift)
				{
					pltRoot->m_pfcSwitchShiftCapStatus(pltObj, GBStatus_Normal, GBFalse, pEngineOutputInfo);	
				}
			}
		}

		return ;
	}	

	// ���Layout��Ч������Ҫ���´�����
	if (pltRoot->penInPostion != PenPositionInNothing)
	{
		ICLayoutVK26MultiLang__SetTouchRegion(pltObj, pEngineOutputInfo);
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
static void ICLayoutVK26MultiLang__SetTouchRegion(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo)
{
	CLASE_THIS_POITER(CLayoutVK26MultiLang, pltObj)
	pltBase->m_pfcSetCandidateTouchRegion(pltObj, pEngineOutputInfo); // ��ѡ��������
	pltBase->m_pfcSetCandidatePageButtonTouchRegion(pltObj, pEngineOutputInfo);// ��ѡ��ҳ������
}

#endif // �����������