/***************************************************************************
* File Name    :  CLayoutVKSymbol.c
* Corporation  :  Guobi Technology Ltd. GuangZhou China
* Copyright    :  Copyright Guobi Technology Ltd. And Subject to Confidentiality 
*                 Restrictions. This document is the confidential and proprietary 
*                 property of Guobi Technology Ltd.It is neither the document                  
*                 itself nor any part of it can be shared with any party  without 
*                 the written  permission. 
* Author       :  haj
* Date         :  2010-06-08
* Purpose      :  虚拟符号Layout
* Version      :  V1.0
* Last Changed List
* Name         |   date   |  Discription
****************************************************************************/
#if defined(__GBIME_VK__) // 国笔虚拟键盘

#include "CLayoutVKSymbol.h"
#include "CLayoutViewVKSymbol.h"

//判断是否是进行符号切换
#define IS_CHANGE_SYMBOL(val) ((val == GBKEY_VK_SYM_comment)\
							||(val == GBKEY_VK_SYM_EMOTION)\
							||(val == GBKEY_VK_SYM_SPECIAL)\
							||(val == GBKEY_VK_SYM_MATH))
//判断是否是选择符号切
#define IS_CHOOSE_SYMBOL(val) ((val >= GBKEY_VK_1_NUMBERKEY) && (val <= GBKEY_VK_9_NUMBERKEY))
//判断是否是向上翻页
#define IS_SYMBOL_PAGE_UP(val) (val == GBKEY_VK_SYM_PAGEUP)
//判断是否是是向下翻页
#define IS_SYMBOL_PAGE_DOWN(val) (val == GBKEY_VK_SYM_PAGEDOWN)

extern const GBUINT16 symbolCommonChnDataArray[];
extern const GBUINT16 symbolCommonEnDataArray[];
extern const GBUINT16 symbolFaceDataArray[];
extern const GBUINT16 symbolEspecialDataArray[];
extern const GBUINT16 symbolMathDataArray[];

/*!
 * \brief 重载父类成员函数声明
 */
GBIMELOCAL GBIMEReturn CLayoutVKSymbol__Init(PCLayoutVKSymbol pltVKSymbol);
GBIMELOCAL GBIMEReturn CLayoutVKSymbol__Paint(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo);
GBIMELOCAL GBIMEReturn CLayoutVKSymbol__PreHandleEvent(GBLPVOID pltObj, GBINT nPlatformEvent, GBINT wParam, GBINT lParam, PEngineOutputInfo pEngineOutputInfo);
GBIMELOCAL GBIMEReturn CLayoutVKSymbol__HandleEvent(GBLPVOID pltObj, PGBIMEEvent pIMEEvent, PEngineOutputInfo pEngineOutputInfo);

/*!
 * \brief 本地函数声明
 */
GBIMELOCAL PenPosition ICLayoutVKSymbol_HitTest(GBLPVOID pltObj, 
									 GBIMEPoint hitPoint,
									 ButtonStatusEnum buttonStatus,
									 PEngineOutputInfo pEngineOutputInfo);
GBIMELOCAL void ICLayoutVKSymbol_UpdateLayoutInformation(GBLPVOID pltObj, 
											  ButtonStatusEnum buttonStatus, 
											  PEngineOutputInfo pEngineOutputInfo);
GBIMELOCAL void ICLayoutVKSymbol__SetSymbol(GBLPVOID pltObj, GBUINT16 layoutKeyValue);
GBIMELOCAL void ICLayoutVKSymbol__InitSymbol(GBLPVOID pltObj, GbSymbolTypeEnum symbolType, GBLPCVOID pSymbolData);
GBIMELOCAL void ICLayoutVKSymbol__MoveToNextPageSymbol(GBLPVOID pltObj);
GBIMELOCAL void ICLayoutVKSymbol__MoveToPrvePageSymbol(GBLPVOID pltObj);
GBIMELOCAL GBUINT16* ICLayoutVKSymbol__GetSymbolByLayoutKeyValue(GBLPVOID pltObj, GBUINT16 layoutKeyValue);
GBIMELOCAL GBIMEReturn ICLayoutVKSymbol__FuncKeyHandler(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo);

CLayoutAbstract * CLayoutVKSymbol__Construct(PCLayoutVKSymbol pltVKSymbol,
											 GBLPCVOID pltUIData,
											 GBLPCVOID pltIMData,
											 GBLPCVOID pltConfigData)
{
	CLayoutVK__Construct(&pltVKSymbol->m_ltParent,
		pltUIData,
		pltIMData,
		pltConfigData);
	CLayoutVKSymbol__Init(pltVKSymbol);

	return (CLayoutAbstract *)pltVKSymbol;
}

GBIMELOCAL GBIMEReturn CLayoutVKSymbol__Init(PCLayoutVKSymbol pltVKSymbol)
{
	PCLayoutViewVKSymbol pLtView = NULL;

	// 构造对应的视图对象
	pLtView = (PCLayoutViewVKSymbol)PT_Malloc(sizeof(CLayoutViewVKSymbol));
	PT_Assert(pLtView != NULL);
	memset(pLtView, 0, sizeof(CLayoutViewVKSymbol));
	CLayoutViewVKSymbol__Construct(pLtView);

	pltVKSymbol->m_ltParent.m_ltParent.m_hltView = (PCLayoutViewAbstract)pLtView;
	pltVKSymbol->m_ltParent.m_ltParent.m_pfcPaint = CLayoutVKSymbol__Paint;
	pltVKSymbol->m_ltParent.m_ltParent.m_pfcPreHandleEvent = CLayoutVKSymbol__PreHandleEvent;
	pltVKSymbol->m_ltParent.m_ltParent.m_pfcHandleEvent = CLayoutVKSymbol__HandleEvent;
	pltVKSymbol->m_ltParent.m_ltParent.m_pfcFuncKeyHandler = ICLayoutVKSymbol__FuncKeyHandler;
	pltVKSymbol->m_bSymbolLock = GBFalse;

	// 根据语言环境初始化全角/半角符号
	if (GBEngine_IsChineseInputMode(Global_GetEnineInstance()))
	{
		pltVKSymbol->m_commonSymbolType = FullShaped;
	}
	else
	{
		pltVKSymbol->m_commonSymbolType = HalfShaped;
	}
	ICLayoutVKSymbol__SetSymbol(pltVKSymbol,GBKEY_VK_SYM_comment);
	return GBIME_OK;
}

GBIMELOCAL GBIMEReturn CLayoutVKSymbol__Paint(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo)
{
	CLASE_THIS_POITER(CLayoutVKSymbol, pltObj)

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
 1. Layout解析处理, 设置是否需要发送相应的按键消息给国笔引擎处理的标记并记录按键值(实际处理消息时用到)
 2. 转换成框架对应的一组消息
 3. 特定功能的参数转换参考：

	// 候选相关操作
	获得下一页的候选: wParam设置为【GBKEY_NEXT_PAGE_CANDIATE】
	获得上一页的候选: wParam设置为【GBKEY_PREV_PAGE_CANDIATE】
	选择光标往后移一个候选: wParam设置为【GBKEY_NEXT_CANDIDATE】
	选择光标往前移一个候选: wParam设置为【GBKEY_PREV_CANDIDATE】
	选择一个候选: wParam设置为【GBEVENT_SELECT_CANDIDATE】; lParam 参数的低16位表示选择的列，高16位表示选择的行

	// 音节相关操作
	获得下一页的拼音: wParam设置为【GBKEY_NEXT_PAGE_SYLLABLE】
	获得上一页的拼音: wParam设置为【GBKEY_PREV_PAGE_SYLLABLE】
	把当前选择的拼音往后移动一个: wParam设置为【GBKEY_NEXT_SYLLABLE】
	把当前选择的拼音往前移动一个: wParam设置为【GBKEY_PREV_SYLLABLE】
	选择当前的页的音节: wParam设置为【GBEVENT_SELECT_SYLLABLE_PAGED】; lParam 参数有效值为0到当前页音节的个数
 */
GBIMELOCAL GBIMEReturn CLayoutVKSymbol__PreHandleEvent(GBLPVOID pltObj, GBINT nPlatformEvent, GBINT wParam, GBINT lParam, PEngineOutputInfo pEngineOutputInfo)
{
	CLASE_THIS_POITER(CLayoutVKSymbol, pltObj)

	PLayoutInfo pLayoutInfo = (PLayoutInfo)(&pltRoot->m_ltData); // Layout UI data point
	GBIMEPoint hitPoint;
	GBINT32 hitButtonIndex = -1;
	PenPosition penPositionInWhere;
	//PCLayoutAbstract pltPreObject = Global_GetPrevLayoutObject();
	GBIMEReturn ret;

	// 将平台触摸屏事件转换成IME框架需要处理的事件集	
	hitPoint.x = wParam;
	hitPoint.y = lParam;
	//hitButtonIndex = GBLayoutButtonHitTest(pltKB, &hitPoint);

	pltRoot->bSendEngineKeyEvent = GBFalse;
	pltRoot->engineKeyEvent = wParam; // 这个参数需要Layout解析成具体的引擎按键值
	pltRoot->engineEventParam = lParam; // 这个参数需要Layout根据具体功能按钮进行解析
	
	switch (nPlatformEvent)
	{
	case GBPEVT_PEN_DOWN:
		pltRoot->m_pfcSlidingPenDownHandler(pltObj, hitPoint);
		penPositionInWhere = ICLayoutVKSymbol_HitTest(pltObj, hitPoint, ButtonStatusDown, pEngineOutputInfo);
		
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

		penPositionInWhere = ICLayoutVKSymbol_HitTest(pltObj, hitPoint, ButtonStatusUp, pEngineOutputInfo);

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
		penPositionInWhere = ICLayoutVKSymbol_HitTest(pltObj, hitPoint, ButtonStatusMove, pEngineOutputInfo);
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

GBIMELOCAL GBIMEReturn CLayoutVKSymbol__HandleEvent(GBLPVOID pltObj, PGBIMEEvent pIMEEvent, PEngineOutputInfo pEngineOutputInfo)
{
	CLASE_THIS_POITER(CLayoutVKSymbol, pltObj)
	
	// 消息分发(完成Layout按钮状态更新、发送引擎按键事件更新引擎输出信息)
	switch (pIMEEvent->nEvent)
	{
	case GBIME_EVENT_LAYOUT_PEN_DOWN:
		break;
	case GBIME_EVENT_LAYOUT_PEN_UP:
		ICLayoutVKSymbol_UpdateLayoutInformation(pltObj, ButtonStatusUp, pEngineOutputInfo);
		// 清除历史按键值
		pltRoot->pressLayoutKeyValue = pltRoot->currentLayoutKeyValue = GBEvent_None;
		pltRoot->penDownPostion = pltRoot->penInPostion = PenPositionInNothing;
		break;
	case GBIME_EVENT_LAYOUT_PEN_MOVE:
		break;
	case GBIME_EVENT_KEY_DOWN: // 物理按键直接刷新	
	case GBIME_EVENT_KEY_UP:
	case GBIME_EVENT_KEY_LONG_PRESS:
		pltRoot->m_refreshFlag = (GBIME_LAYOUT_REFRESH_CANDIDATE_LINE | GBIME_LAYOUT_REFRESH_SYLLABLE_LINE);
		break;
	case GBIME_EVENT_LAYOUT_ACTIVATE: ///<Layout 激活
		pltRoot->m_ltData.ltCurLayer = FirstLayer;
		Global_SetLayoutWinActiveStatus(GBTrue);
		Global_SetLayoutWinHeight(pltRoot->m_pfcGetLayoutHeight(pltObj, pltRoot->m_ltData.pltcData->ltID));
		pltRoot->m_refreshFlag = GBIME_LAYOUT_REFRESH_ALL;
		break;
	case GBIME_EVENT_LAYOUT_DEACTIVATE: ///<Layout 注销
		pltRoot->m_ltData.ltCurLayer = FirstLayer;
		Global_SetLayoutWinActiveStatus(GBFalse);
		Global_SetLayoutWinHeight(0);
		// 重新初始化Layout对象状态变量
		pltRoot->m_pfcInitVariable(pltObj);
		break;
	case GBIME_EVENT_LAYOUT_HIDE: ///<Layout 隐藏
		pltRoot->m_ltData.ltCurLayer = FirstLayer;
		Global_SetLayoutWinActiveStatus(GBFalse);
		pltRoot->m_pfcChangeHeight(pltObj, 0);
		// 重新初始化Layout对象状态变量
		pltRoot->m_pfcInitVariable(pltObj);
		break;
	case GBIME_EVENT_LAYOUT_SHOW: ///<Layout 显示
		pltRoot->m_ltData.ltCurLayer = FirstLayer;
		Global_SetLayoutWinActiveStatus(GBTrue);
		pltRoot->m_pfcChangeHeight(pltObj, pltRoot->m_pfcGetLayoutHeight(pltObj, pltRoot->m_ltData.pltcData->ltID));
		pltRoot->m_refreshFlag = GBIME_LAYOUT_REFRESH_ALL;
		break;
	case GBIME_EVENT_LAYOUT_ENTER:
		// 根据语言环境重新设置符号
		if (GBEngine_IsChineseInputMode(Global_GetEnineInstance()))
		{
			pltThis->m_commonSymbolType = FullShaped;
		}
		else
		{
			pltThis->m_commonSymbolType = HalfShaped;
		}
		ICLayoutVKSymbol__SetSymbol(pltObj, GBKEY_VK_SYM_comment);
		//Global_SetLayoutWinHeight(0); // 高度清零，解决Layout间切换由于高度一样没有刷新VK
		pltRoot->m_pfcChangeHeight(pltObj, pltRoot->m_pfcGetLayoutHeight(pltObj, pltRoot->m_ltData.pltcData->ltID));
		//pltRoot->m_pfcInitVariable(pltObj);
		break;
	case GBIME_EVENT_LAYOUT_EXIT:		
		Global_SetLayoutWinHeight(0); // 高度清零，解决Layout间切换由于高度一样没有刷新VK
		pltRoot->m_pfcInitVariable(pltObj);
		break;
	default:
		break;
	}
	
	return GBIME_OK;
}

/*!
 * \brief 根据符号的序号，取得符号在数组中的开始下标
 * \param pltObj 
 * \param symbolIndex 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-8-19 9:27:47
*/
GBUINT16 CLayoutVKSymbol__GetSymbolOffsetByIndex(GBLPVOID pltObj, GBUINT16 symbolIndex)
{
	CLASE_THIS_POITER(CLayoutVKSymbol, pltObj)

	GBUINT16 iOffset = 0;
	GBUINT16 iCurrentSymbolIndex = 0;
	GBUINT16 iCount = 0;
	GBUINT16 *pSymbolData;

	if (pltThis->m_pltSymbolData != NULL)
	{
		pSymbolData = (GBUINT16*)pltThis->m_pltSymbolData;
		iCount = 0;
		iCurrentSymbolIndex = 0;

		while (pSymbolData[iCount] != SYMBOL_END_FLAG)
		{
			if (iCurrentSymbolIndex == symbolIndex)
			{
				iOffset = iCount;
				return iOffset;
			}
			if (pSymbolData[iCount] == 0)
			{
				iCurrentSymbolIndex++;
			}
			iCount++;
		}
	}

	return iOffset;
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
 * \date 2010-8-19 9:28:40
*/
static PenPosition ICLayoutVKSymbol_HitTest(GBLPVOID pltObj, 
										  GBIMEPoint hitPoint,
										  ButtonStatusEnum buttonStatus,
										  PEngineOutputInfo pEngineOutputInfo)
{
	CLASE_THIS_POITER(CLayoutVKSymbol, pltObj)
	PenPosition penPositionInWhere = PenPositionInNothing;

	// 判断触摸笔是否落在键盘按钮上
	penPositionInWhere = pltRoot->m_pfcPenPositionInKeyboardButton(pltObj, hitPoint, buttonStatus, pEngineOutputInfo);
	return penPositionInWhere;
}

/*!
 * \brief 更新图层信息
 * \param pltObj 
 * \param buttonStatus 
 * \param pEngineOutputInfo 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-8-19 9:28:31
*/
static void ICLayoutVKSymbol_UpdateLayoutInformation(GBLPVOID pltObj, 
												   ButtonStatusEnum buttonStatus, 
												   PEngineOutputInfo pEngineOutputInfo)
{
	CLASE_THIS_POITER(CLayoutVKSymbol, pltObj)

	GBUINT16 iCandidateBoxHeight = 0;
	GBUINT16 iKeyBoardHeight = 0;
	GBUINT16 iCurrentLayoutId;
	GBIMEPoint winPos;

	// 当前LayoutId
	iCurrentLayoutId = GET_LAYOUT_ID(pltRoot);
	// 获取键盘高度
	iKeyBoardHeight = pltRoot->m_pfcGetLayoutHeight(pltObj, iCurrentLayoutId);
	// 取得键盘显示的开始位置【左下角】，并调整为【左上角】
	Global_GetWinPos(&winPos);	
	winPos.y -=  iKeyBoardHeight;

	//pltRoot->m_pfcChangeHeight(pltObj, iKeyBoardHeight);

	if (pltRoot->penInPostion == PenPositionInKeyboardButton)
	{
		if (buttonStatus == ButtonStatusUp
			&& pltRoot->pressLayoutKeyValue == pltRoot->currentLayoutKeyValue) //按键松开时
		{
			if (IS_CHANGE_SYMBOL(pltRoot->currentLayoutKeyValue)) // 是切换符号时
			{
				ICLayoutVKSymbol__SetSymbol(pltObj, pltRoot->currentLayoutKeyValue);
			}
			else if (IS_CHOOSE_SYMBOL(pltRoot->currentLayoutKeyValue)) // 是选择符号时
			{
				GBUINT16 *pSymbol;
				pSymbol = ICLayoutVKSymbol__GetSymbolByLayoutKeyValue(pltObj, pltRoot->currentLayoutKeyValue);
				if (pSymbol)
				{
					pltRoot->m_pfcUpScreen(pltObj, pSymbol);
					if (!pltThis->m_bSymbolLock && pltRoot->m_pfcSwitchPreLayoutObj != NULL)
					{
						pltRoot->m_pfcSwitchPreLayoutObj(pltObj);
					}
				}
			}
			else if (IS_SYMBOL_PAGE_UP(pltRoot->currentLayoutKeyValue))//上一页
			{
				ICLayoutVKSymbol__MoveToPrvePageSymbol(pltObj);
			}
			else if (IS_SYMBOL_PAGE_DOWN(pltRoot->currentLayoutKeyValue))//下一页
			{
				ICLayoutVKSymbol__MoveToNextPageSymbol(pltObj);
			}
		}
		return;
	}
}

/*!
 * \brief 根据layout键值，设置符号类型
 * \param pltObj 
 * \param layoutKeyValue 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-8-19 9:28:21
*/
static void ICLayoutVKSymbol__SetSymbol(GBLPVOID pltObj, GBUINT16 layoutKeyValue)
{
	CLASE_THIS_POITER(CLayoutVKSymbol, pltObj)

	switch(layoutKeyValue)
	{		
	case GBKEY_VK_SYM_comment: // 普通符号
		if (pltThis->m_commonSymbolType == FullShaped)
		{
			ICLayoutVKSymbol__InitSymbol(pltObj, SymbolTypeCommon, (GBLPCVOID)symbolCommonChnDataArray);
		}
		else if (pltThis->m_commonSymbolType == HalfShaped)
		{
			ICLayoutVKSymbol__InitSymbol(pltObj, SymbolTypeCommon, (GBLPCVOID)symbolCommonEnDataArray);
		}
		break;		
	case GBKEY_VK_SYM_EMOTION: // 表情符号
		ICLayoutVKSymbol__InitSymbol(pltObj, SymbolTypeFace, (GBLPCVOID)symbolFaceDataArray);
		break;		
	case GBKEY_VK_SYM_SPECIAL: // 特殊符号
		ICLayoutVKSymbol__InitSymbol(pltObj, SymbolTypeEspecial, (GBLPCVOID)symbolEspecialDataArray);
		break;		
	case GBKEY_VK_SYM_MATH: // 数学符号
		ICLayoutVKSymbol__InitSymbol(pltObj, SymbolTypeNumber, (GBLPCVOID)symbolMathDataArray);
		break;
	default:
		ICLayoutVKSymbol__InitSymbol(pltObj, SymbolTypeCommon, (GBLPCVOID)symbolCommonChnDataArray);
		break;
	}
}

/*!
 * \brief 初始化符号基本参数
 * \param pltObj 
 * \param symbolType 
 * \param pSymbolData 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-8-19 9:28:13
*/
static void ICLayoutVKSymbol__InitSymbol(GBLPVOID pltObj, GbSymbolTypeEnum symbolType, GBLPCVOID pSymbolData)
{
	CLASE_THIS_POITER(CLayoutVKSymbol, pltObj)

	GBUINT16 *pSymbol;
	GBUINT16 iCount;
	GBUINT16 iSingCount;

	PT_Assert(pSymbolData != NULL);

	pSymbol = (GBUINT16 *)pSymbolData;
	pltThis->m_pltSymbolData = pSymbolData;
	pltThis->m_symbolType = symbolType;
	pltThis->m_iCurrentPageStartIndex = 0;
	pltThis->m_iCurrentChooseSymbolIndex = 0;
	pltThis->m_bHasNextPage = 0;
	pltThis->m_bHasPrvePage = 0;
	pltThis->m_hasFullOrHalfSymbol = 0;
	//普通符号时有半角和全角
	if (symbolType == SymbolTypeCommon)
	{
		pltThis->m_hasFullOrHalfSymbol = 1;
	}
	iCount = 0;
	iSingCount = 0;

	// 计算符号个数
	while (pSymbol[iCount] != SYMBOL_END_FLAG)
	{
		if (pSymbol[iCount] == 0) // 发现一个符号结束时，符号个数增加1
		{
			iSingCount++;
		}
		iCount++;
	}

	if (iSingCount >= 1)
	{
		// 得到总的符号个数
		pltThis->m_iSymbolTotalCount = iSingCount;
		
		if (iSingCount > SYMBOL_COUNT_PER_PAGE) // 符号个数大于一页的个数时
		{
			pltThis->m_iCurrentPageEndIndex = SYMBOL_COUNT_PER_PAGE - 1;
			pltThis->m_bHasNextPage = 1;
		}
		else
		{
			pltThis->m_iCurrentPageEndIndex = iSingCount - 1;
		}
	}
}

/*!
 * \brief 移到下一页符号
 * \param pltObj 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-8-19 9:28:05
*/
static void ICLayoutVKSymbol__MoveToNextPageSymbol(GBLPVOID pltObj)
{
	CLASE_THIS_POITER(CLayoutVKSymbol, pltObj)

	// 如果有下一页
	if (pltThis->m_bHasNextPage)
	{
		// 下一页的第一个符号即是当前页最后一个符号的后面
		pltThis->m_iCurrentPageStartIndex = pltThis->m_iCurrentPageEndIndex + 1;
		pltThis->m_bHasPrvePage = 1;
	}

	// 如果没有下一页
	if ((pltThis->m_iCurrentPageStartIndex + SYMBOL_COUNT_PER_PAGE) >= pltThis->m_iSymbolTotalCount)
	{
		pltThis->m_bHasNextPage = 0;
		pltThis->m_iCurrentPageEndIndex = pltThis->m_iSymbolTotalCount - 1;
	}
	else
	{
		pltThis->m_bHasNextPage = 1;
		pltThis->m_iCurrentPageEndIndex = pltThis->m_iCurrentPageStartIndex + SYMBOL_COUNT_PER_PAGE - 1;
	}
}

/*!
 * \brief 移到上一页符号
 * \param pltObj 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-8-19 9:27:55
*/
static void ICLayoutVKSymbol__MoveToPrvePageSymbol(GBLPVOID pltObj)
{
	CLASE_THIS_POITER(CLayoutVKSymbol, pltObj)

	// 如果有上一页,进行翻页
	if (pltThis->m_bHasPrvePage)
	{
		// 上一页的最后一个符号是当前页的第一个的前面
		pltThis->m_iCurrentPageEndIndex = pltThis->m_iCurrentPageStartIndex - 1;
		pltThis->m_iCurrentPageStartIndex = pltThis->m_iCurrentPageEndIndex - SYMBOL_COUNT_PER_PAGE + 1;
		
	}

	// 如果还有上一页
	if (pltThis->m_iCurrentPageStartIndex > 0)
	{
		pltThis->m_bHasPrvePage = 1;
	}
	else
	{
		pltThis->m_bHasPrvePage = 0;
	}

	// 如果有下一页
	if (pltThis->m_iCurrentPageEndIndex < pltThis->m_iSymbolTotalCount - 1)
	{
		pltThis->m_bHasNextPage = 1;
	}
	else
	{
		pltThis->m_bHasNextPage = 0;
	}
}

/*!
 * \brief 根据键盘的键值，取得对应的符号
 * \param pltObj 
 * \param layoutKeyValue 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-8-19 9:27:34
*/
static GBUINT16* ICLayoutVKSymbol__GetSymbolByLayoutKeyValue(GBLPVOID pltObj, GBUINT16 layoutKeyValue)
{
	CLASE_THIS_POITER(CLayoutVKSymbol, pltObj)

	GBUINT16 *pSymbolData;
	GBUINT16 iSymbolOffset;
	GBUINT16 iCurrentChooseIndex = 0;

	// 每页9个符号，在1,2,3....9这几个数字键上,判断该键值是否是选择符号键
	if (IS_CHOOSE_SYMBOL(layoutKeyValue))
	{
		pSymbolData = (GBUINT16 *)pltThis->m_pltSymbolData;
		// 当前页的第一个符号在符号数组中的序号
		iCurrentChooseIndex = pltThis->m_iCurrentPageStartIndex;
		// 选中的符号在符号数组中的序号
		iCurrentChooseIndex += (layoutKeyValue - GBKEY_VK_1_NUMBERKEY);
		// 取得该符号在数组中的偏移位置
		iSymbolOffset = CLayoutVKSymbol__GetSymbolOffsetByIndex(pltObj, iCurrentChooseIndex);
		//iSymbolOffset=0时有两种情况：第一个符号或不存在的符号
		if ((iSymbolOffset == 0)&&(pltThis->m_iCurrentPageStartIndex != 0))//不存在的符号
		{
			return NULL;
		}
		return pSymbolData + iSymbolOffset;
	}

	return NULL;
}
//普通全角中文符号
const GBUINT16 symbolCommonChnDataArray[] = 
{
	0xff0c,0x0000,//，
	0x3002,0x0000,//。
	0xff1f,0x0000,//？
	0xff01,0x0000,//！
	0x3001,0x0000,//、
	0xff1a,0x0000,//：
	0xff1b,0x0000,//；
	0x2026,0x2026,0x0000,//……
	0x0040,0x0000,//＠
	0x201c,0x201d,0x0000,//“”
	0xfe61,0x0000,//﹡
	0xff03,0x0000,//﹟
	0x2018,0x2019,0x0000,//‘ ’
	0xff3f,0x0000,//＿
	0x2014,0x0000,//—
	0xff06,0x0000,//＆
	0xff0f,0x0000,//∕
	0xff3c,0x0000,//﹨
	0x0024,0x0000,//$
	0xffe5,0x0000,//￥
	0xff08,0xff09,0x0000,//（）
	0x300a,0x300b,0x0000,//《》
	0xff3b,0xff3d,0x0000,//［］
	0xff5b,0xff5d,0x0000,//｛｝
	0x3010,0x3011,0x0000,//【】
	0x3016,0x3017,0x0000,//〖〗
	0xff62,0xff63,0x0000,//「」
	0xff5c,0x0000,//｜
	0xff5e,0x0000,//～
	0xff0b,0x0000,//＋
	0xff0d,0x0000,//－
	0x00d7,0x0000,//×
	0x00f7,0x0000,//÷
	0xff1d,0x0000,//＝
	0x0025,0x0000,//%
	0x2103,0x0000,//℃

	//增加符号时，请在这一项之前增加[符号和符号之间用0x0000间隔]
	SYMBOL_END_FLAG	
};
//普通半角英文符号
const GBUINT16 symbolCommonEnDataArray[] = 
{
	0x002c,0x0000,//,
	0x002e,0x0000,//.
	0x003f,0x0000,//?
	0x0021,0x0000,//!
	0x0027,0x0000,//'
	0x003a,0x0000,//:
	0x003b,0x0000,//;
	0x2026,0x0000,//…
	0x0040,0x0000,//@
	0x0022,0x0022,0x0000,//""
	0x002a,0x0000,//*
	0x0023,0x0000,//#
	0x0060,0x0000,//`
	0x005f,0x0000,//_
	0x2014,0x0000,//—
	0x0026,0x0000,//&
	0x002f,0x0000,///
	0x005c,0x0000,//'\'
	0x0024,0x0000,//$
	0x00a5,0x0000,//￥
	0x0028,0x0029,0x0000,//()
	0x003c,0x003e,0x0000,//<>
	0x005b,0x005d,0x0000,//[]
	0x007b,0x007d,0x0000,//{}
	0x3010,0x3011,0x0000,//【】
	0x3016,0x3017,0x0000,//〖〗
	0xff62,0xff63,0x0000,//??
	0x007c,0x0000,//|
	0x007e,0x0000,//~
	0x002b,0x0000,//+
	0x002d,0x0000,//-
	0x00d7,0x0000,//×
	0x00f7,0x0000,//÷
	0x003d,0x0000,//=
	0x0025,0x0000,//%
	0x2103,0x0000,//℃

	//增加符号时，请在这一项之前增加[符号和符号之间用0x0000间隔]
	SYMBOL_END_FLAG	
};

//表情符号
const GBUINT16 symbolFaceDataArray[] = 
{
	0x003a,0x002d,0x0029,0x0000,//:-)
	0x003b,0x002d,0x0029,0x0000,//;-)
	0x003a,0x002d,0x0028,0x0000,//:-(
	0x003a,0x002d,0x0044,0x0000,//:-D
	0x003a,0x002d,0x0070,0x0000,//:-p
	0x002d,0x002e,0x002d,0x007c,0x007c,0x0000,//-.-||
	0x003a,0x002d,0x0078,0x0000,//:-x
	0x003a,0x002d,0x004f,0x0000,//:-O
	0x003a,0x002d,0x002a,0x0000,//:-*
	0x0038,0x0038,0x0036,0x007e,0x0000,//886~
	0x0035,0x0035,0x0035,0x007e,0x0000,//555~
	0x004f,0x0072,0x007a,0x0000,//Orz
	0x0048,0x0061,0x0068,0x0061,0x007e,0x0000,//Haha~
	0x0048,0x0065,0x0068,0x0065,0x007e,0x0000,//Hehe~
	0x0054,0x005f,0x0054,0x0000,//T_T
	0x0024,0x005f,0x0024,0x0000,//$_$
	0x2192,0x005f,0x2192,0x0000,//→_→
	0x003f,0x005f,0x003f,0x0000,//?_?
	0xfe36,0xfe3f,0xfe36,0x0000,//︶︿︶
	0x005e,0x25ce,0x002d,0x0000,//^◎-
	0x002d,0xfe35,0x002d,0x0000,//-︵-
	0x003e,0x0033,0x003c,0x0000,//>3<
	0x003e,0x005f,0x003c,0x0000,//>_<
	0x0040,0x005f,0x0040,0x0000,//@_@
	0xffe3,0xfe41,0xffe3,0x0000,//￣﹁￣
	0x2299,0x002e,0x2299,0x0000,//⊙.⊙
	0x0028,0x2299,0x006f,0x2299,0x0029,0x0000,//(⊙o⊙)
	0x003c,0x203b,0x0000,//<※
	0x005e,0x006f,0x005e,0x0079,0x0000,//^o^y
	0x005e,0x005f,0x007e,0x0000,//^_~
	0x002a,0x005e,0x006f,0x005e,0x002a,0x0000,//*^o^*
	0x005e,0x002e,0x0020,0x002e,0x005e,0x0000,//^. .^
	0x005e,0x005f,0x005e,0x0000,//^_^
	0x0023,0x005e,0x005f,0x005e,0x0000,//#^_^
	0x005e,0x0028,0x006f,0x006f,0x0029,0x005e,0x0000,//^(oo)^
	0x006f,0x005f,0x0030,0x0000,//o_0

	//增加符号时，请在这一项之前增加[符号和符号之间用0x0000间隔]
	SYMBOL_END_FLAG		
};
//网址符号
const GBUINT16 symbolEspecialDataArray[] = 
{
	0x0068,0x0074,0x0074,0x0070,0x003a,0x002f,0x002f,0x0000,//http://
	0x0077,0x0077,0x0077,0x002e,0x0000,//www.
	0x0077,0x0061,0x0070,0x002e,0x0000,//wap.
	0x002e,0x0063,0x006f,0x006d,0x0000,//.com
	0x002e,0x0063,0x006e,0x0000,//.cn
	0x002e,0x0063,0x006f,0x006d,0x002e,0x0063,0x006e,0x0000,//.com.cn
	0x002e,0x006e,0x0065,0x0074,0x0000,//.net
	0x002e,0x006f,0x0072,0x0067,0x0000,//.org
	0x002e,0x006e,0x0065,0x0074,0x002e,0x0063,0x006e,0x0000,//.net.cn
	0x002e,0x0065,0x0064,0x0075,0x0000,//.edu
	0x002e,0x0069,0x006e,0x0066,0x006f,0x0000,//.info
	0x002e,0x006f,0x0072,0x0067,0x002e,0x0063,0x006e,0x0000,//.org.cn
	0x0062,0x0062,0x0073,0x002e,0x0000,//bbs.
	0x002e,0x0063,0x0063,0x0000,//.cc
	0x0033,0x0067,0x002e,0x0000,//3g.
	0x0077,0x0061,0x0070,0x0073,0x002e,0x0000,//waps.
	0x002e,0x006d,0x006f,0x0062,0x0069,0x0000,//.mobi
	0x002e,0x006e,0x0061,0x006d,0x0065,0x0000,//.name


	//增加符号时，请在这一项之前增加[符号和符号之间用0x0000间隔]
	SYMBOL_END_FLAG
};
//特殊符号
const GBUINT16 symbolMathDataArray[] = 
{
	0x2116,0x0000,//№
	0x00b0,0x0000,//°
	0x00a7,0x0000,//§
	0x00a2,0x0000,//￠
	0x00a8,0x0000,//¨
	0x203b,0x0000,//※
	0x00a4,0x0000,//¤
	0x00a9,0x0000,//?
	0x00ae,0x0000,//?
	0x2640,0x0000,//♀
	0x2642,0x0000,//♂
	0x2312,0x0000,//⌒
	0xffe9,0x0000,//?
	0xffeb,0x0000,//?
	0x221a,0x0000,//√
	0xffec,0x0000,//?
	0xffea,0x0000,//?
	0x00b6,0x0000,//?
	0x2121,0x0000,//℡
	0x2122,0x0000,//?
	0x32a3,0x0000,//㊣
	0x25c6,0x0000,//◆
	0x25c7,0x0000,//◇
	0x25b3,0x0000,//△
	0x2605,0x0000,//★
	0x2606,0x0000,//☆
	0x25b2,0x0000,//▲
	0x263f,0x0000,//?
	0x2641,0x0000,//?
	0x306e,0x0000,//の
	0x25cf,0x0000,//●
	0x25cb,0x0000,//○
	0x2661,0x0000,//?
	0x25a0,0x0000,//■
	0x25a1,0x0000,//□
	0x2665,0x0000,//?

	//增加符号时，请在这一项之前增加[符号和符号之间用0x0000间隔]
	SYMBOL_END_FLAG	
};

/*!
 * \brief 功能键处理函数
 * \param pltObj 
 * \param pEngineOutputInfo 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-8-25 11:22:00
*/
static GBIMEReturn ICLayoutVKSymbol__FuncKeyHandler(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo)
{
	CLASE_THIS_POITER(CLayoutVKSymbol, pltObj)
	GBUINT16 tempStr[2];

	switch (pltRoot->currentLayoutKeyValue)
	{
	case GBKEY_VK_SBC_DBC_SWICH: ///<全半角切换键	
		if (pltThis->m_hasFullOrHalfSymbol)
		{
			pltThis->m_commonSymbolType = (pltThis->m_commonSymbolType == FullShaped) ? HalfShaped : FullShaped;
			if (pltThis->m_symbolType == SymbolTypeCommon)
			{
				ICLayoutVKSymbol__SetSymbol(pltObj, (GBUINT16)GBKEY_VK_SYM_comment);
			}
		}
		break;
	case GBKEY_VK_SYM_LOCK: ///<符号锁定键
		pltThis->m_bSymbolLock = (pltThis->m_bSymbolLock) ? GBFalse : GBTrue;
		break;
	case GBKEY_SPACE: // 无输入时，点击【空格】键，输入空格。输入时，点击【空格】键，候选首字上屏
		tempStr[0] = GBKEY_SPACE;
		tempStr[1] = 0;
		pltRoot->m_pfcUpScreen(pltObj, tempStr);
		break;
	case GBKEY_BACK:
		GBInputBox_DeleteChar();
		break;
	default:
		return GBIME_IGNORE;
		break;
	}

	Global_FillEventGroup(GBIME_CT_KEY_UP_REFRESH, 0, 0); // 按钮弹起刷新
	return GBIME_OK;
}

#endif // 国笔虚拟键盘