/***************************************************************************
* File Name    :  CLayoutHandwriteFullScreen.h
* Corporation  :  Guobi Technology Ltd. GuangZhou China
* Copyright    :  Copyright Guobi Technology Ltd. And Subject to Confidentiality 
*                 Restrictions. This document is the confidential and proprietary 
*                 property of Guobi Technology Ltd.It is neither the document                  
*                 itself nor any part of it can be shared with any party  without 
*                 the written  permission. 
* Author       :  weizhiping
* Date         :  2010-08-07
* Purpose      :  全屏手写键盘
* Version      :  V1.0
* Last Changed List
* Name         |   date   |  Discription
****************************************************************************/

#ifndef __CLASS_LAYOUT_HANDWRITE_FULLSCREEN__
#define __CLASS_LAYOUT_HANDWRITE_FULLSCREEN__

#include "CLayoutHandwrite.h"

#if defined(__GBIME_HANDWRITE__)

/*!
 * \Layout this指针和Layout 基类、根类指针转换
 */
#undef CLASE_THIS_POITER
#define CLASE_THIS_POITER(CLASS_NAME, pltObj)   \
	CLASS_NAME * pltThis = (CLASS_NAME *)pltObj; \
	PCLayoutHandwrite pltBase = &pltThis->m_ltParent; \
	PCLayoutAbstract pltRoot = &pltBase->m_ltParent;

/*!
 * \brief 手写引擎识别范围标记位定义
 */
typedef enum
{
	GBIME_HW_RECOGNIZE_RANGE_NONE				= 0x0000 // 识别默认(中文+标点)
	,GBIME_HW_RECOGNIZE_RANGE_ALL				= 0x0001 // 识别全部
	,GBIME_HW_RECOGNIZE_RANGE_NUMBER			= 0x0002 // 识别数字
	,GBIME_HW_RECOGNIZE_RANGE_ENGLISH			= 0x0004 // 识别英文
	,GBIME_HW_RECOGNIZE_RANGE_HOMONYM			= 0x0008 // 识别同音字
}GBIME_HW_RECOGNIZE_RANGE;

#define DIGIT_NUM 10

typedef struct _tag_CLayoutHandwriteFullScreen {
	CLayoutHandwrite			m_ltParent;

	GBIMETouchRegion			candidateTouchRegion;
	GBIMETouchRegion			associateTouchRegion;
	GBPCWCHAR					pNumberCandidates[DIGIT_NUM];///<数字候选0~9

	GBBOOL						bUpdateCandidate;
	GBBOOL						bUpdateAssocWord;
	GBBOOL						bDisableHandwriting; ///<点击了手写键盘区域，此时禁止进入手写识别流程
	GBINT16						nSelectedCandidateIndex;
	GBINT16						nSelectedAssociateWordIndex;
	
	GBIME_HW_RECOGNIZE_RANGE	hwRecognizeRangeFlag;
	//GBBOOL					   bInvalidateFlag;
} CLayoutHandwriteFullScreen, * PCLayoutHandwriteFullScreen;

CLayoutAbstract * CLayoutHandwriteFullScreen__Construct(PCLayoutHandwriteFullScreen pltHandwrite, 
							GBLPCVOID pltUIData,
							GBLPCVOID pltIMData,
							GBLPCVOID pltConfigData);
#endif // defined(__GBIME_HANDWRITE__)

#endif //__CLASS_LAYOUT_HANDWRITE_FULLSCREEN__


