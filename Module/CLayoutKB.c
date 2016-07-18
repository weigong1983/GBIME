/*****************************************************************************
*  Copyright Statement:
*  --------------------
*
* Filename:
* ---------
*    CLayoutKB9PinYin.c
*
* Project:
* --------
*   Guobi Input Method
*
* Description:
* ------------
*
* Author:
* -------
* -------
***********************************************************************/
#if defined(__GBIME_KB__) // 国笔物理键盘
#include "CLayoutKB.h"
#include "CLayoutViewKB.h"
#include "PlatformAdapter.h"

GBIMELOCAL GBIMEReturn CLayoutKB__Init(PCLayoutKB pltKB);
GBIMELOCAL GBIMEReturn CLayoutKB__Paint(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo);
GBIMELOCAL GBIMEReturn CLayoutKB__PreHandleEvent(GBLPVOID pltObj, GBINT nPlatformEvent, GBINT wParam, GBINT lParam, PEngineOutputInfo pEngineOutputInfo);
GBIMELOCAL GBIMEReturn CLayoutKB__HandleCommonEvent(GBLPVOID pltObj, PGBIMEEvent pIMEEvent, PEngineOutputInfo pEngineOutputInfo);
GBIMELOCAL GBIMEReturn CLayoutKB__HandleEvent(GBLPVOID pltObj, PGBIMEEvent pIMEEvent, PEngineOutputInfo pEngineOutputInfo);

GBIMELOCAL void CLayoutKB__SetSyllableRegion(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo); // 音节触摸区
GBIMELOCAL void CLayoutKB__SetOutputStringRegion(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo); // 笔画触摸区
GBIMELOCAL void CLayoutKB__SetCandidateRegion(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo); // 候选触摸区
GBIMELOCAL void CLayoutKB__SetSymbolCandRegion(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo); // 候选触摸区
GBIMELOCAL void CLayoutKB__SetSyllablePageButtonRegion(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo); // 音节翻页按钮触摸区
GBIMELOCAL void CLayoutKB__SetCandidatePageButtonRegion(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo);// 候选翻页按钮触摸区
GBIMELOCAL void CLayoutKB__UpdateLayoutInformation(GBLPVOID pltObj, ButtonStatusEnum buttonStatus,	PEngineOutputInfo pEngineOutputInfo);
CLayoutAbstract * CLayoutKB__Construct(PCLayoutKB pltKB, 
											  GBLPCVOID pltUIData,
											  GBLPCVOID pltIMData,
											  GBLPCVOID pltConfigData)
{
	CLayoutAbstract__Construct(&pltKB->m_ltParent,
								pltUIData,
								pltIMData,
								pltConfigData);
	CLayoutKB__Init(pltKB);

	return (CLayoutAbstract *)pltKB;
}

GBIMELOCAL GBIMEReturn CLayoutKB__Init(PCLayoutKB pltKB)
{
	PCLayoutViewKB pLtView = NULL;
	
	// 构造对应的视图对象
	pLtView = (PCLayoutViewKB)PT_Malloc(sizeof(CLayoutViewKB));
	PT_Assert(pLtView != NULL);
	memset(pLtView, 0, sizeof(CLayoutViewKB));
	CLayoutViewAbstract__Construct(&pLtView->m_ltParent);
	CLayoutViewKB__Init(pLtView);
	pltKB->m_ltParent.m_hltView = (PCLayoutViewAbstract)pLtView;

	pltKB->m_ltParent.m_pfcPaint = CLayoutKB__Paint;
	pltKB->m_ltParent.m_pfcPreHandleEvent = CLayoutKB__PreHandleEvent;
	pltKB->m_ltParent.m_pfcHandleCommonEvent = CLayoutKB__HandleCommonEvent;
	pltKB->m_ltParent.m_pfcHandleEvent = CLayoutKB__HandleEvent;

	pltKB->m_pfcSetSyllableRegion = CLayoutKB__SetSyllableRegion; // 设置音节触摸区
	pltKB->m_pfcSetOutputStringRegion = CLayoutKB__SetOutputStringRegion;
	pltKB->m_pfcSetCandidateRegion = CLayoutKB__SetCandidateRegion; // 设置候选触摸区
	pltKB->m_pfcSetSymbolCandRegion = CLayoutKB__SetSymbolCandRegion;//设置标点显示区域
	pltKB->m_pfcSetSyllablePageButtonRegion = CLayoutKB__SetSyllablePageButtonRegion; // 设置音节翻页按钮触摸区
	pltKB->m_pfcSetCandidatePageButtonRegion = CLayoutKB__SetCandidatePageButtonRegion;// 设置候选翻页按钮触摸区
	pltKB->m_pfcUpdateLayoutInformation = CLayoutKB__UpdateLayoutInformation;

	return GBIME_OK;
}

GBIMELOCAL GBIMEReturn CLayoutKB__Paint(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo)
{
	PCLayoutKB pltKB = (PCLayoutKB)pltObj;

	if (pltKB->m_ltParent.m_hltView != NULL 
		&& pltKB->m_ltParent.m_hltView->m_pfcPaint != NULL
		&& Global_GetLayoutWinActiveStatus()) // 激活状态才显示
	{
		pltKB->m_ltParent.m_hltView->m_pfcPaint(pltKB, pEngineOutputInfo);
	}

	return GBIME_OK;
}

/*!
 *-brief 该函数完成对平台原始消息的预处理：
 1. Layout解析处理, 设置是否需要发送相应的按键消息给国笔引擎处理的标记并记录按键值(实际处理消息时用到)
 2. 转换成框架对应的一组消息
 */
GBIMELOCAL GBIMEReturn CLayoutKB__PreHandleEvent(GBLPVOID pltObj, GBINT nPlatformEvent, GBINT wParam, GBINT lParam, 
									  PEngineOutputInfo pEngineOutputInfo)
{
	CLASE_THIS_POITER(CLayoutKB, pltObj)

//	GBUINT16	iCurrentLayoutID;
	GBUINT16	iKeyBoardHeight = 0;
	GBUINT16	iKeyBoardWidth = 0;
//	GBIMEPoint	winPos;
//	GBIMEPoint	hitPoint;
//	GBRECT		rect;
//	GBINT		iScreenHeight;
	PLayoutInfo	pLayoutInfo = (PLayoutInfo)(&pltBase->m_ltData); // Layout UI data point
	GBINT		nNextInputMode = 0;
	GBUINT16	layoutID = 0;

	// 将平台按键事件转换成IME框架需要处理的事件
	switch (nPlatformEvent)
	{
	case GBPEVT_KEY_DOWN:
		Global_FillEventGroup(GBIME_CT_KEY_DOWN, wParam, lParam);
		pltBase->bSendEngineKeyEvent = GBTrue;
		pltBase->engineKeyEvent = wParam;
		// 激活Layout
		Global_SetLayoutWinActiveStatus(GBTrue);
		// 设置引擎按功能按键处理函数
		//GBInputBox_SetAllIMEFuncKey();
		break;
	case GBPEVT_KEY_UP:
		Global_FillEventGroup(GBIME_CT_KEY_UP, wParam, lParam);
		pltBase->bSendEngineKeyEvent = GBTrue;
		pltBase->engineKeyEvent = wParam;
		break;
	case GBPEVT_KEY_LONG_PRESS:
		Global_FillEventGroup(GBIME_CT_KEY_LONG_PRESS, wParam, lParam);
		pltBase->bSendEngineKeyEvent = GBTrue;
		pltBase->engineKeyEvent = wParam;
		break;
#if 0 // 这段处理提前到框架消息预处理中完成
	case GBPEVT_PEN_DOWN:
		iCurrentLayoutID = GET_LAYOUT_ID(pltBase);
		// 获取键盘高度
		iKeyBoardHeight = pltBase->m_pfcGetLayoutHeight(pltObj,iCurrentLayoutID);
		iKeyBoardWidth = pltBase->m_pfcGetLayoutWidth(pltObj, iCurrentLayoutID);
		Global_GetWinPos(&winPos);
		winPos.y -=  iKeyBoardHeight;
		hitPoint.x = wParam;
		hitPoint.y = lParam;
		rect.left = winPos.x;
		rect.right = winPos.x + iKeyBoardWidth;
		rect.top = winPos.y;
		rect.bottom = winPos.y + iKeyBoardHeight;
		
		if(!PT_PointIsInRect(hitPoint, rect)
			&& hitPoint.y < rect.bottom)
		{
			//切换到物理键盘
			//?该处需要封装下，不同虚拟layout切换到不同物理layout。如vk stroke切换到kb stroke
			if(LayoutIdSP9JPinYin == GET_LAYOUT_ID(pltBase))
			{
				Global_FillEventGroup(GBIME_CT_KEY_DOWN, wParam, lParam);
			}
			else
			{
				Global_FillEventGroup(GBIME_CT_VK_2_KB_SWITCH, LayoutIdSP9JPinYin, lParam);
			}	
		}

		break;
#endif
	case GBPEVT_PEN_DOWN: // 将Pen消息消费掉，否则平台会重置mmi_imc_key_set_softkey
	case GBPEVT_PEN_MOVE:
	case GBPEVT_PEN_UP:
		return GBIME_CONSUMED;
	case GBPEVT_KEY_REPEAT:	
	default:
		return GBIME_LAYOUT_CONVERT_MSG_FAIL;
	}

	return GBIME_OK;
}

/*!
 * \brief Layout公共消息处理函数(完成一些公共的通用消息处理)
 * \param pltObj 
 * \param pIMEEvent 
 * \param pEngineOutputInfo 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-17 10:47:49
*/
GBIMELOCAL GBIMEReturn CLayoutKB__HandleCommonEvent(GBLPVOID pltObj, PGBIMEEvent pIMEEvent, PEngineOutputInfo pEngineOutputInfo)
{
	PCLayoutAbstract pltAbstractObj = (PCLayoutAbstract)pltObj;
	GBIMEReturn ret = GBIME_OK;
	GBUINT16 nCurrentLayoutID = GET_LAYOUT_ID(pltAbstractObj);

	switch (pIMEEvent->nEvent)
	{
	case GBIME_EVENT_LAYOUT_ACTIVATE: ///<Layout 激活
		pltAbstractObj->m_ltData.ltCurLayer = FirstLayer;
		Global_SetLayoutWinActiveStatus(GBTrue);
		// 物理键盘首次激活不显示
		Global_SetLayoutWinHeight(0);
		pltAbstractObj->m_refreshFlag = GBIME_LAYOUT_REFRESH_ALL;
		break;
	case GBIME_EVENT_LAYOUT_DEACTIVATE: ///<Layout 注销
		pltAbstractObj->m_ltData.ltCurLayer = FirstLayer;
		Global_SetLayoutWinActiveStatus(GBFalse);
		Global_SetLayoutWinHeight(0);
		// 重新初始化Layout对象状态变量
		pltAbstractObj->m_pfcInitVariable(pltObj);
		break;
	case GBIME_EVENT_LAYOUT_HIDE: ///<Layout 隐藏
		pltAbstractObj->m_ltData.ltCurLayer = FirstLayer;
		Global_SetLayoutWinActiveStatus(GBFalse);
		pltAbstractObj->m_pfcChangeHeight(pltObj, 0);
		// 重新初始化Layout对象状态变量
		pltAbstractObj->m_pfcInitVariable(pltObj);
		break;
	case GBIME_EVENT_LAYOUT_SHOW: ///<Layout 显示
		pltAbstractObj->m_ltData.ltCurLayer = FirstLayer;
		Global_SetLayoutWinActiveStatus(GBTrue);
		// 物理键盘不显示
		pltAbstractObj->m_pfcChangeHeight(pltObj, 0);		
		pltAbstractObj->m_refreshFlag = GBIME_LAYOUT_REFRESH_ALL;
		break;
	case GBIME_EVENT_LAYOUT_ENTER:
		pltAbstractObj->m_ltData.ltCurLayer = FirstLayer;
		//Global_SetLayoutWinHeight(0); // 高度清零，解决Layout间切换由于高度一样没有刷新VK
		//pltAbstractObj->m_pfcChangeHeight(pltObj, pltAbstractObj->m_pfcGetLayoutHeight(pltObj, nCurrentLayoutID));
		pltAbstractObj->m_refreshFlag = GBIME_LAYOUT_REFRESH_ALL;
		//pltAbstractObj->m_pfcInitVariable(pltObj);
		break;
	case GBIME_EVENT_LAYOUT_EXIT:
		Global_SetLayoutWinHeight(0); // 高度清零，解决Layout间切换由于高度一样没有刷新VK
		pltAbstractObj->m_pfcInitVariable(pltObj);
		break;
	default:
		ret = GBIME_IGNORE;
		break;
	}
	
	return ret;
}

GBIMELOCAL GBIMEReturn CLayoutKB__HandleEvent(GBLPVOID pltObj, PGBIMEEvent pIMEEvent, PEngineOutputInfo pEngineOutputInfo)
{
	CLASE_THIS_POITER(CLayoutKB, pltObj)
		GBIMEReturn ret;

	PT_Assert(pltObj != NULL);
	PT_Assert(pIMEEvent != NULL);
	PT_Assert(pEngineOutputInfo != NULL);

	pltBase->m_refreshFlag = GBIME_LAYOUT_REFRESH_NONE;

	// Layout窗口非激活状态必须先激活显示才能接收事件
	if (!Global_GetLayoutWinActiveStatus()
		&& !(pIMEEvent->nEvent == GBIME_EVENT_LAYOUT_ACTIVATE || pIMEEvent->nEvent == GBIME_EVENT_LAYOUT_SHOW))
	{
		return GBIME_IGNORE;
	}

	// 公共消息处理
	if ((ret = pltBase->m_pfcHandleCommonEvent(pltObj, pIMEEvent, pEngineOutputInfo)) != GBIME_IGNORE)
	{
		return ret;
	}

	// 消息分发(完成Layout按钮状态更新、发送引擎按键事件更新引擎输出信息)
	switch (pIMEEvent->nEvent)
	{
	case GBIME_EVENT_KEY_DOWN://GBIME_EVENT_LAYOUT_PEN_DOWN:		
		pltBase->m_refreshFlag |= (GBIME_LAYOUT_REFRESH_CANDIDATE_LINE | GBIME_LAYOUT_REFRESH_SYLLABLE_LINE);
		break;
	case GBIME_EVENT_KEY_UP://GBIME_EVENT_LAYOUT_PEN_UP:
		pltBase->m_refreshFlag |= (GBIME_LAYOUT_REFRESH_CANDIDATE_LINE | GBIME_LAYOUT_REFRESH_SYLLABLE_LINE);
		break;
	case GBIME_EVENT_KEY_LONG_PRESS://GBIME_EVENT_LAYOUT_PEN_MOVE:
		pltBase->m_refreshFlag = (GBIME_LAYOUT_REFRESH_CANDIDATE_LINE | GBIME_LAYOUT_REFRESH_SYLLABLE_LINE);
		break;
	default:
		break;
	}

	return GBIME_OK;
}

/*!
 * \brief 设置拼音音节触摸区
 * \param pltObj 
 * \param pEngineOutputInfo 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-8-24 17:38:42
*/
static void CLayoutKB__SetSyllableRegion(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo)
{
	CLASE_THIS_POITER(CLayoutKB, pltObj)
	GBPCWCHAR *ppCandidates;
	GBUINT16 iCandidateCount = 0;
	PT_TextAlign textAlign = PT_TEXT_ALIGN_CENTER;	
	GBIMEPoint candStartPoint;
	GBUINT16 iCurLayoutId;
	GBUINT16 iKeyBoardWidth, iKeyBoardHeight;	
	
	pltBase->m_pfcGetLayoutStartPoint(pltObj, &candStartPoint);	
	// 当前LayoutId
	iCurLayoutId = GET_LAYOUT_ID(pltBase);
	// 获取键盘宽度
	iKeyBoardWidth = pltBase->m_pfcGetLayoutWidth(pltObj, iCurLayoutId);
	iKeyBoardHeight = pltBase->m_pfcGetLayoutHeight(pltObj, iCurLayoutId);

	if (/*GBEngine_IsSymbolCandType(Global_GetEnineInstance())*/pEngineOutputInfo->isSymbolCandType
		&& (pEngineOutputInfo->pOutputString && pEngineOutputInfo->pOutputString[0] != 0))
	{
		ppCandidates = &pEngineOutputInfo->pOutputString;
		iCandidateCount = 1;		
	}
	else
	{
		ppCandidates = pEngineOutputInfo->ppSyllables;
		iCandidateCount = (GBUINT16)pEngineOutputInfo->nSyllableNum;
	}

	// 单个候选居左显示，保持产品风格一致
	if (iCandidateCount == 1)
	{
		textAlign = PT_TEXT_ALIGN_LEFT;
	}
	PT_CreateTouchRegion(&pltThis->syllableRegion,			// 音节栏区域
						 pltBase->syllableDisplayStartPos,		// 音节栏显示的开始位置
						 (GBUINT16)(iKeyBoardWidth - 10),	// 音节栏宽度
						 (GBUINT16)(iKeyBoardHeight / 2),	// 音节栏高度
						 ppCandidates,							// 音节数组
						 iCandidateCount,						// 音节个数
						 GBIME_CANDIDATE_FONT_SIZE,				// 字体
						 textAlign);							// 文本对齐
}

/*!
 * \brief 设置笔画触摸区
 * \param pltObj 
 * \param pEngineOutputInfo 
 * \return 
 * \note 
 * \example
 * \author Zhaokun
 * \date 2010-8-24 17:39:07
*/
static void CLayoutKB__SetOutputStringRegion(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo)
{
	CLASE_THIS_POITER(CLayoutKB, pltObj)
	
	GBPCWCHAR *ppStroke;
	GBUINT16 iCount = 0;
	PT_TextAlign textAlign = PT_TEXT_ALIGN_CENTER;
	GBUINT16 iKeyBoardHeight = 0;
	GBUINT16 iKeyBoardWidth = 0;
	GBUINT16 iCurrentLayoutId;
	
	ppStroke = &pEngineOutputInfo->pOutputString;
	iCount = 1;

	// 获取键盘高度
	iCurrentLayoutId = GET_LAYOUT_ID(pltBase);
	iKeyBoardHeight = pltBase->m_pfcGetLayoutHeight(pltObj,iCurrentLayoutId);
	iKeyBoardWidth = pltBase->m_pfcGetLayoutWidth(pltObj, iCurrentLayoutId);
	
// 	// 单个候选居左显示，保持产品风格一致
// 	if (iCount == 1)
// 	{
// 		textAlign = PT_TEXT_ALIGN_LEFT;
// 	}
	PT_CreateTouchRegion(&pltThis->syllableRegion,			// 音节栏区域
		pltBase->syllableDisplayStartPos,		// 音节栏显示的开始位置
		(GBUINT16)iKeyBoardWidth,	// 音节栏宽度
		(GBUINT16)(iKeyBoardHeight / 2),	// 音节栏高度
		ppStroke,							// 音节数组
		iCount,						// 音节个数
		GBIME_CANDIDATE_FONT_SIZE,				// 字体
		PT_TEXT_ALIGN_LEFT);							// 文本对齐
}
/*!
 * \brief 设置候选触摸区
 * \param pltObj 
 * \param pEngineOutputInfo 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-8-24 17:39:07
*/
static void CLayoutKB__SetCandidateRegion(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo)
{
	CLASE_THIS_POITER(CLayoutKB, pltObj)	
	
	GBIMEPoint candStartPoint;
	GBUINT16 iCurLayoutId;
	GBUINT16 iKeyBoardWidth, iKeyBoardHeight;	
	
	pltBase->m_pfcGetLayoutStartPoint(pltObj, &candStartPoint);	
	// 当前LayoutId
	iCurLayoutId = GET_LAYOUT_ID(pltBase);
	// 获取键盘宽度
	iKeyBoardWidth = pltBase->m_pfcGetLayoutWidth(pltObj, iCurLayoutId);
	iKeyBoardHeight = pltBase->m_pfcGetLayoutHeight(pltObj, iCurLayoutId);
	
	PT_CreateTouchRegion(&pltThis->candidateRegion,		// 候选汉字触摸区域
		pltBase->candidateDisplayStartPos,		// 候选汉字显示的开始位置
		(GBUINT16)(iKeyBoardWidth - 10),	// 候选栏宽度
		(GBUINT16)(iKeyBoardHeight / 2),	// 候选栏高度
		pEngineOutputInfo->ppCandidates,		// 候选数组
		(GBUINT16)pEngineOutputInfo->nCandNum,	// 候选个数
		GBIME_CANDIDATE_FONT_SIZE,				// 字体
		PT_TEXT_ALIGN_CENTER);					// 文本对齐	
}
/*!
 * \brief 设置标点坐标
 * \param pltObj 
 * \param pEngineOutputInfo 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-8-24 17:39:07
*/
static void CLayoutKB__SetSymbolCandRegion(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo)
{
	CLASE_THIS_POITER(CLayoutKB, pltObj)
	GBINT i = 0;
	GBUINT16 nCandNum = 0;
	GBIMEPoint candStartPoint;
	GBUINT16 iCurLayoutId;
	GBUINT16 iKeyBoardWidth, iKeyBoardHeight;

	nCandNum = pEngineOutputInfo->nCandNum / 2;

	pltBase->m_pfcGetLayoutStartPoint(pltObj, &candStartPoint);

	// 当前LayoutId
	iCurLayoutId = GET_LAYOUT_ID(pltBase);
	// 获取键盘宽度
	iKeyBoardWidth = pltBase->m_pfcGetLayoutWidth(pltObj, iCurLayoutId);
	iKeyBoardHeight = pltBase->m_pfcGetLayoutHeight(pltObj, iCurLayoutId);

	PT_CreateTouchRegion(&pltThis->syllableRegion,		// 候选汉字触摸区域
		candStartPoint,		// 候选汉字显示的开始位置
		(GBUINT16)(iKeyBoardWidth - 10),	// 候选栏宽度
		(GBUINT16)(iKeyBoardHeight / 2),	// 候选栏高度
		pEngineOutputInfo->ppCandidates,		// 候选数组
		nCandNum,	// 候选个数
		GBIME_CANDIDATE_FONT_SIZE,				// 字体
		PT_TEXT_ALIGN_CENTER);					// 文本对齐

	nCandNum  = pEngineOutputInfo->nCandNum - nCandNum;

	candStartPoint.y += (iKeyBoardHeight / 2);

	PT_CreateTouchRegion(&pltThis->candidateRegion,		// 候选汉字触摸区域
		candStartPoint,		// 候选汉字显示的开始位置
		(GBUINT16)(iKeyBoardWidth - 10),	// 候选栏宽度
		(GBUINT16)(iKeyBoardHeight / 2),	// 候选栏高度
		pEngineOutputInfo->ppCandidates + (pEngineOutputInfo->nCandNum - nCandNum),	// 候选数组
		nCandNum,	// 候选个数
		GBIME_CANDIDATE_FONT_SIZE,				// 字体
		PT_TEXT_ALIGN_CENTER);					// 文本对齐
}


/*!
 * \brief 设置音节翻页按钮触摸区
 * \param pltObj 
 * \param pEngineOutputInfo 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-10 11:44:11
*/
static void CLayoutKB__SetSyllablePageButtonRegion(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo)
{
	CLASE_THIS_POITER(CLayoutKB, pltObj)

	GBRECT buttonRect;
	PGBRECT pTargetRect;
	GBUINT16 iCandidateBoxHeight = 0;
	GBUINT16 iKeyBoardHeight = 0;
	GBUINT16 iKeyBoardWidth = 0;
	GBUINT16 iButtonHeight = 0;
	GBIMEPoint winPos;
	GBUINT16 iCurrentLayoutId, iCurrentLayerNumber;
	GBUINT16 iButtonLayoutId, iButtonLayerId;

	iCurrentLayerNumber = GET_LAYOUT_CURRENT_LAYER(pltBase);
	iButtonLayoutId = iCurrentLayoutId = GET_LAYOUT_ID(pltBase);
	iButtonLayerId = (iCurrentLayerNumber == ThirdLayer) ? ThirdLayer : SecondLayer;
	
	// 取得键显示的开始位置[左下角坐标]
	Global_GetWinPos(&winPos);
	iKeyBoardHeight = pltBase->m_pfcGetLayoutHeight(pltObj, iCurrentLayoutId);
	iKeyBoardWidth = pltBase->m_pfcGetLayoutWidth(pltObj, iCurrentLayoutId);
	// 键盘显示左上角Y坐标
	winPos.y -=  iKeyBoardHeight;
	// 取候选框高度
	iCandidateBoxHeight = pltBase->m_pfcGetCandidateBoxHeight(pltObj, iCurrentLayoutId, iCurrentLayerNumber);

	// 计算候选[向下]翻页的区域坐标
	pTargetRect = &pltThis->syllablePageDownOrUpRectArray[PAGE_DOWN_BUTTON_INDEX];
	pltBase->m_pfcGetBottonRectByKey(pltObj, iButtonLayoutId, iButtonLayerId, GBKEY_NEXT_PAGE_SYLLABLE, &buttonRect);
	iButtonHeight = buttonRect.bottom - buttonRect.top;
	pTargetRect->left = buttonRect.left + (GBUINT16)winPos.x;
	pTargetRect->top = (GBUINT16)winPos.y - iCandidateBoxHeight;
	pTargetRect->right = buttonRect.right + (GBUINT16)winPos.x;
	pTargetRect->bottom = (GBUINT16)winPos.y - (iCandidateBoxHeight - iButtonHeight);
	
	// 计算候选[向上]翻页的区域坐标
	pTargetRect = &pltThis->syllablePageDownOrUpRectArray[PAGE_Up_BUTTON_INDEX];
	pltBase->m_pfcGetBottonRectByKey(pltObj, iButtonLayoutId, iButtonLayerId, GBKEY_PREV_PAGE_SYLLABLE, &buttonRect);
	iButtonHeight = buttonRect.bottom - buttonRect.top;
	pTargetRect->left = buttonRect.left + (GBUINT16)winPos.x;
	pTargetRect->top = (GBUINT16)winPos.y - iCandidateBoxHeight;
	pTargetRect->right = buttonRect.right + (GBUINT16)winPos.x;
	pTargetRect->bottom = (GBUINT16)winPos.y - (iCandidateBoxHeight - iButtonHeight);
}

/*!
 * \brief 设置候选翻页按钮触摸区
 * \param pltObj 
 * \param pEngineOutputInfo 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-10 11:46:08
*/
static void CLayoutKB__SetCandidatePageButtonRegion(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo)
{
	CLASE_THIS_POITER(CLayoutKB, pltObj)

	GBRECT buttonRect;
	PGBRECT pTargetRect;
	GBUINT16 iCandidateBoxHeight = 0;
	GBUINT16 iKeyBoardHeight = 0;
	GBUINT16 iButtonHeight = 0;
	GBIMEPoint winPos;
	GBUINT16 iCurrentLayoutId, iCurrentLayerNumber;
	GBUINT16 iButtonLayoutId, iButtonLayerId;
	
	iCurrentLayerNumber = GET_LAYOUT_CURRENT_LAYER(pltBase);
	iButtonLayoutId = iCurrentLayoutId = GET_LAYOUT_ID(pltBase);
	iButtonLayerId = (iCurrentLayerNumber == ThirdLayer) ? ThirdLayer : SecondLayer;
	
	// 取得键显示的开始位置[左下角坐标]
	Global_GetWinPos(&winPos);
	iKeyBoardHeight = pltBase->m_pfcGetLayoutHeight(pltObj, iCurrentLayoutId);
	// 键盘显示左上角Y坐标
	winPos.y -=  iKeyBoardHeight;
	// 取候选框高度
	iCandidateBoxHeight = pltBase->m_pfcGetCandidateBoxHeight(pltObj, iCurrentLayoutId, iCurrentLayerNumber);

	// 计算候选[向下]翻页的区域坐标
	pTargetRect = &pltThis->candidatePageDownOrUpRectArray[PAGE_DOWN_BUTTON_INDEX];
	pltBase->m_pfcGetBottonRectByKey(pltObj, iButtonLayoutId, iButtonLayerId, GBKEY_NEXT_PAGE_CANDIATE, &buttonRect);
	pTargetRect->left = buttonRect.left + (GBUINT16)winPos.x;
	pTargetRect->top = (GBUINT16)winPos.y - (iCandidateBoxHeight - buttonRect.top);
	pTargetRect->right = buttonRect.right + (GBUINT16)winPos.x;
	pTargetRect->bottom = (GBUINT16)winPos.y;

	// 计算候选[向上]翻页的区域坐标
	pTargetRect = &pltThis->candidatePageDownOrUpRectArray[PAGE_Up_BUTTON_INDEX];
	pltBase->m_pfcGetBottonRectByKey(pltObj, iButtonLayoutId, iButtonLayerId, GBKEY_PREV_PAGE_CANDIATE, &buttonRect);
	pTargetRect->left = buttonRect.left + (GBUINT16)winPos.x;
	pTargetRect->top = (GBUINT16)winPos.y - (iCandidateBoxHeight - buttonRect.top);
	pTargetRect->right = buttonRect.right + (GBUINT16)winPos.x;
	pTargetRect->bottom = (GBUINT16)winPos.y;
}

/*!
 * \brief 更新layout信息
 * \param pltObj 
 * \param pEngineOutputInfo 
 * \return 
 * \note 
 * \example
 * \author Zhaokun
 * \date 2010-10-22 11:46:08
*/
static void CLayoutKB__UpdateLayoutInformation(GBLPVOID pltObj, 
													   ButtonStatusEnum buttonStatus,	
													   PEngineOutputInfo pEngineOutputInfo)
{
	CLASE_THIS_POITER(CLayoutKB, pltObj)
		
	GBUINT16 iKeyBoardHeight = 0;
	GBUINT16 iCurrentLayoutId;
	
	// 当前LayoutId
	iCurrentLayoutId = GET_LAYOUT_ID(pltBase);
	// 获取键盘高度
	iKeyBoardHeight = pltBase->m_pfcGetLayoutHeight(pltObj,iCurrentLayoutId);
	
	
	if (buttonStatus == ButtonStatusDown)
	{
		// 打开候选框
		if (pEngineOutputInfo->bCandWindOpen)
		{
			pltBase->m_pfcChangeHeight(pltObj, iKeyBoardHeight);
		}

		if (pEngineOutputInfo->isMultiTapInputMode && pEngineOutputInfo->bCandWindOpen
			&& !pEngineOutputInfo->isSymbolCandType/*GBEngine_IsSymbolCandType(Global_GetEnineInstance())*/)
		{
			GBINT nCandIndex = 0;
			nCandIndex = pEngineOutputInfo->pRowStart[pEngineOutputInfo->nSelRow] 
				+ pEngineOutputInfo->nSelCol;

			PT_LayerLock();
			if (pEngineOutputInfo->pUpScreenStr && pEngineOutputInfo->pUpScreenStr[0]) // 数字上屏
			{
				GBInputBox_MultitapInputConfirm();
				pltBase->multitapReplaceCandLength = 0;
				pltBase->currentCandidateIndex = 0;
				//pltBase->m_pfcUpScreen(pltObj, pEngineOutputInfo->pUpScreenStr);
				//pEngineOutputInfo->pUpScreenStr[0] = 0;				
// 				GBInputBox_ClearHilight();
// 				GBInputBox_Redraw();
			}

			PT_PrintLogInfo(GBLOG_LAYOUTCONTROL,
				("\n候选个数：%d，col：%d，row：%d，候选：%s\n"
				,pEngineOutputInfo->nCandNum, pEngineOutputInfo->nSelCol
				, pEngineOutputInfo->nSelRow, pEngineOutputInfo->ppCandidates[pEngineOutputInfo->nSelCol]));

			pltBase->multitapReplaceCandLength =
				GBInputBox_ReplaceHighlightString(pltBase->multitapReplaceCandLength,
				pEngineOutputInfo->ppCandidates[nCandIndex]);
			PT_LayerUnLock();

//			GBInputBox_MultiTapInsertChar(pEngineOutputInfo->ppCandidates[nCandIndex]);
// 			PT_GUIStartTimer(GBIME_MULTITAP_TIMER_INTERVAL, pltBase->m_pfcOnMultitapTimer);
			pltBase->bMultitapTimerStartFlag = GBTrue;
			return ;
		}
		if (pEngineOutputInfo->isMultiTapInputMode 
			&& pltBase->bMultitapTimerStartFlag
			&& pEngineOutputInfo->pUpScreenStr && pEngineOutputInfo->pUpScreenStr[0]) // 数字上屏
		{
			pltBase->multitapReplaceCandLength =
				GBInputBox_ReplaceHighlightString(pltBase->multitapReplaceCandLength,
				pEngineOutputInfo->pUpScreenStr);
			GBInputBox_MultitapInputConfirm();
			pltBase->currentCandidateIndex = 0;
			pltBase->multitapReplaceCandLength = 0;
		}
		else if (/*!pEngineOutputInfo->isMultiTapInputMode && */pEngineOutputInfo->pUpScreenStr && pEngineOutputInfo->pUpScreenStr[0]) // 数字上屏
		{			
			pltBase->currentCandidateIndex = 0;
			pltBase->m_pfcUpScreen(pltObj, pEngineOutputInfo->pUpScreenStr);
		}

		// 输入串为空时关闭候选框[数字模式没有候选框]
		if (!pEngineOutputInfo->bCandWindOpen)
		{
			pltBase->currentSyllableIndex = 0; // 清零音节索引
			Global_SetLayoutWinActiveStatus(GBFalse);
			GBInputBox_SetAllPlatformFuncKey();
			pltBase->m_pfcChangeHeight(pltObj, 0);
			return;
		}
	}
}

#endif // 国笔物理键盘