/***************************************************************************
* File Name    :  CLayoutViewVKSymbol.h
* Corporation  :  Guobi Technology Ltd. GuangZhou China
* Copyright    :  Copyright Guobi Technology Ltd. And Subject to Confidentiality 
*                 Restrictions. This document is the confidential and proprietary 
*                 property of Guobi Technology Ltd.It is neither the document                  
*                 itself nor any part of it can be shared with any party  without 
*                 the written  permission. 
* Author       :  haj
* Date         :  2010-06-08
* Purpose      :  ÐéÄâ·ûºÅLayoutÊÓÍ¼Àà
* Version      :  V1.0
* Last Changed List
* Name         |   date   |  Discription
****************************************************************************/
#if defined(__GBIME_VK__) // ¹ú±ÊÐéÄâ¼üÅÌ

#ifndef __CLASS_LAYOUT_VIEW_VK_SYMBOL__
#define __CLASS_LAYOUT_VIEW_VK_SYMBOL__
#include "CLayoutViewVK.h"

typedef struct _tag_CLayoutViewVKSymbol {
	CLayoutViewVK            m_ltParent;
} CLayoutViewVKSymbol, * PCLayoutViewVKSymbol;

CLayoutViewAbstract * CLayoutViewVKSymbol__Construct(PCLayoutViewVKSymbol pltViewVKSymbol);
GBIMEReturn CLayoutViewVKSymbol__Init(PCLayoutViewVKSymbol pltViewVKSymbol);

#endif //__CLASS_LAYOUT_VIEW_VK_SYMBOL__

#endif // ¹ú±ÊÐéÄâ¼üÅÌ
