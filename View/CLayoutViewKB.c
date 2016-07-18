/***************************************************************************
* File Name    :  CLayoutViewKB.h
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
#if defined(__GBIME_KB__) // �����������

#include "CLayoutViewKB.h"
#include "CLayoutKB.h"
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

GBIMELOCAL void ICLayoutViewKB__DrawKeyboard(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo);
GBIMELOCAL void ICLayoutViewKB__DrawOutputString(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo);
GBIMELOCAL void ICLayoutViewKB__DrawCandidate(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo);
GBIMELOCAL void ICLayoutViewKB__DrawPinYin(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo);
GBIMELOCAL void ICLayoutViewKB__DrawStroke(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo);
GBIMELOCAL void ICLayoutViewKB__DrawComponents(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo);
GBIMELOCAL void ICLayoutViewKB__DrawStrokeUpLine(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo);
GBIMELOCAL void ICLayoutViewKB__RedrawArrowButton(GBLPVOID pltObj, 
									   GBUINT16 iStartX, 
									   GBUINT16 iStartY, 
									   GBUINT16 layerId,
									   PEngineOutputInfo pEngineOutputInfo);
//GBIMELOCAL void ICLayoutViewKB__RedrawShiftCapButton(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo);
//GBIMELOCAL void ICLayoutViewKB__RedrawKeyboardSpecialButton(GBLPVOID pltObj);

CLayoutViewAbstract * CLayoutViewKB__Construct(PCLayoutViewKB pltViewKB)
{
	CLayoutViewAbstract__Construct(&pltViewKB->m_ltParent);
	CLayoutViewKB__Init(pltViewKB);

	PT_PrintLogInfo(GBLOG_LAYOUTVIEW, ("CLayoutViewKB__Construct\n"));

	return (CLayoutViewAbstract *)pltViewKB;
}

GBIMEReturn CLayoutViewKB__Init(PCLayoutViewKB pltViewKB)
{
	PT_PrintLogInfo(GBLOG_LAYOUTVIEW, ("CLayoutViewKB__Init\n"));

	pltViewKB->m_pfcDrawKeyboard = ICLayoutViewKB__DrawKeyboard;
	pltViewKB->m_pfcDrawOutputString = ICLayoutViewKB__DrawOutputString;
	pltViewKB->m_pfcDrawCandidate = ICLayoutViewKB__DrawCandidate;
	pltViewKB->m_pfcDrawPinYin = ICLayoutViewKB__DrawPinYin;
	pltViewKB->m_pfcDrawStroke = ICLayoutViewKB__DrawStroke;
	pltViewKB->m_pfcDrawComponents = ICLayoutViewKB__DrawComponents;
	pltViewKB->m_pfcDrawStrokeUpLine = ICLayoutViewKB__DrawStrokeUpLine;
	pltViewKB->m_pfcRedrawArrowButton = ICLayoutViewKB__RedrawArrowButton;
//	pltViewKB->m_pfcRedrawShiftCapButton = ICLayoutViewKB__RedrawShiftCapButton;
//	pltViewKB->m_pfcRedrawKeyboardSpecialButton = ICLayoutViewKB__RedrawKeyboardSpecialButton;
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
static void ICLayoutViewKB__DrawKeyboard(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo)
{
	CLASE_THIS_POITER(CLayoutKB, pltObj)
	CLASS_VIEW_THIS_POITER(CLayoutViewKB, pltThis)

	GBINT16 iScreenWidth = 0;

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
//	GBUINT16 iCandidateRowCount = 1;
	//��ǰ��ť��Ӧ��ͼƬID
//	GBUINT16 iCurrentButtonImageId;
	//��ǰ��ť�İ�ť����
//	GBUINT16 iChangeButtonIndex;
	//��ť�ܸ���
//	GBUINT16 iButtonTotalCount;
//	GBUINT16 iCount;
	//�Ƿ��з�����״̬��ť
//	GBUINT8 bHasButtonChange = 0;
//	GBUINT16 currentButtonStatus = 0;
	GBIMEPoint winPos;
//	GBRECT screenRect,imageRect;
//	GBUINT16 iCandidateBoxHeight = 0;
	GBUINT16 iKeyBoardHeight = 0;
	GBUINT16 iKeyBoardWidth = 0;
	PLayoutInfo pLayoutInfo = (PLayoutInfo)(&pltBase->m_ltData); // Layout UI data point
	GBUINT16 iCurrentLayoutId;
//	PLayoutChgtLayer pltFourthLayerData = NULL; // ���Ĳ�����

	PT_PrintLogInfo(GBLOG_LAYOUTVIEW, ("ICLayoutViewKB__DrawKeyboard\n"));

	iCurrentLayoutId = GET_LAYOUT_ID(pltBase);
	// ��ȡ���̸߶�
	iKeyBoardHeight = pltBase->m_pfcGetLayoutHeight(pltObj,iCurrentLayoutId);
	iKeyBoardWidth = pltBase->m_pfcGetLayoutWidth(pltObj, iCurrentLayoutId);

	// ȡ�ü�����ʾ�Ŀ�ʼλ�á����½ǡ���������Ϊ�����Ͻǡ�
	Global_GetWinPos(&winPos);	
	winPos.y -=  iKeyBoardHeight;
	
	iScreenWidth = PT_GetLcdWidth();
	//iScreenWidth = Interface_GetWinWidth();

	// ��ǰҪ��ʾ�Ĳ�
	iCurrentLayerNumber = pLayoutInfo->ltCurLayer;
	// ȡ����ͼƬ�����Ϣ
	// ��ʾͼƬ�����Ͻ�X����
	iDisplayImageLeftX = (GBUINT16)winPos.x + (iScreenWidth - iKeyBoardWidth) / 2;
	// ��ʾͼƬ�����Ͻ�Y����
	iDisplayImageTopY =  (GBUINT16)winPos.y;
	// ����ͼƬ����״̬�µ�ͼƬID
	iCurrentLayerNomalImageId = pLayoutInfo->pltLayerData[FirstLayer].plycData->lyImageID + IMG_GBIME_SYSTEM;//����ͼƬID��ַ
	// ����ͼƬ����״̬�µ�ͼƬID
	iCurrentLayerDownImageId = pLayoutInfo->pltLayerData[FirstLayer].plycData->lyBKImageID + IMG_GBIME_SYSTEM;//����ͼƬID��ַ
	
	// ��ʾ����ͼƬ����0��]
	PT_DrawImage(iDisplayImageLeftX,iDisplayImageTopY,iCurrentLayerNomalImageId);
	return;
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
static void ICLayoutViewKB__DrawOutputString(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo)
{
	CLASE_THIS_POITER(CLayoutKB, pltObj)
	GBUINT16 nAvailableWidth;
	GBUINT16 iKeyBoardWidth;
	GBUINT16 nVerticalSpacing = 0;

	// ��ȡ���̿��
	iKeyBoardWidth = pltBase->m_pfcGetLayoutWidth(pltObj, GET_LAYOUT_ID(pltBase));
	nAvailableWidth = iKeyBoardWidth - 2 * GBIME_KB_CANDIDATE_LEFT_PADDING;//���봮�����ʾ����
	nVerticalSpacing = (GBUINT16) ((GBIME_SYLLABLE_LINE_HEIGHT - PT_GetStrHeightEx(pEngineOutputInfo->pOutputString, GBIME_SYLLABLE_FONT_SIZE)) / 2);

	PT_RightAlignDrawString((GBINT)(pltBase->syllableDisplayStartPos.x + GBIME_KB_CANDIDATE_LEFT_PADDING),
							(GBINT)(pltBase->syllableDisplayStartPos.y + nVerticalSpacing + GBIME_KB_CANDIDATE_TOP_PADDING), 
							nAvailableWidth, 
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
static void ICLayoutViewKB__DrawCandidate(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo)
{
	CLASE_THIS_POITER(CLayoutKB, pltObj)
	CLASS_VIEW_THIS_POITER(CLayoutViewKB, pltThis)
	GBPCWCHAR pCand;
	PGBRECT pCurCandRect = {0};
	PGBIMEPoint pCurCandPos = {0};
	GBUINT16 nRow = 0;	
	GBUINT16 nCol = 0;
	GBUINT16 lineStart = 0;
	GBUINT16 lineEnd = 0;
//	GBUINT16 focusCandidateIndex = 0;
//	GBUINT16 i;
	GBWCHAR drawCandBuf[GB_CAND_BUFFER_LEN];

	PT_PrintLogInfo(GBLOG_LAYOUTVIEW, ("ICLayoutViewKB__DrawCandidate:cur cand index:%d\n", pltBase->currentCandidateIndex));

// 	// abc ͨ��������Multitap��ʽ�ں�ѡ����ѡ���ѡ
// 	if (pEngineOutputInfo->isMultiTapInputMode 
// 		&& !(pltBase->buttonStatus == ButtonStatusDown && pltBase->penInPostion == PenPositionInCandidate))
// 	{
// 		focusCandidateIndex = pEngineOutputInfo->nSelCol;
// 	}
// 	else
// 	{
// 		focusCandidateIndex = pltBase->currentCandidateIndex;
// 	}

	PT_EnableProprietarySymbolDisplay();
	for (nRow = 0; nRow < pEngineOutputInfo->nRowCount; nRow++)
	{
		lineStart = pEngineOutputInfo->pRowStart[nRow];
		lineEnd = pEngineOutputInfo->pRowStart[nRow+1];
		
		for (nCol = lineStart; nCol < lineEnd; nCol++)
		{
			pCand = pEngineOutputInfo->ppCandidates[nCol];
			if (pEngineOutputInfo->isSymbolCandType/*GBEngine_IsSymbolCandType(Global_GetEnineInstance()) */
				&& ((pEngineOutputInfo->pOutputString && pEngineOutputInfo->pOutputString[0] == 0)
				 || (pEngineOutputInfo->pOutputString && pEngineOutputInfo->pOutputString[0] == '?')))
			{
				if(nRow == 0)
				{
					pCurCandPos = &pltThis->syllableRegion.candidatePosArray[nCol];
					pCurCandRect = &pltThis->syllableRegion.candidateRectArray[nCol];
				}
				else
				{
					pCurCandPos = &pltThis->candidateRegion.candidatePosArray[nCol - lineStart];
					pCurCandRect = &pltThis->candidateRegion.candidateRectArray[nCol - lineStart];
				}				
			}
			else
			{
				pCurCandPos = &pltThis->candidateRegion.candidatePosArray[nCol];
				pCurCandRect = &pltThis->candidateRegion.candidateRectArray[nCol];
			}			

			//ѡ��״̬ʱ���±�
			if(!pEngineOutputInfo->isMultiTapInputMode 
				&& pEngineOutputInfo->isSelectedStatus 
				&& nRow == pEngineOutputInfo->nSelRow)
			{
				GBWCHAR indexBuf[2] = {0};
				indexBuf[0] = '0' + nCol - lineStart + 1;
				indexBuf[1] = 0;

				PT_RightAlignDrawString((GBINT)pCurCandPos->x, (GBINT)(pCurCandPos->y + GBIME_KB_CANDIDATE_TOP_PADDING),
					(GBINT)(pCurCandRect->right - pCurCandRect->left),
					indexBuf, PT_FONT_SIZE_SMALL, GBIME_CANDIDATE_SELECTED_FONT_COLOR);
			}

			// �����к�ѡ�ַ���һ�ι��ˣ����س����ո�ȷǿɼ��ַ������⴦�� - weizhiping/2010/10/28
			memset(drawCandBuf, 0, GB_INPUT_BUFFER_LEN);
			PT_ProprietarySymbolSwitch(pCand, drawCandBuf);

			//��ǰѡ��ĺ�ѡ����
			if(nCol == pEngineOutputInfo->nSelCol
				&& nRow == pEngineOutputInfo->nSelRow)
			{
				GBRECT clipRect;
				GBIMEPoint layoutWinStartPos;
				
				pltBase->m_pfcGetWinPos(pltObj, &layoutWinStartPos);
				FILL_RECT(&clipRect, (GBI16)(pCurCandRect->left), (GBI16)(pCurCandRect->top + GBIME_KB_CANDIDATE_TOP_PADDING + 1),
					(GBI16)(pCurCandRect->right),	(GBI16)(pCurCandRect->bottom));	
				
				//��ѡ��ǰѡ��ĺ�ѡ
				
				// ���ƺ�ѡ
				PT_RightAlignDrawString((GBINT)pCurCandPos->x + 2 * GBIME_KB_CANDIDATE_LEFT_PADDING, (GBINT)(pCurCandPos->y + GBIME_KB_CANDIDATE_TOP_PADDING),
					(GBINT)(pCurCandRect->right - pCurCandRect->left),
					drawCandBuf, GBIME_CANDIDATE_FONT_SIZE, GBIME_CANDIDATE_SELECTED_FONT_COLOR);
			}
			else
			{
				// ���ƺ�ѡ
				PT_RightAlignDrawString((GBINT)pCurCandPos->x + 2 * GBIME_KB_CANDIDATE_LEFT_PADDING, (GBINT)(pCurCandPos->y + GBIME_KB_CANDIDATE_TOP_PADDING),
					(GBINT)(pCurCandRect->right - pCurCandRect->left),
					drawCandBuf,
					GBIME_CANDIDATE_FONT_SIZE, 
					GBIME_CANDIDATE_NORMAL_FONT_COLOR);
			}
			
		}
	}
	PT_DisableProprietarySymbolDisplay();

#if __GBIME_LOG__ > 0
	//��ӡlog
	PT_PrintLogInfo(GBLOG_LAYOUTVIEW, ("\r\n��ѡ��"));
	for (nRow = 0; nRow < pltThis->candidateRegion.candidateNum && nRow < pEngineOutputInfo->nCandNum; nRow++)
	{
		GBACHAR buf[64] = {0};
		pCand = pEngineOutputInfo->ppCandidates[nRow];
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
static void ICLayoutViewKB__DrawPinYin(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo)
{
	CLASE_THIS_POITER(CLayoutKB, pltObj)
	CLASS_VIEW_THIS_POITER(CLayoutViewKB, pltThis)
	GBUINT16 i;
	GBPCWCHAR pCand = NULL;

	PGBRECT pCurSyllableRect = {0};
	PGBIMEPoint pCurSyllablePos = {0};
	GBUINT16 iCurrentLayoutId = pltBase->m_ltData.pltcData->ltID;

	PT_PrintLogInfo(GBLOG_LAYOUTVIEW, ("ICLayoutViewKB__DrawPinYin:cur pinyin index:%d\n", pltBase->currentSyllableIndex));
	
	for (i = 0; /*i < pltThis->syllableTouchRegion.candidateNum &&*/ i < pEngineOutputInfo->nSyllableNum; i++)
	{
		pCand = pEngineOutputInfo->ppSyllables[i];
		pCurSyllablePos = &pltThis->syllableRegion.candidatePosArray[i];
		pCurSyllableRect = &pltThis->syllableRegion.candidateRectArray[i];
		
		// ���ƺ�ѡ
		if (pEngineOutputInfo->nCurrentSyllableIndex/*pltBase->currentSyllableIndex*/ == i)
		{
			GBRECT clipRect;
			GBIMEPoint layoutWinStartPos;
			
			pltBase->m_pfcGetWinPos(pltObj, &layoutWinStartPos);
			FILL_RECT(&clipRect, (GBI16)(pCurSyllableRect->left + 1),
				(GBI16)(pCurSyllableRect->top + 1),
				pCurSyllableRect->right,
				(GBI16)(pCurSyllableRect->bottom - 2));
			
		//	PT_FillRect(clipRect.left, clipRect.top, clipRect.right, clipRect.bottom, GBIME_SYLLABLE_SELECTED_FONT_COLOR);
			
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
	}

#if __GBIME_LOG__ > 0
		//��ӡlog
		PT_PrintLogInfo(GBLOG_LAYOUTVIEW, ("\r\n���ڣ�"));
		for (i = 0; i < pEngineOutputInfo->nSyllableNum; i++)
		{
			GBACHAR buf[64] = {0};
			pCand = pEngineOutputInfo->ppSyllables[i];
			PT_wcstombs(buf, pCand, sizeof(buf));
			PT_PrintLogInfo(GBLOG_LAYOUTVIEW, ("%s, ", buf));
		}
		PT_PrintLogInfo(GBLOG_LAYOUTVIEW, ("\r\n"));
#endif
	
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
static void ICLayoutViewKB__DrawStroke(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo)
{
	CLASE_THIS_POITER(CLayoutKB, pltObj)
	GBUINT16 nAvailableWidth;
	GBUINT16 iKeyBoardWidth;
	GBUINT16 nVerticalSpacing = 0;

	// ��ȡ���̿��
	iKeyBoardWidth = pltBase->m_pfcGetLayoutWidth(pltObj, GET_LAYOUT_ID(pltBase));
	nAvailableWidth = iKeyBoardWidth - 2 * GBIME_KB_CANDIDATE_LEFT_PADDING;//���봮�����ʾ����
	nVerticalSpacing = (GBUINT16) ((GBIME_SYLLABLE_LINE_HEIGHT - PT_GetStrHeightEx(pEngineOutputInfo->pStroke, GBIME_STROKE_FONT_SIZE)) / 2);
	PT_RightAlignDrawStrokeString((GBINT)(pltBase->syllableDisplayStartPos.x + 1),
								  (GBINT)(pltBase->syllableDisplayStartPos.y + nVerticalSpacing),
								   nAvailableWidth,
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
static void ICLayoutViewKB__DrawComponents(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo)
{
	CLASE_THIS_POITER(CLayoutKB, pltObj)

	GBUINT16 iCount = 0;
	GBUINT16 iComponentsCount;
	GBUINT16 iKeyValue = 0;
	GBUINT16 pComponentString[2];
	GBUINT16 iCurrentLayoutId;
//	KEYBOARD_TYPE keyboardType = GBConfig_GetKeyboardType(Global_GetConfigInstance());
	GBINT16 compWidth, compHeight;
	PGBRECT pCurOutputStringRect = {0};
	PGBIMEPoint pCurOutputStringPos = {0};
	GBIMEPoint compPoint = {0};
	GBUINT16 iKeyBoardHeight = 0;
	GBUINT16 iKeyBoardWidth = 0;
	GBUINT16 nComponentWidth;

	PT_PrintLogInfo(GBLOG_LAYOUTVIEW, ("ICLayoutViewKB__DrawComponents\n"));


	pCurOutputStringPos = &pltThis->syllableRegion.candidatePosArray[0];
	pCurOutputStringRect = &pltThis->syllableRegion.candidateRectArray[0];

	// ��ǰLayoutId
	iCurrentLayoutId = GET_LAYOUT_ID(pltBase);
	// ��ȡ���̿��
	iKeyBoardWidth = pltBase->m_pfcGetLayoutWidth(pltObj, iCurrentLayoutId);

	//���"����ͷ��С��ͷ�޷���ʾ������"
	nComponentWidth = PT_GetComponentStringWidth(pEngineOutputInfo->pComponents, GBIME_COMPONENT_FONT_SIZE) + PT_GetStrWidthEx(L"7890", PT_FONT_SIZE_SMALL) + GBIME_KB_CANDIDATE_LEFT_PADDING;

	compPoint.x = pCurOutputStringPos->x + iKeyBoardWidth - nComponentWidth;
	compPoint.y = pCurOutputStringPos->y;

	iComponentsCount = 0;
	while (pEngineOutputInfo->pComponents[iComponentsCount])
	{
		pComponentString[0] = pEngineOutputInfo->pComponents[iComponentsCount];
		pComponentString[1] = 0;
		
		compWidth = PT_GetComponentStringWidth(pComponentString, GBIME_COMPONENT_FONT_SIZE);
		compHeight = GBIME_COMP_HEIGHT;//PT_GetStrHeightEx(pComponentString, GBIME_COMPONENT_FONT_SIZE);
		
		// ������
		PT_PrintStrokeText(pComponentString, compPoint.x, compPoint.y + 5, GBIME_COMPONENT_FONT_SIZE,GBIME_COMPONENT_COLOR);
		
		compPoint.x += compWidth;

		// ��ݲ�����ʾ����Ӧ��ť
		iKeyValue = iComponentsCount + '7';
		if(iKeyValue > '9')
		{
			iKeyValue = '0';
		}		
		pComponentString[0] = iKeyValue;
		pComponentString[1] = 0;

		compWidth = PT_GetComponentStringWidth(pComponentString, PT_FONT_SIZE_SMALL);
		compHeight = GBIME_COMP_HEIGHT;//PT_GetStrHeightEx(pComponentString, PT_FONT_SIZE_SMALL);		
		
		// �����׶�Ӧ����
		PT_PrintStrokeText(pComponentString, compPoint.x, compPoint.y, PT_FONT_SIZE_SMALL,GBIME_COMPONENT_COLOR);
		compPoint.x += compWidth;

		iComponentsCount++;
	}
	return;
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
static void ICLayoutViewKB__DrawStrokeUpLine(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo)
{
	CLASE_THIS_POITER(CLayoutKB, pltObj)
	GBINT nAvailableWidth;// ����ʾ���ܿ��
	GBINT nUseWidth = 0;
 	GBINT left;
 	GBINT top;
	GBIMEPoint layoutStartPoint;
	PGBRECT pCurOutputStringRect = {0};
	PGBIMEPoint pCurOutputStringPos = {0};
	GBUINT16 iCurrentLayoutId;
	GBUINT16 iKeyBoardWidth;
	GBIMEPoint strokeStartPos = {0};
	GBUINT16 iCount = 0;
	GBINT nComponentWidth = 0;
	GBINT nStrokeWidth = 0;
	GBINT nOutputStringWidth = 0;
	GBINT nSpacing = 6;

	pltBase->m_pfcGetLayoutStartPoint(pltObj, &layoutStartPoint);

	// ��ǰLayoutId
	iCurrentLayoutId = GET_LAYOUT_ID(pltBase);
	// ��ȡ���̿��
	iKeyBoardWidth = pltBase->m_pfcGetLayoutWidth(pltObj, iCurrentLayoutId);

	pCurOutputStringPos = &pltThis->syllableRegion.candidatePosArray[0];
	pCurOutputStringRect = &pltThis->syllableRegion.candidateRectArray[0];	
	
	// �жϲ��׿��
	if(pEngineOutputInfo->pComponents && pEngineOutputInfo->pComponents[0] != 0)
	{
		nComponentWidth = PT_GetComponentStringWidth(pEngineOutputInfo->pComponents, GBIME_COMPONENT_FONT_SIZE) + PT_GetStrWidthEx(L"7890", PT_FONT_SIZE_SMALL);
	}
	else
	{
		nComponentWidth = 0;
	}

	// �������봮�ͱʻ����
	nOutputStringWidth = PT_GetStrWidthEx(pEngineOutputInfo->pOutputString, GBIME_STROKE_FONT_SIZE);
	nStrokeWidth = PT_GetStrWidthEx(pEngineOutputInfo->pStroke, GBIME_STROKE_FONT_SIZE);
	nAvailableWidth = iKeyBoardWidth - nComponentWidth - 2 * nSpacing;
	
	// ���봮��ʼλ��
	left = layoutStartPoint.x + nSpacing;
	top = pCurOutputStringPos->y;

	// �������봮
	nUseWidth = PT_RightAlignDrawString(left, top, nAvailableWidth,	pEngineOutputInfo->pOutputString, GBIME_STROKE_FONT_SIZE, GBIME_OUTPUT_STRING_COLOR);	
	strokeStartPos.x = left + nUseWidth;
	strokeStartPos.y = top;
	nAvailableWidth -= nUseWidth;

	if (nAvailableWidth > 2 * nSpacing)
	{
		strokeStartPos.x += nSpacing;

		// ����һ����ֱ�ָ��߲�������ʾ�ʻ�
		PT_DrawVerticalLineType((GBU16)(pltBase->syllableDisplayStartPos.y + 1),
								(GBU16)(pltBase->syllableDisplayStartPos.y + GBIME_SYLLABLE_LINE_HEIGHT - GBIME_KB_CANDIDATE_TOP_PADDING),
								(GBU16)strokeStartPos.x, PT_COLOR_LIGHT_GREY, PT_LINT_TYPE_DOTTED);

		strokeStartPos.x += nSpacing;
		nAvailableWidth -= 4 * nSpacing;

		// ���ʻ���
		PT_RightAlignDrawStrokeString(strokeStartPos.x, strokeStartPos.y, nAvailableWidth, pEngineOutputInfo->pStroke, GBIME_STROKE_FONT_SIZE,	GBIME_STROKE_COLOR);
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
static void ICLayoutViewKB__RedrawArrowButton(GBLPVOID pltObj, 
											 GBUINT16 iStartX, 
											 GBUINT16 iStartY, 
											 GBUINT16 layerId,
											 PEngineOutputInfo pEngineOutputInfo)
{
	CLASE_THIS_POITER(CLayoutKB, pltObj)
	CLASS_VIEW_THIS_POITER(CLayoutViewKB, pltThis)
	GBUINT16 iCurrentLayoutId = pltBase->m_ltData.pltcData->ltID;

	///<���ϼ�����
	if (pEngineOutputInfo->bUpScreenArrow)
	{
		pltViewBase->m_pfcDrawArrowButton(pltObj, iStartX, iStartY,
			iCurrentLayoutId, layerId, 
			GBKEY_UP, ButtonStatusNomal);
	}
	else
	{
		pltViewBase->m_pfcDrawArrowButton(pltObj, iStartX, iStartY,
			iCurrentLayoutId, layerId, 
			GBKEY_UP, ButtonStatusGray);
	}	

	///<ƴ����ҳ��ʾ���ͷ
	if (pEngineOutputInfo->bSyllableArrowLeft)
	{		
		pltViewBase->m_pfcDrawArrowButton(pltObj, iStartX, iStartY,
			iCurrentLayoutId, layerId, 
			GBKEY_PREV_PAGE_SYLLABLE, ButtonStatusNomal);
	} 
	else
	{
		pltViewBase->m_pfcDrawArrowButton(pltObj, iStartX, iStartY,
			iCurrentLayoutId, layerId, 
			GBKEY_PREV_PAGE_SYLLABLE, ButtonStatusGray);
	}

	///<ƴ����ҳ��ʾ�Ҽ�ͷ
	if (pEngineOutputInfo->bSyllableArrowRight)
	{
		pltViewBase->m_pfcDrawArrowButton(pltObj, iStartX, iStartY,
			iCurrentLayoutId, layerId, 
			GBKEY_NEXT_PAGE_SYLLABLE, ButtonStatusNomal);
	} 
	else
	{
		pltViewBase->m_pfcDrawArrowButton(pltObj, iStartX, iStartY,
			iCurrentLayoutId, layerId, 
			GBKEY_NEXT_PAGE_SYLLABLE, ButtonStatusGray);
	}

	///<��һҳ
	if (pEngineOutputInfo->bCandArrowUp)
	{
		pltViewBase->m_pfcDrawArrowButton(pltObj, iStartX, iStartY,
			iCurrentLayoutId, layerId, 
			GBKEY_PREV_PAGE_CANDIATE, ButtonStatusNomal);
	} 
	else
	{
		pltViewBase->m_pfcDrawArrowButton(pltObj, iStartX, iStartY,
			iCurrentLayoutId, layerId, 
			GBKEY_PREV_PAGE_CANDIATE, ButtonStatusGray);
	}

	///<��һҳ
	if (pEngineOutputInfo->bCandArrowDown)
	{
		pltViewBase->m_pfcDrawArrowButton(pltObj, iStartX, iStartY,
			iCurrentLayoutId, layerId, 
			GBKEY_NEXT_PAGE_CANDIATE, ButtonStatusNomal);
	} 
	else
	{
		pltViewBase->m_pfcDrawArrowButton(pltObj, iStartX, iStartY,
			iCurrentLayoutId, layerId, 
 			GBKEY_NEXT_PAGE_CANDIATE, ButtonStatusGray);
	}

//	PT_Invalidate(0, 0, PT_GetLcdWidth(), PT_GetLcdHeight());
}

#endif // �����������