/***************************************************************************
* File Name    :  CSwitchInputMethod.c
* Corporation  :  Guobi Technology Ltd. GuangZhou China
* Copyright    :  Copyright Guobi Technology Ltd. And Subject to Confidentiality 
*                 Restrictions. This document is the confidential and proprietary 
*                 property of Guobi Technology Ltd.It is neither the document                  
*                 itself nor any part of it can be shared with any party  without 
*                 the written  permission. 
* Author       :  fanbo
* Date         :  2010-06-08
* Purpose      :  �л����뷨ʵ���ļ�
* Version      :  V1.0
* Last Changed List
* Name         |   date   |  Discription
****************************************************************************/
#include "CSwitchInputMethod.h"
#include "PlatformAdapter.h"
#include "PlatformInputBox.h"
#include "GB_Global.h"

/*!
* \brief �л���ָ�����뷨ģʽ
* \param tartLayoutID 
* \return 
* \note 
* \example
* \author weizhiping
* \date 2010-08-05
*/
GBIMELOCAL GBIMEReturn CSWitchInputMethod_Switch(GBUINT16 tartLayoutID)
{
	GBIMEReturn ret;
	GBINT platformInputMode;
	GBUINT16 nGBInputMode;
	GBUINT16 nLangID;
	GBUINT16 nSubLangID;
	PCLayoutControl pLtCtl = Global_GetLayoutCtl();
	PCLayoutAbstract pltCurrentObject = Global_GetCurrentLayoutObject();
	PCLayoutAbstract pltPreObject = Global_GetPrevLayoutObject();

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
		if (pltPreObject == NULL)
		{
			return GBIME_LAYOUT_NOEXIST;
		}
		tartLayoutID = CLayoutInfo_GetLayoutID(&pltPreObject->m_ltData);
		break;
	case LayoutIdSwitchLatestObj: // �������Layout
		tartLayoutID = Global_GetLatestMasterLayoutID();
		break;
	case LayoutIdSwitchLatestChnObj: // �����������Layout
		tartLayoutID = Global_GetLatestChnLayoutID();
		break;
	case LayoutIdSwitchLatestAlpObj: // �����������Layout
		tartLayoutID = Global_GetLatestAlpLayoutID();
		break;
	default:
		break;
	}

	// �л�������Layout����Ҫ��������ģʽ[�����Ϳ��Ը�����һ�ε��������;���Ĭ�ϵķ�����ȫ�ǻ��ǰ��]
	if (LayoutVKSymbol == CLayoutInfo_GetLayoutTypeByID(Global_GetSkinData(), tartLayoutID))
	{
		return GBIME_OK;
	}

	// ����LayoutID��ȡƽ̨����ģʽ
	if ((ret = GBPA_GetPlatformInputModeByLayoutID(tartLayoutID, &platformInputMode)) != GBIME_OK)
	{
		return ret;
	}

	// ����LayoutID��ȡ������������ģʽ��Ϣ
	if ((ret = CLayoutInfo_GetGBInputModeInfoByLayoutID(Global_GetSkinData(), 
		 tartLayoutID, &nGBInputMode, &nLangID, &nSubLangID)) != GBIME_OK)
	{
		return ret;
	}

	// ������������ģʽ�л�
	if ((ret = GBEngine_SwitchInputMode(Global_GetEnineInstance(), nGBInputMode, nLangID, nSubLangID)) != GBOK)
	{
		return ret;
	}

	// Abc Multitap����ģʽ
	if (LayoutIdSP9Jabc == tartLayoutID	|| LayoutIdSP26Jabc == tartLayoutID
		|| LayoutIdSP9JABC == tartLayoutID	|| LayoutIdSP26JABC == tartLayoutID
// 		|| LayoutIdWL9Jabc == tartLayoutID || LayoutIdWL26Jabc == tartLayoutID
// 		|| LayoutIdWL9JABC == tartLayoutID || LayoutIdWL26JABC == tartLayoutID
		)
	{
		pLtCtl->engineOutputInfo.isMultiTapInputMode = GBTrue;
		GBEngine_SetCandType(Global_GetEnineInstance(), GBCT_MultiTap_Lower_Cap);
	}
	else if(LayoutIdWL9Jabc == tartLayoutID || LayoutIdWL26Jabc == tartLayoutID)
	{
		pLtCtl->engineOutputInfo.isMultiTapInputMode = GBTrue;
		GBEngine_AttachTimerFunction(Global_GetEnineInstance()
			, GBEngine_GBRequestTimer_t, GBEngine_GBRequestKillTimer_t
			, (GBLPVOID)pltCurrentObject->m_pfcKBMultiTapTimerCallBack);
		GBEngine_SetCandType(Global_GetEnineInstance(), GBCT_MultiTap_Lower);
	}
	else if(LayoutIdWL9JABC == tartLayoutID || LayoutIdWL26JABC == tartLayoutID)
	{
		pLtCtl->engineOutputInfo.isMultiTapInputMode = GBTrue;
		GBEngine_AttachTimerFunction(Global_GetEnineInstance()
			, GBEngine_GBRequestTimer_t, GBEngine_GBRequestKillTimer_t
			, (GBLPVOID)pltCurrentObject->m_pfcKBMultiTapTimerCallBack);
		GBEngine_SetCandType(Global_GetEnineInstance(), GBCT_MultiTap_Cap);			
	}
	else
	{
		pLtCtl->engineOutputInfo.isMultiTapInputMode = GBFalse;
		GBEngine_SetCandType(Global_GetEnineInstance(), GBCT_Normal);
	}

	return GBIME_OK;
}

/*!
* \brief ֪ͨĿ��ƽ̨�л���ָ�����뷨ģʽ
* \param tartLayoutID 
* \return 
* \note 
* \example
* \author weizhiping
* \date 2010-08-05
*/
GBIMELOCAL GBIMEReturn CSWitchInputMethod_Notify(GBUINT16 tartLayoutID)
{
	GBIMEReturn ret = GBIME_IGNORE;
	GBINT platformInputMode;
	PCLayoutAbstract pltPreObject = Global_GetPrevLayoutObject();
	KEYBOARD_TYPE keyboardType;

	// ���LayoutID�Ƿ���Ч
	PT_Assert(IS_VALID_LAYOUT_ID(tartLayoutID));

	// ǰ�ô���
	switch (tartLayoutID)
	{
	case LayoutIdSwitchPreObj: // ������һ��Layout
		if (pltPreObject == NULL)
		{
			return GBIME_LAYOUT_NOEXIST;
		}
		tartLayoutID = CLayoutInfo_GetLayoutID(&pltPreObject->m_ltData);
		break;
	case LayoutIdSwitchLatestObj: // �������Layout
		tartLayoutID = Global_GetLatestMasterLayoutID();
		break;
	case LayoutIdSwitchLatestChnObj: // �����������Layout
		tartLayoutID = Global_GetLatestChnLayoutID();
		break;
	case LayoutIdSwitchLatestAlpObj: // �����������Layout
		tartLayoutID = Global_GetLatestAlpLayoutID();
		break;
	default:
		break;
	}

	// ���ShiftCap״̬��Ч����Ҫת���ɶ�Ӧ�Ĵ�дLayout
	if (Global_GetShiftCapStatus())
	{
		tartLayoutID = CLayoutInfo_GetUpperLayoutID(tartLayoutID);
	}

	// �����ȸ���Ŀ��LayoutID�жϼ�������
	keyboardType = CLayoutInfo_GetKeyboardTypeByLayoutID(tartLayoutID);
	if (keyboardType != GBConfig_GetKeyboardType(Global_GetConfigInstance())) // �������������л�
	{
		GBConfig_SetKeyboardType(Global_GetConfigInstance(), keyboardType);
	}

	// ����LayoutID��ȡƽ̨����ģʽ
	if ((ret = GBPA_GetPlatformInputModeByLayoutID(tartLayoutID, &platformInputMode)) != GBIME_OK)
	{
		return ret;
	}

	// ֪ͨĿ��ƽ̨�л���ָ�����뷨ģʽ
	GBInputBox_ChangeInputMode(platformInputMode, 0);
	return ret;
}

/*!
 * \brief ����ģʽ��Ϣ������
 * \param pIMEEvent 
 * \return 
 */
GBIMEReturn CSWitchInputMethod_HandleMessage(PGBIMEEvent pIMEEvent)
{
	GBIMEReturn ret = GBIME_OK;

	if (pIMEEvent->nType != GBIME_EVENT_TYPE_INPUT_METHOD)
	{
		return GBIME_IGNORE; // ������ģʽ�¼�������
	}

	switch (pIMEEvent->nEvent)
	{
	case GBIME_EVENT_INPUT_METHOD_SWITCH:
		ret = CSWitchInputMethod_Switch((GBUINT16)pIMEEvent->wParam); ///<�л����뷨�¼�[�������ΪLayoutID]	
		break;
	case GBIME_EVENT_INPUT_METHOD_SWITCH_NOTIFY:
		ret = CSWitchInputMethod_Notify((GBUINT16)pIMEEvent->wParam); ///<�л����뷨�¼�[�������ΪLayoutID]	
		break;
	default:
		PT_Assert(0); // �Ƿ��¼�
	}

	return ret;
}