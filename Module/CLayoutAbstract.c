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
	pltDest->m_pfcRelease = CLayoutAbstract__ReleaseBase;	//ȱʡ
	pltDest->m_pfcReleaseBase = CLayoutAbstract__ReleaseBase;
	pltDest->m_pfcSetRefreshFlag = CLayoutAbstract__SetRefreshFlag;
	pltDest->m_pfcPaint = CLayoutAbstract__Paint;
	//pltDest->m_pfcPreHandleEvent = CLayoutAbstract__PreHandleEvent; // VK����
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
	//pltDest->m_pfcPhysicFuncKeyHandler = CLayoutAbstract_PhysicFuncKeyHandler; // VK����
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
 * \brief ��ʼ��Layout��ر���
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

	// �����ṹ��
	memset(&pltAbstractObj->slidingOperation, 0, sizeof(SlidingOperation));
	pltAbstractObj->isSlidingDisabled = GBFalse;

	/*!
	 * \brief Layout ˢ����ʾ����Ϣ������ر���
	 */
	pltAbstractObj->m_refreshFlag = GBIME_LAYOUT_REFRESH_NONE;
	memset(&pltAbstractObj->syllableDisplayStartPos, 0, sizeof(GBIMEPoint));///<��������ʼ��ʾλ��	
	memset(&pltAbstractObj->candidateDisplayStartPos, 0, sizeof(GBIMEPoint));///<��ѡ����ʼ��ʾλ��	
	pltAbstractObj->pressLayoutKeyValue = 0;///<��¼����ʱ��Layout����ֵ(ֻ�е����º͵����ֵһ��ʱ��ִ�а�������)
	pltAbstractObj->currentLayoutKeyValue = 0;///<��ǰ���еİ�����Ӧ��ֵ
	pltAbstractObj->currentSyllableIndex = 0;///<��ǰѡ�е���������
	pltAbstractObj->currentCandidateIndex = 0;///<��ǰѡ�еĺ�ѡ����
	pltAbstractObj->penDownPostion = PenPositionInNothing;///<�����ʰ��µ�λ��
	pltAbstractObj->penInPostion = PenPositionInNothing;///<��ǰ���е�λ��
	pltAbstractObj->buttonStatus = ButtonStatusNomal;///<��ť״̬

	/*!
	 * \brief Multitap���Ź�����ر���
	 * note: ���MultiTap״̬֮ǰ���������õ�ǰLayout��MultiTap��ʱ��, ȡ��������ʾ
	 */
	if (pltAbstractObj->m_pfcKBMultiTapTimerCallBack != NULL)
	{
		pltAbstractObj->m_pfcKBMultiTapTimerCallBack();
	}
	if (pltAbstractObj->m_pfcOnMultitapTimer != NULL)
	{
		pltAbstractObj->m_pfcOnMultitapTimer();
	}

	pltAbstractObj->bMultitapTimerStartFlag = GBFalse;///<MultiTap��ʱ���������
	pltAbstractObj->bMultitapSymbolStartFlag = GBFalse;///<MultiTap�����������
	pltAbstractObj->bMultitapFaceStartFlag = GBFalse;///<MultiTap�����������
	pltAbstractObj->multitapReplaceCandLength = 0;///<�����滻��ѡ�ַ�����
	pltAbstractObj->multitapCandIndex = 0;///<Multitap��ѡ���ŵ�ǰ����

	/*!
	 * \brief GBV5���潻����ر���
	 */
	pltAbstractObj->bSendEngineKeyEvent = GBFalse;///<�Ƿ���Ҫ�������水���¼� 
	pltAbstractObj->engineKeyEvent = GBEvent_None;///<����LayoutԤ����֮��ת��������水���¼�
	pltAbstractObj->engineEventParam = 0;///<����LayoutԤ����֮��ת����������¼�����
	pltAbstractObj->bUpToInputKeySequence = GBFalse;///<�����������
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
		//�ô������ͷţ��������õ� Zhaokun
// 		PT_Free(pltAbstractObj);
// 		pltAbstractObj = NULL;

		pltAbstractObj->lastUseTime = 0;
	}
	

	return GBIME_OK;
}

/*!
 * \brief ����Layout��ʾ����ˢ�±�ǣ�������Чˢ�£����ˢ���ٶ�
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
 * \brief �жϵ�ǰ�������Ƿ����ڼ��̰�ť��
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

	//��ǰҪ��ʾ�Ĳ�
	GBUINT16 iCurrentLayerNumber;
	GBUINT16 iButtonTotalCount;
	GBUINT16 iCount;
	GBUINT16 iEngineKeyValue;
	//GBINT32 left,top,right,buttom;
	GBIMEPoint winPos;
	GBRECT buttonPos;
	PLayoutChgtLayer pLayoutChgtLayer;

	// ��ǰLayoutId
	iCurrentLayoutId = GET_LAYOUT_ID(pltAbstractObj);
	// ��ȡ���̸߶�
	iKeyBoardHeight = pltAbstractObj->m_pfcGetLayoutHeight(pltObj,iCurrentLayoutId);
	// ȡ�ü�����ʾ�Ŀ�ʼλ�á����½ǡ���������Ϊ�����Ͻǡ�
	Global_GetWinPos(&winPos);	
	winPos.y -=  iKeyBoardHeight;
	// ��ǰҪ��ʾ�Ĳ�
	iCurrentLayerNumber = GET_LAYOUT_CURRENT_LAYER(pltAbstractObj);
	// ȡ��ѡ��߶�
	iCandidateBoxHeight = pltAbstractObj->m_pfcGetCandidateBoxHeight(pltObj,iCurrentLayoutId,iCurrentLayerNumber);
	//�Ȳ�ڵز�
	pLayoutChgtLayer = (PLayoutChgtLayer)&(pltAbstractObj->m_ltData.pltLayerData[FirstLayer]);

	// ��һ����������ʱ�������ѯ����
	iButtonTotalCount = pLayoutChgtLayer->lyBtCount;

	// Ĭ��Ϊ��Ч���λ��
	pltAbstractObj->penInPostion = PenPositionInNothing;

	// ��ѯÿһ����ť
	for (iCount = 0; iCount < iButtonTotalCount; iCount++)
	{
		// ��ȡ��ǰ��ťλ�ò�ת��Ϊ��Ļ�������
		buttonPos = pLayoutChgtLayer->plycBtData[iCount].btPos;
		buttonPos.top += winPos.y;
		buttonPos.bottom += winPos.y;

		if (PT_PointIsInRect(hitPoint, buttonPos))
		{
			// �����ɫ���ð�ť��������Ч
			if (pLayoutChgtLayer->plyBtChgData[iCount].btStatus == ButtonStatusGray)
			{
				return pltAbstractObj->penInPostion;
			}

			// ��������״̬[����ʾ��]
			pLayoutChgtLayer->plyBtChgData[iCount].btStatus = buttonStatus;			
			
			// �������̵İ�ť��
			penPositionInWhere = PenPositionInKeyboardButton;
			pltAbstractObj->buttonStatus = buttonStatus; // ��¼��ť״̬
			pltAbstractObj->penInPostion = penPositionInWhere;

			// ��¼����ʱ��Layout����ֵ(ֻ�е����º͵����ֵһ��ʱ��ִ�а�������)
			if (buttonStatus == ButtonStatusDown)
			{
				pltAbstractObj->penDownPostion = PenPositionInKeyboardButton;
				pltAbstractObj->pressLayoutKeyValue = pLayoutChgtLayer->plycBtData[iCount].btKey;
			}

			// ȡ�õ�ǰ��ť��ֵ
			pltAbstractObj->currentLayoutKeyValue = pLayoutChgtLayer->plycBtData[iCount].btKey;
			// ת��Ϊ�����ֵ
			iEngineKeyValue = CLayoutInfo_ConversionLayoutKeyToEngineKey(pltAbstractObj->currentLayoutKeyValue);
			break;
		}
	}

	// �뿪����
	if (iCount == iButtonTotalCount
		&& pltAbstractObj->penDownPostion == PenPositionInKeyboardButton)
	{
		// ��¼��ť״̬
		pltAbstractObj->buttonStatus = buttonStatus;
		// ��¼���е�λ��
		penPositionInWhere = pltAbstractObj->penInPostion = PenPositionLeave;			
		// ��¼���еĺ�ѡ����ֵ
		pltAbstractObj->currentLayoutKeyValue = GBEvent_None;
	}
	
	//����ǵڶ��������ʱ������Ҫ����е��Ƿ��ǵڶ��������ķǺ�ѡ����ť
	if (iCurrentLayerNumber == SecondLayer || iCurrentLayerNumber == ThirdLayer)
	{
		//�Ȳ�ڵز�
		pLayoutChgtLayer = (PLayoutChgtLayer)&(pltAbstractObj->m_ltData.pltLayerData[iCurrentLayerNumber]);
		//��������ʱ�������ѯ����
		iButtonTotalCount = pLayoutChgtLayer->lyBtCount;
		
		// ��ѯÿһ����ť
		for (iCount = 0; iCount < iButtonTotalCount; iCount++)
		{
			//����ǵڶ��������ķǺ�ѡ����ť
			if (pLayoutChgtLayer->plycBtData[iCount].btPos.bottom > iCandidateBoxHeight)
			{
				// ��ȡ��ǰ��ťλ�ò�ת��Ϊ��Ļ�������
				buttonPos = pLayoutChgtLayer->plycBtData[iCount].btPos;
				buttonPos.top += (winPos.y - iCandidateBoxHeight);
				buttonPos.bottom += (winPos.y - iCandidateBoxHeight);
				//��������˵ڶ��������ķǺ�ѡ����ť
				if (PT_PointIsInRect(hitPoint, buttonPos))
				{
					// ��������״̬[����ʾ��]
					pLayoutChgtLayer->plyBtChgData[iCount].btStatus = buttonStatus;			
					
					// �������̵İ�ť��
					penPositionInWhere = PenPositionInKeyboardButton;
					pltAbstractObj->penInPostion = penPositionInWhere;
					pltAbstractObj->buttonStatus = buttonStatus; // ��¼��ť״̬

					// ��¼����ʱ��Layout����ֵ(ֻ�е����º͵����ֵһ��ʱ��ִ�а�������)
					if (buttonStatus == ButtonStatusDown)
					{
						pltAbstractObj->pressLayoutKeyValue = pLayoutChgtLayer->plycBtData[iCount].btKey;
					}

					// ȡ�õ�ǰ��ť��ֵ
					pltAbstractObj->currentLayoutKeyValue = pLayoutChgtLayer->plycBtData[iCount].btKey;
					// ת��Ϊ�����ֵ
					iEngineKeyValue = CLayoutInfo_ConversionLayoutKeyToEngineKey(pltAbstractObj->currentLayoutKeyValue);
					break;
				}
			}
		}
	}

	return pltAbstractObj->penInPostion;
}

/*!
 * \brief ��ȡ��ǰ�����ʰ���ʱ��λ�ã��ں�ѡ�ϣ��ڼ��̰�ť�ϣ������ط�
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
 * \brief Layout������Ϣ������(���һЩ������ͨ����Ϣ����)
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
	case GBIME_EVENT_LAYOUT_ACTIVATE: ///<Layout ����
		pltAbstractObj->m_ltData.ltCurLayer = FirstLayer;
		Global_SetLayoutWinActiveStatus(GBTrue);
		// ��������״μ����ʾ
		if (IS_KB_LAYOUT(nCurrentLayoutID))
		{
			Global_SetLayoutWinHeight(0);
		} 
		else // ��������״μ�����ʾ����
		{
			Global_SetLayoutWinHeight(pltAbstractObj->m_pfcGetLayoutHeight(pltObj, nCurrentLayoutID));
		}
	
		// ����ƽ̨����ģʽ�����Ƿ����ĳЩ���ܰ�ť
		pltAbstractObj->m_pfcDisableFuncKeyHandler(pltObj);

		pltAbstractObj->m_refreshFlag = GBIME_LAYOUT_REFRESH_ALL;
		break;
	case GBIME_EVENT_LAYOUT_DEACTIVATE: ///<Layout ע��
		pltAbstractObj->m_ltData.ltCurLayer = FirstLayer;
		Global_SetLayoutWinActiveStatus(GBFalse);
		Global_SetLayoutWinHeight(0);
		// ���³�ʼ��Layout����״̬����
		pltAbstractObj->m_pfcInitVariable(pltObj);
		break;
	case GBIME_EVENT_LAYOUT_HIDE: ///<Layout ����
		pltAbstractObj->m_ltData.ltCurLayer = FirstLayer;
		Global_SetLayoutWinActiveStatus(GBFalse);
		pltAbstractObj->m_pfcChangeHeight(pltObj, 0);
		// ���³�ʼ��Layout����״̬����
		pltAbstractObj->m_pfcInitVariable(pltObj);
		break;
	case GBIME_EVENT_LAYOUT_SHOW: ///<Layout ��ʾ
		pltAbstractObj->m_ltData.ltCurLayer = FirstLayer;
		Global_SetLayoutWinActiveStatus(GBTrue);
		// ������̲���ʾ
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
		//Global_SetLayoutWinHeight(0); // �߶����㣬���Layout���л����ڸ߶�һ��û��ˢ��VK
		pltAbstractObj->m_pfcChangeHeight(pltObj, pltAbstractObj->m_pfcGetLayoutHeight(pltObj, nCurrentLayoutID));
		pltAbstractObj->m_refreshFlag = GBIME_LAYOUT_REFRESH_ALL;
		//pltAbstractObj->m_pfcInitVariable(pltObj);
		// ����ƽ̨����ģʽ�����Ƿ����ĳЩ���ܰ�ť
		pltAbstractObj->m_pfcDisableFuncKeyHandler(pltObj);
		break;
	case GBIME_EVENT_LAYOUT_EXIT:
		Global_SetLayoutWinHeight(0); // �߶����㣬���Layout���л����ڸ߶�һ��û��ˢ��VK
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
 *-brief Layout �����Ƿ��Layout IDƥ��
 *-return ƥ�䷵��1����ƥ�䷵��-1
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
 *-brief �Ƿ���Ҫ����Layout ����
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
 * \brief �ı�Layout���ڸ߶�, ��Ҫ֪ͨƽ̨���������������ĸ߶�
 * \param pltObj 
 * \param newHeight 
 * \return 
 */
GBIMELOCAL GBIMEReturn CLayoutAbstract__ChangeHeight(GBLPVOID pltObj, GBINT newHeight)
{
	PCLayoutAbstract pCLayoutAbstract = (PCLayoutAbstract)pltObj;

	// ����Ƶ�������߶Ƚ���ˢ��Ч��
 	if (Global_GetLayoutWinHeight() == newHeight)
 	{
 		return GBIME_OK;
 	}

	// ����Layout������ʾ�߶�
	Global_SetLayoutWinHeight(newHeight);

	// ���ݴ���߶���ʾ/���ع������뷨����������
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
 * \brief Layout������ѡ
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
 * \brief Layout ֪ͨ�༭���ػ�
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
 * \brief ȡ�ú�ѡ��ͼ�ĸ߶�[��ѡ��Ҳ��һ��layout]
 * \param layoutId 
 * \return ��ѡ��ͼ�ĸ߶�
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
 * \brief ȡ�ú�ѡ��ͼ�ĸ߶�[��ѡ��Ҳ��һ��layout]
 * \param layoutId 
 * \return ��ѡ��ͼ�ĸ߶�
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
 * \brief ���ݰ�ť��ֵ,ȡ�ð�ť��ͼ���е�����
 * \param layoutId 
 * \return ��ѡ��ͼ�ĸ߶�
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

// ȡ�ò����Ϣ
GBIMELOCAL PLAYOUTCONSTLAYER CLayoutAbstract_GetLayerConstInfo(GBLPVOID pltObj, LayoutIdEnum layoutId, LayerIndex layerId)
{
	GBLPCVOID pSkinData = Global_GetSkinData();
	pltObj = pltObj;

	return CLayoutInfo_GetLayerConstInfo(pSkinData, (GBUINT16)layoutId, (GBUINT16)layerId);
}
// ȡ�ð�ť��Ϣ
GBIMELOCAL PBUTTONCONSTDATA CLayoutAbstract_GetLayerConstButtons(GBLPVOID pltObj, LayoutIdEnum layoutId, LayerIndex layerId)
{
	GBLPCVOID pSkinData = Global_GetSkinData();
	pltObj = pltObj;

	return CLayoutInfo_GetLayerConstButtons(pSkinData, (GBUINT16)layoutId, (GBUINT16)layerId);
}

/*!
 * \brief �л�Layout������
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
 * \brief ���ܼ�������
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
	case GBKEY_VK_SETTING: ///<����
		Global_FillEventGroup(GBIME_CT_CONFIG_MENU, 0, 0);
		break;
	case GBKEY_VK_V9_PINYIN_SEPARATOR://ƴ���ָ���
		if (pEngineOutputInfo->bCandWindOpen) // ���洦������У�����ָ���
		{
			pltBase->bSendEngineKeyEvent = GBTrue;
			pltBase->engineKeyEvent = GBKEY_AMB_SPECIAL_A;
			Global_FillEventGroup(GBIME_CT_NORMAL_KEY_UP, 0, 0);
		}
		break;
	case GBKEY_VK_V9_SYMBOL_KEY: // MultiTap�����Ŵ���		
		pltBase->m_pfcMultiTapSymbolHandler(pltObj);
		Global_FillEventGroup(GBIME_CT_KEY_UP_REFRESH, 0, 0); // ��ť����ˢ��
		break;
	case GBKEY_VK_V9_SYMBOL_FACE_MULTIP_KEY: // MultiTap������Ŵ���		
		pltBase->m_pfcMultiTapFaceHandler(pltObj);
		Global_FillEventGroup(GBIME_CT_KEY_UP_REFRESH, 0, 0); // ��ť����ˢ��
		break;
	case GBKEY_RETURN: // ������ʱ��������س����������С�����ʱ��������س���������������
		if (pEngineOutputInfo->bCandWindOpen)
		{
			// ����������Ҫȷ�����ڷ�ѡ��״̬
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
			Global_FillEventGroup(GBIME_CT_KEY_UP_REFRESH, 0, 0); // ��ť����ˢ��
		}
		break;
	case GBKEY_SPACE: // ������ʱ��������ո񡿼�������ո�����ʱ��������ո񡿼�����ѡ��������
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
			Global_FillEventGroup(GBIME_CT_KEY_UP_REFRESH, 0, 0); // ��ť����ˢ��
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
	case GBKEY_VK_CAP: // �л�Ӣ�������Сд
		if (GBEngine_IsAlpInputMode(Global_GetEnineInstance()))
		{
			pltBase->m_pfcSwitchShiftCapStatus(pltObj, 0, GBTrue, pEngineOutputInfo);	
		}
		break;
	case GBKEY_AMB_SPECIAL_A: // ���롮*��
		tempStr[0] = '*';
		tempStr[1] = 0;
		pltBase->m_pfcUpScreen(pltObj, tempStr);
		Global_FillEventGroup(GBIME_CT_KEY_UP_REFRESH, 0, 0); // ��ť����ˢ��
		break;
	case GBKEY_AMB_SPECIAL_B: // ���롮#��
		tempStr[0] = '#';
		tempStr[1] = 0;
		pltBase->m_pfcUpScreen(pltObj, tempStr);
		Global_FillEventGroup(GBIME_CT_KEY_UP_REFRESH, 0, 0); // ��ť����ˢ��
		break;
	default:
		return GBIME_IGNORE;
		break;
	}
	
	return GBIME_OK;
}


/*!
 * \brief �л���ǰһ��Layout
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
// ����ͼ��ID�Ͳ�ţ�ȡ�ú�ѡ��߶�(ͼ�㲻���ڻ��������ʱ������0)
GBIMELOCAL GBINT16 CLayoutAbstract_GetCandidateBoxHeight(GBLPVOID pltObj, LayoutIdEnum layoutId, LayerIndex layerId)
{
	GBLPCVOID pSkinData = Global_GetSkinData();
	pltObj = pltObj;
	return CLayoutInfo_GetCandidateBoxHeight(pSkinData,(GBUINT16)layoutId,(GBUINT16)layerId);
}

/*!
 * \brief ��ȡLayout�����������Ļ��λ��(Layout���Ͻ�)
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
 * \brief ��ȡLayout������Ч����
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
	Global_GetWinPos(&layoutWinStartPos); // note: ���½�λ��
	FILL_RECT(pLayoutWinRect, 
			  layoutWinStartPos.x, 
			  (GBINT16)(layoutWinStartPos.y - Global_GetLayoutWinHeight()), 
		      Global_GetLayoutWinWidth(), 
			  layoutWinStartPos.y);
}

/*!
 * \brief �ж��Ƿ������밴��
 * \param keyValue 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-17 17:22:37
*/
GBIMELOCAL GBBOOL CLayoutAbstract_IsInputKey(GBUINT16 keyValue)
{
	// �����ֵ
	if ((keyValue > GBEvent_None) && (keyValue < GBKEY_VK_A_UPPER))
	{
		if (((keyValue >= GBKEY_0) && (keyValue <= GBKEY_9)) // ���ּ�
			||((keyValue >= GBKEY_A) && (keyValue <= GBKEY_Z))) // ��ĸ��
		{
			return GBTrue;
		}
		
	}
	else // ������̼�ֵ
	{
		if (((keyValue >= GBKEY_VK_A_UPPER)&&(keyValue <= GBKEY_VK_10_NUMBERKEY)))
		{
			return GBTrue;
		}		
	}

	return GBFalse;
}

/*!
 * \brief ���������ʰ�����Ϣ������
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
	
	// �������������ǲ�������ʱ������¼��ʼλ�á���ʼʱ��
	memset(&pltAbstractObj->slidingOperation, 0, sizeof(SlidingOperation));
	pltAbstractObj->slidingOperation.startPos.x = curPt.x;
	pltAbstractObj->slidingOperation.startPos.y = curPt.y;
	pltAbstractObj->slidingOperation.lastMovePos.x = curPt.x;
	pltAbstractObj->slidingOperation.lastMovePos.y = curPt.y;
	pltAbstractObj->slidingOperation.slidingStartTime = PT_GetCurrentTime();
	pltAbstractObj->slidingOperation.slidingElapsedTime = 0;
	pltAbstractObj->slidingOperation.slidingStatus = GBIME_SLIDING_INIT; // ��ʼ״̬
#endif
	return GBIME_OK;
}

/*!
 * \brief ���������ʵ�����Ϣ������[���������������]
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

	// ���㻬����������ʵ�ֻ����л�Layout
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
		// ���������������״̬��
		memset(&pltAbstractObj->slidingOperation, 0, sizeof(SlidingOperation));
		return GBIME_OK;
	}
	memset(&pltAbstractObj->slidingOperation, 0, sizeof(SlidingOperation));
#endif
	return GBIME_IGNORE;
}

/*!
 * \brief �����������ƶ���Ϣ������
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

	// ����Layout������Ч��������״̬
	if (!PT_PointIsInRect(curPt, layoutWinRect))
	{
		memset(&pltAbstractObj->slidingOperation, 0, sizeof(SlidingOperation));
		return GBIME_IGNORE;
	}

	// ���㵱ǰ���������ƶ�����
	distanceH = curPt.x - pltAbstractObj->slidingOperation.lastMovePos.x;
	distanceV = curPt.y - pltAbstractObj->slidingOperation.lastMovePos.y;
	
	// ��¼��һ��Move������
	pltAbstractObj->slidingOperation.lastMovePos.x = curPt.x;
	pltAbstractObj->slidingOperation.lastMovePos.y = curPt.y;

	// ��ֹƵ�����������Ӧ�ٶ�
	if (ABS(distanceH) < GBIME_SLIDING_MINIMUM_PIXELS 
		&& ABS(distanceV) < GBIME_SLIDING_MINIMUM_PIXELS)
	{
		return GBIME_IGNORE;
	}

	// �жϷ���
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

	// ���ݻ���״̬������
	switch (pltAbstractObj->slidingOperation.slidingStatus)
	{
	case GBIME_SLIDING_INIT: // ��ʼ״̬
		pltAbstractObj->slidingOperation.slidingStatus = GBIME_SLIDING_MOVING;
		pltAbstractObj->slidingOperation.slidingDirection = curSlidingDirection;
		break;
	case GBIME_SLIDING_MOVING: // �ƶ�״̬
		// �����������仯������״̬
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
			// ���»���ʱ��
			tempTime = PT_GetCurrentTime();
			pltAbstractObj->slidingOperation.slidingElapsedTime = tempTime - pltAbstractObj->slidingOperation.slidingStartTime;
			
			// ���¾��롢�ٶ�
			distanceH = ABS(curPt.x - pltAbstractObj->slidingOperation.startPos.x);
			distanceV = ABS(curPt.y - pltAbstractObj->slidingOperation.startPos.y);
			//pltAbstractObj->slidingOperation.slidingDistance = (GBINT)(ABS(sqrt((distanceH * distanceH) + (distanceV * distanceV))));
			pltAbstractObj->slidingOperation.slidingDistance = PT_GetPointDistance(distanceH, distanceV);
			if (pltAbstractObj->slidingOperation.slidingElapsedTime > 0)
			{
				pltAbstractObj->slidingOperation.slidingSpeed = pltAbstractObj->slidingOperation.slidingDistance * 1000
					/ pltAbstractObj->slidingOperation.slidingElapsedTime;
			}
			
			// ���㻬��Ҫ��ʵ��Layout�����л�
			if (pltAbstractObj->slidingOperation.slidingSpeed >= GBIME_SLIDING_ACTION_SPEED_REQUIRED
				&& pltAbstractObj->slidingOperation.slidingDistance >= GBIME_SLIDING_ACTION_DISTANCE_REQUIRED)
			{
				pltAbstractObj->slidingOperation.slidingStatus = GBIME_SLIDING_EXCUTE_ACTION; // ִ�л�������״̬
			}
		}
		break;
	default:
		break;
	}

// 	PT_PrintLogInfo(GBLOG_LAYOUTINFO, ("########### �����ٶ�[%d], ��������[%d] ########\n", 
// 					pltAbstractObj->slidingOperation.slidingSpeed, 
// 					pltAbstractObj->slidingOperation.slidingDistance));
#endif
	return GBIME_OK;
}

/*!
 * \brief ������������(������ɾ��)
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
		else if (pltAbstractObj->buttonStatus == ButtonStatusUp) // ��������ȡ���ص�
		{
			PT_GUICancelTimer(CLayoutAbstract_DeleteAll);
			if (pltAbstractObj->bLongPressFlag)
			{
				pltAbstractObj->bLongPressFlag = GBFalse;
				Global_FillEventGroup(GBIME_CT_KEY_UP_REFRESH, 0, 0); // ��ť����ˢ��
				return GBIME_OK; // ���������������̰ܶ�GBKEY_BACK�Ĺ��ܴ������
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
 * \brief ����ɾ��������
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

	// ����������봮����
	if (pEngineOutputInfo->bCandWindOpen/* && !pEngineOutputInfo->bInputEmpty*/)
	{		
		GBEngine_Reset(Global_GetEnineInstance());
		CLayoutControl_UpdateOutputInfo();
		pltAbstractObj->m_pfcCloseCandWin(pltAbstractObj, pEngineOutputInfo);
	}
	else // ��ձ༭�����������
	{
		GBInputBox_DeleteAll();
	}

	pltAbstractObj->bLongPressFlag = GBTrue;
	
	// ���ù��ܰ���������״̬���رպ�ѡ��
	//GBResetFuncKey();
}

/*!
 * \brief ��ʾ��ѡ��(����ڶ�������״̬)
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
			pltAbstractObj->currentSyllableIndex = 0; // ������������
			pltAbstractObj->currentCandidateIndex = 0; // �����ѡ����
			pltAbstractObj->m_ltData.ltCurLayer = SecondLayer;
			iCandidateBoxHeight = pltAbstractObj->m_pfcGetCandidateBoxHeight(pltObj, GET_LAYOUT_ID(pltAbstractObj), SecondLayer);
			pltAbstractObj->m_pfcChangeHeight(pltObj, iKeyBoardHeight+iCandidateBoxHeight);
			return GBIME_OK;
		}
	}

	return GBIME_IGNORE;
}

/*!
 * \brief ��ʾ�����(�������������״̬)
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
	
	pltAbstractObj->currentSyllableIndex = 0; // ������������
	pltAbstractObj->m_ltData.ltCurLayer = ThirdLayer;	
	pltAbstractObj->m_pfcChangeHeight(pltObj, iKeyBoardHeight + iCandidateBoxHeight);
	return GBIME_OK;
}

/*!
 * \brief ���������ѡ��״̬�����Ƿ�ر�Layout����ѡ��
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

	if (!pEngineOutputInfo->bCandWindOpen) // ���봮Ϊ��ʱ
	{
		pltAbstractObj->currentSyllableIndex = 0; // ������������
		pltAbstractObj->m_ltData.ltCurLayer = FirstLayer;
		pltAbstractObj->m_pfcChangeHeight(pltObj, iKeyBoardHeight);
		return GBIME_OK;
	}

	return GBIME_IGNORE;
}

/*!
 * \brief ����Ƿ���Ҫ������ѡ��
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
	    || pltAbstractObj->bUpToInputKeySequence) // ��������
	{
		pltAbstractObj->currentCandidateIndex = 0;
		pltAbstractObj->m_pfcUpScreen(pltObj, pEngineOutputInfo->pUpScreenStr);
		return GBIME_OK;
	}

	return GBIME_IGNORE;
}

/*!
 * \brief MultiTap���Ŵ�����
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
 * \brief MultiTap������Ŵ�����
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

	// �Ƿ�������Ŵ������Ƥ��������д�ı��鰴������
	if (pFaceString == NULL || wcslen(pFaceString) == 0)
	{
		return GBIME_FAILED;
	}

	// ��ֱ������
	pFaceStart = pFaceString;
	faceStrLen = wcslen(pFaceString);
	for (i = 0; i < faceStrLen; i++)
	{
		if (pFaceString[i] == GBIME_MULTITAP_FACE_SPLIT_CHAR)
		{
			GBINT curFaceLen = (GBINT)(pFaceString + i - pFaceStart);
			wcsncpy(faceArray[faceNum], pFaceStart, curFaceLen);
			faceArray[faceNum][curFaceLen] = 0; // ������
			pFaceStart = pFaceString + i + 1;
			faceNum++;
		}
	}

	// �Ƿ�������Ŵ������Ƥ��������д�ı��鰴������
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
 * \brief MultiTap���Ź��ܶ�ʱ���������ʱ����Ӧ����
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
 * \brief shiftCap״̬�л�(ͬ���л�GBV5����shiftCap״̬)
 * \param pltObj 
 * \param shiftCapStatus �л���ָ��״̬
 * \param bAuto �Զ�ѭ���л�
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
			Global_FillEventGroup(GBIME_CT_KEY_UP_REFRESH, 0, 0); // ��ť����ˢ��
			break;
		case GBStatus_Caplock:
			Global_SetShiftCapStatus(GBStatus_Normal);		
			Global_FillEventGroup(GBIME_CT_SWITCH, CLayoutInfo_GetLowerLayoutID(iCurrentLayoutId), 0);
			break;
		default:
			break;
		}
		
		// ���������״̬����¼��ǰ���봮�����л���Ŀ��Layout�������������봮����ʾΪָ���Ĵ�Сд״̬
// 		if (pEngineOutputInfo->bCandWindOpen 
// 			&& pEngineOutputInfo->pInputString != NULL
// 			&& wcslen(pEngineOutputInfo->pInputString) > 0)
// 		{
// 			wcsncpy(pltAbstractObj->backupEngineInputString, pEngineOutputInfo->pInputString, GB_INPUT_BUFFER_LEN);///<����������������
// 			pltAbstractObj->bReuseInputStringFlag = GBTrue;
// 		}
	}
	else // ָ���л���Ŀ��״̬����Ӧ��Layout[����ĸ��д�������������л���СдLayout]
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
 * \brief ͬ������shiftCap״̬
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
 * \brief ��ȡLayout�������
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

	// ��ǰLayoutId
	iCurrentLayoutId = GET_LAYOUT_ID(pltAbstractObj);
	// ��ȡ���̸߶�
	iKeyBoardHeight = pltAbstractObj->m_pfcGetLayoutHeight(pltObj,iCurrentLayoutId);
	iKeyBoardWidth = pltAbstractObj->m_pfcGetLayoutWidth(pltObj, iCurrentLayoutId);
	// ȡ�ü�����ʾ�Ŀ�ʼλ�á����½ǡ���������Ϊ�����Ͻǡ�
	Global_GetWinPos(&winPos);	
	winPos.y -=  iKeyBoardHeight;
	
	iScreenWidth = PT_GetLcdWidth();
	
	// ȡ��ѡ��߶�
	iCandidateBoxHeight = pltAbstractObj->m_pfcGetCandidateBoxHeight(pltObj,iCurrentLayoutId,FirstLayer);
	// ����õ���ѡ�����Ͻ�Y����
	winPos.y -= iCandidateBoxHeight;

	// ��ʾͼƬ�����Ͻ�X����
	pStartPoint->x = (GBUINT16)winPos.x + (iScreenWidth - iKeyBoardWidth) / 2;
	// ��ʾͼƬ�����Ͻ�Y����
	pStartPoint->y = (GBUINT16)winPos.y;
}

/*!
 * \brief �������ں�ѡ��ʼ��ʾλ��
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
 * \brief ���ú�ѡ����ʼ��ʾλ��
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
 * \brief �������ABC MultiTap��ʱ���������ʱ����Ӧ����
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
			pltAbstractObj->currentSyllableIndex = 0; // ������������
			Global_SetLayoutWinActiveStatus(GBFalse);
			GBInputBox_SetAllPlatformFuncKey();
			pltAbstractObj->m_pfcChangeHeight(pltAbstractObj, 0);		
			return;
		}
	}
}

//////////////////////////////////////////////////////////////////////////
///>start [static const data]
// ���ķ��š� "�� �� �� �� �� ������
static const GBWCHAR symbols_chn[][GBIME_SYMBOL_MAX_LEN] = 
{
	{ 0xFF1F, 0x0000 }, // ��
	{ 0xFF01, 0x0000 }, // ��
	{ 0xFF0C, 0x0000 }, // ��
	{ 0x3002, 0x0000 }, // ��
	{ 0xFF1A, 0x0000 }, // ��
	{ 0x2026, 0x2026, 0x0000 }, // ����

	{ 0x3001,0x0000 }, //��
	{ 0xff1b,0x0000 }, //��
	{ 0x2014,0x0000 }, //��
	{ 0x201c,0x201d,0x0000 }, //����
	{ 0xff08,0xff09,0x0000 }, //����
};
const GBWCHAR * symbolsTable_1_Chn[] = 
{
	symbols_chn[0],	symbols_chn[1],	symbols_chn[2],	symbols_chn[3],	symbols_chn[4],	symbols_chn[5],
	symbols_chn[6],	symbols_chn[7],	symbols_chn[8],	symbols_chn[9],	symbols_chn[10]
};

// Ӣ�ķ��š�,.?!':����
const GBWCHAR symbols_alp[][GBIME_SYMBOL_MAX_LEN] = 
{
	{ 0x003F, 0x0000 }, // ?
	{ 0x0021, 0x0000 }, // !
	{ 0x002C, 0x0000 }, // ,
	{ 0x002E, 0x0000 }, // .
	{ 0x0027, 0x0000 }, // '
	{ 0x0040,0x0000 }, //@

	{ 0x2026, 0x0000 }, // ��
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

// ������š�:-)  ;-)  :-D :-p  -.-||��
const GBWCHAR faces[][GBIME_FACE_MAX_LEN] =
{
	{ 0x003A, 0x002D, 0x0029, 0x0000 } // :-) ����
	,{ 0x003a,0x002d,0x0028,0x0000} //:-(
	,{ 0x003A, 0x002D, 0x0044, 0x0000 } // :-D ����
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
 * \brief ���ñ����ż�
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
 * \brief ���ñ����Ű���
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
	case LayoutIdSP9JPinYin:		// 9��ƴ��
	case LayoutIdSP26JPinYin:		// 26��ƴ��
	case LayoutIdHP9JPinYin:		// ����9��ƴ��
	case LayoutIdHP26JPinYin:		// ����26��ƴ��

	case LayoutIdWL9JPinYin:
	case LayoutIdWL26JPinYin:
		bChineseFlag = GBTrue;
		break;
	case LayoutIdSP9JLowerEnglish:  // 9��Ӣ��Сд
	case LayoutIdSP26JLowerEnglish: // 26��Ӣ��Сд
	case LayoutIdHP26JEnglish:      // ����26��Ӣ��
	case LayoutIdHP9JEnglish:		// ����9��Ӣ��

	case LayoutIdWL9JEnglish:
	case LayoutIdWL26JEnglish:
		bChineseFlag = GBFalse;
		break;
	default:
		return GBIME_IGNORE; // ����Layout������
	}

	// ����Ӣ�ķ����ַ��������ڴ�
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
	GBEngine_SetEngineOption(Global_GetEnineInstance(), GB_ENGINE_FUNCTION,	GBFO_Multi_Tap_Show_Cand_Wnd, 1); // ��������

	if (bChineseFlag)
	{
		// 1������
		PT_ConvertFaceString(pltAbstractObj->pSymbolChineseString, symbolsTable_1_Chn, iChnSymbolsTableSize);
		GBEngine_RegisterInterpunctionKeyEx(Global_GetEnineInstance(), 
											GBKEY_1, 
											(GBLPCWCHAR)pltAbstractObj->pSymbolChineseString, 
											GBIME_SPLIT_CHAR, 
											GBCL_MULTITAP, 
											GBIME_SYMBOL_1_ROW_NUM, 
											iChnSymbolsTableSize, 
											GBIKO_Input_Status_Confirm_Candidate);

		// ����Layout 0�������س����ո�
		if (IS_KB_LAYOUT(GET_LAYOUT_ID(pltAbstractObj)))
		{			
			GBEngine_RegisterInterpunctionKeyEx(Global_GetEnineInstance(), GBKEY_0, GBIME_MULTITAP_SYMBOLS_0, 0, GBCL_MULTITAP, 1, 2, 0);	
		} 
		else
		{
			// 0������
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
		// 1������
		PT_ConvertFaceString(pltAbstractObj->pSymbolEnglishString, symbolsTable_1_Alp, iAlpSymbolsTableSize);
		GBEngine_RegisterInterpunctionKeyEx(Global_GetEnineInstance(), 
											GBKEY_1, 
											(GBLPCWCHAR)pltAbstractObj->pSymbolEnglishString, 
											GBIME_SPLIT_CHAR, 
											GBCL_MULTITAP, 
											GBIME_SYMBOL_1_ROW_NUM, 
											iAlpSymbolsTableSize, 
											GBIKO_Input_Status_Confirm_Candidate);

		// ����Layout 0�������س����ո�
		if (IS_KB_LAYOUT(GET_LAYOUT_ID(pltAbstractObj)))
		{			
			GBEngine_RegisterInterpunctionKeyEx(Global_GetEnineInstance(), GBKEY_0, GBIME_MULTITAP_SYMBOLS_0, 0, GBCL_MULTITAP, 1, 2, 0);	
		} 
		else // �������0������
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
 * \brief ƽ̨ĳЩ����ģʽ��Ҫ����ĳЩ���ܰ�ť(��: ���ٲ���ģʽ��ֹ��������)
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

	// ����ƽ̨������Ƿ��ڿ��ٲ�������ģʽ�����Ƿ����ĳЩ���ܰ�ť	
	if (GBInputBox_IsQuickSearchInputMode())
	{
		switch (keyboardType)
		{
		case KEYBOARD_VK_NUMPAD:
			CLayoutInfo_SetButtonStatus(pLayoutInfo, GBKEY_VK_V9_PINYIN, ButtonStatusGray);		///<����9��ƴ��
			CLayoutInfo_SetButtonStatus(pLayoutInfo, GBKEY_VK_V9_ENG_LOWER, ButtonStatusGray);	///<����9��СдӢ��
			CLayoutInfo_SetButtonStatus(pLayoutInfo, GBKEY_VK_V9_NUM, ButtonStatusGray);		///<����9������
			CLayoutInfo_SetButtonStatus(pLayoutInfo, GBKEY_VK_V9_SYM, ButtonStatusGray);		///<����9������
			CLayoutInfo_SetButtonStatus(pLayoutInfo, GBKEY_VK_V9_FS_HANDWRITE, ButtonStatusGray);///<����ȫ����д
			CLayoutInfo_SetButtonStatus(pLayoutInfo, GBKEY_VK_BACKWORD, ButtonStatusGray);
			break;
		case KEYBOARD_VK_QWERTY:
			CLayoutInfo_SetButtonStatus(pLayoutInfo, GBKEY_VK_CAP, ButtonStatusGray);           ///<��Сд�л���
			CLayoutInfo_SetButtonStatus(pLayoutInfo, GBKEY_VK_V26_PINYIN, ButtonStatusGray);	///<����9��ƴ��
			CLayoutInfo_SetButtonStatus(pLayoutInfo, GBKEY_VK_V26_ENG_LOWER, ButtonStatusGray);	///<����9��СдӢ��
			CLayoutInfo_SetButtonStatus(pLayoutInfo, GBKEY_VK_V9_NUM, ButtonStatusGray);		///<����9������
			CLayoutInfo_SetButtonStatus(pLayoutInfo, GBKEY_VK_V9_SYM, ButtonStatusGray);		///<����9������
			CLayoutInfo_SetButtonStatus(pLayoutInfo, GBKEY_VK_V9_FS_HANDWRITE, ButtonStatusGray);///<����ȫ����д
			CLayoutInfo_SetButtonStatus(pLayoutInfo, GBKEY_VK_BACKWORD, ButtonStatusGray);
		default: // �������̲�֧��
			break;
		}

		// ��ֹ�����л�
		pltAbstractObj->isSlidingDisabled = GBTrue;
	}
	else
	{
		switch (keyboardType)
		{
		case KEYBOARD_VK_NUMPAD:				
			CLayoutInfo_SetButtonStatus(pLayoutInfo, GBKEY_VK_V9_PINYIN, ButtonStatusNomal);	///<����9��ƴ��
			CLayoutInfo_SetButtonStatus(pLayoutInfo, GBKEY_VK_V9_ENG_LOWER, ButtonStatusNomal);	///<����9��СдӢ��
			CLayoutInfo_SetButtonStatus(pLayoutInfo, GBKEY_VK_V9_NUM, ButtonStatusNomal);		///<����9������
			CLayoutInfo_SetButtonStatus(pLayoutInfo, GBKEY_VK_V9_SYM, ButtonStatusNomal);		///<����9������
			CLayoutInfo_SetButtonStatus(pLayoutInfo, GBKEY_VK_V9_FS_HANDWRITE, ButtonStatusNomal);///<����ȫ����д
			CLayoutInfo_SetButtonStatus(pLayoutInfo, GBKEY_VK_BACKWORD, ButtonStatusNomal);
			break;
		case KEYBOARD_VK_QWERTY:
			CLayoutInfo_SetButtonStatus(pLayoutInfo, GBKEY_VK_CAP, ButtonStatusNomal);          ///<��Сд�л���
			CLayoutInfo_SetButtonStatus(pLayoutInfo, GBKEY_VK_V26_PINYIN, ButtonStatusNomal);	///<����9��ƴ��
			CLayoutInfo_SetButtonStatus(pLayoutInfo, GBKEY_VK_V26_ENG_LOWER, ButtonStatusNomal);///<����9��СдӢ��
			CLayoutInfo_SetButtonStatus(pLayoutInfo, GBKEY_VK_V9_NUM, ButtonStatusNomal);		///<����9������
			CLayoutInfo_SetButtonStatus(pLayoutInfo, GBKEY_VK_V9_SYM, ButtonStatusNomal);		///<����9������
			CLayoutInfo_SetButtonStatus(pLayoutInfo, GBKEY_VK_V9_FS_HANDWRITE, ButtonStatusNomal);///<����ȫ����д
			CLayoutInfo_SetButtonStatus(pLayoutInfo, GBKEY_VK_BACKWORD, ButtonStatusNomal);
		default: // �������̲�֧��
			break;
		}

		// �������л�
		pltAbstractObj->isSlidingDisabled = GBFalse;
	}
}