/***************************************************************************
* File Name    :  CConfigMenu.c
* Corporation  :  Guobi Technology Ltd. GuangZhou China
* Copyright    :  Copyright Guobi Technology Ltd. And Subject to Confidentiality 
*                 Restrictions. This document is the confidential and proprietary 
*                 property of Guobi Technology Ltd.It is neither the document                  
*                 itself nor any part of it can be shared with any party  without 
*                 the written  permission. 
* Author       :  Zhuhaian
* Date         :  2010-08-05
* Purpose      :  设置键菜单文件
* Version      :  V1.0
* Last Changed List
* Name         |   date   |  Discription
****************************************************************************/
#include "CConfigMenu.h"
#include "PlatformTools.h"
#include "GB_Global.h"
#include "CProductSetting.h"

#if __MTK__ > 0
#include "CommonScreens.h"
#include "GlobalMenuItems.h"
#endif
void SelectListHandler(void);
void SelectRadioHandler(void);
void GoBackMenu(void);
void EntryGbSetUpScreen(void);
void SelectRadioHandler(void);
void ExitReadOnlyTextDlg(void);
void CreateCheckMenu(GBUINT16 curStrID, GBUINT16* pMenuStrID, void* pMenuValue, GBUINT16 nMenuCount);
void CreateRadioMenu(GBUINT16 curStrID, GBUINT16* pMenuStrID, void* pMenuValue, GBUINT16 nMenuCount);
void CreateListMenu(GBUINT16 curStrID, GBUINT16* pMenuStrID, void* pMenuValue, GBUINT16 nMenuCount, GBUINT16 nHighlight);
void highlight_radio_menu(GBINT nIndex);
void highlight_list_menu(GBINT nIndex);
void GBSettiingSaveConfirm(void);
void GBSettiingSave(void);
GBUINT16 GetCurMenuStrID(GBINT nIndex);

extern const GBU16 gIndexIconsImageList[];
GBUINT8 pTempConfigCheckBoxValue[MAX_SUB_MENUS] = {0};//记录CheckBox状态值
GBUINT16 pTempConfigCheckBoxStrID[MAX_SUB_MENUS] = {0};//记录菜单ID
GBUINT16 g_CurConfigMenuCount = 0;//记录当前屏幕菜单个数
GBUINT16 g_CurKeyboardIndex = 0;//记录键盘菜单中当前键盘下标
GBUINT16 g_CurHighlightKeyboardIndex = 0;//记录键盘菜单中当前高亮的下标
GBUINT16 g_CurRadioIndex;		//记录当前单选菜单下标
GBUINT16 g_CurTempRadioIndex;	//记录当前高亮的单选菜单下标
GBUINT16 g_CurListIndex;		//记录当前菜单列表下表
GBUINT16 g_CurStrIDList[8] = {0};	//记录当前菜单链，从根目录到子目录到各选项
GBUINT16 g_CurHighlightList[8] = {0}; //记录每级目录高亮的下标
GBUINT16 g_CurStrIDListIndex = 0;	//记录当前显示的菜单下标


// 
// void ResetMenu()
// {
// 	memset(pTempConfigCheckBoxValue, 0, sizeof(pTempConfigCheckBoxValue));
// 	memset(pTempConfigCheckBoxStrID, 0, sizeof(pTempConfigCheckBoxStrID));
// 
// 	g_CurConfigMenuCount = 0;//记录当前屏幕菜单个数
// 	g_CurKeyboardIndex = 0;//记录键盘菜单中当前键盘下标
// 	g_CurHighlightKeyboardIndex = 0;//记录键盘菜单中当前高亮的下标
// 	g_CurRadioIndex = 0;		//记录当前单选菜单下标
// 	g_CurTempRadioIndex = 0;	//记录当前高亮的单选菜单下标
// 	g_CurListIndex = 0;		//记录当前菜单列表下表
// 	memset(g_CurStrIDList, 0, sizeof(g_CurStrIDList));
// 	memset(g_CurHighlightList, 0, sizeof(g_CurHighlightList));
// 	g_CurStrIDListIndex = 0;	//记录当前显示的菜单下标
// }


CProductMenuData g_CurKeyboardData[] = 
{
	{KEYBOARD_VK_NUMPAD,	STR_ID_GUOBI_KEYBOARD_NUMPAD},
	{KEYBOARD_VK_QWERTY,	STR_ID_GUOBI_KEYBOARD_QWERTY},
};

 /*!
 * \brief 根据下标从当前记录的菜单字符串中取得id
 * \param
 * \return
 * \note 
 * \author Zhaokun
 * \date 2010年9月10日
 */
GBUINT16 GetCurMenuStrID(GBINT nIndex)
{	
	GBUINT16 curMenuStrID;

	if (nIndex < 0 && nIndex >= g_CurConfigMenuCount)
	{
		curMenuStrID = (GBUINT16)-1;
	}
	else
	{
		curMenuStrID = pTempConfigCheckBoxStrID[nIndex];
	}
	return curMenuStrID;
}

/*!
 * \brief 保存国笔输入法设置
 * \param
 * \return
 * \note 
 * \author ZhaoKun
 * \date 2010年9月10日
 */
void GBSettiingSave(void)
{
	GBINT i = 0;
	for (i = 0; i < g_CurConfigMenuCount; i++)
	{
		//设置缓存中记录的配置值
		SetConfigValueForMenuStrID(pTempConfigCheckBoxStrID[i], &pTempConfigCheckBoxValue[i], 0);
		//根据配置值来设置引擎配置
		SetEngineForStrID(pTempConfigCheckBoxStrID[i], &pTempConfigCheckBoxValue[i], 0);
	}
	//根据缓存，保存配置到文件
	SaveConfigFile(GetCurProductID());

	//DisplayPopup((PU8) GetString(STR_GLOBAL_SAVED), IMG_GLOBAL_SAVE, TRUE, UI_POPUP_NOTIFYDURATION_TIME, SUCCESS_TONE);
	//DeleteNScrId(STR_ID_GUOBI_CHN_FUZZY_SETTING);
	GoBackMenu();
}
/*!
 * \brief 弹出国笔输入法设置保存提示框
 * \param
 * \return
 * \note 
 * \author ZhaoKun
 * \date 2010年9月10日
 */
void GBSettiingSaveConfirm(void)
{
#if __MTK__ > 0
//     DisplayConfirm(
//         STR_GLOBAL_YES,
//         IMG_GLOBAL_YES,
//         STR_GLOBAL_NO,
//         IMG_GLOBAL_NO,
//         get_string(STR_GLOBAL_SAVE_ASK),
//         IMG_GLOBAL_QUESTION,
//         WARNING_TONE);

    PT_SetLeftSoftkeyFunction(GBSettiingSave, KEY_EVENT_UP);
    PT_SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
#endif
}

/*!
 * \brief 菜单列表的高亮
 * \param
 * \return
 * \note 
 * \author ZhaoKun
 * \date 2010年9月10日
 */
void highlight_list_menu(GBINT nIndex)
{
	g_CurListIndex = nIndex;
}

/*!
 * \brief 单选菜单的高亮
 * \param
 * \return
 * \note 
 * \author ZhaoKun
 * \date 2010年9月10日
 */
void highlight_radio_menu(GBINT nIndex)
{
	g_CurTempRadioIndex = nIndex;
}


/*!
 * \brief 选择默认输入键盘
 * \param nStrID 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-12-3 14:29:45
 */
static void SelectKeyboardTypeHandler(GBUINT16 nStrID)
{
	//GBINT productID = 0;
	KEYBOARD_TYPE keyboardType =  KEYBOARD_VK_NUMPAD;

	switch (nStrID)
	{
	case STR_ID_GUOBI_KEYBOARD_NUMPAD:
		//productID = PRODUCE_NUMPAD_1;
		keyboardType = KEYBOARD_VK_NUMPAD;			
		break;
	case STR_ID_GUOBI_KEYBOARD_QWERTY:
		//productID = PRODUCE_EXPLICIT_1;
		keyboardType = KEYBOARD_VK_QWERTY;
		break;
	default:
		PT_Assert(0);
	}
	GBConfig_SetKeyboardType(Global_GetConfigInstance(), keyboardType);
}

/*!
 * \brief 选择默认输入模式
 * \param nStrID 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-12-3 14:29:56
 */
static void SelectInputModeHandler(GBUINT16 nStrID)
{
// 	switch (nStrID)
// 	{
// 	case STR_ID_GUOBI_IM_SMART_PINYIN: //智能拼音
// 	case STR_ID_GUOBI_IM_SMART_STROKE: //智能笔画
// 	case STR_ID_GUOBI_IM_SMART_ENG: //智能英文
// 	case STR_ID_GUOBI_IM_SMART_HANDWRITING: //智能手写
// 	default:
// 		PT_Assert(0);
// 	}
// 	mmi_imc_change_input_mode(desired_mode, 0);
}

/*!
 * \brief 单选菜单的选择
 * \param
 * \return
 * \note 
 * \author ZhaoKun
 * \date 2010年9月10日
 */
void SelectRadioHandler(void)
{
	GBUINT16 nCurSelCheckBoxStrID;

	g_CurRadioIndex = g_CurTempRadioIndex;
	nCurSelCheckBoxStrID = pTempConfigCheckBoxStrID[g_CurRadioIndex];

	if (g_CurRadioIndex < g_CurConfigMenuCount && nCurSelCheckBoxStrID)
	{
		if (nCurSelCheckBoxStrID >= STR_ID_GUOBI_KEYBOARD_NUMPAD && nCurSelCheckBoxStrID <= STR_ID_GUOBI_KEYBOARD_QWERTY)
		{
			// 选择默认输入键盘
			SelectKeyboardTypeHandler(nCurSelCheckBoxStrID);
		}
		else if (nCurSelCheckBoxStrID >= STR_ID_GUOBI_IM_SMART_PINYIN && nCurSelCheckBoxStrID <= STR_ID_GUOBI_IM_SMART_HANDWRITING)
		{
			// 选择默认输入模式
			SelectInputModeHandler(nCurSelCheckBoxStrID);
		}
		else
		{
		}
	}
	
	//PT_DisplayPopupMessageBox(STR_GLOBAL_DONE, 500);

// 	RedrawCategoryFunction();
// 	DisplayPopup((PU8) GetString(STR_GLOBAL_DONE), (U16)IMG_GLOBAL_ACTIVATED, TRUE, 500, (U8)SUCCESS_TONE);
 	GoBackMenu();
}

/*!
 * \brief 创建菜单列表
 * \param
 * \return
 * \note 
 * \author ZhaoKun
 * \date 2010年9月10日
 */
void CreateListMenu(GBUINT16 curStrID, GBUINT16* pMenuStrID, void* pMenuValue, GBUINT16 nMenuCount, GBUINT16 nHighlight)
{
#if __MTK__ > 0
	/*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    GBUINT8 *guiBuffer = NULL;              /* Buffer holding history data */  
    //GBUINT16 nNumofItem = 0;                     /* Stores no of children in the submenu */
	
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
	
	PT_ClearScreen();
	
	PT_RegisterHighlightHandler(highlight_list_menu);	
	
	PT_ShowListScreen(curStrID,
   					0,//NULL,
					STR_GLOBAL_OK,
					IMG_GLOBAL_OK,
					STR_GLOBAL_BACK,
					IMG_GLOBAL_BACK,
					nMenuCount,
					pMenuStrID,
					(GBUINT16*) gIndexIconsImageList,
					NULL,
					0,
					nHighlight,
					guiBuffer
					);
	
	PT_SetLeftSoftkeyFunction(SelectListHandler, KEY_EVENT_UP);
    PT_SetRightSoftkeyFunction(GoBackMenu, KEY_EVENT_UP);
    PT_SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
	
#endif //__MTK__
	
}

/*!
 * \brief 创建单选菜单
 * \param
 * \return
 * \note 
 * \author ZhaoKun
 * \date 2010年9月10日
 */
void CreateRadioMenu(GBUINT16 curStrID, GBUINT16* pMenuStrID, void* pMenuValue, GBUINT16 nMenuCount)
{
#if __MTK__ > 0
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
	GBUINT8 *guiBuffer = NULL;              /* Buffer holding history data */
			
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
  
	PT_ClearScreen();
	
	PT_RegisterHighlightHandler(highlight_radio_menu);

	PT_ShanSelectionsFrameScreen(curStrID,
								0,//NULL,
								STR_GLOBAL_OK,
								IMG_GLOBAL_OK,
								STR_GLOBAL_BACK,
								IMG_GLOBAL_BACK,
								nMenuCount,
								pMenuStrID,
								g_CurRadioIndex,
								guiBuffer); 
	
	PT_SetLeftSoftkeyFunction(SelectRadioHandler, KEY_EVENT_UP);
	// PT_SetLeftSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);  //GoBackHistory改为左键入口函数
    PT_SetKeyHandler(GoBackHistory, KEY_RIGHT_ARROW, KEY_EVENT_DOWN); //GoBackHistory改为左键入口函数
    PT_SetRightSoftkeyFunction(GoBackMenu, KEY_EVENT_UP);
    PT_SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
   
#endif //__MTK__		
}

/*!
 * \brief 创建复选菜单
 * \param
 * \return
 * \note 
 * \author ZhaoKun
 * \date 2010年9月10日
 */
void CreateCheckMenu(GBUINT16 curStrID, GBUINT16* pMenuStrID, void* pMenuValue, GBUINT16 nMenuCount)
{
#if __MTK__ > 0
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
	GBUINT8 *guiBuffer = NULL;              /* Buffer holding history data */
	
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
	PT_ClearScreen();
	
	PT_ReelectionFrameScreen(curStrID,
		0,//NULL,
		nMenuCount,
		pMenuStrID,
		pMenuValue,
		0,
		guiBuffer);
	
	PT_SetCheckboxToggleRightSoftkeyFunctions(GBSettiingSave, GoBackHistory);
	//SetCheckboxToggleRightSoftkeyFunctions(GBSettiingSaveConfirm, GoBackHistory);
    //SetLeftSoftkeyFunction(SelectFuzzyConfig, KEY_EVENT_UP);  //GoBackHistory改为左键入口函数
	
    PT_SetKeyHandler(GoBackHistory, KEY_RIGHT_ARROW, KEY_EVENT_DOWN); //GoBackHistory改为左键入口函数
    PT_SetRightSoftkeyFunction(GoBackMenu, KEY_EVENT_UP);
    PT_SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
	
#endif //__MTK__	
}
/*!
 * \brief 退出只读的文本窗口
 * \return
 * \note 
 * \author ZhaoKun
 * \date 2010年11月23日
 */
void ExitReadOnlyTextDlg(void)
{
	PT_ExitReadOnlyScreen();
	GoBackMenu();
}
/*!
 * \brief 创建只读的文本窗口
 * \param
 * \return
 * \note 
 * \author ZhaoKun
 * \date 2010年9月10日
 */
void CreateReadOnlyTextDlg(GBUINT16 curStrID, GBU8 *buffer, GBINT buffer_length)
{
#if __MTK__ > 0

//	GBPA_EntryNewScreen(0, NULL, NULL, NULL);

	
	
	PT_ClearScreen();

	PT_ReadOnlyTextFrameScreen(
		curStrID,
		0,
		0,
		0,
		STR_GLOBAL_BACK,
		IMG_GLOBAL_BACK,
		//(U8*) L"使用0进入设置功能。\n使用1进入帮助。\n使用2添加书签。\n使用3进入寻找页面。\n使用4进入跳页功能。\n\n使用5进入书签。\n使用6寻找下一个。\n使用7切换自动卷页。\n使用8删除书签。\n使用9切换全屏幕模式。",
		buffer,
		buffer_length,
        NULL);
	
	PT_SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
	PT_SetRightSoftkeyFunction(ExitReadOnlyTextDlg, KEY_EVENT_UP);
#endif //__MTK__
}
/*!
 * \brief 返回上一级目录
 * \param
 * \return
 * \note 
 * \author ZhaoKun
 * \date 2010年9月10日
 */
void GoBackMenu(void)
{
	GBINT chdMenuType;
	GBUINT16 strID;
	GBINT index;
	GBUINT16 curParentHighlight = 0;

	index = g_CurStrIDListIndex - 2;

	if (index < 0)
	{
		return ;
	}
	else if (index == 0)
	{
		g_CurStrIDListIndex = 0;
	//	if(ST_SUCCESS == DeleteUptoScrID(MENU_ID_GUOBI_SETTING))
		{
		//	DeleteNHistory(0);
			EntryGbSetUpScreen();
		}
		
		return ;
	}
	else
	{
		g_CurStrIDListIndex--;
		strID = g_CurStrIDList[index];
		curParentHighlight = g_CurHighlightList[g_CurStrIDListIndex];
	}
	
	chdMenuType = GetConfigStrID(strID, pTempConfigCheckBoxStrID, pTempConfigCheckBoxValue, &g_CurConfigMenuCount);
	
	switch(chdMenuType)
	{
	case LIST_TYPE:		
		CreateListMenu(strID, pTempConfigCheckBoxStrID, pTempConfigCheckBoxValue, g_CurConfigMenuCount, curParentHighlight);
		break;
	case CHECK_TYPE:
		CreateCheckMenu(strID, pTempConfigCheckBoxStrID, pTempConfigCheckBoxValue, g_CurConfigMenuCount);
		break;
	case RADIO_TYPE:
		CreateRadioMenu(strID, pTempConfigCheckBoxStrID, pTempConfigCheckBoxValue, g_CurConfigMenuCount);
		break;
	}	
}

/*!
 * \brief 选择菜单列表
 * \param
 * \return
 * \note 
 * \author ZhaoKun
 * \date 2010年9月10日
 */
void SelectListHandler(void)
{
	GBUINT16 curMenuStrID;
	GBINT chdMenuType;
	GBPCWCHAR pTextInfo = NULL;

	curMenuStrID = GetCurMenuStrID(g_CurListIndex);

	chdMenuType = GetConfigStrID(curMenuStrID, pTempConfigCheckBoxStrID, pTempConfigCheckBoxValue, &g_CurConfigMenuCount);
	
	if (!g_CurConfigMenuCount)
	{
		chdMenuType = GetCurMenuTypeToStrID(curMenuStrID);
		g_CurListIndex = 0;
	}

	g_CurStrIDList[g_CurStrIDListIndex] = curMenuStrID;
	g_CurHighlightList[g_CurStrIDListIndex] = g_CurListIndex;
	g_CurStrIDListIndex++;

	switch(chdMenuType)
	{
	case LIST_TYPE:
		CreateListMenu(curMenuStrID, pTempConfigCheckBoxStrID, pTempConfigCheckBoxValue, g_CurConfigMenuCount, 0);//g_CurListIndex);
		break;
	case CHECK_TYPE:
		CreateCheckMenu(curMenuStrID, pTempConfigCheckBoxStrID, pTempConfigCheckBoxValue, g_CurConfigMenuCount);
		break;
	case RADIO_TYPE:
		CreateRadioMenu(curMenuStrID, pTempConfigCheckBoxStrID, pTempConfigCheckBoxValue, g_CurConfigMenuCount);
		break;
	case ONLY_TEXT_DLG_TYPE:
		pTextInfo = GetTextInfo(curMenuStrID);
		CreateReadOnlyTextDlg(curMenuStrID, (GBU8*)pTextInfo, PT_wcslen(pTextInfo));
		break;		
	}
}
/*!
 * \brief 进入设置键屏幕
 * \param  void
 * \return  void
 * \note 
 * \example
 * \author Zhuhaian
 * \date
 */
void EntryGbSetUpScreen(void)
{
#if __MTK__ > 0
	    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    GBUINT8 *guiBuffer = NULL;              /* Buffer holding history data */
    GBUINT16 nStrItemList[MAX_SUB_MENUS];    /* Stores the strings id of submenus returned */
    GBUINT16 nNumofItem = 0;                     /* Stores no of children in the submenu */
    GBUINT16 nDispAttribute; 
    GBUINT8 *ItemType[MAX_SUB_MENUS];

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/

//	ResetMenu();

    GBPA_EntryNewScreen(MENU_ID_GUOBI_SETTING, NULL, NULL, NULL);

	PT_ClearScreen();
	
	PT_CreateNewScreen(SCR_ID_GUOBI_SETTING,
		MENU_ID_GUOBI_SETTING,
		guiBuffer,
		&nNumofItem,
		&nDispAttribute,
		nStrItemList,
		ItemType);
	
	PT_RegisterHighlightHandler(highlight_list_menu);

	memset(g_CurStrIDList, 0, sizeof(g_CurStrIDList));
	g_CurStrIDListIndex = 1;

	GetConfigStrID(0, pTempConfigCheckBoxStrID, pTempConfigCheckBoxValue, &g_CurConfigMenuCount);

	PT_ShowListScreen(STR_ID_GUOBI_SETTING,
   					0,//NULL,
					STR_GLOBAL_OK,
					IMG_GLOBAL_OK,
					STR_GLOBAL_BACK,
					IMG_GLOBAL_BACK,
					g_CurConfigMenuCount,
					pTempConfigCheckBoxStrID,
					(GBUINT16*) gIndexIconsImageList,
					NULL,
					0,
					g_CurHighlightList[g_CurStrIDListIndex],
					guiBuffer
					);	

	PT_SetLeftSoftkeyFunction(SelectListHandler, KEY_EVENT_UP);	
    PT_SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
    PT_SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);

#endif //__MTK__

}

/*----------------------------------------------------------------*/
/* 键盘设置菜单相关                                               */
/*----------------------------------------------------------------*/
static GBUINT16 GetKeyboardIDByIndex(GBUINT8 index)
{	
	PT_Assert(/*index >= nZero && */index < PT_ARRAY_SIZE(g_CurKeyboardData));
	return g_CurKeyboardData[index].id;
}

static GBUINT8 GetKeyboardIndexByID(GBUINT8 id)
{
	GBUINT8 index = 0;

	for (index = 0; index < PT_ARRAY_SIZE(g_CurKeyboardData); index++)
	{
		if(g_CurKeyboardData[index].id == id)
		{
			return index;
		}
	}

	return (GBUINT8)-1;
}

