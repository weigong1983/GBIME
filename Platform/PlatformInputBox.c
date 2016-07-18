/***************************************************************************
* File Name    :  PlatformInputbox.c
* Corporation  :  Guobi Technology Ltd. GuangZhou China
* Copyright    :  Copyright Guobi Technology Ltd. And Subject to Confidentiality 
*                 Restrictions. This document is the confidential and proprietary 
*                 property of Guobi Technology Ltd.It is neither the document                  
*                 itself nor any part of it can be shared with any party  without 
*                 the written  permission. 
* Author       :  weizhiping
* Date         :  2010-09-03
* Purpose      :  封装MTK平台输入框相关功能函数
* Version      :  V1.0
* Last Changed List
* Name         |   date   |  Discription
****************************************************************************/
#include "gbim.h"
#include "GBIME.h"
#include "PlatformInputBox.h"
#include "PlatformTools.h"
// #include "CLayoutInfo.h"
#include "GB_Global.h"
#include "CGBEngine.h"

#if __MTK__ > 0// MTK平台依赖头文件
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
#endif // __MTK__

#if __MTK__ > 0
	//#ifdef __MMI_TOUCH_SCREEN__
	extern void mmi_pen_editor_handler_before_show_guobi(void);
	extern int mmi_pen_guobi_is_handle_writing(void);
	extern int mmi_pen_guobi_is_multiblock_hw(void);
	extern void mmi_pen_editor_guobi_touch_cand_show(void);
	extern int mmi_pen_guobi_is_vk_show(void);
	extern void mmi_pen_editor_guobi_vk_hide(void);
	extern int  mmi_pen_guobi_is_in_vk_cand(void);
	extern void wgui_hide_right_softkey(void);
	extern mmi_imm_input_mode_enum g_mmi_imm_current_input_mode;
	//#endif

	extern void (*g_imc_key_inputbox_LSK_function) (void);
	extern UI_EMS_input_box MMI_EMS_inputbox;
	extern void wgui_set_EMS_inputbox_RSK(void);
	extern void register_EMS_inputbox_keys(void);
	extern void wgui_hide_left_softkey(void);
	extern MMI_BOOL EMS_inputbox_multitap_input(UI_character_type c);
	extern void EMS_inputbox_multitap_input_complete(void);
	extern void redraw_EMS_inputbox(void);
	extern void mmi_imc_key_change_input_mode(void);
	extern void mmi_imc_key_change_input_mode_ex(mmi_imm_input_mode_enum input_mode, U32 reserved);
	extern MMI_BOOL EMS_inputbox_direct_input(UI_character_type c);
	extern void wgui_inline_edit_register_BACK_function(void);
	extern void GB_register_inline_fixed_list_keys(void);
	extern void inline_text_edit_set_RSK_function(void);

	typedef MMI_BOOL (*PF_MTKUpscreen)(UI_character_type c); ///<MTK上屏回调
#endif // __MTK__

typedef struct tagGBPlatformAppInfo
{
	GBINT inputBoxHistoryWidth;
	GBINT inputBoxHistoryHeight;
	UI_string_type left_softkey_label; ///<左软键显示标签
	UI_string_type right_softkey_label; ///<右软键显示标签	
} GBPlatformAppInfo;
static GBPlatformAppInfo g_PlatformAppInfo;

/*!
 * \brief 平台【数字'0'键】回调函数
 * \param void 
 * \return 
 */
GBIMELOCAL void GBInputBox_Key0PressHandlerForInputBox(void)
{
	GBIME_KeyDownHandler(GBKEY_0);
}
/*!
 * \brief 平台【数字'1'键】回调函数
 * \param void 
 * \return 
 */
GBIMELOCAL void GBInputBox_Key1PressHandlerForInputBox(void)
{
	GBIME_KeyDownHandler(GBKEY_1);
}
/*!
 * \brief 平台【数字'2'键】回调函数
 * \param void 
 * \return 
 */
GBIMELOCAL void GBInputBox_Key2PressHandlerForInputBox(void)
{
	GBIME_KeyDownHandler(GBKEY_2);
}
/*!
 * \brief 平台【数字'3'键】回调函数
 * \param void 
 * \return 
 */
GBIMELOCAL void GBInputBox_Key3PressHandlerForInputBox(void)
{
	GBIME_KeyDownHandler(GBKEY_3);
}
/*!
 * \brief 平台【数字'4'键】回调函数
 * \param void 
 * \return 
 */
GBIMELOCAL void GBInputBox_Key4PressHandlerForInputBox(void)
{
	GBIME_KeyDownHandler(GBKEY_4);
}
/*!
 * \brief 平台【数字'5'键】回调函数
 * \param void 
 * \return 
 */
GBIMELOCAL void GBInputBox_Key5PressHandlerForInputBox(void)
{
	GBIME_KeyDownHandler(GBKEY_5);
}
/*!
 * \brief 平台【数字'6'键】回调函数
 * \param void 
 * \return 
 */
GBIMELOCAL void GBInputBox_Key6PressHandlerForInputBox(void)
{
	GBIME_KeyDownHandler(GBKEY_6);
}
/*!
 * \brief 平台【数字'7'键】回调函数
 * \param void 
 * \return 
 */
GBIMELOCAL void GBInputBox_Key7PressHandlerForInputBox(void)
{
	GBIME_KeyDownHandler(GBKEY_7);
}
/*!
 * \brief 平台【数字'8'键】回调函数
 * \param void 
 * \return 
 */
GBIMELOCAL void GBInputBox_Key8PressHandlerForInputBox(void)
{
	GBIME_KeyDownHandler(GBKEY_8);
}
/*!
 * \brief 平台【数字'9'键】回调函数
 * \param void 
 * \return 
 */
GBIMELOCAL void GBInputBox_Key9PressHandlerForInputBox(void)
{
	GBIME_KeyDownHandler(GBKEY_9);
}

/*!
 * \brief 平台【上方向键】回调函数
 * \param void 
 * \return 
 */
GBIMELOCAL void GBInputBox_KeyArrowUpHandlerForInputBox(void)
{
	GBIME_KeyDownHandler(GBKEY_UP);
}

/*!
 * \brief 平台【下方向键】回调函数
 * \param void 
 * \return 
 */
GBIMELOCAL void GBInputBox_KeyArrowDownHandlerForInputBox(void)
{
	GBIME_KeyDownHandler(GBKEY_DOWN);
}

/*!
 * \brief 平台【左方向键】回调函数
 * \param void 
 * \return 
 */
GBIMELOCAL void GBInputBox_KeyArrowLeftHandlerForInputBox(void)
{
	GBIME_KeyDownHandler(GBKEY_LEFT);
}

/*!
 * \brief 平台【右方向键】回调函数
 * \param void 
 * \return 
 */
GBIMELOCAL void GBInputBox_KeyArrowRightHandlerForInputBox(void)
{
	GBIME_KeyDownHandler(GBKEY_RIGHT);
}

/*!
 * \brief 平台【*键】回调函数
 * \param void 
 * \return 
 */
GBIMELOCAL void GBInputBox_KeyStarPressHandlerForInputBox(void)
{
	GBIME_KeyDownHandler(GBKEY_AMB_SPECIAL_A);
}
/*!
 * \brief 平台【#键】回调函数
 * \param void 
 * \return 
 */
GBIMELOCAL void GBInputBox_KeyPoundPressHandlerForInputBox(void)
{
	GBIME_KeyDownHandler(GBKEY_AMB_SPECIAL_B);
}
/*!
 * \brief 平台【确认键】回调函数
 * \param void 
 * \return 
 */
GBIMELOCAL void GBInputBox_KeyOKHandlerForInputBox(void)
{
	GBIME_KeyDownHandler(GBKEY_OK);
}
/*!
 * \brief 平台【返回键】回调函数
 * \param void 
 * \return 
 */
GBIMELOCAL void GBInputBox_KeyBackHandlerForInputBox(void)
{
	GBIME_KeyDownHandler(GBKEY_BACK);
}

/*!
 * \brief 清除按键回调
 * \param type 
 * \return 
 */
GBIMELOCAL void GBInputBox_ClearKeyHandler(GBU16 type)
{
#if __MTK__ > 0
	static const struct {
		U16 key;
		U16 type;
	} validKeys[] = 
	{
		{KEY_0, gbKeyNum},
		{KEY_1, gbKeyNum},
		{KEY_2, gbKeyNum},
		{KEY_3, gbKeyNum},
		{KEY_4, gbKeyNum},
		{KEY_5, gbKeyNum},
		{KEY_6, gbKeyNum},
		{KEY_7, gbKeyNum},
		{KEY_8, gbKeyNum},
		{KEY_9, gbKeyNum},
		{KEY_STAR,		gbKeyStar},
		{KEY_POUND,		gbKeyPound},
		{KEY_UP_ARROW,	gbKeyArrow},
		{KEY_DOWN_ARROW,gbKeyArrow},
		{KEY_RIGHT_ARROW,gbKeyArrow},
		{KEY_LEFT_ARROW,gbKeyArrow},
		{KEY_ENTER,		gbKeyOK},
	};

	int i;
	U16 keyType;
	
    for(i = 0; i < sizeof(validKeys) / sizeof(validKeys[0]); i++)
	{
		if(validKeys[i].type & type)
		{
			for(keyType = 0; keyType < MAX_KEY_TYPE; keyType++)
			{
				ClearKeyHandler(validKeys[i].key, keyType);
			}
		}
	}
#endif
}

static GBInputBoxType GBInputBox_GetInpuBoxType(void)
{
#if __MTK__ > 0
	GBInputBoxType inputBoxType = GBUnsupportInputBox;

	// 根据MTK平台当前的编辑器样式获取输入法关心的输入框类别
	switch (wgui_inputs_get_editor_handler()->input_box_type)
    {
	case MMI_EDITOR_MULTILINE_INPUT_BOX:
		inputBoxType = GBMultilineInputBox;
		break;

	case MMI_EDITOR_EMS_INPUT_BOX:
		inputBoxType = GBEMSInputBox;
		break;

	case MMI_EDITOR_SINGLE_LINE_INPU_BOX:
		inputBoxType = GBSinglineInputBox;
		break;

	case MMI_EDITOR_INLINE_INPUT_BOX:
		inputBoxType = GBInlineInputBox;
		break;

	default:
		inputBoxType = GBUnsupportInputBox;
		break;
	}

	//PT_Assert(inputBoxType != GBUnsupportInputBox);

	return inputBoxType;
#endif // __MTK__
	return GBUnsupportInputBox;
}

/*!
 * \brief 给MTK 平台IMC发送按键消息
 * \param msg_id: 消息id
 * \param param_0: 参数1, 通常为长度
 * \param param_1: 参数2, 通常为字符或字符串头指针地址
 * \return
 * \note
 * \author weizhiping
 * \date 2010年2月21日
 */
static GBU32 GBInputBox_IMC_SendMessage(GBU8 msg_id, GBU32 param_0, GBU32 param_1)
{
#if __MTK__ > 0
	mmi_imc_message_struct gb_imc_msg = {0};

    gb_imc_msg.message_id    = (U8)msg_id;
    gb_imc_msg.param_0       = (U32)param_0;
    gb_imc_msg.param_1       = (U32)param_1;
    return mmi_imc_send_message(&gb_imc_msg);
#endif // __MTK__
	return 0;
}

/*!
 * \brief 显示国笔输入法框架虚拟键盘【达到重新调整输入框内容显示区域尺寸的目的，以前用的GBInputBox_Resize会出现大量刷新问题】
 * \param newWidth 
 * \param newHeight 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-3 9:26:07
*/
#if __MTK__ > 0
extern void mmi_pen_editor_set_virtual_keyboard(gui_virtual_keyboard_language_enum vk_type);
#endif
void GBInputBox_ShowVirtualKeyboard(void)
{
#if __MTK__ > 0
	// 设置国笔输入法框架自己的虚拟键盘
	mmi_pen_editor_set_virtual_keyboard(GUI_VIRTUAL_KEYBOARD_GBIME_CAND);
#endif // __MTK__
}

/*!
 * \brief 隐藏国笔输入法框架自己的虚拟键盘
 * \param void 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-11-11 19:53:06
 */
void GBInputBox_HideVirtualKeyboard(void)
{
#if __MTK__ > 0
	// 隐藏国笔输入法框架自己的虚拟键盘
	mmi_pen_editor_set_virtual_keyboard(GUI_VIRTUAL_KEYBOARD_TRAY);
#endif // __MTK__
}


/*!
 * \brief 上屏候选到输入框
 * \param pUpScreenStr 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-3 9:26:18
*/
GBBOOL GBInputBox_UpScreenString(GBPCWCHAR pUpScreenStr)
{
	if(NULL == pUpScreenStr || 0 == PT_wcslen(pUpScreenStr))
	{
		return GBFalse;
	}
	GBInputBox_InsertString((GBPWCHAR)pUpScreenStr);
	return GBTrue;
}

/*!
 * \brief 重绘输入框
 * \param void 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-3 9:26:28
*/
extern S32 wgui_inline_item_highlighted_index;

void GBInputBox_Redraw(void)
{
#if __MTK__ > 0
	GBInputBoxType inputBoxType = GBInputBox_GetInpuBoxType();

	if (inputBoxType == GBUnsupportInputBox) // 解决inline输入框没有注册情况下调用mmi_imc_redraw_screen_by_state死机问题
	{
		GBInputBox_IMC_SendMessage(MMI_IMC_MESSAGE_REDRAW, 0, 0);
	}
	else if(inputBoxType == GBInlineInputBox)
	{
		redraw_fixed_list();
		//return;
	}

	mmi_imc_redraw_screen_by_state();
#endif // __MTK__
}

/*!
 * \brief 获取输入框编辑区矩形
 * \param pEditRect 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-3 9:26:44
*/
void GBInputBox_GetEditRect(GBRECT *pEditRect)
{
#if __MTK__ > 0
	GBInputBox_IMC_SendMessage(MMI_IMC_MESSAGE_GET_INPUT_BOX_RECT, (GBU32)pEditRect, 0);
#endif // __MTK__
}

/*!
 * \brief 内容清空
 * \param void 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-3 9:26:56
*/
void GBInputBox_DeleteAll(void)
{
#if __MTK__ > 0
    GBInputBox_IMC_SendMessage(MMI_IMC_MESSAGE_CLEAR_ALL, 1, 0);
#endif // __MTK__
}

/*!
 * \brief 判断输入框是否空
 * \param void 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-3 9:27:10
*/
GBBOOL GBInputBox_InputBoxIsEmtpy(void)
{
#if __MTK__ > 0
	return GBInputBox_IMC_SendMessage(MMI_IMC_MESSAGE_IS_EDITOR_EMPTY, 0, 0);
#endif // __MTK__
	return GBTrue;
}

/*!
 * \brief 切换到指定输入法模式
 * \param input_mode 
 * \param reserved 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-3 9:27:25
*/
GBBOOL GBInputBox_ChangeInputMode(GBINT input_mode, GBU32 reserved)
{
#if __MTK__ > 0
	mmi_imc_key_change_input_mode_ex((mmi_imm_input_mode_enum)input_mode, reserved);
#endif // __MTK__
	return GBTrue;
}

/*!
 * \brief 通过物理按键（如'#'）循环切换输入法模式
 * \param void 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-3 9:27:25
*/
void GBInputBox_KeyChangeInputMode(void)
{
#if __MTK__ > 0
	mmi_imc_key_change_input_mode();
#endif // __MTK__
}

/*!
 * \brief 获取当前平台的输入法模式
 * \param void
 * \return mmi_imm_input_mode_enum
 * \注意：
 */
GBINT GBInputBox_GetCurrentInputMode(void)
{
#if __MTK__ > 0
	return (GBINT)mmi_imm_get_curr_input_mode();
#endif // __MTK__
	return -1;
}

/*!
 * \brief 获取下一个输入法模式
 * \param void
 * \return mmi_imm_input_mode_enum
 * \注意：
 */
GBINT GBInputBox_GetNextInputMode(void)
{
#if __MTK__ > 0
	return (GBINT)mmi_imm_get_next_mode();
#endif // __MTK__
	return -1;
}

/*!
 * \brief 移动光标位置
 * \param cursorDirection 
 * \param step 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-3 9:27:43
*/
void GBInputBox_MoveCursor(GBInputBoxCursorDirection cursorDirection, GBINT step)
{
#if __MTK__ > 0
	switch(cursorDirection)
	{
    case GBINPUTBOX_CURSOR_UP:
		GBInputBox_IMC_SendMessage(MMI_IMC_MESSAGE_MOVE_CURSOR, MMI_IMC_EDITOR_CURSOR_UP, step);
		break;
	case GBINPUTBOX_CURSOR_DOWN:
		GBInputBox_IMC_SendMessage(MMI_IMC_MESSAGE_MOVE_CURSOR, MMI_IMC_EDITOR_CURSOR_DOWN, step);
		break;
	case GBINPUTBOX_CURSOR_LEFT:
		GBInputBox_IMC_SendMessage(MMI_IMC_MESSAGE_MOVE_CURSOR, MMI_IMC_EDITOR_CURSOR_LEFT, step);
		break;
	case GBINPUTBOX_CURSOR_RIGHT:
		GBInputBox_IMC_SendMessage(MMI_IMC_MESSAGE_MOVE_CURSOR, MMI_IMC_EDITOR_CURSOR_RIGHT, step);
		break;
	default:
		break;
	}
#endif // __MTK__
}

/*!
 * \brief 插入单个字符
 * \param character 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-3 9:28:05
*/
void GBInputBox_InsertChar(GBWCHAR character)
{
#if __MTK__ > 0
	GBInputBox_IMC_SendMessage(MMI_IMC_MESSAGE_INSERT_CHAR, (GBU32)character, 0);
#endif // __MTK__
}

/*!
 * \brief 插入字符串
 * \param pInsertString 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-3 9:28:17
*/
void GBInputBox_InsertString(GBPWCHAR pInsertString)
{
#if __MTK__ > 0
	mmi_editor_inputbox_type_enum input_box_type = wgui_inputs_get_editor_handler()->input_box_type;
	GBInputBox_IMC_SendMessage(MMI_IMC_MESSAGE_INSERT_STRING, (GBU32)pInsertString, 0);
#endif // __MTK__
}

/*!
 * \brief 删除单个字符
 * \param void 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-3 9:28:40
*/
void GBInputBox_DeleteChar(void)
{
#if __MTK__ > 0
	GBInputBox_IMC_SendMessage(MMI_IMC_MESSAGE_DELETE_CHAR, 1, 0);
#endif // __MTK__
}

/*!
 * \brief 高亮替换字符串
 * \param nCandLength 
 * \param pCandString 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-3 9:28:56
*/
GBUINT GBInputBox_ReplaceHighlightString(GBUINT nCandLength, GBPCWCHAR pCandString)
{
#if __MTK__ > 0
	GBUINT repaceLen = 0;
	
	gdi_layer_lock_frame_buffer();
	repaceLen = GBInputBox_IMC_SendMessage(MMI_IMC_MESSAGE_REPLACE_HIGHLIGHT_STRING, 
		                             (GBU32)nCandLength, (GBU32)pCandString);    
	gdi_layer_unlock_frame_buffer();
	
	return repaceLen;
#endif // __MTK__
	return 0;
}

/*!
 * \brief 清除高亮
 * \param void 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-3 9:29:09
*/
void GBInputBox_ClearHilight(void)
{
#if __MTK__ > 0
	GBInputBox_IMC_SendMessage(MMI_IMC_MESSAGE_CLEAR_HILIGHT, 0, 0);
#endif // __MTK__
}

/*!
 * \brief 高亮插入单个字符
 * \param pMultiTapString 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-3 9:29:56
*/
void GBInputBox_MultiTapInsertChar(GBPWCHAR pMultiTapString)
{
#if __MTK__ > 0
	gdi_layer_lock_frame_buffer();
	GBInputBox_IMC_SendMessage(MMI_IMC_MESSAGE_INSERT_MULTITAP_CHAR, (GBU32)pMultiTapString[0], 0);
	gdi_layer_unlock_frame_buffer();
#endif // __MTK__
}

/*!
 * \brief 高亮删除指定长度内容
 * \param deleteLen 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-3 9:30:10
*/
void GBInputBox_MultiTapDelete(GBU32 deleteLen)
{
#if __MTK__ > 0
	if (deleteLen <= 0)
	{
		return ;
	}
	
	//gdi_layer_lock_frame_buffer();
    mmi_ime_multitap_confirm();
    GBInputBox_IMC_SendMessage(MMI_IMC_MESSAGE_RESET_MULTITAP_STATE, 0, 0);
	
	if (deleteLen > 1)
	{
		GBInputBox_IMC_SendMessage(MMI_IMC_MESSAGE_DELETE_STRING, deleteLen, 0);
	}
	else // delete one char
	{
		GBInputBox_IMC_SendMessage(MMI_IMC_MESSAGE_DELETE_CHAR, 1, 0);
	}
	
	GBInputBox_ClearHilight(); // 结束MultiTap显示状态
    mmi_imc_set_state(MMI_IMC_STATE_INITIAL);
    //mmi_imc_redraw_screen_by_state();
	GBInputBox_Redraw();
    //gdi_layer_unlock_frame_buffer();
    //gdi_lcd_repaint_all();
#endif // __MTK__
}

/*!
 * \brief 高亮输入确认
 * \param void 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-3 9:30:33
*/
void GBInputBox_MultitapInputConfirm(void)
{
#if __MTK__ > 0
	//gdi_layer_lock_frame_buffer();
	mmi_ime_multitap_confirm();	
	GBInputBox_ClearHilight(); // 结束MultiTap显示状态	
    /* cancel editor multitap state */
    GBInputBox_IMC_SendMessage(MMI_IMC_MESSAGE_RESET_MULTITAP_STATE, 0, 0);
    mmi_imc_set_state(MMI_IMC_STATE_INITIAL);
    //mmi_imc_redraw_screen_by_state();
	GBInputBox_Redraw();
    //gdi_layer_unlock_frame_buffer();
    //gdi_lcd_repaint_all();
#endif // __MTK__
}

/*!
 * \brief 设置LSK响应函数(左软键)
 * \param void 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-3 9:31:00
*/
void GBInputBox_SetPlatformLSK(void)
{
#if __MTK__ > 0
	// 左软键[EMS,singline 和multiline input box右软件设置都是不一样的]
	set_left_softkey_function(g_imc_key_inputbox_LSK_function, KEY_EVENT_UP); // GBInputBox_SetAllIMEFuncKey里面把UP回调置空了
	set_left_softkey_function(UI_dummy_function, KEY_EVENT_DOWN);
#endif // __MTK__
}

/*!
 * \brief 设置RSK响应函数(右软键)
 * \param void 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-3 9:33:52
*/
void GBInputBox_SetPlatformRSK(void)
{
#if __MTK__ > 0
	GBInputBoxType inputBoxType = GBInputBox_GetInpuBoxType();
	
	// 针对不同输入框类别有差异的函数
	switch (inputBoxType)
    {
	case GBMultilineInputBox:
		wgui_set_RSK();
		break;
	case GBEMSInputBox:
		//wgui_set_RSK(); // 引用错误[08B版本短按删除键会弹出"正在加载"，长按死机]
		wgui_set_EMS_inputbox_RSK();
		break;
	case GBSinglineInputBox:		
		wgui_set_singleline_RSK();
		break;
	case GBInlineInputBox:
		inline_text_edit_set_RSK_function();
		break;
	default:
		PT_Assert(0);
		break;
	}
#endif // __MTK__
}

/*!
 * \brief 设置方向键响应函数
 * \param void 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-3 9:34:28
*/
void GBInputBox_SetPlatformArrowKey(void)
{
#if __MTK__ > 0
	GBInputBoxType inputBoxType = GBInputBox_GetInpuBoxType();
	
	// 针对不同输入框类别有差异的函数
	switch (inputBoxType)
    {
	case GBMultilineInputBox:
		register_multiline_inputbox_keys();
		break;
	case GBEMSInputBox:
		register_EMS_inputbox_keys();
		break;
	case GBSinglineInputBox:		
		register_singleline_inputbox_keys();
		break;
	case GBInlineInputBox:
		GB_register_inline_fixed_list_keys();
		break;
	default:
		PT_Assert(0);
		break;
	}
#endif // __MTK__
}

/*!
 * \brief 设置KEY_ENTER键响应函数
 * \param void 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-3 9:34:47
*/
void GBInputBox_SetPlatformEnterKey(void)
{
#if __MTK__ > 0
	SetKeyHandler(g_imc_key_inputbox_LSK_function, KEY_ENTER, KEY_EVENT_DOWN);
#endif // __MTK__
}

/*!
 * \brief 设置平台【功能按键】响应函数
 * \param void 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-3 9:34:56
*/
void GBInputBox_SetAllPlatformFuncKey(void)
{
#if __MTK__ > 0
	GBInputBoxType inputBoxType = GBInputBox_GetInpuBoxType();
	
	GBInputBox_SetPlatformLSK();
	GBInputBox_SetPlatformRSK();
	GBInputBox_SetPlatformEnterKey();
	
	// 针对不同输入框类别有差异的函数
	switch (inputBoxType)
    {
	case GBMultilineInputBox:
		register_multiline_inputbox_keys();
		break;
	case GBEMSInputBox:
		register_EMS_inputbox_keys();
		break;        
	case GBSinglineInputBox:		
		register_singleline_inputbox_keys();
		break;
	case GBInlineInputBox:
		GB_register_inline_fixed_list_keys();
		break;
	default:
		PT_Assert(0);
		break;
	}

	// 注册'#'键切换输入法模式
	SetKeyHandler(mmi_imc_key_change_input_mode, KEY_POUND, KEY_EVENT_DOWN);
#endif // __MTK__
}

/*!
 * \brief 设置IME框架【功能按键】响应函数
 * \param void 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-3 9:35:28
*/
void GBInputBox_SetAllIMEFuncKey(void)
{
#if __MTK__ > 0
	PEngineOutputInfo pEngineOutputInfo = CLayoutControl_GetEngineOutputInfo();

	// 方向箭
	SetKeyHandler(GBInputBox_KeyArrowRightHandlerForInputBox,		KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	SetKeyHandler(GBInputBox_KeyArrowDownHandlerForInputBox,		KEY_DOWN_ARROW,	KEY_EVENT_DOWN);
	SetKeyHandler(GBInputBox_KeyArrowLeftHandlerForInputBox,		KEY_LEFT_ARROW,	KEY_EVENT_DOWN);
	SetKeyHandler(GBInputBox_KeyArrowUpHandlerForInputBox,		KEY_UP_ARROW, KEY_EVENT_DOWN);
	
	// *键[connect时注册之后就不需要改变了]
	//SetKeyHandler(GBInputBox_KeyStarPressHandlerForInputBox,   	KEY_STAR, KEY_EVENT_DOWN);	
	
	// 左软键
	set_left_softkey_function(GBInputBox_KeyOKHandlerForInputBox, KEY_EVENT_DOWN); //pidongming modify 20081105
	set_left_softkey_function(UI_dummy_function, KEY_EVENT_UP);

	// OK键
	SetKeyHandler(GBInputBox_KeyOKHandlerForInputBox, KEY_ENTER, KEY_EVENT_DOWN);
	
#ifndef __MMI_WITH_C_KEY__ // 回退键
	// 将右软件设置为GB自己的删除函数
	set_right_softkey_function(GBInputBox_KeyBackHandlerForInputBox, KEY_EVENT_DOWN);
	set_right_softkey_function(UI_dummy_function, 			KEY_EVENT_UP);
	//set_right_softkey_function(GBKeyClearAllHandlerForInputBox, KEY_EVENT_LONG_PRESS); // 设置长按处理函数. Fix [MTK 08B 0000194]: 有输入有候选栏时长按清除键有误
#else
	SetKeyHandler(GBInputBox_KeyBackHandlerForInputBox, 			KEY_CLEAR, KEY_EVENT_UP);
	SetKeyHandler(UI_dummy_function, 						KEY_CLEAR, KEY_EVENT_DOWN);
	//SetKeyHandler(GBKeyClearAllHandlerForInputBox, 		KEY_CLEAR, KEY_EVENT_LONG_PRESS);
#endif

	// '#'键
	if(!pEngineOutputInfo->bInputEmpty && pEngineOutputInfo->bCandWindOpen)
	{
		SetKeyHandler(GBInputBox_KeyPoundPressHandlerForInputBox, KEY_POUND, KEY_EVENT_DOWN);
	}
	else
	{
		SetKeyHandler(mmi_imc_key_change_input_mode, KEY_POUND, KEY_EVENT_DOWN);
	}
#endif // __MTK__
}

/*!
 * \brief 注册IME框架的按键响应函数
 * \param void 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-3 9:35:37
*/
void GBInputBox_RegisterIMEKeys(void)
{
#if __MTK__ > 0
	// 清除所有其他的处理函数
	GBInputBox_ClearKeyHandler(gbKeyAll);
	
	// 数字键
	SetKeyHandler(GBInputBox_Key0PressHandlerForInputBox, 		KEY_0, KEY_EVENT_DOWN);
	SetKeyHandler(GBInputBox_Key1PressHandlerForInputBox, 		KEY_1, KEY_EVENT_DOWN);
	SetKeyHandler(GBInputBox_Key2PressHandlerForInputBox, 		KEY_2, KEY_EVENT_DOWN);
	SetKeyHandler(GBInputBox_Key3PressHandlerForInputBox, 		KEY_3, KEY_EVENT_DOWN);
	SetKeyHandler(GBInputBox_Key4PressHandlerForInputBox, 		KEY_4, KEY_EVENT_DOWN);
	SetKeyHandler(GBInputBox_Key5PressHandlerForInputBox, 		KEY_5, KEY_EVENT_DOWN);
	SetKeyHandler(GBInputBox_Key6PressHandlerForInputBox, 		KEY_6, KEY_EVENT_DOWN);
	SetKeyHandler(GBInputBox_Key7PressHandlerForInputBox, 		KEY_7, KEY_EVENT_DOWN);
	SetKeyHandler(GBInputBox_Key8PressHandlerForInputBox, 		KEY_8, KEY_EVENT_DOWN);
	SetKeyHandler(GBInputBox_Key9PressHandlerForInputBox, 		KEY_9, KEY_EVENT_DOWN);

	// *键
	SetKeyHandler(GBInputBox_KeyStarPressHandlerForInputBox,   	KEY_STAR, KEY_EVENT_DOWN);
	
	// 初始状态候选框关闭，左右软键、方向键、#键需要注册为系统自己的回调
	GBInputBox_SetAllPlatformFuncKey();
#endif // __MTK__
}

/*!
 * \brief 清除所有按键回调
 * \param void 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-3 9:35:47
*/
void GBInputBox_ClearAllKey(void)
{
	GBInputBox_ClearKeyHandler(gbKeyAll);
	set_left_softkey_function(UI_dummy_function, KEY_EVENT_DOWN);
	set_left_softkey_function(UI_dummy_function, KEY_EVENT_UP);
	set_right_softkey_function(UI_dummy_function, KEY_EVENT_DOWN);
	set_right_softkey_function(UI_dummy_function, KEY_EVENT_UP);
}

/*!
* \brief 重绘右软键
* \param void
* \return void
*/
void GBInputBox_RedrawRightSoftKey(void)
{
#if __MTK__ > 0
	redraw_right_softkey();
#endif // __MTK__
}

/*!
* \brief 重绘左软键
* \param void
* \return void
*/
void GBInputBox_RedrawLeftSoftKey(void)
{
#if __MTK__ > 0
	redraw_left_softkey(); 
#endif // __MTK__
}

/*!
* \brief 获取输入栏文本长度
* \param void
* \return GBUINT16
*/
extern single_line_input_box MMI_inline_singleline_inputbox;

GBUINT16 GBInputBox_GetTextLen(void)
{
#if __MTK__ > 0
	GBUINT16 textLen;
	GBInputBoxType inputBoxType = GBInputBox_GetInpuBoxType();
	
	// 针对不同输入框类别有差异的函数
	switch (inputBoxType)
    {
	case GBMultilineInputBox:
		textLen = (GBUINT16)wgui_inputs_ml_get_text_length();
		break;
	case GBEMSInputBox:
		textLen = (GBUINT16)MMI_EMS_inputbox.data->textLength;
		break;        
	case GBSinglineInputBox:		
		textLen = (GBUINT16)gui_single_line_input_box_get_text_length(&MMI_singleline_inputbox);
		break;

	case GBInlineInputBox:
		textLen = gui_single_line_input_box_get_text_length(&MMI_inline_singleline_inputbox);
		break;
			
	default:
		PT_Assert(0);
		break;
	}
	
	return textLen;
#endif // __MTK__
	return 0;
}

/*!
 * \brief 根据状态重绘输入框界面
 * \param 
 * \return void
 * \注意：
 */
void GBInputBox_RedrawScreenByState(void)
{
#if __MTK__ > 0
	mmi_imc_redraw_screen_by_state();
#endif // __MTK__
}

/*!
 * \brief  重画输入框UI界面组件
 * \param imui_set
 * \return void
 * \注意：
 */
void GBInputBox_RedrawImui(GBInputBoxImuiControlType imuiControlType)
{
#if __MTK__ > 0
	U32 imui_set;

	switch (imuiControlType)
	{
	case GBINPUTBOX_IMUI_INFORMATION_BAR:
		imui_set = MMI_IMUI_CONTROL_MASK(MMI_IMUI_INFORMATION_BAR);
		break;
	case GBINPUTBOX_IMUI_VIRTUAL_KEYBOARD:
		imui_set = MMI_IMUI_CONTROL_MASK(MMI_IMUI_VIRTUAL_KEYBOARD);
		break;
	case GBINPUTBOX_IMUI_COMPOSITION_AREA:
		imui_set = MMI_IMUI_CONTROL_MASK(MMI_IMUI_COMPOSITION_AREA);
		break;
	case GBINPUTBOX_IMUI_CANDIDATE_AREA:
		imui_set = MMI_IMUI_CONTROL_MASK(MMI_IMUI_CANDIDATE_AREA);
		break;
	case GBINPUTBOX_IMUI_INPUT_AREA:
		imui_set = MMI_IMUI_CONTROL_MASK(MMI_IMUI_INPUT_AREA);
		break;
	case GBINPUTBOX_IMUI_TITLE_AREA:
		imui_set = MMI_IMUI_CONTROL_MASK(MMI_IMUI_TITLE_AREA);
		break;
	case GBINPUTBOX_IMUI_MULTI_BLOCK_AREA:
		imui_set = MMI_IMUI_CONTROL_MASK(MMI_IMUI_MULTI_BLOCK_AREA);
		break;
	case GBINPUTBOX_IMUI_IME_HINT_AREA:
		//imui_set = MMI_IMUI_CONTROL_MASK(MMI_IMUI_IME_HINT_AREA);
		break;
	default:
		return;
	}
	mmi_imc_redraw_imui(imui_set);
#endif // __MTK__
}

/*!
* \brief 设置右软键图标
* \param GBPUACHAR
* \return void
*/
void GBInputBox_SetRightSoftkeyIcon(GBPUACHAR icon)
{
#if __MTK__ > 0
	set_right_softkey_icon(icon);
#endif // __MTK__
}

/*!
* \brief 设置左软键图标
* \param GBPUACHAR
* \return void
*/
void GBInputBox_SetLeftSoftkeyIcon(GBPUACHAR icon)
{
#if __MTK__ > 0
	set_left_softkey_icon(icon);
#endif // __MTK__
}

/*!
* \brief 设置右软键提示字符
* \param GBPUINT16
* \return void
*/
void GBInputBox_SetRightSoftkeyLabel(GBPUINT16 s)
{
#if __MTK__ > 0
	set_right_softkey_label(s);
#endif // __MTK__
}

/*!
* \brief 设置左软键提示字符
* \param GBPUINT16
* \return void
*/
void GBInputBox_SetLeftSoftkeyLabel(GBPUINT16 s)
{
#if __MTK__ > 0
	set_left_softkey_label(s);
#endif // __MTK__
}

/*!
* \brief 隐藏右软键及其功能
* \param void
* \return void
*/
void GBInputBox_HideRightSoftkey(void)
{
#if __MTK__ > 0
	wgui_hide_right_softkey(); 
#endif // __MTK__
}

/*!
* \brief 隐藏左软键及其功能
* \param void
* \return void
*/
void GBInputBox_HideLeftSoftkey(void)
{
#if __MTK__ > 0
	wgui_hide_left_softkey(); 
#endif // __MTK__
}

/*!
 * \brief 更新左右软键盘显示状态
 * \param 
 * \return 
 * \note 
 * \author weizhiping
 * \date 2010年2月5日
 */
void GBInputBox_UpdateSoftKeyLabel(void)
{
	PEngineOutputInfo pEngineOutputInfo = CLayoutControl_GetEngineOutputInfo();
	
	// 引擎处理状态
	if (pEngineOutputInfo->bCandWindOpen)
	{
		// 左软键显示为【选择】
		set_left_softkey_label(get_string(STR_GLOBAL_SELECT));
		
		// 输入框为空 && 输入法引擎有内容, 右软键显示为【清除】
		if(GBInputBox_InputBoxIsEmtpy() && !pEngineOutputInfo->bInputEmpty)
		{
			set_right_softkey_label((UI_string_type) get_string(STR_GLOBAL_CLEAR));
		}
	}
	else // 平台处理状态
	{
		UI_string_type pLeftSoftkeyLabel = GBInputBox_GetPlatformLeftSoftkeyLabel();
		UI_string_type pRightSoftkeyLabel = GBInputBox_GetPlatformRightSoftkeyLabel();

		// 左软键显示为平台应用刚进入编辑界面时设置的标签(如无则默认为【选项】)
		if (pLeftSoftkeyLabel != NULL)
		{
			set_left_softkey_label(pLeftSoftkeyLabel);
		}
		else
		{
			set_left_softkey_label(get_string(STR_GLOBAL_OPTIONS));
		}

		// 输入框内容为空, 右软键显示为平台应用刚进入编辑界面时设置的标签(如无则默认为【返回】)
		if(GBInputBox_InputBoxIsEmtpy())
		{			
			if (pRightSoftkeyLabel != NULL)
			{
				set_right_softkey_label(pRightSoftkeyLabel);
			}
			else
			{
				set_right_softkey_label(get_string(STR_GLOBAL_BACK));
			}
		}
		else // 不为空右软键显示为【清除】
		{
			set_right_softkey_label(get_string(STR_GLOBAL_CLEAR));
		}
	}
  	
	// 重绘左软键
	set_left_softkey_icon(NULL);
	wgui_hide_left_softkey();
	redraw_left_softkey();

	// 重绘右软键
	set_right_softkey_icon(NULL);
	wgui_hide_right_softkey();
	redraw_right_softkey();
}

/*!
 * \brief 短按删除键处理函数
 * \param void 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-10-9 9:49:29
*/
void GBInputBox_DeletkeyHandler(void)
{
#if __MTK__ > 0
	PCLayoutAbstract pltAbstractObj = Global_GetCurrentLayoutObject();
	PCLayoutControl pLtCtl = Global_GetLayoutCtl();
	
	gui_reset_clip();
	gdi_layer_lock_frame_buffer();    
	gui_push_clip();
	gui_push_text_clip();

	if (pLtCtl->engineOutputInfo.bCandWindOpen)
	{
// 		pltAbstractObj->bSendEngineKeyEvent = GBTrue;
// 		pltAbstractObj->engineKeyEvent = GBKEY_BACK;
// 		Global_FillEventGroup(GBIME_CT_NORMAL_KEY_UP, GBKEY_BACK, 0);
		GBEngine_SendKeyEvent(Global_GetEnineInstance(), GBKEY_BACK, 0);//引擎pOutputString没有更新[BUG 611]
		//GBIME_Paint();
	}
	else
	{
		GBInputBox_DeleteChar();
	}

	gui_pop_clip();
	gui_pop_text_clip();    
	gdi_layer_unlock_frame_buffer();
	gdi_lcd_repaint_all();

	GBInputBox_UpdateSoftKeyLabel();
#endif
}

/*!
 * \brief 长按删除处理函数
 * \param void 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-10-9 9:49:00
*/
void GBInputBox_DeleteAllHandler(void)
{
#if __MTK__ > 0
	PCLayoutControl pLtCtl = Global_GetLayoutCtl();

	gui_reset_clip();
	gdi_layer_lock_frame_buffer();    
	gui_push_clip();
	gui_push_text_clip();

	// 循环清空引擎输入串内容
	if (pLtCtl->engineOutputInfo.bCandWindOpen)
	{
		GBEngine_Reset(Global_GetEnineInstance());
		//while (pEngineOutputInfo->.inputStringLen > 0)
		{
			//GBEventHandler(GBKEY_BACK, 0);//引擎pOutputString没有更新[BUG 611]		
		}
		//GBIME_Paint();
	}
	else // 清空编辑器输入框内容
	{
		// 		if(!GBInputBoxIsEmpty() && gbCurInputBox.pfDeleteAll)
		// 		{
		// 			(*gbCurInputBox.pfDeleteAll)();
		// 		}
		// 		mmi_imc_message_struct msg_ptr;
		// 		msg_ptr.message_id = MMI_IMC_MESSAGE_CLEAR_ALL;
		// 		msg_ptr.param_0 = 1;
		// 		msg_ptr.param_1 = 0;
		// 		mmi_imc_send_message( &msg_ptr );
		GBInputBox_DeleteAll();
	}

	gui_pop_clip();
	gui_pop_text_clip();    
	gdi_layer_unlock_frame_buffer();
	gdi_lcd_repaint_all();
	
	// 重置功能按键及引擎状态并关闭候选框
	//GBResetFuncKey();
	//GBInputMethodReset();
	//GBDrawCandWnd(1);
#endif
}

/*!
 * \brief 判断是否需要改变编辑框的下拉高度
 * \param void 
 * \return 
 * \note 判断当前下拉高度和原始下拉高度是否相等。如果候选框开启的状态下，当前下拉高度和原始下拉高度一样，则需要改变高度
 * \example
 * \author Zhaokun
 * \date 2010-10-22 18:00:00
*/
GBBOOL GBInputBox_IsChangeHistoryHeight()
{
#if __MTK__ > 0
	mmi_imc_rect input_box_rect;
	GBINT nInputBoxHistoryHeight = 0;
	
	GBInputBox_IMC_SendMessage(MMI_IMC_MESSAGE_GET_INPUT_BOX_RECT, (GBU32)&input_box_rect, 0);
	nInputBoxHistoryHeight = (GBINT)input_box_rect.height;
	
	return (GBBOOL)(nInputBoxHistoryHeight != g_PlatformAppInfo.inputBoxHistoryHeight);
#endif // __MTK__
	return GBTrue;
}

/*!
 * \brief 初始化平台输入框(获取平台应用相关的状态信息)
 * \param 
 * \return
 * \note 进入输入法编辑界面的时候调用(mmi_imc_connect)
 * \author weizhiping
 * \date 2010年2月5日
 */
GBBOOL GBInputBox_Init(void)
{
#if __MTK__ > 0
	mmi_imc_rect input_box_rect;

	memset(&g_PlatformAppInfo, 0, sizeof(GBPlatformAppInfo));

	// 记录进入时的编辑框内容区高度
	GBInputBox_IMC_SendMessage(MMI_IMC_MESSAGE_GET_INPUT_BOX_RECT, (GBU32)&input_box_rect, 0);
	g_PlatformAppInfo.inputBoxHistoryWidth = (GBINT)input_box_rect.width;
	g_PlatformAppInfo.inputBoxHistoryHeight= (GBINT)input_box_rect.height;
	
	// 记录平台应用进入编辑框时的softkey名称
	g_PlatformAppInfo.left_softkey_label = get_left_softkey_label();
	g_PlatformAppInfo.right_softkey_label = get_right_softkey_label();	
	return GBTrue;
#endif // __MTK__
	return GBTrue;
}

/*!
 * \brief 国笔输入法获取平台左软键显示标签
 * \param 
 * \return
 * \note 
 * \author weizhiping
 * \date 2010年2月5日
 */
UI_string_type GBInputBox_GetPlatformLeftSoftkeyLabel(void)
{
	return g_PlatformAppInfo.left_softkey_label;
}

/*!
 * \brief 国笔输入法获取平台左软键显示标签
 * \param 
 * \return
 * \note 
 * \author weizhiping
 * \date 2010年2月5日
 */
UI_string_type GBInputBox_GetPlatformRightSoftkeyLabel(void)
{
	return g_PlatformAppInfo.right_softkey_label;
}

/*!
 * \brief 编辑框是否处于只读文本查看模式
 * \param 
 * \return
 * \note 此时GBIME框架不接管消息处理
 * \author weizhiping
 * \date 2010年11月19日
 */
GBBOOL GBInputBox_IsReadOnlyTextView(void)
{
	if(wgui_inputs_get_editor_handler()->input_box_type == MMI_EDITOR_MULTILINE_INPUT_BOX 
	  && (MMI_multiline_inputbox.flags & UI_MULTI_LINE_INPUT_BOX_VIEW_MODE))
	{
		return GBTrue;
	}

	return GBFalse;
}

/*!
 * \brief 平台输入框处于纯数字输入模式（此时输入法框架禁止切换到其他输入法模式）
 * \param void 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-12-8 9:41:14
*/
GBBOOL GBInputBox_IsDigitInputOnly(void)
{
	// 电话号码、浮点输入界面禁止功能键及滑屏操作
	switch (mmi_imm_get_curr_input_mode())
	{
	case IMM_INPUT_MODE_SIGNED_123:
	case IMM_INPUT_MODE_FLOAT_123:
	case IMM_INPUT_MODE_PHONE_NUMBER:
	case IMM_INPUT_MODE_PHONE_NUMBER_WILDCHAR:
	case IMM_INPUT_MODE_SIM_NUMERIC:
	case IMM_INPUT_MODE_SIGNED_FLOAT_123:
		return GBTrue;
	default:
		break;
	}

	// 日期、IP、时间输入界面禁止功能键及滑屏操作
	switch (wgui_inputs_get_editor_handler()->input_box_sub_type)
	{
	case MMI_EDITOR_DATE:
	case MMI_EDITOR_IP4:
	case MMI_EDITOR_TIME:
	case MMI_EDITOR_TIME_PERIOD:
		return GBTrue;
	default:
		break;
	}

	return GBFalse;
}

/*!
 * \brief 平台输入框处于快速查找输入模式（此时输入法框架禁止切换到其他输入法模式）
 * \param void 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-12-8 9:41:14
*/
GBBOOL GBInputBox_IsQuickSearchInputMode(void)
{
// 	switch (mmi_imm_get_curr_input_mode())
// 	{
// 	case IMM_INPUT_MODE_QUICK_SEARCH_BOPOMO:
// 	case IMM_INPUT_MODE_QUICK_SEARCH_HK_STROKE:
// 	case IMM_INPUT_MODE_QUICK_SEARCH_PINYIN:
// 	case IMM_INPUT_MODE_QUICK_SEARCH_SM_STROKE:
// 	case IMM_INPUT_MODE_QUICK_SEARCH_TR_STROKE:
// 		return GBTrue;
// 	default:
// 		break;
// 	}
	if (wgui_inputs_get_editor_handler()->input_box_sub_type == MMI_EDITOR_QUICK_SEARCH_SINGLE_LINE_INPUT_BOX)
	{
		return GBTrue;
	}

	return GBFalse;
}

/*!
 * \brief 平台处理按键按下消息
 * \param KeyCode 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-12-30 17:05:52
 */
void GBInputBox_ProcessKeyDown(GBU16 KeyCode)
{
	ExecuteCurrKeyHandler(KeyCode, KEY_EVENT_DOWN);
}

/*!
 * \brief 平台处理按键弹起消息
 * \param KeyCode 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-12-30 17:05:52
 */
void GBInputBox_ProcessKeyUp(GBU16 KeyCode)
{
	ExecuteCurrKeyHandler(KeyCode, KEY_EVENT_UP);
}

/*!
 * \brief 平台处理按键消息，不需要V5引擎处理
 * \param void 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-12-30 18:29:15
 */
GBBOOL GBInputBox_PlatformHandleKeyMode(void)
{
	 switch (mmi_imm_get_curr_input_mode())
 	{
 	case IMM_INPUT_MODE_QUICK_SEARCH_SM_STROKE: // 快速查找笔画下面平台处理按键消息，不需要V5引擎处理
 	case IMM_INPUT_MODE_QUICK_SEARCH_TR_STROKE:
 		return GBTrue;
 	default:
 		break;
 	}
	return GBFalse;
}