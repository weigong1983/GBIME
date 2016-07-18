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
* Purpose      :  ����Layout��ͼ��ʵ��
* Version      :  V1.0
* Last Changed List
* Name         |   date   |  Discription
****************************************************************************/
#include "CLayoutViewAbstract.h"
#include "CLayoutAbstract.h"
#include "GBIMEResDef.h"

GBIMELOCAL GBIMEReturn CLayoutViewAbstract__Paint(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo);

/*!
 * \brief ������ͼ��ʾ��Ա����
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
 * \brief ����״̬���ư�ť
 * \param pltObj 
 * \param layoutInScreenLeft 
 * \param layoutInScreenTop 
 * \param pClipRect != NULL��ʾ���ü�����
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

	// ȡ�ò����Ϣ
	pLayerConst = pltBase->m_pfcGetLayerConstInfo(pltObj, layoutId, layerId);
	// ȡ�ð�ť��Ϣ
	pButtonConstData = pltBase->m_pfcGetLayerConstButtons(pltObj, layoutId, layerId);
	if (pLayerConst == NULL || pButtonConstData == NULL)
	{
		return;
	}
	
	// ȡ�øð�ť��layout�е�����
	pltBase->m_pfcGetBottonRectByKey(pltObj,
									layoutId,
									layerId,
									keyValue,
									&buttonRect);
	// ���Ұ�ť��ͼƬID
	for (iCount = 0;iCount < pLayerConst->lyBtCount; iCount++)
	{
		// ���ҵ�ʱ
		if (pButtonConstData[iCount].btKey == keyValue)
		{
			// ȡͼƬID
			switch (buttonStatus)
			{
			case ButtonStatusNomal:
			case ButtonStatusUp:			
				iImageId = pButtonConstData[iCount].btNormal.imageID + IMG_GBIME_SYSTEM;//����ͼƬID��ַ;
				break;
			case ButtonStatusDown:
			case ButtonStatusMove:
				iImageId = pButtonConstData[iCount].btDown.imageID + IMG_GBIME_SYSTEM;//����ͼƬID��ַ;
				break;
			case ButtonStatusGray:
				iImageId = pButtonConstData[iCount].btGrey.imageID + IMG_GBIME_SYSTEM;//����ͼƬID��ַ;
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
 * \brief ���Ƽ�ͷ��ť
 * \param pltObj 
 * \param layoutInScreenLeft 
 * \param layoutInScreenTop 
 * \param pClipRect != NULL��ʾ���ü�����
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
	
	// ȡ�ò����Ϣ
	pLayerConst = pltBase->m_pfcGetLayerConstInfo(pltObj, layoutId, layerId);
	// ȡ�ð�ť��Ϣ
	pButtonConstData = pltBase->m_pfcGetLayerConstButtons(pltObj, layoutId, layerId);
	if (pLayerConst == NULL || pButtonConstData == NULL)
	{
		return;
	}
	
	// ȡ�øð�ť��layout�е�����
	pltBase->m_pfcGetBottonRectByKey(pltObj,
		layoutId,
		layerId,
		keyValue,
		&buttonRect);
	// ���Ұ�ť��ͼƬID
	for (iCount = 0;iCount < pLayerConst->lyBtCount; iCount++)
	{
		// ���ҵ�ʱ
		if (pButtonConstData[iCount].btKey == keyValue)
		{
			// ȡͼƬID
			switch (buttonStatus)
			{
			case ButtonStatusNomal:
			case ButtonStatusUp:			
				iImageId = pButtonConstData[iCount].btNormal.imageID + IMG_GBIME_SYSTEM;//����ͼƬID��ַ;
				break;
			case ButtonStatusDown:
			case ButtonStatusMove:
				iImageId = pButtonConstData[iCount].btDown.imageID + IMG_GBIME_SYSTEM;//����ͼƬID��ַ;
				break;
			case ButtonStatusGray:
				iImageId = pButtonConstData[iCount].btGrey.imageID + IMG_GBIME_SYSTEM;//����ͼƬID��ַ;
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
 * \brief ��ȡָ����ֵ�İ�ťͼƬID
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

	// ȡ�ò����Ϣ
	pLayerConst = pltBase->m_pfcGetLayerConstInfo(pltObj, layoutId, layerId);
	// ȡ�ð�ť��Ϣ
	pButtonConstData = pltBase->m_pfcGetLayerConstButtons(pltObj, layoutId, layerId);

	// ���Ұ�ť��ͼƬID
	for (iCount = 0;iCount < pLayerConst->lyBtCount; iCount++)
	{
		// ���ҵ�ʱ
		if (pButtonConstData[iCount].btKey == keyValue)
		{
			// ȡͼƬID
			switch (buttonStatus)
			{
			case ButtonStatusNomal:
			case ButtonStatusUp:
				iImageId = pButtonConstData[iCount].btNormal.imageID + IMG_GBIME_SYSTEM;//����ͼƬID��ַ;
				break;
			case ButtonStatusDown:
			case ButtonStatusMove:
				iImageId = pButtonConstData[iCount].btDown.imageID + IMG_GBIME_SYSTEM;//����ͼƬID��ַ;
				break;
			case ButtonStatusGray:
				iImageId = pButtonConstData[iCount].btGrey.imageID + IMG_GBIME_SYSTEM;//����ͼƬID��ַ;
				break;
			default:
				break;
			}

			break;
		}
	}
	return iImageId;
}