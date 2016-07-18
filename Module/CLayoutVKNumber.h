/***************************************************************************
* File Name    :  CLayoutVKNumber.h
* Corporation  :  Guobi Technology Ltd. GuangZhou China
* Copyright    :  Copyright Guobi Technology Ltd. And Subject to Confidentiality 
*                 Restrictions. This document is the confidential and proprietary 
*                 property of Guobi Technology Ltd.It is neither the document                  
*                 itself nor any part of it can be shared with any party  without 
*                 the written  permission. 
* Author       :  haj
* Date         :  2010-06-08
* Purpose      :  虚拟数字Layout
* Version      :  V1.0
* Last Changed List
* Name         |   date   |  Discription
****************************************************************************/
#if defined(__GBIME_VK__) // 国笔虚拟键盘

#ifndef __CLASS_LAYOUT_VK_NUMBER__
#define __CLASS_LAYOUT_VK_NUMBER__
#include "CLayoutVK.h"

/*!
 * \Layout this指针和Layout 基类指针转换
 */
#undef CLASE_THIS_POITER
#define CLASE_THIS_POITER(CLASS_NAME, pltObj)   \
	CLASS_NAME * pltThis = (CLASS_NAME *)pltObj; \
	PCLayoutVK pltBase = &pltThis->m_ltParent; \
	PCLayoutAbstract pltRoot = &pltThis->m_ltParent.m_ltParent;

typedef struct _tag_CLayoutVKNumber {
	CLayoutVK				m_ltParent;
} CLayoutVKNumber, * PCLayoutVKNumber;

CLayoutAbstract * CLayoutVKNumber__Construct(PCLayoutVKNumber pltVKNumber, 
											 GBLPCVOID pltUIData,
											 GBLPCVOID pltIMData,
											 GBLPCVOID pltConfigData);
#endif //__CLASS_LAYOUT_VK_NUMBER__

#endif // 国笔虚拟键盘
