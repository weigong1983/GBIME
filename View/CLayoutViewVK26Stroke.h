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
* Purpose      :  –Èƒ‚QWERTY± ª≠Layout ”Õº¿‡
* Version      :  V1.0
* Last Changed List
* Name         |   date   |  Discription
****************************************************************************/
#if defined(__GBIME_VK__) // π˙± –Èƒ‚º¸≈Ã

#ifndef __CLASS_LAYOUT_VIEW_VK26_STROKE__
#define __CLASS_LAYOUT_VIEW_VK26_STROKE__
#include "CLayoutViewVK.h"

typedef struct _tag_CLayoutViewVK26Stroke {
	CLayoutViewVK            m_ltParent;

} CLayoutViewVK26Stroke, * PCLayoutViewVK26Stroke;

CLayoutViewAbstract * CLayoutViewVK26Stroke__Construct(PCLayoutViewVK26Stroke pltViewVK);
GBIMEReturn CLayoutViewVK26Stroke__Init(PCLayoutViewVK26Stroke pltViewVK);

#endif //__CLASS_LAYOUT_VIEW_VK26_STROKE__

#endif // π˙± –Èƒ‚º¸≈Ã

