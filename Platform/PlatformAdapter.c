/***************************************************************************
* File Name    :  PlatformAdapter.c
* Corporation  :  Guobi Technology Ltd. GuangZhou China
* Copyright    :  Copyright Guobi Technology Ltd. And Subject to Confidentiality 
*                 Restrictions. This document is the confidential and proprietary 
*                 property of Guobi Technology Ltd.It is neither the document                  
*                 itself nor any part of it can be shared with any party  without 
*                 the written  permission. 
* Author       :  weizhiping
* Date         :  2010-06-08
* Purpose      :  ��װMTKƽ̨IMC��IMM��IMUI��PEN�����뷨���༭����صĺ������ṩ��MTKIME���ʹ��(���������װAPI��: GuobiIMEngine.h)
* Version      :  V1.0
* Last Changed List
* Name         |   date   |  Discription
****************************************************************************/
#include "gbim.h"
#include "GBIME.h"
#include "PlatformAdapter.h"
#include "PlatformTools.h"
#include "CLayoutInfo.h"
#include "GB_Global.h"

#if __MTK__ > 0 // MTKƽ̨����ͷ�ļ�
#include "MMI_include.h"
#include "GlobalConstants.h"
#include "wgui_inputs.h"
#include "ImeGprot.h"
//#include "Imm.h"
#include "Imc_pen.h"
#include "Imc_symbol.h"
#include "EventsGprot_Int.h"

#include "wgui_ems.h"
#include "gui_data_types.h"
#include "IMERes.h"
#include "Word_engine.h"  
#include "Imc_config.h"
#include "ImmProt.h"
#include "PixtelDataTypes.h"
#include "MMI_features.h"
#include "oslmemory.h"
#include "Eventsdef.h"
#include "Frameworkstruct.h"
#include "EventsDef.h"
#include "Wgui.h"
#include "Timerevents.h"
#include "DebugInitDef.h"
#include "CustDataRes.h"
#include "MessagesResourceData.h"
#include "GlobalDefs.h"
#include "Gui_config.h"
#include "gdi_lcd_config.h"
#include "gui_ems.h"
#include "wgui_categories_inputs.h"
#include "gui.h"
#include "gui_themes.h"
#include "Wgui_softkeys.h"
#include "wgui_categories_defs.h"
#include "wgui_categories_util.h"
#include "gui_virtual_keyboard.h"
#include "wgui_virtual_keyboard.h"
#include "MMI_features.h"
#include "imc.h"
#include "CommonScreens.h"
#include "Multitap_engine.h"

#endif

#if __MTK__ > 0

#define MAX_MAPPING_COUNT			5 ///<���ƽ̨����������ģʽ��ӳ����Ŀ

typedef struct _tagGBIMEInputMethodMap
{
	GBINT				platformInputMode;	///<ƽ̨����ģʽ
	GBINT				mapCount;           ///<ƽ̨����ģʽ��ӳ�����������뷨ģʽ
	///< ��mapCount > 1ʱ���������뷨����ȱʡ���뷨Ӧ��ָ��ĳ�����뷨LayoutID
	GBUINT16            layoutIDArray[MAX_MAPPING_COUNT];
} GBIMEINPUTMETHODMAP, * PGBIMEINPUTMETHODMAP;

// ����ż���
const GBIMEINPUTMETHODMAP VK_9_MAPPING_PTIMM_TO_GBLAYOUTID[] = 
{
#if defined(__MMI_GBIME_LANG_SM_CHINESE__) // ��������
	// ����ƴ��
	{IMM_INPUT_MODE_SM_PINYIN,				1, {LayoutIdSP9JPinYin}},	

	// ����ƴ�����ң�������ĸ��������ĸƥ����ң���Ҫ��ֹ�������룩
	{IMM_INPUT_MODE_QUICK_SEARCH_PINYIN,	1, {LayoutIdSP9Jabc}},	

	// ����ʻ�
	{IMM_INPUT_MODE_SM_STROKE,				1, {LayoutIdSP9JStroke}},

	// ���ٱʻ����ң�������ĸ��������ĸƥ����ң���Ҫ��ֹ�������룩
	{IMM_INPUT_MODE_QUICK_SEARCH_SM_STROKE,	1, {LayoutIdSP9JStroke}},	
#endif

#if defined(__MMI_GBIME_LANG_TR_CHINESE__) // ��������
	// ����ƴ��
	{IMM_INPUT_MODE_TR_BOPOMO,				1, {LayoutIdSP9JPinYin}},	
	// ����ʻ�
	{IMM_INPUT_MODE_TR_STROKE,				1, {LayoutIdSP9JStroke}},
#endif

#if defined(__MMI_GBIME_LANG_ENGLISH__) // Ӣ��
	{IMM_INPUT_MODE_SMART_LOWERCASE_ABC,	1, {LayoutIdSP9JLowerEnglish}},
	{IMM_INPUT_MODE_SMART_UPPERCASE_ABC,	1, {LayoutIdSP9JUpEnglish}},
	{IMM_INPUT_MODE_SMART_FIRST_UPPERCASE_ABC,	1, {LayoutIdSP9JUpEnglish}},
#endif

	// ������������������뷨
	{IMM_INPUT_MODE_GBIME_SYMBOLS,			1, {LayoutIdSP9JSymbol}},
	{IMM_INPUT_MODE_GBIME_FULLSCREEN_HANDWRITE,	1, {LayoutIdSPQuanPingShouXiue}},

	// ����
	//{IMM_INPUT_MODE_MULTITAP_FIRST_UPPERCASE_ABC, 1, {LayoutIdSP9JABC}},
	{IMM_INPUT_MODE_MULTITAP_UPPERCASE_ABC, 1, {LayoutIdSP9JABC}},
	{IMM_INPUT_MODE_MULTITAP_LOWERCASE_ABC, 1, {LayoutIdSP9Jabc}},	
	{IMM_INPUT_MODE_123_SYMBOLS,			1, {LayoutIdSP9JNumber}}

	// ����MTKӦ���ض������뷨
	,{IMM_INPUT_MODE_123,					1, {LayoutIdPassword_V}}
	,{IMM_INPUT_MODE_SIGNED_123,			1, {LayoutIdSP9JNumber}}
	,{IMM_INPUT_MODE_FLOAT_123,				1, {LayoutIdFloatNumber_V}}
	,{IMM_INPUT_MODE_PHONE_NUMBER,			1, {LayoutIdPhoneNumber_V}}
	,{IMM_INPUT_MODE_PHONE_NUMBER_WILDCHAR,	1, {LayoutIdPhoneNumber_V}}
	,{IMM_INPUT_MODE_SIM_NUMERIC,			1, {LayoutIdPhoneNumber_V}}
	,{IMM_INPUT_MODE_SIGNED_FLOAT_123,		1, {LayoutIdFloatNumber_V}}
};

// ����Qwerty����
const GBIMEINPUTMETHODMAP VK_Qt_MAPPING_PTIMM_TO_GBLAYOUTID[] = 
{
#if defined(__MMI_GBIME_LANG_SM_CHINESE__) // ��������
	// ����ƴ��
	{IMM_INPUT_MODE_SM_PINYIN,				1, {LayoutIdSP26JPinYin}},	
	// ����ƴ�����ң�������ĸ��������ĸƥ����ң���Ҫ��ֹ�������룩
	{IMM_INPUT_MODE_QUICK_SEARCH_PINYIN,	1, {LayoutIdSP26Jabc}},	

	// ����ʻ�
	{IMM_INPUT_MODE_SM_STROKE,				1, {LayoutIdSP26JStroke}},
	// ���ٱʻ����ң�������ĸ��������ĸƥ����ң���Ҫ��ֹ�������룩
	{IMM_INPUT_MODE_QUICK_SEARCH_SM_STROKE,	1, {LayoutIdSP9JStroke}},
#endif
	
#if defined(__MMI_GBIME_LANG_TR_CHINESE__) // ��������
	// ����ƴ��
	{IMM_INPUT_MODE_TR_BOPOMO,				1, {LayoutIdSP26JPinYin}},	
	// ����ʻ�
	{IMM_INPUT_MODE_TR_STROKE,				1, {LayoutIdSP26JStroke}},
#endif
	
#if defined(__MMI_GBIME_LANG_ENGLISH__) // Ӣ��
	{IMM_INPUT_MODE_SMART_LOWERCASE_ABC,	1, {LayoutIdSP26JLowerEnglish}},
	{IMM_INPUT_MODE_SMART_UPPERCASE_ABC,	1, {LayoutIdSP26JUpEnglish}},
	{IMM_INPUT_MODE_SMART_FIRST_UPPERCASE_ABC,	1, {LayoutIdSP26JUpEnglish}},
#endif

	// ������������������뷨
	{IMM_INPUT_MODE_GBIME_SYMBOLS,			1, {LayoutIdSP9JSymbol}},
	{IMM_INPUT_MODE_GBIME_FULLSCREEN_HANDWRITE,	1, {LayoutIdSPQuanPingShouXiue}},
	
	// ����
	//{IMM_INPUT_MODE_MULTITAP_FIRST_UPPERCASE_ABC, 1, {LayoutIdSP26JABC}},
	{IMM_INPUT_MODE_MULTITAP_UPPERCASE_ABC, 1, {LayoutIdSP26JABC}},
	{IMM_INPUT_MODE_MULTITAP_LOWERCASE_ABC, 1, {LayoutIdSP26Jabc}},	
	{IMM_INPUT_MODE_123_SYMBOLS,			1, {LayoutIdSP9JNumber}}

	// ����MTKӦ���ض������뷨
	,{IMM_INPUT_MODE_123,					1, {LayoutIdPassword_V}}
	,{IMM_INPUT_MODE_SIGNED_123,			1, {LayoutIdSP9JNumber}}
	,{IMM_INPUT_MODE_FLOAT_123,				1, {LayoutIdFloatNumber_V}}
	,{IMM_INPUT_MODE_PHONE_NUMBER,			1, {LayoutIdPhoneNumber_V}}
	,{IMM_INPUT_MODE_PHONE_NUMBER_WILDCHAR,	1, {LayoutIdPhoneNumber_V}}
	,{IMM_INPUT_MODE_SIM_NUMERIC,			1, {LayoutIdPhoneNumber_V}}
	,{IMM_INPUT_MODE_SIGNED_FLOAT_123,		1, {LayoutIdFloatNumber_V}}
};

// ������̾ż�
const GBIMEINPUTMETHODMAP KB_9_MAPPING_PTIMM_TO_GBLAYOUTID[] = 
{
#if defined(__MMI_GBIME_LANG_SM_CHINESE__) // ��������
	// ����ƴ��
	{IMM_INPUT_MODE_SM_PINYIN,				1, {LayoutIdWL9JPinYin}},	
		// ����ʻ�
	{IMM_INPUT_MODE_SM_STROKE,				1, {LayoutIdWL9JStroke}},

	// ����ƴ�����ң�������ĸ��������ĸƥ����ң���Ҫ��ֹ�������룩
	{IMM_INPUT_MODE_QUICK_SEARCH_PINYIN,	1, {LayoutIdWL9Jabc}},	

	// ���ٱʻ����ң�������ĸ��������ĸƥ����ң���Ҫ��ֹ�������룩
	{IMM_INPUT_MODE_QUICK_SEARCH_SM_STROKE,	1, {LayoutIdWL9JStroke}},
#endif
	
// #if defined(__MMI_GBIME_LANG_TR_CHINESE__) // ��������
// 	// ����ƴ��
// 	{IMM_INPUT_MODE_TR_BOPOMO,				1, {LayoutIdWL9JPinYin}},
// 	// ����ʻ�
// 	{IMM_INPUT_MODE_TR_STROKE,				1, {LayoutIdWL9JStroke}},
// #endif
	
#if defined(__MMI_GBIME_LANG_ENGLISH__) // Ӣ��
	{IMM_INPUT_MODE_SMART_LOWERCASE_ABC,	1, {LayoutIdWL9JEnglish}},	
	{IMM_INPUT_MODE_SMART_UPPERCASE_ABC,	1, {LayoutIdWL9JEnglish}},
	{IMM_INPUT_MODE_SMART_FIRST_UPPERCASE_ABC,	1, {LayoutIdWL9JEnglish}},
#endif
	
	// ������������������뷨
	{IMM_INPUT_MODE_GBIME_SYMBOLS,			1, {LayoutIdWL9JNumber}},
	{IMM_INPUT_MODE_GBIME_FULLSCREEN_HANDWRITE,	1, {LayoutIdWL9JPinYin}},

 	// ����
 	//{IMM_INPUT_MODE_MULTITAP_FIRST_UPPERCASE_ABC, 1, {LayoutIdWL9JABC}},
  	{IMM_INPUT_MODE_MULTITAP_UPPERCASE_ABC, 1, {LayoutIdWL9JABC}},
  	{IMM_INPUT_MODE_MULTITAP_LOWERCASE_ABC, 1, {LayoutIdWL9Jabc}}, 	
	{IMM_INPUT_MODE_123_SYMBOLS,			1, {LayoutIdWL9JNumber}}
	
	// ����MTKӦ���ض������뷨
	,{IMM_INPUT_MODE_123,					1, {LayoutIdWL9JNumber}}
	,{IMM_INPUT_MODE_SIGNED_123,			1, {LayoutIdWL9JNumber}}
	,{IMM_INPUT_MODE_FLOAT_123,				1, {LayoutIdWL9JNumber}}
	,{IMM_INPUT_MODE_PHONE_NUMBER,			1, {LayoutIdWL9JNumber}}
	,{IMM_INPUT_MODE_PHONE_NUMBER_WILDCHAR,	1, {LayoutIdWL9JNumber}}
	,{IMM_INPUT_MODE_SIM_NUMERIC,			1, {LayoutIdWL9JNumber}}
	,{IMM_INPUT_MODE_SIGNED_FLOAT_123,		1, {LayoutIdWL9JNumber}}
};

// �������Qwerty
const GBIMEINPUTMETHODMAP KB_Qt_MAPPING_PTIMM_TO_GBLAYOUTID[] = 
{
#if defined(__MMI_GBIME_LANG_SM_CHINESE__) // ��������
	// ����ƴ��
	{IMM_INPUT_MODE_SM_PINYIN,				1, {LayoutIdWL26JPinYin}},	
		// ����ʻ�
	{IMM_INPUT_MODE_SM_STROKE,				1, {LayoutIdWL26JStroke}},

	// ����ƴ�����ң�������ĸ��������ĸƥ����ң���Ҫ��ֹ�������룩
	{IMM_INPUT_MODE_QUICK_SEARCH_PINYIN,	1, {LayoutIdWL26Jabc}},	

	// ���ٱʻ����ң�������ĸ��������ĸƥ����ң���Ҫ��ֹ�������룩
	{IMM_INPUT_MODE_QUICK_SEARCH_SM_STROKE,	1, {LayoutIdWL9JStroke}},
#endif
	
#if defined(__MMI_GBIME_LANG_TR_CHINESE__) // ��������
	// ����ƴ��
	{IMM_INPUT_MODE_TR_BOPOMO,				1, {LayoutIdWL26JPinYin}},
	// ����ʻ�
	{IMM_INPUT_MODE_TR_STROKE,				1, {LayoutIdWL26JStroke}},
#endif
	
#if defined(__MMI_GBIME_LANG_ENGLISH__) // Ӣ��
	{IMM_INPUT_MODE_SMART_LOWERCASE_ABC,	1, {LayoutIdWL26JEnglish}},
	{IMM_INPUT_MODE_SMART_UPPERCASE_ABC,	1, {LayoutIdWL26JEnglish}},
	{IMM_INPUT_MODE_SMART_FIRST_UPPERCASE_ABC,	1, {LayoutIdWL26JEnglish}},
#endif
	
	// ������������������뷨
	{IMM_INPUT_MODE_GBIME_SYMBOLS,			1, {LayoutIdWL9JNumber}},
	{IMM_INPUT_MODE_GBIME_FULLSCREEN_HANDWRITE,	1, {LayoutIdWL9JPinYin}},

	// ����
	//{IMM_INPUT_MODE_MULTITAP_FIRST_UPPERCASE_ABC, 1, {LayoutIdWL26JEnglish}},
	{IMM_INPUT_MODE_MULTITAP_UPPERCASE_ABC, 1, {LayoutIdWL26JEnglish}},
	{IMM_INPUT_MODE_MULTITAP_LOWERCASE_ABC, 1, {LayoutIdWL26JEnglish}},
	//	{IMM_INPUT_MODE_123,					1, {LayoutIdWL26JEnglish}},
	{IMM_INPUT_MODE_123_SYMBOLS,			1, {LayoutIdWL26JEnglish}},
};

static GBINT GBPA_GetInputMethodNum(void)
{
	KEYBOARD_TYPE keyboardType = GBConfig_GetKeyboardType(Global_GetConfigInstance());
	GBINT num = 0;
 	
	switch (keyboardType)
	{
	case KEYBOARD_VK_NUMPAD:
		num = PT_ARRAY_SIZE(VK_9_MAPPING_PTIMM_TO_GBLAYOUTID);
		break;
	case KEYBOARD_VK_QWERTY:
		num = PT_ARRAY_SIZE(VK_Qt_MAPPING_PTIMM_TO_GBLAYOUTID);
		break;
	case KEYBOARD_KB_NUMPAD:
		num = PT_ARRAY_SIZE(KB_9_MAPPING_PTIMM_TO_GBLAYOUTID);
		break;
	case KEYBOARD_KB_QWERTY:
		num = PT_ARRAY_SIZE(KB_Qt_MAPPING_PTIMM_TO_GBLAYOUTID);
		break;
	default:
		PT_Assert(0);
		break;
	}

	return num;
}

static const GBIMEINPUTMETHODMAP * GBPA_GetInputMethodMap(void)
{
	KEYBOARD_TYPE keyboardType = GBConfig_GetKeyboardType(Global_GetConfigInstance());
	const GBIMEINPUTMETHODMAP * pInputMethodMap = NULL;
	
	switch (keyboardType)
	{
	case KEYBOARD_VK_NUMPAD:
		pInputMethodMap = VK_9_MAPPING_PTIMM_TO_GBLAYOUTID;
		break;
	case KEYBOARD_VK_QWERTY:
		pInputMethodMap = VK_Qt_MAPPING_PTIMM_TO_GBLAYOUTID;
		break;
	case KEYBOARD_KB_NUMPAD:
		pInputMethodMap = KB_9_MAPPING_PTIMM_TO_GBLAYOUTID;
		break;
	case KEYBOARD_KB_QWERTY:
		pInputMethodMap = KB_Qt_MAPPING_PTIMM_TO_GBLAYOUTID;
		break;
	default:
		PT_Assert(0);
		break;
	}
	
	return pInputMethodMap;
}
#endif

/*!
 * \brief ����ƽ̨���뷨ģʽ��ȡĿ��Layout ID
 * \param platformInputMode
 * \param pLayoutID 
 * \return 
 * \note ��mapCount > 1ʱ������ѡ��������뷨����ȱʡ���뷨Ӧ��ָ����LayoutID
 *\example
 * \author weizhiping
 * \date 2010-8-4 16:24:17
 */
GBIMEReturn GBPA_GetLayoutIDByPlatformInputMode(GBINT platformInputMode, GBUINT16 *pLayoutID)
{
#if __MTK__ > 0
	GBINT i, j;
	const GBIMEINPUTMETHODMAP * pInputMethodMap = GBPA_GetInputMethodMap();
	
	for (i = 0; i < GBPA_GetInputMethodNum(); i++)
	{
		if (platformInputMode == pInputMethodMap[i].platformInputMode)
		{
			GBUINT16 preferLayoutID = GBConfig_GetPreferLayoutID(Global_GetConfigInstance());

			PT_Assert(pInputMethodMap[i].mapCount > 0); // �������������ӳ���

			for (j = 0; j < pInputMethodMap[i].mapCount; j++)
			{
				if (preferLayoutID == pInputMethodMap[i].layoutIDArray[j])
				{
					*pLayoutID = preferLayoutID; // ӳ����д�������ָ����ȱʡLayoutID������ѡ��
					return GBIME_OK;
				}			
			}

			// ӳ����в���������ָ����ȱʡLayoutID��ѡ�õ�һ��
			if (j == pInputMethodMap[i].mapCount)
			{
				*pLayoutID = pInputMethodMap[i].layoutIDArray[0];
			}

			return GBIME_OK;
		}
	}
#else
	return GBIME_OK;
#endif
	return GBIME_NOT_SUPPORT_LAYOUT_ID;
}

/*!
 * \brief ����Ŀ���л�Layout ��ID�ҵ���Ӧ��ƽ̨���뷨ģʽ
 * \param layoutID 
 * \param pPlatformInputMode 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-8-4 16:24:17
 */
GBIMEReturn GBPA_GetPlatformInputModeByLayoutID(GBUINT16 layoutID, GBINT *pPlatformInputMode)
{
#if __MTK__ > 0
	GBINT i, j;
	const GBIMEINPUTMETHODMAP * pInputMethodMap = GBPA_GetInputMethodMap();

	for (i = 0; i < GBPA_GetInputMethodNum(); i++)
	{
		for (j = 0; j < pInputMethodMap[i].mapCount; j++)
		{
			if (layoutID == pInputMethodMap[i].layoutIDArray[j])
			{
				*pPlatformInputMode = pInputMethodMap[i].platformInputMode;
				return GBIME_OK;
			}
		}
	}
#else
	return GBIME_OK;
#endif
	return GBIME_NOT_SUPPORT_INPUT_MODE;
}

/*!
 * \brief ���ݵ�ǰLayout ID��ȡ��һ��Layout ID
 * \param nCurLayoutID
 * \param pNextLayoutID 
 * \return 
 * \note ��mapCount > 1ʱ������ѡ��������뷨����ȱʡ���뷨Ӧ��ָ����LayoutID
 *\example
 * \author ZhaoKun
 * \date 2010-10-25 16:24:17
 */
GBIMEReturn GBPA_GetNextLayoutIDByCurLayoutID(GBUINT16 nCurLayoutID, GBUINT16 *pNextLayoutID)
{
#if __MTK__ > 0
	GBINT i, j;
	const GBIMEINPUTMETHODMAP * pInputMethodMap = GBPA_GetInputMethodMap();
	
	if(!pNextLayoutID)
	{
		return GBIME_INVALID_PARAM;
	}

	for (i = 0; i < GBPA_GetInputMethodNum(); i++)
	{
		for (j = 0; j < pInputMethodMap[i].mapCount; j++)
		{
			if (nCurLayoutID != pInputMethodMap[i].layoutIDArray[j])
			{
				continue;
			}
			if (i + 1 < GBPA_GetInputMethodNum())
			{
				*pNextLayoutID = pInputMethodMap[i + 1].layoutIDArray[0];
			}
			else
			{
				*pNextLayoutID = pInputMethodMap[0].layoutIDArray[0];
			}
			return GBIME_OK;
		}
	}

	*pNextLayoutID = pInputMethodMap[0].layoutIDArray[0];
#endif
	return GBIME_OK;
}


#if __MTK__ > 0 // MTKƽ̨�ο�����

/*!
 * \brief ��д��������
 * \param void 
 * \return void
 */
GBIMEGLOBAL void GBPA_PenConfirmHandwritingSelection(void)
{
	#ifdef __MMI_TOUCH_SCREEN__
	mmi_imc_pen_confirm_handwriting_selection();
	#endif
}

/*!
 * \brief ������ѡ
 * \param GBUACHAR 
 * \return void
 */
GBIMEGLOBAL void GBPA_ChangeHighlightedCandidateFromCandidateBox(GBUACHAR candidate_index)
{
	mmi_imc_change_highlighted_candidate_from_candidate_box(candidate_index);
}

/*!
 * \brief multitap����ĸup����
 * \param mmi_imc_key_custom_value,GBU16 
 * \return GBI
 * \ע�⣺mmi_imc_key_multitap_alphabetic_up_handler��imc_key����static�ģ�Ҫ�Ѻ��Ĵ���copy�����
 */
GBIMEGLOBAL GBI GBPA_KeyMultitapAlphabeticUpHandler(mmi_imc_key_custom_value key_code, GBU16 event_type)
{
	//return mmi_imc_key_multitap_alphabetic_up_handler(key_code,event_type);
	return 0;
}

/*!
 * \brief multitap����ĸdown����
 * \param mmi_imc_key_custom_value,GBU16 
 * \return GBI
 * \ע�⣺mmi_imc_key_multitap_alphabetic_down_handler��imc_key����static�ģ�Ҫ�Ѻ��Ĵ���copy�����
 */
GBIMEGLOBAL GBI GBPA_KeyMultitapAlphabeticDownHandler(mmi_imc_key_custom_value key_code, GBU16 event_type)
{
	//return mmi_imc_key_multitap_alphabetic_down_handler(key_code,event_type);
	return 0;
}

/*!
 * \brief ȫ����д
 * \param void
 * \return void
 * \ע�⣺
 */
GBIMEGLOBAL void GBPA_EditorEnableFullScreenHandwriting(void)
{
	//mmi_pen_editor_enable_full_screen_handwriting();
}

/*!
 * \brief ����״̬
 * \param mmi_imc_state_enum
 * \return void
 * \ע�⣺
 */
GBIMEGLOBAL void GBPA_SetState( mmi_imc_state_enum state)
{
	mmi_imc_set_state(state);
}
 
/*!
 * \brief ������д
 * \param 
 * \return void
 * \ע�⣺
 */
GBIMEGLOBAL GBU32 GBPA_ClearHandWritingStroke(void)
{
	//return mmi_imui_clear_hand_writing_stroke();
	return 0;
}

/*!
* \brief ��������Ļ
* \param GBWCHAR, GbFuncPtr,GbFuncPtr,GBPVOID
* \param rect 
* \return GBUACHAR
*/
GBIMEGLOBAL GBUACHAR GBPA_EntryNewScreen(GBWCHAR newscrnID, GbFuncPtr newExitHandler, GbFuncPtr newEntryHandler, GBPVOID flag)
{
	return EntryNewScreen(newscrnID,newExitHandler,newEntryHandler,flag);
}
#endif // __MTK__


