/***************************************************************************
* File Name    :  CLayoutControl.c
* Corporation  :  Guobi Technology Ltd. GuangZhou China
* Copyright    :  Copyright Guobi Technology Ltd. And Subject to Confidentiality 
*                 Restrictions. This document is the confidential and proprietary 
*                 property of Guobi Technology Ltd.It is neither the document                  
*                 itself nor any part of it can be shared with any party  without 
*                 the written  permission. 
* Author       :  fanbo
* Date         :  2010-06-08
* Purpose      :  Layout�ڲ��߼�����ʵ���ļ�
* Version      :  V1.0
* Last Changed List
* Name         |   date   |  Discription
****************************************************************************/
#include "CLayoutControl.h"
#include "GB_Global.h"

/*!
 * \brief Layout����ģ��������������Ϣ
 * \param void 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-07-28
 */
GBIMEReturn CLayoutControl_UpdateOutputInfo(void)
{
	PCLayoutControl pLtCtl = Global_GetLayoutCtl();

	PT_Assert(pLtCtl != NULL);

	pLtCtl->engineOutputInfo.pInputString = GBEngine_GetInputString(Global_GetEnineInstance());
	// �������
	pLtCtl->engineOutputInfo.pUpScreenStr = GBEngine_GetUpScreenString(Global_GetEnineInstance());
	//pLtCtl->engineOutputInfo.nStatusFlag = GBEngine_GetCurStatus();
	pLtCtl->engineOutputInfo.bCandWindOpen = GBEngine_IsActive(Global_GetEnineInstance());
	pLtCtl->engineOutputInfo.bInputEmpty = GBEngine_IsInputEmpty(Global_GetEnineInstance());

	// ��ѡ���
	pLtCtl->engineOutputInfo.nCandNum = GBEngine_GetCandNum(Global_GetEnineInstance());
	pLtCtl->engineOutputInfo.ppCandidates = GBEngine_GetCandidateBuffer(Global_GetEnineInstance());

	// �������
	pLtCtl->engineOutputInfo.nSyllableNum = GBEngine_GetSyllableNum(Global_GetEnineInstance());
	pLtCtl->engineOutputInfo.nCurrentSyllableIndex = GBEngine_GetCurrentSyllableIndex(Global_GetEnineInstance());
	pLtCtl->engineOutputInfo.ppSyllables = GBEngine_GetSyllableBuffer(Global_GetEnineInstance());

	// �ʻ��������
	pLtCtl->engineOutputInfo.pStroke = GBEngine_GetStorkeString(Global_GetEnineInstance());
	pLtCtl->engineOutputInfo.pComponents = GBEngine_GetComponentsString(Global_GetEnineInstance());
	pLtCtl->engineOutputInfo.pOutputString = GBEngine_GetOutputString(Global_GetEnineInstance());

	// ��ѡ���м�ͷ��ָʾ���
	pLtCtl->engineOutputInfo.bUpScreenArrow = GBEngine_GetUpScreenArrowFlag(Global_GetEnineInstance());
	pLtCtl->engineOutputInfo.bCandArrowUp = GBEngine_GetCandUpArrowFlag(Global_GetEnineInstance());		///<��ѡ��ʾ �� ��ͷ
	pLtCtl->engineOutputInfo.bCandArrowDown = GBEngine_GetCandDownArrowFlag(Global_GetEnineInstance());		///<��ѡ��ʾ �� ��ͷ
	pLtCtl->engineOutputInfo.bCandArrowLeft = GBEngine_GetCandLeftArrowFlag(Global_GetEnineInstance());		///<��ѡ��ʾ �� ��ͷ
	pLtCtl->engineOutputInfo.bCandArrowRight = GBEngine_GetCandRightArrowFlag(Global_GetEnineInstance());	///<��ѡ��ʾ �� ��ͷ
	pLtCtl->engineOutputInfo.bSyllableArrowLeft = GBEngine_GetSyllableLeftArrowFlag(Global_GetEnineInstance()); ///<Ambiguous Chinese only, ƴ����ҳ��ʾ���ͷ
	pLtCtl->engineOutputInfo.bSyllableArrowRight = GBEngine_GetSyllableRightArrowFlag(Global_GetEnineInstance());///<Ambiguous Chinese only, ƴ����ҳ��ʾ�Ҽ�ͷ

	// ������Ϣ
	pLtCtl->engineOutputInfo.pRowStart = GBEngine_GetRowStart(Global_GetEnineInstance());
	pLtCtl->engineOutputInfo.nSelCol = GBEngine_GetSelCol(Global_GetEnineInstance());
	pLtCtl->engineOutputInfo.nSelRow = GBEngine_GetSelRow(Global_GetEnineInstance());
	pLtCtl->engineOutputInfo.nRowCount = GBEngine_GetRowCount(Global_GetEnineInstance());

	// Multitap���[����LayoutID�ж�]
	//pLtCtl->engineOutputInfo.isMultiTapInputMode = GBEngine_IsMultiTapInputMode(Global_GetEnineInstance());
	// ALP�����Сд���
	//pLtCtl->engineOutputInfo.shiftCapStatus = GBEngine_GetShiftCapStatus(Global_GetEnineInstance());

	// ״̬���
	pLtCtl->engineOutputInfo.isSelectedStatus = GBEngine_IsSelectedStatus(Global_GetEnineInstance());
	pLtCtl->engineOutputInfo.isSwitchNextInputMode = GBEngine_IsNeedSwitchInputModeState(Global_GetEnineInstance());
	pLtCtl->engineOutputInfo.isSymbolCandType = GBEngine_IsSymbolCandType(Global_GetEnineInstance());
	//pLtCtl->engineOutputInfo.isMultiTapInputMode = GBEngine_IsMultiTapInputMode(Global_GetEnineInstance());
	return GBIME_OK;
}

/*!
 * \brief ��ȡLayout����ģ��ά�������������Ϣ
 * \param void 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-07-28
 */
PEngineOutputInfo CLayoutControl_GetEngineOutputInfo(void)
{
	PCLayoutControl pLtCtl = Global_GetLayoutCtl();

	PT_Assert(pLtCtl != NULL);

	return &pLtCtl->engineOutputInfo;
}

/*!
 * \brief ��ȡ��ѡ���Ƿ��
 * \param void 
 * \return 
 * \note 
 * \example
 * \author Zhaokun
 * \date 2010-10-22
 */
GBBOOL CLayoutControl_IsOpenCandWind(void)
{
	PCLayoutControl pLtCtl = Global_GetLayoutCtl();

	PT_Assert(pLtCtl != NULL);

	return pLtCtl->engineOutputInfo.bCandWindOpen;
}

/*!
 * \brief Layout��Ϣ������
 * \param pIMEEvent 
 * \return 
 */
GBIMEReturn CLayoutControl_HandleMessage(PGBIMEEvent pIMEEvent)
{
	GBIMEReturn ret = GBIME_OK;

	if (pIMEEvent->nType != GBIME_EVENT_TYPE_LAYOUT)
	{
		return GBIME_IGNORE; // ����д�¼�������
	}

	switch (pIMEEvent->nEvent)
	{
	case GBIME_EVENT_LAYOUT_CREATE: // Layout ����
		{		
			PCLayoutAbstract pltTempObject = NULL;
			pltTempObject = Global_LayoutObjectIsLeaveByID((GBUINT16)pIMEEvent->wParam);
			if (NULL == pltTempObject)
			{
				pltTempObject = CFactory_CreateInstance(Global_GetIMEData(), (GBUINT16)pIMEEvent->wParam);
			}
			if (NULL != pltTempObject)
			{
				Global_SaveLayoutObject(pltTempObject);
				// ���ñ����Ź��ܼ�
				if (pltTempObject->m_pfcSetInterpunctionKey != NULL)
				{
					pltTempObject->m_pfcSetInterpunctionKey(pltTempObject);
				}
			}
		}
		break;
	case GBIME_EVENT_LAYOUT_DESTROY: // Layout ����
		break;
	case GBIME_EVENT_LAYOUT_SWITCH: // Layout �л�
		CSwitchLayout_Switch((GBUINT16)pIMEEvent->wParam);
		break;
	case GBIME_EVENT_LAYOUT_PEN_DOWN: ///<Layout �����ʰ���
	case GBIME_EVENT_LAYOUT_PEN_UP:	  ///<Layout �����ʵ���
	case GBIME_EVENT_LAYOUT_PEN_MOVE:  ///<Layout �������ƶ�
	case GBIME_EVENT_LAYOUT_ENTER:
	case GBIME_EVENT_LAYOUT_EXIT:
	case GBIME_EVENT_LAYOUT_HIDE: ///<Layout ����
	case GBIME_EVENT_LAYOUT_SHOW: ///<Layout ��ʾ
	case GBIME_EVENT_LAYOUT_ACTIVATE: ///<Layout ����
	case GBIME_EVENT_LAYOUT_DEACTIVATE: ///<Layout ע��
		{
			PCLayoutAbstract pltObj = Global_GetCurrentLayoutObject();
			
			// Layout��Ϣ����
			if (pltObj->m_pfcHandleEvent != NULL)
			{
				pltObj->m_pfcHandleEvent(pltObj, pIMEEvent, CLayoutControl_GetEngineOutputInfo());
			}
		}
		break;
	case GBIME_EVENT_LAYOUT_REFRESH: ///<Layout ˢ��
		{
			PCLayoutAbstract pltObj = Global_GetCurrentLayoutObject();
			
			// LayoutControlģ����֯����������ݣ�Ȼ��֪ͨLayout��ͼ����ˢ����ʾ
			CLayoutControl_UpdateOutputInfo();
			
			// Layout��ͼˢ����ʾ��������
			if (pltObj->m_pfcPaint != NULL)
			{
				pltObj->m_pfcPaint(pltObj, CLayoutControl_GetEngineOutputInfo());
			}
		}
		break;
	case GBIME_EVENT_LAYOUT_REDRAW:
		{
			PCLayoutAbstract pltObj = Global_GetCurrentLayoutObject();
			
			// LayoutControlģ����֯����������ݣ�Ȼ��֪ͨLayout��ͼ����ˢ����ʾ
			CLayoutControl_UpdateOutputInfo();
			
			// Layout��ͼˢ����ʾ��������
			if (pltObj->m_pfcPaint != NULL)
			{
				pltObj->m_refreshFlag = GBIME_LAYOUT_REFRESH_ALL;
				pltObj->m_pfcPaint(pltObj, CLayoutControl_GetEngineOutputInfo());
			}
		}
		break;
	case GBIME_EVENT_KEY_DOWN:
	case GBIME_EVENT_KEY_UP:
	case GBIME_EVENT_KEY_LONG_PRESS:
		{
			PCLayoutAbstract pltObj = Global_GetCurrentLayoutObject();
		
			// Layout��Ϣ����
			if (pltObj->m_pfcHandleEvent != NULL)
			{
				pltObj->m_pfcHandleEvent(pltObj, pIMEEvent, CLayoutControl_GetEngineOutputInfo());
			}
		}
		break;
	default:
		PT_Assert(0); // �Ƿ��¼�
	}

	return ret;
}