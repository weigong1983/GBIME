/***************************************************************************
* File Name    :  CLayoutHandwrite.h
* Corporation  :  Guobi Technology Ltd. GuangZhou China
* Copyright    :  Copyright Guobi Technology Ltd. And Subject to Confidentiality 
*                 Restrictions. This document is the confidential and proprietary 
*                 property of Guobi Technology Ltd.It is neither the document                  
*                 itself nor any part of it can be shared with any party  without 
*                 the written  permission. 
* Author       :  weizhiping
* Date         :  2010-08-07
* Purpose      :  手写键盘抽象类
* Version      :  V1.0
* Last Changed List
* Name         |   date   |  Discription
****************************************************************************/

#ifndef __CLASS_LAYOUT_HANDWRITE__
#define __CLASS_LAYOUT_HANDWRITE__

#include "CHWEngine.h"
#include "CLayoutAbstract.h"

#if defined(__GBIME_HANDWRITE__)

/*!
 * \Layout this指针和Layout 基类指针转换
 */
#undef CLASE_THIS_POITER
#define CLASE_THIS_POITER(CLASS_NAME, pltObj)   \
	CLASS_NAME * pltThis = (CLASS_NAME *)pltObj; \
	PCLayoutAbstract pltBase = &pltThis->m_ltParent;

// 手写相关
// #define GBIME_DEF_HW_RECOGNIZE_PIXELS 		1 // 控制手写识别灵敏度
// #define GBIME_DEF_HW_MAX_TRACE_PER_WORD 	500
// 
// #define HW_PIXEL_DEFAULT 					3
// #define HW_SPEED_DEFAULT 					800
// #define HW_COLOR_DEFAULT 					PT_COLOR_RED
// 
// typedef enum{
// 	GBTRACE_None,
// 		GBTRACE_Ready,
// 		GBTRACE_Drawing,
// 		GBTRACE_Identified
// }GBTraceStatusEnum;
// 
// typedef void (*pfRecognizeFinishedCallback)(void);
// typedef GBBOOL (*PF__HWTraceHandleEvent)(GBLPVOID pltObj, GBINT nPlatformEvent, GBIMEPoint point);
// 
// typedef struct __tagGBHWTrace
// {
// 	PF__HWTraceHandleEvent		pfHWTraceHandleEvent;
// 	GBU16						timer;
// 	GBTraceStatusEnum			status;
// 	GBINT						total_hits;
// 	GBBOOL						enable;
// 	GBU16						pixel;
// 	PT_COLOR					color;
// 	GBU32						timeout;
// 	GBIMEPoint					last_point;
//     GBINT16						traceBuffer[GBIME_DEF_HW_MAX_TRACE_PER_WORD * 2 + 2 ];
// }GBHWTrace, *PGBHWTrace;

typedef struct _tag_CLayoutHandwrite {
	CLayoutAbstract				m_ltParent;
	//PGBHWTrace					pGBHWTrace;///<手写轨迹管理
} CLayoutHandwrite, * PCLayoutHandwrite;

CLayoutAbstract * CLayoutHandwrite__Construct(PCLayoutHandwrite pltHandwrite, 
					      GBLPCVOID pltUIData,
					      GBLPCVOID pltIMData,
					      GBLPCVOID pltConfigData);

#endif // defined(__GBIME_HANDWRITE__)
#endif //__CLASS_LAYOUT_HANDWRITE__


