/***************************************************************************
* File Name    :  CConfigure.c
* Corporation  :  Guobi Technology Ltd. GuangZhou China
* Copyright    :  Copyright Guobi Technology Ltd. And Subject to Confidentiality 
*                 Restrictions. This document is the confidential and proprietary 
*                 property of Guobi Technology Ltd.It is neither the document                  
*                 itself nor any part of it can be shared with any party  without 
*                 the written  permission. 
* Author       :  fanbo
* Date         :  2010-06-08
* Purpose      :  ����ʵ���ļ�
* Version      :  V1.0
* Last Changed List
* Name         |   date   |  Discription
****************************************************************************/
#include "CConfigure.h"
#include "gbdef.h"
#include "GB_Global.h"
#include "CProductSetting.h"
#include "CConfigMenu.h"

/*!
 * \brief ÿ�ּ��������µ�ȱʡLayoutID���Ʊ�
 */
typedef struct _tagKeyboardLayoutIDMap
{
	KEYBOARD_TYPE       keyboardType;	///<ƽ̨����ģʽ
	GBUINT16			preferLayoutID; ///<ƽ̨����ģʽ��ӳ�����������뷨ģʽ
	GBUINT16			defLatestAlpLayoutID; ///<Ĭ�ϵ��������LayoutID
	GBUINT16			defLatestChnLayoutID; ///<Ĭ�ϵ��������LayoutID
} KeyboardLayoutIDMap, * PKeyboardLayoutIDMap;
static const KeyboardLayoutIDMap DefaultLayoutIDList[KEYBOARD_NUM] = 
{
	{KEYBOARD_VK_NUMPAD,	LayoutIdSP9JPinYin,		LayoutIdSP9JLowerEnglish,	LayoutIdSP9JPinYin}		// ���ּ���
	,{KEYBOARD_VK_QWERTY,	LayoutIdSP26JPinYin,	LayoutIdSP26JLowerEnglish,	LayoutIdSP26JPinYin}	// Qwerty����
	,{KEYBOARD_KB_NUMPAD,	LayoutIdWL9JPinYin,		LayoutIdEnd,	LayoutIdEnd}	// �������9��
	,{KEYBOARD_KB_QWERTY,	LayoutIdWL26JPinYin,	LayoutIdEnd,	LayoutIdEnd}	// �������9��
};

static GBINT IGBConfig_GetPreferLayoutID(KEYBOARD_TYPE keyboardType)
{
	GBINT i;

	for (i = 0; i < KEYBOARD_NUM; i++)
	{
		if (DefaultLayoutIDList[i].keyboardType == keyboardType)
		{
			return DefaultLayoutIDList[i].preferLayoutID;
		}			
	}

	PT_Assert(i < KEYBOARD_NUM); // �����DefaultLayoutIDListӳ���
	return LayoutIdSPQuanPingShouXiue;
}

static GBINT IGBConfig_GetProductIDByKeyboardType(KEYBOARD_TYPE keyboardType)
{
	GBINT productID = PRODUCE_NUMPAD_1;

	switch (keyboardType)
	{
	case KEYBOARD_VK_NUMPAD:
		productID = PRODUCE_NUMPAD_1;
		break;
	case KEYBOARD_VK_QWERTY:
		productID = PRODUCE_EXPLICIT_1;
		break;
	case KEYBOARD_KB_NUMPAD:
		productID = PRODUCE_NUMPAD_1;
		break;
	case KEYBOARD_KB_QWERTY:
		productID = PRODUCE_EXPLICIT_1;
		break;
	default:
		break;
	}

	return productID;
}

/*!
 * \brief ���ع�����������
 * \param  pConfig
 * \return 
 * \note 
 * \example
 * \author fanbo
 * \date
 */
GBIMEReturn  GBConfig_Load(CConfig *pConfig)
{
	// ��ʼ����������
// 	InitEngineConfig(&pConfig->engineConfig);
// 	SelectProduce(&pConfig->engineConfig, PRODUCE_NUMPAD_1);
	
	//ѡ���Ʒ
	SelectProduct(PRODUCE_NUMPAD_1);


	// ��ʼ��Layout����
	pConfig->bHasInit = GBTrue;
	pConfig->m_bSupportSlidingSwitchLayout = GBTrue;

#if defined(__GBIME_VK__) // �����������
	pConfig->m_keyboardType = KEYBOARD_VK_NUMPAD;
	pConfig->m_preferLayoutID = IGBConfig_GetPreferLayoutID(KEYBOARD_VK_NUMPAD);
#else // �������
	pConfig->m_keyboardType = KEYBOARD_KB_NUMPAD;
	pConfig->m_preferLayoutID = IGBConfig_GetPreferLayoutID(KEYBOARD_KB_NUMPAD);
#endif

	pConfig->m_respondKeyType = RESPOND_KEY_UP;
	return GBIME_OK;
}

/*!
 * \brief ж�ع�����������
 * \param pConfig 
 * \return 
 * \note 
 * \example
 * \author fanbo
 * \date
 */
GBIMEReturn  GBConfig_Unload(CConfig *pConfig)
{
	return GBIME_OK;
}

/*!
 * \brief ������Ϣ������
 * \param pIMEEvent 
 * \return 
 */
GBIMEReturn GBConfig_HandleMessage(PGBIMEEvent pIMEEvent)
{
	GBIMEReturn ret = GBIME_OK;

	if (pIMEEvent->nType != GBIME_EVENT_TYPE_CONFIG)
	{
		return GBIME_IGNORE; // �������¼�������
	}

	switch (pIMEEvent->nEvent)
	{
	case GBIME_EVENT_CONFIG_LOAD:
		ret = GBConfig_Load(Global_GetConfigInstance());
		break;
	case GBIME_EVENT_CONFIG_MENU:
		EntryGbSetUpScreen();
		break;
	case GBIME_EVENT_CONFIG_EXIT:
		FreeConfig();
		break;
	default:
		PT_Assert(0); // �Ƿ��¼�
	}

	return ret;
}

/*!
 * \brief ���ü������
 * \param pConfig 
 * \param keyboardType 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-8-19 20:18:24
*/
GBIMEReturn  GBConfig_SetKeyboardType(CConfig *pConfig, KEYBOARD_TYPE keyboardType)
{
	if (pConfig->m_keyboardType != keyboardType)
	{
		// ���ü������
		pConfig->m_keyboardType = keyboardType;
		// ���µ�ǰ���������µ�ȱʡLayoutID
		pConfig->m_preferLayoutID = IGBConfig_GetPreferLayoutID(keyboardType);
		SelectProduct(IGBConfig_GetProductIDByKeyboardType(keyboardType));
	}
	
	return GBIME_OK;
}

/*!
 * \brief ���ü������
 * \param pConfig 
 * \param keyboardType 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-8-19 20:18:24
*/
KEYBOARD_TYPE  GBConfig_GetKeyboardType(CConfig *pConfig)
{
	return pConfig->m_keyboardType;
}

/*!
 * \brief ��ȡȱʡLayoutID
 * \param pConfig 
 * \return ȱʡLayoutID
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-8-19 20:18:24
*/
GBUINT16 GBConfig_GetPreferLayoutID(CConfig *pConfig)
{
	return pConfig->m_preferLayoutID;
}

/*!
 * \brief ��ȡĬ�ϵ��������LayoutID
 * \param pConfig 
 * \param kbType 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-12-8 10:11:25
*/
GBUINT16 GBConfig_GetDefLatestAlpLayoutID(CConfig *pConfig, KEYBOARD_TYPE kbType)
{
	GBINT i;

	for (i = 0; i < KEYBOARD_NUM; i++)
	{
		if (DefaultLayoutIDList[i].keyboardType == kbType)
		{
			return DefaultLayoutIDList[i].defLatestAlpLayoutID;
		}			
	}

	PT_Assert(i < KEYBOARD_NUM); // �����DefaultLayoutIDListӳ���
	return LayoutIdEnd;
}

/*!
 * \brief Ĭ�ϵ��������LayoutID
 * \param pConfig 
 * \param kbType 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-11-22 19:22:10
 */
GBUINT16 GBConfig_GetDefLatestChnLayoutID(CConfig *pConfig, KEYBOARD_TYPE kbType)
{
	GBINT i;

	for (i = 0; i < KEYBOARD_NUM; i++)
	{
		if (DefaultLayoutIDList[i].keyboardType == kbType)
		{
			return DefaultLayoutIDList[i].defLatestChnLayoutID;
		}			
	}

	PT_Assert(i < KEYBOARD_NUM); // �����DefaultLayoutIDListӳ���
	return LayoutIdEnd;
}