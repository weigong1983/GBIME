/***************************************************************************
* File Name    :  GBIMEUtils.c
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
#include "GBIMEUtils.h"
#include "PlatformTools.h"

/*!
 * \brief ���ڼ�m_pfcPreHandleEvent������������¼�ת��ֵ����ת������ȡ��ӦGBIMEEvent��ɵ��¼���
 */
static const GBIMEEventConvertMap gbIMEEventConvertMap[] = 
{
	// Layout���
	// Layout��ť���£� ����ˢ�£� Layout����->ˢ��Layout
	{GBIME_CT_KEY_DOWN_REFRESH,	{{GBIME_EVENT_TYPE_LAYOUT, GBIME_EVENT_LAYOUT_PEN_DOWN}, {GBIME_EVENT_TYPE_LAYOUT, GBIME_EVENT_LAYOUT_REFRESH}}, 2},
	// Layout��ť���� ����ˢ�£� Layout����->ˢ��Layout
	{GBIME_CT_KEY_UP_REFRESH,	{{GBIME_EVENT_TYPE_LAYOUT, GBIME_EVENT_LAYOUT_PEN_UP}, {GBIME_EVENT_TYPE_LAYOUT, GBIME_EVENT_LAYOUT_REFRESH}}, 2},
	// Layout��ť������Ӧ�������水���¼��� ���水��->Layout����->ˢ��Layout
	{GBIME_CT_NORMAL_KEY_DOWN,	{{GBIME_EVENT_TYPE_ENGINE, GBIME_EVENT_ENGINE_KEY}, {GBIME_EVENT_TYPE_LAYOUT, GBIME_EVENT_LAYOUT_PEN_DOWN}, {GBIME_EVENT_TYPE_LAYOUT, GBIME_EVENT_LAYOUT_REFRESH}}, 3},
	// Layout��ť������Ӧ�������水���¼��� ���水��->Layout����->ˢ��Layout
	{GBIME_CT_NORMAL_KEY_UP,	{{GBIME_EVENT_TYPE_ENGINE, GBIME_EVENT_ENGINE_KEY}, {GBIME_EVENT_TYPE_LAYOUT, GBIME_EVENT_LAYOUT_PEN_UP}, {GBIME_EVENT_TYPE_LAYOUT, GBIME_EVENT_LAYOUT_REFRESH}}, 3},
	// Layout ������滬���� Layout Move->ˢ��Layout
	{GBIME_CT_MOVE,			{{GBIME_EVENT_TYPE_LAYOUT, GBIME_EVENT_LAYOUT_PEN_MOVE}, {GBIME_EVENT_TYPE_LAYOUT, GBIME_EVENT_LAYOUT_REFRESH}}, 2},
	// Layout�л��� ����״̬����->���뷨�л�->Layout�л�->ˢ��Layout
	{GBIME_CT_SWITCH,		
	 { {GBIME_EVENT_TYPE_ENGINE, GBIME_EVENT_ENGINE_RESET}
	  ,{GBIME_EVENT_TYPE_LAYOUT, GBIME_EVENT_LAYOUT_EXIT} // �˳���ǰLayout 
	  ,{GBIME_EVENT_TYPE_INPUT_METHOD, GBIME_EVENT_INPUT_METHOD_SWITCH_NOTIFY} /*֪ͨƽ̨����ƽ̨�л�����*/
	  ,{GBIME_EVENT_TYPE_LAYOUT, GBIME_EVENT_LAYOUT_ENTER} // ����Ŀ��Layout 
	  ,{GBIME_EVENT_TYPE_LAYOUT, GBIME_EVENT_LAYOUT_REFRESH}}
	, 5},
	// Layout ��ʾ�����
	{GBIME_CT_SHOW,		{{GBIME_EVENT_TYPE_LAYOUT, GBIME_EVENT_LAYOUT_SHOW}, {GBIME_EVENT_TYPE_LAYOUT, GBIME_EVENT_LAYOUT_REFRESH}}, 2},
	// Layout ���أ�����״̬����->����->ˢ��
	{GBIME_CT_HIDE,		{{GBIME_EVENT_TYPE_ENGINE, GBIME_EVENT_ENGINE_RESET}, {GBIME_EVENT_TYPE_LAYOUT, GBIME_EVENT_LAYOUT_HIDE}, {GBIME_EVENT_TYPE_LAYOUT, GBIME_EVENT_LAYOUT_REFRESH}}, 3},

	// ��д���
	// ������дʶ������ĺ�ѡ: ������д��ѡ->ˢ��Layout
	{GBIME_CT_HANDWRITE_DRAW_CANDIDATE, {{GBIME_EVENT_TYPE_HANDWRITE, GBIME_EVENT_HANDWRITE_DRAW_CANDIDATE}, {GBIME_EVENT_TYPE_LAYOUT, GBIME_EVENT_LAYOUT_REFRESH}}, 2},
	// ���������ַ���: �����ȡ�����->���������->ˢ��Layout
	{GBIME_CT_HANDWRITE_DRAW_ASSOCWORD, {{GBIME_EVENT_TYPE_ENGINE, GBIME_EVENT_ENGINE_GET_ASSOCWORD}, {GBIME_EVENT_TYPE_HANDWRITE, GBIME_EVENT_HANDWRITE_DRAW_ASSOCWORD}, {GBIME_EVENT_TYPE_LAYOUT, GBIME_EVENT_LAYOUT_REFRESH}}, 3},
	// ������дʶ��Χ: ������дʶ��Χ->ˢ��Layout
	{GBIME_CT_HANDWRITE_SET_RECOGNIZE_RANGE, {{GBIME_EVENT_TYPE_HANDWRITE, GBIME_EVENT_HANDWRITE_SET_RECOGNIZE_RANGE}, {GBIME_EVENT_TYPE_LAYOUT, GBIME_EVENT_LAYOUT_REFRESH}}, 2},

	// �������
	// �������ò˵�
	{GBIME_CT_CONFIG_MENU, {{GBIME_EVENT_TYPE_CONFIG, GBIME_EVENT_CONFIG_MENU}}, 1},
	
	// ���������
	// ����������Ӧ�������水���¼��� ���水��->����������->ˢ��Layout
	{GBIME_CT_KEY_DOWN,	{{GBIME_EVENT_TYPE_ENGINE, GBIME_EVENT_ENGINE_KEY}, {GBIME_EVENT_TYPE_LAYOUT, GBIME_EVENT_KEY_DOWN}, {GBIME_EVENT_TYPE_LAYOUT, GBIME_EVENT_LAYOUT_REFRESH}}, 3},
	// ����������Ӧ�������水���¼��� ���水��->����������->ˢ��Layout
	{GBIME_CT_KEY_UP,	{{GBIME_EVENT_TYPE_ENGINE, GBIME_EVENT_ENGINE_KEY}, {GBIME_EVENT_TYPE_LAYOUT, GBIME_EVENT_KEY_UP}, {GBIME_EVENT_TYPE_LAYOUT, GBIME_EVENT_LAYOUT_REFRESH}}, 3},
	// ����������Ӧ�������水���¼��� ���水��->����������->ˢ��Layout
	{GBIME_CT_KEY_LONG_PRESS,	{{GBIME_EVENT_TYPE_ENGINE, GBIME_EVENT_ENGINE_KEY}, {GBIME_EVENT_TYPE_LAYOUT, GBIME_EVENT_KEY_UP}, {GBIME_EVENT_TYPE_LAYOUT, GBIME_EVENT_LAYOUT_REFRESH}}, 3},
	
	// ������� -> �������
	{GBIME_CT_VK_2_KB_SWITCH, 
	{ {GBIME_EVENT_TYPE_ENGINE, GBIME_EVENT_ENGINE_RESET}
	,{GBIME_EVENT_TYPE_LAYOUT, GBIME_EVENT_LAYOUT_EXIT} // �˳���ǰLayout 
  	, {GBIME_EVENT_TYPE_INPUT_METHOD, GBIME_EVENT_INPUT_METHOD_SWITCH_NOTIFY}/*Layout�л������ͬ��ƽ̨����ģʽˢ��VK*/
	, {GBIME_EVENT_TYPE_LAYOUT, GBIME_EVENT_LAYOUT_SHOW} // ��������ʾ����
	, {GBIME_EVENT_TYPE_LAYOUT, GBIME_EVENT_LAYOUT_ENTER} // ����Ŀ��Layout 
 	, {GBIME_EVENT_TYPE_ENGINE, GBIME_EVENT_ENGINE_KEY}	
 	, {GBIME_EVENT_TYPE_LAYOUT, GBIME_EVENT_KEY_DOWN}
 	, {GBIME_EVENT_TYPE_LAYOUT, GBIME_EVENT_LAYOUT_REFRESH}	
	}
	, 8},

	// ������� -> �������
	{GBIME_CT_KB_2_VK_SWITCH, 
	{ {GBIME_EVENT_TYPE_ENGINE, GBIME_EVENT_ENGINE_RESET}
	,{GBIME_EVENT_TYPE_LAYOUT, GBIME_EVENT_LAYOUT_EXIT} // �˳���ǰLayout 
	, {GBIME_EVENT_TYPE_INPUT_METHOD, GBIME_EVENT_INPUT_METHOD_SWITCH_NOTIFY}/*Layout�л������ͬ��ƽ̨����ģʽˢ��VK*/
	, {GBIME_EVENT_TYPE_LAYOUT, GBIME_EVENT_LAYOUT_SHOW} // ��������ʾ����
	, {GBIME_EVENT_TYPE_LAYOUT, GBIME_EVENT_LAYOUT_ENTER} // ����Ŀ��Layout 
	, {GBIME_EVENT_TYPE_LAYOUT, GBIME_EVENT_LAYOUT_REFRESH}	
	}
	, 6}
};

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
GBIMEReturn GBU_GetEventGroupByConvertType(GBIMEConvertTypeEnum convertType, PGBIMEEventGroup pIMEEventGroup)
{
	GBU8 i, j;

	if (pIMEEventGroup == NULL)
	{
		return GBIME_INVALID_PARAM;
	}

	for (i = 0; i < PT_ARRAY_SIZE(gbIMEEventConvertMap); i++)
	{
		if (gbIMEEventConvertMap[i].convertType == convertType)
		{
			for (j = 0; j < gbIMEEventConvertMap[i].eventNum; j++)
			{
				pIMEEventGroup->eventGroup[j].nType = gbIMEEventConvertMap[i].eventHeader[j].nType;
				pIMEEventGroup->eventGroup[j].nEvent = gbIMEEventConvertMap[i].eventHeader[j].nEvent;
			}
			pIMEEventGroup->eventNum = gbIMEEventConvertMap[i].eventNum;
			return GBIME_OK;
		}
	}

	return GBIME_EVENT_GROUP_NOT_EXIST;
}