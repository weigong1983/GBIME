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
* Purpose      :  框架内部工具库
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
 * \brief 填充一个RECT数据
 */
#define FILL_RECT(pRC, l, t, r, b)  \
		(pRC)->left = (l); (pRC)->top = (t); (pRC)->right = (r); (pRC)->bottom = (b);
/*!
 * \brief 平移一个RECT,(x, y)距离
 */
#define MOVE_RECT(pRC, x, y)  \
		(pRC)->left += (x); (pRC)->top += (y); (pRC)->right += (x); (pRC)->bottom += (y);

/*!
 * \brief 根据Layout转换类型值检索转换表，获取GBIMEEvent组成的事件集内容
 * \param convertType Layout转换类型值
 * \param pIMEEventGroup 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-22 9:48:11
 */
GBIMEReturn GBU_GetEventGroupByConvertType(GBIMEConvertTypeEnum convertType, PGBIMEEventGroup pIMEEventGroup);

#endif //__GBIME_UTILS_H_