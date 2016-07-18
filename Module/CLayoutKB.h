/***************************************************************************
* File Name    :  CLayoutKB9PinYin.h
* Corporation  :  Guobi Technology Ltd. GuangZhou China
* Copyright    :  Copyright Guobi Technology Ltd. And Subject to Confidentiality 
*                 Restrictions. This document is the confidential and proprietary 
*                 property of Guobi Technology Ltd.It is neither the document                  
*                 itself nor any part of it can be shared with any party  without 
*                 the written  permission. 
* Author       :  haj
* Date         :  2010-06-08
* Purpose      :  物理九键拼音Layout结构体
* Version      :  V1.0
* Last Changed List
* Name         |   date   |  Discription
****************************************************************************/
#if defined(__GBIME_KB__) // 国笔物理键盘

#ifndef __CLASS_LAYOUT_KEYBOARD_NINE_PINYIN_
#define __CLASS_LAYOUT_KEYBOARD_NINE_PINYIN_
#include "CLayoutAbstract.h"

#include "PlatformAdapter.h"
#include "PlatformTools.h"
#include "CLayoutInfo.h"
#include "GB_Global.h"
#include "CSwitchInputMethod.h"
#include "CSwitchLayout.h"
/*!
 * \Layout this指针和Layout 基类指针转换
 */
#undef CLASE_THIS_POITER
#define CLASE_THIS_POITER(CLASS_NAME, pltObj)   \
	CLASS_NAME * pltThis = (CLASS_NAME *)pltObj; \
	PCLayoutAbstract pltBase = &pltThis->m_ltParent;


typedef void (* PFCLt__SetSyllableRegion)(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo); 
typedef void (* PFCLt__SetOutputStringRegion)(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo); 
typedef void (* PFCLt__SetCandidateRegion)(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo);
typedef void (* PFCLT__SetSymbolCandRegion)(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo);
typedef void (* PFCLt__SetSyllablePageButtonTouchRegion)(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo); 
typedef void (* PFCLt__SetCandidatePageButtonTouchRegion)(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo);
typedef void (* PFCLt__UpdateLayoutInformation)(GBLPVOID pltObj, ButtonStatusEnum buttonStatus,	PEngineOutputInfo pEngineOutputInfo);

#define PAGE_DOWN_BUTTON_INDEX	0
#define PAGE_Up_BUTTON_INDEX	1

typedef struct _tag_CLayoutKB {
	CLayoutAbstract            m_ltParent;  ///<CLayoutKB9PinYin parent class
	GBIMETouchRegion							syllableRegion;///<音节栏触摸区域
	GBIMETouchRegion							candidateRegion;///<候选栏触摸区域
	GBRECT										candidatePageDownOrUpRectArray[2];///<候选翻页对应的矩形区数组 
	GBRECT										syllablePageDownOrUpRectArray[2];///<音节翻页对应的矩形区数组

	PFCLt__SetSyllableRegion					m_pfcSetSyllableRegion; // 设置音节触摸区
	PFCLt__SetOutputStringRegion				m_pfcSetOutputStringRegion; // 设置输出串触摸区
	PFCLt__SetCandidateRegion					m_pfcSetCandidateRegion; // 设置候选触摸区
	PFCLT__SetSymbolCandRegion					m_pfcSetSymbolCandRegion;//设置标点显示区域
	PFCLt__SetSyllablePageButtonTouchRegion		m_pfcSetSyllablePageButtonRegion; // 设置音节翻页按钮触摸区
	PFCLt__SetCandidatePageButtonTouchRegion	m_pfcSetCandidatePageButtonRegion; // 设置候选翻页按钮触摸区	
	PFCLt__UpdateLayoutInformation				m_pfcUpdateLayoutInformation;//更新layout信息
} CLayoutKB, * PCLayoutKB;

CLayoutAbstract * CLayoutKB__Construct(PCLayoutKB pltKB, 
									   GBLPCVOID pltUIData,
									   GBLPCVOID pltIMData,
									   GBLPCVOID pltConfigData);
#endif //__CLASS_LAYOUT_KEYBOARD_NINE_PINYIN_

#endif // 国笔物理键盘