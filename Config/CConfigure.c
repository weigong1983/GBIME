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
* Purpose      :  配置实现文件
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
 * \brief 每种键盘类型下的缺省LayoutID定制表
 */
typedef struct _tagKeyboardLayoutIDMap
{
	KEYBOARD_TYPE       keyboardType;	///<平台输入模式
	GBUINT16			preferLayoutID; ///<平台输入模式能映射多个国笔输入法模式
	GBUINT16			defLatestAlpLayoutID; ///<默认的最近拉丁LayoutID
	GBUINT16			defLatestChnLayoutID; ///<默认的最近中文LayoutID
} KeyboardLayoutIDMap, * PKeyboardLayoutIDMap;
static const KeyboardLayoutIDMap DefaultLayoutIDList[KEYBOARD_NUM] = 
{
	{KEYBOARD_VK_NUMPAD,	LayoutIdSP9JPinYin,		LayoutIdSP9JLowerEnglish,	LayoutIdSP9JPinYin}		// 数字键盘
	,{KEYBOARD_VK_QWERTY,	LayoutIdSP26JPinYin,	LayoutIdSP26JLowerEnglish,	LayoutIdSP26JPinYin}	// Qwerty键盘
	,{KEYBOARD_KB_NUMPAD,	LayoutIdWL9JPinYin,		LayoutIdEnd,	LayoutIdEnd}	// 物理键盘9键
	,{KEYBOARD_KB_QWERTY,	LayoutIdWL26JPinYin,	LayoutIdEnd,	LayoutIdEnd}	// 物理键盘9键
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

	PT_Assert(i < KEYBOARD_NUM); // 请更新DefaultLayoutIDList映射表
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
 * \brief 加载国笔引擎配置
 * \param  pConfig
 * \return 
 * \note 
 * \example
 * \author fanbo
 * \date
 */
GBIMEReturn  GBConfig_Load(CConfig *pConfig)
{
	// 初始化引擎配置
// 	InitEngineConfig(&pConfig->engineConfig);
// 	SelectProduce(&pConfig->engineConfig, PRODUCE_NUMPAD_1);
	
	//选择产品
	SelectProduct(PRODUCE_NUMPAD_1);


	// 初始化Layout配置
	pConfig->bHasInit = GBTrue;
	pConfig->m_bSupportSlidingSwitchLayout = GBTrue;

#if defined(__GBIME_VK__) // 国笔虚拟键盘
	pConfig->m_keyboardType = KEYBOARD_VK_NUMPAD;
	pConfig->m_preferLayoutID = IGBConfig_GetPreferLayoutID(KEYBOARD_VK_NUMPAD);
#else // 物理键盘
	pConfig->m_keyboardType = KEYBOARD_KB_NUMPAD;
	pConfig->m_preferLayoutID = IGBConfig_GetPreferLayoutID(KEYBOARD_KB_NUMPAD);
#endif

	pConfig->m_respondKeyType = RESPOND_KEY_UP;
	return GBIME_OK;
}

/*!
 * \brief 卸载国笔引擎配置
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
 * \brief 配置消息处理函数
 * \param pIMEEvent 
 * \return 
 */
GBIMEReturn GBConfig_HandleMessage(PGBIMEEvent pIMEEvent)
{
	GBIMEReturn ret = GBIME_OK;

	if (pIMEEvent->nType != GBIME_EVENT_TYPE_CONFIG)
	{
		return GBIME_IGNORE; // 非配置事件，忽略
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
		PT_Assert(0); // 非法事件
	}

	return ret;
}

/*!
 * \brief 设置键盘类别
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
		// 设置键盘类别
		pConfig->m_keyboardType = keyboardType;
		// 更新当前键盘类型下的缺省LayoutID
		pConfig->m_preferLayoutID = IGBConfig_GetPreferLayoutID(keyboardType);
		SelectProduct(IGBConfig_GetProductIDByKeyboardType(keyboardType));
	}
	
	return GBIME_OK;
}

/*!
 * \brief 设置键盘类别
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
 * \brief 获取缺省LayoutID
 * \param pConfig 
 * \return 缺省LayoutID
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
 * \brief 获取默认的最近拉丁LayoutID
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

	PT_Assert(i < KEYBOARD_NUM); // 请更新DefaultLayoutIDList映射表
	return LayoutIdEnd;
}

/*!
 * \brief 默认的最近拉丁LayoutID
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

	PT_Assert(i < KEYBOARD_NUM); // 请更新DefaultLayoutIDList映射表
	return LayoutIdEnd;
}