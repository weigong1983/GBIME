/***************************************************************************
* File Name    :  CLayoutViewVK26Stroke.h
* Corporation  :  Guobi Technology Ltd. GuangZhou China
* Copyright    :  Copyright Guobi Technology Ltd. And Subject to Confidentiality 
*                 Restrictions. This document is the confidential and proprietary 
*                 property of Guobi Technology Ltd.It is neither the document                  
*                 itself nor any part of it can be shared with any party  without 
*                 the written  permission. 
* Author       :  haj
* Date         :  2010-06-08
* Purpose      :  ����QWERTY�ʻ�Layout��ͼ��
* Version      :  V1.0
* Last Changed List
* Name         |   date   |  Discription
****************************************************************************/
#if defined(__GBIME_VK__) // �����������

#include "CLayoutViewVK26Stroke.h"
#include "CLayoutVK26Stroke.h"
#include "PlatformAdapter.h"
#include "GB_Global.h"
#include "GBIMEResDef.h"

/*!
 * \ Layout View ָ��Ԥ����
 */
#define CLASS_VIEW_THIS_POITER(CLASS_NAME, pltThis)   \
	CLASS_NAME * pltViewThis = (CLASS_NAME *)pltThis->m_ltParent.m_ltParent.m_hltView; \
	PCLayoutViewVK pltViewBase = &pltViewThis->m_ltParent; \
	PCLayoutViewAbstract pltViewRoot = &pltViewThis->m_ltParent.m_ltParent;

GBIMELOCAL GBIMEReturn CLayoutViewVK26Stroke__Paint(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo);

// ���غ�������
GBIMELOCAL void ICLayoutViewVK26Stroke__DrawCandidateLine(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo);
GBIMELOCAL void ICLayoutViewVK26Stroke__DrawAssociateLine(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo);

CLayoutViewAbstract * CLayoutViewVK26Stroke__Construct(PCLayoutViewVK26Stroke pltViewVK)
{
	CLayoutViewVK__Construct(&pltViewVK->m_ltParent);
	CLayoutViewVK26Stroke__Init(pltViewVK);
	return (CLayoutViewAbstract *)pltViewVK;
}


GBIMEReturn CLayoutViewVK26Stroke__Init(PCLayoutViewVK26Stroke pltViewVK)
{
	pltViewVK->m_ltParent.m_ltParent.m_pfcPaint = CLayoutViewVK26Stroke__Paint;
	return GBIME_OK;
}

GBIMELOCAL GBIMEReturn CLayoutViewVK26Stroke__Paint(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo)
{
	CLASE_THIS_POITER(CLayoutVK26Stroke, pltObj)
	CLASS_VIEW_THIS_POITER(CLayoutViewVK26Stroke, pltThis)

	PLayoutInfo pLayoutInfo = (PLayoutInfo)(&pltRoot->m_ltData); // Layout UI data point
	GBRECT layoutWinRect = {0};

	if (pltRoot->m_refreshFlag == GBIME_LAYOUT_REFRESH_NONE)
	{
		return GBIME_OK;
	}

	PT_LayerLock();

	// ˢ������
	if ((pltRoot->m_refreshFlag & GBIME_LAYOUT_REFRESH_ALL) == GBIME_LAYOUT_REFRESH_ALL)
	{
		pltViewBase->m_pfcDrawKeyboard(pltObj, pEngineOutputInfo);
		pltViewBase->m_pfcDrawComponents(pltObj, pEngineOutputInfo);
	} 
	// ˢ����������
	else if ((pltRoot->m_refreshFlag & GBIME_LAYOUT_REFRESH_KEYBOARD) == GBIME_LAYOUT_REFRESH_KEYBOARD)
	{
		pltViewBase->m_pfcDrawKeyboard(pltObj, pEngineOutputInfo);
		pltViewBase->m_pfcDrawComponents(pltObj, pEngineOutputInfo);
	}
	// ˢ�º�ѡ��
	if ((pltRoot->m_refreshFlag & GBIME_LAYOUT_REFRESH_CANDIDATE_LINE) == GBIME_LAYOUT_REFRESH_CANDIDATE_LINE)
	{
		switch (pLayoutInfo->ltCurLayer)
		{
		case FirstLayer:
			break;
		case SecondLayer:
			ICLayoutViewVK26Stroke__DrawCandidateLine(pltObj, pEngineOutputInfo);
			break;
		case ThirdLayer:
			ICLayoutViewVK26Stroke__DrawAssociateLine(pltObj, pEngineOutputInfo);
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

/*!
 * \brief ���ڶ����ѡ��[��������״̬]
 * \param pltObj 
 * \param pEngineOutputInfo 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-8-19 10:31:36
*/
static void ICLayoutViewVK26Stroke__DrawCandidateLine(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo)
{
	CLASE_THIS_POITER(CLayoutVK26Stroke, pltObj)
	CLASS_VIEW_THIS_POITER(CLayoutViewVK26Stroke, pltThis)

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
	GBUINT16 iCandidateBoxHeight = 0;
	GBRECT screenRect,imageRect;
	GBRECT buttonRect;
	GBIMEPoint winPos;
	GBUINT16 iCurrentLayoutId;
	GBUINT16 invertKeyValue;
	GBBOOL bInvertFlag = GBFalse;

	// �õ�ָ���ѡ���Layout
	pLayerConstInfo = pltRoot->m_ltData.pltLayerData[SecondLayer].plycData;

	if (pLayerConstInfo == NULL)
	{
		return;
	}

	// ��ǰLayoutId
	iCurrentLayoutId = GET_LAYOUT_ID(pltRoot);
	// ��ȡ���̸߶�
	iKeyBoardHeight = pltRoot->m_pfcGetLayoutHeight(pltObj,iCurrentLayoutId);
	// ȡ�ü�����ʾ�Ŀ�ʼλ�á����½ǡ���������Ϊ�����Ͻǡ�
	Global_GetWinPos(&winPos);	
	winPos.y -=  iKeyBoardHeight;
	
	// ȡ��ѡ��߶�
	iCandidateBoxHeight = pltRoot->m_pfcGetCandidateBoxHeight(pltObj,iCurrentLayoutId,SecondLayer);
	// ����õ���ѡ�����Ͻ�Y����
	winPos.y -= iCandidateBoxHeight;
	
	iCandidateNomalImageId = pLayerConstInfo->lyImageID + IMG_GBIME_SYSTEM;//����ͼƬID��ַ;
	iCandidateDownImageId = pLayerConstInfo->lyBKImageID + IMG_GBIME_SYSTEM;//����ͼƬID��ַ;
	
	// ��ʾͼƬ�����Ͻ�X����
	iDisplayImageLeftX = (GBUINT16)winPos.x;
	// ��ʾͼƬ�����Ͻ�Y����
	iDisplayImageTopY = (GBUINT16)winPos.y;

	// ��ʾ����ͼƬ
	PT_DrawImage(iDisplayImageLeftX,iDisplayImageTopY,iCandidateNomalImageId);
	
	// ���¼�ͷ��ť״̬
	pltViewBase->m_pfcRedrawArrowButton(pltObj, iDisplayImageLeftX, iDisplayImageTopY, SecondLayer, pEngineOutputInfo);	
	
	// �ж��Ƿ���з�ҳ��ť
	if(pltRoot->penInPostion == PenPositionInSyllablePageUp && pEngineOutputInfo->bSyllableArrowLeft)
	{
		invertKeyValue = GBKEY_PREV_PAGE_SYLLABLE;
		bInvertFlag = GBTrue;
	}
	else if(pltRoot->penInPostion == PenPositionInSyllablePageDown && pEngineOutputInfo->bSyllableArrowRight)
	{
		invertKeyValue = GBKEY_NEXT_PAGE_SYLLABLE;
		bInvertFlag = GBTrue;
	}
	else if(pltRoot->penInPostion == PenPositionInCandidatePageUp && pEngineOutputInfo->bCandArrowUp)
	{
		invertKeyValue = GBKEY_PREV_PAGE_CANDIATE;
		bInvertFlag = GBTrue;
	}
	else if (pltRoot->penInPostion == PenPositionInCandidatePageDown && pEngineOutputInfo->bCandArrowDown)
	{
		invertKeyValue = GBKEY_NEXT_PAGE_CANDIATE;
		bInvertFlag = GBTrue;
	}
		
	// ���Է�ҳ��ť
	if (bInvertFlag && pltRoot->buttonStatus == ButtonStatusDown)
	{

		// ��ȡ��ť����
		pltRoot->m_pfcGetBottonRectByKey(pltObj, pltRoot->m_ltData.pltcData->ltID, SecondLayer, invertKeyValue,	&buttonRect);
		memcpy(&imageRect, &buttonRect, sizeof(GBRECT));		
		// ͼƬ����Ļ����ʾ��λ��
		memcpy(&screenRect, &imageRect, sizeof(GBRECT));
		MOVE_RECT(&screenRect, winPos.x, iDisplayImageTopY);		
		// ���Է�ҳ��ť
		PT_DrawImageWithClip(iCandidateDownImageId,&screenRect,&imageRect);
	}
	else
	{
		// ȡ��ҳ��ť�Ĵ�С[����ѡʱ�õ�]
		pltRoot->m_pfcGetBottonRectByKey(pltObj, 
										 pltRoot->m_ltData.pltcData->ltID, 
										 SecondLayer, 
										 GBKEY_PREV_PAGE_CANDIATE,	
										 &buttonRect);
	}
	// ��ӡLog��Ϣ
	PT_PrintLogInfo(GBLOG_LAYOUTVIEW, ("***********************ICLayoutViewVK26Stroke__DrawCandidateLine() Draw Candidate image\n"));
	PT_PrintLogInfo(GBLOG_LAYOUTVIEW, ("***********************Candidate Image ID = %d\n", iCandidateNomalImageId));

	// ���Ʊʻ�
	//pltViewBase->m_pfcDrawStroke(pltObj, pEngineOutputInfo);
	pltViewBase->m_pfcDrawStrokeUpLine(pltObj, pEngineOutputInfo);// ����GB2312ƫ�ԡ��^����С�������ڴ�������ʾΪ���ڡ�
	// ���ƺ�ѡ��
	pltViewBase->m_pfcDrawCandidate(pltObj, pEngineOutputInfo);

	// �ػ�ڶ����������߼������ڵ�һ��İ�ť����ƴ���ָ�����...��
	pltViewBase->m_pfcRedrawKeyboardSpecialButton(pltObj);
}

/*!
 * \brief ���������ѡ��[������״̬]
 * \param pltObj 
 * \param pEngineOutputInfo 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-8-19 10:31:28
*/
static void ICLayoutViewVK26Stroke__DrawAssociateLine(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo)
{
	CLASE_THIS_POITER(CLayoutVK26Stroke, pltObj)
	CLASS_VIEW_THIS_POITER(CLayoutViewVK26Stroke, pltThis)

	GBUINT16 iCurrentLayoutId;
	//��ѡ����[Ŀǰ��һ�к�ѡ]
	GBUINT16 iCandidateRowCount = 1;
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
	GBUINT16 iCandidateBoxHeight = 0;
	GBIMEPoint winPos;
	GBRECT screenRect,imageRect;
	GBRECT buttonRect;
	GBUINT16 invertKeyValue;
	GBBOOL bInvertFlag = GBFalse;

	// �õ�ָ���ѡ���Layout
	pLayerConstInfo = pltRoot->m_ltData.pltLayerData[ThirdLayer].plycData;

	if (pLayerConstInfo != NULL)
	{
		// ��ǰLayoutId
		iCurrentLayoutId = GET_LAYOUT_ID(pltRoot);
		// ��ȡ���̸߶�
		iKeyBoardHeight = pltRoot->m_pfcGetLayoutHeight(pltObj,iCurrentLayoutId);
		// ȡ�ü�����ʾ�Ŀ�ʼλ�á����½ǡ���������Ϊ�����Ͻǡ�
		Global_GetWinPos(&winPos);	
		winPos.y -=  iKeyBoardHeight;
		// ȡ��ѡ��߶�
		iCandidateBoxHeight = pltRoot->m_pfcGetCandidateBoxHeight(pltObj,iCurrentLayoutId,ThirdLayer);
		// ����õ���ѡ�����Ͻ�Y����
		winPos.y -= iCandidateBoxHeight;

		iCandidateNomalImageId = pLayerConstInfo->lyImageID + IMG_GBIME_SYSTEM;//����ͼƬID��ַ;
		iCandidateDownImageId = pLayerConstInfo->lyBKImageID + IMG_GBIME_SYSTEM;//����ͼƬID��ַ;
		// ��ʾ[��ѡ]ͼƬ�����Ͻ�X����
		iDisplayImageLeftX = (GBUINT16)winPos.x;
		// ��ʾ[��ѡ]ͼƬ�����Ͻ�Y����
		iDisplayImageTopY = (GBUINT16)winPos.y;

		// ��ʾ����ͼƬ
		PT_DrawImage(iDisplayImageLeftX, iDisplayImageTopY, iCandidateNomalImageId);
		
		// ���¼�ͷ��ť״̬
		pltViewBase->m_pfcRedrawArrowButton(pltObj, iDisplayImageLeftX, iDisplayImageTopY, ThirdLayer, pEngineOutputInfo);	

		// �ж��Ƿ���з�ҳ��ť
		if (pltRoot->penInPostion == PenPositionInCandidatePageDown && pEngineOutputInfo->bCandArrowDown)
		{
			invertKeyValue = GBKEY_NEXT_PAGE_CANDIATE;
			bInvertFlag = GBTrue;
		}
		else if(pltRoot->penInPostion == PenPositionInCandidatePageUp && pEngineOutputInfo->bCandArrowUp)
		{
			invertKeyValue = GBKEY_PREV_PAGE_CANDIATE;
			bInvertFlag = GBTrue;
		}

		// ���Է�ҳ��ť
		if (bInvertFlag && pltRoot->buttonStatus == ButtonStatusDown)
		{
			// ��ȡ��ť����
			pltRoot->m_pfcGetBottonRectByKey(pltObj, pltRoot->m_ltData.pltcData->ltID, ThirdLayer, invertKeyValue, &buttonRect);		
			memcpy(&imageRect, &buttonRect, sizeof(GBRECT));		
			// ͼƬ����Ļ����ʾ��λ��
			memcpy(&screenRect, &imageRect, sizeof(GBRECT));
			MOVE_RECT(&screenRect, winPos.x, iDisplayImageTopY);					
			// ���Է�ҳ��ť
			PT_DrawImageWithClip(iCandidateDownImageId,&screenRect,&imageRect);
		}
		else
		{
			// ����ѡʱ�õ�
			pltRoot->m_pfcGetBottonRectByKey(pltObj,
											pltRoot->m_ltData.pltcData->ltID,
											ThirdLayer,
											GBKEY_PREV_PAGE_CANDIATE,
											&buttonRect);
		}

		// ��ӡLog��Ϣ
		PT_PrintLogInfo(GBLOG_LAYOUTVIEW, ("***********************ICLayoutViewVK26Stroke__DrawAssociateLine() Draw Candidate image\n"));
		PT_PrintLogInfo(GBLOG_LAYOUTVIEW, ("***********************Candidate Image ID = %d\n",iCandidateNomalImageId));

		// ���ƺ�ѡ��
		pltViewBase->m_pfcDrawCandidate(pltObj, pEngineOutputInfo);

		// �ػ�ڶ����������߼������ڵ�һ��İ�ť����ƴ���ָ�����...��
		pltViewBase->m_pfcRedrawKeyboardSpecialButton(pltObj);
	}

}

#endif // �����������