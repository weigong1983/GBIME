/***************************************************************************
* File Name    :  CLayoutVK.h
* Corporation  :  Guobi Technology Ltd. GuangZhou China
* Copyright    :  Copyright Guobi Technology Ltd. And Subject to Confidentiality 
*                 Restrictions. This document is the confidential and proprietary 
*                 property of Guobi Technology Ltd.It is neither the document                  
*                 itself nor any part of it can be shared with any party  without 
*                 the written  permission. 
* Author       :  haj
* Date         :  2010-06-08
* Purpose      :  虚拟键盘Layout抽象类
* Version      :  V1.0
* Last Changed List
* Name         |   date   |  Discription
****************************************************************************/

#if defined(__GBIME_VK__) // 国笔虚拟键盘

#ifndef __CLASS_LAYOUT_VK__
#define __CLASS_LAYOUT_VK__
#include "CLayoutAbstract.h"

#include "PlatformAdapter.h"
#include "PlatformTools.h"
#include "CLayoutInfo.h"
#include "GB_Global.h"
#include "CSwitchInputMethod.h"
#include "CSwitchLayout.h"

typedef void (* PFCLt__SetTouchRegion)(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo);
typedef PenPosition (* PFCLt__PenPositionIsInPinYin)(GBLPVOID pltObj, 
													 GBIMEPoint hitPoint, 
													 ButtonStatusEnum buttonStatus, 
													 PEngineOutputInfo pEngineOutputInfo); 
typedef PenPosition (* PFCLt__PenPositionIsInCandidate)(GBLPVOID pltObj, 
														GBIMEPoint hitPoint, 
														ButtonStatusEnum buttonStatus,
														PEngineOutputInfo pEngineOutputInfo); 
typedef PenPosition (* PFCLt__PenPositionIsInSyllablePageButton)(GBLPVOID pltObj, 
																 GBIMEPoint hitPoint, 
																 ButtonStatusEnum buttonStatus);
typedef PenPosition (* PFCLt__PenPositionIsInCandidatePageButton)(GBLPVOID pltObj, 
																  GBIMEPoint hitPoint,
																  ButtonStatusEnum buttonStatus);


/*!
 * \Layout this指针和Layout 基类指针转换
 */
#undef CLASE_THIS_POITER
#define CLASE_THIS_POITER(CLASS_NAME, pltObj)   \
	CLASS_NAME * pltThis = (CLASS_NAME *)pltObj; \
	PCLayoutAbstract pltBase = &pltThis->m_ltParent;

#define PAGE_DOWN_BUTTON_INDEX	0
#define PAGE_Up_BUTTON_INDEX	1

typedef struct _tag_CLayoutVK {
	CLayoutAbstract								m_ltParent;
	GBIMETouchRegion							syllableTouchRegion;///<音节栏触摸区域
	GBIMETouchRegion							candidateTouchRegion;///<候选栏触摸区域
	GBRECT										candidatePageDownOrUpRectArray[2];///<候选翻页对应的矩形区数组 
	GBRECT										syllablePageDownOrUpRectArray[2];///<音节翻页对应的矩形区数组

	PFCLt__SetTouchRegion						m_pfcSetTouchRegion;///<设置触摸区域(主设置函数, 可包含: 音节、候选、翻页按钮...)
	PFCLt__SetTouchRegion						m_pfcSetSyllableTouchRegion; // 设置音节触摸区
	PFCLt__SetTouchRegion						m_pfcSetCandidateTouchRegion; // 设置候选触摸区
	PFCLt__SetTouchRegion						m_pfcSetSyllablePageButtonTouchRegion; // 设置音节翻页按钮触摸区
	PFCLt__SetTouchRegion						m_pfcSetCandidatePageButtonTouchRegion; // 设置候选翻页按钮触摸区
	PFCLt__PenPositionIsInPinYin				m_pfcPenPositionIsInPinYin; // 检测是否选中音节
	PFCLt__PenPositionIsInCandidate				m_pfcPenPositionIsInCandidate;// 检测是否选中候选
	PFCLt__PenPositionIsInSyllablePageButton	m_pfcPenPositionIsInSyllablePageButton; // 检测是否选中音节翻页按钮
	PFCLt__PenPositionIsInCandidatePageButton	m_pfcPenPositionIsInCandidatePageButton; // 检测是否选中候选翻页按钮
} CLayoutVK, * PCLayoutVK;

CLayoutAbstract * CLayoutVK__Construct(PCLayoutVK pltVK, 
									   GBLPCVOID pltUIData,
									   GBLPCVOID pltIMData,
									   GBLPCVOID pltConfigData);
#endif //__CLASS_LAYOUT_VK__

#endif // 国笔虚拟键盘
