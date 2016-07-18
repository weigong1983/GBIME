/***************************************************************************
* File Name    :  CLayoutVKSymbol.c
* Corporation  :  Guobi Technology Ltd. GuangZhou China
* Copyright    :  Copyright Guobi Technology Ltd. And Subject to Confidentiality 
*                 Restrictions. This document is the confidential and proprietary 
*                 property of Guobi Technology Ltd.It is neither the document                  
*                 itself nor any part of it can be shared with any party  without 
*                 the written  permission. 
* Author       :  haj
* Date         :  2010-06-08
* Purpose      :  �������Layout
* Version      :  V1.0
* Last Changed List
* Name         |   date   |  Discription
****************************************************************************/
#if defined(__GBIME_VK__) // �����������

#include "CLayoutVKSymbol.h"
#include "CLayoutViewVKSymbol.h"

//�ж��Ƿ��ǽ��з����л�
#define IS_CHANGE_SYMBOL(val) ((val == GBKEY_VK_SYM_comment)\
							||(val == GBKEY_VK_SYM_EMOTION)\
							||(val == GBKEY_VK_SYM_SPECIAL)\
							||(val == GBKEY_VK_SYM_MATH))
//�ж��Ƿ���ѡ�������
#define IS_CHOOSE_SYMBOL(val) ((val >= GBKEY_VK_1_NUMBERKEY) && (val <= GBKEY_VK_9_NUMBERKEY))
//�ж��Ƿ������Ϸ�ҳ
#define IS_SYMBOL_PAGE_UP(val) (val == GBKEY_VK_SYM_PAGEUP)
//�ж��Ƿ��������·�ҳ
#define IS_SYMBOL_PAGE_DOWN(val) (val == GBKEY_VK_SYM_PAGEDOWN)

extern const GBUINT16 symbolCommonChnDataArray[];
extern const GBUINT16 symbolCommonEnDataArray[];
extern const GBUINT16 symbolFaceDataArray[];
extern const GBUINT16 symbolEspecialDataArray[];
extern const GBUINT16 symbolMathDataArray[];

/*!
 * \brief ���ظ����Ա��������
 */
GBIMELOCAL GBIMEReturn CLayoutVKSymbol__Init(PCLayoutVKSymbol pltVKSymbol);
GBIMELOCAL GBIMEReturn CLayoutVKSymbol__Paint(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo);
GBIMELOCAL GBIMEReturn CLayoutVKSymbol__PreHandleEvent(GBLPVOID pltObj, GBINT nPlatformEvent, GBINT wParam, GBINT lParam, PEngineOutputInfo pEngineOutputInfo);
GBIMELOCAL GBIMEReturn CLayoutVKSymbol__HandleEvent(GBLPVOID pltObj, PGBIMEEvent pIMEEvent, PEngineOutputInfo pEngineOutputInfo);

/*!
 * \brief ���غ�������
 */
GBIMELOCAL PenPosition ICLayoutVKSymbol_HitTest(GBLPVOID pltObj, 
									 GBIMEPoint hitPoint,
									 ButtonStatusEnum buttonStatus,
									 PEngineOutputInfo pEngineOutputInfo);
GBIMELOCAL void ICLayoutVKSymbol_UpdateLayoutInformation(GBLPVOID pltObj, 
											  ButtonStatusEnum buttonStatus, 
											  PEngineOutputInfo pEngineOutputInfo);
GBIMELOCAL void ICLayoutVKSymbol__SetSymbol(GBLPVOID pltObj, GBUINT16 layoutKeyValue);
GBIMELOCAL void ICLayoutVKSymbol__InitSymbol(GBLPVOID pltObj, GbSymbolTypeEnum symbolType, GBLPCVOID pSymbolData);
GBIMELOCAL void ICLayoutVKSymbol__MoveToNextPageSymbol(GBLPVOID pltObj);
GBIMELOCAL void ICLayoutVKSymbol__MoveToPrvePageSymbol(GBLPVOID pltObj);
GBIMELOCAL GBUINT16* ICLayoutVKSymbol__GetSymbolByLayoutKeyValue(GBLPVOID pltObj, GBUINT16 layoutKeyValue);
GBIMELOCAL GBIMEReturn ICLayoutVKSymbol__FuncKeyHandler(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo);

CLayoutAbstract * CLayoutVKSymbol__Construct(PCLayoutVKSymbol pltVKSymbol,
											 GBLPCVOID pltUIData,
											 GBLPCVOID pltIMData,
											 GBLPCVOID pltConfigData)
{
	CLayoutVK__Construct(&pltVKSymbol->m_ltParent,
		pltUIData,
		pltIMData,
		pltConfigData);
	CLayoutVKSymbol__Init(pltVKSymbol);

	return (CLayoutAbstract *)pltVKSymbol;
}

GBIMELOCAL GBIMEReturn CLayoutVKSymbol__Init(PCLayoutVKSymbol pltVKSymbol)
{
	PCLayoutViewVKSymbol pLtView = NULL;

	// �����Ӧ����ͼ����
	pLtView = (PCLayoutViewVKSymbol)PT_Malloc(sizeof(CLayoutViewVKSymbol));
	PT_Assert(pLtView != NULL);
	memset(pLtView, 0, sizeof(CLayoutViewVKSymbol));
	CLayoutViewVKSymbol__Construct(pLtView);

	pltVKSymbol->m_ltParent.m_ltParent.m_hltView = (PCLayoutViewAbstract)pLtView;
	pltVKSymbol->m_ltParent.m_ltParent.m_pfcPaint = CLayoutVKSymbol__Paint;
	pltVKSymbol->m_ltParent.m_ltParent.m_pfcPreHandleEvent = CLayoutVKSymbol__PreHandleEvent;
	pltVKSymbol->m_ltParent.m_ltParent.m_pfcHandleEvent = CLayoutVKSymbol__HandleEvent;
	pltVKSymbol->m_ltParent.m_ltParent.m_pfcFuncKeyHandler = ICLayoutVKSymbol__FuncKeyHandler;
	pltVKSymbol->m_bSymbolLock = GBFalse;

	// �������Ի�����ʼ��ȫ��/��Ƿ���
	if (GBEngine_IsChineseInputMode(Global_GetEnineInstance()))
	{
		pltVKSymbol->m_commonSymbolType = FullShaped;
	}
	else
	{
		pltVKSymbol->m_commonSymbolType = HalfShaped;
	}
	ICLayoutVKSymbol__SetSymbol(pltVKSymbol,GBKEY_VK_SYM_comment);
	return GBIME_OK;
}

GBIMELOCAL GBIMEReturn CLayoutVKSymbol__Paint(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo)
{
	CLASE_THIS_POITER(CLayoutVKSymbol, pltObj)

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
 3. �ض����ܵĲ���ת���ο���

	// ��ѡ��ز���
	�����һҳ�ĺ�ѡ: wParam����Ϊ��GBKEY_NEXT_PAGE_CANDIATE��
	�����һҳ�ĺ�ѡ: wParam����Ϊ��GBKEY_PREV_PAGE_CANDIATE��
	ѡ����������һ����ѡ: wParam����Ϊ��GBKEY_NEXT_CANDIDATE��
	ѡ������ǰ��һ����ѡ: wParam����Ϊ��GBKEY_PREV_CANDIDATE��
	ѡ��һ����ѡ: wParam����Ϊ��GBEVENT_SELECT_CANDIDATE��; lParam �����ĵ�16λ��ʾѡ����У���16λ��ʾѡ�����

	// ������ز���
	�����һҳ��ƴ��: wParam����Ϊ��GBKEY_NEXT_PAGE_SYLLABLE��
	�����һҳ��ƴ��: wParam����Ϊ��GBKEY_PREV_PAGE_SYLLABLE��
	�ѵ�ǰѡ���ƴ�������ƶ�һ��: wParam����Ϊ��GBKEY_NEXT_SYLLABLE��
	�ѵ�ǰѡ���ƴ����ǰ�ƶ�һ��: wParam����Ϊ��GBKEY_PREV_SYLLABLE��
	ѡ��ǰ��ҳ������: wParam����Ϊ��GBEVENT_SELECT_SYLLABLE_PAGED��; lParam ������ЧֵΪ0����ǰҳ���ڵĸ���
 */
GBIMELOCAL GBIMEReturn CLayoutVKSymbol__PreHandleEvent(GBLPVOID pltObj, GBINT nPlatformEvent, GBINT wParam, GBINT lParam, PEngineOutputInfo pEngineOutputInfo)
{
	CLASE_THIS_POITER(CLayoutVKSymbol, pltObj)

	PLayoutInfo pLayoutInfo = (PLayoutInfo)(&pltRoot->m_ltData); // Layout UI data point
	GBIMEPoint hitPoint;
	GBINT32 hitButtonIndex = -1;
	PenPosition penPositionInWhere;
	//PCLayoutAbstract pltPreObject = Global_GetPrevLayoutObject();
	GBIMEReturn ret;

	// ��ƽ̨�������¼�ת����IME�����Ҫ������¼���	
	hitPoint.x = wParam;
	hitPoint.y = lParam;
	//hitButtonIndex = GBLayoutButtonHitTest(pltKB, &hitPoint);

	pltRoot->bSendEngineKeyEvent = GBFalse;
	pltRoot->engineKeyEvent = wParam; // ���������ҪLayout�����ɾ�������水��ֵ
	pltRoot->engineEventParam = lParam; // ���������ҪLayout���ݾ��幦�ܰ�ť���н���
	
	switch (nPlatformEvent)
	{
	case GBPEVT_PEN_DOWN:
		pltRoot->m_pfcSlidingPenDownHandler(pltObj, hitPoint);
		penPositionInWhere = ICLayoutVKSymbol_HitTest(pltObj, hitPoint, ButtonStatusDown, pEngineOutputInfo);
		
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

		penPositionInWhere = ICLayoutVKSymbol_HitTest(pltObj, hitPoint, ButtonStatusUp, pEngineOutputInfo);

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
		penPositionInWhere = ICLayoutVKSymbol_HitTest(pltObj, hitPoint, ButtonStatusMove, pEngineOutputInfo);
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

GBIMELOCAL GBIMEReturn CLayoutVKSymbol__HandleEvent(GBLPVOID pltObj, PGBIMEEvent pIMEEvent, PEngineOutputInfo pEngineOutputInfo)
{
	CLASE_THIS_POITER(CLayoutVKSymbol, pltObj)
	
	// ��Ϣ�ַ�(���Layout��ť״̬���¡��������水���¼��������������Ϣ)
	switch (pIMEEvent->nEvent)
	{
	case GBIME_EVENT_LAYOUT_PEN_DOWN:
		break;
	case GBIME_EVENT_LAYOUT_PEN_UP:
		ICLayoutVKSymbol_UpdateLayoutInformation(pltObj, ButtonStatusUp, pEngineOutputInfo);
		// �����ʷ����ֵ
		pltRoot->pressLayoutKeyValue = pltRoot->currentLayoutKeyValue = GBEvent_None;
		pltRoot->penDownPostion = pltRoot->penInPostion = PenPositionInNothing;
		break;
	case GBIME_EVENT_LAYOUT_PEN_MOVE:
		break;
	case GBIME_EVENT_KEY_DOWN: // ������ֱ��ˢ��	
	case GBIME_EVENT_KEY_UP:
	case GBIME_EVENT_KEY_LONG_PRESS:
		pltRoot->m_refreshFlag = (GBIME_LAYOUT_REFRESH_CANDIDATE_LINE | GBIME_LAYOUT_REFRESH_SYLLABLE_LINE);
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
		// �������Ի����������÷���
		if (GBEngine_IsChineseInputMode(Global_GetEnineInstance()))
		{
			pltThis->m_commonSymbolType = FullShaped;
		}
		else
		{
			pltThis->m_commonSymbolType = HalfShaped;
		}
		ICLayoutVKSymbol__SetSymbol(pltObj, GBKEY_VK_SYM_comment);
		//Global_SetLayoutWinHeight(0); // �߶����㣬���Layout���л����ڸ߶�һ��û��ˢ��VK
		pltRoot->m_pfcChangeHeight(pltObj, pltRoot->m_pfcGetLayoutHeight(pltObj, pltRoot->m_ltData.pltcData->ltID));
		//pltRoot->m_pfcInitVariable(pltObj);
		break;
	case GBIME_EVENT_LAYOUT_EXIT:		
		Global_SetLayoutWinHeight(0); // �߶����㣬���Layout���л����ڸ߶�һ��û��ˢ��VK
		pltRoot->m_pfcInitVariable(pltObj);
		break;
	default:
		break;
	}
	
	return GBIME_OK;
}

/*!
 * \brief ���ݷ��ŵ���ţ�ȡ�÷����������еĿ�ʼ�±�
 * \param pltObj 
 * \param symbolIndex 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-8-19 9:27:47
*/
GBUINT16 CLayoutVKSymbol__GetSymbolOffsetByIndex(GBLPVOID pltObj, GBUINT16 symbolIndex)
{
	CLASE_THIS_POITER(CLayoutVKSymbol, pltObj)

	GBUINT16 iOffset = 0;
	GBUINT16 iCurrentSymbolIndex = 0;
	GBUINT16 iCount = 0;
	GBUINT16 *pSymbolData;

	if (pltThis->m_pltSymbolData != NULL)
	{
		pSymbolData = (GBUINT16*)pltThis->m_pltSymbolData;
		iCount = 0;
		iCurrentSymbolIndex = 0;

		while (pSymbolData[iCount] != SYMBOL_END_FLAG)
		{
			if (iCurrentSymbolIndex == symbolIndex)
			{
				iOffset = iCount;
				return iOffset;
			}
			if (pSymbolData[iCount] == 0)
			{
				iCurrentSymbolIndex++;
			}
			iCount++;
		}
	}

	return iOffset;
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
 * \date 2010-8-19 9:28:40
*/
static PenPosition ICLayoutVKSymbol_HitTest(GBLPVOID pltObj, 
										  GBIMEPoint hitPoint,
										  ButtonStatusEnum buttonStatus,
										  PEngineOutputInfo pEngineOutputInfo)
{
	CLASE_THIS_POITER(CLayoutVKSymbol, pltObj)
	PenPosition penPositionInWhere = PenPositionInNothing;

	// �жϴ������Ƿ����ڼ��̰�ť��
	penPositionInWhere = pltRoot->m_pfcPenPositionInKeyboardButton(pltObj, hitPoint, buttonStatus, pEngineOutputInfo);
	return penPositionInWhere;
}

/*!
 * \brief ����ͼ����Ϣ
 * \param pltObj 
 * \param buttonStatus 
 * \param pEngineOutputInfo 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-8-19 9:28:31
*/
static void ICLayoutVKSymbol_UpdateLayoutInformation(GBLPVOID pltObj, 
												   ButtonStatusEnum buttonStatus, 
												   PEngineOutputInfo pEngineOutputInfo)
{
	CLASE_THIS_POITER(CLayoutVKSymbol, pltObj)

	GBUINT16 iCandidateBoxHeight = 0;
	GBUINT16 iKeyBoardHeight = 0;
	GBUINT16 iCurrentLayoutId;
	GBIMEPoint winPos;

	// ��ǰLayoutId
	iCurrentLayoutId = GET_LAYOUT_ID(pltRoot);
	// ��ȡ���̸߶�
	iKeyBoardHeight = pltRoot->m_pfcGetLayoutHeight(pltObj, iCurrentLayoutId);
	// ȡ�ü�����ʾ�Ŀ�ʼλ�á����½ǡ���������Ϊ�����Ͻǡ�
	Global_GetWinPos(&winPos);	
	winPos.y -=  iKeyBoardHeight;

	//pltRoot->m_pfcChangeHeight(pltObj, iKeyBoardHeight);

	if (pltRoot->penInPostion == PenPositionInKeyboardButton)
	{
		if (buttonStatus == ButtonStatusUp
			&& pltRoot->pressLayoutKeyValue == pltRoot->currentLayoutKeyValue) //�����ɿ�ʱ
		{
			if (IS_CHANGE_SYMBOL(pltRoot->currentLayoutKeyValue)) // ���л�����ʱ
			{
				ICLayoutVKSymbol__SetSymbol(pltObj, pltRoot->currentLayoutKeyValue);
			}
			else if (IS_CHOOSE_SYMBOL(pltRoot->currentLayoutKeyValue)) // ��ѡ�����ʱ
			{
				GBUINT16 *pSymbol;
				pSymbol = ICLayoutVKSymbol__GetSymbolByLayoutKeyValue(pltObj, pltRoot->currentLayoutKeyValue);
				if (pSymbol)
				{
					pltRoot->m_pfcUpScreen(pltObj, pSymbol);
					if (!pltThis->m_bSymbolLock && pltRoot->m_pfcSwitchPreLayoutObj != NULL)
					{
						pltRoot->m_pfcSwitchPreLayoutObj(pltObj);
					}
				}
			}
			else if (IS_SYMBOL_PAGE_UP(pltRoot->currentLayoutKeyValue))//��һҳ
			{
				ICLayoutVKSymbol__MoveToPrvePageSymbol(pltObj);
			}
			else if (IS_SYMBOL_PAGE_DOWN(pltRoot->currentLayoutKeyValue))//��һҳ
			{
				ICLayoutVKSymbol__MoveToNextPageSymbol(pltObj);
			}
		}
		return;
	}
}

/*!
 * \brief ����layout��ֵ�����÷�������
 * \param pltObj 
 * \param layoutKeyValue 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-8-19 9:28:21
*/
static void ICLayoutVKSymbol__SetSymbol(GBLPVOID pltObj, GBUINT16 layoutKeyValue)
{
	CLASE_THIS_POITER(CLayoutVKSymbol, pltObj)

	switch(layoutKeyValue)
	{		
	case GBKEY_VK_SYM_comment: // ��ͨ����
		if (pltThis->m_commonSymbolType == FullShaped)
		{
			ICLayoutVKSymbol__InitSymbol(pltObj, SymbolTypeCommon, (GBLPCVOID)symbolCommonChnDataArray);
		}
		else if (pltThis->m_commonSymbolType == HalfShaped)
		{
			ICLayoutVKSymbol__InitSymbol(pltObj, SymbolTypeCommon, (GBLPCVOID)symbolCommonEnDataArray);
		}
		break;		
	case GBKEY_VK_SYM_EMOTION: // �������
		ICLayoutVKSymbol__InitSymbol(pltObj, SymbolTypeFace, (GBLPCVOID)symbolFaceDataArray);
		break;		
	case GBKEY_VK_SYM_SPECIAL: // �������
		ICLayoutVKSymbol__InitSymbol(pltObj, SymbolTypeEspecial, (GBLPCVOID)symbolEspecialDataArray);
		break;		
	case GBKEY_VK_SYM_MATH: // ��ѧ����
		ICLayoutVKSymbol__InitSymbol(pltObj, SymbolTypeNumber, (GBLPCVOID)symbolMathDataArray);
		break;
	default:
		ICLayoutVKSymbol__InitSymbol(pltObj, SymbolTypeCommon, (GBLPCVOID)symbolCommonChnDataArray);
		break;
	}
}

/*!
 * \brief ��ʼ�����Ż�������
 * \param pltObj 
 * \param symbolType 
 * \param pSymbolData 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-8-19 9:28:13
*/
static void ICLayoutVKSymbol__InitSymbol(GBLPVOID pltObj, GbSymbolTypeEnum symbolType, GBLPCVOID pSymbolData)
{
	CLASE_THIS_POITER(CLayoutVKSymbol, pltObj)

	GBUINT16 *pSymbol;
	GBUINT16 iCount;
	GBUINT16 iSingCount;

	PT_Assert(pSymbolData != NULL);

	pSymbol = (GBUINT16 *)pSymbolData;
	pltThis->m_pltSymbolData = pSymbolData;
	pltThis->m_symbolType = symbolType;
	pltThis->m_iCurrentPageStartIndex = 0;
	pltThis->m_iCurrentChooseSymbolIndex = 0;
	pltThis->m_bHasNextPage = 0;
	pltThis->m_bHasPrvePage = 0;
	pltThis->m_hasFullOrHalfSymbol = 0;
	//��ͨ����ʱ�а�Ǻ�ȫ��
	if (symbolType == SymbolTypeCommon)
	{
		pltThis->m_hasFullOrHalfSymbol = 1;
	}
	iCount = 0;
	iSingCount = 0;

	// ������Ÿ���
	while (pSymbol[iCount] != SYMBOL_END_FLAG)
	{
		if (pSymbol[iCount] == 0) // ����һ�����Ž���ʱ�����Ÿ�������1
		{
			iSingCount++;
		}
		iCount++;
	}

	if (iSingCount >= 1)
	{
		// �õ��ܵķ��Ÿ���
		pltThis->m_iSymbolTotalCount = iSingCount;
		
		if (iSingCount > SYMBOL_COUNT_PER_PAGE) // ���Ÿ�������һҳ�ĸ���ʱ
		{
			pltThis->m_iCurrentPageEndIndex = SYMBOL_COUNT_PER_PAGE - 1;
			pltThis->m_bHasNextPage = 1;
		}
		else
		{
			pltThis->m_iCurrentPageEndIndex = iSingCount - 1;
		}
	}
}

/*!
 * \brief �Ƶ���һҳ����
 * \param pltObj 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-8-19 9:28:05
*/
static void ICLayoutVKSymbol__MoveToNextPageSymbol(GBLPVOID pltObj)
{
	CLASE_THIS_POITER(CLayoutVKSymbol, pltObj)

	// �������һҳ
	if (pltThis->m_bHasNextPage)
	{
		// ��һҳ�ĵ�һ�����ż��ǵ�ǰҳ���һ�����ŵĺ���
		pltThis->m_iCurrentPageStartIndex = pltThis->m_iCurrentPageEndIndex + 1;
		pltThis->m_bHasPrvePage = 1;
	}

	// ���û����һҳ
	if ((pltThis->m_iCurrentPageStartIndex + SYMBOL_COUNT_PER_PAGE) >= pltThis->m_iSymbolTotalCount)
	{
		pltThis->m_bHasNextPage = 0;
		pltThis->m_iCurrentPageEndIndex = pltThis->m_iSymbolTotalCount - 1;
	}
	else
	{
		pltThis->m_bHasNextPage = 1;
		pltThis->m_iCurrentPageEndIndex = pltThis->m_iCurrentPageStartIndex + SYMBOL_COUNT_PER_PAGE - 1;
	}
}

/*!
 * \brief �Ƶ���һҳ����
 * \param pltObj 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-8-19 9:27:55
*/
static void ICLayoutVKSymbol__MoveToPrvePageSymbol(GBLPVOID pltObj)
{
	CLASE_THIS_POITER(CLayoutVKSymbol, pltObj)

	// �������һҳ,���з�ҳ
	if (pltThis->m_bHasPrvePage)
	{
		// ��һҳ�����һ�������ǵ�ǰҳ�ĵ�һ����ǰ��
		pltThis->m_iCurrentPageEndIndex = pltThis->m_iCurrentPageStartIndex - 1;
		pltThis->m_iCurrentPageStartIndex = pltThis->m_iCurrentPageEndIndex - SYMBOL_COUNT_PER_PAGE + 1;
		
	}

	// ���������һҳ
	if (pltThis->m_iCurrentPageStartIndex > 0)
	{
		pltThis->m_bHasPrvePage = 1;
	}
	else
	{
		pltThis->m_bHasPrvePage = 0;
	}

	// �������һҳ
	if (pltThis->m_iCurrentPageEndIndex < pltThis->m_iSymbolTotalCount - 1)
	{
		pltThis->m_bHasNextPage = 1;
	}
	else
	{
		pltThis->m_bHasNextPage = 0;
	}
}

/*!
 * \brief ���ݼ��̵ļ�ֵ��ȡ�ö�Ӧ�ķ���
 * \param pltObj 
 * \param layoutKeyValue 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-8-19 9:27:34
*/
static GBUINT16* ICLayoutVKSymbol__GetSymbolByLayoutKeyValue(GBLPVOID pltObj, GBUINT16 layoutKeyValue)
{
	CLASE_THIS_POITER(CLayoutVKSymbol, pltObj)

	GBUINT16 *pSymbolData;
	GBUINT16 iSymbolOffset;
	GBUINT16 iCurrentChooseIndex = 0;

	// ÿҳ9�����ţ���1,2,3....9�⼸�����ּ���,�жϸü�ֵ�Ƿ���ѡ����ż�
	if (IS_CHOOSE_SYMBOL(layoutKeyValue))
	{
		pSymbolData = (GBUINT16 *)pltThis->m_pltSymbolData;
		// ��ǰҳ�ĵ�һ�������ڷ��������е����
		iCurrentChooseIndex = pltThis->m_iCurrentPageStartIndex;
		// ѡ�еķ����ڷ��������е����
		iCurrentChooseIndex += (layoutKeyValue - GBKEY_VK_1_NUMBERKEY);
		// ȡ�ø÷����������е�ƫ��λ��
		iSymbolOffset = CLayoutVKSymbol__GetSymbolOffsetByIndex(pltObj, iCurrentChooseIndex);
		//iSymbolOffset=0ʱ�������������һ�����Ż򲻴��ڵķ���
		if ((iSymbolOffset == 0)&&(pltThis->m_iCurrentPageStartIndex != 0))//�����ڵķ���
		{
			return NULL;
		}
		return pSymbolData + iSymbolOffset;
	}

	return NULL;
}
//��ͨȫ�����ķ���
const GBUINT16 symbolCommonChnDataArray[] = 
{
	0xff0c,0x0000,//��
	0x3002,0x0000,//��
	0xff1f,0x0000,//��
	0xff01,0x0000,//��
	0x3001,0x0000,//��
	0xff1a,0x0000,//��
	0xff1b,0x0000,//��
	0x2026,0x2026,0x0000,//����
	0x0040,0x0000,//��
	0x201c,0x201d,0x0000,//����
	0xfe61,0x0000,//�~
	0xff03,0x0000,//�|
	0x2018,0x2019,0x0000,//�� ��
	0xff3f,0x0000,//��
	0x2014,0x0000,//��
	0xff06,0x0000,//��
	0xff0f,0x0000,//�M
	0xff3c,0x0000,//��
	0x0024,0x0000,//$
	0xffe5,0x0000,//��
	0xff08,0xff09,0x0000,//����
	0x300a,0x300b,0x0000,//����
	0xff3b,0xff3d,0x0000,//�ۣ�
	0xff5b,0xff5d,0x0000,//����
	0x3010,0x3011,0x0000,//����
	0x3016,0x3017,0x0000,//����
	0xff62,0xff63,0x0000,//����
	0xff5c,0x0000,//��
	0xff5e,0x0000,//��
	0xff0b,0x0000,//��
	0xff0d,0x0000,//��
	0x00d7,0x0000,//��
	0x00f7,0x0000,//��
	0xff1d,0x0000,//��
	0x0025,0x0000,//%
	0x2103,0x0000,//��

	//���ӷ���ʱ��������һ��֮ǰ����[���źͷ���֮����0x0000���]
	SYMBOL_END_FLAG	
};
//��ͨ���Ӣ�ķ���
const GBUINT16 symbolCommonEnDataArray[] = 
{
	0x002c,0x0000,//,
	0x002e,0x0000,//.
	0x003f,0x0000,//?
	0x0021,0x0000,//!
	0x0027,0x0000,//'
	0x003a,0x0000,//:
	0x003b,0x0000,//;
	0x2026,0x0000,//��
	0x0040,0x0000,//@
	0x0022,0x0022,0x0000,//""
	0x002a,0x0000,//*
	0x0023,0x0000,//#
	0x0060,0x0000,//`
	0x005f,0x0000,//_
	0x2014,0x0000,//��
	0x0026,0x0000,//&
	0x002f,0x0000,///
	0x005c,0x0000,//'\'
	0x0024,0x0000,//$
	0x00a5,0x0000,//��
	0x0028,0x0029,0x0000,//()
	0x003c,0x003e,0x0000,//<>
	0x005b,0x005d,0x0000,//[]
	0x007b,0x007d,0x0000,//{}
	0x3010,0x3011,0x0000,//����
	0x3016,0x3017,0x0000,//����
	0xff62,0xff63,0x0000,//??
	0x007c,0x0000,//|
	0x007e,0x0000,//~
	0x002b,0x0000,//+
	0x002d,0x0000,//-
	0x00d7,0x0000,//��
	0x00f7,0x0000,//��
	0x003d,0x0000,//=
	0x0025,0x0000,//%
	0x2103,0x0000,//��

	//���ӷ���ʱ��������һ��֮ǰ����[���źͷ���֮����0x0000���]
	SYMBOL_END_FLAG	
};

//�������
const GBUINT16 symbolFaceDataArray[] = 
{
	0x003a,0x002d,0x0029,0x0000,//:-)
	0x003b,0x002d,0x0029,0x0000,//;-)
	0x003a,0x002d,0x0028,0x0000,//:-(
	0x003a,0x002d,0x0044,0x0000,//:-D
	0x003a,0x002d,0x0070,0x0000,//:-p
	0x002d,0x002e,0x002d,0x007c,0x007c,0x0000,//-.-||
	0x003a,0x002d,0x0078,0x0000,//:-x
	0x003a,0x002d,0x004f,0x0000,//:-O
	0x003a,0x002d,0x002a,0x0000,//:-*
	0x0038,0x0038,0x0036,0x007e,0x0000,//886~
	0x0035,0x0035,0x0035,0x007e,0x0000,//555~
	0x004f,0x0072,0x007a,0x0000,//Orz
	0x0048,0x0061,0x0068,0x0061,0x007e,0x0000,//Haha~
	0x0048,0x0065,0x0068,0x0065,0x007e,0x0000,//Hehe~
	0x0054,0x005f,0x0054,0x0000,//T_T
	0x0024,0x005f,0x0024,0x0000,//$_$
	0x2192,0x005f,0x2192,0x0000,//��_��
	0x003f,0x005f,0x003f,0x0000,//?_?
	0xfe36,0xfe3f,0xfe36,0x0000,//����
	0x005e,0x25ce,0x002d,0x0000,//^��-
	0x002d,0xfe35,0x002d,0x0000,//-��-
	0x003e,0x0033,0x003c,0x0000,//>3<
	0x003e,0x005f,0x003c,0x0000,//>_<
	0x0040,0x005f,0x0040,0x0000,//@_@
	0xffe3,0xfe41,0xffe3,0x0000,//�����
	0x2299,0x002e,0x2299,0x0000,//��.��
	0x0028,0x2299,0x006f,0x2299,0x0029,0x0000,//(��o��)
	0x003c,0x203b,0x0000,//<��
	0x005e,0x006f,0x005e,0x0079,0x0000,//^o^y
	0x005e,0x005f,0x007e,0x0000,//^_~
	0x002a,0x005e,0x006f,0x005e,0x002a,0x0000,//*^o^*
	0x005e,0x002e,0x0020,0x002e,0x005e,0x0000,//^. .^
	0x005e,0x005f,0x005e,0x0000,//^_^
	0x0023,0x005e,0x005f,0x005e,0x0000,//#^_^
	0x005e,0x0028,0x006f,0x006f,0x0029,0x005e,0x0000,//^(oo)^
	0x006f,0x005f,0x0030,0x0000,//o_0

	//���ӷ���ʱ��������һ��֮ǰ����[���źͷ���֮����0x0000���]
	SYMBOL_END_FLAG		
};
//��ַ����
const GBUINT16 symbolEspecialDataArray[] = 
{
	0x0068,0x0074,0x0074,0x0070,0x003a,0x002f,0x002f,0x0000,//http://
	0x0077,0x0077,0x0077,0x002e,0x0000,//www.
	0x0077,0x0061,0x0070,0x002e,0x0000,//wap.
	0x002e,0x0063,0x006f,0x006d,0x0000,//.com
	0x002e,0x0063,0x006e,0x0000,//.cn
	0x002e,0x0063,0x006f,0x006d,0x002e,0x0063,0x006e,0x0000,//.com.cn
	0x002e,0x006e,0x0065,0x0074,0x0000,//.net
	0x002e,0x006f,0x0072,0x0067,0x0000,//.org
	0x002e,0x006e,0x0065,0x0074,0x002e,0x0063,0x006e,0x0000,//.net.cn
	0x002e,0x0065,0x0064,0x0075,0x0000,//.edu
	0x002e,0x0069,0x006e,0x0066,0x006f,0x0000,//.info
	0x002e,0x006f,0x0072,0x0067,0x002e,0x0063,0x006e,0x0000,//.org.cn
	0x0062,0x0062,0x0073,0x002e,0x0000,//bbs.
	0x002e,0x0063,0x0063,0x0000,//.cc
	0x0033,0x0067,0x002e,0x0000,//3g.
	0x0077,0x0061,0x0070,0x0073,0x002e,0x0000,//waps.
	0x002e,0x006d,0x006f,0x0062,0x0069,0x0000,//.mobi
	0x002e,0x006e,0x0061,0x006d,0x0065,0x0000,//.name


	//���ӷ���ʱ��������һ��֮ǰ����[���źͷ���֮����0x0000���]
	SYMBOL_END_FLAG
};
//�������
const GBUINT16 symbolMathDataArray[] = 
{
	0x2116,0x0000,//��
	0x00b0,0x0000,//��
	0x00a7,0x0000,//��
	0x00a2,0x0000,//��
	0x00a8,0x0000,//��
	0x203b,0x0000,//��
	0x00a4,0x0000,//��
	0x00a9,0x0000,//?
	0x00ae,0x0000,//?
	0x2640,0x0000,//��
	0x2642,0x0000,//��
	0x2312,0x0000,//��
	0xffe9,0x0000,//?
	0xffeb,0x0000,//?
	0x221a,0x0000,//��
	0xffec,0x0000,//?
	0xffea,0x0000,//?
	0x00b6,0x0000,//?
	0x2121,0x0000,//�Y
	0x2122,0x0000,//?
	0x32a3,0x0000,//�I
	0x25c6,0x0000,//��
	0x25c7,0x0000,//��
	0x25b3,0x0000,//��
	0x2605,0x0000,//��
	0x2606,0x0000,//��
	0x25b2,0x0000,//��
	0x263f,0x0000,//?
	0x2641,0x0000,//?
	0x306e,0x0000,//��
	0x25cf,0x0000,//��
	0x25cb,0x0000,//��
	0x2661,0x0000,//?
	0x25a0,0x0000,//��
	0x25a1,0x0000,//��
	0x2665,0x0000,//?

	//���ӷ���ʱ��������һ��֮ǰ����[���źͷ���֮����0x0000���]
	SYMBOL_END_FLAG	
};

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
static GBIMEReturn ICLayoutVKSymbol__FuncKeyHandler(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo)
{
	CLASE_THIS_POITER(CLayoutVKSymbol, pltObj)
	GBUINT16 tempStr[2];

	switch (pltRoot->currentLayoutKeyValue)
	{
	case GBKEY_VK_SBC_DBC_SWICH: ///<ȫ����л���	
		if (pltThis->m_hasFullOrHalfSymbol)
		{
			pltThis->m_commonSymbolType = (pltThis->m_commonSymbolType == FullShaped) ? HalfShaped : FullShaped;
			if (pltThis->m_symbolType == SymbolTypeCommon)
			{
				ICLayoutVKSymbol__SetSymbol(pltObj, (GBUINT16)GBKEY_VK_SYM_comment);
			}
		}
		break;
	case GBKEY_VK_SYM_LOCK: ///<����������
		pltThis->m_bSymbolLock = (pltThis->m_bSymbolLock) ? GBFalse : GBTrue;
		break;
	case GBKEY_SPACE: // ������ʱ��������ո񡿼�������ո�����ʱ��������ո񡿼�����ѡ��������
		tempStr[0] = GBKEY_SPACE;
		tempStr[1] = 0;
		pltRoot->m_pfcUpScreen(pltObj, tempStr);
		break;
	case GBKEY_BACK:
		GBInputBox_DeleteChar();
		break;
	default:
		return GBIME_IGNORE;
		break;
	}

	Global_FillEventGroup(GBIME_CT_KEY_UP_REFRESH, 0, 0); // ��ť����ˢ��
	return GBIME_OK;
}

#endif // �����������