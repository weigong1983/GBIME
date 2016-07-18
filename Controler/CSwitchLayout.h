/***************************************************************************
* File Name    :  CSwitchLayout.h
* Corporation  :  Guobi Technology Ltd. GuangZhou China
* Copyright    :  Copyright Guobi Technology Ltd. And Subject to Confidentiality 
*                 Restrictions. This document is the confidential and proprietary 
*                 property of Guobi Technology Ltd.It is neither the document                  
*                 itself nor any part of it can be shared with any party  without 
*                 the written  permission. 
* Author       :  fanbo
* Date         :  2010-06-08
* Purpose      :  �л�Layoutͷ�ļ�
* Version      :  V1.0
* Last Changed List
* Name         |   date   |  Discription
****************************************************************************/
#ifndef __CSWITCHLAYOUT_H_
#define __CSWITCHLAYOUT_H_
#include "CLayoutAbstract.h"

#include "gbdef.h"
typedef struct __tag_CSwitchLayout 
{
	CLayoutAbstract		*pltBase;

}CSwitchLayout, *PCSwitchLayout;

/*!
 * \brief �л�Layout
 * \param tartLayoutID Ŀ��Layout ID
 * \return 
 * \note 
 * \example
 * \author fanbo
 * \date 2010-06-08
 */
GBINT32 CSwitchLayout_Switch(GBUINT16 tartLayoutID);

#endif //__CSWITCHLAYOUT_H_