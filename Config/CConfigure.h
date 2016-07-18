/***************************************************************************
* File Name    :  CConfigure.h
* Corporation  :  Guobi Technology Ltd. GuangZhou China
* Copyright    :  Copyright Guobi Technology Ltd. And Subject to Confidentiality 
*                 Restrictions. This document is the confidential and proprietary 
*                 property of Guobi Technology Ltd.It is neither the document                  
*                 itself nor any part of it can be shared with any party  without 
*                 the written  permission. 
* Author       :  fanbo
* Date         :  2010-06-08
* Purpose      :  配置头文件
* Version      :  V1.0
* Last Changed List
* Name         |   date   |  Discription
****************************************************************************/
#ifndef __CONFIGURE_H_
#define __CONFIGURE_H_
#include "gbtype.h"
//#include "CEngineConfig.h"
#include "CLayoutConfigAbstract.h"

typedef enum
{
	TYPE_PEN
	,TYPE_PENCIL
	,TYPE_SYSTEM
	,COLOR_RED
	,COLOR_GREEN
	,COLOR_BLUE
	,COLOR_YELLOW
	,COLOR_PURPLE
	,COLOR_GRAY
	,COLOR_BLACK
	,WIDTH_LARGE
	,WIDTH_MEDIUM
	,WIDTH_SMALL
	,SPEED_SLOW
	,SPEED_MEDIUM
	,SPEED_FAST
}HANDWRITING_ENUM;

/* 配置需求分析：
1.	每个Layout类对应一个Layout视图类、Layout配置类
2.	Layout配置类控制域：是否支持滑屏切换(虚拟键盘)；响应DOWN还是UP事件；
3.	Layout切换列表管理?
	(1)	有些Layout是语种敏感的；有些则不是（如：ABC, 123可复用到多个语种）
	(2)	需要明确的是：其实在编译时已经决定了哪些Layout具体类的.c实现代码参加编译，即决定了支持哪些Layout；
	(3)	可通过一个默认配置表填写手机支持的每个语种的【默认Layout切换列表】；
	(4)	系统第一次开机加载到内存全局配置的是【默认Layout切换列表】，之后可在内存中动态修改，例如：关闭/打开/调整次序
	(5)	切换信息以语种ID为标识存储到文件系统中，格式如：langID: CHN; layoutType: layoutKB status: ON/OFF, …
4. 每个Layout子类信息记录一份
*/

/*!
 * \brief 配置总结构体 
 */
typedef struct _tagCCConfig
{
	GBBOOL					bHasInit;							///<配置数据是否已初始化
//	CEngineConfig			engineConfig;						///<国笔引擎相关配置

	// Layout全局配置
	KEYBOARD_TYPE			m_keyboardType;						///<键盘类别
	GBUINT16				m_preferLayoutID;					///<当前键盘类型下的缺省LayoutID
	GBConfigRespondKeyType	m_respondKeyType;					///<响应按键类别(按键按下还是弹起时响应)
	GBConfigSwitchType		m_bSupportSlidingSwitchLayout;		///<虚拟键盘是否支持滑屏切换Layout	
}CConfig, *PCConfig;

/*!
 * \brief 加载国笔引擎配置
 * \param  pConfig
 * \return 
 * \note 
 * \example
 * \author fanbo
 * \date
 */
GBIMEReturn  GBConfig_Load(CConfig *pConfig);

/*!
 * \brief 卸载国笔引擎配置
 * \param  pConfig
 * \return 
 * \note 
 * \example
 * \author fanbo
 * \date
 */
GBIMEReturn  GBConfig_Unload(CConfig *pConfig);

/*!
 * \brief 配置消息处理函数
 * \param pIMEEvent 
 * \return 
 */
GBIMEReturn GBConfig_HandleMessage(PGBIMEEvent pIMEEvent);

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
GBIMEReturn  GBConfig_SetKeyboardType(CConfig *pConfig, KEYBOARD_TYPE keyboardType);

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
KEYBOARD_TYPE  GBConfig_GetKeyboardType(CConfig *pConfig);

/*!
 * \brief 获取缺省LayoutID
 * \param pConfig 
 * \return 缺省LayoutID
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-8-19 20:18:24
*/
GBUINT16 GBConfig_GetPreferLayoutID(CConfig *pConfig);

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
GBUINT16 GBConfig_GetDefLatestAlpLayoutID(CConfig *pConfig, KEYBOARD_TYPE kbType);

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
GBUINT16 GBConfig_GetDefLatestChnLayoutID(CConfig *pConfig, KEYBOARD_TYPE kbType);

#endif //__CONFIGURE_H_