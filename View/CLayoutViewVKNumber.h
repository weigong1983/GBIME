/***************************************************************************
* File Name    :  CLayoutViewVKNumber.h
* Corporation  :  Guobi Technology Ltd. GuangZhou China
* Copyright    :  Copyright Guobi Technology Ltd. And Subject to Confidentiality 
*                 Restrictions. This document is the confidential and proprietary 
*                 property of Guobi Technology Ltd.It is neither the document                  
*                 itself nor any part of it can be shared with any party  without 
*                 the written  permission. 
* Author       :  haj
* Date         :  2010-06-08
* Purpose      :  ��������Layout��ͼ��
* Version      :  V1.0
* Last Changed List
* Name         |   date   |  Discription
****************************************************************************/
#if defined(__GBIME_VK__) // �����������

#ifndef __CLASS_LAYOUT_VIEW_VK_NUMBER__
#define __CLASS_LAYOUT_VIEW_VK_NUMBER__
#include "CLayoutViewVK.h"

typedef struct _tag_CLayoutViewVKNumber {
	CLayoutViewVK            m_ltParent;

} CLayoutViewVKNumber, * PCLayoutViewVKNumber;

CLayoutViewAbstract * CLayoutViewVKNumber__Construct(PCLayoutViewVKNumber pltViewVKNumber);
GBIMEReturn CLayoutViewVKNumber__Init(PCLayoutViewVKNumber pltViewVKNumber);

#endif //__CLASS_LAYOUT_VIEW_VK_NUMBER__

#endif // �����������

