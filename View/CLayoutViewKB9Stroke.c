/***************************************************************************
* File Name    :  CLayoutViewKB9Stroke.h
* Corporation  :  Guobi Technology Ltd. GuangZhou China
* Copyright    :  Copyright Guobi Technology Ltd. And Subject to Confidentiality 
*                 Restrictions. This document is the confidential and proprietary 
*                 property of Guobi Technology Ltd.It is neither the document                  
*                 itself nor any part of it can be shared with any party  without 
*                 the written  permission. 
* Author       :  ZhaoKun
* Date         :  2010-10-15
* Purpose      :  ����ż��ʻ�Layout��ͼ��
* Version      :  V1.0
* Last Changed List
* Name         |   date   |  Discription
****************************************************************************/
#if defined(__GBIME_KB__) // �����������

#include "CLayoutViewKB9Stroke.h"
#include "CLayoutKB9Stroke.h"
#include "PlatformAdapter.h"
#include "GB_Global.h"
#include "GBIMEResDef.h"
/*!
 * \ Layout View ָ��Ԥ����
 */
#define CLASS_VIEW_THIS_POITER(CLASS_NAME, pltThis)   \
	CLASS_NAME * pltViewThis = (CLASS_NAME *)pltThis->m_ltParent.m_ltParent.m_hltView; \
	PCLayoutViewKB pltViewBase = &pltViewThis->m_ltParent; \
	PCLayoutViewAbstract pltViewRoot = &pltViewThis->m_ltParent.m_ltParent;

GBIMELOCAL GBIMEReturn CLayoutViewKB9Stroke__Paint(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo);

// ���غ�������
GBIMELOCAL void ICLayoutViewKB9Stroke__DrawCandidateLine(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo);

CLayoutViewAbstract * CLayoutViewKB9Stroke__Construct(PCLayoutViewKB9Stroke pltViewKB9)
{
	CLayoutViewKB__Construct(&pltViewKB9->m_ltParent);
	CLayoutViewKB9Stroke__Init(pltViewKB9);
	return (CLayoutViewAbstract *)pltViewKB9;
}

GBIMEReturn CLayoutViewKB9Stroke__Init(PCLayoutViewKB9Stroke pltViewKB9)
{
	pltViewKB9->m_ltParent.m_ltParent.m_pfcPaint = CLayoutViewKB9Stroke__Paint;
	return GBIME_OK;
}

GBIMELOCAL GBIMEReturn CLayoutViewKB9Stroke__Paint(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo)
{
 	CLASE_THIS_POITER(CLayoutKB9Stroke, pltObj)
 	CLASS_VIEW_THIS_POITER(CLayoutViewKB9Stroke, pltThis)
 
 	PLayoutInfo pLayoutInfo = (PLayoutInfo)(&pltRoot->m_ltData); // Layout UI data point
	GBRECT layoutWinRect = {0};

	if (pltRoot->m_refreshFlag == GBIME_LAYOUT_REFRESH_NONE)
	{
		return GBIME_OK;
	}

	PT_LayerLock();

	// ˢ������
//	if ((pltRoot->m_refreshFlag & GBIME_LAYOUT_REFRESH_ALL) == GBIME_LAYOUT_REFRESH_ALL)
//	{
//		pltViewBase->m_pfcDrawKeyboard(pltObj, pEngineOutputInfo);
//	}
	// ˢ����������
	if ((pltRoot->m_refreshFlag & GBIME_LAYOUT_REFRESH_KEYBOARD) == GBIME_LAYOUT_REFRESH_KEYBOARD)
	{
		pltViewBase->m_pfcDrawKeyboard(pltObj, pEngineOutputInfo);
	}
	// ˢ�º�ѡ��
	if ((pltRoot->m_refreshFlag & GBIME_LAYOUT_REFRESH_CANDIDATE_LINE) == GBIME_LAYOUT_REFRESH_CANDIDATE_LINE
		&& pEngineOutputInfo->bCandWindOpen)
	{
		switch (pLayoutInfo->ltCurLayer)
		{
		case FirstLayer:
			ICLayoutViewKB9Stroke__DrawCandidateLine(pltObj, pEngineOutputInfo);
			break;
		default:
			PT_Assert(0);
			break;
		}
	}
	PT_LayerUnLock();
	pltRoot->m_pfcGetLayoutWinRect(pltObj, &layoutWinRect);
	PT_Invalidate(layoutWinRect.left, layoutWinRect.top, layoutWinRect.right, layoutWinRect.bottom);

	return GBIME_OK;
}

static void ICLayoutViewKB9Stroke__DrawCandidateLine(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo)
{
	CLASE_THIS_POITER(CLayoutKB9Stroke, pltObj)
	CLASS_VIEW_THIS_POITER(CLayoutViewKB9Stroke, pltThis)

	//��ѡ����[Ŀǰ����]
	GBUINT16 iCandidateRowCount = 2;
	//��ѡ����
	//ָ���Ŀ�ʼλ��
	GBLPCLAYOUTCONSTLAYER pLayerConstInfo;	 
	//��ѡ������״̬�µ�ͼƬ
	GBUINT16 iCandidateNomalImageId;
	//��ѡ����״̬�µ�ͼƬ
	GBUINT16 iCandidateDownImageId;
	//��ʾͼƬ�����Ͻ�X����
	GBUINT16 iDisplayImageLeftX;
	//��ʾͼƬ�����Ͻ�Y����
	GBUINT16 iDisplayImageTopY;
	GBUINT16 iKeyBoardHeight = 0;
	GBUINT16 iKeyBoardWidth = 0;
	GBUINT16 iCandidateBoxHeight = 0;
	GBIMEPoint winPos;
	GBUINT16 iCurrentLayoutId;
	GBBOOL bInvertFlag = GBFalse;
	GBINT16 iScreenWidth = 0;

	// �õ�ָ���ѡ���Layout
	pLayerConstInfo = pltRoot->m_ltData.pltLayerData[FirstLayer].plycData;

	if (pLayerConstInfo == NULL)
	{
		return;
	}

	// ��ǰLayoutId
	iCurrentLayoutId = GET_LAYOUT_ID(pltRoot);
	// ��ȡ���̸߶�
	iKeyBoardHeight = pltRoot->m_pfcGetLayoutHeight(pltObj,iCurrentLayoutId);
	iKeyBoardWidth = pltRoot->m_pfcGetLayoutWidth(pltObj, iCurrentLayoutId);
	// ȡ�ü�����ʾ�Ŀ�ʼλ�á����½ǡ���������Ϊ�����Ͻǡ�
	Global_GetWinPos(&winPos);	
	winPos.y -=  iKeyBoardHeight;
	
	iScreenWidth = PT_GetLcdWidth();

// 	// ����ƴ�ʻ���ʾ�Ŀ�ʼλ��
// 	pltRoot->syllableDisplayStartPos.x = GBIME_SYLLABLE_DISPLAY_START_X;
// 	pltRoot->syllableDisplayStartPos.y = winPos.y - iCandidateBoxHeight;
// 
// 	// ���ú�ѡ��ʾ�Ŀ�ʼλ��
// 	pltRoot->candidateDisplayStartPos.x = GBIME_CANDIDATE_DISPLAY_START_X;
// 	pltRoot->candidateDisplayStartPos.y = winPos.y + iKeyBoardHeight / 2;// GBIME_CANDIDATE_LINE_HEIGHT - 2;


	// ȡ��ѡ��߶�
	iCandidateBoxHeight = pltRoot->m_pfcGetCandidateBoxHeight(pltObj,iCurrentLayoutId,FirstLayer);
	// ����õ���ѡ�����Ͻ�Y����
	winPos.y -= iCandidateBoxHeight;
	
	iCandidateNomalImageId = pLayerConstInfo->lyImageID + IMG_GBIME_SYSTEM;//����ͼƬID��ַ;
	iCandidateDownImageId = pLayerConstInfo->lyBKImageID + IMG_GBIME_SYSTEM;//����ͼƬID��ַ;
	
	// ��ʾͼƬ�����Ͻ�X����
	iDisplayImageLeftX = (GBUINT16)winPos.x + (iScreenWidth - iKeyBoardWidth) / 2;
	// ��ʾͼƬ�����Ͻ�Y����
	iDisplayImageTopY = (GBUINT16)winPos.y;

// 	// ��ʾ����ͼƬ
// 	PT_DrawImage(iDisplayImageLeftX ,iDisplayImageTopY,iCandidateNomalImageId);

	// ���¼�ͷ��ť״̬
	pltViewBase->m_pfcRedrawArrowButton(pltObj, iDisplayImageLeftX,	iDisplayImageTopY, FourthLayer, pEngineOutputInfo);	

	// ��ӡLog��Ϣ
	PT_PrintLogInfo(GBLOG_LAYOUTVIEW, ("***********************ICLayoutViewKB9Stroke__DrawCandidateLine() Draw Candidate image\n"));
	PT_PrintLogInfo(GBLOG_LAYOUTVIEW, ("***********************Candidate Image ID = %d\n", iCandidateNomalImageId));
	
	// �������봮�ͱʻ�
	if(pEngineOutputInfo->pStroke && pEngineOutputInfo->pStroke[0])
	{
		pltViewBase->m_pfcDrawStrokeUpLine(pltObj, pEngineOutputInfo);
	}
	else
	{
		pltViewBase->m_pfcDrawOutputString(pltObj, pEngineOutputInfo);
	}	
//	pltViewBase->m_pfcDrawStroke(pltObj, pEngineOutputInfo);
	if(!pEngineOutputInfo->isSymbolCandType/*GBEngine_IsSymbolCandType(Global_GetEnineInstance())*/)
	{
		pltViewBase->m_pfcDrawComponents(pltObj, pEngineOutputInfo);
	}
	pltViewBase->m_pfcDrawCandidate(pltObj, pEngineOutputInfo);
#ifdef __GBIME_LOG__
{
	GBINT i = 0;
	GBACHAR candBuf[128] = {0};
	PT_PrintLogInfo(GBLOG_LAYOUTVIEW, ("��ѡ��"));
	for (i = 0; i < pEngineOutputInfo->nCandNum; i++)
	{
		PT_wcstombs(candBuf, pEngineOutputInfo->ppCandidates[i], sizeof(candBuf));
		PT_PrintLogInfo(GBLOG_LAYOUTVIEW, ("%s,", candBuf));
	}
	PT_PrintLogInfo(GBLOG_LAYOUTVIEW, ("\r\n"));
}
#endif
}

#endif // �����������