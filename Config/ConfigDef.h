/***************************************************************************
* File Name    :  ConfigDef.h
* Corporation  :  Guobi Technology Ltd. GuangZhou China
* Copyright    :  Copyright Guobi Technology Ltd. And Subject to Confidentiality 
*                 Restrictions. This document is the confidential and proprietary 
*                 property of Guobi Technology Ltd.It is neither the document                  
*                 itself nor any part of it can be shared with any party  without 
*                 the written  permission. 
* Author       :  weizhiping
* Date         :  2010-06-08
* Purpose      :  ���ó�������ͷ�ļ�
* Version      :  V1.0
* Last Changed List
* Name         |   date   |  Discription
****************************************************************************/
#ifndef __CONFIG_DEF_H_
#define __CONFIG_DEF_H_
#include "gbtype.h"


typedef enum tagGBConfigRespondKeyType
{
	RESPOND_KEY_DOWN
	,RESPOND_KEY_UP
	,RESPOND_KEY_MAX
}GBConfigRespondKeyType;

typedef enum tagGBSwitchType
{
	SWITCH_OFF // 0 �ر�
	,SWITCH_ON // 1 ��
}GBConfigSwitchType;

/** \brief ������������*/
typedef struct tagEngineSetData
{
	GBINT		id;				//����ID
	GBINT		type;			//����ID����
	GBINT		value;			//����ֵ
	GBINT		flag;			//���ñ�־ = �Ƿ��ѡ
}GBEngineOptionData, *pGBEngineOptionData;


#endif //__CONFIG_DEF_H_