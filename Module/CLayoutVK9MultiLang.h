/***************************************************************************
* File Name    :  CLayoutVK9MultiLang.h
* Corporation  :  Guobi Technology Ltd. GuangZhou China
* Copyright    :  Copyright Guobi Technology Ltd. And Subject to Confidentiality 
*                 Restrictions. This document is the confidential and proprietary 
*                 property of Guobi Technology Ltd.It is neither the document                  
*                 itself nor any part of it can be shared with any party  without 
*                 the written  permission. 
* Author       :  haj
* Date         :  2010-06-08
* Purpose      :  ÐéÄâ¾Å¼ü¶àÓïÑÔLayout
* Version      :  V1.0
* Last Changed List
* Name         |   date   |  Discription
****************************************************************************/
#if defined(__GBIME_VK__) // ¹ú±ÊÐéÄâ¼üÅÌ

#ifndef __CLASS_LAYOUT_VK9_MULTILANG__
#define __CLASS_LAYOUT_VK9_MULTILANG__
#include "CLayoutVK.h"

/*!
 * \Layout thisÖ¸ÕëºÍLayout »ùÀàÖ¸Õë×ª»»
 */
#undef CLASE_THIS_POITER
#define CLASE_THIS_POITER(CLASS_NAME, pltObj)   \
	CLASS_NAME * pltThis = (CLASS_NAME *)pltObj; \
	PCLayoutVK pltBase = &pltThis->m_ltParent; \
	PCLayoutAbstract pltRoot = &pltThis->m_ltParent.m_ltParent;

typedef struct _tag_CLayoutVK9MultiLang {
	CLayoutVK				m_ltParent;
} CLayoutVK9MultiLang, * PCLayoutVK9MultiLang;

CLayoutAbstract * CLayoutVK9MultiLang__Construct(PCLayoutVK9MultiLang pltVK, 
									   GBLPCVOID pltUIData,
									   GBLPCVOID pltIMData,
									   GBLPCVOID pltConfigData);
#endif //__CLASS_LAYOUT_VK9_MULTILANG__

#endif // ¹ú±ÊÐéÄâ¼üÅÌ
