/***************************************************************************
* File Name    :  CLayoutHandwriteMultiBlock.h
* Corporation  :  Guobi Technology Ltd. GuangZhou China
* Copyright    :  Copyright Guobi Technology Ltd. And Subject to Confidentiality 
*                 Restrictions. This document is the confidential and proprietary 
*                 property of Guobi Technology Ltd.It is neither the document                  
*                 itself nor any part of it can be shared with any party  without 
*                 the written  permission. 
* Author       :  weizhiping
* Date         :  2010-09-08
* Purpose      :  格子手写键盘
* Version      :  V1.0
* Last Changed List
* Name         |   date   |  Discription
****************************************************************************/

#ifndef __CLASS_LAYOUT_HANDWRITE_MULTI_BLOCK__
#define __CLASS_LAYOUT_HANDWRITE_MULTI_BLOCK__

#include "CLayoutHandwrite.h"

#if defined(__GBIME_HANDWRITE__)

/*!
 * \Layout this指针和Layout 基类指针转换
 */
#undef CLASE_THIS_POITER
#define CLASE_THIS_POITER(CLASS_NAME, pltObj)   \
	CLASS_NAME * pltThis = (CLASS_NAME *)pltObj; \
	PCLayoutHandwrite pltBase = &pltThis->m_ltParent; \
	PCLayoutAbstract pltRoot = &pltThis->m_ltParent.m_ltParent;

typedef struct _tag_CLayoutHandwriteMultiBlock {
	CLayoutHandwrite		m_ltParent;
} CLayoutHandwriteMultiBlock, * PCLayoutHandwriteMultiBlock;

CLayoutAbstract * CLayoutHandwriteMultiBlock__Construct(PCLayoutHandwriteMultiBlock pltHandwrite, 
							GBLPCVOID pltUIData,
							GBLPCVOID pltIMData,
							GBLPCVOID pltConfigData);

#endif // defined(__GBIME_HANDWRITE__)

#endif //__CLASS_LAYOUT_HANDWRITE_MULTI_BLOCK__


