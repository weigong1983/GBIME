/***************************************************************************
* File Name    :  CLayoutViewHandwriteFullScreen.h
* Corporation  :  Guobi Technology Ltd. GuangZhou China
* Copyright    :  Copyright Guobi Technology Ltd. And Subject to Confidentiality 
*                 Restrictions. This document is the confidential and proprietary 
*                 property of Guobi Technology Ltd.It is neither the document                  
*                 itself nor any part of it can be shared with any party  without 
*                 the written  permission. 
* Author       :  weizhiping
* Date         :  2010-08-07
* Purpose      :  全屏手写键盘视图类
* Version      :  V1.0
* Last Changed List
* Name         |   date   |  Discription
****************************************************************************/

#ifndef __CLASS_LAYOUT_VIEW_HANDWRITE_FULLSCREEN__
#define __CLASS_LAYOUT_VIEW_HANDWRITE_FULLSCREEN__

#include "CHWEngine.h"
#include "CLayoutViewAbstract.h"
#include "CLayoutViewHandwrite.h"

#if defined(__GBIME_HANDWRITE__)

typedef struct _tag_CLayoutViewHandwriteFullScreen {
	CLayoutViewHandwrite            m_ltParent;
} CLayoutViewHandwriteFullScreen, * PCLayoutViewHandwriteFullScreen;

CLayoutViewAbstract * CLayoutViewHandwriteFullScreen__Construct(PCLayoutViewHandwriteFullScreen pltViewHandwrite);
GBIMEReturn CLayoutViewHandwriteFullScreen__Init(PCLayoutViewHandwriteFullScreen pltViewHandwrite);

#endif // defined(__GBIME_HANDWRITE__)
#endif //__CLASS_LAYOUT_VIEW_HANDWRITE_FULLSCREEN__


