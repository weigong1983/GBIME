/***************************************************************************
* File Name    :  CLayoutViewKB9Pinyin.h
* Corporation  :  Guobi Technology Ltd. GuangZhou China
* Copyright    :  Copyright Guobi Technology Ltd. And Subject to Confidentiality 
*                 Restrictions. This document is the confidential and proprietary 
*                 property of Guobi Technology Ltd.It is neither the document                  
*                 itself nor any part of it can be shared with any party  without 
*                 the written  permission. 
* Author       :  ZhaoKun
* Date         :  2010-10-15
* Purpose      :  物理九键拼音Layout视图类
* Version      :  V1.0
* Last Changed List
* Name         |   date   |  Discription
****************************************************************************/
#if defined(__GBIME_KB__) // 国笔物理键盘

#ifndef __CLASS_LAYOUT_VIEW_KB9_PINYIN__
#define __CLASS_LAYOUT_VIEW_KB9_PINYIN__
#include "CLayoutViewKB.h"

typedef struct _tag_CLayoutViewKB9Pinyin {
	CLayoutViewKB            m_ltParent;

} CLayoutViewKB9Pinyin, * PCLayoutViewKB9Pinyin;

CLayoutViewAbstract * CLayoutViewKB9Pinyin__Construct(PCLayoutViewKB9Pinyin pltViewKB9);
GBIMEReturn CLayoutViewKB9Pinyin__Init(PCLayoutViewKB9Pinyin pltViewKB9);
#endif //__CLASS_LAYOUT_VIEW_KB9_PINYIN__

#endif // 国笔物理键盘

