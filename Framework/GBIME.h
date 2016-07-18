/***************************************************************************
* File Name    :  GBIME.h
* Corporation  :  Guobi Technology Ltd. GuangZhou China
* Copyright    :  Copyright Guobi Technology Ltd. And Subject to Confidentiality 
*                 Restrictions. This document is the confidential and proprietary 
*                 property of Guobi Technology Ltd.It is neither the document                  
*                 itself nor any part of it can be shared with any party  without 
*                 the written  permission. 
* Author       :  weizhiping
* Date         :  2010-06-08
* Purpose      :  输入法框架适配层（适配框架，提供API供平台调用，平台无需知道框架实现细节）
* Version      :  V1.0
* Last Changed List
* Name         |   date   |  Discription
****************************************************************************/
#ifndef __GBIME_H_
#define __GBIME_H_
#include "gbtype.h"
#include "GBIMEDef.h"
#include "CIMEInterface.h"
#include "CConfigure.h"

/*!
* \brief 国笔输入法框架创建
* \param void
* \return 
* \note 
* \example
* \author fanbo
* \date 2010-06-08
*/
GBIMEAPI GBIMEReturn GBIME_Create(void);

/*!
 * \brief 国笔输入法框架和平台输入界面连接(切换到首选默认输入模式并创建对应Layout显示面板)
 * \param platformInputMode 平台启动输入法时的默认输入法模式
 * \return 
 * \note 必须先调用GBIME_Create
 * \example
 * \author weizhiping
 * \date 2010-8-20 10:20:58
*/
GBIMEAPI GBIMEReturn GBIME_Connect(GBINT platformInputMode);

/*!
 * \brief 国笔输入法框架切换输入模式
 * \param platformInputMode
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-8-20 10:20:58
*/
GBIMEAPI GBIMEReturn GBIME_SwitchInputMode(GBINT platformInputMode);

/*!
 * \brief 国笔输入法框架和平台输入界面断开连接
 * \param void 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-8-31 14:17:17
*/
GBIMEAPI GBIMEReturn GBIME_Disconnect(void);

/*!
* \brief 国笔输入法框架销毁
* \return 
* \note 
* \example
* \author fanbo
* \date 2010-06-08
*/
GBIMEAPI GBIMEReturn GBIME_Destroy(void);

/*!
 * \brief 平台输入法框架物理按键按下处理函数
 * \param key_code 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-22 14:09:23
 */
GBIMEAPI GBIMEReturn GBIME_KeyDownHandler(GBINT key_code);

/*!
 * \brief 平台输入法框架物理按键弹起处理函数
 * \param key_code 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-22 14:09:23
 */
GBIMEAPI GBIMEReturn GBIME_KeyUpHandler(GBINT key_code);

/*!
 * \brief 平台输入法框架物理按键长按处理函数
 * \param key_code 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-22 14:09:23
 */
GBIMEAPI GBIMEReturn GBIME_KeyLongPressHandler(GBINT key_code);

/*!
 * \brief 平台输入法框架物理按键快速短按处理函数
 * \param key_code 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-22 14:09:23
 */
GBIMEAPI GBIMEReturn GBIME_KeyRepeatHandler(GBINT key_code);

/*!
 * \brief 平台输入法框架触摸屏按下处理函数
 * \param x
 * \param y
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-22 14:11:58
*/
GBIMEAPI GBIMEReturn GBIME_PenDownHandler(GBINT x, GBINT y);

/*!
 * \brief 平台输入法框架触摸屏弹起处理函数
 * \param x
 * \param y
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-22 14:11:58
*/
GBIMEAPI GBIMEReturn GBIME_PenUpHandler(GBINT x, GBINT y);

/*!
 * \brief 平台输入法框架触摸屏Move处理函数
 * \param x
 * \param y
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-22 14:11:58
*/
GBIMEAPI GBIMEReturn GBIME_PenMoveHandler(GBINT x, GBINT y);

/*!
 * \brief 国笔输入法面板强制刷新显示
 * \return 
 * \note 
 * \example
 * \author fanbo
 * \date 2010-06-08
 */
GBIMEAPI GBIMEReturn GBIME_Show(void);

/*!
 * \brief 国笔输入法面板隐藏
 * \return 
 * \note 
 * \example
 * \author fanbo
 * \date 2010-06-08
 */
GBIMEAPI GBIMEReturn GBIME_Hide(void);

/*!
 * \brief 国笔输入法面板强制刷新显示
 * \param void 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-17 11:17:16
*/
GBIMEAPI GBIMEReturn GBIME_Paint(void);

/*!
 * \brief 设置输入法窗口的的起始显示位置(即左下角相对于屏幕的坐标，因为左上角会随着候选框高度变化)
 * \param pWinPos 
 * \note 可在框架初始化时设置，也可以动态设置
 * \return 
 */
GBIMEAPI GBIMEReturn GBIME_SetWinPos(GBIMEPoint *pWinPos);

/*!
 * \brief 获取输入法窗口的左上角位置(左下角起始显示位置 + Layout窗口显示高度)
 * \param pWinPos 
 * \note 框架处于显示状态才有效
 * \return 
 */
GBIMEAPI GBIMEReturn GBIME_GetWinPos(GBIMEPoint *pWinPos);

/*!
 * \brief 获取输入法窗口的显示
 * \param void 
 * \note 高度发生变化时需要通知MTK平台编辑器进行刷新，否则会导致内容区和输入法窗口刷新不完整
 * \return 
 */
GBIMEAPI GBINT GBIME_GetWinHeight(void);

/*!
 * \brief 获取输入法窗口的显示
 * \param void 
 * \note 宽度发生变化时需要通知MTK平台编辑器进行刷新，否则会导致内容区和输入法窗口刷新不完整
 * \return 
 */
GBIMEAPI GBINT GBIME_GetWinWidth(void);

/*!
 * \brief 判断坐标点是否落在Layout窗口区域内
 * \param point 
 * \return 
 */
GBIMEAPI GBBOOL GBIME_PointIsInLayoutRect(GBIMEPoint point);

/*!
 * \brief 判断输入法框架是否需要处理此触摸屏消息
 * \param nPenEvent 
 * \param point 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-11-22 20:07:44
 */
 GBIMEAPI GBBOOL GBIME_ShouldHandlePenEvent(GBINT nPenEvent, GBIMEPoint point);

/*!
 * \brief 获取当前键盘类型
 * \param void 
 * \return KEYBOARD_TYPE
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-10-28
*/
GBIMEAPI KEYBOARD_TYPE GBIME_GetKeyboardType(void);

#if __TEST_LOG_TO_FILE__ > 0
//测试时使用，专门打印log
GBIMEAPI GBINT GBIME_OpenFile(char* filePath);
GBIMEAPI void GBIME_CloseFile();
#endif

/*!
 * \brief 设置国笔输入法框架虚拟键盘
 * \param input_mode 
 * \note 
 * \return 
 */
//void GBIME_SetVirtualKeyboard(mmi_imm_input_mode_enum input_mode);
#endif //__GBIME_H_