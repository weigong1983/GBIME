/***************************************************************************
* File Name    :  CLayoutAbstract.h
* Corporation  :  Guobi Technology Ltd. GuangZhou China
* Copyright    :  Copyright Guobi Technology Ltd. And Subject to Confidentiality 
*                 Restrictions. This document is the confidential and proprietary 
*                 property of Guobi Technology Ltd.It is neither the document                  
*                 itself nor any part of it can be shared with any party  without 
*                 the written  permission. 
* Author       :  haj
* Date         :  2010-06-08
* Purpose      :  ����Layout�ṹ��
* Version      :  V1.0
* Last Changed List
* Name         |   date   |  Discription
****************************************************************************/

#ifndef __CLASS_LAYOUT_ABSTRACT_
#define __CLASS_LAYOUT_ABSTRACT_
#include "PlatformTools.h"
#include "PlatformInputBox.h"
#include "GBIMEUtils.h"
#include "gbtype.h"
#include "gbdef.h"
#include "gbft_def.h"
#include "CLayoutInfo.h"
#include "CLayoutViewAbstract.h"
#include "CLayoutControl.h"
#include "CGBEngine.h"

// ����ʱ����
#define GBIME_LONG_PRESS_TIME	500

/*!
 * \brief Layoutˢ�±��λ����
 */
enum  GBIME_LAYOUT_REFRESH_FLAG{
	GBIME_LAYOUT_REFRESH_NONE					= 0x0000 // ����ˢ��
	,GBIME_LAYOUT_REFRESH_KEYBOARD_BUTTON		= 0x0001 // ˢ�¼����ϵ�ĳ����ť
	,GBIME_LAYOUT_REFRESH_KEYBOARD				= 0x0002 // ˢ����������
	,GBIME_LAYOUT_REFRESH_CANDIDATE_LINE		= 0x0004 // ˢ�º�ѡ��(���к�ѡʱ)
	,GBIME_LAYOUT_REFRESH_SYLLABLE_LINE			= 0x0008 // ˢ��������(��������ʱ)
	,GBIME_LAYOUT_REFRESH_ALL					= 0xFFFF // ȫ��ˢ��
};

// ��ǰLayout ID
#define GET_LAYOUT_ID(pCLayoutAbstract)     ((pCLayoutAbstract)->m_ltData.pltcData->ltID)
// ��ǰҪ��ʾ�Ĳ�
#define GET_LAYOUT_CURRENT_LAYER(pCLayoutAbstract)  ((pCLayoutAbstract)->m_ltData.ltCurLayer)
// ��ȡָ���㡢ָ��������ֵ
#define GET_BUTTON_KEY_VALUE(pCLayoutAbstract, layer, idx)  ((pCLayoutAbstract)->m_ltData.pltLayerData[layer].plycBtData[idx].btKey)
// �ж��Ƿ������������Ҫ���⴦��������ܰ���(����������ҹ��ܼ���*��#)
//#define IS_VK_SHOULD_HANDLE_PHYSIC_FUNC_KEY(key)	\
//					((key) == GBKEY_LEFT || (key) == GBKEY_RIGHT || (key) == GBKEY_UP || (key) == GBKEY_DOWN \
//					|| (key) == GBKEY_OK || (key) == GBKEY_BACK \
//					|| (key) == GBKEY_AMB_SPECIAL_A \
//					|| (key) == GBKEY_AMB_SPECIAL_B)
#define IS_VK_SHOULD_HANDLE_PHYSIC_FUNC_KEY(key)	((key) == GBKEY_OK || (key) == GBKEY_BACK)

/*!
 * \brief �����������
 */
#define GBIME_SLIDING_MINIMUM_PIXELS			1		///<��������ʶ�𾫶ȿ���
#define GBIME_SLIDING_ACTION_SPEED_REQUIRED		128		///<���������������ٶ�Ҫ��[����/s]
#define GBIME_SLIDING_ACTION_DISTANCE_REQUIRED	30		///<�������������ľ���Ҫ��

typedef enum{
	GBIME_SLIDING_INIT				// ��ʼ״̬
	,GBIME_SLIDING_MOVING			// �ƶ����״̬
	,GBIME_SLIDING_EXCUTE_ACTION	// ִ�л�������״̬
}GBIME_SLIDING_STATUS;

/*!
 * \brief MultiTap�������
 */
#define GBIME_MULTITAP_TIMER_INTERVAL			1000	///<Multiap����ʱ����
#define GBIME_MULTITAP_FACE_NUM					5		///<Multiap�������������
#define GBIME_MULTITAP_FACE_LEN					8		///<Multiap���������󳤶�
#define GBIME_MULTITAP_FACE_SPLIT_CHAR			' '	    ///<Multiap������ŷָ���

typedef struct _tagSlidingOperation
{
	GBIMEPoint				startPos;					///<��ʼλ��	
	GBIMEPoint				lastMovePos;				///<�������ϴ��ƶ�λ��
	GBIME_SLIDING_STATUS    slidingStatus;				///<����״̬��
	GBU32					slidingStartTime;			///<������ʼʱ��(ϵͳ�����ĺ�����)
	GBU32					slidingElapsedTime;			///<�����ܺ�ʱ
	GBSlidingDirection		slidingDirection;			///<��������
	GBINT					slidingSpeed;				///<�����ٶ�
	GBINT					slidingDistance;			///<��������
}SlidingOperation, *PSlidingOperation;

//typedef GBINT32 (* PFCLt__Init)(PCLayoutAbstract);
typedef void		(* PFCLt__InitVariable)(GBLPVOID pltObj);
typedef GBIMEReturn (* PFCLt__Release)(GBLPVOID pltObj, PLayoutInfo pltUIData);
typedef GBIMEReturn (* PFCLt__ReleaseBase)(GBLPVOID pltObj, PLayoutInfo pltUIData);
typedef GBIMEReturn (* PFCLt__SetRefreshFlag)(GBLPVOID pltObj, GBUINT refreshFlag);
typedef GBIMEReturn (* PFCLt__Paint)(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo);
typedef GBIMEReturn (* PFCLt__HandleEvent)(GBLPVOID pltObj, PGBIMEEvent pIMEEvent, PEngineOutputInfo pEngineOutputInfo);
typedef GBIMEReturn (* PFCLt__SetInterpunctionKey)(GBLPVOID pltObj);
typedef GBIMEReturn (* PFCLt__SetConfig)(void);
typedef GBIMEReturn (* PFCLt__SetIM)(void);
typedef GBIMEReturn (* PFCLt__Listenner)(void);
typedef GBIMEReturn (* PFCLt__IsMatchLtObjAndLtID)(GBLPVOID pltDest, GBUINT16 ltID);
typedef GBIMEReturn (* PFCLt__IsNeedLeave)(GBLPVOID pltDest);
typedef GBIMEReturn (* PFCLt__PreHandleEvent)(GBLPVOID pltObj, GBINT nPlatformEvent, GBINT wParam, GBINT lParam, PEngineOutputInfo pEngineOutputInfo);
typedef PenPosition	(* PFCLt__PenPositionInKeyboardButton)(GBLPVOID pltObj, GBIMEPoint hitPoint, ButtonStatusEnum buttonStatus, PEngineOutputInfo pEngineOutputInfo);
typedef PenPosition (* PFCLt__HitTest)(GBLPVOID pltObj, GBIMEPoint hitPoint, ButtonStatusEnum buttonStatus, PEngineOutputInfo pEngineOutputInfo);
typedef void		(* PFCLt__UpdateLayoutInformation)(GBLPVOID pltObj, ButtonStatusEnum buttonStatus, PEngineOutputInfo pEngineOutputInfo);
typedef GBIMEReturn (* PFCLt__ChangeHeight)(GBLPVOID pltObj, GBINT newHeight);
typedef GBIMEReturn (* PFCLt__UpScreen)(GBLPVOID pltObj, GBPCWCHAR pCand);
typedef GBIMEReturn (* PFCLt__Redraw)(GBLPVOID pltObj);
typedef GBIMEReturn	(* PFCLt__HandleMessageCallback)(PGBIMEEvent pIMEEvent);
//typedef GBIMEReturn (* PFCLt__DispatchEvent)(GBLPVOID pltObj, PFCLt__HandleMessageCallback fpCallback);
typedef GBUINT16	(* PFCLt__GetLayoutHeight)(GBLPVOID pltObj, LayoutIdEnum layoutId);
typedef GBUINT16	(* PFCLt__GetLayoutWidth)(GBLPVOID pltObj, LayoutIdEnum layoutId);
typedef GBBOOL	(* PFCLt__GetBottonRectByKey)(GBLPVOID pltObj, LayoutIdEnum layoutId, 
											  LayerIndex layerId,GBUINT16 iKeyValue,PGBRECT pRect);
typedef PLAYOUTCONSTLAYER (* PFCLt__GetLayerConstInfo)(GBLPVOID pltObj, LayoutIdEnum layoutId, LayerIndex layerId);
typedef PBUTTONCONSTDATA (* PFCLt__GetLayerConstButtons)(GBLPVOID pltObj, LayoutIdEnum layoutId, LayerIndex layerId);
typedef GBINT16 (* PFCLt__GetLayoutItemInfo)(GBLPVOID pltObj, LayoutIdEnum layoutId, LayerIndex layerId);
typedef void (* PFCLt__GetWinPos)(GBLPVOID pltObj, PGBIMEPoint pWinPos);
typedef void (* PFCLt__GetLayoutWinRect)(GBLPVOID pltObj, PGBRECT pLayoutWinRect);
typedef void (* PFCLt__GetLayoutStartPoint)(GBLPVOID pltObj, PGBIMEPoint pStartPoint);
typedef void (* PFCLt__SetCandidateDisplayStartPos)(GBLPVOID pltObj, GBINT16 x, GBINT16 y);
typedef void (* PFCLt__SetSyllableDisplayStartPos)(GBLPVOID pltObj, GBINT16 x, GBINT16 y);

typedef GBIMEReturn (* PFCLt__SlidingHandler)(GBLPVOID pltObj, GBIMEPoint pt);
typedef GBIMEReturn (* PFCLt__LongPressKeyHandler)(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo);

/*!
 * \brief ���ܺ���ԭ�Ͷ���
 */
typedef GBIMEReturn	(* PFCLt__SwitchHandler)(GBLPVOID pltObj);
typedef GBIMEReturn	(* PFCLt__FuncKeyHandler)(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo);
typedef GBIMEReturn (* PFCLt__PhysicFuncKeyHandler)(GBLPVOID pltObj, GBINT nPlatformEvent, GBINT wParam, GBINT lParam, PEngineOutputInfo pEngineOutputInfo);
typedef GBIMEReturn	(* PFCLt__SwitchPreLayoutObj)(GBLPVOID pltObj);
typedef GBBOOL (* PFCLt__ShouldSendMsgToEngine)(GBLPVOID pltObj, ButtonStatusEnum buttonStatus);
typedef GBBOOL (* PFCLt__IsInputKey)(GBUINT16 keyValue);
typedef GBIMEReturn	(* PFCLt__ShowCandWin)(GBLPVOID pltObj, ButtonStatusEnum buttonStatus, PEngineOutputInfo pEngineOutputInfo);
typedef GBIMEReturn	(* PFCLt__ShowAssociateWin)(GBLPVOID pltObj, ButtonStatusEnum buttonStatus, PEngineOutputInfo pEngineOutputInfo);
typedef GBIMEReturn	(* PFCLt__CloseCandWind)(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo);
typedef GBIMEReturn	(* PFCLt__CheckUpScreen)(GBLPVOID pltObj, ButtonStatusEnum buttonStatus, PEngineOutputInfo pEngineOutputInfo);
typedef GBIMEReturn	(* PFCLt__MultiTapSymbolHandler)(GBLPVOID pltObj);
typedef GBIMEReturn	(* PFCLt__MultiTapFaceHandler)(GBLPVOID pltObj);
typedef void		(* PFCLt__OnMultitapTimer)(void);
typedef GBIMEReturn	(* PFCLt__SwitchShiftCapStatus)(GBLPVOID pltObj, GBShiftCapStatus shiftCapStatus, GBBOOL bAuto,	PEngineOutputInfo pEngineOutputInfo);
typedef GBIMEReturn	(* PFCLt__SyncEngineShiftCapStatus)(GBLPVOID pltObj);
typedef void (* PFCLt__KBMultiTapTimerCallBack)(void);
typedef GBIMEReturn	(* PFCLt__DisableFuncKeyHandler)(GBLPVOID pltObj);

typedef struct _tag_CLayoutAbstract {
	/*!
	 * \brief Layout ��������
	 */
	GBUINT16						m_ltType;///<layout type
	LayoutInfo						m_ltData;///<layout UI data point
	GBLPCVOID						m_pltIMData;///<layout IM data point
	PCLayoutViewAbstract			m_hltView;///<layout��ͼ���, �����ฺ��ʵ������ֵ
	GBINT							lastUseTime;///<Layout�����ϴ�ʹ��ʱ��

	/*!
	 * \brief Layout����������Ա����
	 */
	SlidingOperation				slidingOperation;
	GBBOOL							isSlidingDisabled; ///<�����л����ñ��[default: enable]

	/*!
	 * \brief Layout ˢ����ʾ����Ϣ������ر���
	 */
	GBUINT							m_refreshFlag;///<Layoutˢ�±��(������Чˢ�£����ˢ���ٶ�)
	GBIMEPoint						syllableDisplayStartPos;///<��������ʼ��ʾλ��	
	GBIMEPoint						candidateDisplayStartPos;///<��ѡ����ʼ��ʾλ��	
	GBUINT16						pressLayoutKeyValue;///<��¼����ʱ��Layout����ֵ(ֻ�е����º͵����ֵһ��ʱ��ִ�а�������)
	GBUINT16						currentLayoutKeyValue;///<��ǰ���еİ�����Ӧ��ֵ
	GBUINT16						currentSyllableIndex;///<��ǰѡ�е���������
	GBUINT16						currentCandidateIndex;///<��ǰѡ�еĺ�ѡ����
	PenPosition						penDownPostion;///<�����ʰ��µ�λ��
	PenPosition						penInPostion;///<��ǰ���е�λ��
	ButtonStatusEnum				buttonStatus;///<��ť״̬
	GBWCHAR							*pSymbolChineseString;///<���ı������ַ���(��̬����)
	GBWCHAR							*pSymbolEnglishString;///<Ӣ�ı������ַ���(��̬����)
	GBWCHAR							*pFaceString;///<��������ַ���(��̬����)
	GBBOOL							bLongPressFlag;///<������������

	/*!
	 * \brief Multitap���Ź�����ر���
	 */
	GBBOOL							bMultitapTimerStartFlag;///<MultiTap��ʱ���������
	GBBOOL							bMultitapSymbolStartFlag;///<MultiTap�����������
	GBBOOL							bMultitapFaceStartFlag;///<MultiTap�����������
	GBINT							multitapReplaceCandLength;///<�����滻��ѡ�ַ�����
	GBUINT16						multitapCandIndex;///<Multitap��ѡ���ŵ�ǰ����

	/*!
	 * \brief GBV5���潻����ر���
	 */
	GBBOOL							bSendEngineKeyEvent;///<�Ƿ���Ҫ�������水���¼� 
	GBINT							engineKeyEvent;///<����LayoutԤ����֮��ת��������水���¼�
	GBINT							engineEventParam;///<����LayoutԤ����֮��ת����������¼�����
	GBBOOL							bUpToInputKeySequence;///<�����������

	/*!
	 * \brief ��Ա����ָ��
	 */
//	PFCLt__Init         m_pfcInit;
	PFCLt__InitVariable				m_pfcInitVariable;
	PFCLt__Release					m_pfcRelease;
	PFCLt__ReleaseBase				m_pfcReleaseBase;
	PFCLt__SetRefreshFlag			m_pfcSetRefreshFlag;
	PFCLt__Paint					m_pfcPaint;
	PFCLt__PreHandleEvent			m_pfcPreHandleEvent;///<��ϢԤ��������ת���ɿ�ܶ�Ӧ��һ����Ϣ�������Ƿ���Ҫ������Ӧ�İ�����Ϣ���������洦��ı�ǲ���¼����ֵ(ʵ�ʴ�����Ϣʱ�õ�)
	PFCLt__PenPositionInKeyboardButton m_pfcPenPositionInKeyboardButton;///<�������Ƿ����ڼ��̰�ť��
	PFCLt__HitTest					m_pfcHitTest;///<���������⣬�жϴ���������Layout��λ��
	PFCLt__UpdateLayoutInformation  m_pfcUpdateLayoutInformation;///<����Layout��Ϣ
	//PFCLt__DispatchEvent m_pfcDispatchEvent;///<��Ϣ�ַ�����
	PFCLt__HandleEvent				m_pfcHandleEvent;///<��Ϣ������
	PFCLt__HandleEvent				m_pfcHandleCommonEvent;///<������Ϣ������
	PFCLt__SetInterpunctionKey		m_pfcSetInterpunctionKey;///<���ñ����ż�
	PFCLt__SetConfig				m_pfcSetConfig;
	PFCLt__SetIM					m_pfcSetIM;
	PFCLt__Listenner				m_pfcListenner;
	PFCLt__IsMatchLtObjAndLtID		m_pfcIsMatchLtObjAndLtID;
	PFCLt__IsNeedLeave				m_pfcIsNeedLeave;	

	// ƽ̨����򽻻�����
	PFCLt__ChangeHeight				m_pfcChangeHeight;
	PFCLt__UpScreen					m_pfcUpScreen;
	PFCLt__Redraw					m_pfcRedraw;
	
	// ��ȡLayout��Ϣ 
	PFCLt__GetLayoutHeight			m_pfcGetLayoutHeight;
	PFCLt__GetLayoutWidth			m_pfcGetLayoutWidth;
	PFCLt__GetBottonRectByKey		m_pfcGetBottonRectByKey;
	PFCLt__GetLayerConstInfo		m_pfcGetLayerConstInfo;
	PFCLt__GetLayerConstButtons		m_pfcGetLayerConstButtons;
	PFCLt__GetLayoutItemInfo		m_pfcGetCandidateBoxHeight;
	PFCLt__GetWinPos				m_pfcGetWinPos;
	PFCLt__GetLayoutWinRect			m_pfcGetLayoutWinRect;
	PFCLt__GetLayoutStartPoint		m_pfcGetLayoutStartPoint;

	// ����Layout��ʾ��Ϣ
	PFCLt__SetSyllableDisplayStartPos  m_pfcSetSyllableDisplayStartPos;///<�������ں�ѡ��ʼ��ʾλ��
	PFCLt__SetCandidateDisplayStartPos m_pfcSetCandidateDisplayStartPos;///<���ú�ѡ����ʼ��ʾλ��

	// ���ܺ���
	PFCLt__SwitchHandler			m_pfcSwitchHandler;
	PFCLt__FuncKeyHandler			m_pfcFuncKeyHandler;
	PFCLt__PhysicFuncKeyHandler		m_pfcPhysicFuncKeyHandler;///<�����ܰ���������
	PFCLt__SwitchPreLayoutObj		m_pfcSwitchPreLayoutObj;
	PFCLt__ShouldSendMsgToEngine	m_pfcShouldSendMsgToEngine;
	PFCLt__IsInputKey				m_pfcIsInputKey;
	PFCLt__LongPressKeyHandler		m_pfcLongPressKeyHandler;
	PFCLt__ShowCandWin				m_pfcShowCandWin;
	PFCLt__ShowAssociateWin			m_pfcShowAssociateWin;
	PFCLt__CloseCandWind			m_pfcCloseCandWin;
	PFCLt__CheckUpScreen			m_pfcCheckUpScreen;
	PFCLt__DisableFuncKeyHandler	m_pfcDisableFuncKeyHandler;

	// ����������
	PFCLt__SlidingHandler			m_pfcSlidingPenDownHandler;
	PFCLt__SlidingHandler			m_pfcSlidingPenUpHandler;
	PFCLt__SlidingHandler			m_pfcSlidingPenMoveHandler;

	// MultiTap���Ź��ܺ���
	PFCLt__MultiTapSymbolHandler	m_pfcMultiTapSymbolHandler;
	PFCLt__MultiTapFaceHandler		m_pfcMultiTapFaceHandler;
	PFCLt__OnMultitapTimer			m_pfcOnMultitapTimer;

	// �л���Сд״̬����
	PFCLt__SwitchShiftCapStatus		m_pfcSwitchShiftCapStatus;
	PFCLt__SyncEngineShiftCapStatus	m_pfcSyncEngineShiftCapStatus;
	PFCLt__KBMultiTapTimerCallBack	m_pfcKBMultiTapTimerCallBack;
}CLayoutAbstract, *PCLayoutAbstract;

CLayoutAbstract * CLayoutAbstract__Construct(PCLayoutAbstract pltDest, 
											 GBLPCVOID pcltUIData,
											 GBLPCVOID pcltIMData,
											 GBLPCVOID hcltImage);

 
#endif //__CLASS_LAYOUT_ABSTRACT_


