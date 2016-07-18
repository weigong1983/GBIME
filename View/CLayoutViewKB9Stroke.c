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
* Purpose      :  物理九键笔画Layout视图类
* Version      :  V1.0
* Last Changed List
* Name         |   date   |  Discription
****************************************************************************/
#if defined(__GBIME_KB__) // 国笔物理键盘

#include "CLayoutViewKB9Stroke.h"
#include "CLayoutKB9Stroke.h"
#include "PlatformAdapter.h"
#include "GB_Global.h"
#include "GBIMEResDef.h"
/*!
 * \ Layout View 指针预定义
 */
#define CLASS_VIEW_THIS_POITER(CLASS_NAME, pltThis)   \
	CLASS_NAME * pltViewThis = (CLASS_NAME *)pltThis->m_ltParent.m_ltParent.m_hltView; \
	PCLayoutViewKB pltViewBase = &pltViewThis->m_ltParent; \
	PCLayoutViewAbstract pltViewRoot = &pltViewThis->m_ltParent.m_ltParent;

GBIMELOCAL GBIMEReturn CLayoutViewKB9Stroke__Paint(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo);

// 本地函数声明
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

	// 刷新所有
//	if ((pltRoot->m_refreshFlag & GBIME_LAYOUT_REFRESH_ALL) == GBIME_LAYOUT_REFRESH_ALL)
//	{
//		pltViewBase->m_pfcDrawKeyboard(pltObj, pEngineOutputInfo);
//	}
	// 刷新整个键盘
	if ((pltRoot->m_refreshFlag & GBIME_LAYOUT_REFRESH_KEYBOARD) == GBIME_LAYOUT_REFRESH_KEYBOARD)
	{
		pltViewBase->m_pfcDrawKeyboard(pltObj, pEngineOutputInfo);
	}
	// 刷新候选栏
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

	//候选行数[目前两行]
	GBUINT16 iCandidateRowCount = 2;
	//候选个数
	//指向层的开始位置
	GBLPCLAYOUTCONSTLAYER pLayerConstInfo;	 
	//候选框正常状态下的图片
	GBUINT16 iCandidateNomalImageId;
	//候选框按下状态下的图片
	GBUINT16 iCandidateDownImageId;
	//显示图片的左上角X坐标
	GBUINT16 iDisplayImageLeftX;
	//显示图片的左上角Y坐标
	GBUINT16 iDisplayImageTopY;
	GBUINT16 iKeyBoardHeight = 0;
	GBUINT16 iKeyBoardWidth = 0;
	GBUINT16 iCandidateBoxHeight = 0;
	GBIMEPoint winPos;
	GBUINT16 iCurrentLayoutId;
	GBBOOL bInvertFlag = GBFalse;
	GBINT16 iScreenWidth = 0;

	// 得到指向候选框的Layout
	pLayerConstInfo = pltRoot->m_ltData.pltLayerData[FirstLayer].plycData;

	if (pLayerConstInfo == NULL)
	{
		return;
	}

	// 当前LayoutId
	iCurrentLayoutId = GET_LAYOUT_ID(pltRoot);
	// 获取键盘高度
	iKeyBoardHeight = pltRoot->m_pfcGetLayoutHeight(pltObj,iCurrentLayoutId);
	iKeyBoardWidth = pltRoot->m_pfcGetLayoutWidth(pltObj, iCurrentLayoutId);
	// 取得键盘显示的开始位置【左下角】，并调整为【左上角】
	Global_GetWinPos(&winPos);	
	winPos.y -=  iKeyBoardHeight;
	
	iScreenWidth = PT_GetLcdWidth();

// 	// 设置拼笔划显示的开始位置
// 	pltRoot->syllableDisplayStartPos.x = GBIME_SYLLABLE_DISPLAY_START_X;
// 	pltRoot->syllableDisplayStartPos.y = winPos.y - iCandidateBoxHeight;
// 
// 	// 设置候选显示的开始位置
// 	pltRoot->candidateDisplayStartPos.x = GBIME_CANDIDATE_DISPLAY_START_X;
// 	pltRoot->candidateDisplayStartPos.y = winPos.y + iKeyBoardHeight / 2;// GBIME_CANDIDATE_LINE_HEIGHT - 2;


	// 取候选框高度
	iCandidateBoxHeight = pltRoot->m_pfcGetCandidateBoxHeight(pltObj,iCurrentLayoutId,FirstLayer);
	// 相减得到候选框左上角Y坐标
	winPos.y -= iCandidateBoxHeight;
	
	iCandidateNomalImageId = pLayerConstInfo->lyImageID + IMG_GBIME_SYSTEM;//加上图片ID基址;
	iCandidateDownImageId = pLayerConstInfo->lyBKImageID + IMG_GBIME_SYSTEM;//加上图片ID基址;
	
	// 显示图片的左上角X坐标
	iDisplayImageLeftX = (GBUINT16)winPos.x + (iScreenWidth - iKeyBoardWidth) / 2;
	// 显示图片的左上角Y坐标
	iDisplayImageTopY = (GBUINT16)winPos.y;

// 	// 显示背景图片
// 	PT_DrawImage(iDisplayImageLeftX ,iDisplayImageTopY,iCandidateNomalImageId);

	// 更新箭头按钮状态
	pltViewBase->m_pfcRedrawArrowButton(pltObj, iDisplayImageLeftX,	iDisplayImageTopY, FourthLayer, pEngineOutputInfo);	

	// 打印Log信息
	PT_PrintLogInfo(GBLOG_LAYOUTVIEW, ("***********************ICLayoutViewKB9Stroke__DrawCandidateLine() Draw Candidate image\n"));
	PT_PrintLogInfo(GBLOG_LAYOUTVIEW, ("***********************Candidate Image ID = %d\n", iCandidateNomalImageId));
	
	// 绘制输入串和笔画
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
	PT_PrintLogInfo(GBLOG_LAYOUTVIEW, ("候选："));
	for (i = 0; i < pEngineOutputInfo->nCandNum; i++)
	{
		PT_wcstombs(candBuf, pEngineOutputInfo->ppCandidates[i], sizeof(candBuf));
		PT_PrintLogInfo(GBLOG_LAYOUTVIEW, ("%s,", candBuf));
	}
	PT_PrintLogInfo(GBLOG_LAYOUTVIEW, ("\r\n"));
}
#endif
}

#endif // 国笔物理键盘