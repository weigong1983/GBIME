/***************************************************************************
* File Name    :  CLayoutViewAbstract.c
* Corporation  :  Guobi Technology Ltd. GuangZhou China
* Copyright    :  Copyright Guobi Technology Ltd. And Subject to Confidentiality 
*                 Restrictions. This document is the confidential and proprietary 
*                 property of Guobi Technology Ltd.It is neither the document                  
*                 itself nor any part of it can be shared with any party  without 
*                 the written  permission. 
* Author       :  weizhiping
* Date         :  2010-07-09
* Purpose      :  抽象Layout视图类实现
* Version      :  V1.0
* Last Changed List
* Name         |   date   |  Discription
****************************************************************************/
#include "CLayoutViewAbstract.h"
#include "CLayoutAbstract.h"
#include "GBIMEResDef.h"

GBIMELOCAL GBIMEReturn CLayoutViewAbstract__Paint(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo);

/*!
 * \brief 公共视图显示成员函数
 */
GBIMELOCAL void ICLayoutViewAbstract__DrawButtonByStatus(GBLPVOID pltObj,
														 GBUINT16 layoutInScreenLeft,
														 GBUINT16 layoutInScreenTop,
														 PGBRECT pClipRect,
														 GBUINT16 layoutId,
														 GBUINT16 layerId,
														 GBUINT16 keyValue,		
														 ButtonStatusEnum buttonStatus);

GBIMELOCAL void ICLayoutViewAbstract__DrawArrowButton(GBLPVOID pltObj,
													  GBUINT16 layoutInScreenLeft,
													  GBUINT16 layoutInScreenTop,
													  GBUINT16 layoutId,
													  GBUINT16 layerId,
													  GBUINT16 keyValue,
													  ButtonStatusEnum buttonStatus);

GBIMELOCAL GBUINT16 ICLayoutViewAbstract__GetButtonImageID(GBLPVOID pltObj,
														   GBUINT16 layoutId,
														   GBUINT16 layerId,
														   GBUINT16 keyValue,
														   ButtonStatusEnum buttonStatus);

CLayoutViewAbstract * CLayoutViewAbstract__Construct(PCLayoutViewAbstract pltDest)
{
	CLayoutViewAbstract__Init(pltDest);
	return pltDest;
}

GBIMEReturn CLayoutViewAbstract__Init(PCLayoutViewAbstract pltDest)
{
	pltDest->m_pfcPaint = CLayoutViewAbstract__Paint;
	pltDest->m_pfcDrawButtonByStatus = ICLayoutViewAbstract__DrawButtonByStatus;
	pltDest->m_pfcDrawArrowButton = ICLayoutViewAbstract__DrawArrowButton;
	pltDest->m_pfcGetButtonImageID = ICLayoutViewAbstract__GetButtonImageID;
	return GBIME_OK;
}

GBIMELOCAL GBIMEReturn CLayoutViewAbstract__Paint(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo)
{
	return GBIME_OK;
}

/*!
 * \brief 根据状态绘制按钮
 * \param pltObj 
 * \param layoutInScreenLeft 
 * \param layoutInScreenTop 
 * \param pClipRect != NULL表示带裁剪处理
 * \param layoutId 
 * \param layerId 
 * \param keyValue 
 * \param buttonStatus 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-26 14:38:01
*/
static void ICLayoutViewAbstract__DrawButtonByStatus(GBLPVOID pltObj,
													 GBUINT16 layoutInScreenLeft,
													 GBUINT16 layoutInScreenTop,
													 PGBRECT pClipRect,
													 GBUINT16 layoutId,
													 GBUINT16 layerId,
													 GBUINT16 keyValue,
													 ButtonStatusEnum buttonStatus)
{
	PCLayoutAbstract pltBase = (PCLayoutAbstract)pltObj;
	PBUTTONCONSTDATA pButtonConstData;
	PLAYOUTCONSTLAYER pLayerConst;
	GBRECT screenRect,imageRect;
	GBRECT buttonRect;
	GBUINT16 iImageId;
	GBUINT16 iCount;
	GBUINT16 iImageDisplayX, iImageDisplayY;

	// 取得层的信息
	pLayerConst = pltBase->m_pfcGetLayerConstInfo(pltObj, layoutId, layerId);
	// 取得按钮信息
	pButtonConstData = pltBase->m_pfcGetLayerConstButtons(pltObj, layoutId, layerId);
	if (pLayerConst == NULL || pButtonConstData == NULL)
	{
		return;
	}
	
	// 取得该按钮在layout中的区域
	pltBase->m_pfcGetBottonRectByKey(pltObj,
									layoutId,
									layerId,
									keyValue,
									&buttonRect);
	// 查找按钮的图片ID
	for (iCount = 0;iCount < pLayerConst->lyBtCount; iCount++)
	{
		// 查找到时
		if (pButtonConstData[iCount].btKey == keyValue)
		{
			// 取图片ID
			switch (buttonStatus)
			{
			case ButtonStatusNomal:
			case ButtonStatusUp:			
				iImageId = pButtonConstData[iCount].btNormal.imageID + IMG_GBIME_SYSTEM;//加上图片ID基址;
				break;
			case ButtonStatusDown:
			case ButtonStatusMove:
				iImageId = pButtonConstData[iCount].btDown.imageID + IMG_GBIME_SYSTEM;//加上图片ID基址;
				break;
			case ButtonStatusGray:
				iImageId = pButtonConstData[iCount].btGrey.imageID + IMG_GBIME_SYSTEM;//加上图片ID基址;
				break;
			default:
				PT_Assert(0);
			}
			
			iImageDisplayX = buttonRect.left + layoutInScreenLeft;
			iImageDisplayY = buttonRect.top + layoutInScreenTop;
			if (buttonStatus == ButtonStatusGray)
			{
				PT_DrawImage((GBINT)iImageDisplayX, (GBINT)iImageDisplayY, iImageId);
			} 
			else
			{		
				if (pClipRect != NULL)
				{
					memcpy(&imageRect, pClipRect, sizeof(GBRECT));
					memcpy(&screenRect, pClipRect, sizeof(GBRECT));
					MOVE_RECT(&imageRect, -layoutInScreenLeft, -layoutInScreenTop);
				}
				else
				{
					memcpy(&imageRect, &buttonRect, sizeof(GBRECT));
					memcpy(&screenRect, &buttonRect, sizeof(GBRECT));
					MOVE_RECT(&screenRect, layoutInScreenLeft, layoutInScreenTop);
				}				
				PT_DrawImageWithClip(iImageId,&screenRect,&imageRect);
			}
			
		}
	}
}

/*!
 * \brief 绘制箭头按钮
 * \param pltObj 
 * \param layoutInScreenLeft 
 * \param layoutInScreenTop 
 * \param pClipRect != NULL表示带裁剪处理
 * \param layoutId 
 * \param layerId 
 * \param keyValue 
 * \param buttonStatus 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-26 14:38:01
*/
static void ICLayoutViewAbstract__DrawArrowButton(GBLPVOID pltObj,
												  GBUINT16 layoutInScreenLeft,
												  GBUINT16 layoutInScreenTop,
												  GBUINT16 layoutId,
												  GBUINT16 layerId,
												  GBUINT16 keyValue,
												  ButtonStatusEnum buttonStatus)
{
	PCLayoutAbstract pltBase = (PCLayoutAbstract)pltObj;
	PBUTTONCONSTDATA pButtonConstData;
	PLAYOUTCONSTLAYER pLayerConst;
//	GBRECT screenRect,imageRect;
	GBRECT buttonRect;
	GBUINT16 iImageId;
	GBUINT16 iCount;
	GBUINT16 iImageDisplayX, iImageDisplayY;
	
	// 取得层的信息
	pLayerConst = pltBase->m_pfcGetLayerConstInfo(pltObj, layoutId, layerId);
	// 取得按钮信息
	pButtonConstData = pltBase->m_pfcGetLayerConstButtons(pltObj, layoutId, layerId);
	if (pLayerConst == NULL || pButtonConstData == NULL)
	{
		return;
	}
	
	// 取得该按钮在layout中的区域
	pltBase->m_pfcGetBottonRectByKey(pltObj,
		layoutId,
		layerId,
		keyValue,
		&buttonRect);
	// 查找按钮的图片ID
	for (iCount = 0;iCount < pLayerConst->lyBtCount; iCount++)
	{
		// 查找到时
		if (pButtonConstData[iCount].btKey == keyValue)
		{
			// 取图片ID
			switch (buttonStatus)
			{
			case ButtonStatusNomal:
			case ButtonStatusUp:			
				iImageId = pButtonConstData[iCount].btNormal.imageID + IMG_GBIME_SYSTEM;//加上图片ID基址;
				break;
			case ButtonStatusDown:
			case ButtonStatusMove:
				iImageId = pButtonConstData[iCount].btDown.imageID + IMG_GBIME_SYSTEM;//加上图片ID基址;
				break;
			case ButtonStatusGray:
				iImageId = pButtonConstData[iCount].btGrey.imageID + IMG_GBIME_SYSTEM;//加上图片ID基址;
				break;
				break;
			default:
				PT_Assert(0);
			}
			
			iImageDisplayX = buttonRect.left + layoutInScreenLeft;
			iImageDisplayY = buttonRect.top + layoutInScreenTop;
			if (1)//(buttonStatus == ButtonStatusGray)
			{
				PT_DrawImage((GBINT)iImageDisplayX, (GBINT)(iImageDisplayY + 1), iImageId);
			} 
//			else
//			{		
// 				if (pClipRect != NULL)
// 				{
// 					memcpy(&imageRect, pClipRect, sizeof(GBRECT));
// 					memcpy(&screenRect, pClipRect, sizeof(GBRECT));
// 					MOVE_RECT(&imageRect, -layoutInScreenLeft, -layoutInScreenTop);
// 				}
// 				else
// 				{
//					memcpy(&imageRect, &buttonRect, sizeof(GBRECT));
//					memcpy(&screenRect, &buttonRect, sizeof(GBRECT));
//					MOVE_RECT(&screenRect, layoutInScreenLeft, layoutInScreenTop);
//				}				
//				PT_DrawImageWithClip(iImageId,&screenRect,&imageRect);
//			}
			
		}
	}
}
/*!
 * \brief 获取指定键值的按钮图片ID
 * \param pltObj 
 * \param layoutId 
 * \param layerId 
 * \param keyValue 
 * \param buttonStatus 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-25 12:35:24
*/
GBIMELOCAL GBUINT16 ICLayoutViewAbstract__GetButtonImageID(GBLPVOID pltObj,
														   GBUINT16 layoutId,
														   GBUINT16 layerId,
														   GBUINT16 keyValue,
														   ButtonStatusEnum buttonStatus)
{
	PCLayoutAbstract pltBase = (PCLayoutAbstract)pltObj;
	PBUTTONCONSTDATA pButtonConstData;
	PLAYOUTCONSTLAYER pLayerConst;
	GBUINT16 iImageId = -1;
	GBUINT16 iCount;

	// 取得层的信息
	pLayerConst = pltBase->m_pfcGetLayerConstInfo(pltObj, layoutId, layerId);
	// 取得按钮信息
	pButtonConstData = pltBase->m_pfcGetLayerConstButtons(pltObj, layoutId, layerId);

	// 查找按钮的图片ID
	for (iCount = 0;iCount < pLayerConst->lyBtCount; iCount++)
	{
		// 查找到时
		if (pButtonConstData[iCount].btKey == keyValue)
		{
			// 取图片ID
			switch (buttonStatus)
			{
			case ButtonStatusNomal:
			case ButtonStatusUp:
				iImageId = pButtonConstData[iCount].btNormal.imageID + IMG_GBIME_SYSTEM;//加上图片ID基址;
				break;
			case ButtonStatusDown:
			case ButtonStatusMove:
				iImageId = pButtonConstData[iCount].btDown.imageID + IMG_GBIME_SYSTEM;//加上图片ID基址;
				break;
			case ButtonStatusGray:
				iImageId = pButtonConstData[iCount].btGrey.imageID + IMG_GBIME_SYSTEM;//加上图片ID基址;
				break;
			default:
				break;
			}

			break;
		}
	}
	return iImageId;
}