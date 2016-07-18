/***************************************************************************
* File Name    :  CSwitchLayout.c
* Corporation  :  Guobi Technology Ltd. GuangZhou China
* Copyright    :  Copyright Guobi Technology Ltd. And Subject to Confidentiality 
*                 Restrictions. This document is the confidential and proprietary 
*                 property of Guobi Technology Ltd.It is neither the document                  
*                 itself nor any part of it can be shared with any party  without 
*                 the written  permission. 
* Author       :  fanbo
* Date         :  2010-06-08
* Purpose      :  �л�Layoutʵ���ļ�
* Version      :  V1.0
* Last Changed List
* Name         |   date   |  Discription
****************************************************************************/
#include "CSwitchLayout.h"
#include "GB_Global.h"

/*!
* \brief �л�Layout
* \param tartLayoutID Ŀ��Layout ID 
* \return 
* \note 
* \example
* \author fanbo
* \date 2010-06-08
*/
GBINT32 CSwitchLayout_Switch(GBUINT16 tartLayoutID)
{
	PCLayoutAbstract pltCurrentObject = Global_GetCurrentLayoutObject();
	PCLayoutAbstract pltTargetObject = NULL;
	GBU16 layoutWidth = 0;

	// ���LayoutID�Ƿ���Ч
	PT_Assert(IS_VALID_LAYOUT_ID(tartLayoutID));

	// Ŀ��Layout���ǵ�ǰLayout����(��ѡ��˵�����ʱ���ִ�״��)
// 	if (pltCurrentObject != NULL && tartLayoutID == GET_LAYOUT_ID(pltCurrentObject))
// 	{
// 		return GBIME_OK;
// 	}

    // ǰ�ô���
	switch (tartLayoutID)
	{
	case LayoutIdSwitchPreObj: // ������һ��Layout
		pltTargetObject = Global_GetPrevLayoutObject();
		break;
	case LayoutIdSwitchLatestObj: // �������Layout
		tartLayoutID = Global_GetLatestMasterLayoutID();
		pltTargetObject = Global_LayoutObjectIsLeaveByID(tartLayoutID);
		break;
	case LayoutIdSwitchLatestChnObj: // �����������Layout
		tartLayoutID = Global_GetLatestChnLayoutID();
		pltTargetObject = Global_LayoutObjectIsLeaveByID(tartLayoutID);
		break;
	case LayoutIdSwitchLatestAlpObj: // �����������Layout
		tartLayoutID = Global_GetLatestAlpLayoutID();
		pltTargetObject = Global_LayoutObjectIsLeaveByID(tartLayoutID);
		break;
	default:
		pltTargetObject = Global_LayoutObjectIsLeaveByID(tartLayoutID);
		break;
	}

	// Ų��m_pfcInitVariable��ͳһ���
// 	// �л����µĵ�Layout֮ǰ���õ�ǰLayout��MultiTap��ʱ��
// 	if (pltCurrentObject != NULL)
// 	{
// 		if (pltCurrentObject->m_pfcKBMultiTapTimerCallBack != NULL)
// 		{
// 			pltCurrentObject->m_pfcKBMultiTapTimerCallBack();
// 		}
// 		if (pltCurrentObject->m_pfcOnMultitapTimer != NULL)
// 		{
// 			pltCurrentObject->m_pfcOnMultitapTimer();
// 		}
// 	}	

	if (NULL == pltTargetObject)
	{
		// Ŀ���л�Layout���󲻴���,��Ҫ���´���
		pltTargetObject = CFactory_CreateInstance(Global_GetIMEData(), tartLayoutID);
		if (NULL != pltTargetObject)
		{
			Global_SaveLayoutObject(pltTargetObject);
		}
		else
		{
			return GBIME_FAILED; // ����Layoutʧ��
		}
	}
	else
	{
		Global_SetCurrentLayoutObject(pltTargetObject);
	}
	
	// �������ñ����Ź��ܼ�
	if (pltTargetObject->m_pfcSetInterpunctionKey != NULL)
	{
		pltTargetObject->m_pfcSetInterpunctionKey(pltTargetObject);
	}
	
#if defined(__GBIME_HANDWRITE__) // ��дLayout����MTKƽ̨��д����
	if (IS_HANDWRITE_LAYOUT(tartLayoutID))
	{
		PT_EnableHandwriting();
	}
	else
	{
		PT_DisableHandwriting();
	}
#endif

	// ���������LayoutID
	Global_SaveLatestLayoutID(tartLayoutID);
	
	// ����Layoutʵ�ʿ�ȶ�̬������ʺ�ѡ��Ŀ�ȡ���Ҫ����2������ť��ȡ�
	layoutWidth = pltTargetObject->m_pfcGetLayoutWidth(Global_GetCurrentLayoutObject(), tartLayoutID);
	GBEngine_SetPageToalWidthOption(Global_GetEnineInstance(), (GBU16)(layoutWidth - GBIME_ARROW_WIDTH * 2 - 4));
	//GBEngine_SetCandRowCount(Global_GetEnineInstance(), GBIME_INPUT_CAND_ROW_COUNT, GBIME_ASSOCIATE_CAND_ROW_COUNT); // ���ú�ѡ����
	return GBIME_OK;
}