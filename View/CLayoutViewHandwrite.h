/***************************************************************************
* File Name    :  CLayoutViewHandwrite.h
* Corporation  :  Guobi Technology Ltd. GuangZhou China
* Copyright    :  Copyright Guobi Technology Ltd. And Subject to Confidentiality 
*                 Restrictions. This document is the confidential and proprietary 
*                 property of Guobi Technology Ltd.It is neither the document                  
*                 itself nor any part of it can be shared with any party  without 
*                 the written  permission. 
* Author       :  weizhiping
* Date         :  2010-08-07
* Purpose      :  手写键盘视图抽象类
* Version      :  V1.0
* Last Changed List
* Name         |   date   |  Discription
****************************************************************************/

#ifndef __CLASS_LAYOUT_VIEW_HANDWRITE__
#define __CLASS_LAYOUT_VIEW_HANDWRITE__

#include "CHWEngine.h"
#include "CLayoutViewAbstract.h"

#if defined(__GBIME_HANDWRITE__)

typedef struct _tag_CLayoutViewHandwrite {
	CLayoutViewAbstract            m_ltParent;
} CLayoutViewHandwrite, * PCLayoutViewHandwrite;

CLayoutViewAbstract * CLayoutViewHandwrite__Construct(PCLayoutViewHandwrite pltViewHandwrite);
GBIMEReturn CLayoutViewHandwrite__Init(PCLayoutViewHandwrite pltViewHandwrite);

#endif // defined(__GBIME_HANDWRITE__)
#endif //__CLASS_LAYOUT_VIEW_HANDWRITE__


