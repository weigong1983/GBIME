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
* Purpose      :  虚拟键盘Layout视图类
* Version      :  V1.0
* Last Changed List
* Name         |   date   |  Discription
****************************************************************************/
#if defined(__GBIME_KB__) // 国笔物理键盘

#include "CLayoutViewKB.h"
#include "CLayoutKB.h"
#include "PlatformAdapter.h"
#include "GB_Global.h"
#include "GBIMEResDef.h"

/*!
 * \ Layout View 指针预定义
 */
#define CLASS_VIEW_THIS_POITER(CLASS_NAME, pltThis)   \
	CLASS_NAME * pltViewThis = (CLASS_NAME *)pltThis->m_ltParent.m_hltView; \
	PCLayoutViewAbstract pltViewBase = &pltViewThis->m_ltParent; \

/*!
 * \brief 公共视图显示成员函数
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
 * \brief 画键盘【第一层】
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

	//当前要显示的层
	GBUINT16 iCurrentLayerNumber;
	//显示图片的左上角X坐标
	GBUINT16 iDisplayImageLeftX;
	//显示图片的左上角Y坐标
	GBUINT16 iDisplayImageTopY;
	//当前要显示的层的正常状态下的图片ID
	GBUINT16 iCurrentLayerNomalImageId;
	//当前要显示的层的按下状态下的图片ID
	GBUINT16 iCurrentLayerDownImageId;
	//候选行数
//	GBUINT16 iCandidateRowCount = 1;
	//当前按钮对应的图片ID
//	GBUINT16 iCurrentButtonImageId;
	//当前按钮的按钮索引
//	GBUINT16 iChangeButtonIndex;
	//按钮总个数
//	GBUINT16 iButtonTotalCount;
//	GBUINT16 iCount;
	//是否有非正常状态按钮
//	GBUINT8 bHasButtonChange = 0;
//	GBUINT16 currentButtonStatus = 0;
	GBIMEPoint winPos;
//	GBRECT screenRect,imageRect;
//	GBUINT16 iCandidateBoxHeight = 0;
	GBUINT16 iKeyBoardHeight = 0;
	GBUINT16 iKeyBoardWidth = 0;
	PLayoutInfo pLayoutInfo = (PLayoutInfo)(&pltBase->m_ltData); // Layout UI data point
	GBUINT16 iCurrentLayoutId;
//	PLayoutChgtLayer pltFourthLayerData = NULL; // 第四层数据

	PT_PrintLogInfo(GBLOG_LAYOUTVIEW, ("ICLayoutViewKB__DrawKeyboard\n"));

	iCurrentLayoutId = GET_LAYOUT_ID(pltBase);
	// 获取键盘高度
	iKeyBoardHeight = pltBase->m_pfcGetLayoutHeight(pltObj,iCurrentLayoutId);
	iKeyBoardWidth = pltBase->m_pfcGetLayoutWidth(pltObj, iCurrentLayoutId);

	// 取得键盘显示的开始位置【左下角】，并调整为【左上角】
	Global_GetWinPos(&winPos);	
	winPos.y -=  iKeyBoardHeight;
	
	iScreenWidth = PT_GetLcdWidth();
	//iScreenWidth = Interface_GetWinWidth();

	// 当前要显示的层
	iCurrentLayerNumber = pLayoutInfo->ltCurLayer;
	// 取背景图片相关信息
	// 显示图片的左上角X坐标
	iDisplayImageLeftX = (GBUINT16)winPos.x + (iScreenWidth - iKeyBoardWidth) / 2;
	// 显示图片的左上角Y坐标
	iDisplayImageTopY =  (GBUINT16)winPos.y;
	// 背景图片正常状态下的图片ID
	iCurrentLayerNomalImageId = pLayoutInfo->pltLayerData[FirstLayer].plycData->lyImageID + IMG_GBIME_SYSTEM;//加上图片ID基址
	// 背景图片按下状态下的图片ID
	iCurrentLayerDownImageId = pLayoutInfo->pltLayerData[FirstLayer].plycData->lyBKImageID + IMG_GBIME_SYSTEM;//加上图片ID基址
	
	// 显示背景图片即第0层]
	PT_DrawImage(iDisplayImageLeftX,iDisplayImageTopY,iCurrentLayerNomalImageId);
	return;
}

/*!
 * \brief 绘制输出串
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

	// 获取键盘宽度
	iKeyBoardWidth = pltBase->m_pfcGetLayoutWidth(pltObj, GET_LAYOUT_ID(pltBase));
	nAvailableWidth = iKeyBoardWidth - 2 * GBIME_KB_CANDIDATE_LEFT_PADDING;//输入串最大显示区域
	nVerticalSpacing = (GBUINT16) ((GBIME_SYLLABLE_LINE_HEIGHT - PT_GetStrHeightEx(pEngineOutputInfo->pOutputString, GBIME_SYLLABLE_FONT_SIZE)) / 2);

	PT_RightAlignDrawString((GBINT)(pltBase->syllableDisplayStartPos.x + GBIME_KB_CANDIDATE_LEFT_PADDING),
							(GBINT)(pltBase->syllableDisplayStartPos.y + nVerticalSpacing + GBIME_KB_CANDIDATE_TOP_PADDING), 
							nAvailableWidth, 
							pEngineOutputInfo->pOutputString, 
							GBIME_SYLLABLE_FONT_SIZE, 
							GBIME_OUTPUT_STRING_COLOR);
}

/*!
 * \brief 绘制候选字
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

// 	// abc 通过按键以Multitap方式在候选框中选择候选
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

			//选择状态时画下标
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

			// 对所有候选字符做一次过滤，将回车、空格等非可见字符做特殊处理 - weizhiping/2010/10/28
			memset(drawCandBuf, 0, GB_INPUT_BUFFER_LEN);
			PT_ProprietarySymbolSwitch(pCand, drawCandBuf);

			//当前选择的候选高亮
			if(nCol == pEngineOutputInfo->nSelCol
				&& nRow == pEngineOutputInfo->nSelRow)
			{
				GBRECT clipRect;
				GBIMEPoint layoutWinStartPos;
				
				pltBase->m_pfcGetWinPos(pltObj, &layoutWinStartPos);
				FILL_RECT(&clipRect, (GBI16)(pCurCandRect->left), (GBI16)(pCurCandRect->top + GBIME_KB_CANDIDATE_TOP_PADDING + 1),
					(GBI16)(pCurCandRect->right),	(GBI16)(pCurCandRect->bottom));	
				
				//反选当前选择的候选
				
				// 绘制候选
				PT_RightAlignDrawString((GBINT)pCurCandPos->x + 2 * GBIME_KB_CANDIDATE_LEFT_PADDING, (GBINT)(pCurCandPos->y + GBIME_KB_CANDIDATE_TOP_PADDING),
					(GBINT)(pCurCandRect->right - pCurCandRect->left),
					drawCandBuf, GBIME_CANDIDATE_FONT_SIZE, GBIME_CANDIDATE_SELECTED_FONT_COLOR);
			}
			else
			{
				// 绘制候选
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
	//打印log
	PT_PrintLogInfo(GBLOG_LAYOUTVIEW, ("\r\n候选："));
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
 * \brief 9键拼音时，画拼音
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
		
		// 绘制候选
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
		//打印log
		PT_PrintLogInfo(GBLOG_LAYOUTVIEW, ("\r\n音节："));
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
 * \brief 绘制笔画
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

	// 获取键盘宽度
	iKeyBoardWidth = pltBase->m_pfcGetLayoutWidth(pltObj, GET_LAYOUT_ID(pltBase));
	nAvailableWidth = iKeyBoardWidth - 2 * GBIME_KB_CANDIDATE_LEFT_PADDING;//输入串最大显示区域
	nVerticalSpacing = (GBUINT16) ((GBIME_SYLLABLE_LINE_HEIGHT - PT_GetStrHeightEx(pEngineOutputInfo->pStroke, GBIME_STROKE_FONT_SIZE)) / 2);
	PT_RightAlignDrawStrokeString((GBINT)(pltBase->syllableDisplayStartPos.x + 1),
								  (GBINT)(pltBase->syllableDisplayStartPos.y + nVerticalSpacing),
								   nAvailableWidth,
								   pEngineOutputInfo->pStroke,
							       GBIME_STROKE_FONT_SIZE,
								   GBIME_STROKE_COLOR);
}

/*!
 * \brief 绘制快捷部首
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

	// 当前LayoutId
	iCurrentLayoutId = GET_LAYOUT_ID(pltBase);
	// 获取键盘宽度
	iKeyBoardWidth = pltBase->m_pfcGetLayoutWidth(pltObj, iCurrentLayoutId);

	//解决"竹子头和小字头无法显示的问题"
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
		
		// 画部首
		PT_PrintStrokeText(pComponentString, compPoint.x, compPoint.y + 5, GBIME_COMPONENT_FONT_SIZE,GBIME_COMPONENT_COLOR);
		
		compPoint.x += compWidth;

		// 快捷部首显示到对应按钮
		iKeyValue = iComponentsCount + '7';
		if(iKeyValue > '9')
		{
			iKeyValue = '0';
		}		
		pComponentString[0] = iKeyValue;
		pComponentString[1] = 0;

		compWidth = PT_GetComponentStringWidth(pComponentString, PT_FONT_SIZE_SMALL);
		compHeight = GBIME_COMP_HEIGHT;//PT_GetStrHeightEx(pComponentString, PT_FONT_SIZE_SMALL);		
		
		// 画部首对应按键
		PT_PrintStrokeText(pComponentString, compPoint.x, compPoint.y, PT_FONT_SIZE_SMALL,GBIME_COMPONENT_COLOR);
		compPoint.x += compWidth;

		iComponentsCount++;
	}
	return;
}

/*!
 * \brief 绘制笔画输入栏
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
	GBINT nAvailableWidth;// 可显示的总宽度
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

	// 当前LayoutId
	iCurrentLayoutId = GET_LAYOUT_ID(pltBase);
	// 获取键盘宽度
	iKeyBoardWidth = pltBase->m_pfcGetLayoutWidth(pltObj, iCurrentLayoutId);

	pCurOutputStringPos = &pltThis->syllableRegion.candidatePosArray[0];
	pCurOutputStringRect = &pltThis->syllableRegion.candidateRectArray[0];	
	
	// 判断部首宽度
	if(pEngineOutputInfo->pComponents && pEngineOutputInfo->pComponents[0] != 0)
	{
		nComponentWidth = PT_GetComponentStringWidth(pEngineOutputInfo->pComponents, GBIME_COMPONENT_FONT_SIZE) + PT_GetStrWidthEx(L"7890", PT_FONT_SIZE_SMALL);
	}
	else
	{
		nComponentWidth = 0;
	}

	// 计算输入串和笔画宽度
	nOutputStringWidth = PT_GetStrWidthEx(pEngineOutputInfo->pOutputString, GBIME_STROKE_FONT_SIZE);
	nStrokeWidth = PT_GetStrWidthEx(pEngineOutputInfo->pStroke, GBIME_STROKE_FONT_SIZE);
	nAvailableWidth = iKeyBoardWidth - nComponentWidth - 2 * nSpacing;
	
	// 输入串起始位置
	left = layoutStartPoint.x + nSpacing;
	top = pCurOutputStringPos->y;

	// 绘制输入串
	nUseWidth = PT_RightAlignDrawString(left, top, nAvailableWidth,	pEngineOutputInfo->pOutputString, GBIME_STROKE_FONT_SIZE, GBIME_OUTPUT_STRING_COLOR);	
	strokeStartPos.x = left + nUseWidth;
	strokeStartPos.y = top;
	nAvailableWidth -= nUseWidth;

	if (nAvailableWidth > 2 * nSpacing)
	{
		strokeStartPos.x += nSpacing;

		// 绘制一条垂直分割线并接着显示笔画
		PT_DrawVerticalLineType((GBU16)(pltBase->syllableDisplayStartPos.y + 1),
								(GBU16)(pltBase->syllableDisplayStartPos.y + GBIME_SYLLABLE_LINE_HEIGHT - GBIME_KB_CANDIDATE_TOP_PADDING),
								(GBU16)strokeStartPos.x, PT_COLOR_LIGHT_GREY, PT_LINT_TYPE_DOTTED);

		strokeStartPos.x += nSpacing;
		nAvailableWidth -= 4 * nSpacing;

		// 画笔画串
		PT_RightAlignDrawStrokeString(strokeStartPos.x, strokeStartPos.y, nAvailableWidth, pEngineOutputInfo->pStroke, GBIME_STROKE_FONT_SIZE,	GBIME_STROKE_COLOR);
	}
}

/*!
* \brief 根据引擎状态重绘箭头按钮
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

	///<按上键上屏
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

	///<拼音翻页显示左箭头
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

	///<拼音翻页显示右箭头
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

	///<上一页
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

	///<下一页
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

#endif // 国笔物理键盘