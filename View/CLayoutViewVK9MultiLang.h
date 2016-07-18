/***************************************************************************
* File Name    :  CLayoutViewVK9MultiLang.h
* Corporation  :  Guobi Technology Ltd. GuangZhou China
* Copyright    :  Copyright Guobi Technology Ltd. And Subject to Confidentiality 
*                 Restrictions. This document is the confidential and proprietary 
*                 property of Guobi Technology Ltd.It is neither the document                  
*                 itself nor any part of it can be shared with any party  without 
*                 the written  permission. 
* Author       :  haj
* Date         :  2010-06-08
* Purpose      :  –Èƒ‚æ≈º¸∂‡”Ô—‘Layout ”Õº¿‡
* Version      :  V1.0
* Last Changed List
* Name         |   date   |  Discription
****************************************************************************/
#if defined(__GBIME_VK__) // π˙± –Èƒ‚º¸≈Ã

#ifndef __CLASS_LAYOUT_VIEW_VK9_MULTILANG__
#define __CLASS_LAYOUT_VIEW_VK9_MULTILANG__
#include "CLayoutViewVK.h"

typedef struct _tag_CLayoutViewVK9MultiLang {
	CLayoutViewVK            m_ltParent;

} CLayoutViewVK9MultiLang, * PCLayoutViewVK9MultiLang;

CLayoutViewAbstract * CLayoutViewVK9MultiLang__Construct(PCLayoutViewVK9MultiLang pltViewVK);
GBIMEReturn CLayoutViewVK9MultiLang__Init(PCLayoutViewVK9MultiLang pltViewVK);

#endif //__CLASS_LAYOUT_VIEW_VK9_MULTILANG__

#endif // π˙± –Èƒ‚º¸≈Ã

