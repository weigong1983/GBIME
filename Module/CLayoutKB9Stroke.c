/***************************************************************************
* File Name    :  CLayoutKB9Stroke.c
* Corporation  :  Guobi Technology Ltd. GuangZhou China
* Copyright    :  Copyright Guobi Technology Ltd. And Subject to Confidentiality 
*                 Restrictions. This document is the confidential and proprietary 
*                 property of Guobi Technology Ltd.It is neither the document                  
*                 itself nor any part of it can be shared with any party  without 
*                 the written  permission. 
* Author       :  ZhaoKun
* Date         :  2010-10-15
* Purpose      :  物理九键笔画Layout
* Version      :  V1.0
* Last Changed List
* Name         |   date   |  Discription
****************************************************************************/
#if defined(__GBIME_KB__) // 国笔物理键盘

#include "CLayoutKB9Stroke.h"
#include "CLayoutViewKB9Stroke.h"



/*!
 * \brief 重载父类成员函数声明
 */
GBIMELOCAL GBIMEReturn CLayoutKB9Stroke__Release(GBLPVOID pltObj, PLayoutInfo pltUIData);
GBIMELOCAL GBIMEReturn CLayoutKB9Stroke__Init(PCLayoutKB9Stroke pltKB9);
GBIMELOCAL GBIMEReturn CLayoutKB9Stroke__Paint(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo);
GBIMELOCAL GBIMEReturn CLayoutKB9Stroke__HandleEvent(GBLPVOID pltObj, PGBIMEEvent pIMEEvent, PEngineOutputInfo pEngineOutputInfo);

GBIMELOCAL void ICLayoutKB9Stroke__SetRegion(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo); // 设置触摸区域

/*!
 * \brief 函数实现
 */
CLayoutAbstract * CLayoutKB9Stroke__Construct(PCLayoutKB9Stroke pltKB, 
											  GBLPCVOID pltUIData,
											  GBLPCVOID pltIMData,
											  GBLPCVOID pltConfigData)
{
	CLayoutKB__Construct(&pltKB->m_ltParent,
		pltUIData,
		pltIMData,
		pltConfigData);
	CLayoutKB9Stroke__Init(pltKB);

	return (CLayoutAbstract *)pltKB;
}

GBIMELOCAL GBIMEReturn CLayoutKB9Stroke__Release(GBLPVOID pltObj, PLayoutInfo pltUIData)
{
	CLASE_THIS_POITER(CLayoutKB9Stroke, pltObj)

	pltRoot->m_pfcReleaseBase(pltObj, pltUIData);
	return GBIME_OK;
}

GBIMELOCAL GBIMEReturn CLayoutKB9Stroke__Init(PCLayoutKB9Stroke pltKB)
{
	PCLayoutViewKB9Stroke pLtView = NULL;

	// 构造对应的视图对象
	pLtView = (PCLayoutViewKB9Stroke)PT_Malloc(sizeof(CLayoutViewKB9Stroke));
	PT_Assert(pLtView != NULL);
	memset(pLtView, 0, sizeof(CLayoutViewKB9Stroke));
	CLayoutViewKB9Stroke__Construct(pLtView);

	pltKB->m_ltParent.m_ltParent.m_hltView = (PCLayoutViewAbstract)pLtView;
	pltKB->m_ltParent.m_ltParent.m_pfcPaint = CLayoutKB9Stroke__Paint;
	pltKB->m_ltParent.m_ltParent.m_pfcHandleEvent = CLayoutKB9Stroke__HandleEvent;
	pltKB->m_ltParent.m_ltParent.m_pfcRelease = CLayoutKB9Stroke__Release;
	pltKB->m_ltParent.m_ltParent.currentSyllableIndex = 0;
	pltKB->m_ltParent.m_ltParent.currentCandidateIndex = 0;

	return GBIME_OK;
}

GBIMELOCAL GBIMEReturn CLayoutKB9Stroke__Paint(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo)
{
	CLASE_THIS_POITER(CLayoutKB9Stroke, pltObj)

		if (pltRoot->m_hltView != NULL 
			&& pltRoot->m_hltView->m_pfcPaint != NULL
			&& pEngineOutputInfo->bCandWindOpen
			&& Global_GetLayoutWinActiveStatus()) // 激活状态才显示
		{
			pltRoot->m_hltView->m_pfcPaint(pltObj, pEngineOutputInfo);
		}

		return GBIME_OK;
}

GBIMELOCAL GBIMEReturn CLayoutKB9Stroke__HandleEvent(GBLPVOID pltObj, PGBIMEEvent pIMEEvent, PEngineOutputInfo pEngineOutputInfo)
{
	CLASE_THIS_POITER(CLayoutKB9Stroke, pltObj)
		GBIMEReturn ret;

	PT_Assert(pltObj != NULL);
	PT_Assert(pIMEEvent != NULL);
	PT_Assert(pEngineOutputInfo != NULL);

	pltRoot->m_refreshFlag = GBIME_LAYOUT_REFRESH_NONE;

	// Layout窗口非激活状态必须先激活显示才能接收事件
	if (!Global_GetLayoutWinActiveStatus()
		&& !(pIMEEvent->nEvent == GBIME_EVENT_LAYOUT_ACTIVATE || pIMEEvent->nEvent == GBIME_EVENT_LAYOUT_SHOW))
	{
		return GBIME_IGNORE;
	}

	// 公共消息处理
	if ((ret = pltRoot->m_pfcHandleCommonEvent(pltObj, pIMEEvent, pEngineOutputInfo)) != GBIME_IGNORE)
	{
		return ret;
	}

	// 消息分发(完成Layout按钮状态更新、发送引擎按键事件更新引擎输出信息)
	switch (pIMEEvent->nEvent)
	{
	case GBIME_EVENT_KEY_DOWN:
		ICLayoutKB9Stroke__SetRegion(pltObj, pEngineOutputInfo);
		pltBase->m_pfcUpdateLayoutInformation(pltObj, ButtonStatusDown, pEngineOutputInfo);		
		pltRoot->m_refreshFlag |= (GBIME_LAYOUT_REFRESH_CANDIDATE_LINE | GBIME_LAYOUT_REFRESH_KEYBOARD);
		break;
	case GBIME_EVENT_KEY_UP:
		break;
	case GBIME_EVENT_KEY_LONG_PRESS:
		break;
	default:
		break;
	}

	return GBIME_OK;
}

/*!
 * \brief 设置区域
 * \param pltObj 
 * \param pEngineOutputInfo 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-10 11:30:08
*/
static void ICLayoutKB9Stroke__SetRegion(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo)
{
	CLASE_THIS_POITER(CLayoutKB9Stroke, pltObj)
	GBUINT16 iCurrentLayoutId;
	GBUINT16 iKeyBoardHeight = 0;
	GBUINT16 iKeyBoardWidth = 0;
	GBIMEPoint winPos;
	GBINT16 iScreenWidth = 0;
	
	// 当前LayoutId
	iCurrentLayoutId = GET_LAYOUT_ID(pltRoot);
	// 获取键盘高度
	iKeyBoardHeight = pltRoot->m_pfcGetLayoutHeight(pltObj,iCurrentLayoutId);
	iKeyBoardWidth = pltRoot->m_pfcGetLayoutWidth(pltObj, iCurrentLayoutId);
	// 取得键盘显示的开始位置【左下角】，并调整为【左上角】
	Global_GetWinPos(&winPos);	
	winPos.y -=  iKeyBoardHeight;
	
	iScreenWidth = PT_GetLcdWidth();
	// 设置拼笔划显示的开始位置
	//pltRoot->syllableDisplayStartPos.x = (iScreenWidth - iKeyBoardWidth) / 2;//GBIME_SYLLABLE_DISPLAY_START_X;
	//pltRoot->syllableDisplayStartPos.y = winPos.y;
	pltRoot->m_pfcSetSyllableDisplayStartPos(pltObj, (iScreenWidth - iKeyBoardWidth) / 2, winPos.y);
	
	// 设置候选显示的开始位置
	//pltRoot->candidateDisplayStartPos.x = GBIME_CANDIDATE_DISPLAY_START_X;
	//pltRoot->candidateDisplayStartPos.y = winPos.y + iKeyBoardHeight / 2;// GBIME_CANDIDATE_LINE_HEIGHT - 2;
	pltRoot->m_pfcSetCandidateDisplayStartPos(pltObj, GBIME_CANDIDATE_DISPLAY_START_X, winPos.y + iKeyBoardHeight / 2);
	
	
	if (pEngineOutputInfo->isSymbolCandType/*GBEngine_IsSymbolCandType(Global_GetEnineInstance())*/
		&& (pEngineOutputInfo->pOutputString && pEngineOutputInfo->pOutputString[0] == 0))
	{
		pltBase->m_pfcSetSymbolCandRegion(pltObj, pEngineOutputInfo); // 音节栏触摸区
	}
	else
	{
		pltBase->m_pfcSetOutputStringRegion(pltObj, pEngineOutputInfo); // 音节栏触摸区
		pltBase->m_pfcSetCandidateRegion(pltObj, pEngineOutputInfo); // 候选栏触摸区
	}
}

#endif // 国笔物理键盘