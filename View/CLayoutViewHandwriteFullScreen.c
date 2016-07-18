/***************************************************************************
* File Name    :  CLayoutViewHandwrite.C
* Corporation  :  Guobi Technology Ltd. GuangZhou China
* Copyright    :  Copyright Guobi Technology Ltd. And Subject to Confidentiality 
*                 Restrictions. This document is the confidential and proprietary 
*                 property of Guobi Technology Ltd.It is neither the document                  
*                 itself nor any part of it can be shared with any party  without 
*                 the written  permission. 
* Author       :  weizhiping
* Date         :  2010-08-07
* Purpose      :  ��д������ͼ��
* Version      :  V1.0
* Last Changed List
* Name         |   date   |  Discription
****************************************************************************/
#include "CLayoutViewHandwriteFullScreen.h"
#include "CLayoutHandwriteFullScreen.h"
#include "PlatformAdapter.h"
#include "PlatformTools.h"
#include "GB_Global.h"
#include "CHWEngine.h"
#include "GBIMEResDef.h"

#if defined(__GBIME_HANDWRITE__)

/*!
 * \ Layout View ָ��Ԥ����
 */
#define CLASS_VIEW_THIS_POITER(CLASS_NAME, pltThis)   \
	CLASS_NAME * pltViewThis = (CLASS_NAME *)pltThis->m_ltParent.m_ltParent.m_hltView; \
	PCLayoutViewHandwrite pltViewBase = &pltViewThis->m_ltParent; \
	PCLayoutViewAbstract pltViewRoot = &pltViewThis->m_ltParent.m_ltParent;

GBIMELOCAL GBIMEReturn CLayoutViewHandwriteFullScreen__Paint(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo);

GBIMELOCAL void ICLayoutViewHandwriteFullScreen_DrawKeyboard(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo);
GBIMELOCAL void ICLayoutViewHandwriteFullScreen_DrawCandidate(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo);
GBIMELOCAL void ICLayoutViewHandwriteFullScreen_DrawAssocWord(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo);
															  
CLayoutViewAbstract * CLayoutViewHandwriteFullScreen__Construct(PCLayoutViewHandwriteFullScreen pltViewHandwrite)
{
	CLayoutViewHandwrite__Construct(&pltViewHandwrite->m_ltParent);
	CLayoutViewHandwriteFullScreen__Init(pltViewHandwrite);
	return (CLayoutViewAbstract *)pltViewHandwrite;
}

GBIMEReturn CLayoutViewHandwriteFullScreen__Init(PCLayoutViewHandwriteFullScreen pltViewHandwrite)
{
	pltViewHandwrite->m_ltParent.m_ltParent.m_pfcPaint = CLayoutViewHandwriteFullScreen__Paint;
	return GBIME_OK;
}

GBIMELOCAL GBIMEReturn CLayoutViewHandwriteFullScreen__Paint(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo)
{
	CLASE_THIS_POITER(CLayoutHandwriteFullScreen, pltObj)

	PLayoutInfo pLayoutInfo = (PLayoutInfo)(&pltRoot->m_ltData); // Layout UI data point

	if (LayoutHandwriteFullScreen != pltRoot->m_ltType)
	{
		//return GBIME_LAYOUT_NOTMATCH;
	}

	if (pltRoot->m_refreshFlag == GBIME_LAYOUT_REFRESH_NONE)
	{
		return GBIME_OK;
	}

	PT_LayerLock();

	// ˢ������
	if ((pltRoot->m_refreshFlag & GBIME_LAYOUT_REFRESH_ALL) == GBIME_LAYOUT_REFRESH_ALL)
	{
		ICLayoutViewHandwriteFullScreen_DrawKeyboard(pltObj, pEngineOutputInfo);
	} 
	// ˢ����������
	else if ((pltRoot->m_refreshFlag & GBIME_LAYOUT_REFRESH_KEYBOARD) == GBIME_LAYOUT_REFRESH_KEYBOARD)
	{
		ICLayoutViewHandwriteFullScreen_DrawKeyboard(pltObj, pEngineOutputInfo);
	}
	// ˢ�º�ѡ��
	if ((pltRoot->m_refreshFlag & GBIME_LAYOUT_REFRESH_CANDIDATE_LINE) == GBIME_LAYOUT_REFRESH_CANDIDATE_LINE)
	{
		// ���ƺ�ѡ�����
		//if (pltThis->bUpdateCandidate)
		{
			ICLayoutViewHandwriteFullScreen_DrawCandidate(pltObj, pEngineOutputInfo);			
		}
		
		//if (pltThis->bUpdateAssocWord) // ��������ʴ����
		{
			ICLayoutViewHandwriteFullScreen_DrawAssocWord(pltObj, pEngineOutputInfo);
		}
	}
	
	// ������ˢ��
	PT_LayerUnLock();
	PT_Invalidate(0, 0, PT_GetLcdWidth(), PT_GetLcdHeight());
	//pltRoot->m_pfcRedraw(pltObj);
	return GBIME_OK;
}

/*!
 * \brief ������д����
 * \param pltObj 
 * \param pEngineOutputInfo 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-8-12 11:47:41
*/
static void ICLayoutViewHandwriteFullScreen_DrawKeyboard(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo)
{
	CLASE_THIS_POITER(CLayoutHandwriteFullScreen, pltObj)
	CLASS_VIEW_THIS_POITER(CLayoutViewHandwriteFullScreen, pltThis)

	//��ǰҪ��ʾ�Ĳ�
	GBUINT16 iCurrentLayerNumber;
	GBUINT16 iCurrentLayoutID;
	//��ʾͼƬ�����Ͻ�X����
	GBUINT16 iDisplayImageLeftX;
	//��ʾͼƬ�����Ͻ�Y����
	GBUINT16 iDisplayImageTopY;
	//��ǰҪ��ʾ�Ĳ������״̬�µ�ͼƬID
	GBUINT16 iCurrentLayerNomalImageId;
	//��ǰҪ��ʾ�Ĳ�İ���״̬�µ�ͼƬID
	GBUINT16 iCurrentLayerDownImageId;
	//��ѡ����
	GBUINT16 iCandidateRowCount = 1;
	//��ť�ܸ���
	GBUINT16 iButtonTotalCount;
	GBUINT16 iCount;
	GBUINT16 currentButtonKey;
	GBUINT16 currentButtonStatus = 0;
	GBUINT16 drawFuncButtonStatus = 0;
	GBIMEPoint winPos;
	//GBRECT screenRect,imageRect;
	GBUINT16 iCandidateBoxHeight = 0;
	GBUINT16 iKeyBoardHeight = 0;
	PLayoutInfo pLayoutInfo = (PLayoutInfo)(&pltRoot->m_ltData); // Layout UI data point

	// ȡ�ü�����ʾ�Ŀ�ʼλ��[���½�����]
	Global_GetWinPos(&winPos);
	iCurrentLayoutID = GET_LAYOUT_ID(pltRoot);
	// ���̸߶�
	iKeyBoardHeight = pltRoot->m_pfcGetLayoutHeight(pltObj, GET_LAYOUT_ID(pltRoot));
	// ����õ��������Ͻ�Y����
	winPos.y -= iKeyBoardHeight;
	
	// ��ǰҪ��ʾ�Ĳ�
	iCurrentLayerNumber = pLayoutInfo->ltCurLayer;
	// ȡ����ͼƬ�����Ϣ
	// ��ʾͼƬ�����Ͻ�X����
	iDisplayImageLeftX = (GBUINT16)winPos.x;
	// ��ʾͼƬ�����Ͻ�Y����
	iDisplayImageTopY =  (GBUINT16)winPos.y;
	// ����ͼƬ����״̬�µ�ͼƬID
	iCurrentLayerNomalImageId = pLayoutInfo->pltLayerData[FirstLayer].plycData->lyImageID + IMG_GBIME_SYSTEM;//����ͼƬID��ַ
	// ����ͼƬ����״̬�µ�ͼƬID
	iCurrentLayerDownImageId = pLayoutInfo->pltLayerData[FirstLayer].plycData->lyBKImageID + IMG_GBIME_SYSTEM;//����ͼƬID��ַ
	
	// ��ʾ����ͼƬ����0��]
	PT_DrawImage(iDisplayImageLeftX,iDisplayImageTopY, iCurrentLayerNomalImageId);

	// ��ӡLog��Ϣ
	PT_PrintLogInfo(GBLOG_HANDWRITE, ("***********************ICLayoutViewHandwriteFullScreen_DrawKeyboard() Draw Keyboard\n"));
	PT_PrintLogInfo(GBLOG_HANDWRITE, ("***********************Image ID = %d\n",iCurrentLayerNomalImageId));
	
	// �õ���ť����
	iButtonTotalCount = pLayoutInfo->pltLayerData[FirstLayer].lyBtCount;
	// �����Ƿ��б仯�İ�ť(��������״̬)
	for (iCount = 0; iCount < iButtonTotalCount; iCount++)
	{
		currentButtonKey = pLayoutInfo->pltLayerData[FirstLayer].plycBtData[iCount].btKey;
		currentButtonStatus = pLayoutInfo->pltLayerData[FirstLayer].plyBtChgData[iCount].btStatus;

		switch (currentButtonKey)
		{
		case GBKEY_VK_V9_NUM: ///<����9������
			if ((pltThis->hwRecognizeRangeFlag & GBIME_HW_RECOGNIZE_RANGE_NUMBER) 
				== GBIME_HW_RECOGNIZE_RANGE_NUMBER)
			{
				drawFuncButtonStatus = ButtonStatusDown;			
			}
			else
			{
				drawFuncButtonStatus = ButtonStatusNomal;
			}
			pLayoutInfo->pltLayerData[FirstLayer].plyBtChgData[iCount].btStatus = drawFuncButtonStatus;
			pltViewRoot->m_pfcDrawButtonByStatus(pltObj,
												 iDisplayImageLeftX,
												 iDisplayImageTopY,
												 NULL,
												 iCurrentLayoutID,
												 FirstLayer,
												 currentButtonKey,
												 drawFuncButtonStatus);
			break;
		case GBKEY_VK_V_HANDWRITE_ALL: ///<������дȫ��
			if ((pltThis->hwRecognizeRangeFlag & GBIME_HW_RECOGNIZE_RANGE_ALL) 
				== GBIME_HW_RECOGNIZE_RANGE_ALL)
			{
				drawFuncButtonStatus = ButtonStatusDown;			
			}
			else
			{
				drawFuncButtonStatus = ButtonStatusNomal;
			}
			pLayoutInfo->pltLayerData[FirstLayer].plyBtChgData[iCount].btStatus = drawFuncButtonStatus;
			pltViewRoot->m_pfcDrawButtonByStatus(pltObj,
												 iDisplayImageLeftX,
												 iDisplayImageTopY,
												 NULL,
												 iCurrentLayoutID,
												 FirstLayer,
												 currentButtonKey,
												 drawFuncButtonStatus);
			break;
		case GBKEY_VK_v_HANDWRITE_ENG: ///<������дӢ��
			if ((pltThis->hwRecognizeRangeFlag & GBIME_HW_RECOGNIZE_RANGE_ENGLISH) 
				== GBIME_HW_RECOGNIZE_RANGE_ENGLISH)
			{
				drawFuncButtonStatus = ButtonStatusDown;			
			}
			else
			{
				drawFuncButtonStatus = ButtonStatusNomal;
			}
			pLayoutInfo->pltLayerData[FirstLayer].plyBtChgData[iCount].btStatus = drawFuncButtonStatus;
			pltViewRoot->m_pfcDrawButtonByStatus(pltObj,
												 iDisplayImageLeftX,
												 iDisplayImageTopY,
												 NULL,
												 iCurrentLayoutID,
												 FirstLayer,
												 currentButtonKey,
												 drawFuncButtonStatus);
			break;
		case GBKEY_VK_v_HANDWRITE_TONGYIN: ///<������дͬ��
			if ((pltThis->hwRecognizeRangeFlag & GBIME_HW_RECOGNIZE_RANGE_HOMONYM) 
				== GBIME_HW_RECOGNIZE_RANGE_HOMONYM)
			{
				drawFuncButtonStatus = ButtonStatusDown;			
			}
			else
			{
				drawFuncButtonStatus = ButtonStatusNomal;
			}
			pLayoutInfo->pltLayerData[FirstLayer].plyBtChgData[iCount].btStatus = drawFuncButtonStatus;
			pltViewRoot->m_pfcDrawButtonByStatus(pltObj,
												 iDisplayImageLeftX,
												 iDisplayImageTopY,
												 NULL,
												 iCurrentLayoutID,
												 FirstLayer,
												 currentButtonKey,
												 drawFuncButtonStatus);
			break;
		default:
			// btStatus : ����״̬��0Ϊ����̬��1Ϊ���£�2Ϊ��ɫ��3���ص�ǰ����
			// ����״̬����0ʱ���ͱ�ʾ�б仯�İ�ť
			if (pLayoutInfo->pltLayerData[FirstLayer].plyBtChgData[iCount].btStatus != ButtonStatusNomal)
			{
				pLayoutInfo->pltLayerData[FirstLayer].plyBtChgData[iCount].btStatus = ButtonStatusNomal;				
				
				// ����б仯�İ�ť[��ѡ��ť�����밴ť���Դ�����]
				if (GBKEY_VK_V_HANDWRITE_CANDIDATE_AREA != currentButtonKey
					&& GBKEY_VK_V_HANDWRITE_ASSOCIATE_AREA != currentButtonKey)
				{
					pltViewRoot->m_pfcDrawButtonByStatus(pltObj,
														 iDisplayImageLeftX,
														 iDisplayImageTopY,
														 NULL,
														 iCurrentLayoutID,
														 FirstLayer,
														 currentButtonKey,
														 currentButtonStatus);
				}
			}
			break;
		}		
	}	
}


/*!
 * \brief ������д��ѡ
 * \param pltObj 
 * \param pEngineOutputInfo 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-8-12 11:49:48
*/
static void ICLayoutViewHandwriteFullScreen_DrawCandidate(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo)
{
	CLASE_THIS_POITER(CLayoutHandwriteFullScreen, pltObj)
	CLASS_VIEW_THIS_POITER(CLayoutViewHandwriteFullScreen, pltThis)
	GBUINT16 i;
	GBPCWCHAR pCand = NULL;

	if (pltThis->candidateTouchRegion.candidateNum == 0)
	{
		return ;
	}

	// ����ѡ
	for (i = 0; i < pltThis->candidateTouchRegion.candidateNum; i++)
	{		
		// �Ƿ����ֺ�ѡ��
		if (GBHW_GetNumberStatus(Global_GetHWEnineInstance()))
		{
			pCand = pltThis->pNumberCandidates[i];
		}
		else
		{
			pCand = GBHW_GetCandByIndex(Global_GetHWEnineInstance(), i);
		}

		// ���ƺ�ѡ
		if (pltThis->nSelectedCandidateIndex == i)
		{		
			// ѡ���ѡ���·���
			if (pltRoot->buttonStatus == ButtonStatusDown
				&& pltThis->bUpdateCandidate)
			{
				GBIMEPoint layoutWinStartPos;
				
				pltRoot->m_pfcGetWinPos(pltObj, &layoutWinStartPos);
				pltViewRoot->m_pfcDrawButtonByStatus(pltObj, 
					layoutWinStartPos.x,
					layoutWinStartPos.y,
					&pltThis->candidateTouchRegion.candidateRectArray[i],
					GET_LAYOUT_ID(pltRoot), 
					FirstLayer, 
					GBKEY_VK_V_HANDWRITE_CANDIDATE_AREA, 
					ButtonStatusDown);
			}

			// ���ƽ����ѡ
			PT_DrawString(pCand, GBIME_CANDIDATE_FONT_SIZE, GBIME_CANDIDATE_SELECTED_FONT_COLOR, 
					      (GBINT)pltThis->candidateTouchRegion.candidatePosArray[i].x, 
						  (GBINT)pltThis->candidateTouchRegion.candidatePosArray[i].y);
		}
		else
		{
			// ���ƺ�ѡ
			PT_DrawString(pCand, GBIME_CANDIDATE_FONT_SIZE, GBIME_CANDIDATE_NORMAL_FONT_COLOR, 
						  (GBINT)pltThis->candidateTouchRegion.candidatePosArray[i].x, 
						  (GBINT)pltThis->candidateTouchRegion.candidatePosArray[i].y);
		}
	}
}

/*!
 * \brief ������д�����
 * \param pltObj 
 * \param pEngineOutputInfo 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-8-12 11:50:02
*/
static void ICLayoutViewHandwriteFullScreen_DrawAssocWord(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo)
{
	CLASE_THIS_POITER(CLayoutHandwriteFullScreen, pltObj)
	CLASS_VIEW_THIS_POITER(CLayoutViewHandwriteFullScreen, pltThis)
	GBUINT16 i;
	GBPCWCHAR pCand = NULL;
	
	if (pltThis->associateTouchRegion.candidateNum == 0)
	{
		return ;
	}
	
	// ����ѡ
	for (i = 0; i < pltThis->associateTouchRegion.candidateNum; i++)
	{
		// �Ƿ����ֺ�ѡ��
		if (GBHW_GetNumberStatus(Global_GetHWEnineInstance()))
		{
			pCand = pltThis->pNumberCandidates[i + 5];
		}
		else
		{
			pCand = pEngineOutputInfo->ppCandidates[i];
		}

		// ���ƺ�ѡ
		if (pltThis->nSelectedAssociateWordIndex == i)
		{	
			// ѡ���ѡ���·���
			if (pltRoot->buttonStatus == ButtonStatusDown
				&& pltThis->bUpdateAssocWord)
			{
				GBIMEPoint layoutWinStartPos;
				
				pltRoot->m_pfcGetWinPos(pltObj, &layoutWinStartPos);
				pltViewRoot->m_pfcDrawButtonByStatus(pltObj, 
					layoutWinStartPos.x,
					layoutWinStartPos.y,
					&pltThis->associateTouchRegion.candidateRectArray[i],
					GET_LAYOUT_ID(pltRoot), 
					FirstLayer, 
					GBKEY_VK_V_HANDWRITE_ASSOCIATE_AREA, 
					ButtonStatusDown);
			}

			// ���ƽ����ѡ
			PT_DrawString(pCand, GBIME_CANDIDATE_FONT_SIZE, GBIME_CANDIDATE_SELECTED_FONT_COLOR, 
				(GBINT)pltThis->associateTouchRegion.candidatePosArray[i].x, 
				(GBINT)pltThis->associateTouchRegion.candidatePosArray[i].y);
		}
		else
		{
			// ���ƺ�ѡ
			PT_DrawString(pCand, GBIME_CANDIDATE_FONT_SIZE, GBIME_CANDIDATE_NORMAL_FONT_COLOR, 
				(GBINT)pltThis->associateTouchRegion.candidatePosArray[i].x, 
				(GBINT)pltThis->associateTouchRegion.candidatePosArray[i].y);
		}
	}
}
#endif // defined(__GBIME_HANDWRITE__)

