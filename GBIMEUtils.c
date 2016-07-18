/***************************************************************************
* File Name    :  GBIMEUtils.c
* Corporation  :  Guobi Technology Ltd. GuangZhou China
* Copyright    :  Copyright Guobi Technology Ltd. And Subject to Confidentiality 
*                 Restrictions. This document is the confidential and proprietary 
*                 property of Guobi Technology Ltd.It is neither the document                  
*                 itself nor any part of it can be shared with any party  without 
*                 the written  permission. 
* Author       :  weizhiping
* Date         :  2010-07-16
* Purpose      :  框架内部工具库
* Version      :  V1.0
* Last Changed List
* Name         |   date   |  Discription
****************************************************************************/
#include "GBIMEUtils.h"
#include "PlatformTools.h"

/*!
 * \brief 用于简化m_pfcPreHandleEvent处理，程序根据事件转换值检索转换表，获取对应GBIMEEvent组成的事件集
 */
static const GBIMEEventConvertMap gbIMEEventConvertMap[] = 
{
	// Layout相关
	// Layout按钮按下， 仅作刷新： Layout按下->刷新Layout
	{GBIME_CT_KEY_DOWN_REFRESH,	{{GBIME_EVENT_TYPE_LAYOUT, GBIME_EVENT_LAYOUT_PEN_DOWN}, {GBIME_EVENT_TYPE_LAYOUT, GBIME_EVENT_LAYOUT_REFRESH}}, 2},
	// Layout按钮弹起， 仅作刷新： Layout弹起->刷新Layout
	{GBIME_CT_KEY_UP_REFRESH,	{{GBIME_EVENT_TYPE_LAYOUT, GBIME_EVENT_LAYOUT_PEN_UP}, {GBIME_EVENT_TYPE_LAYOUT, GBIME_EVENT_LAYOUT_REFRESH}}, 2},
	// Layout按钮按下响应常规引擎按键事件： 引擎按键->Layout按下->刷新Layout
	{GBIME_CT_NORMAL_KEY_DOWN,	{{GBIME_EVENT_TYPE_ENGINE, GBIME_EVENT_ENGINE_KEY}, {GBIME_EVENT_TYPE_LAYOUT, GBIME_EVENT_LAYOUT_PEN_DOWN}, {GBIME_EVENT_TYPE_LAYOUT, GBIME_EVENT_LAYOUT_REFRESH}}, 3},
	// Layout按钮弹起响应常规引擎按键事件： 引擎按键->Layout弹起->刷新Layout
	{GBIME_CT_NORMAL_KEY_UP,	{{GBIME_EVENT_TYPE_ENGINE, GBIME_EVENT_ENGINE_KEY}, {GBIME_EVENT_TYPE_LAYOUT, GBIME_EVENT_LAYOUT_PEN_UP}, {GBIME_EVENT_TYPE_LAYOUT, GBIME_EVENT_LAYOUT_REFRESH}}, 3},
	// Layout 面板上面滑动： Layout Move->刷新Layout
	{GBIME_CT_MOVE,			{{GBIME_EVENT_TYPE_LAYOUT, GBIME_EVENT_LAYOUT_PEN_MOVE}, {GBIME_EVENT_TYPE_LAYOUT, GBIME_EVENT_LAYOUT_REFRESH}}, 2},
	// Layout切换： 引擎状态重置->输入法切换->Layout切换->刷新Layout
	{GBIME_CT_SWITCH,		
	 { {GBIME_EVENT_TYPE_ENGINE, GBIME_EVENT_ENGINE_RESET}
	  ,{GBIME_EVENT_TYPE_LAYOUT, GBIME_EVENT_LAYOUT_EXIT} // 退出当前Layout 
	  ,{GBIME_EVENT_TYPE_INPUT_METHOD, GBIME_EVENT_INPUT_METHOD_SWITCH_NOTIFY} /*通知平台，走平台切换流程*/
	  ,{GBIME_EVENT_TYPE_LAYOUT, GBIME_EVENT_LAYOUT_ENTER} // 进入目标Layout 
	  ,{GBIME_EVENT_TYPE_LAYOUT, GBIME_EVENT_LAYOUT_REFRESH}}
	, 5},
	// Layout 显示（激活）
	{GBIME_CT_SHOW,		{{GBIME_EVENT_TYPE_LAYOUT, GBIME_EVENT_LAYOUT_SHOW}, {GBIME_EVENT_TYPE_LAYOUT, GBIME_EVENT_LAYOUT_REFRESH}}, 2},
	// Layout 隐藏：引擎状态重置->隐藏->刷新
	{GBIME_CT_HIDE,		{{GBIME_EVENT_TYPE_ENGINE, GBIME_EVENT_ENGINE_RESET}, {GBIME_EVENT_TYPE_LAYOUT, GBIME_EVENT_LAYOUT_HIDE}, {GBIME_EVENT_TYPE_LAYOUT, GBIME_EVENT_LAYOUT_REFRESH}}, 3},

	// 手写相关
	// 绘制手写识别出来的候选: 更新手写候选->刷新Layout
	{GBIME_CT_HANDWRITE_DRAW_CANDIDATE, {{GBIME_EVENT_TYPE_HANDWRITE, GBIME_EVENT_HANDWRITE_DRAW_CANDIDATE}, {GBIME_EVENT_TYPE_LAYOUT, GBIME_EVENT_LAYOUT_REFRESH}}, 2},
	// 绘制联想字符串: 引擎获取联想词->绘制联想词->刷新Layout
	{GBIME_CT_HANDWRITE_DRAW_ASSOCWORD, {{GBIME_EVENT_TYPE_ENGINE, GBIME_EVENT_ENGINE_GET_ASSOCWORD}, {GBIME_EVENT_TYPE_HANDWRITE, GBIME_EVENT_HANDWRITE_DRAW_ASSOCWORD}, {GBIME_EVENT_TYPE_LAYOUT, GBIME_EVENT_LAYOUT_REFRESH}}, 3},
	// 设置手写识别范围: 设置手写识别范围->刷新Layout
	{GBIME_CT_HANDWRITE_SET_RECOGNIZE_RANGE, {{GBIME_EVENT_TYPE_HANDWRITE, GBIME_EVENT_HANDWRITE_SET_RECOGNIZE_RANGE}, {GBIME_EVENT_TYPE_LAYOUT, GBIME_EVENT_LAYOUT_REFRESH}}, 2},

	// 配置相关
	// 进入配置菜单
	{GBIME_CT_CONFIG_MENU, {{GBIME_EVENT_TYPE_CONFIG, GBIME_EVENT_CONFIG_MENU}}, 1},
	
	// 物理按键相关
	// 按键按下响应常规引擎按键事件： 引擎按键->物理按键按下->刷新Layout
	{GBIME_CT_KEY_DOWN,	{{GBIME_EVENT_TYPE_ENGINE, GBIME_EVENT_ENGINE_KEY}, {GBIME_EVENT_TYPE_LAYOUT, GBIME_EVENT_KEY_DOWN}, {GBIME_EVENT_TYPE_LAYOUT, GBIME_EVENT_LAYOUT_REFRESH}}, 3},
	// 按键弹起响应常规引擎按键事件： 引擎按键->物理按键弹起->刷新Layout
	{GBIME_CT_KEY_UP,	{{GBIME_EVENT_TYPE_ENGINE, GBIME_EVENT_ENGINE_KEY}, {GBIME_EVENT_TYPE_LAYOUT, GBIME_EVENT_KEY_UP}, {GBIME_EVENT_TYPE_LAYOUT, GBIME_EVENT_LAYOUT_REFRESH}}, 3},
	// 按键长按响应常规引擎按键事件： 引擎按键->物理按键弹起->刷新Layout
	{GBIME_CT_KEY_LONG_PRESS,	{{GBIME_EVENT_TYPE_ENGINE, GBIME_EVENT_ENGINE_KEY}, {GBIME_EVENT_TYPE_LAYOUT, GBIME_EVENT_KEY_UP}, {GBIME_EVENT_TYPE_LAYOUT, GBIME_EVENT_LAYOUT_REFRESH}}, 3},
	
	// 虚拟键盘 -> 物理键盘
	{GBIME_CT_VK_2_KB_SWITCH, 
	{ {GBIME_EVENT_TYPE_ENGINE, GBIME_EVENT_ENGINE_RESET}
	,{GBIME_EVENT_TYPE_LAYOUT, GBIME_EVENT_LAYOUT_EXIT} // 退出当前Layout 
  	, {GBIME_EVENT_TYPE_INPUT_METHOD, GBIME_EVENT_INPUT_METHOD_SWITCH_NOTIFY}/*Layout切换完毕再同步平台输入模式刷新VK*/
	, {GBIME_EVENT_TYPE_LAYOUT, GBIME_EVENT_LAYOUT_SHOW} // 必须先显示出来
	, {GBIME_EVENT_TYPE_LAYOUT, GBIME_EVENT_LAYOUT_ENTER} // 进入目标Layout 
 	, {GBIME_EVENT_TYPE_ENGINE, GBIME_EVENT_ENGINE_KEY}	
 	, {GBIME_EVENT_TYPE_LAYOUT, GBIME_EVENT_KEY_DOWN}
 	, {GBIME_EVENT_TYPE_LAYOUT, GBIME_EVENT_LAYOUT_REFRESH}	
	}
	, 8},

	// 物理键盘 -> 虚拟键盘
	{GBIME_CT_KB_2_VK_SWITCH, 
	{ {GBIME_EVENT_TYPE_ENGINE, GBIME_EVENT_ENGINE_RESET}
	,{GBIME_EVENT_TYPE_LAYOUT, GBIME_EVENT_LAYOUT_EXIT} // 退出当前Layout 
	, {GBIME_EVENT_TYPE_INPUT_METHOD, GBIME_EVENT_INPUT_METHOD_SWITCH_NOTIFY}/*Layout切换完毕再同步平台输入模式刷新VK*/
	, {GBIME_EVENT_TYPE_LAYOUT, GBIME_EVENT_LAYOUT_SHOW} // 必须先显示出来
	, {GBIME_EVENT_TYPE_LAYOUT, GBIME_EVENT_LAYOUT_ENTER} // 进入目标Layout 
	, {GBIME_EVENT_TYPE_LAYOUT, GBIME_EVENT_LAYOUT_REFRESH}	
	}
	, 6}
};

/*!
 * \brief 根据Layout转换类型值检索转换表，获取GBIMEEvent组成的事件集内容
 * \param convertType Layout转换类型值
 * \param pIMEEventGroup 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-22 9:48:11
 */
GBIMEReturn GBU_GetEventGroupByConvertType(GBIMEConvertTypeEnum convertType, PGBIMEEventGroup pIMEEventGroup)
{
	GBU8 i, j;

	if (pIMEEventGroup == NULL)
	{
		return GBIME_INVALID_PARAM;
	}

	for (i = 0; i < PT_ARRAY_SIZE(gbIMEEventConvertMap); i++)
	{
		if (gbIMEEventConvertMap[i].convertType == convertType)
		{
			for (j = 0; j < gbIMEEventConvertMap[i].eventNum; j++)
			{
				pIMEEventGroup->eventGroup[j].nType = gbIMEEventConvertMap[i].eventHeader[j].nType;
				pIMEEventGroup->eventGroup[j].nEvent = gbIMEEventConvertMap[i].eventHeader[j].nEvent;
			}
			pIMEEventGroup->eventNum = gbIMEEventConvertMap[i].eventNum;
			return GBIME_OK;
		}
	}

	return GBIME_EVENT_GROUP_NOT_EXIST;
}