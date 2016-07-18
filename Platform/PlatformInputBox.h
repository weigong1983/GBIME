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
* Purpose      :  ��װMTKƽ̨�������ع��ܺ���
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
 * \brief �������������
 */
typedef enum {
	GBSinglineInputBox	 	///<����
	,GBMultilineInputBox	///<����
	,GBEMSInputBox 			///<����
	,GBInlineInputBox
	,GBUnsupportInputBox	///<��֧��
}GBInputBoxType;

/*!
 * \brief ����������귽��
 */
typedef enum {
	GBINPUTBOX_CURSOR_UP		///<��
	,GBINPUTBOX_CURSOR_DOWN		///<��
	,GBINPUTBOX_CURSOR_LEFT		///<��
	,GBINPUTBOX_CURSOR_RIGHT	///<��
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
 * \brief ��ʾ�������뷨���������̡��ﵽ���µ��������������ʾ����ߴ��Ŀ�ģ���ǰ�õ�GBInputBox_Resize����ִ���ˢ�����⡿
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
 * \brief ���ع������뷨����Լ����������
 * \param void 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-11-11 19:53:06
 */
void GBInputBox_HideVirtualKeyboard(void);

/*!
 * \brief ������ѡ�������
 * \param pUpScreenStr 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-3 9:26:18
*/
GBBOOL GBInputBox_UpScreenString(GBPCWCHAR pUpScreenStr);

/*!
 * \brief �ػ������
 * \param void 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-3 9:26:28
*/
void GBInputBox_Redraw(void);

/*!
 * \brief ��ȡ�����༭������
 * \param pEditRect 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-3 9:26:44
*/
void GBInputBox_GetEditRect(GBRECT *pEditRect);

/*!
 * \brief �������
 * \param void 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-3 9:26:56
*/
void GBInputBox_DeleteAll(void);

/*!
 * \brief �ж�������Ƿ��
 * \param void 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-3 9:27:10
*/
GBBOOL GBInputBox_InputBoxIsEmtpy(void);

/*!
 * \brief �л���ָ�����뷨ģʽ
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
 * \brief ͨ������������'#'��ѭ���л����뷨ģʽ
 * \param void 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-3 9:27:25
*/
void GBInputBox_KeyChangeInputMode(void);

/*!
 * \brief ��ȡ��ǰƽ̨�����뷨ģʽ
 * \param void
 * \return mmi_imm_input_mode_enum
 * \ע�⣺
 */
GBINT GBInputBox_GetCurrentInputMode(void);

/*!
 * \brief ��ȡ��һ�����뷨ģʽ
 * \param void
 * \return mmi_imm_input_mode_enum
 * \ע�⣺
 */
GBINT GBInputBox_GetNextInputMode(void);

/*!
 * \brief �ƶ����λ��
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
 * \brief ���뵥���ַ�
 * \param character 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-3 9:28:05
*/
void GBInputBox_InsertChar(GBWCHAR character);

/*!
 * \brief �����ַ���
 * \param pInsertString 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-3 9:28:17
*/
void GBInputBox_InsertString(GBPWCHAR pInsertString);

/*!
 * \brief ɾ�������ַ�
 * \param void 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-3 9:28:40
*/
void GBInputBox_DeleteChar(void);

/*!
 * \brief �����滻�ַ���
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
 * \brief �������
 * \param void 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-3 9:29:09
*/
void GBInputBox_ClearHilight(void);

/*!
 * \brief �������뵥���ַ�
 * \param pMultiTapString 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-3 9:29:56
*/
void GBInputBox_MultiTapInsertChar(GBPWCHAR pMultiTapString);

/*!
 * \brief ����ɾ��ָ����������
 * \param deleteLen 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-3 9:30:10
*/
void GBInputBox_MultiTapDelete(GBU32 deleteLen);

/*!
 * \brief ��������ȷ��
 * \param void 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-3 9:30:33
*/
void GBInputBox_MultitapInputConfirm(void);

/*!
 * \brief ����LSK��Ӧ����(�����)
 * \param void 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-3 9:31:00
*/
void GBInputBox_SetPlatformLSK(void);

/*!
 * \brief ����RSK��Ӧ����(�����)
 * \param void 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-3 9:33:52
*/
void GBInputBox_SetPlatformRSK(void);

/*!
 * \brief ���÷������Ӧ����
 * \param void 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-3 9:34:28
*/
void GBInputBox_SetPlatformArrowKey(void);

/*!
 * \brief ����KEY_ENTER����Ӧ����
 * \param void 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-3 9:34:47
*/
void GBInputBox_SetPlatformEnterKey(void);

/*!
 * \brief ����ƽ̨�����ܰ�������Ӧ����
 * \param void 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-3 9:34:56
*/
void GBInputBox_SetAllPlatformFuncKey(void);

/*!
 * \brief ����IME��ܡ����ܰ�������Ӧ����
 * \param void 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-3 9:35:28
*/
void GBInputBox_SetAllIMEFuncKey(void);

/*!
 * \brief ע��IME��ܵİ�����Ӧ����
 * \param void 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-3 9:35:37
*/
void GBInputBox_RegisterIMEKeys(void);

/*!
 * \brief ������а����ص�
 * \param void 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-3 9:35:47
*/
void GBInputBox_ClearAllKey(void);

/*!
* \brief �ػ������
* \param void
* \return void
*/
void GBInputBox_RedrawRightSoftKey(void);

/*!
* \brief �ػ������
* \param void
* \return void
*/
void GBInputBox_RedrawLeftSoftKey(void);

/*!
* \brief ��ȡ�������ı�����
* \param void
* \return GBUINT16
*/
GBUINT16 GBInputBox_GetTextLen(void);

/*!
 * \brief ����״̬�ػ���������
 * \param 
 * \return void
 * \ע�⣺
 */
void GBInputBox_RedrawScreenByState(void);

/*!
 * \brief  �ػ������UI�������
 * \param imui_set
 * \return void
 * \ע�⣺
 */
void GBInputBox_RedrawImui(GBInputBoxImuiControlType imuiControlType);

/*!
* \brief ���������ͼ��
* \param GBPUACHAR
* \return void
*/
void GBInputBox_SetRightSoftkeyIcon(GBPUACHAR icon);

/*!
* \brief ���������ͼ��
* \param GBPUACHAR
* \return void
*/
void GBInputBox_SetLeftSoftkeyIcon(GBPUACHAR icon);

/*!
* \brief �����������ʾ�ַ�
* \param GBPUINT16
* \return void
*/
void GBInputBox_SetRightSoftkeyLabel(GBPUINT16 s);

/*!
* \brief �����������ʾ�ַ�
* \param GBPUINT16
* \return void
*/
void GBInputBox_SetLeftSoftkeyLabel(GBPUINT16 s);

/*!
* \brief ������������书��
* \param void
* \return void
*/
void GBInputBox_HideRightSoftkey(void);

/*!
* \brief ������������书��
* \param void
* \return void
*/
void GBInputBox_HideLeftSoftkey(void);

/*!
 * \brief ����ƽ̨Softkey��ǩ
 * \param void 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-10-9 9:49:55
*/
void GBInputBox_UpdateSoftKeyLabel(void);

/*!
 * \brief �̰�ɾ����������
 * \param void 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-10-9 9:49:29
*/
void GBInputBox_DeletkeyHandler(void);

/*!
 * \brief ����ɾ��������
 * \param void 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-10-9 9:49:00
*/
void GBInputBox_DeleteAllHandler(void);

/*!
 * \brief �ж��Ƿ���Ҫ�ı�༭��������߶�
 * \param void 
 * \return 
 * \note �жϵ�ǰ�����߶Ⱥ�ԭʼ�����߶��Ƿ���ȡ������ѡ������״̬�£���ǰ�����߶Ⱥ�ԭʼ�����߶�һ��������Ҫ�ı�߶�
 * \example
 * \author Zhaokun
 * \date 2010-10-22 18:00:00
*/
GBBOOL GBInputBox_IsChangeHistoryHeight();

/*!
 * \brief ��ʼ��ƽ̨�����(��ȡƽ̨Ӧ����ص�״̬��Ϣ)
 * \param 
 * \return
 * \note �������뷨�༭�����ʱ�����(mmi_imc_connect)
 * \author weizhiping
 * \date 2010��2��5��
 */
GBBOOL GBInputBox_Init(void);

/*!
 * \brief �������뷨��ȡƽ̨�������ʾ��ǩ
 * \param 
 * \return
 * \note 
 * \author weizhiping
 * \date 2010��2��5��
 */
UI_string_type GBInputBox_GetPlatformLeftSoftkeyLabel(void);

/*!
 * \brief �������뷨��ȡƽ̨�������ʾ��ǩ
 * \param 
 * \return
 * \note 
 * \author weizhiping
 * \date 2010��2��5��
 */
UI_string_type GBInputBox_GetPlatformRightSoftkeyLabel(void);

/*!
 * \brief �༭���Ƿ���ֻ���ı��鿴ģʽ
 * \param 
 * \return
 * \note ��ʱGBIME��ܲ��ӹ���Ϣ����
 * \author weizhiping
 * \date 2010��11��19��
 */
GBBOOL GBInputBox_IsReadOnlyTextView(void);

/*!
 * \brief ƽ̨������ڴ���������ģʽ����ʱ���뷨��ܽ�ֹ�л����������뷨ģʽ��
 * \param void 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-12-8 9:41:14
*/
GBBOOL GBInputBox_IsDigitInputOnly(void);

/*!
 * \brief ƽ̨������ڿ��ٲ�������ģʽ����ʱ���뷨��ܽ�ֹ�л����������뷨ģʽ��
 * \param void 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-12-8 9:41:14
*/
GBBOOL GBInputBox_IsQuickSearchInputMode(void);

/*!
 * \brief ƽ̨������������Ϣ
 * \param KeyCode 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-12-30 17:05:52
 */
void GBInputBox_ProcessKeyDown(GBU16 KeyCode);

/*!
 * \brief ƽ̨������������Ϣ
 * \param KeyCode 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-12-30 17:05:52
 */
void GBInputBox_ProcessKeyUp(GBU16 KeyCode);

/*!
 * \brief ƽ̨��������Ϣ������ҪV5���洦��
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
