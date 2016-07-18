/***************************************************************************
* File Name    :  CIMEInterface.h
* Corporation  :  Guobi Technology Ltd. GuangZhou China
* Copyright    :  Copyright Guobi Technology Ltd. And Subject to Confidentiality 
*                 Restrictions. This document is the confidential and proprietary 
*                 property of Guobi Technology Ltd.It is neither the document                  
*                 itself nor any part of it can be shared with any party  without 
*                 the written  permission. 
* Author       :  fanbo
* Date         :  2010-06-08
* Purpose      :  消息处理头文件
* Version      :  V1.0
* Last Changed List
* Name         |   date   |  Discription
****************************************************************************/
#ifndef __CMESSAGE_H_
#define __CMESSAGE_H_
#include "gbtype.h"
#include "gbdef.h"
#include "IMECommonData.h"
#include "CLayoutFactory.h"
#include "CLayoutControl.h"
#include "CSwitchLayout.h"
#include "CSwitchInputMethod.h"
#include "CConfigure.h"
#include "CGBEngine.h"
#include "CHWEngine.h"

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * \ 保存在内存中的最大Layout数目[这样就不需要在频繁切换的时候反复创建]
 */
#define MAX_LEAVE_LAYOUT_OBJECT_NUM   10

typedef struct __tag_CIMEInterface
{
	GBBOOL				bFrameworkInited; ///<框架是否初始化
	GBBOOL				bIMEConnected; ///<MTK平台输入法模块是否连接(mmi_imc_connect/mmi_imc_disconnect设置)
	IMECommonData       comIMEData;

	GBBOOL				layoutWinActiveStatus; ///<Layout窗口是否处于激活状态
	GBIMEPoint			layoutWinStartPos; ///< layout窗口的起始显示位置(即左下角相对于屏幕的坐标，因为左上角会随着候选框高度变化)
	GBINT				layoutWinWidth;///<Layout显示宽度(初始化设定==屏幕宽度)
	GBINT				layoutWinHeight;///<Layout显示高度(动态更新)

	GBShiftCapStatus	shiftCapStatus;///<大小写状态标记(一种遵循框架实现的保证候选和Layout键盘大小写一致的方法) 

	GBBOOL				bVk2KbSwitchFlag;///<虚拟键盘切换到物理键盘标记
	GBINT				Vk2KbSwitchEngineKeyValue;///<虚拟键盘切换到物理键盘时用户按下的物理按键键值

	GBUINT16			latestMasterLayoutID;///<最近的主LayoutID(如: 主语言输入法)
	GBUINT16			latestChnNumLayoutID;///<最近的中文数字键盘LayoutID
	GBUINT16			latestChnQwertyLayoutID;///<最近的中文Qwerty键盘LayoutID
	GBUINT16			latestAlpNumLayoutID;///<最近的拉丁数字键盘LayoutID
	GBUINT16			latestAlpQwertyLayoutID;///<最近的拉丁Qwerty键盘LayoutID
	PCLayoutAbstract    pltCurObject;
	PCLayoutAbstract    pltPreObject;
	PCLayoutAbstract    ltObjectArray[MAX_LEAVE_LAYOUT_OBJECT_NUM];
	CLayoutControl		*pltCtl;
	CSwitchLayout		*pschLt;
	CSWitchInputMethod	*pschIM;
	CFactory			*pfactory;
	CConfig				*pConfig;
	CGBEngine			gbIMEngine; // 使用系统的动态内存分配最多2KB
#if defined(__GBIME_HANDWRITE__)
	PGBHWEngine 		pGBHWEngine;
#endif
	GBIMEEventGroup		sysImeEventGroup;//<由平台事件经过Layout解析后从gbIMEEventConvertMap取得的事件组
	GBIMEEventGroup		sysImePostEventGroup;//<Layout处理过程中产生的后处理事件组
	GBBOOL				sysImeEventGroupLock;///<防止多个地方同时填充sysImeEventGroup
	GBBOOL				sysImePostEventGroupLock;///<防止多个地方同时填充sysImePostEventGroup
}CIMEInterface, *PCIMEInterface;

/*!
 * \brief 初始化框架接口对象
 * \param void 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-8-11 9:33:50
*/
GBAPI void Interface_InitIME(void);

/*!
 * \brief 判断框架接口对象是否已经初始化
 * \param void 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-8-11 9:33:50
*/
GBAPI GBBOOL Interface_IsIMEInited(void);

/*!
 * \brief 获取IME模块连接状态
 * \param void 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-8-11 9:33:50
*/
GBAPI GBBOOL Interface_IsIMEConnected(void);

/*!
 * \brief 设置IME模块连接状态
 * \param GBBOOL 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-8-11 9:33:50
*/
GBAPI void Interface_SetIMEConnectedStatus(GBBOOL status);

/*!
 * \brief 消息处理函数(由具体Layout实例对象负责将平台消息转换为IME框架消息)
 * \param nPlatformEvent 
 * \param wParam 
 * \param lParam 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-6-22 14:58:51
 */
GBAPI GBINT32 Handle_GBIMEInterface(GBINT nPlatformEvent, GBINT wParam, GBINT lParam);

/*!
 * \brief IME 框架消息处理函数
 * \param pIMEEvent 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-6-22 14:58:27
 */
GBAPI GBINT32 Handle_GBIMEMessage(PGBIMEEvent pIMEEvent);

/*!
 * \brief 获取国笔引擎实例对象指针
 * \param void 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-26 15:31:24
 */
PCGBEngine Interface_GetEnineInstance(void);

/*!
 * \brief 设置输入法Layout窗口的起始显示位置(即左下角相对于屏幕的坐标，因为左上角会随着候选框高度变化)
 * \param pWinPos 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-21 11:28:23
 */
GBIMEReturn Interface_SetWinPos(GBIMEPoint *pWinPos);

/*!
 * \brief 获取输入法Layout窗口的左上角位置(左下角起始显示位置 + Layout窗口显示高度)
 * \param pWinPos 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-21 11:28:37
 */
GBIMEReturn Interface_GetWinPos(GBIMEPoint *pWinPos);

/*!
 * \brief 设置输入法窗口的显示高度
 * \param newHeight 
 * \note 高度发生变化时需要通知MTK平台编辑器进行刷新，否则会导致内容区和输入法窗口刷新不完整
 * \return 
 */
void Interface_SetWinHeight(GBINT newHeight);

/*!
 * \brief 获取输入法窗口的显示高度
 * \param void 
 * \note 高度发生变化时需要通知MTK平台编辑器进行刷新，否则会导致内容区和输入法窗口刷新不完整
 * \return 
 */
GBINT Interface_GetWinHeight(void);

/*!
 * \brief 获取输入法窗口的显示宽度
 * \param newWidth 
 * \note 宽度发生变化时需要通知MTK平台编辑器进行刷新，否则会导致内容区和输入法窗口刷新不完整
 * \return 
 */
void Interface_SetWinWidth(GBINT newWidth);

/*!
 * \brief 获取输入法窗口的显示宽度
 * \param void 
 * \note 宽度发生变化时需要通知MTK平台编辑器进行刷新，否则会导致内容区和输入法窗口刷新不完整
 * \return 
 */
GBINT Interface_GetWinWidth(void);

/*!
 * \brief 获取Layout窗口激活状态
 * \param void 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-8-11 11:45:39
*/
GBBOOL Interface_GetLayoutWinActiveStatus(void);

/*!
 * \brief 设置Layout窗口激活状态
 * \param status 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-8-11 11:45:39
*/
void Interface_SetLayoutWinActiveStatus(GBBOOL status);

#ifdef __cplusplus
};
#endif
#endif //__CMESSAGE_H_
