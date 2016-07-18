/***************************************************************************
* File Name    :  CLayoutViewVKSymbol.c
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

#include "CLayoutViewVKSymbol.h"
#include "CLayoutVKSymbol.h"
#include "PlatformAdapter.h"
#include "GBIMEResDef.h"
#include "GB_Global.h"
#include "CLayoutVKSymbol.h"

/*!
 * \ Layout View ָ��Ԥ����
 */
#define CLASS_VIEW_THIS_POITER(CLASS_NAME, pltThis)   \
	CLASS_NAME * pltViewThis = (CLASS_NAME *)pltThis->m_ltParent.m_ltParent.m_hltView; \
	PCLayoutViewVK pltViewBase = &pltViewThis->m_ltParent; \
	PCLayoutViewAbstract pltViewRoot = &pltViewThis->m_ltParent.m_ltParent;

#define SIGN_COLOR		PT_COLOR_BLACK

GBIMELOCAL GBIMEReturn CLayoutViewVKSymbol__Paint(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo);

// ���غ�������
GBIMELOCAL void ICLayoutViewVKSymbol_DrawKeyboard(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo);
GBIMELOCAL void ICLayoutViewVKSymbol_DrawCurrentPageSymbol(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo);

CLayoutViewAbstract * CLayoutViewVKSymbol__Construct(PCLayoutViewVKSymbol pltViewVKSymbol)
{
	CLayoutViewVK__Construct(&pltViewVKSymbol->m_ltParent);

	CLayoutViewVKSymbol__Init(pltViewVKSymbol);
	return (CLayoutViewAbstract *)pltViewVKSymbol;
}

GBIMEReturn CLayoutViewVKSymbol__Init(PCLayoutViewVKSymbol pltViewVKSymbol)
{
	pltViewVKSymbol->m_ltParent.m_ltParent.m_pfcPaint = CLayoutViewVKSymbol__Paint;
	pltViewVKSymbol->m_ltParent.m_pfcDrawKeyboard = ICLayoutViewVKSymbol_DrawKeyboard;
	return GBIME_OK;
}

GBIMELOCAL GBIMEReturn CLayoutViewVKSymbol__Paint(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo)
{
	CLASE_THIS_POITER(CLayoutVKSymbol, pltObj)
	CLASS_VIEW_THIS_POITER(CLayoutViewVKSymbol, pltThis)

	PLayoutInfo pLayoutInfo = NULL;
	GBRECT layoutWinRect = {0};

	if (LayoutVKSymbol != pltRoot->m_ltType)
	{
		return GBIME_LAYOUT_NOTMATCH;
	}

	pLayoutInfo = (PLayoutInfo)(&pltRoot->m_ltData); // Layout UI data point
	pltViewBase->m_pfcDrawKeyboard(pltObj, pEngineOutputInfo);
	ICLayoutViewVKSymbol_DrawCurrentPageSymbol(pltObj, pEngineOutputInfo);
	pltRoot->m_pfcGetLayoutWinRect(pltObj, &layoutWinRect);
	PT_Invalidate(layoutWinRect.left, layoutWinRect.top, layoutWinRect.right, layoutWinRect.bottom);
	return GBIME_OK;
}

static void ICLayoutViewVKSymbol_DrawKeyboard(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo)
{
	CLASE_THIS_POITER(CLayoutVKSymbol, pltObj)
	CLASS_VIEW_THIS_POITER(CLayoutViewVKSymbol, pltThis)

	//��ѡ����
//	GBUINT16 iCandidateCount;
	//ָ���Ŀ�ʼλ��
	PLAYOUTCONSTLAYER pLayerConstInfo;
	//��ѡ������״̬�µ�ͼƬ
	GBUINT16 iCandidateNomalImageId;
	//��ѡ����״̬�µ�ͼƬ
	GBUINT16 iCandidateDownImageId;
	//��ʾͼƬ�����Ͻ�X����
	GBUINT16 iDisplayImageLeftX;
	//��ʾͼƬ�����Ͻ�Y����
	GBUINT16 iDisplayImageTopY;
	//һ����ѡռ�����ؿ��
//	GBUINT16 iCandidateWidth;
//	GBUINT16 iCurrentCandidateWidth;
	GBUINT16 iKeyBoardHeight = 0;
	GBUINT16 iCandidateBoxHeight = 0;
	GBIMEPoint winPos;
//	GBPCWCHAR pCand;
	GBRECT screenRect,imageRect;
//	GBRECT buttonRect;
	GBUINT16 iButtonTotalCount;
	GBBOOL bHasButtonChange = 0;
	GBUINT16 currentButtonStatus = 0;
	GBUINT16 iChangeButtonIndex = 0;
	GBUINT16 iCurrentButtonImageId = 0;
	GBUINT16 iCount;
	GBUINT16 iCurrentLayoutId;
	GBUINT16 keyValue;
	
	PLayoutInfo pLayoutInfo = (PLayoutInfo)(&pltRoot->m_ltData); // Layout UI data point
		
	pLayerConstInfo = pltRoot->m_pfcGetLayerConstInfo(pltObj, LayoutIdSP9JSymbol,FirstLayer);
	
	if (pLayerConstInfo != NULL)
	{
		iCurrentLayoutId = GET_LAYOUT_ID(pltRoot);
		// ��ȡ���̸߶�
		iKeyBoardHeight = pltRoot->m_pfcGetLayoutHeight(pltObj, iCurrentLayoutId);
		// ȡ�ü�����ʾ�Ŀ�ʼλ�á����½ǡ���������Ϊ�����Ͻǡ�
		Global_GetWinPos(&winPos);	
		winPos.y -=  iKeyBoardHeight;

		iCandidateNomalImageId = pLayerConstInfo->lyImageID + IMG_GBIME_SYSTEM; // ����ͼƬID��ַ;
		iCandidateDownImageId = pLayerConstInfo->lyBKImageID + IMG_GBIME_SYSTEM; // ����ͼƬID��ַ;
		// ��ʾ[��ѡ]ͼƬ�����Ͻ�X����
		iDisplayImageLeftX = (GBUINT16)winPos.x;
		// ��ʾ[��ѡ]ͼƬ�����Ͻ�Y����
		iDisplayImageTopY = (GBUINT16)winPos.y;
		// ��ʾ����ͼƬ
		PT_DrawImage(iDisplayImageLeftX, iDisplayImageTopY, iCandidateNomalImageId);

		// �õ���ť����
		iButtonTotalCount = pLayoutInfo->pltLayerData[FirstLayer].lyBtCount;
		// �����Ƿ��б仯�İ�ť(��������״̬)
		for (iCount = 0;iCount < iButtonTotalCount; iCount++)
		{
			keyValue = GET_BUTTON_KEY_VALUE(pltRoot, FirstLayer, iCount);
			if (GBKEY_VK_SYM_comment == keyValue || GBKEY_VK_SYM_EMOTION == keyValue
				|| GBKEY_VK_SYM_SPECIAL == keyValue || GBKEY_VK_SYM_MATH == keyValue)
			{
				continue; // �������Ͱ�����������
			}

			// btStatus : ����״̬��0Ϊ����̬��1Ϊ���£�2Ϊ��ɫ��3���ص�ǰ����
			// ����״̬����0ʱ���ͱ�ʾ�б仯�İ�ť
			if (pLayoutInfo->pltLayerData[FirstLayer].plyBtChgData[iCount].btStatus != ButtonStatusNomal)
			{
				bHasButtonChange = 1;
				currentButtonStatus = pLayoutInfo->pltLayerData[FirstLayer].plyBtChgData[iCount].btStatus;
				iChangeButtonIndex = iCount;
				pLayoutInfo->pltLayerData[FirstLayer].plyBtChgData[iCount].btStatus = ButtonStatusNomal;
				break;
			}
		}

		// ����б仯�İ�ť
		if (bHasButtonChange)
		{
			// ȡ��ť��ͼƬ�е�λ��
			memcpy(&imageRect, &pLayoutInfo->pltLayerData[FirstLayer].plycBtData[iChangeButtonIndex].btPos, sizeof(GBRECT));
			// ͼƬ����Ļ����ʾ��λ��
			memcpy(&screenRect, &imageRect, sizeof(GBRECT));
			MOVE_RECT(&screenRect, winPos.x, iDisplayImageTopY);
			
			// ���ݰ����Ĳ�ͬ״̬��ȡ����Ӧ��ͼƬID
			if ((currentButtonStatus) == ButtonStatusNomal || (currentButtonStatus == ButtonStatusUp))
			{
				iCurrentButtonImageId = pLayoutInfo->pltLayerData[FirstLayer].plycBtData[iChangeButtonIndex].btNormal.imageID + IMG_GBIME_SYSTEM;//����ͼƬID��ַ
			}
			else if (currentButtonStatus == ButtonStatusDown || currentButtonStatus == ButtonStatusMove)
			{
				iCurrentButtonImageId = pLayoutInfo->pltLayerData[FirstLayer].plycBtData[iChangeButtonIndex].btDown.imageID + IMG_GBIME_SYSTEM;//����ͼƬID��ַ
			}
			else if(currentButtonStatus == ButtonStatusGray)
			{
				iCurrentButtonImageId = pLayoutInfo->pltLayerData[FirstLayer].plycBtData[iChangeButtonIndex].btGrey.imageID + IMG_GBIME_SYSTEM;//����ͼƬID��ַ	
			}
			else
			{
				// Ĭ��
				iCurrentButtonImageId = pLayoutInfo->pltLayerData[FirstLayer].plycBtData[iChangeButtonIndex].btNormal.imageID + IMG_GBIME_SYSTEM;//����ͼƬID��ַ
			}
			PT_DrawImageWithClip(iCurrentButtonImageId,&screenRect,&imageRect);
		}
		if (!pltThis->m_bHasPrvePage)//û����һҳʱ
		{
			pltViewRoot->m_pfcDrawButtonByStatus(pltObj, 
												iDisplayImageLeftX, iDisplayImageTopY, NULL,
												iCurrentLayoutId, FirstLayer, 
												GBKEY_VK_SYM_PAGEUP, ButtonStatusGray);
			
		}
		if (!pltThis->m_bHasNextPage)//û����һҳʱ
		{
			pltViewRoot->m_pfcDrawButtonByStatus(pltObj, 
												iDisplayImageLeftX, iDisplayImageTopY, NULL,
												iCurrentLayoutId, FirstLayer, 
												GBKEY_VK_SYM_PAGEDOWN, ButtonStatusGray);
		}
		if (!pltThis->m_hasFullOrHalfSymbol)//û�а�ǻ�ȫ��ʱ
		{
			pltViewRoot->m_pfcDrawButtonByStatus(pltObj, 
												iDisplayImageLeftX, iDisplayImageTopY, NULL,
												iCurrentLayoutId, FirstLayer, 
												GBKEY_VK_SBC_DBC_SWICH, ButtonStatusGray);
		}
		
		// ����
		if (pltThis->m_bSymbolLock)
		{
			pltViewRoot->m_pfcDrawButtonByStatus(pltObj, 
											iDisplayImageLeftX, iDisplayImageTopY, NULL,
											iCurrentLayoutId, FirstLayer, 
											GBKEY_VK_SYM_LOCK, ButtonStatusDown);
		}

		if ((pltThis->m_hasFullOrHalfSymbol)&&(pltThis->m_commonSymbolType == HalfShaped)) // ��ť�����ǰ�Ƿ���
		{
			pltViewRoot->m_pfcDrawButtonByStatus(pltObj, 
				iDisplayImageLeftX, iDisplayImageTopY, NULL,
				iCurrentLayoutId, FirstLayer, 
				GBKEY_VK_SBC_DBC_SWICH, ButtonStatusDown);
		}

		// ���Ե�ǰ�������Ͱ���������ʾ
		switch (pltThis->m_symbolType)
		{
		case SymbolTypeCommon: // ��ͨ����
			keyValue = GBKEY_VK_SYM_comment;
			break;
		case SymbolTypeFace: // �������
			keyValue = GBKEY_VK_SYM_EMOTION;
			break;
		case SymbolTypeEspecial: // �������
			keyValue = GBKEY_VK_SYM_SPECIAL;
			break;
		case SymbolTypeNumber: // ��ѧ����
			keyValue = GBKEY_VK_SYM_MATH;
			break;
		default:
			PT_Assert(0); // �Ƿ�����
			break;
		}
		pltViewRoot->m_pfcDrawButtonByStatus(pltObj, iDisplayImageLeftX, iDisplayImageTopY, NULL,
											 iCurrentLayoutId, FirstLayer, keyValue, 
											 ButtonStatusDown);

		//��ӡLog��Ϣ
		PT_PrintLogInfo(GBLOG_LAYOUTVIEW, ("***********************ICLayoutViewVKSymbol_DrawLayerOne() Draw Candidate image\n"));
		PT_PrintLogInfo(GBLOG_LAYOUTVIEW, ("***********************Candidate Image ID = %d\n", iCandidateNomalImageId));
	}
}

/*!
 * \brief ��ʾ��ǰҳ��ķ���
 * \param pltObj 
 * \param pEngineOutputInfo 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-8-19 9:45:48
*/
static void ICLayoutViewVKSymbol_DrawCurrentPageSymbol(GBLPVOID pltObj, PEngineOutputInfo pEngineOutputInfo)
{
#if __MTK__ > 0
	CLASE_THIS_POITER(CLayoutVKSymbol, pltObj)

	PLAYOUTCONSTLAYER pLayerConstInfo;
	PBUTTONCONSTDATA pButtonConstData;
	GBUINT16 iCount;
//	GBUINT16 iKeyValue;
	GBIMEPoint winPos;
	GBUINT16 iKeyBoardHeight = 0;
	GBUINT16 iSymbolOffset;
	GBUINT16 iSymbolIndex;
	GBPCWCHAR pSymbolData;
	GBUINT16 iSymbolWidth;
	GBUINT16 iSymbolHeight;
	PT_FontSize font = PT_FONT_SIZE_DEFAULT;

	pLayerConstInfo = pltRoot->m_pfcGetLayerConstInfo(pltObj, LayoutIdSP9JSymbol,FirstLayer);
	pButtonConstData = pltRoot->m_pfcGetLayerConstButtons(pltObj, LayoutIdSP9JSymbol,FirstLayer);

	// ��ȡ���̸߶�
	iKeyBoardHeight = pltRoot->m_ltData.pltLayerData[FirstLayer].plycData->lyPos.bottom;
	// ȡ�ü�����ʾ�Ŀ�ʼλ�á����½ǡ���������Ϊ�����Ͻǡ�
	Global_GetWinPos(&winPos);	
	winPos.y -=  iKeyBoardHeight;

	iSymbolIndex = pltThis->m_iCurrentPageStartIndex;
	pSymbolData = (GBPCWCHAR)pltThis->m_pltSymbolData;

	//�鿴�Ƿ���Ҫ�ı���ŵ�����	
	{
		GBUINT16 iTempSymbolIdex = iSymbolIndex;
		for (;iTempSymbolIdex <= pltThis->m_iCurrentPageEndIndex;iTempSymbolIdex++)
		{
			iSymbolOffset = CLayoutVKSymbol__GetSymbolOffsetByIndex(pltObj, iTempSymbolIdex);
			//ֻҪ��һ�����ų��ȴﵽ3���ַ����ͽ���������С��(����ť�޷���ʾ����)
			if (PT_GetStrLen((GBWCHAR *)(pSymbolData + iSymbolOffset)) >= 3)
			{
				font = PT_FONT_SIZE_SMALL;
				break;
			}
		}
	}

	// Ŀǰ10������ÿҳ,��ʾ��1��2��3.....9��9�����ּ���
	for (iCount = 0;iCount < pLayerConstInfo->lyBtCount; iCount++)
	{
		// ����Ѿ������һ������
		if (iSymbolIndex > pltThis->m_iCurrentPageEndIndex)
		{
			break;
		}

		if (pButtonConstData[iCount].btKey >= GBKEY_VK_1_NUMBERKEY 
			&&  pButtonConstData[iCount].btKey <= GBKEY_VK_9_NUMBERKEY)
		{
			iSymbolOffset = CLayoutVKSymbol__GetSymbolOffsetByIndex(pltObj, iSymbolIndex);
			//�õ�����ռ�Ŀ��
			iSymbolWidth = PT_GetStrWidthEx((GBLPWCHAR)(pSymbolData + iSymbolOffset), font);
			//��ʾ���м�λ��
			iSymbolWidth = (pButtonConstData[iCount].btNormal.rcPos.right - iSymbolWidth)/2;
			if (iSymbolWidth < 0)
			{
				iSymbolWidth = 0;
			}
			iSymbolHeight = PT_GetStrHeightEx((GBLPWCHAR)(pSymbolData + iSymbolOffset), font);
			//��ʾ���м�λ��
			iSymbolHeight = (pButtonConstData[iCount].btNormal.rcPos.bottom - iSymbolHeight)/2;
			if (iSymbolHeight < 0)
			{
				iSymbolHeight = 0;
			}
			PT_DrawString(pSymbolData + iSymbolOffset, font, SIGN_COLOR,
					(GBINT)(pButtonConstData[iCount].btNormal.rcPos.left + winPos.x + iSymbolWidth),
					(GBINT)(pButtonConstData[iCount].btNormal.rcPos.top + winPos.y) + iSymbolHeight);
			iSymbolIndex++;
		}
	}
#endif // __MTK__
}

#endif // �����������