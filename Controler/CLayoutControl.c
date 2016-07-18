/***************************************************************************
* File Name    :  CLayoutControl.c
* Corporation  :  Guobi Technology Ltd. GuangZhou China
* Copyright    :  Copyright Guobi Technology Ltd. And Subject to Confidentiality 
*                 Restrictions. This document is the confidential and proprietary 
*                 property of Guobi Technology Ltd.It is neither the document                  
*                 itself nor any part of it can be shared with any party  without 
*                 the written  permission. 
* Author       :  fanbo
* Date         :  2010-06-08
* Purpose      :  Layout内部逻辑控制实现文件
* Version      :  V1.0
* Last Changed List
* Name         |   date   |  Discription
****************************************************************************/
#include "CLayoutControl.h"
#include "GB_Global.h"

/*!
 * \brief Layout控制模块更新引擎输出信息
 * \param void 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-07-28
 */
GBIMEReturn CLayoutControl_UpdateOutputInfo(void)
{
	PCLayoutControl pLtCtl = Global_GetLayoutCtl();

	PT_Assert(pLtCtl != NULL);

	pLtCtl->engineOutputInfo.pInputString = GBEngine_GetInputString(Global_GetEnineInstance());
	// 上屏相关
	pLtCtl->engineOutputInfo.pUpScreenStr = GBEngine_GetUpScreenString(Global_GetEnineInstance());
	//pLtCtl->engineOutputInfo.nStatusFlag = GBEngine_GetCurStatus();
	pLtCtl->engineOutputInfo.bCandWindOpen = GBEngine_IsActive(Global_GetEnineInstance());
	pLtCtl->engineOutputInfo.bInputEmpty = GBEngine_IsInputEmpty(Global_GetEnineInstance());

	// 候选相关
	pLtCtl->engineOutputInfo.nCandNum = GBEngine_GetCandNum(Global_GetEnineInstance());
	pLtCtl->engineOutputInfo.ppCandidates = GBEngine_GetCandidateBuffer(Global_GetEnineInstance());

	// 音节相关
	pLtCtl->engineOutputInfo.nSyllableNum = GBEngine_GetSyllableNum(Global_GetEnineInstance());
	pLtCtl->engineOutputInfo.nCurrentSyllableIndex = GBEngine_GetCurrentSyllableIndex(Global_GetEnineInstance());
	pLtCtl->engineOutputInfo.ppSyllables = GBEngine_GetSyllableBuffer(Global_GetEnineInstance());

	// 笔画部首相关
	pLtCtl->engineOutputInfo.pStroke = GBEngine_GetStorkeString(Global_GetEnineInstance());
	pLtCtl->engineOutputInfo.pComponents = GBEngine_GetComponentsString(Global_GetEnineInstance());
	pLtCtl->engineOutputInfo.pOutputString = GBEngine_GetOutputString(Global_GetEnineInstance());

	// 候选框中箭头的指示标记
	pLtCtl->engineOutputInfo.bUpScreenArrow = GBEngine_GetUpScreenArrowFlag(Global_GetEnineInstance());
	pLtCtl->engineOutputInfo.bCandArrowUp = GBEngine_GetCandUpArrowFlag(Global_GetEnineInstance());		///<候选显示 上 箭头
	pLtCtl->engineOutputInfo.bCandArrowDown = GBEngine_GetCandDownArrowFlag(Global_GetEnineInstance());		///<候选显示 下 箭头
	pLtCtl->engineOutputInfo.bCandArrowLeft = GBEngine_GetCandLeftArrowFlag(Global_GetEnineInstance());		///<候选显示 左 箭头
	pLtCtl->engineOutputInfo.bCandArrowRight = GBEngine_GetCandRightArrowFlag(Global_GetEnineInstance());	///<候选显示 右 箭头
	pLtCtl->engineOutputInfo.bSyllableArrowLeft = GBEngine_GetSyllableLeftArrowFlag(Global_GetEnineInstance()); ///<Ambiguous Chinese only, 拼音翻页显示左箭头
	pLtCtl->engineOutputInfo.bSyllableArrowRight = GBEngine_GetSyllableRightArrowFlag(Global_GetEnineInstance());///<Ambiguous Chinese only, 拼音翻页显示右箭头

	// 行列信息
	pLtCtl->engineOutputInfo.pRowStart = GBEngine_GetRowStart(Global_GetEnineInstance());
	pLtCtl->engineOutputInfo.nSelCol = GBEngine_GetSelCol(Global_GetEnineInstance());
	pLtCtl->engineOutputInfo.nSelRow = GBEngine_GetSelRow(Global_GetEnineInstance());
	pLtCtl->engineOutputInfo.nRowCount = GBEngine_GetRowCount(Global_GetEnineInstance());

	// Multitap标记[改用LayoutID判断]
	//pLtCtl->engineOutputInfo.isMultiTapInputMode = GBEngine_IsMultiTapInputMode(Global_GetEnineInstance());
	// ALP引擎大小写标记
	//pLtCtl->engineOutputInfo.shiftCapStatus = GBEngine_GetShiftCapStatus(Global_GetEnineInstance());

	// 状态相关
	pLtCtl->engineOutputInfo.isSelectedStatus = GBEngine_IsSelectedStatus(Global_GetEnineInstance());
	pLtCtl->engineOutputInfo.isSwitchNextInputMode = GBEngine_IsNeedSwitchInputModeState(Global_GetEnineInstance());
	pLtCtl->engineOutputInfo.isSymbolCandType = GBEngine_IsSymbolCandType(Global_GetEnineInstance());
	//pLtCtl->engineOutputInfo.isMultiTapInputMode = GBEngine_IsMultiTapInputMode(Global_GetEnineInstance());
	return GBIME_OK;
}

/*!
 * \brief 获取Layout控制模块维护的引擎输出信息
 * \param void 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-07-28
 */
PEngineOutputInfo CLayoutControl_GetEngineOutputInfo(void)
{
	PCLayoutControl pLtCtl = Global_GetLayoutCtl();

	PT_Assert(pLtCtl != NULL);

	return &pLtCtl->engineOutputInfo;
}

/*!
 * \brief 获取候选框是否打开
 * \param void 
 * \return 
 * \note 
 * \example
 * \author Zhaokun
 * \date 2010-10-22
 */
GBBOOL CLayoutControl_IsOpenCandWind(void)
{
	PCLayoutControl pLtCtl = Global_GetLayoutCtl();

	PT_Assert(pLtCtl != NULL);

	return pLtCtl->engineOutputInfo.bCandWindOpen;
}

/*!
 * \brief Layout消息处理函数
 * \param pIMEEvent 
 * \return 
 */
GBIMEReturn CLayoutControl_HandleMessage(PGBIMEEvent pIMEEvent)
{
	GBIMEReturn ret = GBIME_OK;

	if (pIMEEvent->nType != GBIME_EVENT_TYPE_LAYOUT)
	{
		return GBIME_IGNORE; // 非手写事件，忽略
	}

	switch (pIMEEvent->nEvent)
	{
	case GBIME_EVENT_LAYOUT_CREATE: // Layout 创建
		{		
			PCLayoutAbstract pltTempObject = NULL;
			pltTempObject = Global_LayoutObjectIsLeaveByID((GBUINT16)pIMEEvent->wParam);
			if (NULL == pltTempObject)
			{
				pltTempObject = CFactory_CreateInstance(Global_GetIMEData(), (GBUINT16)pIMEEvent->wParam);
			}
			if (NULL != pltTempObject)
			{
				Global_SaveLayoutObject(pltTempObject);
				// 设置标点符号功能键
				if (pltTempObject->m_pfcSetInterpunctionKey != NULL)
				{
					pltTempObject->m_pfcSetInterpunctionKey(pltTempObject);
				}
			}
		}
		break;
	case GBIME_EVENT_LAYOUT_DESTROY: // Layout 销毁
		break;
	case GBIME_EVENT_LAYOUT_SWITCH: // Layout 切换
		CSwitchLayout_Switch((GBUINT16)pIMEEvent->wParam);
		break;
	case GBIME_EVENT_LAYOUT_PEN_DOWN: ///<Layout 触摸笔按下
	case GBIME_EVENT_LAYOUT_PEN_UP:	  ///<Layout 触摸笔弹起
	case GBIME_EVENT_LAYOUT_PEN_MOVE:  ///<Layout 触摸笔移动
	case GBIME_EVENT_LAYOUT_ENTER:
	case GBIME_EVENT_LAYOUT_EXIT:
	case GBIME_EVENT_LAYOUT_HIDE: ///<Layout 隐藏
	case GBIME_EVENT_LAYOUT_SHOW: ///<Layout 显示
	case GBIME_EVENT_LAYOUT_ACTIVATE: ///<Layout 激活
	case GBIME_EVENT_LAYOUT_DEACTIVATE: ///<Layout 注销
		{
			PCLayoutAbstract pltObj = Global_GetCurrentLayoutObject();
			
			// Layout消息处理
			if (pltObj->m_pfcHandleEvent != NULL)
			{
				pltObj->m_pfcHandleEvent(pltObj, pIMEEvent, CLayoutControl_GetEngineOutputInfo());
			}
		}
		break;
	case GBIME_EVENT_LAYOUT_REFRESH: ///<Layout 刷新
		{
			PCLayoutAbstract pltObj = Global_GetCurrentLayoutObject();
			
			// LayoutControl模块组织引擎输出内容，然后通知Layout视图进行刷新显示
			CLayoutControl_UpdateOutputInfo();
			
			// Layout视图刷新显示函数调用
			if (pltObj->m_pfcPaint != NULL)
			{
				pltObj->m_pfcPaint(pltObj, CLayoutControl_GetEngineOutputInfo());
			}
		}
		break;
	case GBIME_EVENT_LAYOUT_REDRAW:
		{
			PCLayoutAbstract pltObj = Global_GetCurrentLayoutObject();
			
			// LayoutControl模块组织引擎输出内容，然后通知Layout视图进行刷新显示
			CLayoutControl_UpdateOutputInfo();
			
			// Layout视图刷新显示函数调用
			if (pltObj->m_pfcPaint != NULL)
			{
				pltObj->m_refreshFlag = GBIME_LAYOUT_REFRESH_ALL;
				pltObj->m_pfcPaint(pltObj, CLayoutControl_GetEngineOutputInfo());
			}
		}
		break;
	case GBIME_EVENT_KEY_DOWN:
	case GBIME_EVENT_KEY_UP:
	case GBIME_EVENT_KEY_LONG_PRESS:
		{
			PCLayoutAbstract pltObj = Global_GetCurrentLayoutObject();
		
			// Layout消息处理
			if (pltObj->m_pfcHandleEvent != NULL)
			{
				pltObj->m_pfcHandleEvent(pltObj, pIMEEvent, CLayoutControl_GetEngineOutputInfo());
			}
		}
		break;
	default:
		PT_Assert(0); // 非法事件
	}

	return ret;
}