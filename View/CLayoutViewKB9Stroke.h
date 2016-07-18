/***************************************************************************
* File Name    :  CLayoutViewKB9Stroke.h
* Corporation  :  Guobi Technology Ltd. GuangZhou China
* Copyright    :  Copyright Guobi Technology Ltd. And Subject to Confidentiality 
*                 Restrictions. This document is the confidential and proprietary 
*                 property of Guobi Technology Ltd.It is neither the document                  
*                 itself nor any part of it can be shared with any party  without 
*                 the written  permission. 
* Author       :  ZhaoKun
* Date         :  2010-10-15
* Purpose      :  物理九键笔画Layout视图类
* Version      :  V1.0
* Last Changed List
* Name         |   date   |  Discription
****************************************************************************/
#if defined(__GBIME_KB__) // 国笔物理键盘

#ifndef __CLASS_LAYOUT_VIEW_KB9_STROKE__
#define __CLASS_LAYOUT_VIEW_KB9_STROKE__
#include "CLayoutViewKB.h"

typedef struct _tag_CLayoutViewKB9Stroke {
	CLayoutViewKB            m_ltParent;

} CLayoutViewKB9Stroke, * PCLayoutViewKB9Stroke;

CLayoutViewAbstract * CLayoutViewKB9Stroke__Construct(PCLayoutViewKB9Stroke pltViewKB9);
GBIMEReturn CLayoutViewKB9Stroke__Init(PCLayoutViewKB9Stroke pltViewKB9);
#endif //__CLASS_LAYOUT_VIEW_KB9_STROKE__

#endif // 国笔物理键盘
