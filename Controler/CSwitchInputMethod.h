/***************************************************************************
* File Name    :  CSwitchInputMethod.h
* Corporation  :  Guobi Technology Ltd. GuangZhou China
* Copyright    :  Copyright Guobi Technology Ltd. And Subject to Confidentiality 
*                 Restrictions. This document is the confidential and proprietary 
*                 property of Guobi Technology Ltd.It is neither the document                  
*                 itself nor any part of it can be shared with any party  without 
*                 the written  permission. 
* Author       :  fanbo
* Date         :  2010-06-08
* Purpose      :  切换输入法头文件
* Version      :  V1.0
* Last Changed List
* Name         |   date   |  Discription
****************************************************************************/
#ifndef __CSWITCHINPUTMETHOD_H_
#define __CSWITCHINPUTMETHOD_H_
#include "CGBEngine.h"
#include "CSwitchLayout.h"

#include "gbdef.h"

typedef struct __tag_CSWitchInputMethod 
{
	//GBEngine	*pGBEngine;
	CSwitchLayout	*pschLt;

}CSWitchInputMethod;

/*!
 * \brief 输入模式消息处理函数
 * \param pIMEEvent 
 * \return 
 */
GBIMEReturn CSWitchInputMethod_HandleMessage(PGBIMEEvent pIMEEvent);

#endif //__CSWITCHINPUTMETHOD_H_