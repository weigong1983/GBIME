/***************************************************************************
* File Name    :  CLayoutVK26Stroke.h
* Corporation  :  Guobi Technology Ltd. GuangZhou China
* Copyright    :  Copyright Guobi Technology Ltd. And Subject to Confidentiality 
*                 Restrictions. This document is the confidential and proprietary 
*                 property of Guobi Technology Ltd.It is neither the document                  
*                 itself nor any part of it can be shared with any party  without 
*                 the written  permission. 
* Author       :  weizhiping
* Date         :  2010-08-17
* Purpose      :  ÐéÄâQwerty±Ê»­Layout
* Version      :  V1.0
* Last Changed List
* Name         |   date   |  Discription
****************************************************************************/
#if defined(__GBIME_VK__) // ¹ú±ÊÐéÄâ¼üÅÌ

#ifndef __CLASS_LAYOUT_VK26_STROKE__
#define __CLASS_LAYOUT_VK26_STROKE__
#include "CLayoutVK.h"

/*!
 * \Layout thisÖ¸ÕëºÍLayout »ùÀàÖ¸Õë×ª»»
 */
#undef CLASE_THIS_POITER
#define CLASE_THIS_POITER(CLASS_NAME, pltObj)   \
	CLASS_NAME * pltThis = (CLASS_NAME *)pltObj; \
	PCLayoutVK pltBase = &pltThis->m_ltParent; \
	PCLayoutAbstract pltRoot = &pltThis->m_ltParent.m_ltParent;

typedef struct _tag_CLayoutVK26Stroke {
	CLayoutVK            m_ltParent;
} CLayoutVK26Stroke, * PCLayoutVK26Stroke;

CLayoutAbstract * CLayoutVK26Stroke__Construct(PCLayoutVK26Stroke pltVK26, 
									     GBLPCVOID pltUIData,
									     GBLPCVOID pltIMData,
									     GBLPCVOID pltConfigData);
#endif //__CLASS_LAYOUT_VK26_STROKE__

#endif // ¹ú±ÊÐéÄâ¼üÅÌ

