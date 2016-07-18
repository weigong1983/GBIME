/*****************************************************************************
*  Copyright Statement:
*  --------------------
*
* Filename:
* ---------
*    CLayoutAbstract.c
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
#include "CLayoutAbstract.h"
#include "GB_Global.h"

#define GBIME_SYMBOL_0_ROW_NUM				2
#define GBIME_SYMBOL_1_ROW_NUM				2
#define GBIME_MULTITAP_SYMBOLS_0 			L" \n"

GBIMELOCAL GBIMEReturn CLayoutAbstract__Init(PCLayoutAbstract pltDest);
GBIMELOCAL void CLayoutAbstract__InitVariable(GBLPVOID pltObj);
GBIMELOCAL GBIMEReturn CLayoutAbstract__ReleaseBase(GBLPVOID pltObj, PLayoutInfo pltUIData);
GBIMELOCAL GBIMEReturn CLayoutAbstract__SetRefreshFlag(GBLPVOID pltObj, GBUINT refreshFlag);
GBIMELOCAL GBIMEReturn CLayoutAbstract__Paint(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo);
//GBIMELOCAL GBIMEReturn CLayoutAbstract__PreHandleEvent(GBLPVOID pltObj, GBINT nPlatformEvent, GBINT wParam, GBINT lParam, PEngineOutputInfo pEngineOutputInfo);
GBIMELOCAL PenPosition CLayoutAbstract__PenPositionInKeyboardButton(GBLPVOID pltObj, GBIMEPoint hitPoint, 
																	ButtonStatusEnum buttonStatus, PEngineOutputInfo pEngineOutputInfo);
GBIMELOCAL PenPosition CLayoutAbstract__HitTest(GBLPVOID pltObj, GBIMEPoint hitPoint, ButtonStatusEnum buttonStatus, PEngineOutputInfo pEngineOutputInfo);
GBIMELOCAL GBIMEReturn CLayoutAbstract__HandleCommonEvent(GBLPVOID pltObj, PGBIMEEvent pIMEEvent, PEngineOutputInfo pEngineOutputInfo);
GBIMELOCAL GBIMEReturn CLayoutAbstract__ResetEventGroup(GBLPVOID pltObj);
GBIMELOCAL GBIMEReturn CLayoutAbstract__FillEventGroup(GBLPVOID pltObj, GBIMEConvertTypeEnum convertType, GBINT wParam, GBINT lParam);
GBIMELOCAL PGBIMEEventGroup CLayoutAbstract__GetIMEEventGroup(GBLPVOID pltObj);
GBIMELOCAL GBIMEReturn CLayoutAbstract__HandleEvent(GBLPVOID pltObj, PGBIMEEvent pIMEEvent, PEngineOutputInfo pEngineOutputInfo);
GBIMELOCAL GBIMEReturn CLayoutAbstract__SetConfig(void);
GBIMELOCAL GBIMEReturn CLayoutAbstract__SetInputMethod(void);
GBIMELOCAL GBIMEReturn CLayoutAbstract__Listenner(void);
GBIMELOCAL GBIMEReturn CLayoutAbstract__IsMatchLtObjAndLtID(GBLPVOID pltObj, GBUINT16 ltID);
GBIMELOCAL GBIMEReturn CLayoutAbstract__IsNeedLeave(GBLPVOID pltObj);
GBIMELOCAL GBIMEReturn CLayoutAbstract__ChangeHeight(GBLPVOID pltObj, GBINT newHeight);
GBIMELOCAL GBIMEReturn CLayoutAbstract__UpScreen(GBLPVOID pltObj, GBPCWCHAR pCand);
GBIMELOCAL GBIMEReturn CLayoutAbstract__Redraw(GBLPVOID pltObj);
GBIMELOCAL GBUINT16 CLayoutAbstract_GetLayoutHeight(GBLPVOID pltObj, LayoutIdEnum layoutId);
GBIMELOCAL GBUINT16 CLayoutAbstract_GetLayoutWidth(GBLPVOID pltObj, LayoutIdEnum layoutId);
GBIMELOCAL GBBOOL CLayoutAbstract_GetBottonRectByKey(GBLPVOID pltObj,LayoutIdEnum layoutId,
								  LayerIndex layerId, GBUINT16 iKeyValue, PGBRECT pRect);
GBIMELOCAL PLAYOUTCONSTLAYER CLayoutAbstract_GetLayerConstInfo(GBLPVOID pltObj, LayoutIdEnum layoutId, LayerIndex layerId);
GBIMELOCAL PBUTTONCONSTDATA CLayoutAbstract_GetLayerConstButtons(GBLPVOID pltObj, LayoutIdEnum layoutId, LayerIndex layerId);
GBIMELOCAL GBIMEReturn CLayoutAbstract_SwitchHandler(GBLPVOID pltObj);
GBIMELOCAL GBIMEReturn CLayoutAbstract_FuncKeyHandler(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo);
//GBIMELOCAL GBIMEReturn CLayoutAbstract_PhysicFuncKeyHandler(GBLPVOID pltObj, GBINT nPlatformEvent, GBINT wParam, GBINT lParam, PEngineOutputInfo pEngineOutputInfo);
GBIMELOCAL GBIMEReturn CLayoutAbstract_SwitchPreLayoutObj(GBLPVOID pltObj);
GBIMELOCAL GBINT16 CLayoutAbstract_GetCandidateBoxHeight(GBLPVOID pltObj, LayoutIdEnum layoutId, LayerIndex layerId);
GBIMELOCAL void CLayoutAbstract_GetWinPos(GBLPVOID pltObj, PGBIMEPoint pWinPos);
GBIMELOCAL void CLayoutAbstract_GetLayoutWinRect(GBLPVOID pltObj, PGBRECT pLayoutWinRect);
GBIMELOCAL void CLayoutAbstract_GetLayoutStartPoint(GBLPVOID pltObj, PGBIMEPoint startPoint);
GBIMELOCAL void CLayoutAbstract_SetSyllableDisplayStartPos(GBLPVOID pltObj, GBINT16 x, GBINT16 y);
GBIMELOCAL void CLayoutAbstract_SetCandidateDisplayStartPos(GBLPVOID pltObj, GBINT16 x, GBINT16 y);
GBIMELOCAL GBBOOL CLayoutAbstract_IsInputKey(GBUINT16 keyValue);
GBIMELOCAL GBIMEReturn CLayoutAbstract_SlidingPenDownHandler(GBLPVOID pltObj, GBIMEPoint pt);
GBIMELOCAL GBIMEReturn CLayoutAbstract_SlidingPenUpHandler(GBLPVOID pltObj, GBIMEPoint pt);
GBIMELOCAL GBIMEReturn CLayoutAbstract_SlidingPenMoveHandler(GBLPVOID pltObj, GBIMEPoint pt);
GBIMELOCAL GBIMEReturn CLayoutAbstract_LongPressKeyHandler(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo);
GBIMELOCAL void CLayoutAbstract_DeleteAll(void);
GBIMELOCAL GBIMEReturn CLayoutAbstract_ShowCandWin(GBLPVOID pltObj, ButtonStatusEnum buttonStatus, PEngineOutputInfo pEngineOutputInfo);
GBIMELOCAL GBIMEReturn CLayoutAbstract_ShowAssociateWin(GBLPVOID pltObj, ButtonStatusEnum buttonStatus, PEngineOutputInfo pEngineOutputInfo);
GBIMELOCAL GBIMEReturn CLayoutAbstract_CloseCandWin(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo);
GBIMELOCAL GBIMEReturn CLayoutAbstract_CheckUpScreen(GBLPVOID pltObj, ButtonStatusEnum buttonStatus, PEngineOutputInfo pEngineOutputInfo);
GBIMELOCAL GBIMEReturn CLayoutAbstract_MultiTapSymbolHandler(GBLPVOID pltObj);
GBIMELOCAL GBIMEReturn CLayoutAbstract_MultiTapFaceHandler(GBLPVOID pltObj);
GBIMELOCAL void CLayoutAbstract_OnMultitapTimer(void);
GBIMELOCAL GBIMEReturn CLayoutAbstract_SwitchShiftCapStatus(GBLPVOID pltObj, GBShiftCapStatus shiftCapStatus, GBBOOL bAuto,	PEngineOutputInfo pEngineOutputInfo);
GBIMELOCAL GBIMEReturn CLayoutAbstract_SyncEngineShiftCapStatus(GBLPVOID pltObj);
GBIMELOCAL void CLayoutAbstract_KBMultiTapTimerCallBack(void);
GBIMELOCAL GBIMEReturn CLayoutAbstract_SetInterpunctionKey(GBLPVOID pltObj);
GBIMELOCAL GBIMEReturn CLayoutAbstract_DisableFuncKeyHandler(GBLPVOID pltObj);


GBIMEGLOBAL CLayoutAbstract * CLayoutAbstract__Construct(PCLayoutAbstract pltDest, 
											 GBLPCVOID pcltUIData,
											 GBLPCVOID pcltIMData,
											 GBLPCVOID hcltImage)
{
	memset(pltDest, 0, sizeof(CLayoutAbstract));
	pltDest->m_ltType = CLayoutInfo_GetLayoutTypeFromLtData(pcltUIData);
	pltDest->m_ltData.pltcData = pcltUIData;
	pltDest->m_pltIMData = pcltIMData;
	Global_SetLayoutWinWidth(PT_GetLcdWidth());
	Global_SetLayoutWinHeight(0);
	pltDest->m_refreshFlag = GBIME_LAYOUT_REFRESH_NONE;

	CLayoutInfo_LayoutInitData(&pltDest->m_ltData, pcltUIData);

	CLayoutAbstract__Init(pltDest);

	return pltDest;
}


GBIMELOCAL GBIMEReturn CLayoutAbstract__Init(PCLayoutAbstract pltDest)
{
	//pltDest->m_pfcInit = CLayoutAbstract__Init;
	pltDest->m_pfcInitVariable = CLayoutAbstract__InitVariable;
	pltDest->m_pfcRelease = CLayoutAbstract__ReleaseBase;	//缺省
	pltDest->m_pfcReleaseBase = CLayoutAbstract__ReleaseBase;
	pltDest->m_pfcSetRefreshFlag = CLayoutAbstract__SetRefreshFlag;
	pltDest->m_pfcPaint = CLayoutAbstract__Paint;
	//pltDest->m_pfcPreHandleEvent = CLayoutAbstract__PreHandleEvent; // VK重载
	pltDest->m_pfcPenPositionInKeyboardButton = CLayoutAbstract__PenPositionInKeyboardButton;
	pltDest->m_pfcHitTest = CLayoutAbstract__HitTest;
	pltDest->m_pfcHandleCommonEvent = CLayoutAbstract__HandleCommonEvent;
	//pltDest->m_pfcDispatchEvent = CLayoutAbstract__DispatchEvent;
	pltDest->m_pfcHandleEvent = CLayoutAbstract__HandleEvent;	
	pltDest->m_pfcSetConfig = CLayoutAbstract__SetConfig;
	pltDest->m_pfcSetIM = CLayoutAbstract__SetInputMethod;
	pltDest->m_pfcListenner = CLayoutAbstract__Listenner;
	pltDest->m_pfcIsMatchLtObjAndLtID = CLayoutAbstract__IsMatchLtObjAndLtID;
	pltDest->m_pfcIsNeedLeave = CLayoutAbstract__IsNeedLeave;	
	pltDest->m_pfcChangeHeight = CLayoutAbstract__ChangeHeight;
	pltDest->m_pfcUpScreen = CLayoutAbstract__UpScreen;
	pltDest->m_pfcRedraw = CLayoutAbstract__Redraw;
	pltDest->m_pfcGetLayoutHeight = CLayoutAbstract_GetLayoutHeight;
	pltDest->m_pfcGetLayoutWidth = CLayoutAbstract_GetLayoutWidth;
	pltDest->m_pfcGetBottonRectByKey = CLayoutAbstract_GetBottonRectByKey;
	pltDest->m_pfcGetLayerConstInfo = CLayoutAbstract_GetLayerConstInfo;
	pltDest->m_pfcGetLayerConstButtons = CLayoutAbstract_GetLayerConstButtons;
	pltDest->m_pfcSwitchHandler = CLayoutAbstract_SwitchHandler;
	pltDest->m_pfcFuncKeyHandler = CLayoutAbstract_FuncKeyHandler;
	//pltDest->m_pfcPhysicFuncKeyHandler = CLayoutAbstract_PhysicFuncKeyHandler; // VK重载
	pltDest->m_pfcLongPressKeyHandler = CLayoutAbstract_LongPressKeyHandler;
	pltDest->m_pfcSwitchPreLayoutObj = CLayoutAbstract_SwitchPreLayoutObj;
	pltDest->m_pfcGetCandidateBoxHeight = CLayoutAbstract_GetCandidateBoxHeight;
	pltDest->m_pfcGetWinPos = CLayoutAbstract_GetWinPos;
	pltDest->m_pfcGetLayoutWinRect = CLayoutAbstract_GetLayoutWinRect;
	pltDest->m_pfcGetLayoutStartPoint = CLayoutAbstract_GetLayoutStartPoint;
	pltDest->m_pfcSetSyllableDisplayStartPos = CLayoutAbstract_SetSyllableDisplayStartPos;
	pltDest->m_pfcSetCandidateDisplayStartPos = CLayoutAbstract_SetCandidateDisplayStartPos;
	pltDest->m_pfcIsInputKey = CLayoutAbstract_IsInputKey;	
	pltDest->m_pfcSlidingPenDownHandler = CLayoutAbstract_SlidingPenDownHandler;
	pltDest->m_pfcSlidingPenUpHandler = CLayoutAbstract_SlidingPenUpHandler;
	pltDest->m_pfcSlidingPenMoveHandler = CLayoutAbstract_SlidingPenMoveHandler;
	pltDest->m_pfcShowCandWin = CLayoutAbstract_ShowCandWin;
	pltDest->m_pfcShowAssociateWin = CLayoutAbstract_ShowAssociateWin;
	pltDest->m_pfcCloseCandWin = CLayoutAbstract_CloseCandWin;
	pltDest->m_pfcCheckUpScreen = CLayoutAbstract_CheckUpScreen;
	pltDest->m_pfcMultiTapSymbolHandler = CLayoutAbstract_MultiTapSymbolHandler;
	pltDest->m_pfcMultiTapFaceHandler = CLayoutAbstract_MultiTapFaceHandler;
	pltDest->m_pfcOnMultitapTimer = CLayoutAbstract_OnMultitapTimer;
	pltDest->m_pfcSwitchShiftCapStatus = CLayoutAbstract_SwitchShiftCapStatus;
	pltDest->m_pfcSyncEngineShiftCapStatus = CLayoutAbstract_SyncEngineShiftCapStatus;
	pltDest->m_pfcKBMultiTapTimerCallBack = CLayoutAbstract_KBMultiTapTimerCallBack;
	pltDest->m_pfcSetInterpunctionKey = CLayoutAbstract_SetInterpunctionKey;
	pltDest->m_pfcDisableFuncKeyHandler = CLayoutAbstract_DisableFuncKeyHandler;
	pltDest->lastUseTime = 0;

	return GBIME_OK;
}

/*!
 * \brief 初始化Layout相关变量
 * \param pltObj 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-10-29 15:06:27
 */
GBIMELOCAL void CLayoutAbstract__InitVariable(GBLPVOID pltObj)
{
	PCLayoutAbstract pltAbstractObj = (PCLayoutAbstract)pltObj;

	// 滑屏结构体
	memset(&pltAbstractObj->slidingOperation, 0, sizeof(SlidingOperation));
	pltAbstractObj->isSlidingDisabled = GBFalse;

	/*!
	 * \brief Layout 刷新显示及消息处理相关变量
	 */
	pltAbstractObj->m_refreshFlag = GBIME_LAYOUT_REFRESH_NONE;
	memset(&pltAbstractObj->syllableDisplayStartPos, 0, sizeof(GBIMEPoint));///<音节栏起始显示位置	
	memset(&pltAbstractObj->candidateDisplayStartPos, 0, sizeof(GBIMEPoint));///<候选栏起始显示位置	
	pltAbstractObj->pressLayoutKeyValue = 0;///<记录按下时的Layout按键值(只有当按下和弹起键值一致时才执行按键操作)
	pltAbstractObj->currentLayoutKeyValue = 0;///<当前点中的按键对应键值
	pltAbstractObj->currentSyllableIndex = 0;///<当前选中的音节索引
	pltAbstractObj->currentCandidateIndex = 0;///<当前选中的候选索引
	pltAbstractObj->penDownPostion = PenPositionInNothing;///<触摸笔按下的位置
	pltAbstractObj->penInPostion = PenPositionInNothing;///<当前点中的位置
	pltAbstractObj->buttonStatus = ButtonStatusNomal;///<按钮状态

	/*!
	 * \brief Multitap符号功能相关变量
	 * note: 清除MultiTap状态之前必须先重置当前Layout的MultiTap定时器, 取消高亮显示
	 */
	if (pltAbstractObj->m_pfcKBMultiTapTimerCallBack != NULL)
	{
		pltAbstractObj->m_pfcKBMultiTapTimerCallBack();
	}
	if (pltAbstractObj->m_pfcOnMultitapTimer != NULL)
	{
		pltAbstractObj->m_pfcOnMultitapTimer();
	}

	pltAbstractObj->bMultitapTimerStartFlag = GBFalse;///<MultiTap定时器启动标记
	pltAbstractObj->bMultitapSymbolStartFlag = GBFalse;///<MultiTap符号启动标记
	pltAbstractObj->bMultitapFaceStartFlag = GBFalse;///<MultiTap表情启动标记
	pltAbstractObj->multitapReplaceCandLength = 0;///<高亮替换候选字符长度
	pltAbstractObj->multitapCandIndex = 0;///<Multitap候选符号当前索引

	/*!
	 * \brief GBV5引擎交互相关变量
	 */
	pltAbstractObj->bSendEngineKeyEvent = GBFalse;///<是否需要发送引擎按键事件 
	pltAbstractObj->engineKeyEvent = GBEvent_None;///<经过Layout预处理之后转换后的引擎按键事件
	pltAbstractObj->engineEventParam = 0;///<经过Layout预处理之后转换后的引擎事件参数
	pltAbstractObj->bUpToInputKeySequence = GBFalse;///<数字上屏标记
}

GBIMELOCAL GBIMEReturn CLayoutAbstract__ReleaseBase(GBLPVOID pltObj, PLayoutInfo pltUIData)
{
	PCLayoutAbstract pltAbstractObj = (PCLayoutAbstract)pltObj;

	CLayoutInfo_LayoutReleaseData(pltUIData);

	if (pltAbstractObj != NULL)
	{
		if (pltAbstractObj->m_hltView != NULL)
		{
			PT_Free(pltAbstractObj->m_hltView);
			pltAbstractObj->m_hltView = NULL;
		}
		
		if (pltAbstractObj->pSymbolChineseString != NULL)
		{
			PT_Free(pltAbstractObj->pSymbolChineseString);
			pltAbstractObj->pSymbolChineseString = NULL;
		}
		if (pltAbstractObj->pSymbolEnglishString != NULL)
		{
			PT_Free(pltAbstractObj->pSymbolEnglishString);
			pltAbstractObj->pSymbolEnglishString = NULL;
		}
		if (pltAbstractObj->pFaceString != NULL)
		{
			PT_Free(pltAbstractObj->pFaceString);
			pltAbstractObj->pFaceString = NULL;
		}
		//该处不能释放，外面有用到 Zhaokun
// 		PT_Free(pltAbstractObj);
// 		pltAbstractObj = NULL;

		pltAbstractObj->lastUseTime = 0;
	}
	

	return GBIME_OK;
}

/*!
 * \brief 设置Layout显示面板的刷新标记，避免无效刷新，提高刷新速度
 * \param pltObj 
 * \param refreshFlag 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-27 15:15:47
 */
GBIMELOCAL GBIMEReturn CLayoutAbstract__SetRefreshFlag(GBLPVOID pltObj, GBUINT refreshFlag)
{
	PCLayoutAbstract pltTempObj = (PCLayoutAbstract)pltObj;
	PT_Assert(pltTempObj);
	pltTempObj->m_refreshFlag |= refreshFlag;
	return GBIME_OK;
}

GBIMELOCAL GBIMEReturn CLayoutAbstract__Paint(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo)
{
	return GBIME_OK;
}

/*!
 * \brief 判断当前触摸笔是否点击在键盘按钮上
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
GBIMELOCAL PenPosition CLayoutAbstract__PenPositionInKeyboardButton(GBLPVOID pltObj, 
																	GBIMEPoint hitPoint, 
																	ButtonStatusEnum buttonStatus,
																	PEngineOutputInfo pEngineOutputInfo)
{
	PCLayoutAbstract pltAbstractObj = (PCLayoutAbstract)pltObj;
    GBUINT16 iCandidateBoxHeight = 0;
	GBUINT16 iKeyBoardHeight = 0;
	PenPosition penPositionInWhere = PenPositionInNothing;
	GBUINT16 iCurrentLayoutId;

	//当前要显示的层
	GBUINT16 iCurrentLayerNumber;
	GBUINT16 iButtonTotalCount;
	GBUINT16 iCount;
	GBUINT16 iEngineKeyValue;
	//GBINT32 left,top,right,buttom;
	GBIMEPoint winPos;
	GBRECT buttonPos;
	PLayoutChgtLayer pLayoutChgtLayer;

	// 当前LayoutId
	iCurrentLayoutId = GET_LAYOUT_ID(pltAbstractObj);
	// 获取键盘高度
	iKeyBoardHeight = pltAbstractObj->m_pfcGetLayoutHeight(pltObj,iCurrentLayoutId);
	// 取得键盘显示的开始位置【左下角】，并调整为【左上角】
	Global_GetWinPos(&winPos);	
	winPos.y -=  iKeyBoardHeight;
	// 当前要显示的层
	iCurrentLayerNumber = GET_LAYOUT_CURRENT_LAYER(pltAbstractObj);
	// 取候选框高度
	iCandidateBoxHeight = pltAbstractObj->m_pfcGetCandidateBoxHeight(pltObj,iCurrentLayoutId,iCurrentLayerNumber);
	//先查第地层
	pLayoutChgtLayer = (PLayoutChgtLayer)&(pltAbstractObj->m_ltData.pltLayerData[FirstLayer]);

	// 第一，二或三层时，都会查询键盘
	iButtonTotalCount = pLayoutChgtLayer->lyBtCount;

	// 默认为无效点击位置
	pltAbstractObj->penInPostion = PenPositionInNothing;

	// 查询每一个按钮
	for (iCount = 0; iCount < iButtonTotalCount; iCount++)
	{
		// 获取当前按钮位置并转换为屏幕相对坐标
		buttonPos = pLayoutChgtLayer->plycBtData[iCount].btPos;
		buttonPos.top += winPos.y;
		buttonPos.bottom += winPos.y;

		if (PT_PointIsInRect(hitPoint, buttonPos))
		{
			// 点击灰色禁用按钮，操作无效
			if (pLayoutChgtLayer->plyBtChgData[iCount].btStatus == ButtonStatusGray)
			{
				return pltAbstractObj->penInPostion;
			}

			// 按键按下状态[反显示用]
			pLayoutChgtLayer->plyBtChgData[iCount].btStatus = buttonStatus;			
			
			// 按到键盘的按钮上
			penPositionInWhere = PenPositionInKeyboardButton;
			pltAbstractObj->buttonStatus = buttonStatus; // 记录按钮状态
			pltAbstractObj->penInPostion = penPositionInWhere;

			// 记录按下时的Layout按键值(只有当按下和弹起键值一致时才执行按键操作)
			if (buttonStatus == ButtonStatusDown)
			{
				pltAbstractObj->penDownPostion = PenPositionInKeyboardButton;
				pltAbstractObj->pressLayoutKeyValue = pLayoutChgtLayer->plycBtData[iCount].btKey;
			}

			// 取得当前按钮键值
			pltAbstractObj->currentLayoutKeyValue = pLayoutChgtLayer->plycBtData[iCount].btKey;
			// 转化为引擎键值
			iEngineKeyValue = CLayoutInfo_ConversionLayoutKeyToEngineKey(pltAbstractObj->currentLayoutKeyValue);
			break;
		}
	}

	// 离开键盘
	if (iCount == iButtonTotalCount
		&& pltAbstractObj->penDownPostion == PenPositionInKeyboardButton)
	{
		// 记录按钮状态
		pltAbstractObj->buttonStatus = buttonStatus;
		// 记录点中的位置
		penPositionInWhere = pltAbstractObj->penInPostion = PenPositionLeave;			
		// 记录点中的候选索引值
		pltAbstractObj->currentLayoutKeyValue = GBEvent_None;
	}
	
	//如果是第二或第三层时，还需要查点中的是否是第二或第三层的非候选区按钮
	if (iCurrentLayerNumber == SecondLayer || iCurrentLayerNumber == ThirdLayer)
	{
		//先查第地层
		pLayoutChgtLayer = (PLayoutChgtLayer)&(pltAbstractObj->m_ltData.pltLayerData[iCurrentLayerNumber]);
		//二或三层时，都会查询键盘
		iButtonTotalCount = pLayoutChgtLayer->lyBtCount;
		
		// 查询每一个按钮
		for (iCount = 0; iCount < iButtonTotalCount; iCount++)
		{
			//如果是第二或第三层的非候选区按钮
			if (pLayoutChgtLayer->plycBtData[iCount].btPos.bottom > iCandidateBoxHeight)
			{
				// 获取当前按钮位置并转换为屏幕相对坐标
				buttonPos = pLayoutChgtLayer->plycBtData[iCount].btPos;
				buttonPos.top += (winPos.y - iCandidateBoxHeight);
				buttonPos.bottom += (winPos.y - iCandidateBoxHeight);
				//如果点中了第二或第三层的非候选区按钮
				if (PT_PointIsInRect(hitPoint, buttonPos))
				{
					// 按键按下状态[反显示用]
					pLayoutChgtLayer->plyBtChgData[iCount].btStatus = buttonStatus;			
					
					// 按到键盘的按钮上
					penPositionInWhere = PenPositionInKeyboardButton;
					pltAbstractObj->penInPostion = penPositionInWhere;
					pltAbstractObj->buttonStatus = buttonStatus; // 记录按钮状态

					// 记录按下时的Layout按键值(只有当按下和弹起键值一致时才执行按键操作)
					if (buttonStatus == ButtonStatusDown)
					{
						pltAbstractObj->pressLayoutKeyValue = pLayoutChgtLayer->plycBtData[iCount].btKey;
					}

					// 取得当前按钮键值
					pltAbstractObj->currentLayoutKeyValue = pLayoutChgtLayer->plycBtData[iCount].btKey;
					// 转化为引擎键值
					iEngineKeyValue = CLayoutInfo_ConversionLayoutKeyToEngineKey(pltAbstractObj->currentLayoutKeyValue);
					break;
				}
			}
		}
	}

	return pltAbstractObj->penInPostion;
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
GBIMELOCAL PenPosition CLayoutAbstract__HitTest(GBLPVOID pltObj, GBIMEPoint hitPoint, ButtonStatusEnum buttonStatus, PEngineOutputInfo pEngineOutputInfo)
{
	return PenPositionInNothing;
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
GBIMELOCAL GBIMEReturn CLayoutAbstract__HandleCommonEvent(GBLPVOID pltObj, PGBIMEEvent pIMEEvent, PEngineOutputInfo pEngineOutputInfo)
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
		if (IS_KB_LAYOUT(nCurrentLayoutID))
		{
			Global_SetLayoutWinHeight(0);
		} 
		else // 虚拟键盘首次激活显示键盘
		{
			Global_SetLayoutWinHeight(pltAbstractObj->m_pfcGetLayoutHeight(pltObj, nCurrentLayoutID));
		}
	
		// 根据平台输入模式决定是否禁用某些功能按钮
		pltAbstractObj->m_pfcDisableFuncKeyHandler(pltObj);

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
		if (IS_KB_LAYOUT(nCurrentLayoutID))
		{
			pltAbstractObj->m_pfcChangeHeight(pltObj, 0);
		} 
		else
		{
			pltAbstractObj->m_pfcChangeHeight(pltObj, pltAbstractObj->m_pfcGetLayoutHeight(pltObj, nCurrentLayoutID));
		}		
		pltAbstractObj->m_refreshFlag = GBIME_LAYOUT_REFRESH_ALL;
		break;
	case GBIME_EVENT_LAYOUT_ENTER:
		pltAbstractObj->m_ltData.ltCurLayer = FirstLayer;
		//Global_SetLayoutWinHeight(0); // 高度清零，解决Layout间切换由于高度一样没有刷新VK
		pltAbstractObj->m_pfcChangeHeight(pltObj, pltAbstractObj->m_pfcGetLayoutHeight(pltObj, nCurrentLayoutID));
		pltAbstractObj->m_refreshFlag = GBIME_LAYOUT_REFRESH_ALL;
		//pltAbstractObj->m_pfcInitVariable(pltObj);
		// 根据平台输入模式决定是否禁用某些功能按钮
		pltAbstractObj->m_pfcDisableFuncKeyHandler(pltObj);
		break;
	case GBIME_EVENT_LAYOUT_EXIT:
		Global_SetLayoutWinHeight(0); // 高度清零，解决Layout间切换由于高度一样没有刷新VK
		pltAbstractObj->m_pfcInitVariable(pltObj);
		break;
// 	case GBIME_EVENT_LAYOUT_SWITCH_OK:
// 		if (pltAbstractObj->bReuseInputStringFlag)
// 		{
// 			pltAbstractObj->m_pfcShowCandWin(pltObj, ButtonStatusUp, pEngineOutputInfo);
// 			GBEngine_SetInputString(Global_GetEnineInstance(), pltAbstractObj->backupEngineInputString);
// 			pltAbstractObj->m_refreshFlag = GBIME_LAYOUT_REFRESH_ALL;
// 			pltAbstractObj->bReuseInputStringFlag = GBFalse;
// 			pltAbstractObj->backupEngineInputString[0] = 0;
// 		}
// 		break;
	default:
		ret = GBIME_IGNORE;
		break;
	}
	
	return ret;
}

GBIMELOCAL GBIMEReturn CLayoutAbstract__HandleEvent(GBLPVOID pltObj, PGBIMEEvent pIMEEvent, PEngineOutputInfo pEngineOutputInfo)
{
	return GBIME_OK;
}

GBIMELOCAL GBIMEReturn CLayoutAbstract__SetConfig(void)
{

	return GBIME_OK;
}

GBIMELOCAL GBIMEReturn CLayoutAbstract__SetInputMethod(void)
{

	return GBIME_OK;
}


GBIMELOCAL GBIMEReturn CLayoutAbstract__Listenner(void)
{

	return GBIME_OK;
}

/*!
 *-brief Layout 对象是否和Layout ID匹配
 *-return 匹配返回1，不匹配返回-1
 */
GBIMELOCAL GBIMEReturn CLayoutAbstract__IsMatchLtObjAndLtID(GBLPVOID pltObj, GBUINT16 ltID)
{
	PCLayoutAbstract pltTempObj = pltObj;

	if (pltTempObj->m_ltData.pltcData->ltID == ltID)
	{
		return 1;
	}
	return 0;
}

/*!
 *-brief 是否需要保留Layout 对象
 */
GBIMELOCAL GBIMEReturn CLayoutAbstract__IsNeedLeave(GBLPVOID pltObj)
{
	PCLayoutAbstract pltTempObj = pltObj;
	//if (LayoutVKSign == pltTempObj->m_ltData.pltcData->ltType)
	{
		return 1;
	}
	//return 0;
}


/*!
 * \brief 改变Layout窗口高度, 需要通知平台输入框调整内容区的高度
 * \param pltObj 
 * \param newHeight 
 * \return 
 */
GBIMELOCAL GBIMEReturn CLayoutAbstract__ChangeHeight(GBLPVOID pltObj, GBINT newHeight)
{
	PCLayoutAbstract pCLayoutAbstract = (PCLayoutAbstract)pltObj;

	// 避免频繁调整高度降低刷新效率
 	if (Global_GetLayoutWinHeight() == newHeight)
 	{
 		return GBIME_OK;
 	}

	// 更新Layout窗口显示高度
	Global_SetLayoutWinHeight(newHeight);

	// 根据传入高度显示/隐藏国笔输入法框架虚拟键盘
	if (newHeight > 0)
	{
		GBInputBox_ShowVirtualKeyboard();
	} 
	else
	{
		GBInputBox_HideVirtualKeyboard();
	}

	return GBIME_OK;
}

/*!
 * \brief Layout上屏候选
 * \param pltObj 
 * \param pCand 
 * \return 
 */
GBIMELOCAL GBIMEReturn CLayoutAbstract__UpScreen(GBLPVOID pltObj, GBPCWCHAR pCand)
{
	GBInputBox_UpScreenString(pCand);	
	return GBIME_OK;
}

/*!
 * \brief Layout 通知编辑框重绘
 * \param pltObj 
 * \param pCand 
 * \return 
 */
GBIMELOCAL GBIMEReturn CLayoutAbstract__Redraw(GBLPVOID pltObj)
{
	GBInputBox_Redraw();	
	return GBIME_OK;
}

/*!
 * \brief 取得候选框图的高度[候选框也是一个layout]
 * \param layoutId 
 * \return 候选框图的高度
 * \autor huanjin
 * \data 2010-8-26
 */
GBIMELOCAL GBUINT16 CLayoutAbstract_GetLayoutHeight(GBLPVOID pltObj, LayoutIdEnum layoutId)
{
	GBLPCVOID pSkinData = Global_GetSkinData();
	pltObj = pltObj;

	return CLayoutInfo_GetLayoutHeight(pSkinData, (GBUINT16)layoutId);
}

/*!
 * \brief 取得候选框图的高度[候选框也是一个layout]
 * \param layoutId 
 * \return 候选框图的高度
 * \autor ZhaoKun
 * \data 2010-10-22
 */
GBIMELOCAL GBUINT16 CLayoutAbstract_GetLayoutWidth(GBLPVOID pltObj, LayoutIdEnum layoutId)
{
	GBLPCVOID pSkinData = Global_GetSkinData();
	pltObj = pltObj;

	return CLayoutInfo_GetLayoutWidth(pSkinData, (GBUINT16)layoutId);
}

/*!
 * \brief 根据按钮键值,取得按钮在图层中的坐标
 * \param layoutId 
 * \return 候选框图的高度
 * \autor huanjin
 * \data 2010-8-26
 */
GBIMELOCAL GBBOOL CLayoutAbstract_GetBottonRectByKey(GBLPVOID pltObj,LayoutIdEnum layoutId,
													 LayerIndex layerId, GBUINT16 iKeyValue, PGBRECT pRect)
{
	GBLPCVOID pSkinData = Global_GetSkinData();
	pltObj = pltObj;
	
	return CLayoutInfo_GetBottonRectByKey(pSkinData, (GBUINT16)layoutId,
		(GBUINT16)layerId, iKeyValue, pRect);
}

// 取得层的信息
GBIMELOCAL PLAYOUTCONSTLAYER CLayoutAbstract_GetLayerConstInfo(GBLPVOID pltObj, LayoutIdEnum layoutId, LayerIndex layerId)
{
	GBLPCVOID pSkinData = Global_GetSkinData();
	pltObj = pltObj;

	return CLayoutInfo_GetLayerConstInfo(pSkinData, (GBUINT16)layoutId, (GBUINT16)layerId);
}
// 取得按钮信息
GBIMELOCAL PBUTTONCONSTDATA CLayoutAbstract_GetLayerConstButtons(GBLPVOID pltObj, LayoutIdEnum layoutId, LayerIndex layerId)
{
	GBLPCVOID pSkinData = Global_GetSkinData();
	pltObj = pltObj;

	return CLayoutInfo_GetLayerConstButtons(pSkinData, (GBUINT16)layoutId, (GBUINT16)layerId);
}

/*!
 * \brief 切换Layout处理函数
 * \param pltObj 
 * \return 
 * \note 
 * \author haj
 * \date 2010-8-27
*/
GBIMELOCAL GBIMEReturn CLayoutAbstract_SwitchHandler(GBLPVOID pltObj)
{
	PCLayoutAbstract pltBase = (PCLayoutAbstract)pltObj;

	GBUINT16 targetLayoutID = 0;
	PCMAPPING_LAYOUTKEY2LAYOUTID pcMapCursor = &GBCMappingLayoutKey2ID[0];

	while (0 != pcMapCursor->layoutKey)
	{
		if (pcMapCursor->layoutKey == pltBase->currentLayoutKeyValue)
		{
			return Global_FillEventGroup(GBIME_CT_SWITCH, pcMapCursor->layoutID, 0);
		}

		pcMapCursor++;		
	} //while

	return GBIME_IGNORE;
}

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
GBIMELOCAL GBIMEReturn CLayoutAbstract_FuncKeyHandler(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo)
{
	PCLayoutAbstract pltBase = (PCLayoutAbstract)pltObj;
	GBUINT16 tempStr[2];

	pltBase->bUpToInputKeySequence = GBFalse;

	switch (pltBase->currentLayoutKeyValue)
	{
	case GBKEY_VK_SETTING: ///<设置
		Global_FillEventGroup(GBIME_CT_CONFIG_MENU, 0, 0);
		break;
	case GBKEY_VK_V9_PINYIN_SEPARATOR://拼音分隔符
		if (pEngineOutputInfo->bCandWindOpen) // 引擎处理过程中，输入分隔符
		{
			pltBase->bSendEngineKeyEvent = GBTrue;
			pltBase->engineKeyEvent = GBKEY_AMB_SPECIAL_A;
			Global_FillEventGroup(GBIME_CT_NORMAL_KEY_UP, 0, 0);
		}
		break;
	case GBKEY_VK_V9_SYMBOL_KEY: // MultiTap标点符号处理		
		pltBase->m_pfcMultiTapSymbolHandler(pltObj);
		Global_FillEventGroup(GBIME_CT_KEY_UP_REFRESH, 0, 0); // 按钮弹起刷新
		break;
	case GBKEY_VK_V9_SYMBOL_FACE_MULTIP_KEY: // MultiTap表情符号处理		
		pltBase->m_pfcMultiTapFaceHandler(pltObj);
		Global_FillEventGroup(GBIME_CT_KEY_UP_REFRESH, 0, 0); // 按钮弹起刷新
		break;
	case GBKEY_RETURN: // 无输入时，点击【回车】键，换行。输入时，点击【回车】键，数字上屏
		if (pEngineOutputInfo->bCandWindOpen)
		{
			// 数字上屏需要确保处于非选择状态
			GBEngine_ExitSelectedStatus(Global_GetEnineInstance());
			pltBase->bSendEngineKeyEvent = GBTrue;
			pltBase->engineKeyEvent = GBKEY_UP;
			pltBase->bUpToInputKeySequence = GBTrue;
			Global_FillEventGroup(GBIME_CT_NORMAL_KEY_UP, 0, 0);
		}
		else
		{
			tempStr[0] = GBKEY_RETURN;
			tempStr[1] = 0;
			pltBase->m_pfcUpScreen(pltObj, tempStr);
			Global_FillEventGroup(GBIME_CT_KEY_UP_REFRESH, 0, 0); // 按钮弹起刷新
		}
		break;
	case GBKEY_SPACE: // 无输入时，点击【空格】键，输入空格。输入时，点击【空格】键，候选首字上屏
		if (pEngineOutputInfo->bCandWindOpen)
		{
			pltBase->bSendEngineKeyEvent = GBTrue;
			pltBase->engineKeyEvent = GBEVENT_SELECT_CANDIDATE;
			pltBase->engineEventParam = pltBase->currentCandidateIndex;
			pltBase->bUpToInputKeySequence = GBTrue;
			Global_FillEventGroup(GBIME_CT_NORMAL_KEY_UP, 0, 0);
		}
		else
		{
			tempStr[0] = GBKEY_SPACE;
			tempStr[1] = 0;
			pltBase->m_pfcUpScreen(pltObj, tempStr);
			Global_FillEventGroup(GBIME_CT_KEY_UP_REFRESH, 0, 0); // 按钮弹起刷新
		}
		break;
	case GBKEY_BACK:
		if (pEngineOutputInfo->bCandWindOpen)
		{
			if (pEngineOutputInfo->isMultiTapInputMode)
			{
				pltBase->bSendEngineKeyEvent = GBTrue;
				pltBase->engineKeyEvent = GBKEY_OK;
				Global_FillEventGroup(GBIME_CT_NORMAL_KEY_UP, GBKEY_OK, 0);
			}
			else
			{
				Global_FillEventGroup(GBIME_CT_NORMAL_KEY_UP, GBKEY_BACK, 0);
			}
		}
		else
		{
			GBInputBox_DeleteChar();
			Global_FillEventGroup(GBIME_CT_KEY_UP_REFRESH, GBKEY_BACK, 0);
		}
		break;
	case GBKEY_VK_CAP: // 切换英文输入大小写
		if (GBEngine_IsAlpInputMode(Global_GetEnineInstance()))
		{
			pltBase->m_pfcSwitchShiftCapStatus(pltObj, 0, GBTrue, pEngineOutputInfo);	
		}
		break;
	case GBKEY_AMB_SPECIAL_A: // 输入‘*’
		tempStr[0] = '*';
		tempStr[1] = 0;
		pltBase->m_pfcUpScreen(pltObj, tempStr);
		Global_FillEventGroup(GBIME_CT_KEY_UP_REFRESH, 0, 0); // 按钮弹起刷新
		break;
	case GBKEY_AMB_SPECIAL_B: // 输入‘#’
		tempStr[0] = '#';
		tempStr[1] = 0;
		pltBase->m_pfcUpScreen(pltObj, tempStr);
		Global_FillEventGroup(GBIME_CT_KEY_UP_REFRESH, 0, 0); // 按钮弹起刷新
		break;
	default:
		return GBIME_IGNORE;
		break;
	}
	
	return GBIME_OK;
}


/*!
 * \brief 切换回前一个Layout
 * \param pltObj 
 * \return 
 * \note 
 * \author haj
 * \date 2010-8-27
*/
GBIMELOCAL GBIMEReturn CLayoutAbstract_SwitchPreLayoutObj(GBLPVOID pltObj)
{
	PCLayoutAbstract pltBase = (PCLayoutAbstract)pltObj;
	PCLayoutAbstract pltPrevObject = Global_GetPrevLayoutObject();

	if (pltPrevObject != NULL)
	{
		//return Global_FillEventGroup(GBIME_CT_SWITCH, LayoutIdSwitchPreObj, 0);
		return Global_FillPostEventGroup(GBIME_CT_SWITCH, LayoutIdSwitchPreObj, 0);
	}	

	return GBIME_LAYOUT_NOEXIST;
}
// 根据图层ID和层号，取得候选框高度(图层不存在或参数错误时，返回0)
GBIMELOCAL GBINT16 CLayoutAbstract_GetCandidateBoxHeight(GBLPVOID pltObj, LayoutIdEnum layoutId, LayerIndex layerId)
{
	GBLPCVOID pSkinData = Global_GetSkinData();
	pltObj = pltObj;
	return CLayoutInfo_GetCandidateBoxHeight(pSkinData,(GBUINT16)layoutId,(GBUINT16)layerId);
}

/*!
 * \brief 获取Layout窗口相对于屏幕的位置(Layout左上角)
 * \param pltObj 
 * \param pWinPos 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-26 15:14:32
*/
GBIMELOCAL void CLayoutAbstract_GetWinPos(GBLPVOID pltObj, PGBIMEPoint pWinPos)
{
	PCLayoutAbstract pltAbstractObj = (PCLayoutAbstract)pltObj;
	PLayoutInfo pLayoutInfo = (PLayoutInfo)(&pltAbstractObj->m_ltData); // Layout UI data point
			
	GBUINT16 iCandidateBoxHeight = 0;
	GBUINT16 iKeyBoardHeight = 0;
	GBUINT16 iCurrentLayoutId;
	GBUINT16 iCurrentLayerNumber;

	iCurrentLayoutId = GET_LAYOUT_ID(pltAbstractObj);
	iCurrentLayerNumber = GET_LAYOUT_CURRENT_LAYER(pltAbstractObj);
	iKeyBoardHeight = pltAbstractObj->m_pfcGetLayoutHeight(pltObj,iCurrentLayoutId);
	iCandidateBoxHeight = pltAbstractObj->m_pfcGetCandidateBoxHeight(pltObj, iCurrentLayoutId, iCurrentLayerNumber);
	
	Global_GetWinPos(pWinPos);
	switch (iCurrentLayerNumber)
	{
	case FirstLayer:
		pWinPos->y -=  iKeyBoardHeight;
		break;
	case SecondLayer:
		pWinPos->y -=  (iKeyBoardHeight + iCandidateBoxHeight);
		break;
	case ThirdLayer:
		pWinPos->y -=  (iKeyBoardHeight + iCandidateBoxHeight);
		break;
	default:
		pWinPos->y -=  iKeyBoardHeight;
		break;
	}
}

/*!
 * \brief 获取Layout窗口有效区域
 * \param pltObj 
 * \param pLayoutWinRect 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-27 10:00:31
*/
GBIMELOCAL void CLayoutAbstract_GetLayoutWinRect(GBLPVOID pltObj, PGBRECT pLayoutWinRect)
{
	GBIMEPoint layoutWinStartPos;
	PCLayoutAbstract pltAbstractObj = (PCLayoutAbstract)pltObj;	
	Global_GetWinPos(&layoutWinStartPos); // note: 左下角位置
	FILL_RECT(pLayoutWinRect, 
			  layoutWinStartPos.x, 
			  (GBINT16)(layoutWinStartPos.y - Global_GetLayoutWinHeight()), 
		      Global_GetLayoutWinWidth(), 
			  layoutWinStartPos.y);
}

/*!
 * \brief 判断是否是输入按键
 * \param keyValue 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-17 17:22:37
*/
GBIMELOCAL GBBOOL CLayoutAbstract_IsInputKey(GBUINT16 keyValue)
{
	// 引擎键值
	if ((keyValue > GBEvent_None) && (keyValue < GBKEY_VK_A_UPPER))
	{
		if (((keyValue >= GBKEY_0) && (keyValue <= GBKEY_9)) // 数字键
			||((keyValue >= GBKEY_A) && (keyValue <= GBKEY_Z))) // 字母键
		{
			return GBTrue;
		}
		
	}
	else // 虚拟键盘键值
	{
		if (((keyValue >= GBKEY_VK_A_UPPER)&&(keyValue <= GBKEY_VK_10_NUMBERKEY)))
		{
			return GBTrue;
		}		
	}

	return GBFalse;
}

/*!
 * \brief 滑屏触摸笔按下消息处理函数
 * \param pltObj 
 * \param pt 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-24 17:19:49
*/
GBIMELOCAL GBIMEReturn CLayoutAbstract_SlidingPenDownHandler(GBLPVOID pltObj, GBIMEPoint pt)
{
#if GBIME_CFG_SLIDING_SWITCH_SUPPORT > 0
	PCLayoutAbstract pltAbstractObj = (PCLayoutAbstract)pltObj;
	GBIMEPoint curPt;
	GBRECT layoutWinRect;

	curPt.x = pt.x;
	curPt.y = pt.y;
	CLayoutAbstract_GetLayoutWinRect(pltObj, &layoutWinRect);

	if (pltAbstractObj->isSlidingDisabled || !PT_PointIsInRect(curPt, layoutWinRect))
	{
		return GBIME_IGNORE;
	}
	
	// 按下清除滑动标记并启动计时器、记录起始位置、起始时间
	memset(&pltAbstractObj->slidingOperation, 0, sizeof(SlidingOperation));
	pltAbstractObj->slidingOperation.startPos.x = curPt.x;
	pltAbstractObj->slidingOperation.startPos.y = curPt.y;
	pltAbstractObj->slidingOperation.lastMovePos.x = curPt.x;
	pltAbstractObj->slidingOperation.lastMovePos.y = curPt.y;
	pltAbstractObj->slidingOperation.slidingStartTime = PT_GetCurrentTime();
	pltAbstractObj->slidingOperation.slidingElapsedTime = 0;
	pltAbstractObj->slidingOperation.slidingStatus = GBIME_SLIDING_INIT; // 初始状态
#endif
	return GBIME_OK;
}

/*!
 * \brief 滑屏触摸笔弹起消息处理函数[清除滑动操作变量]
 * \param pltObj 
 * \param pt 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-24 17:19:42
*/
GBIMELOCAL GBIMEReturn CLayoutAbstract_SlidingPenUpHandler(GBLPVOID pltObj, GBIMEPoint pt)
{
#if GBIME_CFG_SLIDING_SWITCH_SUPPORT > 0
	PCLayoutAbstract pltAbstractObj = (PCLayoutAbstract)pltObj;	
	PLayoutInfo pLayoutInfo = (PLayoutInfo)(&pltAbstractObj->m_ltData); // Layout UI data point

	if (pltAbstractObj->isSlidingDisabled)
	{
		return GBIME_IGNORE;
	}

	// 满足滑屏操作条件实现滑屏切换Layout
	if (pltAbstractObj->slidingOperation.slidingStatus == GBIME_SLIDING_EXCUTE_ACTION)
	{
		GBUINT16 targetLayoutID = CLayoutInfo_GetSlidingSwitchLayoutID(pLayoutInfo, pltAbstractObj->slidingOperation.slidingDirection);
		if (LayoutIdHide == targetLayoutID)
		{
			Global_FillEventGroup(GBIME_CT_HIDE, 0, 0);
		}
		else
		{
			Global_FillEventGroup(GBIME_CT_SWITCH, targetLayoutID, 0);
		}		
		// 结束滑屏操作清除状态量
		memset(&pltAbstractObj->slidingOperation, 0, sizeof(SlidingOperation));
		return GBIME_OK;
	}
	memset(&pltAbstractObj->slidingOperation, 0, sizeof(SlidingOperation));
#endif
	return GBIME_IGNORE;
}

/*!
 * \brief 滑屏触摸笔移动消息处理函数
 * \param pltObj 
 * \param pt 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-24 17:19:31
*/
GBIMELOCAL GBIMEReturn CLayoutAbstract_SlidingPenMoveHandler(GBLPVOID pltObj, GBIMEPoint pt)
{
#if GBIME_CFG_SLIDING_SWITCH_SUPPORT > 0
	PCLayoutAbstract pltAbstractObj = (PCLayoutAbstract)pltObj;
	PLayoutInfo pLayoutInfo = (PLayoutInfo)(&pltAbstractObj->m_ltData); // Layout UI data point
	GBIMEPoint curPt;
	GBINT16 distanceH = 0;
	GBINT16 distanceV = 0;
	GBU32 tempTime = 0;
	GBRECT layoutWinRect;
	GBSlidingDirection curSlidingDirection;

	curPt.x = pt.x;
	curPt.y = pt.y;
	CLayoutAbstract_GetLayoutWinRect(pltObj, &layoutWinRect);
	
	if (pltAbstractObj->isSlidingDisabled)
	{
		return GBIME_IGNORE;
	}

	if (pltAbstractObj->slidingOperation.slidingStatus == GBIME_SLIDING_EXCUTE_ACTION)
	{
		return GBIME_IGNORE;
	}

	// 划出Layout窗口有效区域重置状态
	if (!PT_PointIsInRect(curPt, layoutWinRect))
	{
		memset(&pltAbstractObj->slidingOperation, 0, sizeof(SlidingOperation));
		return GBIME_IGNORE;
	}

	// 计算当前横向、纵向移动距离
	distanceH = curPt.x - pltAbstractObj->slidingOperation.lastMovePos.x;
	distanceV = curPt.y - pltAbstractObj->slidingOperation.lastMovePos.y;
	
	// 记录上一个Move点坐标
	pltAbstractObj->slidingOperation.lastMovePos.x = curPt.x;
	pltAbstractObj->slidingOperation.lastMovePos.y = curPt.y;

	// 防止频繁处理，提高响应速度
	if (ABS(distanceH) < GBIME_SLIDING_MINIMUM_PIXELS 
		&& ABS(distanceV) < GBIME_SLIDING_MINIMUM_PIXELS)
	{
		return GBIME_IGNORE;
	}

	// 判断方向
	if (distanceV < 0 && ABS(distanceH) <= ABS(distanceV))
	{
		curSlidingDirection = SLIDING_DIRECTION_UP;
	}
	else if (distanceV > 0 && ABS(distanceH) <= ABS(distanceV))
	{
		curSlidingDirection = SLIDING_DIRECTION_DOWN;
	}
	else if (distanceH < 0 && ABS(distanceH) > ABS(distanceV))
	{
		curSlidingDirection = SLIDING_DIRECTION_LEFT;
	}
	else if (distanceH > 0 && ABS(distanceH) > ABS(distanceV))
	{
		curSlidingDirection = SLIDING_DIRECTION_RIGHT;
	}

	// 根据滑动状态做处理
	switch (pltAbstractObj->slidingOperation.slidingStatus)
	{
	case GBIME_SLIDING_INIT: // 初始状态
		pltAbstractObj->slidingOperation.slidingStatus = GBIME_SLIDING_MOVING;
		pltAbstractObj->slidingOperation.slidingDirection = curSlidingDirection;
		break;
	case GBIME_SLIDING_MOVING: // 移动状态
		// 滑动方向发生变化，重置状态
		if (curSlidingDirection != pltAbstractObj->slidingOperation.slidingDirection)
		{
			pltAbstractObj->slidingOperation.slidingDirection = curSlidingDirection;
			pltAbstractObj->slidingOperation.startPos.x = curPt.x;
			pltAbstractObj->slidingOperation.startPos.y = curPt.y;
			pltAbstractObj->slidingOperation.lastMovePos.x = curPt.x;
			pltAbstractObj->slidingOperation.lastMovePos.y = curPt.y;
			pltAbstractObj->slidingOperation.slidingStartTime = PT_GetCurrentTime();
			pltAbstractObj->slidingOperation.slidingElapsedTime = 0;
			pltAbstractObj->slidingOperation.slidingSpeed = 0;
			pltAbstractObj->slidingOperation.slidingDistance = 0;
		}
		else
		{
			// 更新滑动时间
			tempTime = PT_GetCurrentTime();
			pltAbstractObj->slidingOperation.slidingElapsedTime = tempTime - pltAbstractObj->slidingOperation.slidingStartTime;
			
			// 更新距离、速度
			distanceH = ABS(curPt.x - pltAbstractObj->slidingOperation.startPos.x);
			distanceV = ABS(curPt.y - pltAbstractObj->slidingOperation.startPos.y);
			//pltAbstractObj->slidingOperation.slidingDistance = (GBINT)(ABS(sqrt((distanceH * distanceH) + (distanceV * distanceV))));
			pltAbstractObj->slidingOperation.slidingDistance = PT_GetPointDistance(distanceH, distanceV);
			if (pltAbstractObj->slidingOperation.slidingElapsedTime > 0)
			{
				pltAbstractObj->slidingOperation.slidingSpeed = pltAbstractObj->slidingOperation.slidingDistance * 1000
					/ pltAbstractObj->slidingOperation.slidingElapsedTime;
			}
			
			// 满足滑屏要求，实现Layout滑屏切换
			if (pltAbstractObj->slidingOperation.slidingSpeed >= GBIME_SLIDING_ACTION_SPEED_REQUIRED
				&& pltAbstractObj->slidingOperation.slidingDistance >= GBIME_SLIDING_ACTION_DISTANCE_REQUIRED)
			{
				pltAbstractObj->slidingOperation.slidingStatus = GBIME_SLIDING_EXCUTE_ACTION; // 执行滑屏操作状态
			}
		}
		break;
	default:
		break;
	}

// 	PT_PrintLogInfo(GBLOG_LAYOUTINFO, ("########### 滑动速度[%d], 滑动距离[%d] ########\n", 
// 					pltAbstractObj->slidingOperation.slidingSpeed, 
// 					pltAbstractObj->slidingOperation.slidingDistance));
#endif
	return GBIME_OK;
}

/*!
 * \brief 长按键处理函数(处理长按删除)
 * \param pltObj 
 * \param pEngineOutputInfo 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-12-17 11:20:44
 */
GBIMELOCAL GBIMEReturn CLayoutAbstract_LongPressKeyHandler(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo)
{
	PCLayoutAbstract pltAbstractObj = (PCLayoutAbstract)pltObj;

	switch (pltAbstractObj->currentLayoutKeyValue)
	{
	case GBKEY_BACK:
		if (pltAbstractObj->buttonStatus == ButtonStatusDown)
		{
			PT_GUIStartTimer(GBIME_LONG_PRESS_TIME, CLayoutAbstract_DeleteAll);		
		} 
		else if (pltAbstractObj->buttonStatus == ButtonStatusUp) // 按键弹起取消回调
		{
			PT_GUICancelTimer(CLayoutAbstract_DeleteAll);
			if (pltAbstractObj->bLongPressFlag)
			{
				pltAbstractObj->bLongPressFlag = GBFalse;
				Global_FillEventGroup(GBIME_CT_KEY_UP_REFRESH, 0, 0); // 按钮弹起刷新
				return GBIME_OK; // 结束处理，不继续跑短按GBKEY_BACK的功能处理代码
			}
		}
		break;
	default:
		return GBIME_IGNORE;
		break;
	}

	return GBIME_IGNORE;
}

/*!
 * \brief 长按删除处理函数
 * \param void 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-10-9 9:49:00
*/
GBIMELOCAL void CLayoutAbstract_DeleteAll(void)
{
	//PCLayoutControl pLtCtl = Global_GetLayoutCtl();
	PEngineOutputInfo pEngineOutputInfo = CLayoutControl_GetEngineOutputInfo();
	PCLayoutAbstract pltAbstractObj = (PCLayoutAbstract)Global_GetCurrentLayoutObject();

	// 清空引擎输入串内容
	if (pEngineOutputInfo->bCandWindOpen/* && !pEngineOutputInfo->bInputEmpty*/)
	{		
		GBEngine_Reset(Global_GetEnineInstance());
		CLayoutControl_UpdateOutputInfo();
		pltAbstractObj->m_pfcCloseCandWin(pltAbstractObj, pEngineOutputInfo);
	}
	else // 清空编辑器输入框内容
	{
		GBInputBox_DeleteAll();
	}

	pltAbstractObj->bLongPressFlag = GBTrue;
	
	// 重置功能按键及引擎状态并关闭候选框
	//GBResetFuncKey();
}

/*!
 * \brief 显示候选框(进入第二层输入状态)
 * \param pltObj 
 * \param buttonStatus 
 * \param pEngineOutputInfo 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-10-19 9:56:58
*/
GBIMELOCAL GBIMEReturn CLayoutAbstract_ShowCandWin(GBLPVOID pltObj, ButtonStatusEnum buttonStatus, PEngineOutputInfo pEngineOutputInfo)
{
	PCLayoutAbstract pltAbstractObj = (PCLayoutAbstract)pltObj;
	GBUINT16 iCandidateBoxHeight = 0;
	GBUINT16 iKeyBoardHeight = 0;
	GBUINT16 iCurrentLayoutId;
	
	iCurrentLayoutId = GET_LAYOUT_ID(pltAbstractObj);
	iKeyBoardHeight = pltAbstractObj->m_pfcGetLayoutHeight(pltObj,iCurrentLayoutId);

	if (buttonStatus == ButtonStatusUp)
	{
		if (pltAbstractObj->m_pfcIsInputKey(pltAbstractObj->currentLayoutKeyValue))
		{
			pltAbstractObj->currentSyllableIndex = 0; // 清零音节索引
			pltAbstractObj->currentCandidateIndex = 0; // 清零候选索引
			pltAbstractObj->m_ltData.ltCurLayer = SecondLayer;
			iCandidateBoxHeight = pltAbstractObj->m_pfcGetCandidateBoxHeight(pltObj, GET_LAYOUT_ID(pltAbstractObj), SecondLayer);
			pltAbstractObj->m_pfcChangeHeight(pltObj, iKeyBoardHeight+iCandidateBoxHeight);
			return GBIME_OK;
		}
	}

	return GBIME_IGNORE;
}

/*!
 * \brief 显示联想框(进入第三层联想状态)
 * \param pltObj 
 * \param buttonStatus 
 * \param pEngineOutputInfo 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-10-19 9:56:58
*/
GBIMELOCAL GBIMEReturn CLayoutAbstract_ShowAssociateWin(GBLPVOID pltObj, ButtonStatusEnum buttonStatus, PEngineOutputInfo pEngineOutputInfo)
{
	PCLayoutAbstract pltAbstractObj = (PCLayoutAbstract)pltObj;
	GBUINT16 iCandidateBoxHeight = 0;
	GBUINT16 iKeyBoardHeight = 0;
	GBUINT16 iCurrentLayoutId;
	
	iCurrentLayoutId = GET_LAYOUT_ID(pltAbstractObj);
	iKeyBoardHeight = pltAbstractObj->m_pfcGetLayoutHeight(pltObj,iCurrentLayoutId);
	iCandidateBoxHeight = pltAbstractObj->m_pfcGetCandidateBoxHeight(pltObj, iCurrentLayoutId, ThirdLayer);
	
	pltAbstractObj->currentSyllableIndex = 0; // 清零音节索引
	pltAbstractObj->m_ltData.ltCurLayer = ThirdLayer;	
	pltAbstractObj->m_pfcChangeHeight(pltObj, iKeyBoardHeight + iCandidateBoxHeight);
	return GBIME_OK;
}

/*!
 * \brief 根据引擎候选框状态决定是否关闭Layout面板候选栏
 * \param pltObj 
 * \param pEngineOutputInfo 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-10-19 10:12:16
*/
GBIMELOCAL GBIMEReturn CLayoutAbstract_CloseCandWin(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo)
{
	PCLayoutAbstract pltAbstractObj = (PCLayoutAbstract)pltObj;
	GBUINT16 iKeyBoardHeight = pltAbstractObj->m_pfcGetLayoutHeight(pltObj, GET_LAYOUT_ID(pltAbstractObj));

	if (!pEngineOutputInfo->bCandWindOpen) // 输入串为空时
	{
		pltAbstractObj->currentSyllableIndex = 0; // 清零音节索引
		pltAbstractObj->m_ltData.ltCurLayer = FirstLayer;
		pltAbstractObj->m_pfcChangeHeight(pltObj, iKeyBoardHeight);
		return GBIME_OK;
	}

	return GBIME_IGNORE;
}

/*!
 * \brief 检测是否需要上屏候选字
 * \param pltObj 
 * \param buttonStatus 
 * \param pEngineOutputInfo 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-10-19 10:15:15
*/
GBIMELOCAL GBIMEReturn CLayoutAbstract_CheckUpScreen(GBLPVOID pltObj, ButtonStatusEnum buttonStatus, PEngineOutputInfo pEngineOutputInfo)
{
	PCLayoutAbstract pltAbstractObj = (PCLayoutAbstract)pltObj;

	if ((buttonStatus == ButtonStatusUp 
		 && pltAbstractObj->penInPostion == PenPositionInCandidate
		 && pltAbstractObj->penDownPostion == PenPositionInCandidate)
	    || pltAbstractObj->bUpToInputKeySequence) // 数字上屏
	{
		pltAbstractObj->currentCandidateIndex = 0;
		pltAbstractObj->m_pfcUpScreen(pltObj, pEngineOutputInfo->pUpScreenStr);
		return GBIME_OK;
	}

	return GBIME_IGNORE;
}

/*!
 * \brief MultiTap符号处理函数
 * \param pltObj 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-10-20 16:43:20
*/
GBIMELOCAL GBIMEReturn CLayoutAbstract_MultiTapSymbolHandler(GBLPVOID pltObj)
{
	PCLayoutAbstract pltAbstractObj = (PCLayoutAbstract)pltObj;
	GBWCHAR *pSymbolArray;
	GBUINT16 symbolNum = 0;
	GBWCHAR hlightSymbolString[2];

	pSymbolArray = CLayoutInfo_GetBottonKeyName(Global_GetSkinData(), GET_LAYOUT_ID(pltAbstractObj), 
							GET_LAYOUT_CURRENT_LAYER(pltAbstractObj), pltAbstractObj->currentLayoutKeyValue);

	if (pSymbolArray == NULL || wcslen(pSymbolArray) == 0)
	{
		return GBIME_FAILED;
	}

	symbolNum = wcslen(pSymbolArray);
	if (pltAbstractObj->multitapCandIndex == symbolNum)
	{
		pltAbstractObj->multitapCandIndex = 0;
	}
	hlightSymbolString[0] = pSymbolArray[pltAbstractObj->multitapCandIndex];
	hlightSymbolString[1] = 0;
	pltAbstractObj->multitapReplaceCandLength = GBInputBox_ReplaceHighlightString(pltAbstractObj->multitapReplaceCandLength, (GBPCWCHAR)hlightSymbolString);
	PT_GUIStartTimer(GBIME_MULTITAP_TIMER_INTERVAL, pltAbstractObj->m_pfcOnMultitapTimer);
	pltAbstractObj->bMultitapTimerStartFlag = GBTrue;
	pltAbstractObj->bMultitapSymbolStartFlag = GBTrue;
	pltAbstractObj->multitapCandIndex++;

	return GBIME_OK;
}

/*!
 * \brief MultiTap表情符号处理函数
 * \param pltObj 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-10-20 16:43:12
*/
GBIMELOCAL GBIMEReturn CLayoutAbstract_MultiTapFaceHandler(GBLPVOID pltObj)
{
	PCLayoutAbstract pltAbstractObj = (PCLayoutAbstract)pltObj;
	GBWCHAR *pFaceString;
	GBWCHAR *pFaceStart;
	GBINT faceStrLen = 0;
	GBINT faceNum = 0;
	GBWCHAR faceArray[GBIME_MULTITAP_FACE_NUM][GBIME_MULTITAP_FACE_LEN + 1];
	GBUINT16 symbolNum = 0;
	GBINT i =  0;

	pFaceString = CLayoutInfo_GetBottonKeyName(Global_GetSkinData(), GET_LAYOUT_ID(pltAbstractObj), 
							GET_LAYOUT_CURRENT_LAYER(pltAbstractObj), pltAbstractObj->currentLayoutKeyValue);

	// 非法表情符号串，检查皮肤工具填写的表情按键名称
	if (pFaceString == NULL || wcslen(pFaceString) == 0)
	{
		return GBIME_FAILED;
	}

	// 拆分表情符号
	pFaceStart = pFaceString;
	faceStrLen = wcslen(pFaceString);
	for (i = 0; i < faceStrLen; i++)
	{
		if (pFaceString[i] == GBIME_MULTITAP_FACE_SPLIT_CHAR)
		{
			GBINT curFaceLen = (GBINT)(pFaceString + i - pFaceStart);
			wcsncpy(faceArray[faceNum], pFaceStart, curFaceLen);
			faceArray[faceNum][curFaceLen] = 0; // 结束符
			pFaceStart = pFaceString + i + 1;
			faceNum++;
		}
	}

	// 非法表情符号串，检查皮肤工具填写的表情按键名称
	if (faceNum == 0)
	{
		return GBIME_FAILED;
	}

	if (pltAbstractObj->multitapCandIndex == faceNum)
	{
		pltAbstractObj->multitapCandIndex = 0;
	}

	pltAbstractObj->multitapReplaceCandLength = GBInputBox_ReplaceHighlightString(pltAbstractObj->multitapReplaceCandLength, 
															(GBPCWCHAR)faceArray[pltAbstractObj->multitapCandIndex]);
	PT_GUIStartTimer(GBIME_MULTITAP_TIMER_INTERVAL, pltAbstractObj->m_pfcOnMultitapTimer);
	pltAbstractObj->bMultitapTimerStartFlag = GBTrue;
	pltAbstractObj->bMultitapFaceStartFlag = GBTrue;
	pltAbstractObj->multitapCandIndex++;
	return GBIME_OK;
}

/*!
 * \brief MultiTap符号功能定时器间隔到达时的响应函数
 * \param void 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-10-20 11:32:42
*/
static void CLayoutAbstract_OnMultitapTimer(void)
{
	PCLayoutAbstract pltAbstractObj = (PCLayoutAbstract)Global_GetCurrentLayoutObject();

	if (pltAbstractObj->bMultitapTimerStartFlag)
	{
		pltAbstractObj->bMultitapTimerStartFlag = GBFalse;
		pltAbstractObj->bMultitapSymbolStartFlag = GBFalse;
		pltAbstractObj->bMultitapFaceStartFlag = GBFalse;
		pltAbstractObj->multitapReplaceCandLength = 0;
		pltAbstractObj->multitapCandIndex = 0;
		PT_GUICancelTimer(pltAbstractObj->m_pfcOnMultitapTimer);
		GBInputBox_MultitapInputConfirm();
	}
}

/*!
 * \brief shiftCap状态切换(同步切换GBV5引擎shiftCap状态)
 * \param pltObj 
 * \param shiftCapStatus 切换到指定状态
 * \param bAuto 自动循环切换
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-10-21 11:29:08
*/
GBIMELOCAL GBIMEReturn CLayoutAbstract_SwitchShiftCapStatus(GBLPVOID pltObj, 
															GBShiftCapStatus shiftCapStatus, 
															GBBOOL bAuto,
															PEngineOutputInfo pEngineOutputInfo)
{
	PCLayoutAbstract pltAbstractObj = (PCLayoutAbstract)Global_GetCurrentLayoutObject();
	GBUINT16 iCurrentLayoutId = GET_LAYOUT_ID(pltAbstractObj);
	GBShiftCapStatus curShiftCapStatus = Global_GetShiftCapStatus();

	if (!GBEngine_IsAlpInputMode(Global_GetEnineInstance()))
	{
		return GBIME_IGNORE;
	}

	if (bAuto)
	{
		switch (curShiftCapStatus)
		{
		case GBStatus_Normal:
			Global_SetShiftCapStatus(GBStatus_Shift);
			Global_FillEventGroup(GBIME_CT_SWITCH, CLayoutInfo_GetUpperLayoutID(iCurrentLayoutId), 0);
			break;
		case GBStatus_Shift:
			Global_SetShiftCapStatus(GBStatus_Caplock);
			Global_FillEventGroup(GBIME_CT_KEY_UP_REFRESH, 0, 0); // 按钮弹起刷新
			break;
		case GBStatus_Caplock:
			Global_SetShiftCapStatus(GBStatus_Normal);		
			Global_FillEventGroup(GBIME_CT_SWITCH, CLayoutInfo_GetLowerLayoutID(iCurrentLayoutId), 0);
			break;
		default:
			break;
		}
		
		// 如果是输入状态，记录当前输入串，等切换到目标Layout再重新设置输入串并显示为指定的大小写状态
// 		if (pEngineOutputInfo->bCandWindOpen 
// 			&& pEngineOutputInfo->pInputString != NULL
// 			&& wcslen(pEngineOutputInfo->pInputString) > 0)
// 		{
// 			wcsncpy(pltAbstractObj->backupEngineInputString, pEngineOutputInfo->pInputString, GB_INPUT_BUFFER_LEN);///<备份引擎输入序列
// 			pltAbstractObj->bReuseInputStringFlag = GBTrue;
// 		}
	}
	else // 指定切换到目标状态及相应的Layout[首字母大写上屏后进入后处理切换回小写Layout]
	{
		Global_SetShiftCapStatus(shiftCapStatus);
		switch (shiftCapStatus)
		{
		case GBStatus_Normal:		
			Global_FillPostEventGroup(GBIME_CT_SWITCH, CLayoutInfo_GetLowerLayoutID(iCurrentLayoutId), 0);
			break;
		case GBStatus_Shift:
		case GBStatus_Caplock:
			Global_FillPostEventGroup(GBIME_CT_SWITCH, CLayoutInfo_GetUpperLayoutID(iCurrentLayoutId), 0);
			break;
		default:
			break;
		}
	}
	

	return GBIME_OK;
}

/*!
 * \brief 同步引擎shiftCap状态
 * \param pltObj 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-10-22 12:52:49
*/
GBIMELOCAL GBIMEReturn CLayoutAbstract_SyncEngineShiftCapStatus(GBLPVOID pltObj)
{
	PCLayoutAbstract pltAbstractObj = (PCLayoutAbstract)pltObj;
	GBINT bShiftDown;
	GBINT bCapsLockToggle;
	GBShiftCapStatus curShiftCapStatus = Global_GetShiftCapStatus();

	if (GBEngine_IsAlpInputMode(Global_GetEnineInstance()))
	{
		bShiftDown = (curShiftCapStatus == GBStatus_Shift) ? GBTrue : GBFalse;
		bCapsLockToggle = (curShiftCapStatus == GBStatus_Caplock) ? GBTrue : GBFalse;
		GBEngine_SetShiftCap(Global_GetEnineInstance(), bShiftDown, bCapsLockToggle);
		return GBIME_OK;
	}

	return GBIME_IGNORE;	
}

/*!
 * \brief 获取Layout起点坐标
 * \param pltObj 
 * \return 
 * \note 
 * \example
 * \author Zhaokun
 * \date 2010-10-23 11:46:08
*/
GBIMELOCAL void CLayoutAbstract_GetLayoutStartPoint(GBLPVOID pltObj, PGBIMEPoint pStartPoint)
{
	PCLayoutAbstract pltAbstractObj = (PCLayoutAbstract)pltObj;
	GBUINT16 iKeyBoardHeight = 0;
	GBUINT16 iKeyBoardWidth = 0;
	GBIMEPoint winPos;
	GBUINT16 iCurrentLayoutId;
	GBUINT16 iCandidateBoxHeight;
	GBINT16 iScreenWidth = 0;

	if(!pStartPoint)
	{
		return;
	}

	// 当前LayoutId
	iCurrentLayoutId = GET_LAYOUT_ID(pltAbstractObj);
	// 获取键盘高度
	iKeyBoardHeight = pltAbstractObj->m_pfcGetLayoutHeight(pltObj,iCurrentLayoutId);
	iKeyBoardWidth = pltAbstractObj->m_pfcGetLayoutWidth(pltObj, iCurrentLayoutId);
	// 取得键盘显示的开始位置【左下角】，并调整为【左上角】
	Global_GetWinPos(&winPos);	
	winPos.y -=  iKeyBoardHeight;
	
	iScreenWidth = PT_GetLcdWidth();
	
	// 取候选框高度
	iCandidateBoxHeight = pltAbstractObj->m_pfcGetCandidateBoxHeight(pltObj,iCurrentLayoutId,FirstLayer);
	// 相减得到候选框左上角Y坐标
	winPos.y -= iCandidateBoxHeight;

	// 显示图片的左上角X坐标
	pStartPoint->x = (GBUINT16)winPos.x + (iScreenWidth - iKeyBoardWidth) / 2;
	// 显示图片的左上角Y坐标
	pStartPoint->y = (GBUINT16)winPos.y;
}

/*!
 * \brief 设置音节候选起始显示位置
 * \param pltObj 
 * \param x 
 * \param y 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-12-17 18:17:16
 */
GBIMELOCAL void CLayoutAbstract_SetSyllableDisplayStartPos(GBLPVOID pltObj, GBINT16 x, GBINT16 y)
{
	PCLayoutAbstract pltAbstractObj = (PCLayoutAbstract)pltObj;
	pltAbstractObj->syllableDisplayStartPos.x = x;
	pltAbstractObj->syllableDisplayStartPos.y = y;
}

/*!
 * \brief 设置候选字起始显示位置
 * \param pltObj 
 * \param x 
 * \param y 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-12-17 18:17:29
 */
GBIMELOCAL void CLayoutAbstract_SetCandidateDisplayStartPos(GBLPVOID pltObj, GBINT16 x, GBINT16 y)
{
	PCLayoutAbstract pltAbstractObj = (PCLayoutAbstract)pltObj;
	pltAbstractObj->candidateDisplayStartPos.x = x;
	pltAbstractObj->candidateDisplayStartPos.y = y;
}

/*!
 * \brief 物理键盘ABC MultiTap定时器间隔到达时的响应函数
 * \param void 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-10-20 11:32:42
*/
static void CLayoutAbstract_KBMultiTapTimerCallBack(void)
{
	PCLayoutAbstract pltAbstractObj = (PCLayoutAbstract)Global_GetCurrentLayoutObject();
	PEngineOutputInfo pEngineOutputInfo = CLayoutControl_GetEngineOutputInfo();
	if (pltAbstractObj->bMultitapTimerStartFlag)
	{
		GBInputBox_MultitapInputConfirm();
		GBEngine_SendKeyEvent(Global_GetEnineInstance(), GBKEY_OK, 0);
		CLayoutControl_UpdateOutputInfo();
		pltAbstractObj->bMultitapTimerStartFlag = GBFalse;
		pltAbstractObj->multitapReplaceCandLength = 0;

		if (!pEngineOutputInfo->bCandWindOpen)
		{
			pltAbstractObj->currentSyllableIndex = 0; // 清零音节索引
			Global_SetLayoutWinActiveStatus(GBFalse);
			GBInputBox_SetAllPlatformFuncKey();
			pltAbstractObj->m_pfcChangeHeight(pltAbstractObj, 0);		
			return;
		}
	}
}

//////////////////////////////////////////////////////////////////////////
///>start [static const data]
// 中文符号【 "。 ？ ！ ， ： ……】
static const GBWCHAR symbols_chn[][GBIME_SYMBOL_MAX_LEN] = 
{
	{ 0xFF1F, 0x0000 }, // ？
	{ 0xFF01, 0x0000 }, // ！
	{ 0xFF0C, 0x0000 }, // ，
	{ 0x3002, 0x0000 }, // 。
	{ 0xFF1A, 0x0000 }, // ：
	{ 0x2026, 0x2026, 0x0000 }, // ……

	{ 0x3001,0x0000 }, //、
	{ 0xff1b,0x0000 }, //；
	{ 0x2014,0x0000 }, //—
	{ 0x201c,0x201d,0x0000 }, //“”
	{ 0xff08,0xff09,0x0000 }, //（）
};
const GBWCHAR * symbolsTable_1_Chn[] = 
{
	symbols_chn[0],	symbols_chn[1],	symbols_chn[2],	symbols_chn[3],	symbols_chn[4],	symbols_chn[5],
	symbols_chn[6],	symbols_chn[7],	symbols_chn[8],	symbols_chn[9],	symbols_chn[10]
};

// 英文符号【,.?!':…】
const GBWCHAR symbols_alp[][GBIME_SYMBOL_MAX_LEN] = 
{
	{ 0x003F, 0x0000 }, // ?
	{ 0x0021, 0x0000 }, // !
	{ 0x002C, 0x0000 }, // ,
	{ 0x002E, 0x0000 }, // .
	{ 0x0027, 0x0000 }, // '
	{ 0x0040,0x0000 }, //@

	{ 0x2026, 0x0000 }, // …
	{ 0x003A, 0x0000 }, // :
	{ 0x003b,0x0000 }, //;
	{ 0x002d,0x0000 }, //-
	{ 0x005f,0x0000 }, //_
	{ 0x002f,0x0000 }, ///
};
const GBWCHAR * symbolsTable_1_Alp[] = 
{
	symbols_alp[0],	symbols_alp[1],	symbols_alp[2],	symbols_alp[3],	symbols_alp[4],		symbols_alp[5], 
	symbols_alp[6],	symbols_alp[7],	symbols_alp[8],	symbols_alp[9],	symbols_alp[10],	symbols_alp[11]
};

// 表情符号【:-)  ;-)  :-D :-p  -.-||】
const GBWCHAR faces[][GBIME_FACE_MAX_LEN] =
{
	{ 0x003A, 0x002D, 0x0029, 0x0000 } // :-) 开心
	,{ 0x003a,0x002d,0x0028,0x0000} //:-(
	,{ 0x003A, 0x002D, 0x0044, 0x0000 } // :-D 开心
	,{ 0x003A, 0x002D, 0x0070, 0x0000 } // :-p
	,{ 0x002D, 0x002E, 0x002D, 0x007C, 0x007C, 0x0000 } // -.-||

	,{ 0x004f,0x0072,0x007a,0x0000 } //Orz
	,{ 0x003a,0x002d,0x0078,0x0000 } //:-x
	,{ 0x003a,0x002d,0x004f,0x0000 } //:-O
	,{ 0x005e,0x005f,0x005e,0x0000 } //^_^
	,{ 0x0054,0x005f,0x0054,0x0000 } //T_T
};
const GBWCHAR * facesTable_0[] =
{
	faces[0],	faces[1],	faces[2], faces[3],	faces[4],
	faces[5],	faces[6],	faces[7], faces[8],	faces[9]
};
///>end [static const data]
//////////////////////////////////////////////////////////////////////////
GBIMELOCAL GBUINT16 ICLayoutAbstract_GetStaticArraySize_symbolsTable_1_Alp(void)
{
	return PT_ARRAY_SIZE(symbolsTable_1_Alp);
}
GBIMELOCAL GBUINT16 ICLayoutAbstract_GetStaticArraySize_symbolsTable_1_Chn(void)
{
	return PT_ARRAY_SIZE(symbolsTable_1_Chn);
}
GBIMELOCAL GBUINT16 ICLayoutAbstract_GetStaticArraySize_facesTable(void)
{
	return PT_ARRAY_SIZE(facesTable_0);
}
/*!
 * \brief 重置标点符号键
 * \param pltObj 
 * \return 
 * \note 
 * \example
 * \author Zhaokun
 * \date 2010-11-24 10:00:14
*/
static void ICLayoutAbstract_ResetInterpunctionKey(GBLPVOID pltObj)
{
	GBEngine_UnregisterInterpunctionKey(Global_GetEnineInstance(), GBKEY_0);
	GBEngine_UnregisterInterpunctionKey(Global_GetEnineInstance(), GBKEY_1);
}

/*!
 * \brief 设置标点符号按键
 * \param pltObj 
 * \return 
 * \note 
 * \example
 * \author Zhaokun
 * \date 2010-11-24 10:00:14
*/
GBIMELOCAL GBIMEReturn CLayoutAbstract_SetInterpunctionKey(GBLPVOID pltObj)
{
	//CLASE_THIS_POITER(CLayoutVK, pltObj)
	PCLayoutAbstract pltAbstractObj = (PCLayoutAbstract)pltObj;
	GBINT size = 0;
	GBBOOL bChineseFlag = GBFalse;
	GBINT iChnSymbolsTableSize = ICLayoutAbstract_GetStaticArraySize_symbolsTable_1_Chn();
	GBINT iAlpSymbolsTableSize = ICLayoutAbstract_GetStaticArraySize_symbolsTable_1_Alp();
	GBINT iFacesTableSize = ICLayoutAbstract_GetStaticArraySize_facesTable();

	ICLayoutAbstract_ResetInterpunctionKey(pltObj);

	switch(pltAbstractObj->m_ltData.pltcData->ltID)
	{
	case LayoutIdSP9JPinYin:		// 9键拼音
	case LayoutIdSP26JPinYin:		// 26键拼音
	case LayoutIdHP9JPinYin:		// 横屏9键拼音
	case LayoutIdHP26JPinYin:		// 横屏26键拼音

	case LayoutIdWL9JPinYin:
	case LayoutIdWL26JPinYin:
		bChineseFlag = GBTrue;
		break;
	case LayoutIdSP9JLowerEnglish:  // 9键英文小写
	case LayoutIdSP26JLowerEnglish: // 26键英文小写
	case LayoutIdHP26JEnglish:      // 横屏26键英文
	case LayoutIdHP9JEnglish:		// 横屏9键英文

	case LayoutIdWL9JEnglish:
	case LayoutIdWL26JEnglish:
		bChineseFlag = GBFalse;
		break;
	default:
		return GBIME_IGNORE; // 其他Layout不处理
	}

	// 给中英文符号字符串申请内存
	if (bChineseFlag)
	{
		if (pltAbstractObj->pSymbolChineseString != NULL)
		{
			PT_Free(pltAbstractObj->pSymbolChineseString);
			pltAbstractObj->pSymbolChineseString = NULL;
		}
		size = iChnSymbolsTableSize * GBIME_SYMBOL_MAX_LEN * sizeof(GBWCHAR);
		pltAbstractObj->pSymbolChineseString = (GBWCHAR *)PT_Malloc(size);
		PT_Assert(pltAbstractObj->pSymbolChineseString != NULL);
		memset(pltAbstractObj->pSymbolChineseString, 0, size);
	}
	else
	{
		if (pltAbstractObj->pSymbolEnglishString != NULL)
		{
			PT_Free(pltAbstractObj->pSymbolEnglishString);
			pltAbstractObj->pSymbolEnglishString = NULL;
		}
		size = iAlpSymbolsTableSize * GBIME_SYMBOL_MAX_LEN * sizeof(GBWCHAR);
		pltAbstractObj->pSymbolEnglishString = (GBWCHAR *)PT_Malloc(size);
		PT_Assert(pltAbstractObj->pSymbolEnglishString != NULL);
		memset(pltAbstractObj->pSymbolEnglishString, 0, size);
	}
	
	if (pltAbstractObj->pFaceString != NULL)
	{
		PT_Free(pltAbstractObj->pFaceString);
		pltAbstractObj->pFaceString = NULL;
	}
	size = iFacesTableSize * GBIME_FACE_MAX_LEN * sizeof(GBWCHAR);
	pltAbstractObj->pFaceString = (GBWCHAR *)PT_Malloc(size);
	PT_Assert(pltAbstractObj->pFaceString != NULL);
	memset(pltAbstractObj->pFaceString, 0, size);

	GBEngine_SetEngineOption(Global_GetEnineInstance(), GB_ENGINE_FUNCTION,	GBFO_Alp_All_Num_Can_Continuously_Input, 1);			
	GBEngine_SetEngineOption(Global_GetEnineInstance(), GB_ENGINE_FUNCTION,	GBFO_Multi_Tap_Show_Cand_Wnd, 1); // 关联配置

	if (bChineseFlag)
	{
		// 1键符号
		PT_ConvertFaceString(pltAbstractObj->pSymbolChineseString, symbolsTable_1_Chn, iChnSymbolsTableSize);
		GBEngine_RegisterInterpunctionKeyEx(Global_GetEnineInstance(), 
											GBKEY_1, 
											(GBLPCWCHAR)pltAbstractObj->pSymbolChineseString, 
											GBIME_SPLIT_CHAR, 
											GBCL_MULTITAP, 
											GBIME_SYMBOL_1_ROW_NUM, 
											iChnSymbolsTableSize, 
											GBIKO_Input_Status_Confirm_Candidate);

		// 物理Layout 0键出来回车、空格
		if (IS_KB_LAYOUT(GET_LAYOUT_ID(pltAbstractObj)))
		{			
			GBEngine_RegisterInterpunctionKeyEx(Global_GetEnineInstance(), GBKEY_0, GBIME_MULTITAP_SYMBOLS_0, 0, GBCL_MULTITAP, 1, 2, 0);	
		} 
		else
		{
			// 0键表情
			PT_ConvertFaceString(pltAbstractObj->pFaceString, facesTable_0,iFacesTableSize);
			GBEngine_RegisterInterpunctionKeyEx(Global_GetEnineInstance(), 
				GBKEY_0, 
				(GBLPCWCHAR)pltAbstractObj->pFaceString, 
				GBIME_SPLIT_CHAR, 
				GBCL_MULTITAP, 
				GBIME_SYMBOL_0_ROW_NUM, 
				iFacesTableSize, 
				GBIKO_Input_Status_Confirm_Candidate);
		}		
	} 
	else
	{
		// 1键符号
		PT_ConvertFaceString(pltAbstractObj->pSymbolEnglishString, symbolsTable_1_Alp, iAlpSymbolsTableSize);
		GBEngine_RegisterInterpunctionKeyEx(Global_GetEnineInstance(), 
											GBKEY_1, 
											(GBLPCWCHAR)pltAbstractObj->pSymbolEnglishString, 
											GBIME_SPLIT_CHAR, 
											GBCL_MULTITAP, 
											GBIME_SYMBOL_1_ROW_NUM, 
											iAlpSymbolsTableSize, 
											GBIKO_Input_Status_Confirm_Candidate);

		// 物理Layout 0键出来回车、空格
		if (IS_KB_LAYOUT(GET_LAYOUT_ID(pltAbstractObj)))
		{			
			GBEngine_RegisterInterpunctionKeyEx(Global_GetEnineInstance(), GBKEY_0, GBIME_MULTITAP_SYMBOLS_0, 0, GBCL_MULTITAP, 1, 2, 0);	
		} 
		else // 虚拟键盘0键表情
		{			
			PT_ConvertFaceString(pltAbstractObj->pFaceString, facesTable_0, iFacesTableSize);
			GBEngine_RegisterInterpunctionKeyEx(Global_GetEnineInstance(), 
												GBKEY_0, 
												(GBLPCWCHAR)pltAbstractObj->pFaceString, 
												GBIME_SPLIT_CHAR, 
												GBCL_MULTITAP, 
												GBIME_SYMBOL_0_ROW_NUM, 
												iFacesTableSize, 
												GBIKO_Input_Status_Confirm_Candidate);
		}
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
GBIMELOCAL GBIMEReturn CLayoutAbstract_DisableFuncKeyHandler(GBLPVOID pltObj)
{
	PCLayoutAbstract pltAbstractObj = (PCLayoutAbstract)pltObj;
	PLayoutInfo pLayoutInfo = (PLayoutInfo)(&pltAbstractObj->m_ltData); // Layout UI data point
	KEYBOARD_TYPE keyboardType = GBConfig_GetKeyboardType(Global_GetConfigInstance());

	// 根据平台输入框是否处于快速查找输入模式决定是否禁用某些功能按钮	
	if (GBInputBox_IsQuickSearchInputMode())
	{
		switch (keyboardType)
		{
		case KEYBOARD_VK_NUMPAD:
			CLayoutInfo_SetButtonStatus(pLayoutInfo, GBKEY_VK_V9_PINYIN, ButtonStatusGray);		///<竖屏9键拼音
			CLayoutInfo_SetButtonStatus(pLayoutInfo, GBKEY_VK_V9_ENG_LOWER, ButtonStatusGray);	///<竖屏9键小写英文
			CLayoutInfo_SetButtonStatus(pLayoutInfo, GBKEY_VK_V9_NUM, ButtonStatusGray);		///<竖屏9键数字
			CLayoutInfo_SetButtonStatus(pLayoutInfo, GBKEY_VK_V9_SYM, ButtonStatusGray);		///<竖屏9键符号
			CLayoutInfo_SetButtonStatus(pLayoutInfo, GBKEY_VK_V9_FS_HANDWRITE, ButtonStatusGray);///<竖屏全屏手写
			CLayoutInfo_SetButtonStatus(pLayoutInfo, GBKEY_VK_BACKWORD, ButtonStatusGray);
			break;
		case KEYBOARD_VK_QWERTY:
			CLayoutInfo_SetButtonStatus(pLayoutInfo, GBKEY_VK_CAP, ButtonStatusGray);           ///<大小写切换键
			CLayoutInfo_SetButtonStatus(pLayoutInfo, GBKEY_VK_V26_PINYIN, ButtonStatusGray);	///<竖屏9键拼音
			CLayoutInfo_SetButtonStatus(pLayoutInfo, GBKEY_VK_V26_ENG_LOWER, ButtonStatusGray);	///<竖屏9键小写英文
			CLayoutInfo_SetButtonStatus(pLayoutInfo, GBKEY_VK_V9_NUM, ButtonStatusGray);		///<竖屏9键数字
			CLayoutInfo_SetButtonStatus(pLayoutInfo, GBKEY_VK_V9_SYM, ButtonStatusGray);		///<竖屏9键符号
			CLayoutInfo_SetButtonStatus(pLayoutInfo, GBKEY_VK_V9_FS_HANDWRITE, ButtonStatusGray);///<竖屏全屏手写
			CLayoutInfo_SetButtonStatus(pLayoutInfo, GBKEY_VK_BACKWORD, ButtonStatusGray);
		default: // 其他键盘不支持
			break;
		}

		// 禁止滑屏切换
		pltAbstractObj->isSlidingDisabled = GBTrue;
	}
	else
	{
		switch (keyboardType)
		{
		case KEYBOARD_VK_NUMPAD:				
			CLayoutInfo_SetButtonStatus(pLayoutInfo, GBKEY_VK_V9_PINYIN, ButtonStatusNomal);	///<竖屏9键拼音
			CLayoutInfo_SetButtonStatus(pLayoutInfo, GBKEY_VK_V9_ENG_LOWER, ButtonStatusNomal);	///<竖屏9键小写英文
			CLayoutInfo_SetButtonStatus(pLayoutInfo, GBKEY_VK_V9_NUM, ButtonStatusNomal);		///<竖屏9键数字
			CLayoutInfo_SetButtonStatus(pLayoutInfo, GBKEY_VK_V9_SYM, ButtonStatusNomal);		///<竖屏9键符号
			CLayoutInfo_SetButtonStatus(pLayoutInfo, GBKEY_VK_V9_FS_HANDWRITE, ButtonStatusNomal);///<竖屏全屏手写
			CLayoutInfo_SetButtonStatus(pLayoutInfo, GBKEY_VK_BACKWORD, ButtonStatusNomal);
			break;
		case KEYBOARD_VK_QWERTY:
			CLayoutInfo_SetButtonStatus(pLayoutInfo, GBKEY_VK_CAP, ButtonStatusNomal);          ///<大小写切换键
			CLayoutInfo_SetButtonStatus(pLayoutInfo, GBKEY_VK_V26_PINYIN, ButtonStatusNomal);	///<竖屏9键拼音
			CLayoutInfo_SetButtonStatus(pLayoutInfo, GBKEY_VK_V26_ENG_LOWER, ButtonStatusNomal);///<竖屏9键小写英文
			CLayoutInfo_SetButtonStatus(pLayoutInfo, GBKEY_VK_V9_NUM, ButtonStatusNomal);		///<竖屏9键数字
			CLayoutInfo_SetButtonStatus(pLayoutInfo, GBKEY_VK_V9_SYM, ButtonStatusNomal);		///<竖屏9键符号
			CLayoutInfo_SetButtonStatus(pLayoutInfo, GBKEY_VK_V9_FS_HANDWRITE, ButtonStatusNomal);///<竖屏全屏手写
			CLayoutInfo_SetButtonStatus(pLayoutInfo, GBKEY_VK_BACKWORD, ButtonStatusNomal);
		default: // 其他键盘不支持
			break;
		}

		// 允许滑屏切换
		pltAbstractObj->isSlidingDisabled = GBFalse;
	}
}