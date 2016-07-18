/***************************************************************************
* File Name    :  CConfigMenuDef.h
* Corporation  :  Guobi Technology Ltd. GuangZhou China
* Copyright    :  Copyright Guobi Technology Ltd. And Subject to Confidentiality 
*                 Restrictions. This document is the confidential and proprietary 
*                 property of Guobi Technology Ltd.It is neither the document                  
*                 itself nor any part of it can be shared with any party  without 
*                 the written  permission. 
* Author       :  Zhuhaian
* Date         :  2010-08-05
* Purpose      :  配置菜单头文件
* Version      :  V1.0
* Last Changed List
* Name         |   date   |  Discription
****************************************************************************/
#ifndef __CONFIGUREMENU_H_
#define __CONFIGUREMENU_H_

#include "PlatformAdapter.h"
#include "GBIMEResDef.h"

#if __MTK__ > 0
#include "MMIDataType.h"
#include "GlobalConstants.h"
#include "CustDataProts.h"
#include "GlobalDefs.h"
#endif //__MTK__



typedef struct _tag_CProductMenuData
{
	GBUINT16 id;
	GBUINT16 strID;
}CProductMenuData;


/*!
 * \brief 进入设置键屏幕
 * \param  void
 * \return  void
 * \note 
 * \example
 * \author Zhuhaian
 * \date
 */
void EntryGbSetUpScreen(void);

#endif   //__CONFIGUREMENU_H_