/***************************************************************************
* File Name    :  CLayoutViewVK26Pinyin.h
* Corporation  :  Guobi Technology Ltd. GuangZhou China
* Copyright    :  Copyright Guobi Technology Ltd. And Subject to Confidentiality 
*                 Restrictions. This document is the confidential and proprietary 
*                 property of Guobi Technology Ltd.It is neither the document                  
*                 itself nor any part of it can be shared with any party  without 
*                 the written  permission. 
* Author       :  haj
* Date         :  2010-06-08
* Purpose      :  –Èƒ‚QWERTY∆¥“ÙLayout ”Õº¿‡
* Version      :  V1.0
* Last Changed List
* Name         |   date   |  Discription
****************************************************************************/
#if defined(__GBIME_VK__) // π˙± –Èƒ‚º¸≈Ã

#ifndef __CLASS_LAYOUT_VIEW_VK26_PINYIN__
#define __CLASS_LAYOUT_VIEW_VK26_PINYIN__
#include "CLayoutViewVK.h"

typedef struct _tag_CLayoutViewVK26Pinyin {
	CLayoutViewVK            m_ltParent;
} CLayoutViewVK26Pinyin, * PCLayoutViewVK26Pinyin;

CLayoutViewAbstract * CLayoutViewVK26Pinyin__Construct(PCLayoutViewVK26Pinyin pltViewVK);
GBIMEReturn CLayoutViewVK26Pinyin__Init(PCLayoutViewVK26Pinyin pltViewVK);

#endif //__CLASS_LAYOUT_VIEW_VK26_PINYIN__

#endif // π˙± –Èƒ‚º¸≈Ã

