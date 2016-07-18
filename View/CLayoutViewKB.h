/***************************************************************************
* File Name    :  CLayoutViewKB.h
* Corporation  :  Guobi Technology Ltd. GuangZhou China
* Copyright    :  Copyright Guobi Technology Ltd. And Subject to Confidentiality 
*                 Restrictions. This document is the confidential and proprietary 
*                 property of Guobi Technology Ltd.It is neither the document                  
*                 itself nor any part of it can be shared with any party  without 
*                 the written  permission. 
* Author       :  weizhiping
* Date         :  2010-07-09
* Purpose      :  物理九键Layout结构体
* Version      :  V1.0
* Last Changed List
* Name         |   date   |  Discription
****************************************************************************/
#if defined(__GBIME_KB__) // 国笔物理键盘

#ifndef __CLASS_LAYOUT_VIEW_KEYBOARD__
#define __CLASS_LAYOUT_VIEW_KEYBOARD__
#include "CLayoutViewAbstract.h"

#define GBIME_KB_CANDIDATE_LEFT_PADDING		5 ///<候选字起始绘制位置相对于候选框左端的空白间距
#define GBIME_KB_CANDIDATE_TOP_PADDING		2 ///<候选字起始绘制位置相对于候选框上端的空白间距

typedef void (*PFCLtView__DrawKeyboard)(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo);
typedef void (*PFCLtView__DrawOutputString)(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo);
typedef void (*PFCLtView__DrawCandidate)(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo);
typedef void (*PFCLtView__DrawPinYin)(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo);
typedef void (*PFCLtView__DrawStroke)(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo);
typedef void (*PFCLtView__DrawComponents)(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo);
typedef void (*PFCLtView__DrawStrokeUpLine)(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo);
typedef void (*PFCLtView__RedrawCandidateButton)(GBLPVOID pltObj, 
												 GBUINT16 iStartX, 
												 GBUINT16 iStartY, 
												 GBUINT16 layerId,
												 PEngineOutputInfo pEngineOutputInfo);
//typedef void (*PFCLtView__RedrawKeyBoardButton)(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo);
//typedef void (*PFCLtView__RedrawKeyboardSpecialButton)(GBLPVOID pltObj);

typedef struct _tag_CLayoutViewKB {
	CLayoutViewAbstract            m_ltParent;

	// 公共绘图函数
	PFCLtView__DrawKeyboard			m_pfcDrawKeyboard;///<绘制键盘
	PFCLtView__DrawOutputString		m_pfcDrawOutputString;///<绘制输出串
	PFCLtView__DrawCandidate		m_pfcDrawCandidate;///<绘制候选字
	PFCLtView__DrawPinYin			m_pfcDrawPinYin;///<绘制拼音音节
	PFCLtView__DrawStroke			m_pfcDrawStroke;///<绘制笔画
	PFCLtView__DrawComponents		m_pfcDrawComponents;///<绘制部首
	PFCLtView__DrawStrokeUpLine		m_pfcDrawStrokeUpLine;///<绘制笔画输入栏(九键)
	PFCLtView__RedrawCandidateButton m_pfcRedrawArrowButton;///<绘制箭头按钮
//	PFCLtView__RedrawKeyBoardButton	 m_pfcRedrawShiftCapButton;///<大小写按钮
//	PFCLtView__RedrawKeyboardSpecialButton m_pfcRedrawKeyboardSpecialButton;///<重绘第二、三层中逻辑上属于第一层的按钮（如拼音分隔符等...）

} CLayoutViewKB, * PCLayoutViewKB;

CLayoutViewAbstract * CLayoutViewKB__Construct(PCLayoutViewKB pltViewKB);
GBIMEReturn CLayoutViewKB__Init(PCLayoutViewKB pltViewKB);

#endif //__CLASS_LAYOUT_VIEW_KEYBOARD__

#endif // 国笔物理键盘

