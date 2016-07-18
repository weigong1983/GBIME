/***************************************************************************
* File Name    :  CLayoutVKNumber.c
* Corporation  :  Guobi Technology Ltd. GuangZhou China
* Copyright    :  Copyright Guobi Technology Ltd. And Subject to Confidentiality 
*                 Restrictions. This document is the confidential and proprietary 
*                 property of Guobi Technology Ltd.It is neither the document                  
*                 itself nor any part of it can be shared with any party  without 
*                 the written  permission. 
* Author       :  haj
* Date         :  2010-06-08
* Purpose      :  ��������Layout
* Version      :  V1.0
* Last Changed List
* Name         |   date   |  Discription
****************************************************************************/
#if defined(__GBIME_VK__) // �����������

#include "CLayoutVKNumber.h"
#include "CLayoutViewVKNumber.h"

/*!
 * \brief ���ظ����Ա��������
 */
GBIMELOCAL GBIMEReturn CLayoutVKNumber__Release(GBLPVOID pltObj, PLayoutInfo pltUIData);
GBIMELOCAL GBIMEReturn CLayoutVKNumber__Init(PCLayoutVKNumber pltVK);
GBIMELOCAL GBIMEReturn CLayoutVKNumber__Paint(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo);
GBIMELOCAL GBIMEReturn CLayoutVKNumber__PreHandleEvent(GBLPVOID pltObj, GBINT nPlatformEvent, GBINT wParam, GBINT lParam, PEngineOutputInfo pEngineOutputInfo);
GBIMELOCAL GBIMEReturn CLayoutVKNumber__HandleEvent(GBLPVOID pltObj, PGBIMEEvent pIMEEvent, PEngineOutputInfo pEngineOutputInfo);
GBIMELOCAL GBIMEReturn CLayoutVKNumber__DisableFuncKeyHandler(GBLPVOID pltObj);
GBIMELOCAL GBBOOL CLayoutVKNumber__IsInputKey(GBUINT16 keyValue);

/*!
 * \brief ���غ�������
 */
GBIMELOCAL void ICLayoutVKNumber__ProcessLayoutPenDown(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo);
GBIMELOCAL void ICLayoutVKNumber__ProcessLayoutPenUp(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo);
GBIMELOCAL PenPosition ICLayoutVKNumber__HitTest(GBLPVOID pltObj, 
									  GBIMEPoint hitPoint, 
									  ButtonStatusEnum buttonStatus, 
									  PEngineOutputInfo pEngineOutputInfo);
GBIMELOCAL void ICLayoutVKNumber__UpdateLayoutInformation(GBLPVOID pltObj, 
											   ButtonStatusEnum buttonStatus,
											   PEngineOutputInfo pEngineOutputInfo);

CLayoutAbstract * CLayoutVKNumber__Construct(PCLayoutVKNumber pltVK, 
											 GBLPCVOID pltUIData,
											 GBLPCVOID pltIMData,
											 GBLPCVOID pltConfigData)
{
	CLayoutVK__Construct(&pltVK->m_ltParent,
		pltUIData,
		pltIMData,
		pltConfigData);
	CLayoutVKNumber__Init(pltVK);

	return (CLayoutAbstract *)pltVK;
}

GBIMELOCAL GBIMEReturn CLayoutVKNumber__Release(GBLPVOID pltObj, PLayoutInfo pltUIData)
{
	CLASE_THIS_POITER(CLayoutVKNumber, pltObj)

	pltRoot->m_pfcReleaseBase(pltObj, pltUIData);
	return GBIME_OK;
}

GBIMELOCAL GBIMEReturn CLayoutVKNumber__Init(PCLayoutVKNumber pltVK)
{
	PCLayoutViewVKNumber pLtView = NULL;

	// �����Ӧ����ͼ����
	pLtView = (PCLayoutViewVKNumber)PT_Malloc(sizeof(CLayoutViewVKNumber));
	PT_Assert(pLtView != NULL);
	memset(pLtView, 0, sizeof(CLayoutViewVKNumber));
	CLayoutViewVKNumber__Construct(pLtView);

	pltVK->m_ltParent.m_ltParent.m_hltView = (PCLayoutViewAbstract)pLtView;
	pltVK->m_ltParent.m_ltParent.m_pfcPaint = CLayoutVKNumber__Paint;
	pltVK->m_ltParent.m_ltParent.m_pfcPreHandleEvent = CLayoutVKNumber__PreHandleEvent;
	pltVK->m_ltParent.m_ltParent.m_pfcHandleEvent = CLayoutVKNumber__HandleEvent;
	pltVK->m_ltParent.m_ltParent.m_pfcRelease = CLayoutVKNumber__Release;
	pltVK->m_ltParent.m_ltParent.m_pfcIsInputKey = CLayoutVKNumber__IsInputKey;
	pltVK->m_ltParent.m_ltParent.m_pfcDisableFuncKeyHandler = CLayoutVKNumber__DisableFuncKeyHandler;
	
	return GBIME_OK;
}

GBIMELOCAL GBIMEReturn CLayoutVKNumber__Paint(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo)
{
	CLASE_THIS_POITER(CLayoutVKNumber, pltObj)

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
*/
GBIMELOCAL GBIMEReturn CLayoutVKNumber__PreHandleEvent(GBLPVOID pltObj, GBINT nPlatformEvent, GBINT wParam, GBINT lParam, PEngineOutputInfo pEngineOutputInfo)
{
	CLASE_THIS_POITER(CLayoutVKNumber, pltObj)

	PLayoutInfo pLayoutInfo = (PLayoutInfo)(&pltRoot->m_ltData); // Layout UI data point
	GBIMEPoint hitPoint;
	GBINT32 hitButtonIndex = -1;
	PenPosition penPositionInWhere;
	GBUINT16 targetLayoutID = LayoutIdSP9JPinYin;
	GBIMEReturn ret;

	// ��ƽ̨�������¼�ת����IME�����Ҫ������¼���	
	hitPoint.x = wParam;
	hitPoint.y = lParam;
	//hitButtonIndex = GBLayoutButtonHitTest(pltKB, &hitPoint);
	pltRoot->bSendEngineKeyEvent = GBFalse;
	pltRoot->engineKeyEvent = wParam; // ���������ҪLayout�����ɾ�������水��ֵ
	pltRoot->engineEventParam = lParam; // ���������ҪLayout���ݾ��幦�ܰ�ť���н���
	
	switch(nPlatformEvent)
	{
	case GBPEVT_PEN_DOWN:		
		pltRoot->m_pfcSlidingPenDownHandler(pltObj, hitPoint);
		penPositionInWhere = ICLayoutVKNumber__HitTest(pltObj, hitPoint, ButtonStatusDown, pEngineOutputInfo);
		if (pltRoot->m_pfcShouldSendMsgToEngine(pltObj, ButtonStatusDown))
		{
			pltRoot->bSendEngineKeyEvent = GBTrue;
			pltRoot->engineKeyEvent = CLayoutInfo_ConversionLayoutKeyToEngineKey(pltRoot->currentLayoutKeyValue);
		}

		// ��������
		pltRoot->m_pfcLongPressKeyHandler(pltObj, pEngineOutputInfo);

		// ����Ԥ�������ж��Ƿ������水���¼�
		if (pltRoot->bSendEngineKeyEvent)
		{
			Global_FillEventGroup(GBIME_CT_NORMAL_KEY_DOWN, wParam, lParam);
		} 
		else
		{
			Global_FillEventGroup(GBIME_CT_KEY_DOWN_REFRESH, wParam, lParam);
		}
		break;
	case GBPEVT_PEN_UP:		
		// ����ǻ����л�Layout��ֱ�ӽ�������
		if ((ret = pltRoot->m_pfcSlidingPenUpHandler(pltObj, hitPoint)) != GBIME_IGNORE)
		{
			return ret;
		}
		
		penPositionInWhere = ICLayoutVKNumber__HitTest(pltObj, hitPoint, ButtonStatusUp, pEngineOutputInfo);

		// ����������
		if ((ret = pltRoot->m_pfcLongPressKeyHandler(pltObj, pEngineOutputInfo)) != GBIME_IGNORE)
		{
			break; // ��������
		}

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
		
		if (penPositionInWhere != PenPositionInNothing)
		{
			// �����������ʱ���º͵����ֵһ�²�����Ӧ
			if (pltRoot->pressLayoutKeyValue == pltRoot->currentLayoutKeyValue)
			{
				if ((ret = pltRoot->m_pfcSwitchHandler(pltObj)) != GBIME_IGNORE) // Layout�л�����
				{				
					break;
				}
				else if ((ret = pltRoot->m_pfcFuncKeyHandler(pltObj, pEngineOutputInfo)) != GBIME_IGNORE) // ���ܼ�����
				{
					break;
				}
			}			
			Global_FillEventGroup(GBIME_CT_KEY_UP_REFRESH, wParam, lParam);
		}
	    break;
	case GBPEVT_PEN_MOVE:
		pltRoot->m_pfcSlidingPenMoveHandler(pltObj, hitPoint);
		// ��������Move��Ϣ������Layout��ť״̬
		penPositionInWhere = ICLayoutVKNumber__HitTest(pltObj, hitPoint, ButtonStatusMove, pEngineOutputInfo);
		Global_FillEventGroup(GBIME_CT_MOVE, wParam, lParam);
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

GBIMELOCAL GBIMEReturn CLayoutVKNumber__HandleEvent(GBLPVOID pltObj, PGBIMEEvent pIMEEvent, PEngineOutputInfo pEngineOutputInfo)
{
	CLASE_THIS_POITER(CLayoutVKNumber, pltObj)
	PLayoutInfo pLayoutInfo = (PLayoutInfo)(&pltRoot->m_ltData); // Layout UI data point
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

	// ��������Layoutʱ����ƽ̨������Ƿ��ڴ���������ģʽ�����Ƿ����ĳЩ���ܰ�ť
// 	if (pIMEEvent->nEvent == GBIME_EVENT_LAYOUT_ACTIVATE)
// 	{
// 		if (GBInputBox_IsDigitInputOnly())
// 		{
// 			CLayoutInfo_SetButtonStatus(pLayoutInfo, GBKEY_VK_BACKWORD, ButtonStatusGray);
// 			pltRoot->isSlidingDisabled = GBTrue; // ��ֹ�����л�
// 		}
// 		else
// 		{
// 			CLayoutInfo_SetButtonStatus(pLayoutInfo, GBKEY_VK_BACKWORD, ButtonStatusNomal);
// 			pltRoot->isSlidingDisabled = GBFalse; // �������л�
// 		}
// 	}

	// ������Ϣ����
	if ((ret = pltRoot->m_pfcHandleCommonEvent(pltObj, pIMEEvent, pEngineOutputInfo)) != GBIME_IGNORE)
	{
		return ret;
	}

	// ��Ϣ�ַ�(���Layout��ť״̬���¡��������水���¼��������������Ϣ)
	switch (pIMEEvent->nEvent)
	{
	case GBIME_EVENT_LAYOUT_PEN_DOWN:
		ICLayoutVKNumber__ProcessLayoutPenDown(pltObj, pEngineOutputInfo);
		if (pltRoot->penInPostion != PenPositionInNothing)
		{
			pltRoot->m_refreshFlag = GBIME_LAYOUT_REFRESH_KEYBOARD;
		}
		break;
	case GBIME_EVENT_LAYOUT_PEN_UP:
		ICLayoutVKNumber__ProcessLayoutPenUp(pltObj, pEngineOutputInfo);
		if (pltRoot->penInPostion != PenPositionInNothing)
		{
			pltRoot->m_refreshFlag = GBIME_LAYOUT_REFRESH_KEYBOARD;
		}
		// �����ʷ����ֵ
		pltRoot->pressLayoutKeyValue = pltRoot->currentLayoutKeyValue = GBEvent_None;
		pltRoot->penDownPostion = pltRoot->penInPostion = PenPositionInNothing;
		break;
	case GBIME_EVENT_LAYOUT_PEN_MOVE:
		if (pltRoot->penInPostion != PenPositionInNothing)
		{
			pltRoot->m_refreshFlag = GBIME_LAYOUT_REFRESH_KEYBOARD;
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
	
	return GBIME_OK;
}

/*!
 * \brief ƽ̨ĳЩ����ģʽ��Ҫ����ĳЩ���ܰ�ť(��: ���ٲ���ģʽ��ֹ��������)
 * \param pltObj 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-12-30 13:45:08
 */
GBIMELOCAL GBIMEReturn CLayoutVKNumber__DisableFuncKeyHandler(GBLPVOID pltObj)
{
	CLASE_THIS_POITER(CLayoutVKNumber, pltObj)
	PLayoutInfo pLayoutInfo = (PLayoutInfo)(&pltRoot->m_ltData); // Layout UI data point
	GBIMEReturn ret;

	if (GBInputBox_IsDigitInputOnly())
	{
		CLayoutInfo_SetButtonStatus(pLayoutInfo, GBKEY_VK_BACKWORD, ButtonStatusGray);
		pltRoot->isSlidingDisabled = GBTrue; // ��ֹ�����л�
	}
	else
	{
		CLayoutInfo_SetButtonStatus(pLayoutInfo, GBKEY_VK_BACKWORD, ButtonStatusNomal);
		pltRoot->isSlidingDisabled = GBFalse; // �������л�
	}

	return GBIME_OK;
}

/*!
 * \brief �������ʰ����¼�
 * \param pltObj 
 * \param pEngineOutputInfo 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-8-17 16:47:32
*/
static void ICLayoutVKNumber__ProcessLayoutPenDown(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo)
{
	ICLayoutVKNumber__UpdateLayoutInformation(pltObj, ButtonStatusDown, pEngineOutputInfo);
}

/*!
 * \brief ���������ɿ��¼�
 * \param pltObj 
 * \param pEngineOutputInfo 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-8-17 16:47:49
*/
static void ICLayoutVKNumber__ProcessLayoutPenUp(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo)
{
	ICLayoutVKNumber__UpdateLayoutInformation(pltObj, ButtonStatusUp, pEngineOutputInfo);
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
static PenPosition ICLayoutVKNumber__HitTest(GBLPVOID pltObj, 
									 GBIMEPoint hitPoint, 
									 ButtonStatusEnum buttonStatus, 
									 PEngineOutputInfo pEngineOutputInfo)
{
	CLASE_THIS_POITER(CLayoutVKNumber, pltObj)

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
static void ICLayoutVKNumber__UpdateLayoutInformation(GBLPVOID pltObj, ButtonStatusEnum buttonStatus,	PEngineOutputInfo pEngineOutputInfo)
{
	CLASE_THIS_POITER(CLayoutVKNumber, pltObj)

	GBUINT16 iCandidateBoxHeight = 0;
	GBUINT16 iKeyBoardHeight = 0;
	GBUINT16 iCurrentLayoutId;

	// ��ǰҪ��ʾ�Ĳ�
	GBUINT16 iCurrentLayerNumber;
	GBIMEPoint winPos;

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

	// �����������������º͵����ֵ��һ�²�����Ӧ
	if (pltRoot->penInPostion == PenPositionInKeyboardButton)
	{
		pltRoot->m_ltData.ltCurLayer = FirstLayer;

		if (buttonStatus == ButtonStatusUp
			&& pltRoot->pressLayoutKeyValue == pltRoot->currentLayoutKeyValue)
		{
			if (pltRoot->m_pfcIsInputKey(pltRoot->currentLayoutKeyValue))
			{
				GBUINT16 strNumber[2];
				strNumber[0] = CLayoutInfo_ConversionLayoutKeyToEngineKey(pltRoot->currentLayoutKeyValue);
				strNumber[1] = 0;
				pltRoot->m_pfcUpScreen(pltObj, strNumber);
			}
		}
	}
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
GBBOOL CLayoutVKNumber__IsInputKey(GBUINT16 keyValue)
{
	if ((keyValue >= GBKEY_0) && (keyValue <= GBKEY_9))
	{
		return GBTrue;
	}
	else if(keyValue >= GBKEY_VK_V9_SYMBOL_ADD && keyValue <= GBKEY_VK_V9_SYMBOL_FULL_STOP)
	{
		return GBTrue;
	}

	return GBFalse;
}

#endif // �����������