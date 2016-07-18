/***************************************************************************
* File Name    :  CLayoutVKSymbol.h
* Corporation  :  Guobi Technology Ltd. GuangZhou China
* Copyright    :  Copyright Guobi Technology Ltd. And Subject to Confidentiality 
*                 Restrictions. This document is the confidential and proprietary 
*                 property of Guobi Technology Ltd.It is neither the document                  
*                 itself nor any part of it can be shared with any party  without 
*                 the written  permission. 
* Author       :  haj
* Date         :  2010-06-08
* Purpose      :  虚拟符号Layout
* Version      :  V1.0
* Last Changed List
* Name         |   date   |  Discription
****************************************************************************/
#if defined(__GBIME_VK__) // 国笔虚拟键盘

#ifndef __CLASS_LAYOUT_VK_SYMBOL__
#define __CLASS_LAYOUT_VK_SYMBOL__
#include "CLayoutVK.h"

/*!
 * \Layout this指针和Layout 基类指针转换
 */
#undef CLASE_THIS_POITER
#define CLASE_THIS_POITER(CLASS_NAME, pltObj)   \
	CLASS_NAME * pltThis = (CLASS_NAME *)pltObj; \
	PCLayoutVK pltBase = &pltThis->m_ltParent; \
	PCLayoutAbstract pltRoot = &pltThis->m_ltParent.m_ltParent;

#define SYMBOL_COUNT_PER_PAGE	9		// 每页的符号个数
#define SYMBOL_END_FLAG			0xffff	// 符号结束标志
typedef enum _GbSymbolTypeEnum
{
	SymbolTypeCommon,					// 普通符号
	SymbolTypeFace,						// 表情符号
	SymbolTypeEspecial,					// 特殊符号
	SymbolTypeNumber					// 数学符号
}GbSymbolTypeEnum;

typedef enum _GBCommonSymbolTypeEnum
{
	FullShaped,							// 普通全角符号
	HalfShaped,							// 普通半角符号
}GBCommonSymbolType;

typedef struct _tag_CLayoutVKSymbol {
	CLayoutVK					m_ltParent;
	
	GBINT16						m_symbolType;                   ///<符号类型
	GBINT16						m_pageCount;					///<当前显示符号列表那一页
	GBLPCVOID					m_pltSymbolData;                ///<layout IM data point
	GBUINT16					m_iCurrentPageStartIndex;		///<当前页的第一个符号在总符号中的序号
	GBUINT16					m_iCurrentPageEndIndex;			///<当前页的最后一个符号在总符号中的序号
	GBUINT16					m_iCurrentChooseSymbolIndex;	///<当前选择的符号的索引
	GBBOOL						m_bHasNextPage;					///<是否有下一页的标志
	GBBOOL						m_bHasPrvePage;					///<是否有上一页的标志
	GBUINT16					m_iSymbolTotalCount;			///<符号的总个数
	GBUINT16					m_iSymbolCountPerPage;			///<每页的符号的个数
	GBBOOL						m_bSymbolLock;					///<符号锁定状态，可以连续输入符号，否则输入一个符号后自动返回上一输入模式
	GBCommonSymbolType			m_commonSymbolType;				///<普通符号类型(全角中文/半角英文)
	GBBOOL						m_hasFullOrHalfSymbol;          ///<是否有半角或全角符号的标志
} CLayoutVKSymbol, * PCLayoutVKSymbol;

CLayoutAbstract * CLayoutVKSymbol__Construct(PCLayoutVKSymbol pltVKSymbol, 
										   GBLPCVOID pltUIData,
										   GBLPCVOID pltIMData,
										   GBLPCVOID pltConfigData);
GBUINT16	CLayoutVKSymbol__GetSymbolOffsetByIndex(GBLPVOID pltObj, GBUINT16 SymbolIndex);

#endif //__CLASS_LAYOUT_VK_SYMBOL__

#endif // 国笔虚拟键盘
