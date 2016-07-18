/***************************************************************************
* File Name    :  CLayoutViewHandwrite.C
* Corporation  :  Guobi Technology Ltd. GuangZhou China
* Copyright    :  Copyright Guobi Technology Ltd. And Subject to Confidentiality 
*                 Restrictions. This document is the confidential and proprietary 
*                 property of Guobi Technology Ltd.It is neither the document                  
*                 itself nor any part of it can be shared with any party  without 
*                 the written  permission. 
* Author       :  weizhiping
* Date         :  2010-08-07
* Purpose      :  手写键盘视图类
* Version      :  V1.0
* Last Changed List
* Name         |   date   |  Discription
****************************************************************************/
#include "CLayoutViewHandwriteFullScreen.h"
#include "CLayoutHandwriteFullScreen.h"
#include "PlatformAdapter.h"
#include "PlatformTools.h"
#include "GB_Global.h"
#include "CHWEngine.h"
#include "GBIMEResDef.h"

#if defined(__GBIME_HANDWRITE__)

/*!
 * \ Layout View 指针预定义
 */
#define CLASS_VIEW_THIS_POITER(CLASS_NAME, pltThis)   \
	CLASS_NAME * pltViewThis = (CLASS_NAME *)pltThis->m_ltParent.m_ltParent.m_hltView; \
	PCLayoutViewHandwrite pltViewBase = &pltViewThis->m_ltParent; \
	PCLayoutViewAbstract pltViewRoot = &pltViewThis->m_ltParent.m_ltParent;

GBIMELOCAL GBIMEReturn CLayoutViewHandwriteFullScreen__Paint(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo);

GBIMELOCAL void ICLayoutViewHandwriteFullScreen_DrawKeyboard(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo);
GBIMELOCAL void ICLayoutViewHandwriteFullScreen_DrawCandidate(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo);
GBIMELOCAL void ICLayoutViewHandwriteFullScreen_DrawAssocWord(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo);
															  
CLayoutViewAbstract * CLayoutViewHandwriteFullScreen__Construct(PCLayoutViewHandwriteFullScreen pltViewHandwrite)
{
	CLayoutViewHandwrite__Construct(&pltViewHandwrite->m_ltParent);
	CLayoutViewHandwriteFullScreen__Init(pltViewHandwrite);
	return (CLayoutViewAbstract *)pltViewHandwrite;
}

GBIMEReturn CLayoutViewHandwriteFullScreen__Init(PCLayoutViewHandwriteFullScreen pltViewHandwrite)
{
	pltViewHandwrite->m_ltParent.m_ltParent.m_pfcPaint = CLayoutViewHandwriteFullScreen__Paint;
	return GBIME_OK;
}

GBIMELOCAL GBIMEReturn CLayoutViewHandwriteFullScreen__Paint(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo)
{
	CLASE_THIS_POITER(CLayoutHandwriteFullScreen, pltObj)

	PLayoutInfo pLayoutInfo = (PLayoutInfo)(&pltRoot->m_ltData); // Layout UI data point

	if (LayoutHandwriteFullScreen != pltRoot->m_ltType)
	{
		//return GBIME_LAYOUT_NOTMATCH;
	}

	if (pltRoot->m_refreshFlag == GBIME_LAYOUT_REFRESH_NONE)
	{
		return GBIME_OK;
	}

	PT_LayerLock();

	// 刷新所有
	if ((pltRoot->m_refreshFlag & GBIME_LAYOUT_REFRESH_ALL) == GBIME_LAYOUT_REFRESH_ALL)
	{
		ICLayoutViewHandwriteFullScreen_DrawKeyboard(pltObj, pEngineOutputInfo);
	} 
	// 刷新整个键盘
	else if ((pltRoot->m_refreshFlag & GBIME_LAYOUT_REFRESH_KEYBOARD) == GBIME_LAYOUT_REFRESH_KEYBOARD)
	{
		ICLayoutViewHandwriteFullScreen_DrawKeyboard(pltObj, pEngineOutputInfo);
	}
	// 刷新候选栏
	if ((pltRoot->m_refreshFlag & GBIME_LAYOUT_REFRESH_CANDIDATE_LINE) == GBIME_LAYOUT_REFRESH_CANDIDATE_LINE)
	{
		// 绘制候选代码段
		//if (pltThis->bUpdateCandidate)
		{
			ICLayoutViewHandwriteFullScreen_DrawCandidate(pltObj, pEngineOutputInfo);			
		}
		
		//if (pltThis->bUpdateAssocWord) // 绘制联想词代码段
		{
			ICLayoutViewHandwriteFullScreen_DrawAssocWord(pltObj, pEngineOutputInfo);
		}
	}
	
	// 解锁、刷屏
	PT_LayerUnLock();
	PT_Invalidate(0, 0, PT_GetLcdWidth(), PT_GetLcdHeight());
	//pltRoot->m_pfcRedraw(pltObj);
	return GBIME_OK;
}

/*!
 * \brief 绘制手写键盘
 * \param pltObj 
 * \param pEngineOutputInfo 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-8-12 11:47:41
*/
static void ICLayoutViewHandwriteFullScreen_DrawKeyboard(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo)
{
	CLASE_THIS_POITER(CLayoutHandwriteFullScreen, pltObj)
	CLASS_VIEW_THIS_POITER(CLayoutViewHandwriteFullScreen, pltThis)

	//当前要显示的层
	GBUINT16 iCurrentLayerNumber;
	GBUINT16 iCurrentLayoutID;
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
	//按钮总个数
	GBUINT16 iButtonTotalCount;
	GBUINT16 iCount;
	GBUINT16 currentButtonKey;
	GBUINT16 currentButtonStatus = 0;
	GBUINT16 drawFuncButtonStatus = 0;
	GBIMEPoint winPos;
	//GBRECT screenRect,imageRect;
	GBUINT16 iCandidateBoxHeight = 0;
	GBUINT16 iKeyBoardHeight = 0;
	PLayoutInfo pLayoutInfo = (PLayoutInfo)(&pltRoot->m_ltData); // Layout UI data point

	// 取得键盘显示的开始位置[左下角坐标]
	Global_GetWinPos(&winPos);
	iCurrentLayoutID = GET_LAYOUT_ID(pltRoot);
	// 键盘高度
	iKeyBoardHeight = pltRoot->m_pfcGetLayoutHeight(pltObj, GET_LAYOUT_ID(pltRoot));
	// 相减得到键盘左上角Y坐标
	winPos.y -= iKeyBoardHeight;
	
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
	PT_DrawImage(iDisplayImageLeftX,iDisplayImageTopY, iCurrentLayerNomalImageId);

	// 打印Log信息
	PT_PrintLogInfo(GBLOG_HANDWRITE, ("***********************ICLayoutViewHandwriteFullScreen_DrawKeyboard() Draw Keyboard\n"));
	PT_PrintLogInfo(GBLOG_HANDWRITE, ("***********************Image ID = %d\n",iCurrentLayerNomalImageId));
	
	// 得到按钮总数
	iButtonTotalCount = pLayoutInfo->pltLayerData[FirstLayer].lyBtCount;
	// 查找是否有变化的按钮(不是正常状态)
	for (iCount = 0; iCount < iButtonTotalCount; iCount++)
	{
		currentButtonKey = pLayoutInfo->pltLayerData[FirstLayer].plycBtData[iCount].btKey;
		currentButtonStatus = pLayoutInfo->pltLayerData[FirstLayer].plyBtChgData[iCount].btStatus;

		switch (currentButtonKey)
		{
		case GBKEY_VK_V9_NUM: ///<竖屏9键数字
			if ((pltThis->hwRecognizeRangeFlag & GBIME_HW_RECOGNIZE_RANGE_NUMBER) 
				== GBIME_HW_RECOGNIZE_RANGE_NUMBER)
			{
				drawFuncButtonStatus = ButtonStatusDown;			
			}
			else
			{
				drawFuncButtonStatus = ButtonStatusNomal;
			}
			pLayoutInfo->pltLayerData[FirstLayer].plyBtChgData[iCount].btStatus = drawFuncButtonStatus;
			pltViewRoot->m_pfcDrawButtonByStatus(pltObj,
												 iDisplayImageLeftX,
												 iDisplayImageTopY,
												 NULL,
												 iCurrentLayoutID,
												 FirstLayer,
												 currentButtonKey,
												 drawFuncButtonStatus);
			break;
		case GBKEY_VK_V_HANDWRITE_ALL: ///<竖屏手写全部
			if ((pltThis->hwRecognizeRangeFlag & GBIME_HW_RECOGNIZE_RANGE_ALL) 
				== GBIME_HW_RECOGNIZE_RANGE_ALL)
			{
				drawFuncButtonStatus = ButtonStatusDown;			
			}
			else
			{
				drawFuncButtonStatus = ButtonStatusNomal;
			}
			pLayoutInfo->pltLayerData[FirstLayer].plyBtChgData[iCount].btStatus = drawFuncButtonStatus;
			pltViewRoot->m_pfcDrawButtonByStatus(pltObj,
												 iDisplayImageLeftX,
												 iDisplayImageTopY,
												 NULL,
												 iCurrentLayoutID,
												 FirstLayer,
												 currentButtonKey,
												 drawFuncButtonStatus);
			break;
		case GBKEY_VK_v_HANDWRITE_ENG: ///<竖屏手写英文
			if ((pltThis->hwRecognizeRangeFlag & GBIME_HW_RECOGNIZE_RANGE_ENGLISH) 
				== GBIME_HW_RECOGNIZE_RANGE_ENGLISH)
			{
				drawFuncButtonStatus = ButtonStatusDown;			
			}
			else
			{
				drawFuncButtonStatus = ButtonStatusNomal;
			}
			pLayoutInfo->pltLayerData[FirstLayer].plyBtChgData[iCount].btStatus = drawFuncButtonStatus;
			pltViewRoot->m_pfcDrawButtonByStatus(pltObj,
												 iDisplayImageLeftX,
												 iDisplayImageTopY,
												 NULL,
												 iCurrentLayoutID,
												 FirstLayer,
												 currentButtonKey,
												 drawFuncButtonStatus);
			break;
		case GBKEY_VK_v_HANDWRITE_TONGYIN: ///<竖屏手写同音
			if ((pltThis->hwRecognizeRangeFlag & GBIME_HW_RECOGNIZE_RANGE_HOMONYM) 
				== GBIME_HW_RECOGNIZE_RANGE_HOMONYM)
			{
				drawFuncButtonStatus = ButtonStatusDown;			
			}
			else
			{
				drawFuncButtonStatus = ButtonStatusNomal;
			}
			pLayoutInfo->pltLayerData[FirstLayer].plyBtChgData[iCount].btStatus = drawFuncButtonStatus;
			pltViewRoot->m_pfcDrawButtonByStatus(pltObj,
												 iDisplayImageLeftX,
												 iDisplayImageTopY,
												 NULL,
												 iCurrentLayoutID,
												 FirstLayer,
												 currentButtonKey,
												 drawFuncButtonStatus);
			break;
		default:
			// btStatus : 按键状态，0为正常态，1为按下，2为灰色，3隐藏当前按键
			// 发现状态不是0时，就表示有变化的按钮
			if (pLayoutInfo->pltLayerData[FirstLayer].plyBtChgData[iCount].btStatus != ButtonStatusNomal)
			{
				pLayoutInfo->pltLayerData[FirstLayer].plyBtChgData[iCount].btStatus = ButtonStatusNomal;				
				
				// 如果有变化的按钮[候选按钮、联想按钮独自处理反显]
				if (GBKEY_VK_V_HANDWRITE_CANDIDATE_AREA != currentButtonKey
					&& GBKEY_VK_V_HANDWRITE_ASSOCIATE_AREA != currentButtonKey)
				{
					pltViewRoot->m_pfcDrawButtonByStatus(pltObj,
														 iDisplayImageLeftX,
														 iDisplayImageTopY,
														 NULL,
														 iCurrentLayoutID,
														 FirstLayer,
														 currentButtonKey,
														 currentButtonStatus);
				}
			}
			break;
		}		
	}	
}


/*!
 * \brief 绘制手写候选
 * \param pltObj 
 * \param pEngineOutputInfo 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-8-12 11:49:48
*/
static void ICLayoutViewHandwriteFullScreen_DrawCandidate(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo)
{
	CLASE_THIS_POITER(CLayoutHandwriteFullScreen, pltObj)
	CLASS_VIEW_THIS_POITER(CLayoutViewHandwriteFullScreen, pltThis)
	GBUINT16 i;
	GBPCWCHAR pCand = NULL;

	if (pltThis->candidateTouchRegion.candidateNum == 0)
	{
		return ;
	}

	// 画候选
	for (i = 0; i < pltThis->candidateTouchRegion.candidateNum; i++)
	{		
		// 是否数字候选串
		if (GBHW_GetNumberStatus(Global_GetHWEnineInstance()))
		{
			pCand = pltThis->pNumberCandidates[i];
		}
		else
		{
			pCand = GBHW_GetCandByIndex(Global_GetHWEnineInstance(), i);
		}

		// 绘制候选
		if (pltThis->nSelectedCandidateIndex == i)
		{		
			// 选择候选按下反显
			if (pltRoot->buttonStatus == ButtonStatusDown
				&& pltThis->bUpdateCandidate)
			{
				GBIMEPoint layoutWinStartPos;
				
				pltRoot->m_pfcGetWinPos(pltObj, &layoutWinStartPos);
				pltViewRoot->m_pfcDrawButtonByStatus(pltObj, 
					layoutWinStartPos.x,
					layoutWinStartPos.y,
					&pltThis->candidateTouchRegion.candidateRectArray[i],
					GET_LAYOUT_ID(pltRoot), 
					FirstLayer, 
					GBKEY_VK_V_HANDWRITE_CANDIDATE_AREA, 
					ButtonStatusDown);
			}

			// 绘制焦点候选
			PT_DrawString(pCand, GBIME_CANDIDATE_FONT_SIZE, GBIME_CANDIDATE_SELECTED_FONT_COLOR, 
					      (GBINT)pltThis->candidateTouchRegion.candidatePosArray[i].x, 
						  (GBINT)pltThis->candidateTouchRegion.candidatePosArray[i].y);
		}
		else
		{
			// 绘制候选
			PT_DrawString(pCand, GBIME_CANDIDATE_FONT_SIZE, GBIME_CANDIDATE_NORMAL_FONT_COLOR, 
						  (GBINT)pltThis->candidateTouchRegion.candidatePosArray[i].x, 
						  (GBINT)pltThis->candidateTouchRegion.candidatePosArray[i].y);
		}
	}
}

/*!
 * \brief 绘制手写联想词
 * \param pltObj 
 * \param pEngineOutputInfo 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-8-12 11:50:02
*/
static void ICLayoutViewHandwriteFullScreen_DrawAssocWord(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo)
{
	CLASE_THIS_POITER(CLayoutHandwriteFullScreen, pltObj)
	CLASS_VIEW_THIS_POITER(CLayoutViewHandwriteFullScreen, pltThis)
	GBUINT16 i;
	GBPCWCHAR pCand = NULL;
	
	if (pltThis->associateTouchRegion.candidateNum == 0)
	{
		return ;
	}
	
	// 画候选
	for (i = 0; i < pltThis->associateTouchRegion.candidateNum; i++)
	{
		// 是否数字候选串
		if (GBHW_GetNumberStatus(Global_GetHWEnineInstance()))
		{
			pCand = pltThis->pNumberCandidates[i + 5];
		}
		else
		{
			pCand = pEngineOutputInfo->ppCandidates[i];
		}

		// 绘制候选
		if (pltThis->nSelectedAssociateWordIndex == i)
		{	
			// 选择候选按下反显
			if (pltRoot->buttonStatus == ButtonStatusDown
				&& pltThis->bUpdateAssocWord)
			{
				GBIMEPoint layoutWinStartPos;
				
				pltRoot->m_pfcGetWinPos(pltObj, &layoutWinStartPos);
				pltViewRoot->m_pfcDrawButtonByStatus(pltObj, 
					layoutWinStartPos.x,
					layoutWinStartPos.y,
					&pltThis->associateTouchRegion.candidateRectArray[i],
					GET_LAYOUT_ID(pltRoot), 
					FirstLayer, 
					GBKEY_VK_V_HANDWRITE_ASSOCIATE_AREA, 
					ButtonStatusDown);
			}

			// 绘制焦点候选
			PT_DrawString(pCand, GBIME_CANDIDATE_FONT_SIZE, GBIME_CANDIDATE_SELECTED_FONT_COLOR, 
				(GBINT)pltThis->associateTouchRegion.candidatePosArray[i].x, 
				(GBINT)pltThis->associateTouchRegion.candidatePosArray[i].y);
		}
		else
		{
			// 绘制候选
			PT_DrawString(pCand, GBIME_CANDIDATE_FONT_SIZE, GBIME_CANDIDATE_NORMAL_FONT_COLOR, 
				(GBINT)pltThis->associateTouchRegion.candidatePosArray[i].x, 
				(GBINT)pltThis->associateTouchRegion.candidatePosArray[i].y);
		}
	}
}
#endif // defined(__GBIME_HANDWRITE__)

