/***************************************************************************
* File Name    :  CLayoutViewVKSymbol.c
* Corporation  :  Guobi Technology Ltd. GuangZhou China
* Copyright    :  Copyright Guobi Technology Ltd. And Subject to Confidentiality 
*                 Restrictions. This document is the confidential and proprietary 
*                 property of Guobi Technology Ltd.It is neither the document                  
*                 itself nor any part of it can be shared with any party  without 
*                 the written  permission. 
* Author       :  haj
* Date         :  2010-06-08
* Purpose      :  虚拟符号Layout视图类
* Version      :  V1.0
* Last Changed List
* Name         |   date   |  Discription
****************************************************************************/
#if defined(__GBIME_VK__) // 国笔虚拟键盘

#include "CLayoutViewVKSymbol.h"
#include "CLayoutVKSymbol.h"
#include "PlatformAdapter.h"
#include "GBIMEResDef.h"
#include "GB_Global.h"
#include "CLayoutVKSymbol.h"

/*!
 * \ Layout View 指针预定义
 */
#define CLASS_VIEW_THIS_POITER(CLASS_NAME, pltThis)   \
	CLASS_NAME * pltViewThis = (CLASS_NAME *)pltThis->m_ltParent.m_ltParent.m_hltView; \
	PCLayoutViewVK pltViewBase = &pltViewThis->m_ltParent; \
	PCLayoutViewAbstract pltViewRoot = &pltViewThis->m_ltParent.m_ltParent;

#define SIGN_COLOR		PT_COLOR_BLACK

GBIMELOCAL GBIMEReturn CLayoutViewVKSymbol__Paint(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo);

// 本地函数声明
GBIMELOCAL void ICLayoutViewVKSymbol_DrawKeyboard(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo);
GBIMELOCAL void ICLayoutViewVKSymbol_DrawCurrentPageSymbol(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo);

CLayoutViewAbstract * CLayoutViewVKSymbol__Construct(PCLayoutViewVKSymbol pltViewVKSymbol)
{
	CLayoutViewVK__Construct(&pltViewVKSymbol->m_ltParent);

	CLayoutViewVKSymbol__Init(pltViewVKSymbol);
	return (CLayoutViewAbstract *)pltViewVKSymbol;
}

GBIMEReturn CLayoutViewVKSymbol__Init(PCLayoutViewVKSymbol pltViewVKSymbol)
{
	pltViewVKSymbol->m_ltParent.m_ltParent.m_pfcPaint = CLayoutViewVKSymbol__Paint;
	pltViewVKSymbol->m_ltParent.m_pfcDrawKeyboard = ICLayoutViewVKSymbol_DrawKeyboard;
	return GBIME_OK;
}

GBIMELOCAL GBIMEReturn CLayoutViewVKSymbol__Paint(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo)
{
	CLASE_THIS_POITER(CLayoutVKSymbol, pltObj)
	CLASS_VIEW_THIS_POITER(CLayoutViewVKSymbol, pltThis)

	PLayoutInfo pLayoutInfo = NULL;
	GBRECT layoutWinRect = {0};

	if (LayoutVKSymbol != pltRoot->m_ltType)
	{
		return GBIME_LAYOUT_NOTMATCH;
	}

	pLayoutInfo = (PLayoutInfo)(&pltRoot->m_ltData); // Layout UI data point
	pltViewBase->m_pfcDrawKeyboard(pltObj, pEngineOutputInfo);
	ICLayoutViewVKSymbol_DrawCurrentPageSymbol(pltObj, pEngineOutputInfo);
	pltRoot->m_pfcGetLayoutWinRect(pltObj, &layoutWinRect);
	PT_Invalidate(layoutWinRect.left, layoutWinRect.top, layoutWinRect.right, layoutWinRect.bottom);
	return GBIME_OK;
}

static void ICLayoutViewVKSymbol_DrawKeyboard(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo)
{
	CLASE_THIS_POITER(CLayoutVKSymbol, pltObj)
	CLASS_VIEW_THIS_POITER(CLayoutViewVKSymbol, pltThis)

	//候选个数
//	GBUINT16 iCandidateCount;
	//指向层的开始位置
	PLAYOUTCONSTLAYER pLayerConstInfo;
	//候选框正常状态下的图片
	GBUINT16 iCandidateNomalImageId;
	//候选框按下状态下的图片
	GBUINT16 iCandidateDownImageId;
	//显示图片的左上角X坐标
	GBUINT16 iDisplayImageLeftX;
	//显示图片的左上角Y坐标
	GBUINT16 iDisplayImageTopY;
	//一个候选占的象素宽度
//	GBUINT16 iCandidateWidth;
//	GBUINT16 iCurrentCandidateWidth;
	GBUINT16 iKeyBoardHeight = 0;
	GBUINT16 iCandidateBoxHeight = 0;
	GBIMEPoint winPos;
//	GBPCWCHAR pCand;
	GBRECT screenRect,imageRect;
//	GBRECT buttonRect;
	GBUINT16 iButtonTotalCount;
	GBBOOL bHasButtonChange = 0;
	GBUINT16 currentButtonStatus = 0;
	GBUINT16 iChangeButtonIndex = 0;
	GBUINT16 iCurrentButtonImageId = 0;
	GBUINT16 iCount;
	GBUINT16 iCurrentLayoutId;
	GBUINT16 keyValue;
	
	PLayoutInfo pLayoutInfo = (PLayoutInfo)(&pltRoot->m_ltData); // Layout UI data point
		
	pLayerConstInfo = pltRoot->m_pfcGetLayerConstInfo(pltObj, LayoutIdSP9JSymbol,FirstLayer);
	
	if (pLayerConstInfo != NULL)
	{
		iCurrentLayoutId = GET_LAYOUT_ID(pltRoot);
		// 获取键盘高度
		iKeyBoardHeight = pltRoot->m_pfcGetLayoutHeight(pltObj, iCurrentLayoutId);
		// 取得键盘显示的开始位置【左下角】，并调整为【左上角】
		Global_GetWinPos(&winPos);	
		winPos.y -=  iKeyBoardHeight;

		iCandidateNomalImageId = pLayerConstInfo->lyImageID + IMG_GBIME_SYSTEM; // 加上图片ID基址;
		iCandidateDownImageId = pLayerConstInfo->lyBKImageID + IMG_GBIME_SYSTEM; // 加上图片ID基址;
		// 显示[候选]图片的左上角X坐标
		iDisplayImageLeftX = (GBUINT16)winPos.x;
		// 显示[候选]图片的左上角Y坐标
		iDisplayImageTopY = (GBUINT16)winPos.y;
		// 显示背景图片
		PT_DrawImage(iDisplayImageLeftX, iDisplayImageTopY, iCandidateNomalImageId);

		// 得到按钮总数
		iButtonTotalCount = pLayoutInfo->pltLayerData[FirstLayer].lyBtCount;
		// 查找是否有变化的按钮(不是正常状态)
		for (iCount = 0;iCount < iButtonTotalCount; iCount++)
		{
			keyValue = GET_BUTTON_KEY_VALUE(pltRoot, FirstLayer, iCount);
			if (GBKEY_VK_SYM_comment == keyValue || GBKEY_VK_SYM_EMOTION == keyValue
				|| GBKEY_VK_SYM_SPECIAL == keyValue || GBKEY_VK_SYM_MATH == keyValue)
			{
				continue; // 符号类型按键不做反显
			}

			// btStatus : 按键状态，0为正常态，1为按下，2为灰色，3隐藏当前按键
			// 发现状态不是0时，就表示有变化的按钮
			if (pLayoutInfo->pltLayerData[FirstLayer].plyBtChgData[iCount].btStatus != ButtonStatusNomal)
			{
				bHasButtonChange = 1;
				currentButtonStatus = pLayoutInfo->pltLayerData[FirstLayer].plyBtChgData[iCount].btStatus;
				iChangeButtonIndex = iCount;
				pLayoutInfo->pltLayerData[FirstLayer].plyBtChgData[iCount].btStatus = ButtonStatusNomal;
				break;
			}
		}

		// 如果有变化的按钮
		if (bHasButtonChange)
		{
			// 取按钮在图片中的位置
			memcpy(&imageRect, &pLayoutInfo->pltLayerData[FirstLayer].plycBtData[iChangeButtonIndex].btPos, sizeof(GBRECT));
			// 图片在屏幕上显示的位置
			memcpy(&screenRect, &imageRect, sizeof(GBRECT));
			MOVE_RECT(&screenRect, winPos.x, iDisplayImageTopY);
			
			// 根据按键的不同状态来取其相应的图片ID
			if ((currentButtonStatus) == ButtonStatusNomal || (currentButtonStatus == ButtonStatusUp))
			{
				iCurrentButtonImageId = pLayoutInfo->pltLayerData[FirstLayer].plycBtData[iChangeButtonIndex].btNormal.imageID + IMG_GBIME_SYSTEM;//加上图片ID基址
			}
			else if (currentButtonStatus == ButtonStatusDown || currentButtonStatus == ButtonStatusMove)
			{
				iCurrentButtonImageId = pLayoutInfo->pltLayerData[FirstLayer].plycBtData[iChangeButtonIndex].btDown.imageID + IMG_GBIME_SYSTEM;//加上图片ID基址
			}
			else if(currentButtonStatus == ButtonStatusGray)
			{
				iCurrentButtonImageId = pLayoutInfo->pltLayerData[FirstLayer].plycBtData[iChangeButtonIndex].btGrey.imageID + IMG_GBIME_SYSTEM;//加上图片ID基址	
			}
			else
			{
				// 默认
				iCurrentButtonImageId = pLayoutInfo->pltLayerData[FirstLayer].plycBtData[iChangeButtonIndex].btNormal.imageID + IMG_GBIME_SYSTEM;//加上图片ID基址
			}
			PT_DrawImageWithClip(iCurrentButtonImageId,&screenRect,&imageRect);
		}
		if (!pltThis->m_bHasPrvePage)//没有上一页时
		{
			pltViewRoot->m_pfcDrawButtonByStatus(pltObj, 
												iDisplayImageLeftX, iDisplayImageTopY, NULL,
												iCurrentLayoutId, FirstLayer, 
												GBKEY_VK_SYM_PAGEUP, ButtonStatusGray);
			
		}
		if (!pltThis->m_bHasNextPage)//没有下一页时
		{
			pltViewRoot->m_pfcDrawButtonByStatus(pltObj, 
												iDisplayImageLeftX, iDisplayImageTopY, NULL,
												iCurrentLayoutId, FirstLayer, 
												GBKEY_VK_SYM_PAGEDOWN, ButtonStatusGray);
		}
		if (!pltThis->m_hasFullOrHalfSymbol)//没有半角或全角时
		{
			pltViewRoot->m_pfcDrawButtonByStatus(pltObj, 
												iDisplayImageLeftX, iDisplayImageTopY, NULL,
												iCurrentLayoutId, FirstLayer, 
												GBKEY_VK_SBC_DBC_SWICH, ButtonStatusGray);
		}
		
		// 更新
		if (pltThis->m_bSymbolLock)
		{
			pltViewRoot->m_pfcDrawButtonByStatus(pltObj, 
											iDisplayImageLeftX, iDisplayImageTopY, NULL,
											iCurrentLayoutId, FirstLayer, 
											GBKEY_VK_SYM_LOCK, ButtonStatusDown);
		}

		if ((pltThis->m_hasFullOrHalfSymbol)&&(pltThis->m_commonSymbolType == HalfShaped)) // 按钮按下是半角符号
		{
			pltViewRoot->m_pfcDrawButtonByStatus(pltObj, 
				iDisplayImageLeftX, iDisplayImageTopY, NULL,
				iCurrentLayoutId, FirstLayer, 
				GBKEY_VK_SBC_DBC_SWICH, ButtonStatusDown);
		}

		// 反显当前符号类型按键以作提示
		switch (pltThis->m_symbolType)
		{
		case SymbolTypeCommon: // 普通符号
			keyValue = GBKEY_VK_SYM_comment;
			break;
		case SymbolTypeFace: // 表情符号
			keyValue = GBKEY_VK_SYM_EMOTION;
			break;
		case SymbolTypeEspecial: // 特殊符号
			keyValue = GBKEY_VK_SYM_SPECIAL;
			break;
		case SymbolTypeNumber: // 数学符号
			keyValue = GBKEY_VK_SYM_MATH;
			break;
		default:
			PT_Assert(0); // 非法类型
			break;
		}
		pltViewRoot->m_pfcDrawButtonByStatus(pltObj, iDisplayImageLeftX, iDisplayImageTopY, NULL,
											 iCurrentLayoutId, FirstLayer, keyValue, 
											 ButtonStatusDown);

		//打印Log信息
		PT_PrintLogInfo(GBLOG_LAYOUTVIEW, ("***********************ICLayoutViewVKSymbol_DrawLayerOne() Draw Candidate image\n"));
		PT_PrintLogInfo(GBLOG_LAYOUTVIEW, ("***********************Candidate Image ID = %d\n", iCandidateNomalImageId));
	}
}

/*!
 * \brief 显示当前页面的符号
 * \param pltObj 
 * \param pEngineOutputInfo 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-8-19 9:45:48
*/
static void ICLayoutViewVKSymbol_DrawCurrentPageSymbol(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo)
{
#if __MTK__ > 0
	CLASE_THIS_POITER(CLayoutVKSymbol, pltObj)

	PLAYOUTCONSTLAYER pLayerConstInfo;
	PBUTTONCONSTDATA pButtonConstData;
	GBUINT16 iCount;
//	GBUINT16 iKeyValue;
	GBIMEPoint winPos;
	GBUINT16 iKeyBoardHeight = 0;
	GBUINT16 iSymbolOffset;
	GBUINT16 iSymbolIndex;
	GBPCWCHAR pSymbolData;
	GBUINT16 iSymbolWidth;
	GBUINT16 iSymbolHeight;
	PT_FontSize font = PT_FONT_SIZE_DEFAULT;

	pLayerConstInfo = pltRoot->m_pfcGetLayerConstInfo(pltObj, LayoutIdSP9JSymbol,FirstLayer);
	pButtonConstData = pltRoot->m_pfcGetLayerConstButtons(pltObj, LayoutIdSP9JSymbol,FirstLayer);

	// 获取键盘高度
	iKeyBoardHeight = pltRoot->m_ltData.pltLayerData[FirstLayer].plycData->lyPos.bottom;
	// 取得键盘显示的开始位置【左下角】，并调整为【左上角】
	Global_GetWinPos(&winPos);	
	winPos.y -=  iKeyBoardHeight;

	iSymbolIndex = pltThis->m_iCurrentPageStartIndex;
	pSymbolData = (GBPCWCHAR)pltThis->m_pltSymbolData;

	//查看是否需要改变符号的字体	
	{
		GBUINT16 iTempSymbolIdex = iSymbolIndex;
		for (;iTempSymbolIdex <= pltThis->m_iCurrentPageEndIndex;iTempSymbolIdex++)
		{
			iSymbolOffset = CLayoutVKSymbol__GetSymbolOffsetByIndex(pltObj, iTempSymbolIdex);
			//只要有一个符号长度达到3个字符，就将字体设置小号(否则按钮无法显示符号)
			if (PT_GetStrLen((GBWCHAR *)(pSymbolData + iSymbolOffset)) >= 3)
			{
				font = PT_FONT_SIZE_SMALL;
				break;
			}
		}
	}

	// 目前10个符号每页,显示到1，2，3.....9这9个数字键上
	for (iCount = 0;iCount < pLayerConstInfo->lyBtCount; iCount++)
	{
		// 如果已经到最后一个符号
		if (iSymbolIndex > pltThis->m_iCurrentPageEndIndex)
		{
			break;
		}

		if (pButtonConstData[iCount].btKey >= GBKEY_VK_1_NUMBERKEY 
			&&  pButtonConstData[iCount].btKey <= GBKEY_VK_9_NUMBERKEY)
		{
			iSymbolOffset = CLayoutVKSymbol__GetSymbolOffsetByIndex(pltObj, iSymbolIndex);
			//得到符号占的宽度
			iSymbolWidth = PT_GetStrWidthEx((GBLPWCHAR)(pSymbolData + iSymbolOffset), font);
			//显示到中间位置
			iSymbolWidth = (pButtonConstData[iCount].btNormal.rcPos.right - iSymbolWidth)/2;
			if (iSymbolWidth < 0)
			{
				iSymbolWidth = 0;
			}
			iSymbolHeight = PT_GetStrHeightEx((GBLPWCHAR)(pSymbolData + iSymbolOffset), font);
			//显示到中间位置
			iSymbolHeight = (pButtonConstData[iCount].btNormal.rcPos.bottom - iSymbolHeight)/2;
			if (iSymbolHeight < 0)
			{
				iSymbolHeight = 0;
			}
			PT_DrawString(pSymbolData + iSymbolOffset, font, SIGN_COLOR,
					(GBINT)(pButtonConstData[iCount].btNormal.rcPos.left + winPos.x + iSymbolWidth),
					(GBINT)(pButtonConstData[iCount].btNormal.rcPos.top + winPos.y) + iSymbolHeight);
			iSymbolIndex++;
		}
	}
#endif // __MTK__
}

#endif // 国笔虚拟键盘