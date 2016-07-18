/***************************************************************************
* File Name    :  CLayoutViewVK.h
* Corporation  :  Guobi Technology Ltd. GuangZhou China
* Copyright    :  Copyright Guobi Technology Ltd. And Subject to Confidentiality 
*                 Restrictions. This document is the confidential and proprietary 
*                 property of Guobi Technology Ltd.It is neither the document                  
*                 itself nor any part of it can be shared with any party  without 
*                 the written  permission. 
* Author       :  haj
* Date         :  2010-06-08
* Purpose      :  �������Layout��ͼ��
* Version      :  V1.0
* Last Changed List
* Name         |   date   |  Discription
****************************************************************************/
#if defined(__GBIME_VK__) // �����������

#include "CLayoutViewVK.h"
#include "CLayoutVK.h"
#include "PlatformAdapter.h"
#include "GB_Global.h"
#include "GBIMEResDef.h"

/*!
 * \ Layout View ָ��Ԥ����
 */
#define CLASS_VIEW_THIS_POITER(CLASS_NAME, pltThis)   \
	CLASS_NAME * pltViewThis = (CLASS_NAME *)pltThis->m_ltParent.m_hltView; \
	PCLayoutViewAbstract pltViewBase = &pltViewThis->m_ltParent; \

/*!
 * \brief ������ͼ��ʾ��Ա����
 */

GBIMELOCAL void ICLayoutViewVK__DrawKeyboard(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo);
GBIMELOCAL void ICLayoutViewVK__DrawOutputString(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo);
GBIMELOCAL void ICLayoutViewVK__DrawCandidate(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo);
GBIMELOCAL void ICLayoutViewVK__DrawPinYin(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo);
GBIMELOCAL void ICLayoutViewVK__DrawStroke(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo);
GBIMELOCAL void ICLayoutViewVK__DrawComponents(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo);
GBIMELOCAL void ICLayoutViewVK__DrawStrokeUpLine(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo);
GBIMELOCAL void ICLayoutViewVK__RedrawArrowButton(GBLPVOID pltObj, 
									   GBUINT16 iStartX, 
									   GBUINT16 iStartY, 
									   GBUINT16 layerId,
									   PEngineOutputInfo pEngineOutputInfo);
GBIMELOCAL void ICLayoutViewVK__RedrawShiftCapButton(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo);
GBIMELOCAL void ICLayoutViewVK__RedrawKeyboardSpecialButton(GBLPVOID pltObj);

CLayoutViewAbstract * CLayoutViewVK__Construct(PCLayoutViewVK pltViewVK)
{
	CLayoutViewAbstract__Construct(&pltViewVK->m_ltParent);
	CLayoutViewVK__Init(pltViewVK);
	return (CLayoutViewAbstract *)pltViewVK;
}

GBIMEReturn CLayoutViewVK__Init(PCLayoutViewVK pltViewVK)
{
	pltViewVK->m_pfcDrawKeyboard = ICLayoutViewVK__DrawKeyboard;
	pltViewVK->m_pfcDrawOutputString = ICLayoutViewVK__DrawOutputString;
	pltViewVK->m_pfcDrawCandidate = ICLayoutViewVK__DrawCandidate;
	pltViewVK->m_pfcDrawPinYin = ICLayoutViewVK__DrawPinYin;
	pltViewVK->m_pfcDrawStroke = ICLayoutViewVK__DrawStroke;
	pltViewVK->m_pfcDrawComponents = ICLayoutViewVK__DrawComponents;
	pltViewVK->m_pfcDrawStrokeUpLine = ICLayoutViewVK__DrawStrokeUpLine;
	pltViewVK->m_pfcRedrawArrowButton = ICLayoutViewVK__RedrawArrowButton;
	pltViewVK->m_pfcRedrawShiftCapButton = ICLayoutViewVK__RedrawShiftCapButton;
	pltViewVK->m_pfcRedrawKeyboardSpecialButton = ICLayoutViewVK__RedrawKeyboardSpecialButton;
	return GBIME_OK;
}

/*!
 * \brief �����̡���һ�㡿
 * \param pltObj 
 * \param pEngineOutputInfo 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-8-19 10:31:52
*/
static void ICLayoutViewVK__DrawKeyboard(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo)
{
	CLASE_THIS_POITER(CLayoutVK, pltObj)
	CLASS_VIEW_THIS_POITER(CLayoutViewVK, pltThis)

	//��ǰҪ��ʾ�Ĳ�
	GBUINT16 iCurrentLayerNumber;
	//��ʾͼƬ�����Ͻ�X����
	GBUINT16 iDisplayImageLeftX;
	//��ʾͼƬ�����Ͻ�Y����
	GBUINT16 iDisplayImageTopY;
	//��ǰҪ��ʾ�Ĳ������״̬�µ�ͼƬID
	GBUINT16 iCurrentLayerNomalImageId;
	//��ǰҪ��ʾ�Ĳ�İ���״̬�µ�ͼƬID
	GBUINT16 iCurrentLayerDownImageId;
	//��ѡ����
	GBUINT16 iCandidateRowCount = 1;
	//��ǰ��ť��Ӧ��ͼƬID
	GBUINT16 iCurrentButtonImageId;
	//��ǰ��ť�İ�ť����
	GBUINT16 iChangeButtonIndex;
	//��ť�ܸ���
	GBUINT16 iButtonTotalCount;
	GBUINT16 iCount;
	//�Ƿ��з�����״̬��ť
	GBUINT8 bHasButtonChange = 0;
	GBUINT16 currentButtonStatus = 0;
	GBIMEPoint winPos;
	GBRECT screenRect,imageRect;
	GBUINT16 iCandidateBoxHeight = 0;
	GBUINT16 iKeyBoardHeight = 0;
	PLayoutInfo pLayoutInfo = (PLayoutInfo)(&pltBase->m_ltData); // Layout UI data point
	GBUINT16 iCurrentLayoutId;
	PLayoutChgtLayer pltFourthLayerData = NULL; // ���Ĳ�����

	iCurrentLayoutId = GET_LAYOUT_ID(pltBase);
	// ��ȡ���̸߶�
	iKeyBoardHeight = pltBase->m_pfcGetLayoutHeight(pltObj,iCurrentLayoutId);
	// ȡ�ü�����ʾ�Ŀ�ʼλ�á����½ǡ���������Ϊ�����Ͻǡ�
	Global_GetWinPos(&winPos);	
	winPos.y -=  iKeyBoardHeight;
	
	// ��ǰҪ��ʾ�Ĳ�
	iCurrentLayerNumber = pLayoutInfo->ltCurLayer;
	// ȡ����ͼƬ�����Ϣ
	// ��ʾͼƬ�����Ͻ�X����
	iDisplayImageLeftX = (GBUINT16)winPos.x;
	// ��ʾͼƬ�����Ͻ�Y����
	iDisplayImageTopY =  (GBUINT16)winPos.y;
	// ����ͼƬ����״̬�µ�ͼƬID
	iCurrentLayerNomalImageId = pLayoutInfo->pltLayerData[FirstLayer].plycData->lyImageID + IMG_GBIME_SYSTEM;//����ͼƬID��ַ
	// ����ͼƬ����״̬�µ�ͼƬID
	iCurrentLayerDownImageId = pLayoutInfo->pltLayerData[FirstLayer].plycData->lyBKImageID + IMG_GBIME_SYSTEM;//����ͼƬID��ַ
	
	// ��ʾ����ͼƬ����0��]
	PT_DrawImage(iDisplayImageLeftX,iDisplayImageTopY,iCurrentLayerNomalImageId);
	
	// ���Ĳ����ݲ�Ϊ��
// 	if ((pltFourthLayerData = pLayoutInfo->pltLayerData[FourthLayer]) != NULL)
// 	{
// 		pltViewBase->m_pfcDrawButtonByStatus(pltObj, iDisplayImageLeftX, iDisplayImageTopY, NULL,
// 										     iCurrentLayoutId, FourthLayer, 
// 											 GBKEY_PREV_PAGE_SYLLABLE, ButtonStatusNomal);
// 	}


	// ��ӡLog��Ϣ
	//PT_PrintLogInfo(GBLOG_LAYOUTVIEW, ("***********************ICLayoutViewVK26MultiLang_PaintLayerOne() Draw Keyboard\n"));
	//PT_PrintLogInfo(GBLOG_LAYOUTVIEW, ("***********************GBLOG_LAYOUTVIEW Image ID = %d\n", iCurrentLayerNomalImageId));

	// �õ���ť����
	iButtonTotalCount = pLayoutInfo->pltLayerData[FirstLayer].lyBtCount;
	// �����Ƿ��б仯�İ�ť(��������״̬)
	for (iCount = 0;iCount < iButtonTotalCount; iCount++)
	{
		PBUTTONCHGDATA plyTempChgBt;
		plyTempChgBt = &pLayoutInfo->pltLayerData[FirstLayer].plyBtChgData[iCount];
		// btStatus : ����״̬��0Ϊ����̬��1Ϊ���£�2Ϊ��ɫ��3���ص�ǰ����
		// ����״̬����0ʱ���ͱ�ʾ�б仯�İ�ť
		if (plyTempChgBt->btStatus >= ButtonStatusDown 
			&& plyTempChgBt->btStatus <= ButtonStatusMove)
		{
			bHasButtonChange = 1;
			currentButtonStatus = plyTempChgBt->btStatus;
			iChangeButtonIndex = iCount;
			plyTempChgBt->btStatus = ButtonStatusNomal;
			break;
		}
	}

	// ����б仯�İ�ť
	if (bHasButtonChange)
	{
		GBLPCBUTTONCONSTDATA plyTempConstBt = &pLayoutInfo->pltLayerData[FirstLayer].plycBtData[iChangeButtonIndex];
		// ȡ��ť��ͼƬ�е�λ��
		memcpy(&imageRect, &plyTempConstBt->btPos, sizeof(GBRECT));
		
		// ͼƬ����Ļ����ʾ��λ��
		memcpy(&screenRect, &imageRect, sizeof(GBRECT));
		MOVE_RECT(&screenRect, winPos.x, iDisplayImageTopY);		
		// ���ݰ����Ĳ�ͬ״̬��ȡ����Ӧ��ͼƬID
		if ((currentButtonStatus) == ButtonStatusNomal || (currentButtonStatus == ButtonStatusUp))
		{
			iCurrentButtonImageId = plyTempConstBt->btNormal.imageID + IMG_GBIME_SYSTEM;//����ͼƬID��ַ
		}
		else if (currentButtonStatus == ButtonStatusDown || currentButtonStatus == ButtonStatusMove)
		{
			iCurrentButtonImageId = plyTempConstBt->btDown.imageID + IMG_GBIME_SYSTEM;//����ͼƬID��ַ
		}
		else if(currentButtonStatus == ButtonStatusGray)
		{
			iCurrentButtonImageId = plyTempConstBt->btGrey.imageID + IMG_GBIME_SYSTEM;//����ͼƬID��ַ	
		}
		else// Ĭ��
		{
			iCurrentButtonImageId = plyTempConstBt->btNormal.imageID + IMG_GBIME_SYSTEM;//����ͼƬID��ַ
		}

		PT_DrawImageWithClip(iCurrentButtonImageId, &screenRect, &imageRect);

		// ��ӡLog��Ϣ
// 		PT_PrintLogInfo(GBLOG_LAYOUTVIEW, ("***********************ICLayoutViewVK26MultiLang_PaintLayerOne() Draw Button\n"));
// 		PT_PrintLogInfo(GBLOG_LAYOUTVIEW, ("***********************Button Index = %d\t",iChangeButtonIndex));
// 		PT_PrintLogInfo(GBLOG_LAYOUTVIEW, ("***********************Conversion To Engine Key Value is 0x%4x\n",currentButtonStatus));
	}

	// ���ƻ�ɫ���ð�ť
	for (iCount = 0; iCount < iButtonTotalCount; iCount++)
	{
		PBUTTONCHGDATA plyTempChgBt = &pLayoutInfo->pltLayerData[FirstLayer].plyBtChgData[iCount];
		GBLPCBUTTONCONSTDATA plyTempConstBt = &pLayoutInfo->pltLayerData[FirstLayer].plycBtData[iCount];
		if (plyTempChgBt->btStatus == ButtonStatusGray)
		{
			pltViewBase->m_pfcDrawButtonByStatus(pltObj, iDisplayImageLeftX, iDisplayImageTopY, NULL,
				iCurrentLayoutId, FirstLayer, plyTempConstBt->btKey, ButtonStatusGray);
		}
	}

	// �ػ�ShiftCap��ť
	ICLayoutViewVK__RedrawShiftCapButton(pltObj, pEngineOutputInfo);
}

/*!
 * \brief ���������
 * \param pltObj 
 * \param pEngineOutputInfo 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-8-19 10:30:49
*/
static void ICLayoutViewVK__DrawOutputString(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo)
{
	CLASE_THIS_POITER(CLayoutVK, pltObj)
	GBUINT16 nVerticalSpacing = (GBUINT16) ((GBIME_SYLLABLE_LINE_HEIGHT - 
								PT_GetStrHeightEx(pEngineOutputInfo->pOutputString, GBIME_SYLLABLE_FONT_SIZE)) / 2);

	PT_RightAlignDrawString((GBINT)(pltBase->syllableDisplayStartPos.x + 1),
							(GBINT)(pltBase->syllableDisplayStartPos.y + nVerticalSpacing + 2), 
							GBIME_SYLLABLE_LINE_WIDTH, 
							pEngineOutputInfo->pOutputString, 
							GBIME_SYLLABLE_FONT_SIZE, 
							GBIME_OUTPUT_STRING_COLOR);
}

/*!
 * \brief ���ƺ�ѡ��
 * \param pltObj 
 * \param pEngineOutputInfo 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-10 18:11:30
*/
static void ICLayoutViewVK__DrawCandidate(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo)
{
	CLASE_THIS_POITER(CLayoutVK, pltObj)
	CLASS_VIEW_THIS_POITER(CLayoutViewVK, pltThis)
	GBPCWCHAR pCand;
	PGBRECT pCurCandRect;
	PGBIMEPoint pCurCandPos;
	GBUINT16 i;
	GBUINT16 focusCandidateIndex = 0;
	GBWCHAR drawCandBuf[GB_INPUT_BUFFER_LEN];

	// abc ͨ��������Multitap��ʽ�ں�ѡ����ѡ���ѡ
	if (pEngineOutputInfo->isMultiTapInputMode 
		&& !(pltBase->buttonStatus == ButtonStatusDown && pltBase->penInPostion == PenPositionInCandidate))
	{
		focusCandidateIndex = pEngineOutputInfo->nSelCol;
	}
	else
	{
		focusCandidateIndex = pltBase->currentCandidateIndex;
	}

	PT_EnableProprietarySymbolDisplay();
	// ����ѡ����
	for (i = 0; i < pltThis->candidateTouchRegion.candidateNum && i < pEngineOutputInfo->nCandNum; i++)
	{
		pCand = pEngineOutputInfo->ppCandidates[i];
		pCurCandPos = &pltThis->candidateTouchRegion.candidatePosArray[i];
		pCurCandRect = &pltThis->candidateTouchRegion.candidateRectArray[i];

		
		// �����к�ѡ�ַ���һ�ι��ˣ����س����ո�ȷǿɼ��ַ������⴦�� - weizhiping/2010/10/28
		memset(drawCandBuf, 0, GB_INPUT_BUFFER_LEN);
		//wcsncpy(drawCandBuf, pCand, wcslen(pCand));
		PT_ProprietarySymbolSwitch(pCand, drawCandBuf);
// 		for(j = 0; j < wcslen(pCand); j++)
// 		{
// 			switch (drawCandBuf[j])
// 			{
// 			case 0x0020: // �ո�
// 				drawCandBuf[j] = 0x0086;
// 				break;
// 			case 0x000A: // �س�
// 				drawCandBuf[j] = 0x0085;
// 				break;
// 			default:
// 				break;
// 			}
// 		}

		// ���ƺ�ѡ
		if (focusCandidateIndex == i)
		{
			// ��ѡ��ť����
			if (pltBase->buttonStatus == ButtonStatusDown 
				&& pltBase->penInPostion == PenPositionInCandidate)
			{
				GBRECT clipRect;
				GBIMEPoint layoutWinStartPos;
				
				pltBase->m_pfcGetWinPos(pltObj, &layoutWinStartPos);
				FILL_RECT(&clipRect, (GBI16)(pCurCandRect->left), (GBI16)(pCurCandRect->top),
					(GBI16)(pCurCandRect->right),	(GBI16)(pCurCandRect->bottom - 3));
				pltViewBase->m_pfcDrawButtonByStatus(pltObj, 
					layoutWinStartPos.x,
					layoutWinStartPos.y, 
					&clipRect,
					GET_LAYOUT_ID(pltBase), 
					GET_LAYOUT_CURRENT_LAYER(pltBase), 
					GBKEY_VK_SYM_CANDIDATE_AREA, 
					ButtonStatusDown);				
			}

			// ���ƺ�ѡ
			PT_RightAlignDrawString((GBINT)pCurCandPos->x, (GBINT)(pCurCandPos->y),
									(GBINT)(pCurCandRect->right - pCurCandRect->left),
									drawCandBuf, 
									GBIME_CANDIDATE_FONT_SIZE, 
									GBIME_CANDIDATE_SELECTED_FONT_COLOR);
		}
		else
		{
			// ���ƺ�ѡ
			PT_RightAlignDrawString((GBINT)pCurCandPos->x, (GBINT)(pCurCandPos->y),
									(GBINT)(pCurCandRect->right - pCurCandRect->left),
									drawCandBuf,
									GBIME_CANDIDATE_FONT_SIZE, 
									GBIME_CANDIDATE_NORMAL_FONT_COLOR);
		}
		
		// ���ƴ�ֱ��ѡ�ָ���[���һ������]
		if (i < pltThis->candidateTouchRegion.candidateNum - 1)
		{
			PT_DrawVerticalLineType((GBU16)(pCurCandRect->top + 3), 
				(GBU16)(pCurCandRect->bottom - 1), 
				pCurCandRect->right,
				PT_COLOR_LIGHT_GREY, 
				PT_LINT_TYPE_DOTTED);
		}
	}
	PT_DisableProprietarySymbolDisplay();

#if __GBIME_LOG__ > 0
	//��ӡlog
	PT_PrintLogInfo(GBLOG_LAYOUTVIEW, ("\r\n��ѡ��"));
	for (i = 0; i < pltThis->candidateTouchRegion.candidateNum && i < pEngineOutputInfo->nCandNum; i++)
	{
		GBACHAR buf[64] = {0};
		pCand = pEngineOutputInfo->ppCandidates[i];
		PT_wcstombs(buf, pCand, sizeof(buf));
		PT_PrintLogInfo(GBLOG_LAYOUTVIEW, ("%s, ", buf));
	}
	PT_PrintLogInfo(GBLOG_LAYOUTVIEW, ("\r\n"));
#endif
}

/*!
 * \brief 9��ƴ��ʱ����ƴ��
 * \param pltObj 
 * \param pEngineOutputInfo 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-8-19 10:31:11
*/
static void ICLayoutViewVK__DrawPinYin(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo)
{
	CLASE_THIS_POITER(CLayoutVK, pltObj)
	CLASS_VIEW_THIS_POITER(CLayoutViewVK, pltThis)
	GBUINT16 i;
	GBPCWCHAR pCand;
	GBRECT buttonRect;
	PGBRECT pCurSyllableRect;
	PGBIMEPoint pCurSyllablePos;
	GBUINT16 iCurrentLayoutId = pltBase->m_ltData.pltcData->ltID;

	// ����Ƿ�ҳ
	if (pltBase->penInPostion == PenPositionInCandidatePageDown
		|| pltBase->penInPostion == PenPositionInCandidatePageUp)
	{
		if (pltBase->penInPostion == PenPositionInCandidatePageDown)
		{
			pltBase->m_pfcGetBottonRectByKey(pltObj,
				iCurrentLayoutId,
				SecondLayer,
				GBKEY_NEXT_PAGE_CANDIATE,
				&buttonRect);
		}
		else
		{
			pltBase->m_pfcGetBottonRectByKey(pltObj,
				iCurrentLayoutId,
				SecondLayer,
				GBKEY_PREV_PAGE_CANDIATE,
				&buttonRect);
		}
	}
	else
	{
		// ����ѡʱ�õ�
		pltBase->m_pfcGetBottonRectByKey(pltObj,
										 iCurrentLayoutId,
										 SecondLayer,
										 GBKEY_PREV_PAGE_CANDIATE,
										 &buttonRect);
	}

	// �����������������������������
	if (pEngineOutputInfo->isSymbolCandType/*GBEngine_IsSymbolCandType(Global_GetEnineInstance())*/)
	{
		if (pltThis->syllableTouchRegion.candidateNum == 1 
			&& pEngineOutputInfo->pOutputString != NULL)
		{			
			pCand = pEngineOutputInfo->pOutputString;
			pCurSyllablePos = &pltThis->syllableTouchRegion.candidatePosArray[0];
			pCurSyllableRect = &pltThis->syllableTouchRegion.candidateRectArray[0];
		
			// ��ť����
// 			if (pltBase->buttonStatus == ButtonStatusDown
// 				&& pltBase->penInPostion == PenPositionInSyllable)
// 			{
// 				GBRECT clipRect;
// 				GBIMEPoint layoutWinStartPos;
// 
// 				pltBase->m_pfcGetWinPos(pltObj, &layoutWinStartPos);
// 				FILL_RECT(&clipRect, (GBI16)(pCurSyllableRect->left + 1),
// 					(GBI16)(pCurSyllableRect->top + 1),
// 					pCurSyllableRect->right,
// 					(GBI16)(pCurSyllableRect->bottom - 2));
// 				pltViewBase->m_pfcDrawButtonByStatus(pltObj, 
// 					layoutWinStartPos.x,
// 					layoutWinStartPos.y, 
// 					&clipRect,
// 					GET_LAYOUT_ID(pltBase), 
// 					GET_LAYOUT_CURRENT_LAYER(pltBase), 
// 					GBKEY_VK_SYM_INPUT_AREA, 
// 					ButtonStatusDown);
// 			}

			PT_RightAlignDrawString((GBINT)pCurSyllablePos->x, (GBINT)pCurSyllablePos->y, 
				(GBINT)(pCurSyllableRect->right - pCurSyllableRect->left), 
				pCand, 
				GBIME_SYLLABLE_FONT_SIZE, GBIME_SYLLABLE_SELECTED_FONT_COLOR);
		}
	}
	else // ����ѡƴ��
	{		
		for (i = 0; i < pltThis->syllableTouchRegion.candidateNum && pEngineOutputInfo->nSyllableNum; i++)
		{
			pCand = pEngineOutputInfo->ppSyllables[i];
			pCurSyllablePos = &pltThis->syllableTouchRegion.candidatePosArray[i];
			pCurSyllableRect = &pltThis->syllableTouchRegion.candidateRectArray[i];
			
			// ���ƺ�ѡ
			if (pltBase->currentSyllableIndex == i)
			{
				// ��ѡ��ť����
				if (pltBase->buttonStatus == ButtonStatusDown
					&& pltBase->penInPostion == PenPositionInSyllable)
				{
					GBRECT clipRect;
					GBIMEPoint layoutWinStartPos;
					
					pltBase->m_pfcGetWinPos(pltObj, &layoutWinStartPos);
					FILL_RECT(&clipRect, (GBI16)(pCurSyllableRect->left + 1),
						(GBI16)(pCurSyllableRect->top),
						pCurSyllableRect->right,
						(GBI16)(pCurSyllableRect->bottom - 1));
					pltViewBase->m_pfcDrawButtonByStatus(pltObj, 
						layoutWinStartPos.x,
						layoutWinStartPos.y,
						&clipRect,
						GET_LAYOUT_ID(pltBase), 
						GET_LAYOUT_CURRENT_LAYER(pltBase), 
						GBKEY_VK_SYM_INPUT_AREA, 
						ButtonStatusDown);
				}
				
				PT_RightAlignDrawString((GBINT)pCurSyllablePos->x, (GBINT)pCurSyllablePos->y, 
					(GBINT)(pCurSyllableRect->right - pCurSyllableRect->left), 
					pCand, 
					GBIME_SYLLABLE_FONT_SIZE, GBIME_SYLLABLE_SELECTED_FONT_COLOR);
			}
			else
			{
				PT_RightAlignDrawString((GBINT)pCurSyllablePos->x, (GBINT)pCurSyllablePos->y, 
					(GBINT)(pCurSyllableRect->right - pCurSyllableRect->left), 
					pCand, 
					GBIME_SYLLABLE_FONT_SIZE, GBIME_SYLLABLE_NORMAL_FONT_COLOR);
			}
			
			// ���ƴ�ֱ��ѡ�ָ���[���һ������]
			if (i < pltThis->syllableTouchRegion.candidateNum - 1)
			{			
				PT_DrawVerticalLineType((GBU16)(pCurSyllableRect->top + 1), 
										(GBU16)(pCurSyllableRect->bottom - 2), 
										pCurSyllableRect->right, 
										PT_COLOR_LIGHT_GREY, 
										PT_LINT_TYPE_DOTTED);
			}
		}
#if __GBIME_LOG__ > 0
		//��ӡlog
		PT_PrintLogInfo(GBLOG_LAYOUTVIEW, ("\r\n���ڣ�"));
		for (i = 0; i < pltThis->syllableTouchRegion.candidateNum && pEngineOutputInfo->nSyllableNum; i++)
		{
			GBACHAR buf[64] = {0};
			pCand = pEngineOutputInfo->ppSyllables[i];
			PT_wcstombs(buf, pCand, sizeof(buf));
			PT_PrintLogInfo(GBLOG_LAYOUTVIEW, ("%s, ", buf));
		}
		PT_PrintLogInfo(GBLOG_LAYOUTVIEW, ("\r\n"));
#endif
	}	
}

/*!
 * \brief ���Ʊʻ�
 * \param pltObj 
 * \param pEngineOutputInfo 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-8-19 10:31:03
*/
static void ICLayoutViewVK__DrawStroke(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo)
{
	CLASE_THIS_POITER(CLayoutVK, pltObj)
	GBUINT16 nVerticalSpacing = (GBUINT16) ((GBIME_SYLLABLE_LINE_HEIGHT - 
								PT_GetStrHeightEx(pEngineOutputInfo->pStroke, GBIME_STROKE_FONT_SIZE)) / 2);

	PT_RightAlignDrawString((GBINT)(pltBase->syllableDisplayStartPos.x + 1),
							(GBINT)(pltBase->syllableDisplayStartPos.y + nVerticalSpacing),
							GBIME_CAND_AREA_WIDTH,
							pEngineOutputInfo->pStroke,
							GBIME_STROKE_FONT_SIZE,
							GBIME_STROKE_COLOR);
}

/*!
 * \brief ���ƿ�ݲ���
 * \param pltObj 
 * \param pEngineOutputInfo 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-8-19 10:30:57
*/
static void ICLayoutViewVK__DrawComponents(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo)
{
	CLASE_THIS_POITER(CLayoutVK, pltObj)

	GBUINT16 iCount = 0;
	GBUINT16 iComponentsCount;
	GBRECT buttonRect;
	GBIMEPoint winPos;
	GBUINT16 iKeyBoardHeight;
	GBUINT16 iKeyValue;
	GBUINT16 pComponentString[2];
	GBBOOL bTrue;
	GBUINT16 iCurrentLayoutId;
	KEYBOARD_TYPE keyboardType = GBConfig_GetKeyboardType(Global_GetConfigInstance());
	//GBWCHAR drawCandBuf[GB_INPUT_BUFFER_LEN];

	// ȡ�ü�����ʾ�Ŀ�ʼλ��[���½�����]
	Global_GetWinPos(&winPos);
	iCurrentLayoutId = GET_LAYOUT_ID(pltBase);
	// ���̸߶�
	iKeyBoardHeight = pltBase->m_pfcGetLayoutHeight(pltObj, iCurrentLayoutId);
	// ����õ��������Ͻ�Y����
	winPos.y -= iKeyBoardHeight;
	
	iComponentsCount = 0;
	while (pEngineOutputInfo->pComponents[iComponentsCount])
	{
		iComponentsCount++;
	}

	//PT_EnableProprietarySymbolDisplay();	
	// ����ݲ���,�ӵ�7������ʼ
	for (iCount = 0; iCount< iComponentsCount; iCount++)
	{
		pComponentString[0] = pEngineOutputInfo->pComponents[iCount];
		pComponentString[1] = 0;

		// ��ݲ�����ʾ����Ӧ��ť
		if (keyboardType == KEYBOARD_VK_QWERTY)
		{
			iKeyValue = (GBUINT16)(GBKEY_VK_7_NUMBERKEY + iCount);
		} 
		else
		{
			iKeyValue = (GBUINT16)((iCount < 3) ? (GBKEY_7 + iCount) : GBKEY_0);
		}		

		// ȡ��ť������
		bTrue = pltBase->m_pfcGetBottonRectByKey(pltObj, iCurrentLayoutId, FirstLayer, iKeyValue, &buttonRect);
		if (bTrue)
		{
			GBINT x, y;
			GBINT16 compWidth = PT_GetStrWidthEx(pComponentString, GBIME_COMPONENT_FONT_SIZE);
			GBINT16 compHeight = PT_GetStrHeightEx(pComponentString, GBIME_COMPONENT_FONT_SIZE);			
			
			MOVE_RECT(&buttonRect, winPos.x, winPos.y);

			// ��ʾ�����ڰ�ť�м�
			x = (GBINT)(buttonRect.left + (buttonRect.right - buttonRect.left - compWidth) / 2);
			y = (GBINT)(buttonRect.top + (buttonRect.bottom - buttonRect.top - compHeight) / 2);

			// ���س����ո�ȷǿɼ��ַ������⴦�� - weizhiping/2010/10/28
			//memset(drawCandBuf, 0, GB_INPUT_BUFFER_LEN);
			//PT_ProprietarySymbolSwitch(pComponentString, drawCandBuf);
			//PT_DrawString(pComponentString, GBIME_COMPONENT_FONT_SIZE,GBIME_COMPONENT_COLOR, x, y);
			PT_PrintStrokeText(pComponentString, x, y, GBIME_COMPONENT_FONT_SIZE, GBIME_COMPONENT_COLOR);
		}
	}
	//PT_DisableProprietarySymbolDisplay();
}

/*!
 * \brief ���Ʊʻ�������
 * \param pltObj 
 * \param pEngineOutputInfo 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-8-19 10:31:03
*/
static void ICLayoutViewVK__DrawStrokeUpLine(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo)
{
	CLASE_THIS_POITER(CLayoutVK, pltObj)
	GBINT nAvailableWidth = GBIME_CAND_AREA_WIDTH; // ����ʾ���ܿ��
	GBINT nUseWidth = 0;
	GBINT nVerticalSpacing = (GBUINT16) ((GBIME_SYLLABLE_LINE_HEIGHT - 
								PT_GetStrHeightEx(pEngineOutputInfo->pStroke, GBIME_STROKE_FONT_SIZE)) / 2);
	GBINT left = (GBINT)(pltBase->syllableDisplayStartPos.x + 1);
	GBINT top = (GBINT)(pltBase->syllableDisplayStartPos.y + nVerticalSpacing);
	GBINT nSpacing = 5;

	// �������봮
	if (pEngineOutputInfo->pOutputString != NULL && pEngineOutputInfo->pOutputString[0])
	{
		nUseWidth = PT_RightAlignDrawString(left, top, nAvailableWidth,	pEngineOutputInfo->pOutputString, 
			GBIME_STROKE_FONT_SIZE,	GBIME_OUTPUT_STRING_COLOR);

		nAvailableWidth = GBIME_CAND_AREA_WIDTH - nUseWidth - 4 * nSpacing;

		// ����һ����ֱ�ָ��߲�������ʾ�ʻ�
		if (nAvailableWidth > 0)
		{
			left += nUseWidth + nSpacing;
			PT_DrawVerticalLineType((GBU16)(pltBase->syllableDisplayStartPos.y + 1),
				(GBU16)(pltBase->syllableDisplayStartPos.y + GBIME_SYLLABLE_LINE_HEIGHT - 2),
				(GBU16)left,
				PT_COLOR_LIGHT_GREY, PT_LINT_TYPE_DOTTED);
			left += nSpacing;
		}
	}
	else
	{
		nAvailableWidth = GBIME_CAND_AREA_WIDTH - 2 * nSpacing;
	}

	PT_RightAlignDrawStrokeString(left, top, nAvailableWidth, pEngineOutputInfo->pStroke, GBIME_STROKE_FONT_SIZE, GBIME_STROKE_COLOR);
}

/*!
 * \brief ��������״̬�ػ��ͷ��ť
 * \param pltObj 
 * \param iStartX 
 * \param iStartY 
 * \param pEngineOutputInfo 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-8-31 11:30:24
*/
static void ICLayoutViewVK__RedrawArrowButton(GBLPVOID pltObj, 
											 GBUINT16 iStartX, 
											 GBUINT16 iStartY, 
											 GBUINT16 layerId,
											 PEngineOutputInfo pEngineOutputInfo)
{
	CLASE_THIS_POITER(CLayoutVK, pltObj)
	CLASS_VIEW_THIS_POITER(CLayoutViewVK, pltThis)
	GBUINT16 iCurrentLayoutId = pltBase->m_ltData.pltcData->ltID;

	///<ƴ����ҳ��ʾ���ͷ
	if (pEngineOutputInfo->bSyllableArrowLeft)
	{
		pltViewBase->m_pfcDrawButtonByStatus(pltObj, iStartX, iStartY, NULL,
			iCurrentLayoutId, layerId, 
			GBKEY_PREV_PAGE_SYLLABLE, ButtonStatusNomal);
	} 
	else
	{
		pltViewBase->m_pfcDrawButtonByStatus(pltObj, iStartX, iStartY, NULL,
			iCurrentLayoutId, layerId, 
			GBKEY_PREV_PAGE_SYLLABLE, ButtonStatusGray);
	}

	///<ƴ����ҳ��ʾ�Ҽ�ͷ
	if (pEngineOutputInfo->bSyllableArrowRight)
	{
		pltViewBase->m_pfcDrawButtonByStatus(pltObj, iStartX, iStartY, NULL,
			iCurrentLayoutId, layerId, 
			GBKEY_NEXT_PAGE_SYLLABLE, ButtonStatusNomal);
	} 
	else
	{
		pltViewBase->m_pfcDrawButtonByStatus(pltObj, iStartX, iStartY, NULL,
			iCurrentLayoutId, layerId, 
			GBKEY_NEXT_PAGE_SYLLABLE, ButtonStatusGray);
	}

	///<��һҳ
	if (pEngineOutputInfo->bCandArrowUp)
	{
		pltViewBase->m_pfcDrawButtonByStatus(pltObj, iStartX, iStartY, NULL,
			iCurrentLayoutId, layerId, 
			GBKEY_PREV_PAGE_CANDIATE, ButtonStatusNomal);
	} 
	else
	{
		pltViewBase->m_pfcDrawButtonByStatus(pltObj, iStartX, iStartY, NULL,
			iCurrentLayoutId, layerId, 
			GBKEY_PREV_PAGE_CANDIATE, ButtonStatusGray);
	}

	///<��һҳ
	if (pEngineOutputInfo->bCandArrowDown)
	{
		pltViewBase->m_pfcDrawButtonByStatus(pltObj, iStartX, iStartY, NULL,
			iCurrentLayoutId, layerId, 
			GBKEY_NEXT_PAGE_CANDIATE, ButtonStatusNomal);
	} 
	else
	{
		pltViewBase->m_pfcDrawButtonByStatus(pltObj, iStartX, iStartY, NULL, 
			iCurrentLayoutId, layerId, 
			GBKEY_NEXT_PAGE_CANDIATE, ButtonStatusGray);
	}

}

/*!
 * \brief ��������״̬�ػ��ͷ��ť
 * \param pltObj 
 * \param iStartX 
 * \param iStartY 
 * \param pEngineOutputInfo 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-8-31 11:30:24
*/
static void ICLayoutViewVK__RedrawShiftCapButton(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo)
{
	CLASE_THIS_POITER(CLayoutVK, pltObj)
	CLASS_VIEW_THIS_POITER(CLayoutViewVK, pltThis)
	GBUINT16 iCurrentLayoutId = pltBase->m_ltData.pltcData->ltID;
	GBUINT16 iStartX; 
	GBUINT16 iStartY; 
	GBUINT16 layerId = FirstLayer;
	GBIMEPoint winPos;
	GBUINT16 iKeyBoardHeight;
	GBShiftCapStatus curShiftCapStatus = Global_GetShiftCapStatus();
	
	// ȡ�ü�����ʾ�Ŀ�ʼλ��[���½�����]
	Global_GetWinPos(&winPos);
	iCurrentLayoutId = GET_LAYOUT_ID(pltBase);
	// ���̸߶�
	iKeyBoardHeight = pltBase->m_pfcGetLayoutHeight(pltObj, iCurrentLayoutId);
	// ����õ��������Ͻ�Y����
	winPos.y -= iKeyBoardHeight;
	iStartX = winPos.x;
	iStartY = winPos.y;

	switch (curShiftCapStatus)
	{
	case GBStatus_Shift:
		pltViewBase->m_pfcDrawButtonByStatus(pltObj, iStartX, iStartY, NULL,
			iCurrentLayoutId, layerId, 
			GBKEY_VK_CAP, ButtonStatusDown);
		break;
	case GBStatus_Caplock:
		pltViewBase->m_pfcDrawButtonByStatus(pltObj, iStartX, iStartY, NULL,
			iCurrentLayoutId, layerId, 
			GBKEY_VK_CAP, ButtonStatusGray);
		break;
	default:
		pltViewBase->m_pfcDrawButtonByStatus(pltObj, iStartX, iStartY, NULL,
			iCurrentLayoutId, layerId, 
			GBKEY_VK_CAP, ButtonStatusNomal);
		break;
	}
}

/*!
 * \brief �ػ�ڶ����������߼������ڵ�һ��İ�ť����ƴ���ָ�����...��
 * \param pltObj 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-10-12 15:58:45
*/
static void ICLayoutViewVK__RedrawKeyboardSpecialButton(GBLPVOID pltObj)
{
#if __MTK__ > 0
	CLASE_THIS_POITER(CLayoutVK, pltObj)

	GBIMEPoint winPos;
	GBUINT16 iCurrentLayoutId, iCurrentLayerNumber;
	GBUINT16 iKeyBoardHeight = 0;
	GBUINT16 iCandidateBoxHeight = 0;
	PLayoutChgtLayer pLayoutChgtLayer;
	ButtonStatusEnum buttonStatus;
	GBUINT16 iCurrentButtonImageId;
	GBINT16 iDisplayImageLeftX, iDisplayImageTopY;
	GBUINT16 i = 0;

	Global_GetWinPos(&winPos);
	iCurrentLayerNumber = GET_LAYOUT_CURRENT_LAYER(pltBase);
	iCurrentLayoutId = GET_LAYOUT_ID(pltBase);	
	iKeyBoardHeight = pltBase->m_pfcGetLayoutHeight(pltObj, iCurrentLayoutId);
	iCandidateBoxHeight = pltBase->m_pfcGetCandidateBoxHeight(pltObj, iCurrentLayoutId, iCurrentLayerNumber);
	winPos.y -= (iKeyBoardHeight + iCandidateBoxHeight);

	pLayoutChgtLayer = (PLayoutChgtLayer)&(pltBase->m_ltData.pltLayerData[iCurrentLayerNumber]);
	for (i = 0;i < pLayoutChgtLayer->lyBtCount;i++)
	{
		// ������Ǻ�ѡ�������еİ�ť
		if (pLayoutChgtLayer->plycBtData[i].btPos.bottom > iCandidateBoxHeight)
		{
			buttonStatus = pLayoutChgtLayer->plyBtChgData[i].btStatus;

			// �ж��Ƿ�ѡ�иð�ť
			if (pLayoutChgtLayer->plycBtData[i].btKey == pltBase->currentLayoutKeyValue
				&& (buttonStatus == ButtonStatusDown || buttonStatus == ButtonStatusMove))				   
			{
				iCurrentButtonImageId = pLayoutChgtLayer->plycBtData[i].btDown.imageID + IMG_GBIME_SYSTEM;//����ͼƬID��ַ;
			}
			else
			{
				iCurrentButtonImageId = pLayoutChgtLayer->plycBtData[i].btNormal.imageID + IMG_GBIME_SYSTEM;//����ͼƬID��ַ;
			}
			
			iDisplayImageLeftX = winPos.x + pLayoutChgtLayer->plycBtData[i].btNormal.rcPos.left;
			iDisplayImageTopY = winPos.y + pLayoutChgtLayer->plycBtData[i].btNormal.rcPos.top;
			PT_DrawImage(iDisplayImageLeftX, iDisplayImageTopY, iCurrentButtonImageId);
		}
	}
#endif
}

#endif // �����������