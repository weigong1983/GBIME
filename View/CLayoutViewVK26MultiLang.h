/***************************************************************************
* File Name    :  CLayoutViewVK26MultiLang.h
* Corporation  :  Guobi Technology Ltd. GuangZhou China
* Copyright    :  Copyright Guobi Technology Ltd. And Subject to Confidentiality 
*                 Restrictions. This document is the confidential and proprietary 
*                 property of Guobi Technology Ltd.It is neither the document                  
*                 itself nor any part of it can be shared with any party  without 
*                 the written  permission. 
* Author       :  haj
* Date         :  2010-06-08
* Purpose      :  –Èƒ‚QWERTY ∂‡”Ô—‘Layout ”Õº¿‡
* Version      :  V1.0
* Last Changed List
* Name         |   date   |  Discription
****************************************************************************/
#if defined(__GBIME_VK__) // π˙± –Èƒ‚º¸≈Ã

#ifndef __CLASS_LAYOUT_VIEW_VK26_MULTILANG__
#define __CLASS_LAYOUT_VIEW_VK26_MULTILANG__
#include "CLayoutViewVK.h"

typedef struct _tag_CLayoutViewVK26MultiLang {
	CLayoutViewVK            m_ltParent;
} CLayoutViewVK26MultiLang, * PCLayoutViewVK26MultiLang;

CLayoutViewAbstract * CLayoutViewVK26MultiLang__Construct(PCLayoutViewVK26MultiLang pltViewVK);
GBIMEReturn CLayoutViewVK26MultiLang__Init(PCLayoutViewVK26MultiLang pltViewVK);

#endif //__CLASS_LAYOUT_VIEW_VK26_MULTILANG__

#endif // π˙± –Èƒ‚º¸≈Ã

