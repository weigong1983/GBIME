/***************************************************************************
* File Name    :  CLayoutViewVK.h
* Corporation  :  Guobi Technology Ltd. GuangZhou China
* Copyright    :  Copyright Guobi Technology Ltd. And Subject to Confidentiality 
*                 Restrictions. This document is the confidential and proprietary 
*                 property of Guobi Technology Ltd.It is neither the document                  
*                 itself nor any part of it can be shared with any party  without 
*                 the written  permission. 
* Author       :  haj
* Date         :  2010-06-08
* Purpose      :  �������Layout��ͼ��
* Version      :  V1.0
* Last Changed List
* Name         |   date   |  Discription
****************************************************************************/
#if defined(__GBIME_VK__) // �����������

#ifndef __CLASS_LAYOUT_VIEW_VK__
#define __CLASS_LAYOUT_VIEW_VK__
#include "CLayoutViewAbstract.h"

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
typedef void (*PFCLtView__RedrawKeyBoardButton)(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo);
typedef void (*PFCLtView__RedrawKeyboardSpecialButton)(GBLPVOID pltObj);

typedef struct _tag_CLayoutViewVK {
	CLayoutViewAbstract            m_ltParent;
	
	// ������ͼ����
	PFCLtView__DrawKeyboard			m_pfcDrawKeyboard;///<���Ƽ���
	PFCLtView__DrawOutputString		m_pfcDrawOutputString;///<���������
	PFCLtView__DrawCandidate		m_pfcDrawCandidate;///<���ƺ�ѡ��
	PFCLtView__DrawPinYin			m_pfcDrawPinYin;///<����ƴ������
	PFCLtView__DrawStroke			m_pfcDrawStroke;///<���Ʊʻ�
	PFCLtView__DrawComponents		m_pfcDrawComponents;///<���Ʋ���
	PFCLtView__DrawStrokeUpLine		m_pfcDrawStrokeUpLine;///<���Ʊʻ�������(�ż�)
	PFCLtView__RedrawCandidateButton m_pfcRedrawArrowButton;///<���Ƽ�ͷ��ť
	PFCLtView__RedrawKeyBoardButton	 m_pfcRedrawShiftCapButton;///<��Сд��ť
	PFCLtView__RedrawKeyboardSpecialButton m_pfcRedrawKeyboardSpecialButton;///<�ػ�ڶ����������߼������ڵ�һ��İ�ť����ƴ���ָ�����...��
} CLayoutViewVK, * PCLayoutViewVK;

CLayoutViewAbstract * CLayoutViewVK__Construct(PCLayoutViewVK pltViewVK);
GBIMEReturn CLayoutViewVK__Init(PCLayoutViewVK pltViewVK);

#endif //__CLASS_LAYOUT_VIEW_VK__

#endif // �����������

