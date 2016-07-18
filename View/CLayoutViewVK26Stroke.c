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
* Purpose      :  虚拟QWERTY笔画Layout视图类
* Version      :  V1.0
* Last Changed List
* Name         |   date   |  Discription
****************************************************************************/
#if defined(__GBIME_VK__) // 国笔虚拟键盘

#include "CLayoutViewVK26Stroke.h"
#include "CLayoutVK26Stroke.h"
#include "PlatformAdapter.h"
#include "GB_Global.h"
#include "GBIMEResDef.h"

/*!
 * \ Layout View 指针预定义
 */
#define CLASS_VIEW_THIS_POITER(CLASS_NAME, pltThis)   \
	CLASS_NAME * pltViewThis = (CLASS_NAME *)pltThis->m_ltParent.m_ltParent.m_hltView; \
	PCLayoutViewVK pltViewBase = &pltViewThis->m_ltParent; \
	PCLayoutViewAbstract pltViewRoot = &pltViewThis->m_ltParent.m_ltParent;

GBIMELOCAL GBIMEReturn CLayoutViewVK26Stroke__Paint(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo);

// 本地函数声明
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

	// 刷新所有
	if ((pltRoot->m_refreshFlag & GBIME_LAYOUT_REFRESH_ALL) == GBIME_LAYOUT_REFRESH_ALL)
	{
		pltViewBase->m_pfcDrawKeyboard(pltObj, pEngineOutputInfo);
		pltViewBase->m_pfcDrawComponents(pltObj, pEngineOutputInfo);
	} 
	// 刷新整个键盘
	else if ((pltRoot->m_refreshFlag & GBIME_LAYOUT_REFRESH_KEYBOARD) == GBIME_LAYOUT_REFRESH_KEYBOARD)
	{
		pltViewBase->m_pfcDrawKeyboard(pltObj, pEngineOutputInfo);
		pltViewBase->m_pfcDrawComponents(pltObj, pEngineOutputInfo);
	}
	// 刷新候选栏
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
 * \brief 画第二层候选区[不是联想状态]
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
	GBUINT16 iCandidateBoxHeight = 0;
	GBRECT screenRect,imageRect;
	GBRECT buttonRect;
	GBIMEPoint winPos;
	GBUINT16 iCurrentLayoutId;
	GBUINT16 invertKeyValue;
	GBBOOL bInvertFlag = GBFalse;

	// 得到指向候选框的Layout
	pLayerConstInfo = pltRoot->m_ltData.pltLayerData[SecondLayer].plycData;

	if (pLayerConstInfo == NULL)
	{
		return;
	}

	// 当前LayoutId
	iCurrentLayoutId = GET_LAYOUT_ID(pltRoot);
	// 获取键盘高度
	iKeyBoardHeight = pltRoot->m_pfcGetLayoutHeight(pltObj,iCurrentLayoutId);
	// 取得键盘显示的开始位置【左下角】，并调整为【左上角】
	Global_GetWinPos(&winPos);	
	winPos.y -=  iKeyBoardHeight;
	
	// 取候选框高度
	iCandidateBoxHeight = pltRoot->m_pfcGetCandidateBoxHeight(pltObj,iCurrentLayoutId,SecondLayer);
	// 相减得到候选框左上角Y坐标
	winPos.y -= iCandidateBoxHeight;
	
	iCandidateNomalImageId = pLayerConstInfo->lyImageID + IMG_GBIME_SYSTEM;//加上图片ID基址;
	iCandidateDownImageId = pLayerConstInfo->lyBKImageID + IMG_GBIME_SYSTEM;//加上图片ID基址;
	
	// 显示图片的左上角X坐标
	iDisplayImageLeftX = (GBUINT16)winPos.x;
	// 显示图片的左上角Y坐标
	iDisplayImageTopY = (GBUINT16)winPos.y;

	// 显示背景图片
	PT_DrawImage(iDisplayImageLeftX,iDisplayImageTopY,iCandidateNomalImageId);
	
	// 更新箭头按钮状态
	pltViewBase->m_pfcRedrawArrowButton(pltObj, iDisplayImageLeftX, iDisplayImageTopY, SecondLayer, pEngineOutputInfo);	
	
	// 判断是否点中翻页按钮
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
		
	// 反显翻页按钮
	if (bInvertFlag && pltRoot->buttonStatus == ButtonStatusDown)
	{

		// 获取按钮区域
		pltRoot->m_pfcGetBottonRectByKey(pltObj, pltRoot->m_ltData.pltcData->ltID, SecondLayer, invertKeyValue,	&buttonRect);
		memcpy(&imageRect, &buttonRect, sizeof(GBRECT));		
		// 图片在屏幕上显示的位置
		memcpy(&screenRect, &imageRect, sizeof(GBRECT));
		MOVE_RECT(&screenRect, winPos.x, iDisplayImageTopY);		
		// 反显翻页按钮
		PT_DrawImageWithClip(iCandidateDownImageId,&screenRect,&imageRect);
	}
	else
	{
		// 取翻页按钮的大小[画候选时用到]
		pltRoot->m_pfcGetBottonRectByKey(pltObj, 
										 pltRoot->m_ltData.pltcData->ltID, 
										 SecondLayer, 
										 GBKEY_PREV_PAGE_CANDIATE,	
										 &buttonRect);
	}
	// 打印Log信息
	PT_PrintLogInfo(GBLOG_LAYOUTVIEW, ("***********************ICLayoutViewVK26Stroke__DrawCandidateLine() Draw Candidate image\n"));
	PT_PrintLogInfo(GBLOG_LAYOUTVIEW, ("***********************Candidate Image ID = %d\n", iCandidateNomalImageId));

	// 绘制笔画
	//pltViewBase->m_pfcDrawStroke(pltObj, pEngineOutputInfo);
	pltViewBase->m_pfcDrawStrokeUpLine(pltObj, pEngineOutputInfo);// 处理GB2312偏旁“乛”“小”“竹”在代码区显示为“口”
	// 绘制候选字
	pltViewBase->m_pfcDrawCandidate(pltObj, pEngineOutputInfo);

	// 重绘第二、三层中逻辑上属于第一层的按钮（如拼音分隔符等...）
	pltViewBase->m_pfcRedrawKeyboardSpecialButton(pltObj);
}

/*!
 * \brief 画第三层候选区[是联想状态]
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
	//候选行数[目前暂一行候选]
	GBUINT16 iCandidateRowCount = 1;
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
	GBUINT16 iCandidateBoxHeight = 0;
	GBIMEPoint winPos;
	GBRECT screenRect,imageRect;
	GBRECT buttonRect;
	GBUINT16 invertKeyValue;
	GBBOOL bInvertFlag = GBFalse;

	// 得到指向候选框的Layout
	pLayerConstInfo = pltRoot->m_ltData.pltLayerData[ThirdLayer].plycData;

	if (pLayerConstInfo != NULL)
	{
		// 当前LayoutId
		iCurrentLayoutId = GET_LAYOUT_ID(pltRoot);
		// 获取键盘高度
		iKeyBoardHeight = pltRoot->m_pfcGetLayoutHeight(pltObj,iCurrentLayoutId);
		// 取得键盘显示的开始位置【左下角】，并调整为【左上角】
		Global_GetWinPos(&winPos);	
		winPos.y -=  iKeyBoardHeight;
		// 取候选框高度
		iCandidateBoxHeight = pltRoot->m_pfcGetCandidateBoxHeight(pltObj,iCurrentLayoutId,ThirdLayer);
		// 相减得到候选框左上角Y坐标
		winPos.y -= iCandidateBoxHeight;

		iCandidateNomalImageId = pLayerConstInfo->lyImageID + IMG_GBIME_SYSTEM;//加上图片ID基址;
		iCandidateDownImageId = pLayerConstInfo->lyBKImageID + IMG_GBIME_SYSTEM;//加上图片ID基址;
		// 显示[候选]图片的左上角X坐标
		iDisplayImageLeftX = (GBUINT16)winPos.x;
		// 显示[候选]图片的左上角Y坐标
		iDisplayImageTopY = (GBUINT16)winPos.y;

		// 显示背景图片
		PT_DrawImage(iDisplayImageLeftX, iDisplayImageTopY, iCandidateNomalImageId);
		
		// 更新箭头按钮状态
		pltViewBase->m_pfcRedrawArrowButton(pltObj, iDisplayImageLeftX, iDisplayImageTopY, ThirdLayer, pEngineOutputInfo);	

		// 判断是否点中翻页按钮
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

		// 反显翻页按钮
		if (bInvertFlag && pltRoot->buttonStatus == ButtonStatusDown)
		{
			// 获取按钮区域
			pltRoot->m_pfcGetBottonRectByKey(pltObj, pltRoot->m_ltData.pltcData->ltID, ThirdLayer, invertKeyValue, &buttonRect);		
			memcpy(&imageRect, &buttonRect, sizeof(GBRECT));		
			// 图片在屏幕上显示的位置
			memcpy(&screenRect, &imageRect, sizeof(GBRECT));
			MOVE_RECT(&screenRect, winPos.x, iDisplayImageTopY);					
			// 反显翻页按钮
			PT_DrawImageWithClip(iCandidateDownImageId,&screenRect,&imageRect);
		}
		else
		{
			// 画候选时用到
			pltRoot->m_pfcGetBottonRectByKey(pltObj,
											pltRoot->m_ltData.pltcData->ltID,
											ThirdLayer,
											GBKEY_PREV_PAGE_CANDIATE,
											&buttonRect);
		}

		// 打印Log信息
		PT_PrintLogInfo(GBLOG_LAYOUTVIEW, ("***********************ICLayoutViewVK26Stroke__DrawAssociateLine() Draw Candidate image\n"));
		PT_PrintLogInfo(GBLOG_LAYOUTVIEW, ("***********************Candidate Image ID = %d\n",iCandidateNomalImageId));

		// 绘制候选字
		pltViewBase->m_pfcDrawCandidate(pltObj, pEngineOutputInfo);

		// 重绘第二、三层中逻辑上属于第一层的按钮（如拼音分隔符等...）
		pltViewBase->m_pfcRedrawKeyboardSpecialButton(pltObj);
	}

}

#endif // 国笔虚拟键盘