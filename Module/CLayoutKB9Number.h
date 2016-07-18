/***************************************************************************
* File Name    :  CLayoutKB9Number.h
* Corporation  :  Guobi Technology Ltd. GuangZhou China
* Copyright    :  Copyright Guobi Technology Ltd. And Subject to Confidentiality 
*                 Restrictions. This document is the confidential and proprietary 
*                 property of Guobi Technology Ltd.It is neither the document                  
*                 itself nor any part of it can be shared with any party  without 
*                 the written  permission. 
* Author       :  ZhaoKun
* Date         :  2010-10-15
* Purpose      :  物理九键数字Layout
* Version      :  V1.0
* Last Changed List
* Name         |   date   |  Discription
****************************************************************************/
#if defined(__GBIME_KB__) // 国笔物理键盘

#ifndef __CLASS_LAYOUT_KB9_NUMBER__
#define __CLASS_LAYOUT_KB9_NUMBER__
#include "CLayoutKB.h"

/*!
 * \Layout this指针和Layout 基类指针转换
 */
#undef CLASE_THIS_POITER
#define CLASE_THIS_POITER(CLASS_NAME, pltObj)   \
	CLASS_NAME * pltThis = (CLASS_NAME *)pltObj; \
	PCLayoutKB pltBase = &pltThis->m_ltParent; \
	PCLayoutAbstract pltRoot = &pltThis->m_ltParent.m_ltParent;

typedef struct _tag_CLayoutKB9Number {
	CLayoutKB				m_ltParent;
} CLayoutKB9Number, * PCLayoutKB9Number;

CLayoutAbstract * CLayoutKB9Number__Construct(PCLayoutKB9Number pltKB, 
									   GBLPCVOID pltUIData,
									   GBLPCVOID pltIMData,
									   GBLPCVOID pltConfigData);
#endif //__CLASS_LAYOUT_KB9_Number__

#endif // 国笔物理键盘

