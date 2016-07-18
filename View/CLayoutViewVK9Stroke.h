/***************************************************************************
* File Name    :  CLayoutViewVK9Stroke.h
* Corporation  :  Guobi Technology Ltd. GuangZhou China
* Copyright    :  Copyright Guobi Technology Ltd. And Subject to Confidentiality 
*                 Restrictions. This document is the confidential and proprietary 
*                 property of Guobi Technology Ltd.It is neither the document                  
*                 itself nor any part of it can be shared with any party  without 
*                 the written  permission. 
* Author       :  haj
* Date         :  2010-06-08
* Purpose      :  ÐéÄâ¾Å¼ü±Ê»­LayoutÊÓÍ¼Àà
* Version      :  V1.0
* Last Changed List
* Name         |   date   |  Discription
****************************************************************************/
#if defined(__GBIME_VK__) // ¹ú±ÊÐéÄâ¼üÅÌ

#ifndef __CLASS_LAYOUT_VIEW_VK9_STROKE__
#define __CLASS_LAYOUT_VIEW_VK9_STROKE__
#include "CLayoutViewVK.h"

typedef struct _tag_CLayoutViewVK9Stroke {
	CLayoutViewVK            m_ltParent;

} CLayoutViewVK9Stroke, * PCLayoutViewVK9Stroke;

CLayoutViewAbstract * CLayoutViewVK9Stroke__Construct(PCLayoutViewVK9Stroke pltViewVK);
GBIMEReturn CLayoutViewVK9Stroke__Init(PCLayoutViewVK9Stroke pltViewVK);

#endif //__CLASS_LAYOUT_VIEW_VK9_STROKE__

#endif // ¹ú±ÊÐéÄâ¼üÅÌ
