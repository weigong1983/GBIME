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
* Purpose      :  抽象Layout结构体
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

// 长按时间间隔
#define GBIME_LONG_PRESS_TIME	500

/*!
 * \brief Layout刷新标记位定义
 */
enum  GBIME_LAYOUT_REFRESH_FLAG{
	GBIME_LAYOUT_REFRESH_NONE					= 0x0000 // 不做刷新
	,GBIME_LAYOUT_REFRESH_KEYBOARD_BUTTON		= 0x0001 // 刷新键盘上的某个按钮
	,GBIME_LAYOUT_REFRESH_KEYBOARD				= 0x0002 // 刷新整个键盘
	,GBIME_LAYOUT_REFRESH_CANDIDATE_LINE		= 0x0004 // 刷新候选栏(点中候选时)
	,GBIME_LAYOUT_REFRESH_SYLLABLE_LINE			= 0x0008 // 刷新音节栏(点中音节时)
	,GBIME_LAYOUT_REFRESH_ALL					= 0xFFFF // 全部刷新
};

// 当前Layout ID
#define GET_LAYOUT_ID(pCLayoutAbstract)     ((pCLayoutAbstract)->m_ltData.pltcData->ltID)
// 当前要显示的层
#define GET_LAYOUT_CURRENT_LAYER(pCLayoutAbstract)  ((pCLayoutAbstract)->m_ltData.ltCurLayer)
// 获取指定层、指定索引键值
#define GET_BUTTON_KEY_VALUE(pCLayoutAbstract, layer, idx)  ((pCLayoutAbstract)->m_ltData.pltLayerData[layer].plycBtData[idx].btKey)
// 判断是否虚拟键盘下需要特殊处理的物理功能按键(四向键、左右功能键、*、#)
//#define IS_VK_SHOULD_HANDLE_PHYSIC_FUNC_KEY(key)	\
//					((key) == GBKEY_LEFT || (key) == GBKEY_RIGHT || (key) == GBKEY_UP || (key) == GBKEY_DOWN \
//					|| (key) == GBKEY_OK || (key) == GBKEY_BACK \
//					|| (key) == GBKEY_AMB_SPECIAL_A \
//					|| (key) == GBKEY_AMB_SPECIAL_B)
#define IS_VK_SHOULD_HANDLE_PHYSIC_FUNC_KEY(key)	((key) == GBKEY_OK || (key) == GBKEY_BACK)

/*!
 * \brief 滑屏操作相关
 */
#define GBIME_SLIDING_MINIMUM_PIXELS			1		///<滑动操作识别精度控制
#define GBIME_SLIDING_ACTION_SPEED_REQUIRED		128		///<产生滑动操作的速度要求[像素/s]
#define GBIME_SLIDING_ACTION_DISTANCE_REQUIRED	30		///<产生滑动操作的距离要求

typedef enum{
	GBIME_SLIDING_INIT				// 初始状态
	,GBIME_SLIDING_MOVING			// 移动检测状态
	,GBIME_SLIDING_EXCUTE_ACTION	// 执行滑屏动作状态
}GBIME_SLIDING_STATUS;

/*!
 * \brief MultiTap符号相关
 */
#define GBIME_MULTITAP_TIMER_INTERVAL			1000	///<Multiap上屏时间间隔
#define GBIME_MULTITAP_FACE_NUM					5		///<Multiap表情符号最大个数
#define GBIME_MULTITAP_FACE_LEN					8		///<Multiap表情符号最大长度
#define GBIME_MULTITAP_FACE_SPLIT_CHAR			' '	    ///<Multiap表情符号分隔符

typedef struct _tagSlidingOperation
{
	GBIMEPoint				startPos;					///<初始位置	
	GBIMEPoint				lastMovePos;				///<触摸笔上次移动位置
	GBIME_SLIDING_STATUS    slidingStatus;				///<滑屏状态量
	GBU32					slidingStartTime;			///<滑动开始时间(系统启动的毫秒数)
	GBU32					slidingElapsedTime;			///<滑动总耗时
	GBSlidingDirection		slidingDirection;			///<滑动方向
	GBINT					slidingSpeed;				///<滑动速度
	GBINT					slidingDistance;			///<滑动距离
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
 * \brief 功能函数原型定义
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
	 * \brief Layout 基础变量
	 */
	GBUINT16						m_ltType;///<layout type
	LayoutInfo						m_ltData;///<layout UI data point
	GBLPCVOID						m_pltIMData;///<layout IM data point
	PCLayoutViewAbstract			m_hltView;///<layout视图句柄, 由子类负责实例化赋值
	GBINT							lastUseTime;///<Layout对象上次使用时间

	/*!
	 * \brief Layout滑屏操作成员变量
	 */
	SlidingOperation				slidingOperation;
	GBBOOL							isSlidingDisabled; ///<滑屏切换禁用标记[default: enable]

	/*!
	 * \brief Layout 刷新显示及消息处理相关变量
	 */
	GBUINT							m_refreshFlag;///<Layout刷新标记(避免无效刷新，提高刷新速度)
	GBIMEPoint						syllableDisplayStartPos;///<音节栏起始显示位置	
	GBIMEPoint						candidateDisplayStartPos;///<候选栏起始显示位置	
	GBUINT16						pressLayoutKeyValue;///<记录按下时的Layout按键值(只有当按下和弹起键值一致时才执行按键操作)
	GBUINT16						currentLayoutKeyValue;///<当前点中的按键对应键值
	GBUINT16						currentSyllableIndex;///<当前选中的音节索引
	GBUINT16						currentCandidateIndex;///<当前选中的候选索引
	PenPosition						penDownPostion;///<触摸笔按下的位置
	PenPosition						penInPostion;///<当前点中的位置
	ButtonStatusEnum				buttonStatus;///<按钮状态
	GBWCHAR							*pSymbolChineseString;///<中文标点符号字符串(动态申请)
	GBWCHAR							*pSymbolEnglishString;///<英文标点符号字符串(动态申请)
	GBWCHAR							*pFaceString;///<表情符号字符串(动态申请)
	GBBOOL							bLongPressFlag;///<长按键处理标记

	/*!
	 * \brief Multitap符号功能相关变量
	 */
	GBBOOL							bMultitapTimerStartFlag;///<MultiTap定时器启动标记
	GBBOOL							bMultitapSymbolStartFlag;///<MultiTap符号启动标记
	GBBOOL							bMultitapFaceStartFlag;///<MultiTap表情启动标记
	GBINT							multitapReplaceCandLength;///<高亮替换候选字符长度
	GBUINT16						multitapCandIndex;///<Multitap候选符号当前索引

	/*!
	 * \brief GBV5引擎交互相关变量
	 */
	GBBOOL							bSendEngineKeyEvent;///<是否需要发送引擎按键事件 
	GBINT							engineKeyEvent;///<经过Layout预处理之后转换后的引擎按键事件
	GBINT							engineEventParam;///<经过Layout预处理之后转换后的引擎事件参数
	GBBOOL							bUpToInputKeySequence;///<数字上屏标记

	/*!
	 * \brief 成员函数指针
	 */
//	PFCLt__Init         m_pfcInit;
	PFCLt__InitVariable				m_pfcInitVariable;
	PFCLt__Release					m_pfcRelease;
	PFCLt__ReleaseBase				m_pfcReleaseBase;
	PFCLt__SetRefreshFlag			m_pfcSetRefreshFlag;
	PFCLt__Paint					m_pfcPaint;
	PFCLt__PreHandleEvent			m_pfcPreHandleEvent;///<消息预处理函数：转换成框架对应的一组消息；设置是否需要发送相应的按键消息给国笔引擎处理的标记并记录按键值(实际处理消息时用到)
	PFCLt__PenPositionInKeyboardButton m_pfcPenPositionInKeyboardButton;///<触摸点是否落在键盘按钮上
	PFCLt__HitTest					m_pfcHitTest;///<触摸点击检测，判断触摸点落在Layout的位置
	PFCLt__UpdateLayoutInformation  m_pfcUpdateLayoutInformation;///<更新Layout信息
	//PFCLt__DispatchEvent m_pfcDispatchEvent;///<消息分发函数
	PFCLt__HandleEvent				m_pfcHandleEvent;///<消息处理函数
	PFCLt__HandleEvent				m_pfcHandleCommonEvent;///<公共消息处理函数
	PFCLt__SetInterpunctionKey		m_pfcSetInterpunctionKey;///<设置标点符号键
	PFCLt__SetConfig				m_pfcSetConfig;
	PFCLt__SetIM					m_pfcSetIM;
	PFCLt__Listenner				m_pfcListenner;
	PFCLt__IsMatchLtObjAndLtID		m_pfcIsMatchLtObjAndLtID;
	PFCLt__IsNeedLeave				m_pfcIsNeedLeave;	

	// 平台输入框交互函数
	PFCLt__ChangeHeight				m_pfcChangeHeight;
	PFCLt__UpScreen					m_pfcUpScreen;
	PFCLt__Redraw					m_pfcRedraw;
	
	// 获取Layout信息 
	PFCLt__GetLayoutHeight			m_pfcGetLayoutHeight;
	PFCLt__GetLayoutWidth			m_pfcGetLayoutWidth;
	PFCLt__GetBottonRectByKey		m_pfcGetBottonRectByKey;
	PFCLt__GetLayerConstInfo		m_pfcGetLayerConstInfo;
	PFCLt__GetLayerConstButtons		m_pfcGetLayerConstButtons;
	PFCLt__GetLayoutItemInfo		m_pfcGetCandidateBoxHeight;
	PFCLt__GetWinPos				m_pfcGetWinPos;
	PFCLt__GetLayoutWinRect			m_pfcGetLayoutWinRect;
	PFCLt__GetLayoutStartPoint		m_pfcGetLayoutStartPoint;

	// 设置Layout显示信息
	PFCLt__SetSyllableDisplayStartPos  m_pfcSetSyllableDisplayStartPos;///<设置音节候选起始显示位置
	PFCLt__SetCandidateDisplayStartPos m_pfcSetCandidateDisplayStartPos;///<设置候选字起始显示位置

	// 功能函数
	PFCLt__SwitchHandler			m_pfcSwitchHandler;
	PFCLt__FuncKeyHandler			m_pfcFuncKeyHandler;
	PFCLt__PhysicFuncKeyHandler		m_pfcPhysicFuncKeyHandler;///<物理功能按键处理函数
	PFCLt__SwitchPreLayoutObj		m_pfcSwitchPreLayoutObj;
	PFCLt__ShouldSendMsgToEngine	m_pfcShouldSendMsgToEngine;
	PFCLt__IsInputKey				m_pfcIsInputKey;
	PFCLt__LongPressKeyHandler		m_pfcLongPressKeyHandler;
	PFCLt__ShowCandWin				m_pfcShowCandWin;
	PFCLt__ShowAssociateWin			m_pfcShowAssociateWin;
	PFCLt__CloseCandWind			m_pfcCloseCandWin;
	PFCLt__CheckUpScreen			m_pfcCheckUpScreen;
	PFCLt__DisableFuncKeyHandler	m_pfcDisableFuncKeyHandler;

	// 滑屏处理函数
	PFCLt__SlidingHandler			m_pfcSlidingPenDownHandler;
	PFCLt__SlidingHandler			m_pfcSlidingPenUpHandler;
	PFCLt__SlidingHandler			m_pfcSlidingPenMoveHandler;

	// MultiTap符号功能函数
	PFCLt__MultiTapSymbolHandler	m_pfcMultiTapSymbolHandler;
	PFCLt__MultiTapFaceHandler		m_pfcMultiTapFaceHandler;
	PFCLt__OnMultitapTimer			m_pfcOnMultitapTimer;

	// 切换大小写状态函数
	PFCLt__SwitchShiftCapStatus		m_pfcSwitchShiftCapStatus;
	PFCLt__SyncEngineShiftCapStatus	m_pfcSyncEngineShiftCapStatus;
	PFCLt__KBMultiTapTimerCallBack	m_pfcKBMultiTapTimerCallBack;
}CLayoutAbstract, *PCLayoutAbstract;

CLayoutAbstract * CLayoutAbstract__Construct(PCLayoutAbstract pltDest, 
											 GBLPCVOID pcltUIData,
											 GBLPCVOID pcltIMData,
											 GBLPCVOID hcltImage);

 
#endif //__CLASS_LAYOUT_ABSTRACT_


