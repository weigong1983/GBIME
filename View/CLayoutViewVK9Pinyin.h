/***************************************************************************
* File Name    :  CLayoutViewVK9Pinyin.h
* Corporation  :  Guobi Technology Ltd. GuangZhou China
* Copyright    :  Copyright Guobi Technology Ltd. And Subject to Confidentiality 
*                 Restrictions. This document is the confidential and proprietary 
*                 property of Guobi Technology Ltd.It is neither the document                  
*                 itself nor any part of it can be shared with any party  without 
*                 the written  permission. 
* Author       :  haj
* Date         :  2010-06-08
* Purpose      :  –Èƒ‚æ≈º¸∆¥“ÙLayout ”Õº¿‡
* Version      :  V1.0
* Last Changed List
* Name         |   date   |  Discription
****************************************************************************/
#if defined(__GBIME_VK__) // π˙± –Èƒ‚º¸≈Ã

#ifndef __CLASS_LAYOUT_VIEW_VK9_PINYIN__
#define __CLASS_LAYOUT_VIEW_VK9_PINYIN__
#include "CLayoutViewVK.h"

typedef struct _tag_CLayoutViewVK9Pinyin {
	CLayoutViewVK            m_ltParent;

} CLayoutViewVK9Pinyin, * PCLayoutViewVK9Pinyin;

CLayoutViewAbstract * CLayoutViewVK9Pinyin__Construct(PCLayoutViewVK9Pinyin pltViewVK);
GBIMEReturn CLayoutViewVK9Pinyin__Init(PCLayoutViewVK9Pinyin pltViewVK);

#endif //__CLASS_LAYOUT_VIEW_VK9_PINYIN__

#endif // π˙± –Èƒ‚º¸≈Ã
