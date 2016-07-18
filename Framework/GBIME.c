/***************************************************************************
* File Name    :  GBIME.c
* Corporation  :  Guobi Technology Ltd. GuangZhou China
* Copyright    :  Copyright Guobi Technology Ltd. And Subject to Confidentiality 
*                 Restrictions. This document is the confidential and proprietary 
*                 property of Guobi Technology Ltd.It is neither the document
*                 itself nor any part of it can be shared with any party  without 
*                 the written  permission. 
* Author       :  fanbo
* Date         :  2010-06-08
* Purpose      :  ���뷨�������㣨�����ܣ��ṩAPI��ƽ̨���ã�ƽ̨����֪�����ʵ��ϸ�ڣ�
* Version      :  V1.0
* Last Changed List
* Name         |   date   |  Discription
****************************************************************************/
#include "GBIME.h"
#include "gbft_def.h"
#include "CIMEInterface.h"
#include "PlatformTools.h"
#include "PlatformAdapter.h"
#include "CIMEInterface.h"
#include "CLayoutInfo.h"
#include "GB_Global.h"
#include "CGBEngine.h"

/*!
* \brief �������뷨��ܴ���
* \param void
* \return 
* \note 
* \example
* \author fanbo
* \date 2010-06-08
*/
GBIMEReturn GBIME_Create(void)
{
	GBIMEReturn ret;
	GBIMEEvent imeEvent;

	// ��ֹ�ظ���ʼ��
	if (Interface_IsIMEInited())
	{
		return GBIME_OK;
	}

	// ��ʼ����ܽӿڶ���
	Interface_InitIME();

	// Init Framework
	imeEvent.nType = GBIME_EVENT_TYPE_FRAMEWORK;
	imeEvent.nEvent = GBIME_EVENT_FRAMEWORK_INIT;
	if ((ret = Handle_GBIMEMessage(&imeEvent)) != GBIME_OK)
	{
		return ret;
	}

	// Init Engine
	imeEvent.nType = GBIME_EVENT_TYPE_ENGINE;
	imeEvent.nEvent = GBIME_EVENT_ENGINE_INIT;
	if ((ret = Handle_GBIMEMessage(&imeEvent)) != GBIME_OK)
	{
		return ret;
	}

	// Init Handwrite Engine
#if defined(__GBIME_HANDWRITE__)
	imeEvent.nType = GBIME_EVENT_TYPE_HANDWRITE;
	imeEvent.nEvent = GBIME_EVENT_HANDWRITE_ENGINE_INIT;
	if ((ret = Handle_GBIMEMessage(&imeEvent)) != GBIME_OK)
	{
		return ret;
	}
#endif

	// ��������
	imeEvent.nType = GBIME_EVENT_TYPE_CONFIG;
	imeEvent.nEvent = GBIME_EVENT_CONFIG_LOAD;
	if ((ret = Handle_GBIMEMessage(&imeEvent)) != GBIME_OK)
	{
		return ret;
	}

	return ret;
}

/*!
 * \brief �������뷨��ܺ�ƽ̨�����������(�л�����ѡĬ������ģʽ��������ӦLayout��ʾ���)
 * \param platformInputMode ƽ̨�������뷨ʱ��Ĭ�����뷨ģʽ
 * \return 
 * \note �����ȵ���GBIME_Create
 * \example
 * \author weizhiping
 * \date 2010-8-20 10:20:58
*/
GBIMEReturn GBIME_Connect(GBINT platformInputMode)
{
	GBInputBox_Init();
	// ����IMEģ������״̬
	Interface_SetIMEConnectedStatus(GBTrue);
	return GBIME_OK;
}

/*!
 * \brief �������뷨����л�����ģʽ
 * \param platformInputMode
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-8-20 10:20:58
*/
GBIMEReturn GBIME_SwitchInputMode(GBINT platformInputMode)
{
	GBIMEReturn ret;
	GBIMEEvent imeEvent;
	GBUINT16 nInitialLayoutID = 0;

	PT_Assert(Interface_IsIMEInited());

	if ((ret = GBPA_GetLayoutIDByPlatformInputMode(platformInputMode, &nInitialLayoutID)) != GBIME_OK)
	{
		PT_Assert(ret != GBIME_OK); // ��֧�ֵ����뷨ģʽ
		return ret;
	}
	
	// �л�����ʼLayout ���뷨���
	imeEvent.nType = GBIME_EVENT_TYPE_LAYOUT;
	imeEvent.nEvent = GBIME_EVENT_LAYOUT_SWITCH;
	imeEvent.wParam = nInitialLayoutID;
	Handle_GBIMEMessage(&imeEvent);

	// �л����뷨ģʽ
	imeEvent.nType = GBIME_EVENT_TYPE_INPUT_METHOD;
	imeEvent.nEvent = GBIME_EVENT_INPUT_METHOD_SWITCH;
	imeEvent.wParam = nInitialLayoutID;
	Handle_GBIMEMessage(&imeEvent);

	// ÿ���л�Layout֮�����¼���������뷨���
	imeEvent.nType = GBIME_EVENT_TYPE_LAYOUT;
	imeEvent.nEvent = GBIME_EVENT_LAYOUT_ACTIVATE;
	ret = Handle_GBIMEMessage(&imeEvent);

	return GBIME_OK;
}

/*!
 * \brief �������뷨��ܺ�ƽ̨�������Ͽ�����
 * \param void 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-8-31 14:17:17
*/
GBIMEReturn GBIME_Disconnect(void)
{
 	GBIMEReturn ret;
	GBIMEEvent imeEvent;
// 	
// 	PT_Assert(Interface_IsIMEInited());
// 	
// 	// Reset Engine
// 	imeEvent.nType = GBIME_EVENT_TYPE_ENGINE;
// 	imeEvent.nEvent = GBIME_EVENT_ENGINE_RESET;
// 	if ((ret = Handle_GBIMEMessage(&imeEvent)) != GBIME_OK)
// 	{
// 		return ret;
// 	}

	//GBIME_Hide();
	// ע���������뷨���[ĳЩĳЩӦ�õ�һ�ν����Ͽ�imc�����˺�������������������ʱ����VK�ᵼ��inline�༭���޷�����]
	imeEvent.nType = GBIME_EVENT_TYPE_LAYOUT;
	imeEvent.nEvent = GBIME_EVENT_LAYOUT_DEACTIVATE;
	ret = Handle_GBIMEMessage(&imeEvent);

	// ���IMEģ������״̬
	Interface_SetIMEConnectedStatus(GBFalse);
	
	return GBIME_OK;
}

/*!
* \brief �������뷨�������
* \return 
* \note �����ٿ���ڲ���ģ�飬�������ٿ��
* \example
* \author fanbo
* \date 2010-06-08
*/
GBIMEReturn GBIME_Destroy(void)
{
	GBIMEReturn ret;
	GBIMEEvent imeEvent;

	if (!Interface_IsIMEInited())
	{
		return GBIME_OK;
	}
	
	// exit Engine
	imeEvent.nType = GBIME_EVENT_TYPE_ENGINE;
	imeEvent.nEvent = GBIME_EVENT_ENGINE_EXIT;
	if ((ret = Handle_GBIMEMessage(&imeEvent)) != GBIME_OK)
	{
		return ret;
	}

	// exit Handwrite Engine
#if defined(__GBIME_HANDWRITE__)
	imeEvent.nType = GBIME_EVENT_TYPE_HANDWRITE;
	imeEvent.nEvent = GBIME_EVENT_HANDWRITE_ENGINE_EXIT;
	if ((ret = Handle_GBIMEMessage(&imeEvent)) != GBIME_OK)
	{
		return ret;
	}
#endif

	// exit Config
	imeEvent.nType = GBIME_EVENT_TYPE_CONFIG;
	imeEvent.nEvent = GBIME_EVENT_CONFIG_EXIT;
	if ((ret = Handle_GBIMEMessage(&imeEvent)) != GBIME_OK)
	{
		return ret;
	}

	// exit Framework
	imeEvent.nType = GBIME_EVENT_TYPE_FRAMEWORK;
	imeEvent.nEvent = GBIME_EVENT_FRAMEWORK_EXIT;
	if ((ret = Handle_GBIMEMessage(&imeEvent)) != GBIME_OK)
	{
		return ret;
	}
	
	return ret;
}

/*!
 * \brief ƽ̨���뷨������������´�����
 * \param key_code 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-22 14:09:23
 */
GBIMEReturn GBIME_KeyDownHandler(GBINT key_code)
{
	return Handle_GBIMEInterface(GBPEVT_KEY_DOWN, key_code, 0);
}

/*!
 * \brief ƽ̨���뷨�����������������
 * \param key_code 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-22 14:09:23
 */
GBIMEReturn GBIME_KeyUpHandler(GBINT key_code)
{
	return Handle_GBIMEInterface(GBPEVT_KEY_UP, key_code, 0);
}

/*!
 * \brief ƽ̨���뷨�������������������
 * \param key_code 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-22 14:09:23
 */
GBIMEReturn GBIME_KeyLongPressHandler(GBINT key_code)
{
	return Handle_GBIMEInterface(GBPEVT_KEY_LONG_PRESS, key_code, 0);
}

/*!
 * \brief ƽ̨���뷨������������ٶ̰�������
 * \param key_code 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-22 14:09:23
 */
GBIMEReturn GBIME_KeyRepeatHandler(GBINT key_code)
{
	return Handle_GBIMEInterface(GBPEVT_KEY_REPEAT, key_code, 0);
}

/*!
 * \brief ƽ̨���뷨��ܴ��������´�����
 * \param x
 * \param y
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-22 14:11:58
 */
GBIMEReturn GBIME_PenDownHandler(GBINT x, GBINT y)
{
	return Handle_GBIMEInterface(GBPEVT_PEN_DOWN, x, y);
}
/*!
 * \brief ƽ̨���뷨��ܴ�������������
 * \param x
 * \param y
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-22 14:11:58
*/
GBIMEReturn GBIME_PenUpHandler(GBINT x, GBINT y)
{
	return Handle_GBIMEInterface(GBPEVT_PEN_UP, x, y);
}

/*!
 * \brief ƽ̨���뷨��ܴ�����Move������
 * \param x
 * \param y
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-22 14:11:58
*/
GBIMEReturn GBIME_PenMoveHandler(GBINT x, GBINT y)
{
	return Handle_GBIMEInterface(GBPEVT_PEN_MOVE, x, y);
}

/*!
 * \brief �������뷨��嵯����ʾ
 * \return 
 * \note 
 * \example
 * \author fanbo
 * \date 2010-06-08
 */
GBIMEReturn GBIME_Show(void)
{
	GBIMEReturn ret;
	GBIMEEvent imeEvent;

	imeEvent.nType = GBIME_EVENT_TYPE_LAYOUT;
	imeEvent.nEvent = GBIME_EVENT_LAYOUT_SHOW;
	if ((ret = Handle_GBIMEMessage(&imeEvent)) != GBIME_OK)
	{
		return ret;
	}

	return ret;
}

/*!
 * \brief �������뷨�������
 * \return 
 * \note 
 * \example
 * \author fanbo
 * \date 2010-06-08
 */
GBIMEReturn GBIME_Hide(void)
{
	GBIMEReturn ret;
	GBIMEEvent imeEvent;
	
	imeEvent.nType = GBIME_EVENT_TYPE_LAYOUT;
	imeEvent.nEvent = GBIME_EVENT_LAYOUT_HIDE;
	if ((ret = Handle_GBIMEMessage(&imeEvent)) != GBIME_OK)
	{
		return ret;
	}

	return ret;
}

/*!
 * \brief �������뷨���ǿ��ˢ����ʾ
 * \param void 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-17 11:17:16
*/
GBIMEReturn GBIME_Paint(void)
{
	GBIMEReturn ret;
	GBIMEEvent imeEvent;

	// Layout ���ڴ�������״̬���ػ�
	if (!Interface_GetLayoutWinActiveStatus())
	{
		return GBIME_IGNORE;
	}

	//pidongming add
	if(mmi_imc_is_key_disabled())
	{
		return GBIME_IGNORE;
	}
	//pidongming add end

	imeEvent.nType = GBIME_EVENT_TYPE_LAYOUT;
	imeEvent.nEvent = GBIME_EVENT_LAYOUT_REDRAW;
	if ((ret = Handle_GBIMEMessage(&imeEvent)) != GBIME_OK)
	{
		return ret;
	}

	return ret;
}

/*!
 * \brief �������뷨���ڵ���ʼ��ʾλ��(�����½��������Ļ�����꣬��Ϊ���Ͻǻ����ź�ѡ��߶ȱ仯)
 * \param pWinPos 
 * \note ���ڿ�ܳ�ʼ��ʱ���ã�Ҳ���Զ�̬����
 * \return 
 */
GBIMEReturn GBIME_SetWinPos(GBIMEPoint *pWinPos)
{
	return Interface_SetWinPos(pWinPos);
}

/*!
 * \brief ��ȡ���뷨���ڵ����Ͻ�λ��(���½���ʼ��ʾλ�� + Layout������ʾ�߶�)
 * \param pWinPos 
 * \note ��ܴ�����ʾ״̬����Ч
 * \return 
 */
GBIMEReturn GBIME_GetWinPos(GBIMEPoint *pWinPos)
{
	return Interface_GetWinPos(pWinPos);
}

/*!
 * \brief ��ȡ���뷨���ڵ���ʾ
 * \param void 
 * \note �߶ȷ����仯ʱ��Ҫ֪ͨMTKƽ̨�༭������ˢ�£�����ᵼ�������������뷨����ˢ�²�����
 * \return 
 */
GBINT GBIME_GetWinHeight(void)
{
	return Interface_GetWinHeight();
}
/*!
 * \brief ��ȡ���뷨���ڵ���ʾ
 * \param void 
 * \note ��ȷ����仯ʱ��Ҫ֪ͨMTKƽ̨�༭������ˢ�£�����ᵼ�������������뷨����ˢ�²�����
 * \return 
 */
GBINT GBIME_GetWinWidth(void)
{
	return Interface_GetWinWidth();
}

/*!
 * \brief �ж�������Ƿ�����Layout����������
 * \param point 
 * \return 
 */
GBBOOL GBIME_PointIsInLayoutRect(GBIMEPoint point)
{
	GBRECT layoutRect = {0};
	GBIMEPoint layoutWinStartPos;
	PCLayoutAbstract pCurrentLayoutObject = Global_GetCurrentLayoutObject();	

	Global_GetWinPos(&layoutWinStartPos); // note: ���½�λ��
	// ��ȡLayout��������
	FILL_RECT(&layoutRect, 
		layoutWinStartPos.x, 
		(GBINT16)(layoutWinStartPos.y - Interface_GetWinHeight()), 
		Interface_GetWinWidth(), 
		layoutWinStartPos.y);

	if(PT_PointIsInRect(point, layoutRect))
	{
		return GBTrue;
	}

	return GBFalse;
}

/*!
 * \brief �ж����뷨����Ƿ���Ҫ����˴�������Ϣ
 * \param nPenEvent 
 * \param point 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-11-22 20:07:44
 */
 GBBOOL GBIME_ShouldHandlePenEvent(GBINT nPenEvent, GBIMEPoint point)
{
	GBRECT layoutRect = {0};
	GBIMEPoint layoutWinStartPos;
	PCLayoutAbstract pCurrentLayoutObject = Global_GetCurrentLayoutObject();
	Global_GetWinPos(&layoutWinStartPos); // note: ���½�λ��

	/*if(IS_HANDWRITE_LAYOUT(GET_LAYOUT_ID(pCurrentLayoutObject))
		&& point.y < layoutWinStartPos.y) // ������������޷���Ӧ
	{
		return GBTrue; // ��дLayout��Ҫȫ������Pen��Ϣ[��Ҫ��ƽ̨ʶ���������HitTest���������ú�candidateDisplayStartPos->Layout�ڲ�����]
	}
	else */if(IS_KB_LAYOUT(GET_LAYOUT_ID(pCurrentLayoutObject)) && point.y < layoutWinStartPos.y)
	{
		return GBTrue; // ����Layout�����ѡ������������Ҫ�л�������Layout || �����ѡ��������ֱ��Cosume Pen��Ϣ
	}
	else if (pCurrentLayoutObject->penDownPostion != PenPositionInNothing
		&& (nPenEvent == GBPEVT_PEN_MOVE || nPenEvent == GBPEVT_PEN_UP))
	{
		return GBTrue; // ��������Layout������Ȼ���뿪Layout����Ҫˢ��Layout��ť״̬
	}
	else if (!Global_GetLayoutWinActiveStatus() && point.y < layoutWinStartPos.y)
	{
		return GBTrue; // ����״̬���������Pen��Ϣ��������������
	}
	else
	{
		return GBIME_PointIsInLayoutRect(point);
	}

	return GBFalse;
}

/*!
 * \brief ��ȡ��ǰ��������
 * \param void 
 * \return KEYBOARD_TYPE
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-10-28
*/
KEYBOARD_TYPE GBIME_GetKeyboardType(void)
{
	return GBConfig_GetKeyboardType(Global_GetConfigInstance());
}

#if __GBIME_LOG_TO_FILE__ > 0
GBINT GBIME_OpenFile(char* filePath)
{
	return PT_OpenFile(filePath);
}
void GBIME_CloseFile()
{
	PT_CloseFile();
}
#endif

/*!
 * \brief ���ù������뷨����������
 * \param input_mode 
 * \note 
 * \return 
 */
/*void GBIME_SetVirtualKeyboard(mmi_imm_input_mode_enum input_mode)
{
	GBIME_Connect(input_mode);
	GBIME_Show();
}*/