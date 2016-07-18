/***************************************************************************
* File Name    :  CMessage.c
* Corporation  :  Guobi Technology Ltd. GuangZhou China
* Copyright    :  Copyright Guobi Technology Ltd. And Subject to Confidentiality 
*                 Restrictions. This document is the confidential and proprietary 
*                 property of Guobi Technology Ltd.It is neither the document                  
*                 itself nor any part of it can be shared with any party  without 
*                 the written  permission. 
* Author       :  fanbo
* Date         :  2010-06-08
* Purpose      :  ��Ϣ����ʵ���ļ�
* Version      :  V1.0
* Last Changed List
* Name         |   date   |  Discription
****************************************************************************/
#include "PlatformTools.h"
#include "PlatformAdapter.h"
#include "PlatformInputBox.h"
#include "CIMEInterface.h"
#include "gbft_def.h"
#include "GB_Global.h"
#include "CSwitchLayout.h"
#include "CSwitchInputMethod.h"
#include "GBIMEUtils.h"
#include "CHWEngine.h"
#include "CConfigMenu.h"

/*!
 * \brief ����ڲ�������ģ�����Ϣ������
 */
GBIMELOCAL GBIMEReturn Framework_HandleMessage(PGBIMEEvent pIMEEvent);
GBIMELOCAL GBIMEReturn Interface__DispatchEvent(GBIMEEventGroup * pIMEEvtGroup);
GBIMELOCAL GBIMEReturn Interface_PreHandleEvent(GBINT nPlatformEvent, GBINT wParam, GBINT lParam);
GBIMELOCAL GBIMEReturn Interface_PostHandleEvent(GBINT nPlatformEvent, GBINT wParam, GBINT lParam);

/*!
 * \brief ��ʼ����ܽӿڶ���
 * \param void 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-8-11 9:33:50
*/
GBAPI void Interface_InitIME(void)
{
	Global_InitIMEInterface();
}

/*!
 * \brief �жϿ�ܽӿڶ����Ƿ��Ѿ���ʼ��
 * \param void 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-8-11 9:33:50
*/
GBAPI GBBOOL Interface_IsIMEInited(void)
{
	return Global_IMEInterfaceIsInited();
}

/*!
 * \brief ��ȡIMEģ������״̬
 * \param void 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-8-11 9:33:50
*/
GBAPI GBBOOL Interface_IsIMEConnected(void)
{
	return Global_IMEIsConnected();
}

/*!
 * \brief ����IMEģ������״̬
 * \param GBBOOL 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-8-11 9:33:50
*/
GBAPI void Interface_SetIMEConnectedStatus(GBBOOL status)
{
	Global_SetIMEConnectedStatus(status);
}

/*!
 * \brief ��Ϣ������(�ɾ���Layoutʵ��������ƽ̨��Ϣת��ΪIME�����Ϣ)
 * \param nPlatformEvent 
 * \param wParam 
 * \param lParam 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-6-22 14:58:51
 */
GBAPI GBINT32 Handle_GBIMEInterface(GBINT nPlatformEvent, GBINT wParam, GBINT lParam)
{
	PCLayoutAbstract pltObj = Global_GetCurrentLayoutObject();
	GBIMEReturn ret = GBIME_IGNORE; 
	PEngineOutputInfo pEngineOutputInfo = CLayoutControl_GetEngineOutputInfo();

	if (pltObj == NULL)
	{
		return GBIME_LAYOUT_NOEXIST;
	}

	if (!Global_IMEInterfaceIsInited())
	{
		return GBIME_UNINITIALIZED;
	}

	// ������״̬��������
	if (!Interface_IsIMEConnected())
	{
		return GBIME_IGNORE;
	}

	// ����¼��飬��������δ������䴦������ʷ�¼�
	Global_ResetEventGroup();
	
	// ��ܲ���ϢԤ����δ�������������Layout������
	if ((ret = Interface_PreHandleEvent(nPlatformEvent, wParam, lParam)) == GBIME_IGNORE)
	{
		/*! Layout��ϢԤ����: ����Ϊһ���໥�����Ŀ���¼�����
		�����Ƿ���Ҫ������Ӧ�İ�����Ϣ���������洦��ı�ǲ���¼����ֵ(ʵ�ʴ�����Ϣʱ�õ�)
		 */
		if (pltObj->m_pfcPreHandleEvent)
		{
			CLayoutControl_UpdateOutputInfo();
			if ((ret = pltObj->m_pfcPreHandleEvent(pltObj, nPlatformEvent, wParam, lParam, pEngineOutputInfo)) != GBIME_OK)
			{
				return ret;
			}
		}
	}

	// ����¼�����Ϣ�ַ�(ѭ������Handle_GBIMEMessage����ÿһ���¼�)
	if ((ret = Interface__DispatchEvent(Global_GetIMEEventGroup())) != GBIME_OK)
	{
		return ret;
	}

	// Note: �����ת�����ò˵���Ҫ�������ù��ܼ��ص�(��ʱ���뷨��崦�ڷǼ���״̬)
	//if (pltObj->currentLayoutKeyValue != GBKEY_VK_SETTING)
	if (Global_GetLayoutWinActiveStatus())
	{
		// GBIME��ܴ�����Ϣ��ϲ��ҹ�����������ע��ƽ̨�Ĺ��ܰ���Ϊ����Լ��Ĵ�����
		if (ret == GBIME_OK && pEngineOutputInfo->bCandWindOpen)
		{
			GBInputBox_SetAllIMEFuncKey();			
		}
		else
		{
			GBInputBox_SetAllPlatformFuncKey();
		}

		// ����ƽ̨Softkey��ǩ
		GBInputBox_UpdateSoftKeyLabel();
	}

	// �����Ϣ����
	if ((ret = Interface_PostHandleEvent(nPlatformEvent, wParam, lParam)) != GBIME_IGNORE)
	{
		return ret;
	}

	return ret;
}


/*!
 * \brief IME �����Ϣ������(ע����ǡ��ʱ�����ð�ť��btpfcResponse��Ӧ����)
 * \param pIMEEvent 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-6-22 14:58:27
 */
GBAPI GBIMEReturn Handle_GBIMEMessage(PGBIMEEvent pIMEEvent)
{
	GBIMEReturn ret = GBIME_IGNORE;

	if (!Global_IMEInterfaceIsInited())
	{
		return GBIME_UNINITIALIZED;
	}

	switch (pIMEEvent->nType)
	{
	case GBIME_EVENT_TYPE_FRAMEWORK:
		if ((ret = Framework_HandleMessage(pIMEEvent)) != GBIME_IGNORE)
		{
			break; // ��ģ���Ѵ���
		}
	case GBIME_EVENT_TYPE_ENGINE: ///<���������¼�		
		if ((ret = GBEngine_HandleMessage(pIMEEvent)) != GBIME_IGNORE)
		{
			break; // ��ģ���Ѵ���
		}
#if defined(__GBIME_HANDWRITE__)
	case GBIME_EVENT_TYPE_HANDWRITE:		
		if ((ret = GBHW_HandleMessage(pIMEEvent)) != GBIME_IGNORE)
		{
			break; // ��ģ���Ѵ���
		}
#endif
	case GBIME_EVENT_TYPE_LAYOUT: ///<Layout�¼�		
		if ((ret = CLayoutControl_HandleMessage(pIMEEvent)) != GBIME_IGNORE)
		{
			break; // ��ģ���Ѵ���
		}
	case GBIME_EVENT_TYPE_INPUT_METHOD: // ���뷨ģʽ����ģ��
		if ((ret = CSWitchInputMethod_HandleMessage(pIMEEvent)) != GBIME_IGNORE)
		{
			break; // ��ģ���Ѵ���
		}
	case GBIME_EVENT_TYPE_CONFIG: ///<�����¼�	
		if ((ret = GBConfig_HandleMessage(pIMEEvent)) != GBIME_IGNORE)
		{
			break; // ��ģ���Ѵ���
		}
	default:
		break;
	}

	return ret;
}

/*!
 * \brief �ַ��¼����¼�
 * \param pIMEEvtGroup �¼���
 * \return 
 * \autor huanjin
 * \data 2010-8-26
 */
static GBIMEReturn Interface__DispatchEvent(GBIMEEventGroup * pIMEEvtGroup)
{
	GBIMEReturn ret;
	GBU8 i;
	GBU8 eventNum = 0;

	PT_Assert(pIMEEvtGroup != NULL);

	eventNum = pIMEEvtGroup->eventNum;
	
	if (0 == eventNum)
	{
		return GBIME_EVENT_GROUP_IS_NULL;
	}

	for (i=0; i < eventNum; i++)
	{
		ret = Handle_GBIMEMessage(&pIMEEvtGroup->eventGroup[i]);
		if (ret != GBIME_OK)
		{
			//return ret; // fail!!
			continue;
		}
	}

	return GBIME_OK;
}

/*!
 * \brief ��ȡ��������ʵ������ָ��
 * \param void 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-26 15:31:24
 */
PCGBEngine Interface_GetEnineInstance(void)
{
	return Global_GetEnineInstance();
}

/*!
 * \brief �������뷨Layout���ڵ���ʼ��ʾλ��(�����½��������Ļ�����꣬��Ϊ���Ͻǻ����ź�ѡ��߶ȱ仯)
 * \param pWinPos 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-21 11:28:23
 */
GBIMEReturn Interface_SetWinPos(GBIMEPoint *pWinPos)
{
	return Global_SetWinPos(pWinPos);
}

/*!
 * \brief ��ȡ���뷨Layout���ڵ����Ͻ�λ��(���½���ʼ��ʾλ�� + Layout������ʾ�߶�)
 * \param pWinPos 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-21 11:28:37
 */
GBIMEReturn Interface_GetWinPos(GBIMEPoint *pWinPos)
{
	GBIMEReturn ret;

	ret = Global_GetWinPos(pWinPos);
	if (ret == GBIME_OK)
	{
		pWinPos->y -= Interface_GetWinHeight(); // ƽ̨��Ҫ��ȡ���Ͻ�λ��
	}
	return ret;
}

/*!
 * \brief �������뷨���ڵ���ʾ�߶�
 * \param newHeight 
 * \note �߶ȷ����仯ʱ��Ҫ֪ͨMTKƽ̨�༭������ˢ�£�����ᵼ�������������뷨����ˢ�²�����
 * \return 
 */
void Interface_SetWinHeight(GBINT newHeight)
{
	Global_SetLayoutWinWidth(newHeight);
}

/*!
 * \brief ��ȡ���뷨���ڵ���ʾ�߶�
 * \param void 
 * \note �߶ȷ����仯ʱ��Ҫ֪ͨMTKƽ̨�༭������ˢ�£�����ᵼ�������������뷨����ˢ�²�����
 * \return 
 */
GBINT Interface_GetWinHeight(void)
{
	return Global_GetLayoutWinHeight();
}

/*!
 * \brief ��ȡ���뷨���ڵ���ʾ���
 * \param newWidth 
 * \note ��ȷ����仯ʱ��Ҫ֪ͨMTKƽ̨�༭������ˢ�£�����ᵼ�������������뷨����ˢ�²�����
 * \return 
 */
void Interface_SetWinWidth(GBINT newWidth)
{
	Global_SetLayoutWinWidth(newWidth);
}

/*!
 * \brief ��ȡ���뷨���ڵ���ʾ���
 * \param void 
 * \note ��ȷ����仯ʱ��Ҫ֪ͨMTKƽ̨�༭������ˢ�£�����ᵼ�������������뷨����ˢ�²�����
 * \return 
 */
GBINT Interface_GetWinWidth(void)
{
	return Global_GetLayoutWinWidth();
}

/*!
 * \brief ��ȡLayout���ڼ���״̬
 * \param void 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-8-11 11:45:39
*/
GBBOOL Interface_GetLayoutWinActiveStatus(void)
{
	return Global_GetLayoutWinActiveStatus();
}

/*!
 * \brief ����Layout���ڼ���״̬
 * \param status 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-8-11 11:45:39
*/
void Interface_SetLayoutWinActiveStatus(GBBOOL status)
{
	Global_SetLayoutWinActiveStatus(status);
}

/*!
 * \brief ����ڲ���Ϣ������
 * \param pIMEEvent 
 * \return 
 */
GBIMELOCAL GBIMEReturn Framework_HandleMessage(PGBIMEEvent pIMEEvent)
{
	GBIMEReturn ret = GBIME_OK;

	if (pIMEEvent->nType != GBIME_EVENT_TYPE_FRAMEWORK)
	{
		return GBIME_IGNORE; // �ǿ���¼�������
	}

	switch (pIMEEvent->nEvent)
	{
	case GBIME_EVENT_FRAMEWORK_INIT:
		//GBInputBox_Init(); // �Ƴٵ�GBIME_Connect
		break;
	case GBIME_EVENT_FRAMEWORK_EXIT:
		Global_ReleaseIMEInterface();
		break;
	default:
		PT_Assert(0); // �Ƿ��¼�
	}

	return ret;
}

/*!
 * \brief ��ܲ����ϢԤ������(ʵ��һЩLayout�������֮�ϵ�ȫ���Դ���������Layout������Layout֮����л�)
 * \param nPlatformEvent 
 * \param wParam 
 * \param lParam 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-10-29 14:28:57
 */
GBIMELOCAL GBIMEReturn Interface_PreHandleEvent(GBINT nPlatformEvent, GBINT wParam, GBINT lParam)
{
	PCLayoutAbstract pltAbstractObj = Global_GetCurrentLayoutObject();
	KEYBOARD_TYPE keyboardType = CLayoutInfo_GetKeyboardTypeByLayoutID(GET_LAYOUT_ID(pltAbstractObj));
	GBIMEReturn ret = GBIME_IGNORE;
	GBUINT16 targetLayoutID;
	GBIMEPoint point;
	GBRECT layoutRect = {0};
	GBIMEPoint layoutWinStartPos;
	
	point.x = wParam;
	point.y = lParam;

	// ��ȡLayout��������
	Global_GetWinPos(&layoutWinStartPos); // note: ���½�λ��
	FILL_RECT(&layoutRect, layoutWinStartPos.x,	(GBINT16)(layoutWinStartPos.y - Interface_GetWinHeight()), Interface_GetWinWidth(), layoutWinStartPos.y);

	switch(nPlatformEvent)
	{
	case GBPEVT_PEN_UP: // ��������л����������[������Layout�����л�VK����ȻӰ��ƽ̨��궨λ����]
	#if defined(__GBIME_VK__) // �����������
		if(point.y < layoutRect.top) // ����Layout�����ѡ������������Ҫ�л�������Layout
		{
			switch(keyboardType)
			{
			case KEYBOARD_KB_NUMPAD: // �л�������ż���ӦLayout
				GBConfig_SetKeyboardType(Global_GetConfigInstance(), KEYBOARD_VK_NUMPAD);
				if ((ret = GBPA_GetLayoutIDByPlatformInputMode(GBInputBox_GetCurrentInputMode(), &targetLayoutID)) != GBIME_OK)
				{
					return GBIME_LAYOUT_NOEXIST; // �Ҳ�����Ӧ���������LayoutID
				}			
				Global_FillEventGroup(GBIME_CT_KB_2_VK_SWITCH, targetLayoutID, lParam);
				return GBIME_OK;
			case KEYBOARD_KB_QWERTY: // �л�������26����ӦLayout
				GBConfig_SetKeyboardType(Global_GetConfigInstance(), KEYBOARD_VK_QWERTY);
				if ((ret = GBPA_GetLayoutIDByPlatformInputMode(GBInputBox_GetCurrentInputMode(), &targetLayoutID)) != GBIME_OK)
				{
					return GBIME_LAYOUT_NOEXIST; // �Ҳ�����Ӧ���������LayoutID
				}
				Global_FillEventGroup(GBIME_CT_KB_2_VK_SWITCH, targetLayoutID, lParam);
				return GBIME_OK;
			default:
				break;
			}
		}
	#endif // �����������
		break;
	case GBPEVT_KEY_DOWN: // ������̴���������
	#if defined(__GBIME_KB__) // �����������
		switch(keyboardType)
		{
		case KEYBOARD_VK_NUMPAD:
		case KEYBOARD_VK_QWERTY:
			if (IS_VK_SHOULD_HANDLE_PHYSIC_FUNC_KEY(wParam))
			{
				//return GBIME_OK; // ����Ҫͬ��Layout״̬����ʱ�����ԣ�������
				return GBIME_IGNORE; // �����������Ҫ������Ӧ����������������m_pfcPhysicFuncKeyHandler����
			}
			else // ��������л����������
			{
				GBConfig_SetKeyboardType(Global_GetConfigInstance(), KEYBOARD_KB_NUMPAD);
				if ((ret = GBPA_GetLayoutIDByPlatformInputMode(GBInputBox_GetCurrentInputMode(), &targetLayoutID)) != GBIME_OK)
				{
					return GBIME_LAYOUT_NOEXIST; // �Ҳ�����Ӧ���������LayoutID
				}
				Global_FillEventGroup(GBIME_CT_VK_2_KB_SWITCH, targetLayoutID, wParam);
				Global_SetVk2KbSwitchFlag(GBTrue);
				Global_SetVk2KbSwitchEngineKeyValue(wParam);
			}
			return GBIME_OK;
		default:
			break;
		}
	#endif // �����������
		break;
	default:
		break;
	}

	return ret;
}

/*!
 * \brief �����Ϣ����(��Щ�����Layout�ڲ���Ҫ�������´����µ�һ���¼�)
 * \param nPlatformEvent 
 * \param wParam 
 * \param lParam 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-12-9 8:41:24
*/
GBIMELOCAL GBIMEReturn Interface_PostHandleEvent(GBINT nPlatformEvent, GBINT wParam, GBINT lParam)
{
	GBIMEReturn ret = GBIME_IGNORE; 

	// ����¼�����Ϣ�ַ�(ѭ������Handle_GBIMEMessage����ÿһ���¼�)
	if ((ret = Interface__DispatchEvent(Global_GetIMEPostEventGroup())) != GBIME_OK)
	{
		return ret;
	}

	return ret;
}
