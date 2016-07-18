/***************************************************************************
* File Name    :  CLayoutConfigAbstract.h
* Corporation  :  Guobi Technology Ltd. GuangZhou China
* Copyright    :  Copyright Guobi Technology Ltd. And Subject to Confidentiality 
*                 Restrictions. This document is the confidential and proprietary 
*                 property of Guobi Technology Ltd.It is neither the document                  
*                 itself nor any part of it can be shared with any party  without 
*                 the written  permission. 
* Author       :  weizhiping
* Date         :  2010-07-27
* Purpose      :  抽象Layout配置类
* Version      :  V1.0
* Last Changed List
* Name         |   date   |  Discription
****************************************************************************/

#ifndef __CLASS_LAYOUT_CONFIG_ABSTRACT_
#define __CLASS_LAYOUT_CONFIG_ABSTRACT_
#include "PlatformTools.h"
#include "gbtype.h"
#include "gbdef.h"
#include "gbft_def.h"
#include "GBIMEDef.h"
#include "CLayoutInfo.h"
#include "ConfigDef.h"

typedef struct _tagCLayoutConfigAbstract // layout配置抽象类
{
	GBUINT16 layoutType;
	//GBConfigRespondKeyType	m_respondKeyType;
}CLayoutConfigAbstract;

#endif //__CLASS_LAYOUT_CONFIG_ABSTRACT_


