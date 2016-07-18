/***************************************************************************
* File Name    :  CLayoutViewAbstract.h
* Corporation  :  Guobi Technology Ltd. GuangZhou China
* Copyright    :  Copyright Guobi Technology Ltd. And Subject to Confidentiality 
*                 Restrictions. This document is the confidential and proprietary 
*                 property of Guobi Technology Ltd.It is neither the document                  
*                 itself nor any part of it can be shared with any party  without 
*                 the written  permission. 
* Author       :  weizhiping
* Date         :  2010-07-09
* Purpose      :  抽象Layout视图类
* Version      :  V1.0
* Last Changed List
* Name         |   date   |  Discription
****************************************************************************/

#ifndef __CLASS_LAYOUT_VIEW_ABSTRACT_
#define __CLASS_LAYOUT_VIEW_ABSTRACT_
#include "PlatformTools.h"
#include "gbtype.h"
#include "gbdef.h"
#include "gbft_def.h"
#include "GBIMEDef.h"
#include "CLayoutInfo.h"
#include "CLayoutControl.h"

typedef GBINT32 (* PFCLtView__Paint)(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo);
typedef void (*PFCLtView__DrawButtonByStatus)(GBLPVOID pltObj,
											  GBUINT16 layoutInScreenLeft,
											  GBUINT16 layoutInScreenTop,
											  PGBRECT pClipRect,
											  GBUINT16 layoutId,
											  GBUINT16 layerId,
											  GBUINT16 keyValue,
											  ButtonStatusEnum buttonStatus);

typedef void (*PFCLtView__DrawArrowButton)(GBLPVOID pltObj,
										   GBUINT16 layoutInScreenLeft,
										   GBUINT16 layoutInScreenTop,
										   GBUINT16 layoutId,
										   GBUINT16 layerId,
										   GBUINT16 keyValue,
										   ButtonStatusEnum buttonStatus);

typedef GBUINT16 (*PFCLtView__GetButtonImageID)(GBLPVOID pltObj,
												GBUINT16 layoutId,
												GBUINT16 layerId,
												GBUINT16 keyValue,
												ButtonStatusEnum buttonStatus);

typedef struct _tag_CLayoutViewAbstract {
	PFCLtView__Paint				m_pfcPaint;
	PFCLtView__DrawButtonByStatus	m_pfcDrawButtonByStatus;///<根据按钮状态绘制按钮
	PFCLtView__DrawArrowButton		m_pfcDrawArrowButton;
	PFCLtView__GetButtonImageID		m_pfcGetButtonImageID;///<根据按钮状态获取按钮图片ID
}CLayoutViewAbstract, *PCLayoutViewAbstract;

CLayoutViewAbstract * CLayoutViewAbstract__Construct(PCLayoutViewAbstract pltDest);
GBIMEReturn CLayoutViewAbstract__Init(PCLayoutViewAbstract pltDest);

#endif //__CLASS_LAYOUT_VIEW_ABSTRACT_


