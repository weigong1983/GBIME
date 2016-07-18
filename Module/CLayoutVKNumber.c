/***************************************************************************
* File Name    :  CLayoutVKNumber.c
* Corporation  :  Guobi Technology Ltd. GuangZhou China
* Copyright    :  Copyright Guobi Technology Ltd. And Subject to Confidentiality 
*                 Restrictions. This document is the confidential and proprietary 
*                 property of Guobi Technology Ltd.It is neither the document                  
*                 itself nor any part of it can be shared with any party  without 
*                 the written  permission. 
* Author       :  haj
* Date         :  2010-06-08
* Purpose      :  虚拟数字Layout
* Version      :  V1.0
* Last Changed List
* Name         |   date   |  Discription
****************************************************************************/
#if defined(__GBIME_VK__) // 国笔虚拟键盘

#include "CLayoutVKNumber.h"
#include "CLayoutViewVKNumber.h"

/*!
 * \brief 重载父类成员函数声明
 */
GBIMELOCAL GBIMEReturn CLayoutVKNumber__Release(GBLPVOID pltObj, PLayoutInfo pltUIData);
GBIMELOCAL GBIMEReturn CLayoutVKNumber__Init(PCLayoutVKNumber pltVK);
GBIMELOCAL GBIMEReturn CLayoutVKNumber__Paint(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo);
GBIMELOCAL GBIMEReturn CLayoutVKNumber__PreHandleEvent(GBLPVOID pltObj, GBINT nPlatformEvent, GBINT wParam, GBINT lParam, PEngineOutputInfo pEngineOutputInfo);
GBIMELOCAL GBIMEReturn CLayoutVKNumber__HandleEvent(GBLPVOID pltObj, PGBIMEEvent pIMEEvent, PEngineOutputInfo pEngineOutputInfo);
GBIMELOCAL GBIMEReturn CLayoutVKNumber__DisableFuncKeyHandler(GBLPVOID pltObj);
GBIMELOCAL GBBOOL CLayoutVKNumber__IsInputKey(GBUINT16 keyValue);

/*!
 * \brief 本地函数声明
 */
GBIMELOCAL void ICLayoutVKNumber__ProcessLayoutPenDown(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo);
GBIMELOCAL void ICLayoutVKNumber__ProcessLayoutPenUp(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo);
GBIMELOCAL PenPosition ICLayoutVKNumber__HitTest(GBLPVOID pltObj, 
									  GBIMEPoint hitPoint, 
									  ButtonStatusEnum buttonStatus, 
									  PEngineOutputInfo pEngineOutputInfo);
GBIMELOCAL void ICLayoutVKNumber__UpdateLayoutInformation(GBLPVOID pltObj, 
											   ButtonStatusEnum buttonStatus,
											   PEngineOutputInfo pEngineOutputInfo);

CLayoutAbstract * CLayoutVKNumber__Construct(PCLayoutVKNumber pltVK, 
											 GBLPCVOID pltUIData,
											 GBLPCVOID pltIMData,
											 GBLPCVOID pltConfigData)
{
	CLayoutVK__Construct(&pltVK->m_ltParent,
		pltUIData,
		pltIMData,
		pltConfigData);
	CLayoutVKNumber__Init(pltVK);

	return (CLayoutAbstract *)pltVK;
}

GBIMELOCAL GBIMEReturn CLayoutVKNumber__Release(GBLPVOID pltObj, PLayoutInfo pltUIData)
{
	CLASE_THIS_POITER(CLayoutVKNumber, pltObj)

	pltRoot->m_pfcReleaseBase(pltObj, pltUIData);
	return GBIME_OK;
}

GBIMELOCAL GBIMEReturn CLayoutVKNumber__Init(PCLayoutVKNumber pltVK)
{
	PCLayoutViewVKNumber pLtView = NULL;

	// 构造对应的视图对象
	pLtView = (PCLayoutViewVKNumber)PT_Malloc(sizeof(CLayoutViewVKNumber));
	PT_Assert(pLtView != NULL);
	memset(pLtView, 0, sizeof(CLayoutViewVKNumber));
	CLayoutViewVKNumber__Construct(pLtView);

	pltVK->m_ltParent.m_ltParent.m_hltView = (PCLayoutViewAbstract)pLtView;
	pltVK->m_ltParent.m_ltParent.m_pfcPaint = CLayoutVKNumber__Paint;
	pltVK->m_ltParent.m_ltParent.m_pfcPreHandleEvent = CLayoutVKNumber__PreHandleEvent;
	pltVK->m_ltParent.m_ltParent.m_pfcHandleEvent = CLayoutVKNumber__HandleEvent;
	pltVK->m_ltParent.m_ltParent.m_pfcRelease = CLayoutVKNumber__Release;
	pltVK->m_ltParent.m_ltParent.m_pfcIsInputKey = CLayoutVKNumber__IsInputKey;
	pltVK->m_ltParent.m_ltParent.m_pfcDisableFuncKeyHandler = CLayoutVKNumber__DisableFuncKeyHandler;
	
	return GBIME_OK;
}

GBIMELOCAL GBIMEReturn CLayoutVKNumber__Paint(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo)
{
	CLASE_THIS_POITER(CLayoutVKNumber, pltObj)

	if (pltRoot->m_hltView != NULL 
		&& pltRoot->m_hltView->m_pfcPaint != NULL
		&& Global_GetLayoutWinActiveStatus()) // 激活状态才显示
	{
		pltRoot->m_hltView->m_pfcPaint(pltObj, pEngineOutputInfo);
	}

	return GBIME_OK;
}

/*!
 *-brief 该函数完成对平台原始消息的预处理：
*/
GBIMELOCAL GBIMEReturn CLayoutVKNumber__PreHandleEvent(GBLPVOID pltObj, GBINT nPlatformEvent, GBINT wParam, GBINT lParam, PEngineOutputInfo pEngineOutputInfo)
{
	CLASE_THIS_POITER(CLayoutVKNumber, pltObj)

	PLayoutInfo pLayoutInfo = (PLayoutInfo)(&pltRoot->m_ltData); // Layout UI data point
	GBIMEPoint hitPoint;
	GBINT32 hitButtonIndex = -1;
	PenPosition penPositionInWhere;
	GBUINT16 targetLayoutID = LayoutIdSP9JPinYin;
	GBIMEReturn ret;

	// 将平台触摸屏事件转换成IME框架需要处理的事件集	
	hitPoint.x = wParam;
	hitPoint.y = lParam;
	//hitButtonIndex = GBLayoutButtonHitTest(pltKB, &hitPoint);
	pltRoot->bSendEngineKeyEvent = GBFalse;
	pltRoot->engineKeyEvent = wParam; // 这个参数需要Layout解析成具体的引擎按键值
	pltRoot->engineEventParam = lParam; // 这个参数需要Layout根据具体功能按钮进行解析
	
	switch(nPlatformEvent)
	{
	case GBPEVT_PEN_DOWN:		
		pltRoot->m_pfcSlidingPenDownHandler(pltObj, hitPoint);
		penPositionInWhere = ICLayoutVKNumber__HitTest(pltObj, hitPoint, ButtonStatusDown, pEngineOutputInfo);
		if (pltRoot->m_pfcShouldSendMsgToEngine(pltObj, ButtonStatusDown))
		{
			pltRoot->bSendEngineKeyEvent = GBTrue;
			pltRoot->engineKeyEvent = CLayoutInfo_ConversionLayoutKeyToEngineKey(pltRoot->currentLayoutKeyValue);
		}

		// 长按处理
		pltRoot->m_pfcLongPressKeyHandler(pltObj, pEngineOutputInfo);

		// 根据预处理结果判断是否发送引擎按键事件
		if (pltRoot->bSendEngineKeyEvent)
		{
			Global_FillEventGroup(GBIME_CT_NORMAL_KEY_DOWN, wParam, lParam);
		} 
		else
		{
			Global_FillEventGroup(GBIME_CT_KEY_DOWN_REFRESH, wParam, lParam);
		}
		break;
	case GBPEVT_PEN_UP:		
		// 如果是滑屏切换Layout则直接结束处理
		if ((ret = pltRoot->m_pfcSlidingPenUpHandler(pltObj, hitPoint)) != GBIME_IGNORE)
		{
			return ret;
		}
		
		penPositionInWhere = ICLayoutVKNumber__HitTest(pltObj, hitPoint, ButtonStatusUp, pEngineOutputInfo);

		// 长按键处理
		if ((ret = pltRoot->m_pfcLongPressKeyHandler(pltObj, pEngineOutputInfo)) != GBIME_IGNORE)
		{
			break; // 结束处理
		}

		// 实现点击显示、隐藏Layout面板
		if (0)//penPositionInWhere == PenPositionInNothing && Global_GetLayoutWinActiveStatus())
		{
			Global_FillEventGroup(GBIME_CT_HIDE, wParam, lParam);
			break; // 直接退出switch
		}
		else if (/*penPositionInWhere != PenPositionInNothing && */!Global_GetLayoutWinActiveStatus()) // 当前Layout处于隐藏状态，需要先激活
		{
			Global_FillEventGroup(GBIME_CT_SHOW, wParam, lParam);
			break; // 直接退出switch
		}
		
		if (penPositionInWhere != PenPositionInNothing)
		{
			// 点击键盘区域时按下和弹起键值一致才作响应
			if (pltRoot->pressLayoutKeyValue == pltRoot->currentLayoutKeyValue)
			{
				if ((ret = pltRoot->m_pfcSwitchHandler(pltObj)) != GBIME_IGNORE) // Layout切换处理
				{				
					break;
				}
				else if ((ret = pltRoot->m_pfcFuncKeyHandler(pltObj, pEngineOutputInfo)) != GBIME_IGNORE) // 功能键处理
				{
					break;
				}
			}			
			Global_FillEventGroup(GBIME_CT_KEY_UP_REFRESH, wParam, lParam);
		}
	    break;
	case GBPEVT_PEN_MOVE:
		pltRoot->m_pfcSlidingPenMoveHandler(pltObj, hitPoint);
		// 处理触摸笔Move消息，更新Layout按钮状态
		penPositionInWhere = ICLayoutVKNumber__HitTest(pltObj, hitPoint, ButtonStatusMove, pEngineOutputInfo);
		Global_FillEventGroup(GBIME_CT_MOVE, wParam, lParam);
		break;
	case GBPEVT_KEY_DOWN:
	case GBPEVT_KEY_UP:
		// 按键处理
		//pltRoot->m_pfcPhysicFuncKeyHandler(pltObj, nPlatformEvent, wParam, lParam, pEngineOutputInfo);
		break;
	default:
	    break;
	}

	return GBIME_OK;
}

GBIMELOCAL GBIMEReturn CLayoutVKNumber__HandleEvent(GBLPVOID pltObj, PGBIMEEvent pIMEEvent, PEngineOutputInfo pEngineOutputInfo)
{
	CLASE_THIS_POITER(CLayoutVKNumber, pltObj)
	PLayoutInfo pLayoutInfo = (PLayoutInfo)(&pltRoot->m_ltData); // Layout UI data point
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

	// 激活数字Layout时根据平台输入框是否处于纯数字输入模式决定是否禁用某些功能按钮
// 	if (pIMEEvent->nEvent == GBIME_EVENT_LAYOUT_ACTIVATE)
// 	{
// 		if (GBInputBox_IsDigitInputOnly())
// 		{
// 			CLayoutInfo_SetButtonStatus(pLayoutInfo, GBKEY_VK_BACKWORD, ButtonStatusGray);
// 			pltRoot->isSlidingDisabled = GBTrue; // 禁止滑屏切换
// 		}
// 		else
// 		{
// 			CLayoutInfo_SetButtonStatus(pLayoutInfo, GBKEY_VK_BACKWORD, ButtonStatusNomal);
// 			pltRoot->isSlidingDisabled = GBFalse; // 允许滑屏切换
// 		}
// 	}

	// 公共消息处理
	if ((ret = pltRoot->m_pfcHandleCommonEvent(pltObj, pIMEEvent, pEngineOutputInfo)) != GBIME_IGNORE)
	{
		return ret;
	}

	// 消息分发(完成Layout按钮状态更新、发送引擎按键事件更新引擎输出信息)
	switch (pIMEEvent->nEvent)
	{
	case GBIME_EVENT_LAYOUT_PEN_DOWN:
		ICLayoutVKNumber__ProcessLayoutPenDown(pltObj, pEngineOutputInfo);
		if (pltRoot->penInPostion != PenPositionInNothing)
		{
			pltRoot->m_refreshFlag = GBIME_LAYOUT_REFRESH_KEYBOARD;
		}
		break;
	case GBIME_EVENT_LAYOUT_PEN_UP:
		ICLayoutVKNumber__ProcessLayoutPenUp(pltObj, pEngineOutputInfo);
		if (pltRoot->penInPostion != PenPositionInNothing)
		{
			pltRoot->m_refreshFlag = GBIME_LAYOUT_REFRESH_KEYBOARD;
		}
		// 清除历史按键值
		pltRoot->pressLayoutKeyValue = pltRoot->currentLayoutKeyValue = GBEvent_None;
		pltRoot->penDownPostion = pltRoot->penInPostion = PenPositionInNothing;
		break;
	case GBIME_EVENT_LAYOUT_PEN_MOVE:
		if (pltRoot->penInPostion != PenPositionInNothing)
		{
			pltRoot->m_refreshFlag = GBIME_LAYOUT_REFRESH_KEYBOARD;
		}
		break;
	case GBIME_EVENT_KEY_DOWN: // 物理按键直接刷新	
	case GBIME_EVENT_KEY_UP:
	case GBIME_EVENT_KEY_LONG_PRESS:
		pltRoot->m_refreshFlag = (GBIME_LAYOUT_REFRESH_CANDIDATE_LINE | GBIME_LAYOUT_REFRESH_SYLLABLE_LINE);
		break;
	default:
		break;
	}
	
	return GBIME_OK;
}

/*!
 * \brief 平台某些输入模式需要禁用某些功能按钮(如: 快速查找模式禁止输入中文)
 * \param pltObj 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-12-30 13:45:08
 */
GBIMELOCAL GBIMEReturn CLayoutVKNumber__DisableFuncKeyHandler(GBLPVOID pltObj)
{
	CLASE_THIS_POITER(CLayoutVKNumber, pltObj)
	PLayoutInfo pLayoutInfo = (PLayoutInfo)(&pltRoot->m_ltData); // Layout UI data point
	GBIMEReturn ret;

	if (GBInputBox_IsDigitInputOnly())
	{
		CLayoutInfo_SetButtonStatus(pLayoutInfo, GBKEY_VK_BACKWORD, ButtonStatusGray);
		pltRoot->isSlidingDisabled = GBTrue; // 禁止滑屏切换
	}
	else
	{
		CLayoutInfo_SetButtonStatus(pLayoutInfo, GBKEY_VK_BACKWORD, ButtonStatusNomal);
		pltRoot->isSlidingDisabled = GBFalse; // 允许滑屏切换
	}

	return GBIME_OK;
}

/*!
 * \brief 处理触摸笔按下事件
 * \param pltObj 
 * \param pEngineOutputInfo 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-8-17 16:47:32
*/
static void ICLayoutVKNumber__ProcessLayoutPenDown(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo)
{
	ICLayoutVKNumber__UpdateLayoutInformation(pltObj, ButtonStatusDown, pEngineOutputInfo);
}

/*!
 * \brief 处理触摸笔松开事件
 * \param pltObj 
 * \param pEngineOutputInfo 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-8-17 16:47:49
*/
static void ICLayoutVKNumber__ProcessLayoutPenUp(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo)
{
	ICLayoutVKNumber__UpdateLayoutInformation(pltObj, ButtonStatusUp, pEngineOutputInfo);
}

/*!
 * \brief 获取当前触摸笔按下时的位置：在候选上，在键盘按钮上，其他地方
 * \param pltObj 
 * \param hitPoint 
 * \param buttonStatus 
 * \param pEngineOutputInfo 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-8-17 16:48:11
*/
static PenPosition ICLayoutVKNumber__HitTest(GBLPVOID pltObj, 
									 GBIMEPoint hitPoint, 
									 ButtonStatusEnum buttonStatus, 
									 PEngineOutputInfo pEngineOutputInfo)
{
	CLASE_THIS_POITER(CLayoutVKNumber, pltObj)

    GBUINT16 iCandidateBoxHeight = 0;
	GBUINT16 iKeyBoardHeight = 0;
	PenPosition penPositionInWhere = PenPositionInNothing;
	GBUINT16 iCurrentLayoutId;	

	//当前要显示的层
	GBUINT16 iCurrentLayerNumber;
	GBUINT16 iButtonTotalCount;
	GBIMEPoint winPos;
	PLayoutChgtLayer pLayoutChgtLayer;

	PT_PrintLogInfo(GBLOG_LAYOUTINFO, ("###################################################[x,y]=[%3d,%3d]\n", hitPoint.x, hitPoint.y));

	// 当前LayoutId
	iCurrentLayoutId = GET_LAYOUT_ID(pltRoot);
	// 获取键盘高度
	iKeyBoardHeight = pltRoot->m_pfcGetLayoutHeight(pltObj,iCurrentLayoutId);
	// 取得键盘显示的开始位置【左下角】，并调整为【左上角】
	Global_GetWinPos(&winPos);	
	winPos.y -=  iKeyBoardHeight;
	// 当前要显示的层
	iCurrentLayerNumber = GET_LAYOUT_CURRENT_LAYER(pltRoot);
	// 取候选框高度
	iCandidateBoxHeight = pltRoot->m_pfcGetCandidateBoxHeight(pltObj,iCurrentLayoutId,iCurrentLayerNumber);
	//先查第地层
	pLayoutChgtLayer = (PLayoutChgtLayer)&(pltRoot->m_ltData.pltLayerData[FirstLayer]);

	// 第一，二或三层时，都会查询键盘
	iButtonTotalCount = pLayoutChgtLayer->lyBtCount;

	// 默认为无效点击位置
	pltRoot->penInPostion = PenPositionInNothing;

	// 判断触摸笔是否落在键盘按钮上
	penPositionInWhere = pltRoot->m_pfcPenPositionInKeyboardButton(pltObj, hitPoint, buttonStatus, pEngineOutputInfo);

	return penPositionInWhere;
}

/*!
 * \brief 更新图层信息(控制层的切换、通知平台输入框调整高度)
 * \param pltObj 
 * \param buttonStatus 
 * \param pEngineOutputInfo 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-8-17 16:49:58
*/
static void ICLayoutVKNumber__UpdateLayoutInformation(GBLPVOID pltObj, ButtonStatusEnum buttonStatus,	PEngineOutputInfo pEngineOutputInfo)
{
	CLASE_THIS_POITER(CLayoutVKNumber, pltObj)

	GBUINT16 iCandidateBoxHeight = 0;
	GBUINT16 iKeyBoardHeight = 0;
	GBUINT16 iCurrentLayoutId;

	// 当前要显示的层
	GBUINT16 iCurrentLayerNumber;
	GBIMEPoint winPos;

	// 当前LayoutId
	iCurrentLayoutId = GET_LAYOUT_ID(pltRoot);
	// 获取键盘高度
	iKeyBoardHeight = pltRoot->m_pfcGetLayoutHeight(pltObj,iCurrentLayoutId);
	// 取得键盘显示的开始位置【左下角】，并调整为【左上角】
	Global_GetWinPos(&winPos);	
	winPos.y -=  iKeyBoardHeight;
	// 当前要显示的层
	iCurrentLayerNumber = GET_LAYOUT_CURRENT_LAYER(pltRoot);
	// 取候选框高度
	iCandidateBoxHeight = pltRoot->m_pfcGetCandidateBoxHeight(pltObj,iCurrentLayoutId,iCurrentLayerNumber);

	// 点击键盘区域如果按下和弹起键值不一致不作响应
	if (pltRoot->penInPostion == PenPositionInKeyboardButton)
	{
		pltRoot->m_ltData.ltCurLayer = FirstLayer;

		if (buttonStatus == ButtonStatusUp
			&& pltRoot->pressLayoutKeyValue == pltRoot->currentLayoutKeyValue)
		{
			if (pltRoot->m_pfcIsInputKey(pltRoot->currentLayoutKeyValue))
			{
				GBUINT16 strNumber[2];
				strNumber[0] = CLayoutInfo_ConversionLayoutKeyToEngineKey(pltRoot->currentLayoutKeyValue);
				strNumber[1] = 0;
				pltRoot->m_pfcUpScreen(pltObj, strNumber);
			}
		}
	}
}

/*!
 * \brief 判断是否输入键
 * \param keyValue 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-17 17:33:07
*/
GBBOOL CLayoutVKNumber__IsInputKey(GBUINT16 keyValue)
{
	if ((keyValue >= GBKEY_0) && (keyValue <= GBKEY_9))
	{
		return GBTrue;
	}
	else if(keyValue >= GBKEY_VK_V9_SYMBOL_ADD && keyValue <= GBKEY_VK_V9_SYMBOL_FULL_STOP)
	{
		return GBTrue;
	}

	return GBFalse;
}

#endif // 国笔虚拟键盘