/*****************************************************************************
*  Copyright Statement:
*  --------------------
*
* Filename:
* ---------
*    CLayoutKB9PinYin.c
*
* Project:
* --------
*   Guobi Input Method
*
* Description:
* ------------
*
* Author:
* -------
* -------
***********************************************************************/
#if defined(__GBIME_KB__) // �����������
#include "CLayoutKB.h"
#include "CLayoutViewKB.h"
#include "PlatformAdapter.h"

GBIMELOCAL GBIMEReturn CLayoutKB__Init(PCLayoutKB pltKB);
GBIMELOCAL GBIMEReturn CLayoutKB__Paint(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo);
GBIMELOCAL GBIMEReturn CLayoutKB__PreHandleEvent(GBLPVOID pltObj, GBINT nPlatformEvent, GBINT wParam, GBINT lParam, PEngineOutputInfo pEngineOutputInfo);
GBIMELOCAL GBIMEReturn CLayoutKB__HandleCommonEvent(GBLPVOID pltObj, PGBIMEEvent pIMEEvent, PEngineOutputInfo pEngineOutputInfo);
GBIMELOCAL GBIMEReturn CLayoutKB__HandleEvent(GBLPVOID pltObj, PGBIMEEvent pIMEEvent, PEngineOutputInfo pEngineOutputInfo);

GBIMELOCAL void CLayoutKB__SetSyllableRegion(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo); // ���ڴ�����
GBIMELOCAL void CLayoutKB__SetOutputStringRegion(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo); // �ʻ�������
GBIMELOCAL void CLayoutKB__SetCandidateRegion(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo); // ��ѡ������
GBIMELOCAL void CLayoutKB__SetSymbolCandRegion(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo); // ��ѡ������
GBIMELOCAL void CLayoutKB__SetSyllablePageButtonRegion(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo); // ���ڷ�ҳ��ť������
GBIMELOCAL void CLayoutKB__SetCandidatePageButtonRegion(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo);// ��ѡ��ҳ��ť������
GBIMELOCAL void CLayoutKB__UpdateLayoutInformation(GBLPVOID pltObj, ButtonStatusEnum buttonStatus,	PEngineOutputInfo pEngineOutputInfo);
CLayoutAbstract * CLayoutKB__Construct(PCLayoutKB pltKB, 
											  GBLPCVOID pltUIData,
											  GBLPCVOID pltIMData,
											  GBLPCVOID pltConfigData)
{
	CLayoutAbstract__Construct(&pltKB->m_ltParent,
								pltUIData,
								pltIMData,
								pltConfigData);
	CLayoutKB__Init(pltKB);

	return (CLayoutAbstract *)pltKB;
}

GBIMELOCAL GBIMEReturn CLayoutKB__Init(PCLayoutKB pltKB)
{
	PCLayoutViewKB pLtView = NULL;
	
	// �����Ӧ����ͼ����
	pLtView = (PCLayoutViewKB)PT_Malloc(sizeof(CLayoutViewKB));
	PT_Assert(pLtView != NULL);
	memset(pLtView, 0, sizeof(CLayoutViewKB));
	CLayoutViewAbstract__Construct(&pLtView->m_ltParent);
	CLayoutViewKB__Init(pLtView);
	pltKB->m_ltParent.m_hltView = (PCLayoutViewAbstract)pLtView;

	pltKB->m_ltParent.m_pfcPaint = CLayoutKB__Paint;
	pltKB->m_ltParent.m_pfcPreHandleEvent = CLayoutKB__PreHandleEvent;
	pltKB->m_ltParent.m_pfcHandleCommonEvent = CLayoutKB__HandleCommonEvent;
	pltKB->m_ltParent.m_pfcHandleEvent = CLayoutKB__HandleEvent;

	pltKB->m_pfcSetSyllableRegion = CLayoutKB__SetSyllableRegion; // �������ڴ�����
	pltKB->m_pfcSetOutputStringRegion = CLayoutKB__SetOutputStringRegion;
	pltKB->m_pfcSetCandidateRegion = CLayoutKB__SetCandidateRegion; // ���ú�ѡ������
	pltKB->m_pfcSetSymbolCandRegion = CLayoutKB__SetSymbolCandRegion;//���ñ����ʾ����
	pltKB->m_pfcSetSyllablePageButtonRegion = CLayoutKB__SetSyllablePageButtonRegion; // �������ڷ�ҳ��ť������
	pltKB->m_pfcSetCandidatePageButtonRegion = CLayoutKB__SetCandidatePageButtonRegion;// ���ú�ѡ��ҳ��ť������
	pltKB->m_pfcUpdateLayoutInformation = CLayoutKB__UpdateLayoutInformation;

	return GBIME_OK;
}

GBIMELOCAL GBIMEReturn CLayoutKB__Paint(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo)
{
	PCLayoutKB pltKB = (PCLayoutKB)pltObj;

	if (pltKB->m_ltParent.m_hltView != NULL 
		&& pltKB->m_ltParent.m_hltView->m_pfcPaint != NULL
		&& Global_GetLayoutWinActiveStatus()) // ����״̬����ʾ
	{
		pltKB->m_ltParent.m_hltView->m_pfcPaint(pltKB, pEngineOutputInfo);
	}

	return GBIME_OK;
}

/*!
 *-brief �ú�����ɶ�ƽ̨ԭʼ��Ϣ��Ԥ����
 1. Layout��������, �����Ƿ���Ҫ������Ӧ�İ�����Ϣ���������洦��ı�ǲ���¼����ֵ(ʵ�ʴ�����Ϣʱ�õ�)
 2. ת���ɿ�ܶ�Ӧ��һ����Ϣ
 */
GBIMELOCAL GBIMEReturn CLayoutKB__PreHandleEvent(GBLPVOID pltObj, GBINT nPlatformEvent, GBINT wParam, GBINT lParam, 
									  PEngineOutputInfo pEngineOutputInfo)
{
	CLASE_THIS_POITER(CLayoutKB, pltObj)

//	GBUINT16	iCurrentLayoutID;
	GBUINT16	iKeyBoardHeight = 0;
	GBUINT16	iKeyBoardWidth = 0;
//	GBIMEPoint	winPos;
//	GBIMEPoint	hitPoint;
//	GBRECT		rect;
//	GBINT		iScreenHeight;
	PLayoutInfo	pLayoutInfo = (PLayoutInfo)(&pltBase->m_ltData); // Layout UI data point
	GBINT		nNextInputMode = 0;
	GBUINT16	layoutID = 0;

	// ��ƽ̨�����¼�ת����IME�����Ҫ������¼�
	switch (nPlatformEvent)
	{
	case GBPEVT_KEY_DOWN:
		Global_FillEventGroup(GBIME_CT_KEY_DOWN, wParam, lParam);
		pltBase->bSendEngineKeyEvent = GBTrue;
		pltBase->engineKeyEvent = wParam;
		// ����Layout
		Global_SetLayoutWinActiveStatus(GBTrue);
		// �������水���ܰ���������
		//GBInputBox_SetAllIMEFuncKey();
		break;
	case GBPEVT_KEY_UP:
		Global_FillEventGroup(GBIME_CT_KEY_UP, wParam, lParam);
		pltBase->bSendEngineKeyEvent = GBTrue;
		pltBase->engineKeyEvent = wParam;
		break;
	case GBPEVT_KEY_LONG_PRESS:
		Global_FillEventGroup(GBIME_CT_KEY_LONG_PRESS, wParam, lParam);
		pltBase->bSendEngineKeyEvent = GBTrue;
		pltBase->engineKeyEvent = wParam;
		break;
#if 0 // ��δ�����ǰ�������ϢԤ���������
	case GBPEVT_PEN_DOWN:
		iCurrentLayoutID = GET_LAYOUT_ID(pltBase);
		// ��ȡ���̸߶�
		iKeyBoardHeight = pltBase->m_pfcGetLayoutHeight(pltObj,iCurrentLayoutID);
		iKeyBoardWidth = pltBase->m_pfcGetLayoutWidth(pltObj, iCurrentLayoutID);
		Global_GetWinPos(&winPos);
		winPos.y -=  iKeyBoardHeight;
		hitPoint.x = wParam;
		hitPoint.y = lParam;
		rect.left = winPos.x;
		rect.right = winPos.x + iKeyBoardWidth;
		rect.top = winPos.y;
		rect.bottom = winPos.y + iKeyBoardHeight;
		
		if(!PT_PointIsInRect(hitPoint, rect)
			&& hitPoint.y < rect.bottom)
		{
			//�л����������
			//?�ô���Ҫ��װ�£���ͬ����layout�л�����ͬ����layout����vk stroke�л���kb stroke
			if(LayoutIdSP9JPinYin == GET_LAYOUT_ID(pltBase))
			{
				Global_FillEventGroup(GBIME_CT_KEY_DOWN, wParam, lParam);
			}
			else
			{
				Global_FillEventGroup(GBIME_CT_VK_2_KB_SWITCH, LayoutIdSP9JPinYin, lParam);
			}	
		}

		break;
#endif
	case GBPEVT_PEN_DOWN: // ��Pen��Ϣ���ѵ�������ƽ̨������mmi_imc_key_set_softkey
	case GBPEVT_PEN_MOVE:
	case GBPEVT_PEN_UP:
		return GBIME_CONSUMED;
	case GBPEVT_KEY_REPEAT:	
	default:
		return GBIME_LAYOUT_CONVERT_MSG_FAIL;
	}

	return GBIME_OK;
}

/*!
 * \brief Layout������Ϣ������(���һЩ������ͨ����Ϣ����)
 * \param pltObj 
 * \param pIMEEvent 
 * \param pEngineOutputInfo 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-17 10:47:49
*/
GBIMELOCAL GBIMEReturn CLayoutKB__HandleCommonEvent(GBLPVOID pltObj, PGBIMEEvent pIMEEvent, PEngineOutputInfo pEngineOutputInfo)
{
	PCLayoutAbstract pltAbstractObj = (PCLayoutAbstract)pltObj;
	GBIMEReturn ret = GBIME_OK;
	GBUINT16 nCurrentLayoutID = GET_LAYOUT_ID(pltAbstractObj);

	switch (pIMEEvent->nEvent)
	{
	case GBIME_EVENT_LAYOUT_ACTIVATE: ///<Layout ����
		pltAbstractObj->m_ltData.ltCurLayer = FirstLayer;
		Global_SetLayoutWinActiveStatus(GBTrue);
		// ��������״μ����ʾ
		Global_SetLayoutWinHeight(0);
		pltAbstractObj->m_refreshFlag = GBIME_LAYOUT_REFRESH_ALL;
		break;
	case GBIME_EVENT_LAYOUT_DEACTIVATE: ///<Layout ע��
		pltAbstractObj->m_ltData.ltCurLayer = FirstLayer;
		Global_SetLayoutWinActiveStatus(GBFalse);
		Global_SetLayoutWinHeight(0);
		// ���³�ʼ��Layout����״̬����
		pltAbstractObj->m_pfcInitVariable(pltObj);
		break;
	case GBIME_EVENT_LAYOUT_HIDE: ///<Layout ����
		pltAbstractObj->m_ltData.ltCurLayer = FirstLayer;
		Global_SetLayoutWinActiveStatus(GBFalse);
		pltAbstractObj->m_pfcChangeHeight(pltObj, 0);
		// ���³�ʼ��Layout����״̬����
		pltAbstractObj->m_pfcInitVariable(pltObj);
		break;
	case GBIME_EVENT_LAYOUT_SHOW: ///<Layout ��ʾ
		pltAbstractObj->m_ltData.ltCurLayer = FirstLayer;
		Global_SetLayoutWinActiveStatus(GBTrue);
		// ������̲���ʾ
		pltAbstractObj->m_pfcChangeHeight(pltObj, 0);		
		pltAbstractObj->m_refreshFlag = GBIME_LAYOUT_REFRESH_ALL;
		break;
	case GBIME_EVENT_LAYOUT_ENTER:
		pltAbstractObj->m_ltData.ltCurLayer = FirstLayer;
		//Global_SetLayoutWinHeight(0); // �߶����㣬���Layout���л����ڸ߶�һ��û��ˢ��VK
		//pltAbstractObj->m_pfcChangeHeight(pltObj, pltAbstractObj->m_pfcGetLayoutHeight(pltObj, nCurrentLayoutID));
		pltAbstractObj->m_refreshFlag = GBIME_LAYOUT_REFRESH_ALL;
		//pltAbstractObj->m_pfcInitVariable(pltObj);
		break;
	case GBIME_EVENT_LAYOUT_EXIT:
		Global_SetLayoutWinHeight(0); // �߶����㣬���Layout���л����ڸ߶�һ��û��ˢ��VK
		pltAbstractObj->m_pfcInitVariable(pltObj);
		break;
	default:
		ret = GBIME_IGNORE;
		break;
	}
	
	return ret;
}

GBIMELOCAL GBIMEReturn CLayoutKB__HandleEvent(GBLPVOID pltObj, PGBIMEEvent pIMEEvent, PEngineOutputInfo pEngineOutputInfo)
{
	CLASE_THIS_POITER(CLayoutKB, pltObj)
		GBIMEReturn ret;

	PT_Assert(pltObj != NULL);
	PT_Assert(pIMEEvent != NULL);
	PT_Assert(pEngineOutputInfo != NULL);

	pltBase->m_refreshFlag = GBIME_LAYOUT_REFRESH_NONE;

	// Layout���ڷǼ���״̬�����ȼ�����ʾ���ܽ����¼�
	if (!Global_GetLayoutWinActiveStatus()
		&& !(pIMEEvent->nEvent == GBIME_EVENT_LAYOUT_ACTIVATE || pIMEEvent->nEvent == GBIME_EVENT_LAYOUT_SHOW))
	{
		return GBIME_IGNORE;
	}

	// ������Ϣ����
	if ((ret = pltBase->m_pfcHandleCommonEvent(pltObj, pIMEEvent, pEngineOutputInfo)) != GBIME_IGNORE)
	{
		return ret;
	}

	// ��Ϣ�ַ�(���Layout��ť״̬���¡��������水���¼��������������Ϣ)
	switch (pIMEEvent->nEvent)
	{
	case GBIME_EVENT_KEY_DOWN://GBIME_EVENT_LAYOUT_PEN_DOWN:		
		pltBase->m_refreshFlag |= (GBIME_LAYOUT_REFRESH_CANDIDATE_LINE | GBIME_LAYOUT_REFRESH_SYLLABLE_LINE);
		break;
	case GBIME_EVENT_KEY_UP://GBIME_EVENT_LAYOUT_PEN_UP:
		pltBase->m_refreshFlag |= (GBIME_LAYOUT_REFRESH_CANDIDATE_LINE | GBIME_LAYOUT_REFRESH_SYLLABLE_LINE);
		break;
	case GBIME_EVENT_KEY_LONG_PRESS://GBIME_EVENT_LAYOUT_PEN_MOVE:
		pltBase->m_refreshFlag = (GBIME_LAYOUT_REFRESH_CANDIDATE_LINE | GBIME_LAYOUT_REFRESH_SYLLABLE_LINE);
		break;
	default:
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
static void CLayoutKB__SetSyllableRegion(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo)
{
	CLASE_THIS_POITER(CLayoutKB, pltObj)
	GBPCWCHAR *ppCandidates;
	GBUINT16 iCandidateCount = 0;
	PT_TextAlign textAlign = PT_TEXT_ALIGN_CENTER;	
	GBIMEPoint candStartPoint;
	GBUINT16 iCurLayoutId;
	GBUINT16 iKeyBoardWidth, iKeyBoardHeight;	
	
	pltBase->m_pfcGetLayoutStartPoint(pltObj, &candStartPoint);	
	// ��ǰLayoutId
	iCurLayoutId = GET_LAYOUT_ID(pltBase);
	// ��ȡ���̿��
	iKeyBoardWidth = pltBase->m_pfcGetLayoutWidth(pltObj, iCurLayoutId);
	iKeyBoardHeight = pltBase->m_pfcGetLayoutHeight(pltObj, iCurLayoutId);

	if (/*GBEngine_IsSymbolCandType(Global_GetEnineInstance())*/pEngineOutputInfo->isSymbolCandType
		&& (pEngineOutputInfo->pOutputString && pEngineOutputInfo->pOutputString[0] != 0))
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
	PT_CreateTouchRegion(&pltThis->syllableRegion,			// ����������
						 pltBase->syllableDisplayStartPos,		// ��������ʾ�Ŀ�ʼλ��
						 (GBUINT16)(iKeyBoardWidth - 10),	// ���������
						 (GBUINT16)(iKeyBoardHeight / 2),	// �������߶�
						 ppCandidates,							// ��������
						 iCandidateCount,						// ���ڸ���
						 GBIME_CANDIDATE_FONT_SIZE,				// ����
						 textAlign);							// �ı�����
}

/*!
 * \brief ���ñʻ�������
 * \param pltObj 
 * \param pEngineOutputInfo 
 * \return 
 * \note 
 * \example
 * \author Zhaokun
 * \date 2010-8-24 17:39:07
*/
static void CLayoutKB__SetOutputStringRegion(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo)
{
	CLASE_THIS_POITER(CLayoutKB, pltObj)
	
	GBPCWCHAR *ppStroke;
	GBUINT16 iCount = 0;
	PT_TextAlign textAlign = PT_TEXT_ALIGN_CENTER;
	GBUINT16 iKeyBoardHeight = 0;
	GBUINT16 iKeyBoardWidth = 0;
	GBUINT16 iCurrentLayoutId;
	
	ppStroke = &pEngineOutputInfo->pOutputString;
	iCount = 1;

	// ��ȡ���̸߶�
	iCurrentLayoutId = GET_LAYOUT_ID(pltBase);
	iKeyBoardHeight = pltBase->m_pfcGetLayoutHeight(pltObj,iCurrentLayoutId);
	iKeyBoardWidth = pltBase->m_pfcGetLayoutWidth(pltObj, iCurrentLayoutId);
	
// 	// ������ѡ������ʾ�����ֲ�Ʒ���һ��
// 	if (iCount == 1)
// 	{
// 		textAlign = PT_TEXT_ALIGN_LEFT;
// 	}
	PT_CreateTouchRegion(&pltThis->syllableRegion,			// ����������
		pltBase->syllableDisplayStartPos,		// ��������ʾ�Ŀ�ʼλ��
		(GBUINT16)iKeyBoardWidth,	// ���������
		(GBUINT16)(iKeyBoardHeight / 2),	// �������߶�
		ppStroke,							// ��������
		iCount,						// ���ڸ���
		GBIME_CANDIDATE_FONT_SIZE,				// ����
		PT_TEXT_ALIGN_LEFT);							// �ı�����
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
static void CLayoutKB__SetCandidateRegion(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo)
{
	CLASE_THIS_POITER(CLayoutKB, pltObj)	
	
	GBIMEPoint candStartPoint;
	GBUINT16 iCurLayoutId;
	GBUINT16 iKeyBoardWidth, iKeyBoardHeight;	
	
	pltBase->m_pfcGetLayoutStartPoint(pltObj, &candStartPoint);	
	// ��ǰLayoutId
	iCurLayoutId = GET_LAYOUT_ID(pltBase);
	// ��ȡ���̿��
	iKeyBoardWidth = pltBase->m_pfcGetLayoutWidth(pltObj, iCurLayoutId);
	iKeyBoardHeight = pltBase->m_pfcGetLayoutHeight(pltObj, iCurLayoutId);
	
	PT_CreateTouchRegion(&pltThis->candidateRegion,		// ��ѡ���ִ�������
		pltBase->candidateDisplayStartPos,		// ��ѡ������ʾ�Ŀ�ʼλ��
		(GBUINT16)(iKeyBoardWidth - 10),	// ��ѡ�����
		(GBUINT16)(iKeyBoardHeight / 2),	// ��ѡ���߶�
		pEngineOutputInfo->ppCandidates,		// ��ѡ����
		(GBUINT16)pEngineOutputInfo->nCandNum,	// ��ѡ����
		GBIME_CANDIDATE_FONT_SIZE,				// ����
		PT_TEXT_ALIGN_CENTER);					// �ı�����	
}
/*!
 * \brief ���ñ������
 * \param pltObj 
 * \param pEngineOutputInfo 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-8-24 17:39:07
*/
static void CLayoutKB__SetSymbolCandRegion(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo)
{
	CLASE_THIS_POITER(CLayoutKB, pltObj)
	GBINT i = 0;
	GBUINT16 nCandNum = 0;
	GBIMEPoint candStartPoint;
	GBUINT16 iCurLayoutId;
	GBUINT16 iKeyBoardWidth, iKeyBoardHeight;

	nCandNum = pEngineOutputInfo->nCandNum / 2;

	pltBase->m_pfcGetLayoutStartPoint(pltObj, &candStartPoint);

	// ��ǰLayoutId
	iCurLayoutId = GET_LAYOUT_ID(pltBase);
	// ��ȡ���̿��
	iKeyBoardWidth = pltBase->m_pfcGetLayoutWidth(pltObj, iCurLayoutId);
	iKeyBoardHeight = pltBase->m_pfcGetLayoutHeight(pltObj, iCurLayoutId);

	PT_CreateTouchRegion(&pltThis->syllableRegion,		// ��ѡ���ִ�������
		candStartPoint,		// ��ѡ������ʾ�Ŀ�ʼλ��
		(GBUINT16)(iKeyBoardWidth - 10),	// ��ѡ�����
		(GBUINT16)(iKeyBoardHeight / 2),	// ��ѡ���߶�
		pEngineOutputInfo->ppCandidates,		// ��ѡ����
		nCandNum,	// ��ѡ����
		GBIME_CANDIDATE_FONT_SIZE,				// ����
		PT_TEXT_ALIGN_CENTER);					// �ı�����

	nCandNum  = pEngineOutputInfo->nCandNum - nCandNum;

	candStartPoint.y += (iKeyBoardHeight / 2);

	PT_CreateTouchRegion(&pltThis->candidateRegion,		// ��ѡ���ִ�������
		candStartPoint,		// ��ѡ������ʾ�Ŀ�ʼλ��
		(GBUINT16)(iKeyBoardWidth - 10),	// ��ѡ�����
		(GBUINT16)(iKeyBoardHeight / 2),	// ��ѡ���߶�
		pEngineOutputInfo->ppCandidates + (pEngineOutputInfo->nCandNum - nCandNum),	// ��ѡ����
		nCandNum,	// ��ѡ����
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
static void CLayoutKB__SetSyllablePageButtonRegion(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo)
{
	CLASE_THIS_POITER(CLayoutKB, pltObj)

	GBRECT buttonRect;
	PGBRECT pTargetRect;
	GBUINT16 iCandidateBoxHeight = 0;
	GBUINT16 iKeyBoardHeight = 0;
	GBUINT16 iKeyBoardWidth = 0;
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
	iKeyBoardWidth = pltBase->m_pfcGetLayoutWidth(pltObj, iCurrentLayoutId);
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
static void CLayoutKB__SetCandidatePageButtonRegion(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo)
{
	CLASE_THIS_POITER(CLayoutKB, pltObj)

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
 * \brief ����layout��Ϣ
 * \param pltObj 
 * \param pEngineOutputInfo 
 * \return 
 * \note 
 * \example
 * \author Zhaokun
 * \date 2010-10-22 11:46:08
*/
static void CLayoutKB__UpdateLayoutInformation(GBLPVOID pltObj, 
													   ButtonStatusEnum buttonStatus,	
													   PEngineOutputInfo pEngineOutputInfo)
{
	CLASE_THIS_POITER(CLayoutKB, pltObj)
		
	GBUINT16 iKeyBoardHeight = 0;
	GBUINT16 iCurrentLayoutId;
	
	// ��ǰLayoutId
	iCurrentLayoutId = GET_LAYOUT_ID(pltBase);
	// ��ȡ���̸߶�
	iKeyBoardHeight = pltBase->m_pfcGetLayoutHeight(pltObj,iCurrentLayoutId);
	
	
	if (buttonStatus == ButtonStatusDown)
	{
		// �򿪺�ѡ��
		if (pEngineOutputInfo->bCandWindOpen)
		{
			pltBase->m_pfcChangeHeight(pltObj, iKeyBoardHeight);
		}

		if (pEngineOutputInfo->isMultiTapInputMode && pEngineOutputInfo->bCandWindOpen
			&& !pEngineOutputInfo->isSymbolCandType/*GBEngine_IsSymbolCandType(Global_GetEnineInstance())*/)
		{
			GBINT nCandIndex = 0;
			nCandIndex = pEngineOutputInfo->pRowStart[pEngineOutputInfo->nSelRow] 
				+ pEngineOutputInfo->nSelCol;

			PT_LayerLock();
			if (pEngineOutputInfo->pUpScreenStr && pEngineOutputInfo->pUpScreenStr[0]) // ��������
			{
				GBInputBox_MultitapInputConfirm();
				pltBase->multitapReplaceCandLength = 0;
				pltBase->currentCandidateIndex = 0;
				//pltBase->m_pfcUpScreen(pltObj, pEngineOutputInfo->pUpScreenStr);
				//pEngineOutputInfo->pUpScreenStr[0] = 0;				
// 				GBInputBox_ClearHilight();
// 				GBInputBox_Redraw();
			}

			PT_PrintLogInfo(GBLOG_LAYOUTCONTROL,
				("\n��ѡ������%d��col��%d��row��%d����ѡ��%s\n"
				,pEngineOutputInfo->nCandNum, pEngineOutputInfo->nSelCol
				, pEngineOutputInfo->nSelRow, pEngineOutputInfo->ppCandidates[pEngineOutputInfo->nSelCol]));

			pltBase->multitapReplaceCandLength =
				GBInputBox_ReplaceHighlightString(pltBase->multitapReplaceCandLength,
				pEngineOutputInfo->ppCandidates[nCandIndex]);
			PT_LayerUnLock();

//			GBInputBox_MultiTapInsertChar(pEngineOutputInfo->ppCandidates[nCandIndex]);
// 			PT_GUIStartTimer(GBIME_MULTITAP_TIMER_INTERVAL, pltBase->m_pfcOnMultitapTimer);
			pltBase->bMultitapTimerStartFlag = GBTrue;
			return ;
		}
		if (pEngineOutputInfo->isMultiTapInputMode 
			&& pltBase->bMultitapTimerStartFlag
			&& pEngineOutputInfo->pUpScreenStr && pEngineOutputInfo->pUpScreenStr[0]) // ��������
		{
			pltBase->multitapReplaceCandLength =
				GBInputBox_ReplaceHighlightString(pltBase->multitapReplaceCandLength,
				pEngineOutputInfo->pUpScreenStr);
			GBInputBox_MultitapInputConfirm();
			pltBase->currentCandidateIndex = 0;
			pltBase->multitapReplaceCandLength = 0;
		}
		else if (/*!pEngineOutputInfo->isMultiTapInputMode && */pEngineOutputInfo->pUpScreenStr && pEngineOutputInfo->pUpScreenStr[0]) // ��������
		{			
			pltBase->currentCandidateIndex = 0;
			pltBase->m_pfcUpScreen(pltObj, pEngineOutputInfo->pUpScreenStr);
		}

		// ���봮Ϊ��ʱ�رպ�ѡ��[����ģʽû�к�ѡ��]
		if (!pEngineOutputInfo->bCandWindOpen)
		{
			pltBase->currentSyllableIndex = 0; // ������������
			Global_SetLayoutWinActiveStatus(GBFalse);
			GBInputBox_SetAllPlatformFuncKey();
			pltBase->m_pfcChangeHeight(pltObj, 0);
			return;
		}
	}
}

#endif // �����������