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
* Purpose      :  封装MTK平台IMC、IMM、IMUI、PEN等输入法及编辑器相关的函数，提供给MTKIME框架使用(国笔引擎封装API在: GuobiIMEngine.h)
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

#if __MTK__ > 0 // MTK平台依赖头文件
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

#define MAX_MAPPING_COUNT			5 ///<最大平台到国笔输入模式的映射数目

typedef struct _tagGBIMEInputMethodMap
{
	GBINT				platformInputMode;	///<平台输入模式
	GBINT				mapCount;           ///<平台输入模式能映射多个国笔输入法模式
	///< 当mapCount > 1时，国笔输入法配置缺省输入法应该指定某个输入法LayoutID
	GBUINT16            layoutIDArray[MAX_MAPPING_COUNT];
} GBIMEINPUTMETHODMAP, * PGBIMEINPUTMETHODMAP;

// 虚拟九键盘
const GBIMEINPUTMETHODMAP VK_9_MAPPING_PTIMM_TO_GBLAYOUTID[] = 
{
#if defined(__MMI_GBIME_LANG_SM_CHINESE__) // 简体中文
	// 简体拼音
	{IMM_INPUT_MODE_SM_PINYIN,				1, {LayoutIdSP9JPinYin}},	

	// 快速拼音查找（输入字母进行首字母匹配查找，需要禁止汉字输入）
	{IMM_INPUT_MODE_QUICK_SEARCH_PINYIN,	1, {LayoutIdSP9Jabc}},	

	// 简体笔画
	{IMM_INPUT_MODE_SM_STROKE,				1, {LayoutIdSP9JStroke}},

	// 快速笔画查找（输入字母进行首字母匹配查找，需要禁止汉字输入）
	{IMM_INPUT_MODE_QUICK_SEARCH_SM_STROKE,	1, {LayoutIdSP9JStroke}},	
#endif

#if defined(__MMI_GBIME_LANG_TR_CHINESE__) // 繁体中文
	// 繁体拼音
	{IMM_INPUT_MODE_TR_BOPOMO,				1, {LayoutIdSP9JPinYin}},	
	// 繁体笔画
	{IMM_INPUT_MODE_TR_STROKE,				1, {LayoutIdSP9JStroke}},
#endif

#if defined(__MMI_GBIME_LANG_ENGLISH__) // 英文
	{IMM_INPUT_MODE_SMART_LOWERCASE_ABC,	1, {LayoutIdSP9JLowerEnglish}},
	{IMM_INPUT_MODE_SMART_UPPERCASE_ABC,	1, {LayoutIdSP9JUpEnglish}},
	{IMM_INPUT_MODE_SMART_FIRST_UPPERCASE_ABC,	1, {LayoutIdSP9JUpEnglish}},
#endif

	// 国笔虚拟键盘特有输入法
	{IMM_INPUT_MODE_GBIME_SYMBOLS,			1, {LayoutIdSP9JSymbol}},
	{IMM_INPUT_MODE_GBIME_FULLSCREEN_HANDWRITE,	1, {LayoutIdSPQuanPingShouXiue}},

	// 公共
	//{IMM_INPUT_MODE_MULTITAP_FIRST_UPPERCASE_ABC, 1, {LayoutIdSP9JABC}},
	{IMM_INPUT_MODE_MULTITAP_UPPERCASE_ABC, 1, {LayoutIdSP9JABC}},
	{IMM_INPUT_MODE_MULTITAP_LOWERCASE_ABC, 1, {LayoutIdSP9Jabc}},	
	{IMM_INPUT_MODE_123_SYMBOLS,			1, {LayoutIdSP9JNumber}}

	// 补充MTK应用特定的输入法
	,{IMM_INPUT_MODE_123,					1, {LayoutIdPassword_V}}
	,{IMM_INPUT_MODE_SIGNED_123,			1, {LayoutIdSP9JNumber}}
	,{IMM_INPUT_MODE_FLOAT_123,				1, {LayoutIdFloatNumber_V}}
	,{IMM_INPUT_MODE_PHONE_NUMBER,			1, {LayoutIdPhoneNumber_V}}
	,{IMM_INPUT_MODE_PHONE_NUMBER_WILDCHAR,	1, {LayoutIdPhoneNumber_V}}
	,{IMM_INPUT_MODE_SIM_NUMERIC,			1, {LayoutIdPhoneNumber_V}}
	,{IMM_INPUT_MODE_SIGNED_FLOAT_123,		1, {LayoutIdFloatNumber_V}}
};

// 虚拟Qwerty键盘
const GBIMEINPUTMETHODMAP VK_Qt_MAPPING_PTIMM_TO_GBLAYOUTID[] = 
{
#if defined(__MMI_GBIME_LANG_SM_CHINESE__) // 简体中文
	// 简体拼音
	{IMM_INPUT_MODE_SM_PINYIN,				1, {LayoutIdSP26JPinYin}},	
	// 快速拼音查找（输入字母进行首字母匹配查找，需要禁止汉字输入）
	{IMM_INPUT_MODE_QUICK_SEARCH_PINYIN,	1, {LayoutIdSP26Jabc}},	

	// 简体笔画
	{IMM_INPUT_MODE_SM_STROKE,				1, {LayoutIdSP26JStroke}},
	// 快速笔画查找（输入字母进行首字母匹配查找，需要禁止汉字输入）
	{IMM_INPUT_MODE_QUICK_SEARCH_SM_STROKE,	1, {LayoutIdSP9JStroke}},
#endif
	
#if defined(__MMI_GBIME_LANG_TR_CHINESE__) // 繁体中文
	// 繁体拼音
	{IMM_INPUT_MODE_TR_BOPOMO,				1, {LayoutIdSP26JPinYin}},	
	// 繁体笔画
	{IMM_INPUT_MODE_TR_STROKE,				1, {LayoutIdSP26JStroke}},
#endif
	
#if defined(__MMI_GBIME_LANG_ENGLISH__) // 英文
	{IMM_INPUT_MODE_SMART_LOWERCASE_ABC,	1, {LayoutIdSP26JLowerEnglish}},
	{IMM_INPUT_MODE_SMART_UPPERCASE_ABC,	1, {LayoutIdSP26JUpEnglish}},
	{IMM_INPUT_MODE_SMART_FIRST_UPPERCASE_ABC,	1, {LayoutIdSP26JUpEnglish}},
#endif

	// 国笔虚拟键盘特有输入法
	{IMM_INPUT_MODE_GBIME_SYMBOLS,			1, {LayoutIdSP9JSymbol}},
	{IMM_INPUT_MODE_GBIME_FULLSCREEN_HANDWRITE,	1, {LayoutIdSPQuanPingShouXiue}},
	
	// 公共
	//{IMM_INPUT_MODE_MULTITAP_FIRST_UPPERCASE_ABC, 1, {LayoutIdSP26JABC}},
	{IMM_INPUT_MODE_MULTITAP_UPPERCASE_ABC, 1, {LayoutIdSP26JABC}},
	{IMM_INPUT_MODE_MULTITAP_LOWERCASE_ABC, 1, {LayoutIdSP26Jabc}},	
	{IMM_INPUT_MODE_123_SYMBOLS,			1, {LayoutIdSP9JNumber}}

	// 补充MTK应用特定的输入法
	,{IMM_INPUT_MODE_123,					1, {LayoutIdPassword_V}}
	,{IMM_INPUT_MODE_SIGNED_123,			1, {LayoutIdSP9JNumber}}
	,{IMM_INPUT_MODE_FLOAT_123,				1, {LayoutIdFloatNumber_V}}
	,{IMM_INPUT_MODE_PHONE_NUMBER,			1, {LayoutIdPhoneNumber_V}}
	,{IMM_INPUT_MODE_PHONE_NUMBER_WILDCHAR,	1, {LayoutIdPhoneNumber_V}}
	,{IMM_INPUT_MODE_SIM_NUMERIC,			1, {LayoutIdPhoneNumber_V}}
	,{IMM_INPUT_MODE_SIGNED_FLOAT_123,		1, {LayoutIdFloatNumber_V}}
};

// 物理键盘九键
const GBIMEINPUTMETHODMAP KB_9_MAPPING_PTIMM_TO_GBLAYOUTID[] = 
{
#if defined(__MMI_GBIME_LANG_SM_CHINESE__) // 简体中文
	// 简体拼音
	{IMM_INPUT_MODE_SM_PINYIN,				1, {LayoutIdWL9JPinYin}},	
		// 简体笔画
	{IMM_INPUT_MODE_SM_STROKE,				1, {LayoutIdWL9JStroke}},

	// 快速拼音查找（输入字母进行首字母匹配查找，需要禁止汉字输入）
	{IMM_INPUT_MODE_QUICK_SEARCH_PINYIN,	1, {LayoutIdWL9Jabc}},	

	// 快速笔画查找（输入字母进行首字母匹配查找，需要禁止汉字输入）
	{IMM_INPUT_MODE_QUICK_SEARCH_SM_STROKE,	1, {LayoutIdWL9JStroke}},
#endif
	
// #if defined(__MMI_GBIME_LANG_TR_CHINESE__) // 繁体中文
// 	// 繁体拼音
// 	{IMM_INPUT_MODE_TR_BOPOMO,				1, {LayoutIdWL9JPinYin}},
// 	// 繁体笔画
// 	{IMM_INPUT_MODE_TR_STROKE,				1, {LayoutIdWL9JStroke}},
// #endif
	
#if defined(__MMI_GBIME_LANG_ENGLISH__) // 英文
	{IMM_INPUT_MODE_SMART_LOWERCASE_ABC,	1, {LayoutIdWL9JEnglish}},	
	{IMM_INPUT_MODE_SMART_UPPERCASE_ABC,	1, {LayoutIdWL9JEnglish}},
	{IMM_INPUT_MODE_SMART_FIRST_UPPERCASE_ABC,	1, {LayoutIdWL9JEnglish}},
#endif
	
	// 国笔虚拟键盘特有输入法
	{IMM_INPUT_MODE_GBIME_SYMBOLS,			1, {LayoutIdWL9JNumber}},
	{IMM_INPUT_MODE_GBIME_FULLSCREEN_HANDWRITE,	1, {LayoutIdWL9JPinYin}},

 	// 公共
 	//{IMM_INPUT_MODE_MULTITAP_FIRST_UPPERCASE_ABC, 1, {LayoutIdWL9JABC}},
  	{IMM_INPUT_MODE_MULTITAP_UPPERCASE_ABC, 1, {LayoutIdWL9JABC}},
  	{IMM_INPUT_MODE_MULTITAP_LOWERCASE_ABC, 1, {LayoutIdWL9Jabc}}, 	
	{IMM_INPUT_MODE_123_SYMBOLS,			1, {LayoutIdWL9JNumber}}
	
	// 补充MTK应用特定的输入法
	,{IMM_INPUT_MODE_123,					1, {LayoutIdWL9JNumber}}
	,{IMM_INPUT_MODE_SIGNED_123,			1, {LayoutIdWL9JNumber}}
	,{IMM_INPUT_MODE_FLOAT_123,				1, {LayoutIdWL9JNumber}}
	,{IMM_INPUT_MODE_PHONE_NUMBER,			1, {LayoutIdWL9JNumber}}
	,{IMM_INPUT_MODE_PHONE_NUMBER_WILDCHAR,	1, {LayoutIdWL9JNumber}}
	,{IMM_INPUT_MODE_SIM_NUMERIC,			1, {LayoutIdWL9JNumber}}
	,{IMM_INPUT_MODE_SIGNED_FLOAT_123,		1, {LayoutIdWL9JNumber}}
};

// 物理键盘Qwerty
const GBIMEINPUTMETHODMAP KB_Qt_MAPPING_PTIMM_TO_GBLAYOUTID[] = 
{
#if defined(__MMI_GBIME_LANG_SM_CHINESE__) // 简体中文
	// 简体拼音
	{IMM_INPUT_MODE_SM_PINYIN,				1, {LayoutIdWL26JPinYin}},	
		// 简体笔画
	{IMM_INPUT_MODE_SM_STROKE,				1, {LayoutIdWL26JStroke}},

	// 快速拼音查找（输入字母进行首字母匹配查找，需要禁止汉字输入）
	{IMM_INPUT_MODE_QUICK_SEARCH_PINYIN,	1, {LayoutIdWL26Jabc}},	

	// 快速笔画查找（输入字母进行首字母匹配查找，需要禁止汉字输入）
	{IMM_INPUT_MODE_QUICK_SEARCH_SM_STROKE,	1, {LayoutIdWL9JStroke}},
#endif
	
#if defined(__MMI_GBIME_LANG_TR_CHINESE__) // 繁体中文
	// 繁体拼音
	{IMM_INPUT_MODE_TR_BOPOMO,				1, {LayoutIdWL26JPinYin}},
	// 繁体笔画
	{IMM_INPUT_MODE_TR_STROKE,				1, {LayoutIdWL26JStroke}},
#endif
	
#if defined(__MMI_GBIME_LANG_ENGLISH__) // 英文
	{IMM_INPUT_MODE_SMART_LOWERCASE_ABC,	1, {LayoutIdWL26JEnglish}},
	{IMM_INPUT_MODE_SMART_UPPERCASE_ABC,	1, {LayoutIdWL26JEnglish}},
	{IMM_INPUT_MODE_SMART_FIRST_UPPERCASE_ABC,	1, {LayoutIdWL26JEnglish}},
#endif
	
	// 国笔虚拟键盘特有输入法
	{IMM_INPUT_MODE_GBIME_SYMBOLS,			1, {LayoutIdWL9JNumber}},
	{IMM_INPUT_MODE_GBIME_FULLSCREEN_HANDWRITE,	1, {LayoutIdWL9JPinYin}},

	// 公共
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
 * \brief 根据平台输入法模式获取目标Layout ID
 * \param platformInputMode
 * \param pLayoutID 
 * \return 
 * \note 当mapCount > 1时，优先选择国笔输入法配置缺省输入法应该指定的LayoutID
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

			PT_Assert(pInputMethodMap[i].mapCount > 0); // 如果不成立请检查映射表

			for (j = 0; j < pInputMethodMap[i].mapCount; j++)
			{
				if (preferLayoutID == pInputMethodMap[i].layoutIDArray[j])
				{
					*pLayoutID = preferLayoutID; // 映射表中存在配置指定的缺省LayoutID则优先选用
					return GBIME_OK;
				}			
			}

			// 映射表中不存在配置指定的缺省LayoutID则选用第一个
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
 * \brief 根据目标切换Layout 的ID找到对应的平台输入法模式
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
 * \brief 根据当前Layout ID获取下一个Layout ID
 * \param nCurLayoutID
 * \param pNextLayoutID 
 * \return 
 * \note 当mapCount > 1时，优先选择国笔输入法配置缺省输入法应该指定的LayoutID
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


#if __MTK__ > 0 // MTK平台参考函数

/*!
 * \brief 手写触摸处理
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
 * \brief 高亮候选
 * \param GBUACHAR 
 * \return void
 */
GBIMEGLOBAL void GBPA_ChangeHighlightedCandidateFromCandidateBox(GBUACHAR candidate_index)
{
	mmi_imc_change_highlighted_candidate_from_candidate_box(candidate_index);
}

/*!
 * \brief multitap的字母up处理
 * \param mmi_imc_key_custom_value,GBU16 
 * \return GBI
 * \注意：mmi_imc_key_multitap_alphabetic_up_handler在imc_key中是static的，要把核心代码copy到这里。
 */
GBIMEGLOBAL GBI GBPA_KeyMultitapAlphabeticUpHandler(mmi_imc_key_custom_value key_code, GBU16 event_type)
{
	//return mmi_imc_key_multitap_alphabetic_up_handler(key_code,event_type);
	return 0;
}

/*!
 * \brief multitap的字母down处理
 * \param mmi_imc_key_custom_value,GBU16 
 * \return GBI
 * \注意：mmi_imc_key_multitap_alphabetic_down_handler在imc_key中是static的，要把核心代码copy到这里。
 */
GBIMEGLOBAL GBI GBPA_KeyMultitapAlphabeticDownHandler(mmi_imc_key_custom_value key_code, GBU16 event_type)
{
	//return mmi_imc_key_multitap_alphabetic_down_handler(key_code,event_type);
	return 0;
}

/*!
 * \brief 全屏手写
 * \param void
 * \return void
 * \注意：
 */
GBIMEGLOBAL void GBPA_EditorEnableFullScreenHandwriting(void)
{
	//mmi_pen_editor_enable_full_screen_handwriting();
}

/*!
 * \brief 设置状态
 * \param mmi_imc_state_enum
 * \return void
 * \注意：
 */
GBIMEGLOBAL void GBPA_SetState( mmi_imc_state_enum state)
{
	mmi_imc_set_state(state);
}
 
/*!
 * \brief 清理手写
 * \param 
 * \return void
 * \注意：
 */
GBIMEGLOBAL GBU32 GBPA_ClearHandWritingStroke(void)
{
	//return mmi_imui_clear_hand_writing_stroke();
	return 0;
}

/*!
* \brief 进入新屏幕
* \param GBWCHAR, GbFuncPtr,GbFuncPtr,GBPVOID
* \param rect 
* \return GBUACHAR
*/
GBIMEGLOBAL GBUACHAR GBPA_EntryNewScreen(GBWCHAR newscrnID, GbFuncPtr newExitHandler, GbFuncPtr newEntryHandler, GBPVOID flag)
{
	return EntryNewScreen(newscrnID,newExitHandler,newEntryHandler,flag);
}
#endif // __MTK__


