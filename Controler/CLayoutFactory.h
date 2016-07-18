/***************************************************************************
* File Name    :  CFactory.h
* Corporation  :  Guobi Technology Ltd. GuangZhou China
* Copyright    :  Copyright Guobi Technology Ltd. And Subject to Confidentiality 
*                 Restrictions. This document is the confidential and proprietary 
*                 property of Guobi Technology Ltd.It is neither the document                  
*                 itself nor any part of it can be shared with any party  without 
*                 the written  permission. 
* Author       :  fanbo
* Date         :  2010-06-08
* Purpose      :  工厂头文件
* Version      :  V1.0
* Last Changed List
* Name         |   date   |  Discription
****************************************************************************/
#ifndef __CFACTORY_H_
#define __CFACTORY_H_
#include "IMECommonData.h"
#include "CSwitchLayout.h"
#include "CLayoutAbstract.h"

typedef struct __tag_CFactory 
{
	CSwitchLayout	*pschLt;
	CLayoutAbstract *pltBase;
}CFactory;

/*!
 * \brief 创建Layout实例
 * \param id 
 * \return 
 * \note 
 * \example
 * \author fanbo
 * \date 2010-06-08
 */
PCLayoutAbstract CFactory_CreateInstance(PIMECommonData pIME, GBUINT16 ltID);
#endif //__CFACTORY_H_