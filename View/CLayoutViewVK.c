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
* Purpose      :  虚拟键盘Layout视图类
* Version      :  V1.0
* Last Changed List
* Name         |   date   |  Discription
****************************************************************************/
#if defined(__GBIME_VK__) // 国笔虚拟键盘

#include "CLayoutViewVK.h"
#include "CLayoutVK.h"
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
 * \brief 画键盘【第一层】
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
	GBUINT16 iCandidateRowCount = 1;
	//当前按钮对应的图片ID
	GBUINT16 iCurrentButtonImageId;
	//当前按钮的按钮索引
	GBUINT16 iChangeButtonIndex;
	//按钮总个数
	GBUINT16 iButtonTotalCount;
	GBUINT16 iCount;
	//是否有非正常状态按钮
	GBUINT8 bHasButtonChange = 0;
	GBUINT16 currentButtonStatus = 0;
	GBIMEPoint winPos;
	GBRECT screenRect,imageRect;
	GBUINT16 iCandidateBoxHeight = 0;
	GBUINT16 iKeyBoardHeight = 0;
	PLayoutInfo pLayoutInfo = (PLayoutInfo)(&pltBase->m_ltData); // Layout UI data point
	GBUINT16 iCurrentLayoutId;
	PLayoutChgtLayer pltFourthLayerData = NULL; // 第四层数据

	iCurrentLayoutId = GET_LAYOUT_ID(pltBase);
	// 获取键盘高度
	iKeyBoardHeight = pltBase->m_pfcGetLayoutHeight(pltObj,iCurrentLayoutId);
	// 取得键盘显示的开始位置【左下角】，并调整为【左上角】
	Global_GetWinPos(&winPos);	
	winPos.y -=  iKeyBoardHeight;
	
	// 当前要显示的层
	iCurrentLayerNumber = pLayoutInfo->ltCurLayer;
	// 取背景图片相关信息
	// 显示图片的左上角X坐标
	iDisplayImageLeftX = (GBUINT16)winPos.x;
	// 显示图片的左上角Y坐标
	iDisplayImageTopY =  (GBUINT16)winPos.y;
	// 背景图片正常状态下的图片ID
	iCurrentLayerNomalImageId = pLayoutInfo->pltLayerData[FirstLayer].plycData->lyImageID + IMG_GBIME_SYSTEM;//加上图片ID基址
	// 背景图片按下状态下的图片ID
	iCurrentLayerDownImageId = pLayoutInfo->pltLayerData[FirstLayer].plycData->lyBKImageID + IMG_GBIME_SYSTEM;//加上图片ID基址
	
	// 显示背景图片即第0层]
	PT_DrawImage(iDisplayImageLeftX,iDisplayImageTopY,iCurrentLayerNomalImageId);
	
	// 第四层数据不为空
// 	if ((pltFourthLayerData = pLayoutInfo->pltLayerData[FourthLayer]) != NULL)
// 	{
// 		pltViewBase->m_pfcDrawButtonByStatus(pltObj, iDisplayImageLeftX, iDisplayImageTopY, NULL,
// 										     iCurrentLayoutId, FourthLayer, 
// 											 GBKEY_PREV_PAGE_SYLLABLE, ButtonStatusNomal);
// 	}


	// 打印Log信息
	//PT_PrintLogInfo(GBLOG_LAYOUTVIEW, ("***********************ICLayoutViewVK26MultiLang_PaintLayerOne() Draw Keyboard\n"));
	//PT_PrintLogInfo(GBLOG_LAYOUTVIEW, ("***********************GBLOG_LAYOUTVIEW Image ID = %d\n", iCurrentLayerNomalImageId));

	// 得到按钮总数
	iButtonTotalCount = pLayoutInfo->pltLayerData[FirstLayer].lyBtCount;
	// 查找是否有变化的按钮(不是正常状态)
	for (iCount = 0;iCount < iButtonTotalCount; iCount++)
	{
		PBUTTONCHGDATA plyTempChgBt;
		plyTempChgBt = &pLayoutInfo->pltLayerData[FirstLayer].plyBtChgData[iCount];
		// btStatus : 按键状态，0为正常态，1为按下，2为灰色，3隐藏当前按键
		// 发现状态不是0时，就表示有变化的按钮
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

	// 如果有变化的按钮
	if (bHasButtonChange)
	{
		GBLPCBUTTONCONSTDATA plyTempConstBt = &pLayoutInfo->pltLayerData[FirstLayer].plycBtData[iChangeButtonIndex];
		// 取按钮在图片中的位置
		memcpy(&imageRect, &plyTempConstBt->btPos, sizeof(GBRECT));
		
		// 图片在屏幕上显示的位置
		memcpy(&screenRect, &imageRect, sizeof(GBRECT));
		MOVE_RECT(&screenRect, winPos.x, iDisplayImageTopY);		
		// 根据按键的不同状态来取其相应的图片ID
		if ((currentButtonStatus) == ButtonStatusNomal || (currentButtonStatus == ButtonStatusUp))
		{
			iCurrentButtonImageId = plyTempConstBt->btNormal.imageID + IMG_GBIME_SYSTEM;//加上图片ID基址
		}
		else if (currentButtonStatus == ButtonStatusDown || currentButtonStatus == ButtonStatusMove)
		{
			iCurrentButtonImageId = plyTempConstBt->btDown.imageID + IMG_GBIME_SYSTEM;//加上图片ID基址
		}
		else if(currentButtonStatus == ButtonStatusGray)
		{
			iCurrentButtonImageId = plyTempConstBt->btGrey.imageID + IMG_GBIME_SYSTEM;//加上图片ID基址	
		}
		else// 默认
		{
			iCurrentButtonImageId = plyTempConstBt->btNormal.imageID + IMG_GBIME_SYSTEM;//加上图片ID基址
		}

		PT_DrawImageWithClip(iCurrentButtonImageId, &screenRect, &imageRect);

		// 打印Log信息
// 		PT_PrintLogInfo(GBLOG_LAYOUTVIEW, ("***********************ICLayoutViewVK26MultiLang_PaintLayerOne() Draw Button\n"));
// 		PT_PrintLogInfo(GBLOG_LAYOUTVIEW, ("***********************Button Index = %d\t",iChangeButtonIndex));
// 		PT_PrintLogInfo(GBLOG_LAYOUTVIEW, ("***********************Conversion To Engine Key Value is 0x%4x\n",currentButtonStatus));
	}

	// 绘制灰色禁用按钮
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

	// 重绘ShiftCap按钮
	ICLayoutViewVK__RedrawShiftCapButton(pltObj, pEngineOutputInfo);
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
 * \brief 绘制候选字
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

	// abc 通过按键以Multitap方式在候选框中选择候选
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
	// 画候选汉字
	for (i = 0; i < pltThis->candidateTouchRegion.candidateNum && i < pEngineOutputInfo->nCandNum; i++)
	{
		pCand = pEngineOutputInfo->ppCandidates[i];
		pCurCandPos = &pltThis->candidateTouchRegion.candidatePosArray[i];
		pCurCandRect = &pltThis->candidateTouchRegion.candidateRectArray[i];

		
		// 对所有候选字符做一次过滤，将回车、空格等非可见字符做特殊处理 - weizhiping/2010/10/28
		memset(drawCandBuf, 0, GB_INPUT_BUFFER_LEN);
		//wcsncpy(drawCandBuf, pCand, wcslen(pCand));
		PT_ProprietarySymbolSwitch(pCand, drawCandBuf);
// 		for(j = 0; j < wcslen(pCand); j++)
// 		{
// 			switch (drawCandBuf[j])
// 			{
// 			case 0x0020: // 空格
// 				drawCandBuf[j] = 0x0086;
// 				break;
// 			case 0x000A: // 回车
// 				drawCandBuf[j] = 0x0085;
// 				break;
// 			default:
// 				break;
// 			}
// 		}

		// 绘制候选
		if (focusCandidateIndex == i)
		{
			// 候选按钮反显
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

			// 绘制候选
			PT_RightAlignDrawString((GBINT)pCurCandPos->x, (GBINT)(pCurCandPos->y),
									(GBINT)(pCurCandRect->right - pCurCandRect->left),
									drawCandBuf, 
									GBIME_CANDIDATE_FONT_SIZE, 
									GBIME_CANDIDATE_SELECTED_FONT_COLOR);
		}
		else
		{
			// 绘制候选
			PT_RightAlignDrawString((GBINT)pCurCandPos->x, (GBINT)(pCurCandPos->y),
									(GBINT)(pCurCandRect->right - pCurCandRect->left),
									drawCandBuf,
									GBIME_CANDIDATE_FONT_SIZE, 
									GBIME_CANDIDATE_NORMAL_FONT_COLOR);
		}
		
		// 绘制垂直候选分割线[最后一个不画]
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
	//打印log
	PT_PrintLogInfo(GBLOG_LAYOUTVIEW, ("\r\n候选："));
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
 * \brief 9键拼音时，画拼音
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

	// 如果是翻页
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
		// 画候选时用到
		pltBase->m_pfcGetBottonRectByKey(pltObj,
										 iCurrentLayoutId,
										 SecondLayer,
										 GBKEY_PREV_PAGE_CANDIATE,
										 &buttonRect);
	}

	// 符号输入框下面绘制输出串到输入栏
	if (pEngineOutputInfo->isSymbolCandType/*GBEngine_IsSymbolCandType(Global_GetEnineInstance())*/)
	{
		if (pltThis->syllableTouchRegion.candidateNum == 1 
			&& pEngineOutputInfo->pOutputString != NULL)
		{			
			pCand = pEngineOutputInfo->pOutputString;
			pCurSyllablePos = &pltThis->syllableTouchRegion.candidatePosArray[0];
			pCurSyllableRect = &pltThis->syllableTouchRegion.candidateRectArray[0];
		
			// 按钮反显
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
	else // 画候选拼音
	{		
		for (i = 0; i < pltThis->syllableTouchRegion.candidateNum && pEngineOutputInfo->nSyllableNum; i++)
		{
			pCand = pEngineOutputInfo->ppSyllables[i];
			pCurSyllablePos = &pltThis->syllableTouchRegion.candidatePosArray[i];
			pCurSyllableRect = &pltThis->syllableTouchRegion.candidateRectArray[i];
			
			// 绘制候选
			if (pltBase->currentSyllableIndex == i)
			{
				// 候选按钮反显
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
			
			// 绘制垂直候选分割线[最后一个不画]
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
		//打印log
		PT_PrintLogInfo(GBLOG_LAYOUTVIEW, ("\r\n音节："));
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
 * \brief 绘制笔画
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
 * \brief 绘制快捷部首
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

	// 取得键盘显示的开始位置[左下角坐标]
	Global_GetWinPos(&winPos);
	iCurrentLayoutId = GET_LAYOUT_ID(pltBase);
	// 键盘高度
	iKeyBoardHeight = pltBase->m_pfcGetLayoutHeight(pltObj, iCurrentLayoutId);
	// 相减得到键盘左上角Y坐标
	winPos.y -= iKeyBoardHeight;
	
	iComponentsCount = 0;
	while (pEngineOutputInfo->pComponents[iComponentsCount])
	{
		iComponentsCount++;
	}

	//PT_EnableProprietarySymbolDisplay();	
	// 画快捷部首,从第7个键开始
	for (iCount = 0; iCount< iComponentsCount; iCount++)
	{
		pComponentString[0] = pEngineOutputInfo->pComponents[iCount];
		pComponentString[1] = 0;

		// 快捷部首显示到对应按钮
		if (keyboardType == KEYBOARD_VK_QWERTY)
		{
			iKeyValue = (GBUINT16)(GBKEY_VK_7_NUMBERKEY + iCount);
		} 
		else
		{
			iKeyValue = (GBUINT16)((iCount < 3) ? (GBKEY_7 + iCount) : GBKEY_0);
		}		

		// 取按钮的区域
		bTrue = pltBase->m_pfcGetBottonRectByKey(pltObj, iCurrentLayoutId, FirstLayer, iKeyValue, &buttonRect);
		if (bTrue)
		{
			GBINT x, y;
			GBINT16 compWidth = PT_GetStrWidthEx(pComponentString, GBIME_COMPONENT_FONT_SIZE);
			GBINT16 compHeight = PT_GetStrHeightEx(pComponentString, GBIME_COMPONENT_FONT_SIZE);			
			
			MOVE_RECT(&buttonRect, winPos.x, winPos.y);

			// 显示部首在按钮中间
			x = (GBINT)(buttonRect.left + (buttonRect.right - buttonRect.left - compWidth) / 2);
			y = (GBINT)(buttonRect.top + (buttonRect.bottom - buttonRect.top - compHeight) / 2);

			// 将回车、空格等非可见字符做特殊处理 - weizhiping/2010/10/28
			//memset(drawCandBuf, 0, GB_INPUT_BUFFER_LEN);
			//PT_ProprietarySymbolSwitch(pComponentString, drawCandBuf);
			//PT_DrawString(pComponentString, GBIME_COMPONENT_FONT_SIZE,GBIME_COMPONENT_COLOR, x, y);
			PT_PrintStrokeText(pComponentString, x, y, GBIME_COMPONENT_FONT_SIZE, GBIME_COMPONENT_COLOR);
		}
	}
	//PT_DisableProprietarySymbolDisplay();
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
static void ICLayoutViewVK__DrawStrokeUpLine(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo)
{
	CLASE_THIS_POITER(CLayoutVK, pltObj)
	GBINT nAvailableWidth = GBIME_CAND_AREA_WIDTH; // 可显示的总宽度
	GBINT nUseWidth = 0;
	GBINT nVerticalSpacing = (GBUINT16) ((GBIME_SYLLABLE_LINE_HEIGHT - 
								PT_GetStrHeightEx(pEngineOutputInfo->pStroke, GBIME_STROKE_FONT_SIZE)) / 2);
	GBINT left = (GBINT)(pltBase->syllableDisplayStartPos.x + 1);
	GBINT top = (GBINT)(pltBase->syllableDisplayStartPos.y + nVerticalSpacing);
	GBINT nSpacing = 5;

	// 绘制输入串
	if (pEngineOutputInfo->pOutputString != NULL && pEngineOutputInfo->pOutputString[0])
	{
		nUseWidth = PT_RightAlignDrawString(left, top, nAvailableWidth,	pEngineOutputInfo->pOutputString, 
			GBIME_STROKE_FONT_SIZE,	GBIME_OUTPUT_STRING_COLOR);

		nAvailableWidth = GBIME_CAND_AREA_WIDTH - nUseWidth - 4 * nSpacing;

		// 绘制一条垂直分割线并接着显示笔画
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
static void ICLayoutViewVK__RedrawArrowButton(GBLPVOID pltObj, 
											 GBUINT16 iStartX, 
											 GBUINT16 iStartY, 
											 GBUINT16 layerId,
											 PEngineOutputInfo pEngineOutputInfo)
{
	CLASE_THIS_POITER(CLayoutVK, pltObj)
	CLASS_VIEW_THIS_POITER(CLayoutViewVK, pltThis)
	GBUINT16 iCurrentLayoutId = pltBase->m_ltData.pltcData->ltID;

	///<拼音翻页显示左箭头
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

	///<拼音翻页显示右箭头
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

	///<上一页
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

	///<下一页
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
	
	// 取得键盘显示的开始位置[左下角坐标]
	Global_GetWinPos(&winPos);
	iCurrentLayoutId = GET_LAYOUT_ID(pltBase);
	// 键盘高度
	iKeyBoardHeight = pltBase->m_pfcGetLayoutHeight(pltObj, iCurrentLayoutId);
	// 相减得到键盘左上角Y坐标
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
 * \brief 重绘第二、三层中逻辑上属于第一层的按钮（如拼音分隔符等...）
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
		// 如果不是候选框区域中的按钮
		if (pLayoutChgtLayer->plycBtData[i].btPos.bottom > iCandidateBoxHeight)
		{
			buttonStatus = pLayoutChgtLayer->plyBtChgData[i].btStatus;

			// 判断是否选中该按钮
			if (pLayoutChgtLayer->plycBtData[i].btKey == pltBase->currentLayoutKeyValue
				&& (buttonStatus == ButtonStatusDown || buttonStatus == ButtonStatusMove))				   
			{
				iCurrentButtonImageId = pLayoutChgtLayer->plycBtData[i].btDown.imageID + IMG_GBIME_SYSTEM;//加上图片ID基址;
			}
			else
			{
				iCurrentButtonImageId = pLayoutChgtLayer->plycBtData[i].btNormal.imageID + IMG_GBIME_SYSTEM;//加上图片ID基址;
			}
			
			iDisplayImageLeftX = winPos.x + pLayoutChgtLayer->plycBtData[i].btNormal.rcPos.left;
			iDisplayImageTopY = winPos.y + pLayoutChgtLayer->plycBtData[i].btNormal.rcPos.top;
			PT_DrawImage(iDisplayImageLeftX, iDisplayImageTopY, iCurrentButtonImageId);
		}
	}
#endif
}

#endif // 国笔虚拟键盘