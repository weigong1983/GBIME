/***************************************************************************
* File Name    :  CLayoutVK9Pinyin.h
* Corporation  :  Guobi Technology Ltd. GuangZhou China
* Copyright    :  Copyright Guobi Technology Ltd. And Subject to Confidentiality 
*                 Restrictions. This document is the confidential and proprietary 
*                 property of Guobi Technology Ltd.It is neither the document                  
*                 itself nor any part of it can be shared with any party  without 
*                 the written  permission. 
* Author       :  haj
* Date         :  2010-06-08
* Purpose      :  ÐéÄâ¾Å¼üÆ´ÒôLayout
* Version      :  V1.0
* Last Changed List
* Name         |   date   |  Discription
****************************************************************************/
#if defined(__GBIME_VK__) // ¹ú±ÊÐéÄâ¼üÅÌ

#ifndef __CLASS_LAYOUT_VK9_PINYIN__
#define __CLASS_LAYOUT_VK9_PINYIN__
#include "CLayoutVK.h"

/*!
 * \Layout thisÖ¸ÕëºÍLayout »ùÀàÖ¸Õë×ª»»
 */
#undef CLASE_THIS_POITER
#define CLASE_THIS_POITER(CLASS_NAME, pltObj)   \
	CLASS_NAME * pltThis = (CLASS_NAME *)pltObj; \
	PCLayoutVK pltBase = &pltThis->m_ltParent; \
	PCLayoutAbstract pltRoot = &pltThis->m_ltParent.m_ltParent;

typedef struct _tag_CLayoutVK9Pinyin {
	CLayoutVK				m_ltParent;
} CLayoutVK9Pinyin, * PCLayoutVK9Pinyin;

CLayoutAbstract * CLayoutVK9Pinyin__Construct(PCLayoutVK9Pinyin pltVK, 
									   GBLPCVOID pltUIData,
									   GBLPCVOID pltIMData,
									   GBLPCVOID pltConfigData);
#endif //__CLASS_LAYOUT_VK9_PINYIN__

#endif // ¹ú±ÊÐéÄâ¼üÅÌ
