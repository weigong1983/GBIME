/***************************************************************************
* File Name    :  CLayoutKB9MultiLang.c
* Corporation  :  Guobi Technology Ltd. GuangZhou China
* Copyright    :  Copyright Guobi Technology Ltd. And Subject to Confidentiality 
*                 Restrictions. This document is the confidential and proprietary 
*                 property of Guobi Technology Ltd.It is neither the document                  
*                 itself nor any part of it can be shared with any party  without 
*                 the written  permission. 
* Author       :  ZhaoKun
* Date         :  2010-10-15
* Purpose      :  ����ż�ƴ��Layout
* Version      :  V1.0
* Last Changed List
* Name         |   date   |  Discription
****************************************************************************/
#if defined(__GBIME_KB__) // �����������

#include "CLayoutKB9MultiLang.h"
#include "CLayoutViewKB9MultiLang.h"



/*!
 * \brief ���ظ����Ա��������
 */
GBIMELOCAL GBIMEReturn CLayoutKB9MultiLang__Release(GBLPVOID pltObj, PLayoutInfo pltUIData);
GBIMELOCAL GBIMEReturn CLayoutKB9MultiLang__Init(PCLayoutKB9MultiLang pltKB9);
GBIMELOCAL GBIMEReturn CLayoutKB9MultiLang__Paint(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo);
GBIMELOCAL GBIMEReturn CLayoutKB9MultiLang__HandleEvent(GBLPVOID pltObj, PGBIMEEvent pIMEEvent, PEngineOutputInfo pEngineOutputInfo);

GBIMELOCAL void ICLayoutKB9MultiLang__SetRegion(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo); // ���ô�������


/*!
 * \brief ����ʵ��
 */
CLayoutAbstract * CLayoutKB9MultiLang__Construct(PCLayoutKB9MultiLang pltKB, 
											  GBLPCVOID pltUIData,
											  GBLPCVOID pltIMData,
											  GBLPCVOID pltConfigData)
{
	CLayoutKB__Construct(&pltKB->m_ltParent,
		pltUIData,
		pltIMData,
		pltConfigData);
	CLayoutKB9MultiLang__Init(pltKB);

	return (CLayoutAbstract *)pltKB;
}

GBIMELOCAL GBIMEReturn CLayoutKB9MultiLang__Release(GBLPVOID pltObj, PLayoutInfo pltUIData)
{
	CLASE_THIS_POITER(CLayoutKB9MultiLang, pltObj)

	pltRoot->m_pfcReleaseBase(pltObj, pltUIData);
	return GBIME_OK;
}

GBIMELOCAL GBIMEReturn CLayoutKB9MultiLang__Init(PCLayoutKB9MultiLang pltKB)
{
	PCLayoutViewKB9MultiLang pLtView = NULL;

	// �����Ӧ����ͼ����
	pLtView = (PCLayoutViewKB9MultiLang)PT_Malloc(sizeof(CLayoutViewKB9MultiLang));
	PT_Assert(pLtView != NULL);
	memset(pLtView, 0, sizeof(CLayoutViewKB9MultiLang));
	CLayoutViewKB9MultiLang__Construct(pLtView);

	pltKB->m_ltParent.m_ltParent.m_hltView = (PCLayoutViewAbstract)pLtView;
	pltKB->m_ltParent.m_ltParent.m_pfcPaint = CLayoutKB9MultiLang__Paint;
	pltKB->m_ltParent.m_ltParent.m_pfcHandleEvent = CLayoutKB9MultiLang__HandleEvent;
	pltKB->m_ltParent.m_ltParent.m_pfcRelease = CLayoutKB9MultiLang__Release;
	pltKB->m_ltParent.m_ltParent.currentSyllableIndex = 0;
	pltKB->m_ltParent.m_ltParent.currentCandidateIndex = 0;

	return GBIME_OK;
}

GBIMELOCAL GBIMEReturn CLayoutKB9MultiLang__Paint(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo)
{
	CLASE_THIS_POITER(CLayoutKB9MultiLang, pltObj)

		if (pltRoot->m_hltView != NULL 
			&& pltRoot->m_hltView->m_pfcPaint != NULL
			&& pEngineOutputInfo->bCandWindOpen
			&& Global_GetLayoutWinActiveStatus()) // ����״̬����ʾ
		{
			pltRoot->m_hltView->m_pfcPaint(pltObj, pEngineOutputInfo);
		}

		return GBIME_OK;
}

GBIMELOCAL GBIMEReturn CLayoutKB9MultiLang__HandleEvent(GBLPVOID pltObj, PGBIMEEvent pIMEEvent, PEngineOutputInfo pEngineOutputInfo)
{
	CLASE_THIS_POITER(CLayoutKB9MultiLang, pltObj)
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
	case GBIME_EVENT_KEY_DOWN:
		ICLayoutKB9MultiLang__SetRegion(pltObj, pEngineOutputInfo);
		pltBase->m_pfcUpdateLayoutInformation(pltObj, ButtonStatusDown, pEngineOutputInfo);
		pltRoot->m_refreshFlag |= (GBIME_LAYOUT_REFRESH_CANDIDATE_LINE | GBIME_LAYOUT_REFRESH_KEYBOARD);
		break;
	case GBIME_EVENT_KEY_UP:
		break;
	case GBIME_EVENT_KEY_LONG_PRESS:
		break;
	default:
		break;
	}

	return GBIME_OK;
}

/*!
 * \brief ��������
 * \param pltObj 
 * \param pEngineOutputInfo 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-10 11:30:08
*/
static void ICLayoutKB9MultiLang__SetRegion(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo)
{
	CLASE_THIS_POITER(CLayoutKB9MultiLang, pltObj)
	GBUINT16 iCurrentLayoutId;
	GBUINT16 iKeyBoardHeight = 0;
	GBUINT16 iKeyBoardWidth = 0;
	GBIMEPoint winPos;
	GBINT16 iScreenWidth = 0;
	
	// ��ǰLayoutId
	iCurrentLayoutId = GET_LAYOUT_ID(pltRoot);
	// ��ȡ���̸߶�
	iKeyBoardHeight = pltRoot->m_pfcGetLayoutHeight(pltObj,iCurrentLayoutId);
	iKeyBoardWidth = pltRoot->m_pfcGetLayoutWidth(pltObj, iCurrentLayoutId);
	// ȡ�ü�����ʾ�Ŀ�ʼλ�á����½ǡ���������Ϊ�����Ͻǡ�
	Global_GetWinPos(&winPos);	
	winPos.y -=  iKeyBoardHeight;
	
	iScreenWidth = PT_GetLcdWidth();
	// ����ƴ�ʻ���ʾ�Ŀ�ʼλ��
	//pltRoot->syllableDisplayStartPos.x = (iScreenWidth - iKeyBoardWidth) / 2 + 10;//GBIME_SYLLABLE_DISPLAY_START_X;
	//pltRoot->syllableDisplayStartPos.y = winPos.y;
	pltRoot->m_pfcSetSyllableDisplayStartPos(pltObj, (iScreenWidth - iKeyBoardWidth) / 2 + 10, winPos.y);
		
	// ���ú�ѡ��ʾ�Ŀ�ʼλ��
	//pltRoot->candidateDisplayStartPos.x = GBIME_CANDIDATE_DISPLAY_START_X;
	//pltRoot->candidateDisplayStartPos.y = winPos.y + iKeyBoardHeight / 2;// GBIME_CANDIDATE_LINE_HEIGHT - 2;
	pltRoot->m_pfcSetCandidateDisplayStartPos(pltObj, GBIME_CANDIDATE_DISPLAY_START_X, winPos.y + iKeyBoardHeight / 2);
	
	
	if (pEngineOutputInfo->isSymbolCandType
		&& (pEngineOutputInfo->pInputString[0] != L'1' && pEngineOutputInfo->pInputString[1] == 0
		&& pEngineOutputInfo->pInputString[0] != L'0' && pEngineOutputInfo->pInputString[1] == 0
		)
		/*GBEngine_IsSymbolCandType(Global_GetEnineInstance())*/
	/*	&& (pEngineOutputInfo->pOutputString && pEngineOutputInfo->pOutputString[0] == 0)*/)
	{
		pltBase->m_pfcSetSymbolCandRegion(pltObj, pEngineOutputInfo); // ������������
	}
	else
	{
		pltBase->m_pfcSetOutputStringRegion(pltObj, pEngineOutputInfo); // ������������
		pltBase->m_pfcSetCandidateRegion(pltObj, pEngineOutputInfo); // ��ѡ��������
	}
}

#endif // �����������