/***************************************************************************
* File Name    :  GBIMEUtils.h
* Corporation  :  Guobi Technology Ltd. GuangZhou China
* Copyright    :  Copyright Guobi Technology Ltd. And Subject to Confidentiality 
*                 Restrictions. This document is the confidential and proprietary 
*                 property of Guobi Technology Ltd.It is neither the document                  
*                 itself nor any part of it can be shared with any party  without 
*                 the written  permission. 
* Author       :  weizhiping
* Date         :  2010-07-16
* Purpose      :  ����ڲ����߿�
* Version      :  V1.0
* Last Changed List
* Name         |   date   |  Discription
****************************************************************************/
#ifndef __GBIME_UTILS_H_
#define __GBIME_UTILS_H_

#include "gbtype.h"
#include "gbft_def.h"
#include "GBIMEDef.h"

/*!
 * \brief ���һ��RECT����
 */
#define FILL_RECT(pRC, l, t, r, b)  \
		(pRC)->left = (l); (pRC)->top = (t); (pRC)->right = (r); (pRC)->bottom = (b);
/*!
 * \brief ƽ��һ��RECT,(x, y)����
 */
#define MOVE_RECT(pRC, x, y)  \
		(pRC)->left += (x); (pRC)->top += (y); (pRC)->right += (x); (pRC)->bottom += (y);

/*!
 * \brief ����Layoutת������ֵ����ת������ȡGBIMEEvent��ɵ��¼�������
 * \param convertType Layoutת������ֵ
 * \param pIMEEventGroup 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-22 9:48:11
 */
GBIMEReturn GBU_GetEventGroupByConvertType(GBIMEConvertTypeEnum convertType, PGBIMEEventGroup pIMEEventGroup);

#endif //__GBIME_UTILS_H_