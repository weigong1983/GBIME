/***************************************************************************
* File Name    :  PlatformInputBox.h
* Corporation  :  Guobi Technology Ltd. GuangZhou China
* Copyright    :  Copyright Guobi Technology Ltd. And Subject to Confidentiality 
*                 Restrictions. This document is the confidential and proprietary 
*                 property of Guobi Technology Ltd.It is neither the document                  
*                 itself nor any part of it can be shared with any party  without 
*                 the written  permission. 
* Author       :  weizhiping
* Date         :  2010-06-08
* Purpose      :  封装MTK平台输入框相关功能函数
* Version      :  V1.0
* Last Changed List
* Name         |   date   |  Discription
****************************************************************************/
#ifndef __PLATFORM_INPUTBOX__
#define __PLATFORM_INPUTBOX__

#include "gbtype.h"
#include "GBIMEDef.h"
#include "gbft_def.h"

enum KeyType {
	gbKeyNum = 1,
	gbKeyArrow = 2,
	gbKeyStar = 4,
	gbKeyPound = 8,
	gbKeyStarPound = gbKeyStar + gbKeyPound,
	gbKeyOK = 16,
	gbKeyAll = (GBU16)-1
};

/*!
 * \brief 国笔输入框类型
 */
typedef enum {
	GBSinglineInputBox	 	///<单行
	,GBMultilineInputBox	///<多行
	,GBEMSInputBox 			///<短信
	,GBInlineInputBox
	,GBUnsupportInputBox	///<不支持
}GBInputBoxType;

/*!
 * \brief 国笔输入框光标方向
 */
typedef enum {
	GBINPUTBOX_CURSOR_UP		///<上
	,GBINPUTBOX_CURSOR_DOWN		///<下
	,GBINPUTBOX_CURSOR_LEFT		///<左
	,GBINPUTBOX_CURSOR_RIGHT	///<右
}GBInputBoxCursorDirection;

typedef enum
{
    GBINPUTBOX_IMUI_INFORMATION_BAR = 0,
	GBINPUTBOX_IMUI_VIRTUAL_KEYBOARD,
	GBINPUTBOX_IMUI_COMPOSITION_AREA,
	GBINPUTBOX_IMUI_CANDIDATE_AREA,
	GBINPUTBOX_IMUI_INPUT_AREA,
	GBINPUTBOX_IMUI_TITLE_AREA,
	GBINPUTBOX_IMUI_MULTI_BLOCK_AREA,
	GBINPUTBOX_IMUI_IME_HINT_AREA,
	GBINPUTBOX_IMUI_CONTROL_MAX
} GBInputBoxImuiControlType;
#ifdef __cplusplus
extern "C" {
#endif

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
void GBInputBox_ShowVirtualKeyboard(void);

/*!
 * \brief 隐藏国笔输入法框架自己的虚拟键盘
 * \param void 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-11-11 19:53:06
 */
void GBInputBox_HideVirtualKeyboard(void);

/*!
 * \brief 上屏候选到输入框
 * \param pUpScreenStr 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-3 9:26:18
*/
GBBOOL GBInputBox_UpScreenString(GBPCWCHAR pUpScreenStr);

/*!
 * \brief 重绘输入框
 * \param void 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-3 9:26:28
*/
void GBInputBox_Redraw(void);

/*!
 * \brief 获取输入框编辑区矩形
 * \param pEditRect 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-3 9:26:44
*/
void GBInputBox_GetEditRect(GBRECT *pEditRect);

/*!
 * \brief 内容清空
 * \param void 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-3 9:26:56
*/
void GBInputBox_DeleteAll(void);

/*!
 * \brief 判断输入框是否空
 * \param void 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-3 9:27:10
*/
GBBOOL GBInputBox_InputBoxIsEmtpy(void);

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
GBBOOL GBInputBox_ChangeInputMode(GBINT input_mode, GBU32 reserved);

/*!
 * \brief 通过物理按键（如'#'）循环切换输入法模式
 * \param void 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-3 9:27:25
*/
void GBInputBox_KeyChangeInputMode(void);

/*!
 * \brief 获取当前平台的输入法模式
 * \param void
 * \return mmi_imm_input_mode_enum
 * \注意：
 */
GBINT GBInputBox_GetCurrentInputMode(void);

/*!
 * \brief 获取下一个输入法模式
 * \param void
 * \return mmi_imm_input_mode_enum
 * \注意：
 */
GBINT GBInputBox_GetNextInputMode(void);

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
void GBInputBox_MoveCursor(GBInputBoxCursorDirection cursorDirection, GBINT step);

/*!
 * \brief 插入单个字符
 * \param character 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-3 9:28:05
*/
void GBInputBox_InsertChar(GBWCHAR character);

/*!
 * \brief 插入字符串
 * \param pInsertString 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-3 9:28:17
*/
void GBInputBox_InsertString(GBPWCHAR pInsertString);

/*!
 * \brief 删除单个字符
 * \param void 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-3 9:28:40
*/
void GBInputBox_DeleteChar(void);

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
GBUINT GBInputBox_ReplaceHighlightString(GBUINT nCandLength, GBPCWCHAR pCandString);

/*!
 * \brief 清除高亮
 * \param void 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-3 9:29:09
*/
void GBInputBox_ClearHilight(void);

/*!
 * \brief 高亮插入单个字符
 * \param pMultiTapString 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-3 9:29:56
*/
void GBInputBox_MultiTapInsertChar(GBPWCHAR pMultiTapString);

/*!
 * \brief 高亮删除指定长度内容
 * \param deleteLen 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-3 9:30:10
*/
void GBInputBox_MultiTapDelete(GBU32 deleteLen);

/*!
 * \brief 高亮输入确认
 * \param void 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-3 9:30:33
*/
void GBInputBox_MultitapInputConfirm(void);

/*!
 * \brief 设置LSK响应函数(左软键)
 * \param void 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-3 9:31:00
*/
void GBInputBox_SetPlatformLSK(void);

/*!
 * \brief 设置RSK响应函数(右软键)
 * \param void 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-3 9:33:52
*/
void GBInputBox_SetPlatformRSK(void);

/*!
 * \brief 设置方向键响应函数
 * \param void 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-3 9:34:28
*/
void GBInputBox_SetPlatformArrowKey(void);

/*!
 * \brief 设置KEY_ENTER键响应函数
 * \param void 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-3 9:34:47
*/
void GBInputBox_SetPlatformEnterKey(void);

/*!
 * \brief 设置平台【功能按键】响应函数
 * \param void 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-3 9:34:56
*/
void GBInputBox_SetAllPlatformFuncKey(void);

/*!
 * \brief 设置IME框架【功能按键】响应函数
 * \param void 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-3 9:35:28
*/
void GBInputBox_SetAllIMEFuncKey(void);

/*!
 * \brief 注册IME框架的按键响应函数
 * \param void 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-3 9:35:37
*/
void GBInputBox_RegisterIMEKeys(void);

/*!
 * \brief 清除所有按键回调
 * \param void 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-3 9:35:47
*/
void GBInputBox_ClearAllKey(void);

/*!
* \brief 重绘右软键
* \param void
* \return void
*/
void GBInputBox_RedrawRightSoftKey(void);

/*!
* \brief 重绘左软键
* \param void
* \return void
*/
void GBInputBox_RedrawLeftSoftKey(void);

/*!
* \brief 获取输入栏文本长度
* \param void
* \return GBUINT16
*/
GBUINT16 GBInputBox_GetTextLen(void);

/*!
 * \brief 根据状态重绘输入框界面
 * \param 
 * \return void
 * \注意：
 */
void GBInputBox_RedrawScreenByState(void);

/*!
 * \brief  重画输入框UI界面组件
 * \param imui_set
 * \return void
 * \注意：
 */
void GBInputBox_RedrawImui(GBInputBoxImuiControlType imuiControlType);

/*!
* \brief 设置右软键图标
* \param GBPUACHAR
* \return void
*/
void GBInputBox_SetRightSoftkeyIcon(GBPUACHAR icon);

/*!
* \brief 设置左软键图标
* \param GBPUACHAR
* \return void
*/
void GBInputBox_SetLeftSoftkeyIcon(GBPUACHAR icon);

/*!
* \brief 设置右软键提示字符
* \param GBPUINT16
* \return void
*/
void GBInputBox_SetRightSoftkeyLabel(GBPUINT16 s);

/*!
* \brief 设置左软键提示字符
* \param GBPUINT16
* \return void
*/
void GBInputBox_SetLeftSoftkeyLabel(GBPUINT16 s);

/*!
* \brief 隐藏右软键及其功能
* \param void
* \return void
*/
void GBInputBox_HideRightSoftkey(void);

/*!
* \brief 隐藏左软键及其功能
* \param void
* \return void
*/
void GBInputBox_HideLeftSoftkey(void);

/*!
 * \brief 更新平台Softkey标签
 * \param void 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-10-9 9:49:55
*/
void GBInputBox_UpdateSoftKeyLabel(void);

/*!
 * \brief 短按删除键处理函数
 * \param void 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-10-9 9:49:29
*/
void GBInputBox_DeletkeyHandler(void);

/*!
 * \brief 长按删除处理函数
 * \param void 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-10-9 9:49:00
*/
void GBInputBox_DeleteAllHandler(void);

/*!
 * \brief 判断是否需要改变编辑框的下拉高度
 * \param void 
 * \return 
 * \note 判断当前下拉高度和原始下拉高度是否相等。如果候选框开启的状态下，当前下拉高度和原始下拉高度一样，则需要改变高度
 * \example
 * \author Zhaokun
 * \date 2010-10-22 18:00:00
*/
GBBOOL GBInputBox_IsChangeHistoryHeight();

/*!
 * \brief 初始化平台输入框(获取平台应用相关的状态信息)
 * \param 
 * \return
 * \note 进入输入法编辑界面的时候调用(mmi_imc_connect)
 * \author weizhiping
 * \date 2010年2月5日
 */
GBBOOL GBInputBox_Init(void);

/*!
 * \brief 国笔输入法获取平台左软键显示标签
 * \param 
 * \return
 * \note 
 * \author weizhiping
 * \date 2010年2月5日
 */
UI_string_type GBInputBox_GetPlatformLeftSoftkeyLabel(void);

/*!
 * \brief 国笔输入法获取平台左软键显示标签
 * \param 
 * \return
 * \note 
 * \author weizhiping
 * \date 2010年2月5日
 */
UI_string_type GBInputBox_GetPlatformRightSoftkeyLabel(void);

/*!
 * \brief 编辑框是否处于只读文本查看模式
 * \param 
 * \return
 * \note 此时GBIME框架不接管消息处理
 * \author weizhiping
 * \date 2010年11月19日
 */
GBBOOL GBInputBox_IsReadOnlyTextView(void);

/*!
 * \brief 平台输入框处于纯数字输入模式（此时输入法框架禁止切换到其他输入法模式）
 * \param void 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-12-8 9:41:14
*/
GBBOOL GBInputBox_IsDigitInputOnly(void);

/*!
 * \brief 平台输入框处于快速查找输入模式（此时输入法框架禁止切换到其他输入法模式）
 * \param void 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-12-8 9:41:14
*/
GBBOOL GBInputBox_IsQuickSearchInputMode(void);

/*!
 * \brief 平台处理按键按下消息
 * \param KeyCode 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-12-30 17:05:52
 */
void GBInputBox_ProcessKeyDown(GBU16 KeyCode);

/*!
 * \brief 平台处理按键弹起消息
 * \param KeyCode 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-12-30 17:05:52
 */
void GBInputBox_ProcessKeyUp(GBU16 KeyCode);

/*!
 * \brief 平台处理按键消息，不需要V5引擎处理
 * \param void 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-12-30 18:29:15
 */
GBBOOL GBInputBox_PlatformHandleKeyMode(void);

#ifdef __cplusplus
};
#endif

#endif // __PLATFORM_INPUTBOX__
