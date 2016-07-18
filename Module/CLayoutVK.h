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
* Purpose      :  �������Layout������
* Version      :  V1.0
* Last Changed List
* Name         |   date   |  Discription
****************************************************************************/

#if defined(__GBIME_VK__) // �����������

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
 * \Layout thisָ���Layout ����ָ��ת��
 */
#undef CLASE_THIS_POITER
#define CLASE_THIS_POITER(CLASS_NAME, pltObj)   \
	CLASS_NAME * pltThis = (CLASS_NAME *)pltObj; \
	PCLayoutAbstract pltBase = &pltThis->m_ltParent;

#define PAGE_DOWN_BUTTON_INDEX	0
#define PAGE_Up_BUTTON_INDEX	1

typedef struct _tag_CLayoutVK {
	CLayoutAbstract								m_ltParent;
	GBIMETouchRegion							syllableTouchRegion;///<��������������
	GBIMETouchRegion							candidateTouchRegion;///<��ѡ����������
	GBRECT										candidatePageDownOrUpRectArray[2];///<��ѡ��ҳ��Ӧ�ľ��������� 
	GBRECT										syllablePageDownOrUpRectArray[2];///<���ڷ�ҳ��Ӧ�ľ���������

	PFCLt__SetTouchRegion						m_pfcSetTouchRegion;///<���ô�������(�����ú���, �ɰ���: ���ڡ���ѡ����ҳ��ť...)
	PFCLt__SetTouchRegion						m_pfcSetSyllableTouchRegion; // �������ڴ�����
	PFCLt__SetTouchRegion						m_pfcSetCandidateTouchRegion; // ���ú�ѡ������
	PFCLt__SetTouchRegion						m_pfcSetSyllablePageButtonTouchRegion; // �������ڷ�ҳ��ť������
	PFCLt__SetTouchRegion						m_pfcSetCandidatePageButtonTouchRegion; // ���ú�ѡ��ҳ��ť������
	PFCLt__PenPositionIsInPinYin				m_pfcPenPositionIsInPinYin; // ����Ƿ�ѡ������
	PFCLt__PenPositionIsInCandidate				m_pfcPenPositionIsInCandidate;// ����Ƿ�ѡ�к�ѡ
	PFCLt__PenPositionIsInSyllablePageButton	m_pfcPenPositionIsInSyllablePageButton; // ����Ƿ�ѡ�����ڷ�ҳ��ť
	PFCLt__PenPositionIsInCandidatePageButton	m_pfcPenPositionIsInCandidatePageButton; // ����Ƿ�ѡ�к�ѡ��ҳ��ť
} CLayoutVK, * PCLayoutVK;

CLayoutAbstract * CLayoutVK__Construct(PCLayoutVK pltVK, 
									   GBLPCVOID pltUIData,
									   GBLPCVOID pltIMData,
									   GBLPCVOID pltConfigData);
#endif //__CLASS_LAYOUT_VK__

#endif // �����������
