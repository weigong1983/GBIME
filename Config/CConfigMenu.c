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
* Purpose      :  ���ü��˵��ļ�
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
GBUINT8 pTempConfigCheckBoxValue[MAX_SUB_MENUS] = {0};//��¼CheckBox״ֵ̬
GBUINT16 pTempConfigCheckBoxStrID[MAX_SUB_MENUS] = {0};//��¼�˵�ID
GBUINT16 g_CurConfigMenuCount = 0;//��¼��ǰ��Ļ�˵�����
GBUINT16 g_CurKeyboardIndex = 0;//��¼���̲˵��е�ǰ�����±�
GBUINT16 g_CurHighlightKeyboardIndex = 0;//��¼���̲˵��е�ǰ�������±�
GBUINT16 g_CurRadioIndex;		//��¼��ǰ��ѡ�˵��±�
GBUINT16 g_CurTempRadioIndex;	//��¼��ǰ�����ĵ�ѡ�˵��±�
GBUINT16 g_CurListIndex;		//��¼��ǰ�˵��б��±�
GBUINT16 g_CurStrIDList[8] = {0};	//��¼��ǰ�˵������Ӹ�Ŀ¼����Ŀ¼����ѡ��
GBUINT16 g_CurHighlightList[8] = {0}; //��¼ÿ��Ŀ¼�������±�
GBUINT16 g_CurStrIDListIndex = 0;	//��¼��ǰ��ʾ�Ĳ˵��±�


// 
// void ResetMenu()
// {
// 	memset(pTempConfigCheckBoxValue, 0, sizeof(pTempConfigCheckBoxValue));
// 	memset(pTempConfigCheckBoxStrID, 0, sizeof(pTempConfigCheckBoxStrID));
// 
// 	g_CurConfigMenuCount = 0;//��¼��ǰ��Ļ�˵�����
// 	g_CurKeyboardIndex = 0;//��¼���̲˵��е�ǰ�����±�
// 	g_CurHighlightKeyboardIndex = 0;//��¼���̲˵��е�ǰ�������±�
// 	g_CurRadioIndex = 0;		//��¼��ǰ��ѡ�˵��±�
// 	g_CurTempRadioIndex = 0;	//��¼��ǰ�����ĵ�ѡ�˵��±�
// 	g_CurListIndex = 0;		//��¼��ǰ�˵��б��±�
// 	memset(g_CurStrIDList, 0, sizeof(g_CurStrIDList));
// 	memset(g_CurHighlightList, 0, sizeof(g_CurHighlightList));
// 	g_CurStrIDListIndex = 0;	//��¼��ǰ��ʾ�Ĳ˵��±�
// }


CProductMenuData g_CurKeyboardData[] = 
{
	{KEYBOARD_VK_NUMPAD,	STR_ID_GUOBI_KEYBOARD_NUMPAD},
	{KEYBOARD_VK_QWERTY,	STR_ID_GUOBI_KEYBOARD_QWERTY},
};

 /*!
 * \brief �����±�ӵ�ǰ��¼�Ĳ˵��ַ�����ȡ��id
 * \param
 * \return
 * \note 
 * \author Zhaokun
 * \date 2010��9��10��
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
 * \brief ����������뷨����
 * \param
 * \return
 * \note 
 * \author ZhaoKun
 * \date 2010��9��10��
 */
void GBSettiingSave(void)
{
	GBINT i = 0;
	for (i = 0; i < g_CurConfigMenuCount; i++)
	{
		//���û����м�¼������ֵ
		SetConfigValueForMenuStrID(pTempConfigCheckBoxStrID[i], &pTempConfigCheckBoxValue[i], 0);
		//��������ֵ��������������
		SetEngineForStrID(pTempConfigCheckBoxStrID[i], &pTempConfigCheckBoxValue[i], 0);
	}
	//���ݻ��棬�������õ��ļ�
	SaveConfigFile(GetCurProductID());

	//DisplayPopup((PU8) GetString(STR_GLOBAL_SAVED), IMG_GLOBAL_SAVE, TRUE, UI_POPUP_NOTIFYDURATION_TIME, SUCCESS_TONE);
	//DeleteNScrId(STR_ID_GUOBI_CHN_FUZZY_SETTING);
	GoBackMenu();
}
/*!
 * \brief �����������뷨���ñ�����ʾ��
 * \param
 * \return
 * \note 
 * \author ZhaoKun
 * \date 2010��9��10��
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
 * \brief �˵��б�ĸ���
 * \param
 * \return
 * \note 
 * \author ZhaoKun
 * \date 2010��9��10��
 */
void highlight_list_menu(GBINT nIndex)
{
	g_CurListIndex = nIndex;
}

/*!
 * \brief ��ѡ�˵��ĸ���
 * \param
 * \return
 * \note 
 * \author ZhaoKun
 * \date 2010��9��10��
 */
void highlight_radio_menu(GBINT nIndex)
{
	g_CurTempRadioIndex = nIndex;
}


/*!
 * \brief ѡ��Ĭ���������
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
 * \brief ѡ��Ĭ������ģʽ
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
// 	case STR_ID_GUOBI_IM_SMART_PINYIN: //����ƴ��
// 	case STR_ID_GUOBI_IM_SMART_STROKE: //���ܱʻ�
// 	case STR_ID_GUOBI_IM_SMART_ENG: //����Ӣ��
// 	case STR_ID_GUOBI_IM_SMART_HANDWRITING: //������д
// 	default:
// 		PT_Assert(0);
// 	}
// 	mmi_imc_change_input_mode(desired_mode, 0);
}

/*!
 * \brief ��ѡ�˵���ѡ��
 * \param
 * \return
 * \note 
 * \author ZhaoKun
 * \date 2010��9��10��
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
			// ѡ��Ĭ���������
			SelectKeyboardTypeHandler(nCurSelCheckBoxStrID);
		}
		else if (nCurSelCheckBoxStrID >= STR_ID_GUOBI_IM_SMART_PINYIN && nCurSelCheckBoxStrID <= STR_ID_GUOBI_IM_SMART_HANDWRITING)
		{
			// ѡ��Ĭ������ģʽ
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
 * \brief �����˵��б�
 * \param
 * \return
 * \note 
 * \author ZhaoKun
 * \date 2010��9��10��
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
 * \brief ������ѡ�˵�
 * \param
 * \return
 * \note 
 * \author ZhaoKun
 * \date 2010��9��10��
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
	// PT_SetLeftSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);  //GoBackHistory��Ϊ�����ں���
    PT_SetKeyHandler(GoBackHistory, KEY_RIGHT_ARROW, KEY_EVENT_DOWN); //GoBackHistory��Ϊ�����ں���
    PT_SetRightSoftkeyFunction(GoBackMenu, KEY_EVENT_UP);
    PT_SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
   
#endif //__MTK__		
}

/*!
 * \brief ������ѡ�˵�
 * \param
 * \return
 * \note 
 * \author ZhaoKun
 * \date 2010��9��10��
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
    //SetLeftSoftkeyFunction(SelectFuzzyConfig, KEY_EVENT_UP);  //GoBackHistory��Ϊ�����ں���
	
    PT_SetKeyHandler(GoBackHistory, KEY_RIGHT_ARROW, KEY_EVENT_DOWN); //GoBackHistory��Ϊ�����ں���
    PT_SetRightSoftkeyFunction(GoBackMenu, KEY_EVENT_UP);
    PT_SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
	
#endif //__MTK__	
}
/*!
 * \brief �˳�ֻ�����ı�����
 * \return
 * \note 
 * \author ZhaoKun
 * \date 2010��11��23��
 */
void ExitReadOnlyTextDlg(void)
{
	PT_ExitReadOnlyScreen();
	GoBackMenu();
}
/*!
 * \brief ����ֻ�����ı�����
 * \param
 * \return
 * \note 
 * \author ZhaoKun
 * \date 2010��9��10��
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
		//(U8*) L"ʹ��0�������ù��ܡ�\nʹ��1���������\nʹ��2�����ǩ��\nʹ��3����Ѱ��ҳ�档\nʹ��4������ҳ���ܡ�\n\nʹ��5������ǩ��\nʹ��6Ѱ����һ����\nʹ��7�л��Զ���ҳ��\nʹ��8ɾ����ǩ��\nʹ��9�л�ȫ��Ļģʽ��",
		buffer,
		buffer_length,
        NULL);
	
	PT_SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
	PT_SetRightSoftkeyFunction(ExitReadOnlyTextDlg, KEY_EVENT_UP);
#endif //__MTK__
}
/*!
 * \brief ������һ��Ŀ¼
 * \param
 * \return
 * \note 
 * \author ZhaoKun
 * \date 2010��9��10��
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
 * \brief ѡ��˵��б�
 * \param
 * \return
 * \note 
 * \author ZhaoKun
 * \date 2010��9��10��
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
 * \brief �������ü���Ļ
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
/* �������ò˵����                                               */
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

