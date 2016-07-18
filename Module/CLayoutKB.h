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
* Purpose      :  ����ż�ƴ��Layout�ṹ��
* Version      :  V1.0
* Last Changed List
* Name         |   date   |  Discription
****************************************************************************/
#if defined(__GBIME_KB__) // �����������

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
 * \Layout thisָ���Layout ����ָ��ת��
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
	GBIMETouchRegion							syllableRegion;///<��������������
	GBIMETouchRegion							candidateRegion;///<��ѡ����������
	GBRECT										candidatePageDownOrUpRectArray[2];///<��ѡ��ҳ��Ӧ�ľ��������� 
	GBRECT										syllablePageDownOrUpRectArray[2];///<���ڷ�ҳ��Ӧ�ľ���������

	PFCLt__SetSyllableRegion					m_pfcSetSyllableRegion; // �������ڴ�����
	PFCLt__SetOutputStringRegion				m_pfcSetOutputStringRegion; // ���������������
	PFCLt__SetCandidateRegion					m_pfcSetCandidateRegion; // ���ú�ѡ������
	PFCLT__SetSymbolCandRegion					m_pfcSetSymbolCandRegion;//���ñ����ʾ����
	PFCLt__SetSyllablePageButtonTouchRegion		m_pfcSetSyllablePageButtonRegion; // �������ڷ�ҳ��ť������
	PFCLt__SetCandidatePageButtonTouchRegion	m_pfcSetCandidatePageButtonRegion; // ���ú�ѡ��ҳ��ť������	
	PFCLt__UpdateLayoutInformation				m_pfcUpdateLayoutInformation;//����layout��Ϣ
} CLayoutKB, * PCLayoutKB;

CLayoutAbstract * CLayoutKB__Construct(PCLayoutKB pltKB, 
									   GBLPCVOID pltUIData,
									   GBLPCVOID pltIMData,
									   GBLPCVOID pltConfigData);
#endif //__CLASS_LAYOUT_KEYBOARD_NINE_PINYIN_

#endif // �����������