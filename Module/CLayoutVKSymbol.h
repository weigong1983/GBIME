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
* Purpose      :  �������Layout
* Version      :  V1.0
* Last Changed List
* Name         |   date   |  Discription
****************************************************************************/
#if defined(__GBIME_VK__) // �����������

#ifndef __CLASS_LAYOUT_VK_SYMBOL__
#define __CLASS_LAYOUT_VK_SYMBOL__
#include "CLayoutVK.h"

/*!
 * \Layout thisָ���Layout ����ָ��ת��
 */
#undef CLASE_THIS_POITER
#define CLASE_THIS_POITER(CLASS_NAME, pltObj)   \
	CLASS_NAME * pltThis = (CLASS_NAME *)pltObj; \
	PCLayoutVK pltBase = &pltThis->m_ltParent; \
	PCLayoutAbstract pltRoot = &pltThis->m_ltParent.m_ltParent;

#define SYMBOL_COUNT_PER_PAGE	9		// ÿҳ�ķ��Ÿ���
#define SYMBOL_END_FLAG			0xffff	// ���Ž�����־
typedef enum _GbSymbolTypeEnum
{
	SymbolTypeCommon,					// ��ͨ����
	SymbolTypeFace,						// �������
	SymbolTypeEspecial,					// �������
	SymbolTypeNumber					// ��ѧ����
}GbSymbolTypeEnum;

typedef enum _GBCommonSymbolTypeEnum
{
	FullShaped,							// ��ͨȫ�Ƿ���
	HalfShaped,							// ��ͨ��Ƿ���
}GBCommonSymbolType;

typedef struct _tag_CLayoutVKSymbol {
	CLayoutVK					m_ltParent;
	
	GBINT16						m_symbolType;                   ///<��������
	GBINT16						m_pageCount;					///<��ǰ��ʾ�����б���һҳ
	GBLPCVOID					m_pltSymbolData;                ///<layout IM data point
	GBUINT16					m_iCurrentPageStartIndex;		///<��ǰҳ�ĵ�һ���������ܷ����е����
	GBUINT16					m_iCurrentPageEndIndex;			///<��ǰҳ�����һ���������ܷ����е����
	GBUINT16					m_iCurrentChooseSymbolIndex;	///<��ǰѡ��ķ��ŵ�����
	GBBOOL						m_bHasNextPage;					///<�Ƿ�����һҳ�ı�־
	GBBOOL						m_bHasPrvePage;					///<�Ƿ�����һҳ�ı�־
	GBUINT16					m_iSymbolTotalCount;			///<���ŵ��ܸ���
	GBUINT16					m_iSymbolCountPerPage;			///<ÿҳ�ķ��ŵĸ���
	GBBOOL						m_bSymbolLock;					///<��������״̬����������������ţ���������һ�����ź��Զ�������һ����ģʽ
	GBCommonSymbolType			m_commonSymbolType;				///<��ͨ��������(ȫ������/���Ӣ��)
	GBBOOL						m_hasFullOrHalfSymbol;          ///<�Ƿ��а�ǻ�ȫ�Ƿ��ŵı�־
} CLayoutVKSymbol, * PCLayoutVKSymbol;

CLayoutAbstract * CLayoutVKSymbol__Construct(PCLayoutVKSymbol pltVKSymbol, 
										   GBLPCVOID pltUIData,
										   GBLPCVOID pltIMData,
										   GBLPCVOID pltConfigData);
GBUINT16	CLayoutVKSymbol__GetSymbolOffsetByIndex(GBLPVOID pltObj, GBUINT16 SymbolIndex);

#endif //__CLASS_LAYOUT_VK_SYMBOL__

#endif // �����������
