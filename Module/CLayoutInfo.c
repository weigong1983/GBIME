/***************************************************************************
* File Name    :  CLayoutInfo.c
* Corporation  :  Guobi Technology Ltd. GuangZhou China
* Copyright    :  Copyright Guobi Technology Ltd. And Subject to Confidentiality 
*                 Restrictions. This document is the confidential and proprietary 
*                 property of Guobi Technology Ltd.It is neither the document
*                 itself nor any part of it can be shared with any party  without 
*                 the written  permission. 
* Author       :  fanbo
* Date         :  2010-06-08
* Purpose      :  ��װ��������
* Version      :  V1.0
* Last Changed List
* Name         |   date   |  Discription
****************************************************************************/
#include "PlatformTools.h"
#include "CLayoutInfo.h"
#include "gbft_def.h"
#include "gbdef.h"

///<start [haj add 2010-8-27]
const MAPPING_LAYOUTKEY2LAYOUTID GBCMappingLayoutKey2ID[] = {
	///<����ȫ����Layout��ֵӳ�䵽LayouID
	GBKEY_VK_V26_PINYIN, LayoutIdSP26JPinYin
		,GBKEY_VK_V26_STROKE, LayoutIdSP26JStroke
		,GBKEY_VK_V9_NUM, LayoutIdSP9JNumber
		,GBKEY_VK_V9_FS_HANDWRITE, LayoutIdSPQuanPingShouXiue
		,GBKEY_VK_V26_ENG_LOWER, LayoutIdSP26JLowerEnglish		
		,GBKEY_VK_V26_ABC_LOWER, LayoutIdSP26Jabc
		,GBKEY_VK_V26_ENG_UPPER, LayoutIdSP26JUpEnglish
		,GBKEY_VK_V26_ABC_UPPER, LayoutIdSP26JABC
		,GBKEY_VK_V9_SYM, LayoutIdSP9JSymbol
		
		///<����ż�Layout��ֵӳ�䵽LayouID
		,GBKEY_VK_V9_PINYIN, LayoutIdSP9JPinYin
		,GBKEY_VK_V9_STROKE, LayoutIdSP9JStroke
		,GBKEY_VK_V9_NUM, LayoutIdSP9JNumber
		,GBKEY_VK_V9_FS_HANDWRITE, LayoutIdSPQuanPingShouXiue
		,GBKEY_VK_V9_ENG_LOWER, LayoutIdSP9JLowerEnglish
		,GBKEY_VK_V9_ABC_LOWER, LayoutIdSP9Jabc
		,GBKEY_VK_V9_ENG_UPPER, LayoutIdSP9JUpEnglish
		,GBKEY_VK_V9_ABC_LOWER, LayoutIdSP9JABC
		,GBKEY_VK_V9_SYM, LayoutIdSP9JSymbol
		
		///<�����ؼ��л�����һ��Layout
		,GBKEY_VK_BACKWORD, LayoutIdSwitchPreObj
		,GBKEY_VK_LATEST_KEYBOARD, LayoutIdSwitchLatestObj
		,0,0
}; //GBCMappingLayoutKey2ID
///<end [haj add 2010-8-27]

typedef struct __tagMappingUpperLowerLayoutID{
	GBUINT16 lowerLayoutID;
	GBUINT16 upperLayoutID;
}MappingUpperLowerLayoutID, * PMappingUpperLowerLayoutID;

const MappingUpperLowerLayoutID GBUpperLowerLayoutIDMapping[] = {
	{LayoutIdSP9JLowerEnglish,		LayoutIdSP9JUpEnglish}
	,{LayoutIdSP9Jabc,				LayoutIdSP9JABC}
	,{LayoutIdSP26JLowerEnglish,	LayoutIdSP26JUpEnglish}
	,{LayoutIdSP26Jabc,				LayoutIdSP26JABC}
};

/*!
* \brief �������û�ȱʡ����
* \return 
* \note 
* \example
* \author haj
* \date 2010-06-08
*/
static GBUINT16 CLayoutInfo_GetDefaultLayoutID(GBLPCVOID pskinData)
{
	PLayoutBinaryDataIndexInfo pskLtIndex = NULL;

	pskLtIndex = (PLayoutBinaryDataIndexInfo)pskinData;

	return pskLtIndex->ltDefID;
}

static GBLPCLayoutBinaryDataOffset CLayoutInfo_GetLayoutOffsetByID(GBLPCVOID pskinData, GBUINT16 ltID)
{
	LayoutBinaryDataIndexInfo ltIndexInfo;
	int ltIndex = 0;

	ltIndexInfo.ltCount = *(GBPUINT16)pskinData;
	ltIndexInfo.ltOffsetArray = (GBLPCLayoutBinaryDataOffset)((GBPACHAR)pskinData + 
		sizeof(LayoutBinaryDataIndexInfo)-sizeof(GBLPCLayoutBinaryDataOffset));
	while ( ltIndex < ltIndexInfo.ltCount)
	{
		if (ltIndexInfo.ltOffsetArray[ltIndex].ltID == ltID)
		{
			break;
		}
		ltIndex++;
	}

	if (ltIndex < ltIndexInfo.ltCount) 
	{
		return &ltIndexInfo.ltOffsetArray[ltIndex];
	}
	///< Ƥ��������û�����Layout ID
	return NULL;
}

/*!
* \brief ���layout ����
* \param id 
* \return 
* \note 
* \example
* \author haj
* \date 2010-06-08
*/
GBUINT16 CLayoutInfo_GetLayoutTypeByID(GBLPCVOID pskinData, GBUINT16 ltID)
{
	GBLPCLAYOUTCONSTINFO pltData = NULL;
	pltData = CLayoutInfo_GetLayoutDataByID(pskinData, ltID);

	//return LayoutVK;
	return pltData->ltType;
} //CLayoutInfo_GetLayoutTypeByID

/*!
* \brief ���layout ����
* \param id 
* \return 
* \note 
* \example
* \author haj
* \date 2010-06-08
*/
GBUINT16 CLayoutInfo_GetLayoutTypeFromLtData(GBLPCLAYOUTCONSTINFO pltData)
{
	PT_Assert(pltData);

	return pltData->ltType;
} //CLayoutInfo_GetLayoutTypeByID

/*!
* \brief ���layout ����ָ��
* \param id 
* \return 
* \note 
* \example
* \author haj
* \date 2010-06-08
*/
GBLPCLAYOUTCONSTINFO CLayoutInfo_GetLayoutDataByID(GBLPCVOID pskinData, GBUINT16 ltID)
{
	GBLPCLAYOUTCONSTINFO pltData = NULL;
	//int ltIndex = 0;
	GBLPCLayoutBinaryDataOffset ltOffset = CLayoutInfo_GetLayoutOffsetByID(pskinData, ltID);
	if (NULL == ltOffset)
	{
		ltOffset = CLayoutInfo_GetLayoutOffsetByID(pskinData, CLayoutInfo_GetDefaultLayoutID(pskinData)); //
	}

	pltData = (GBLPCLAYOUTCONSTINFO)((GBPACHAR)pskinData +
		(ltOffset->ltHighOffset << 16) + ltOffset->ltLowerOffset);

	return pltData;
} //Common_GetLayoutDataByID

/*!
* \brief ���layer ����ָ��
* \param id 
* \return 
* \note 
* \example
* \author haj
* \date 2010-06-08
*/
GBLPCBUTTONCONSTDATA CLayoutInfo_GetLayerButtonData(GBLPCLAYOUTCONSTINFO pltcData, 
													GBUINT16 ltLayerIndex, PLayoutChgtLayer pltLyChgData)
{
	int i = 0;
	GBUINT16 preltBtSize = 0;

	for (i=0; i < ltLayerIndex; i++)
	{
		PT_Assert(pltLyChgData[i].plycData);
		preltBtSize += pltLyChgData[i].plycData->lyBtCount * sizeof(BUTTONCONSTDATA);
	}
	
	return (GBLPCBUTTONCONSTDATA)((GBPACHAR)pltcData
		+ sizeof(LAYOUTCONSTINFO)
		+ (ltLayerIndex + 1) * sizeof(LAYOUTCONSTLAYER) 
		+ preltBtSize);
} //CLayoutInfo_GetLayerButtonData

/*!
* \brief ��ʼ��layout ����
* \param id 
* \return 
* \note 
* \example
* \author haj
* \date 2010-06-08
*/
GBINT32 CLayoutInfo_LayoutInitData(PLayoutInfo pltInfo, GBLPCVOID pltData)
{
	GBUINT16 ltLayerIndex = 0;
	GBUINT16 iAllButtonsOffsetSum = 0;

	pltInfo->pltcData = pltData;//CLayoutInfo_GetLayoutDataByID(pltData, ltID);
	pltInfo->lyLayerCount = pltInfo->pltcData->lyLayerCount;
	pltInfo->ltCurLayer = 0;

	pltInfo->pltLayerData = (PLayoutChgtLayer)PT_Malloc(sizeof(LayoutChgtLayer) * pltInfo->lyLayerCount);
	memset(pltInfo->pltLayerData, 0, sizeof(sizeof(LayoutChgtLayer) * pltInfo->lyLayerCount));


	for (ltLayerIndex=0; ltLayerIndex < pltInfo->lyLayerCount; ltLayerIndex++)
	{
		pltInfo->pltLayerData[ltLayerIndex].plycData = (GBLPCLAYOUTCONSTLAYER)((GBPACHAR)pltInfo->pltcData + 
			sizeof(LAYOUTCONSTINFO) + sizeof(LAYOUTCONSTLAYER) * ltLayerIndex + iAllButtonsOffsetSum);
		pltInfo->pltLayerData[ltLayerIndex].lyBtCount = pltInfo->pltLayerData[ltLayerIndex].plycData->lyBtCount;
		iAllButtonsOffsetSum += (pltInfo->pltLayerData[ltLayerIndex].lyBtCount*sizeof(BUTTONCONSTDATA));
		pltInfo->pltLayerData[ltLayerIndex].plycBtData = CLayoutInfo_GetLayerButtonData(pltInfo->pltcData, 
			ltLayerIndex, pltInfo->pltLayerData);
		pltInfo->pltLayerData[ltLayerIndex].plyBtChgData = (PBUTTONCHGDATA)PT_Malloc(
			pltInfo->pltLayerData[ltLayerIndex].lyBtCount * sizeof(BUTTONCHGDATA));
		memset(pltInfo->pltLayerData[ltLayerIndex].plyBtChgData, 0,
			pltInfo->pltLayerData[ltLayerIndex].lyBtCount * sizeof(BUTTONCHGDATA));
	}

	return GBIME_OK;
}

GBINT32 CLayoutInfo_LayoutInitDataEx(PLayoutInfo pltInfo, PSkinData pskInfo, GBUINT16 ltID)
{
	//pskInfo->ltCurID = ltID;
	CLayoutInfo_LayoutInitData(pltInfo, CLayoutInfo_GetLayoutDataByID(pskInfo->pskcData, ltID));

	return GBIME_OK;
} //CLayoutInfo_InitData


/*!
* \brief �ͷ�layout ����
* \param id 
* \return 
* \note 
* \example
* \author haj
* \date 2010-06-08
*/
GBINT32 CLayoutInfo_LayoutReleaseData(PLayoutInfo pltInfo)
{
	GBUINT16 ltLayerIndex = 0;

	if (pltInfo == NULL)
	{
		return GBIME_INVALID_PARAM; 
	}

	if (pltInfo->pltLayerData != NULL) // �������Layout������Ŀ��������ڴ��쳣(�����жϱ���)
	{
		for (ltLayerIndex=0; ltLayerIndex < pltInfo->lyLayerCount; ltLayerIndex++)
		{
			if (pltInfo->pltLayerData[ltLayerIndex].plyBtChgData != NULL)
			{
				PT_Free(pltInfo->pltLayerData[ltLayerIndex].plyBtChgData);
				pltInfo->pltLayerData[ltLayerIndex].plyBtChgData = NULL;
			}
		}

		PT_Free(pltInfo->pltLayerData);
		pltInfo->pltLayerData = NULL;
	}
	else
	{
		PT_Assert(0);
	}

	// ����Layout��Ϣ
	memset(pltInfo, 0, sizeof(LayoutInfo));
	return GBIME_OK;
}
GBINT32 CLayoutInfo_SkinReleaseData(PSkinData pskInfo)
{
#ifdef Layout_Test //Layout������Ҫ��̬����
	GBUINT16 ltIndex = 0;
	for (ltIndex=0; ltIndex < pskInfo->ltCount; ltIndex++)
	{
		CLayoutInfo_LayoutReleaseData(&pskInfo->pskLtData[ltIndex]);
	}

	PT_Free(pskInfo->pskLtData);
	pskInfo->pskLtData = NULL;
#endif
	return GBIME_OK;
} //CLayoutInfo_ReleaseData


/*!
* \brief ��ʼ��layout ����
* \param id 
* \return 
* \note 
* \example
* \author haj
* \date 2010-06-08
*/
GBINT32 CLayoutInfo_SkinInitData(PSkinData pskInfo, GBPCVOID pskcData)
{
#ifdef Layout_Test //Layout������Ҫ��̬����
	GBUINT16 ltIndex = 0;
#endif

	PLayoutBinaryDataIndexInfo pskLtIndex = NULL;
	GBLPCLayoutBinaryDataOffset pltOffsetArray = NULL;

	pskLtIndex = (PLayoutBinaryDataIndexInfo)pskcData;
	pltOffsetArray = (GBLPCLayoutBinaryDataOffset)((GBPACHAR)pskcData + 
		sizeof(LayoutBinaryDataIndexInfo) - sizeof(GBPVOID));
	pskInfo->pskcData = pskcData;
	pskInfo->ltCount = pskLtIndex->ltCount;
	pskInfo->ltDefID= pskLtIndex->ltDefID;

// 	pskInfo->ltCurID = 0;
// 	pskInfo->ltIMSub = 0;
// 	pskInfo->ltIMLang = 0;
// 	pskInfo->fpGetLayoutDataByID = NULL;
#ifdef Layout_Test //Layout������Ҫ��̬����
	pskInfo->pskLtData = (PLayoutInfo)PT_Malloc(sizeof(LayoutInfo) * pskLtIndex->ltCount);
	memset(pskInfo->pskLtData, 0, sizeof(LayoutInfo) * pskLtIndex->ltCount);


	for (ltIndex=0; ltIndex < pskInfo->ltCount; ltIndex++)
	{
		CLayoutInfo_LayoutInitDataEx(&pskInfo->pskLtData[ltIndex], pskInfo, pltOffsetArray[ltIndex].ltID);
	}
#endif
//	pskInfo->ltCurID = pskInfo->ltDefID;
	return GBIME_OK;
}
/************************************************************************
���ܣ�
��Ƥ�������У�����ͼ��IDȡͼ����Ϣ
������
pskinData��ָ��Ƥ�����ݵ�ָ��
layoutId��ͼ��ID
************************************************************************/
GBLPCLAYOUTCONSTINFO CLayoutInfo_GetLayoutConstInfo(GBLPCVOID pskinData,GBUINT16 layoutId)
{
	GBLPCLAYOUTCONSTINFO pltData = NULL;
	//int ltIndex = 0;
	GBLPCLayoutBinaryDataOffset ltOffset;
	ltOffset = CLayoutInfo_GetLayoutOffsetByID(pskinData, layoutId);
	if (NULL == ltOffset)
	{
		ltOffset = CLayoutInfo_GetLayoutOffsetByID(pskinData, CLayoutInfo_GetDefaultLayoutID(pskinData)); //
	}
	
	pltData = (GBLPCLAYOUTCONSTINFO)((GBPACHAR)pskinData +
		(ltOffset->ltHighOffset << 16) + ltOffset->ltLowerOffset);
	
	return pltData;
}
/************************************************************************
���ܣ�
��Ƥ�������У�ȡͼ����ĳһ�����Ϣ
������
pskinData��ָ��Ƥ�����ݵ�ָ��
layoutId��ͼ��ID
layerId�ǲ�ID
************************************************************************/
PLAYOUTCONSTLAYER CLayoutInfo_GetLayerConstInfo(GBLPCVOID pskinData,GBUINT16 layoutId,GBUINT16 layerId)

{
	GBLPCLAYOUTCONSTINFO pLayoutConstInfo;
	GBUINT16 iLayerTotalCount;
	GBUINT16 iCount;
	GBUINT16 iButtonOffsetSum;
	GBUINT8 * pVoid;
	PLAYOUTCONSTLAYER pLayer;
	//�õ�ָ��ͼ��Ŀ�ʼλ��
	pLayoutConstInfo = CLayoutInfo_GetLayoutConstInfo(pskinData,layoutId);
	if (pLayoutConstInfo)
	{
		//�жϲ����Ƿ���Ч
		if ((layerId >= FirstLayer) && (layerId <= FourthLayer))
		{
			;
		}
		else
		{
			layerId = FirstLayer;
		}
		//�õ�ͼ��Ĳ���
		iLayerTotalCount = pLayoutConstInfo->lyLayerCount;
		pVoid = (GBUINT8 *)pLayoutConstInfo;
		//���в㰴ť����ƫ����
		iButtonOffsetSum = 0;
		//һ��ͼ������ж����[1---3��]
		for (iCount=0;iCount < iLayerTotalCount;iCount++)
		{
			//ָ���iCount��������ݿ�ʼ��
			pLayer=(PLAYOUTCONSTLAYER)(pVoid + sizeof(LAYOUTCONSTINFO) + iCount*sizeof(LAYOUTCONSTLAYER)+iButtonOffsetSum);
			//�ۼӰ�ťƫ����
			iButtonOffsetSum+=(sizeof(BUTTONCONSTDATA)*pLayer->lyBtCount);
			//�ҵ���Ӧ�Ĳ�ʱ
			if (iCount == layerId)
			{
				return pLayer;
			}
		}
		return NULL;
	}
	else
	{
		return NULL;
	}
}
/************************************************************************
���ܣ�
��Ƥ�������У�ͼ����ĳһ��İ�ť��Ϣ
������
pskinData��ָ��Ƥ�����ݵ�ָ��
layoutId��ͼ��ID
layerId�ǲ�ID
************************************************************************/
PBUTTONCONSTDATA CLayoutInfo_GetLayerConstButtons(GBLPCVOID pskinData,GBUINT16 layoutId,GBUINT16 layerId)
{
	PLAYOUTCONSTLAYER pLayerConstInfo;
	GBUINT8 * pVoid;
	PBUTTONCONSTDATA pButtons;
	pLayerConstInfo = CLayoutInfo_GetLayerConstInfo(pskinData,layoutId,layerId);
	if (pLayerConstInfo)
	{
		//ָ���Ŀ�ʼλ��
		pVoid = (GBUINT8 *)pLayerConstInfo;
		//����Ϣ������ǰ�ť����
		pButtons = (PBUTTONCONSTDATA)(pVoid + sizeof(LAYOUTCONSTLAYER));
		return pButtons;
	}
	else
	{
		return NULL;
	}
}

/************************************************************************
���ܣ�
��������̵ļ�ֵת��Ϊ����ļ�ֵ
������

************************************************************************/
GBUINT16 CLayoutInfo_ConversionLayoutKeyToEngineKey(GBUINT16 iLayoutKeyValue)
{
	GBUINT16 iEngineKeyValue;
	iEngineKeyValue = GBEvent_None;
	//GBKEY_VK_A_UPPER��������̼�ֵ����Ŀ�ʼ��С�ڸ�ֵʱ�������¼�ֵ������ת��
	if ((iLayoutKeyValue > GBEvent_None)&&(iLayoutKeyValue < GBKEY_VK_A_UPPER))
	{
		return iLayoutKeyValue;
	}
	else//��������̼�ֵʱ
	{
		//��д��ĸ
		if ((iLayoutKeyValue >= GBKEY_VK_A_UPPER)&&(iLayoutKeyValue <= GBKEY_VK_Z_UPPER))
		{
			//���治�����д
			iEngineKeyValue = 'a' + iLayoutKeyValue - GBKEY_VK_A_UPPER;
			//�����ｫ���������Ϊ��д
		}
		else if ((iLayoutKeyValue >= GBKEY_VK_1_NUMBERKEY)&&(iLayoutKeyValue <= GBKEY_VK_10_NUMBERKEY))//���̰���
		{
			iEngineKeyValue = '0' + (iLayoutKeyValue - GBKEY_VK_1_NUMBERKEY +1)%10;
		}
		//Note:haj add ò�������⣬GBKEY_VK_BACKWORD ��һ��UI����ֵ����ô��ת��ΪGBKEY_RETURN ����ļ�ֵ�ء�
		else if (iLayoutKeyValue == GBKEY_VK_BACKWORD)//���ؼ�
		{
			iEngineKeyValue = GBKEY_RETURN;
		}
		else if (iLayoutKeyValue == GBKEY_VK_V9_SYMBOL_ADD)//��������ģʽ"+"��
		{
			iEngineKeyValue = '+';
		}
		else if (iLayoutKeyValue == GBKEY_VK_V9_SYMBOL_AT)//��������ģʽ"@"��
		{
			iEngineKeyValue = '@';
		}
		else if (iLayoutKeyValue == GBKEY_VK_V9_SYMBOL_DOLL)//��������ģʽ"$"��
		{
			iEngineKeyValue = '$';
		}
		else if (iLayoutKeyValue == GBKEY_VK_V9_SYMBOL_PERCENT)//��������ģʽ"%"��
		{
			iEngineKeyValue = '%';
		}
		else if (iLayoutKeyValue == GBKEY_VK_V9_SYMBOL_POINT)//��������ģʽ"."��
		{
			iEngineKeyValue = '.';
		}
		else if (iLayoutKeyValue == GBKEY_VK_V9_SYMBOL_SUBTRACT)//��������ģʽ"-"��
		{
			iEngineKeyValue = '-';
		}
		else if (iLayoutKeyValue == GBKEY_VK_V9_SYMBOL_COMMA)//��������ģʽ","��
		{
			iEngineKeyValue = ',';
		}
		else if (iLayoutKeyValue == GBKEY_VK_V9_PINYIN_SEPARATOR)//ƴ���ָ�����
		{
			iEngineKeyValue = ',';
		}
		else if (iLayoutKeyValue == GBKEY_VK_V9_SYMBOL_FULL_STOP)//���
		{
			iEngineKeyValue = 0x3002;
		}
		
	}

	return iEngineKeyValue;
}
/************************************************************************
���ܣ�
ȡ�ú�ѡ��ͼ�ĸ߶�[��ѡ��Ҳ��һ��layout]
������

************************************************************************/
GBUINT16 CLayoutInfo_GetLayoutHeight(GBLPCVOID pskinData,LayoutIdEnum layoutId)
{
	PLAYOUTCONSTLAYER pLayerConst;
	GBUINT16 iCandidateBoxHeight = 0;
	pLayerConst = CLayoutInfo_GetLayerConstInfo(pskinData, (GBUINT16)layoutId, (GBUINT16)FirstLayer);
	if (pLayerConst != NULL)
	{
		iCandidateBoxHeight = pLayerConst->lyPos.bottom - pLayerConst->lyPos.top + 1;
	}
	return iCandidateBoxHeight;
}
/************************************************************************
���ܣ�
ȡ�ú�ѡ��ͼ�Ŀ��[��ѡ��Ҳ��һ��layout]
������

************************************************************************/
GBUINT16 CLayoutInfo_GetLayoutWidth(GBLPCVOID pskinData,LayoutIdEnum layoutId)
{
	PLAYOUTCONSTLAYER pLayerConst;
	GBUINT16 iCandidateBoxWidth = 0;
	pLayerConst = CLayoutInfo_GetLayerConstInfo(pskinData, (GBUINT16)layoutId, (GBUINT16)FirstLayer);
	if (pLayerConst != NULL)
	{
		iCandidateBoxWidth = pLayerConst->lyPos.right - pLayerConst->lyPos.left;
	}
	return iCandidateBoxWidth;
}
/************************************************************************
���ܣ�
���ݰ�ť��ֵ,ȡ�ð�ť��ͼ���е�����
������

************************************************************************/
GBBOOL CLayoutInfo_GetBottonRectByKey(GBLPCVOID pskinData,GBUINT16 layoutId,GBUINT16 layerId,GBUINT16 iKeyValue,PGBRECT pRect)
{
	PBUTTONCONSTDATA pButtonConstData;
	PLAYOUTCONSTLAYER pLayerConstInfo;
	GBUINT16 iCount;
	//ȡ�ò���Ϣ
	pLayerConstInfo = CLayoutInfo_GetLayerConstInfo(pskinData,layoutId,layerId);
	//ȡ�ò��Ӧ�İ�ť����
	pButtonConstData = CLayoutInfo_GetLayerConstButtons(pskinData,layoutId,layerId);
	if (pLayerConstInfo == NULL || pButtonConstData == NULL)
	{
		return GBFalse;
	}
	for (iCount = 0;iCount < pLayerConstInfo->lyBtCount;iCount++)
	{
		if (pButtonConstData[iCount].btKey == iKeyValue)
		{
// 			pRect->left = pButtonConstData[iCount].btPos.left;
// 			pRect->top = pButtonConstData[iCount].btPos.top;
// 			pRect->right = pButtonConstData[iCount].btPos.right;
// 			pRect->bottom = pButtonConstData[iCount].btPos.bottom;
			memcpy(pRect, &pButtonConstData[iCount].btPos, sizeof(GBRECT));
			return GBTrue;
		}
	}
	return GBFalse;
}

/*!
 * \brief ����LayoutID��ȡ�����л�����ģʽ�л���Ҫ����Ϣ
 * \param pSkinData 
 * \param layoutId 
 * \param pGBInputMode 
 * \param pLang 
 * \param pSubLang 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-8-20 12:55:50
*/
GBIMEReturn CLayoutInfo_GetGBInputModeInfoByLayoutID(GBLPCVOID pSkinData,
													 GBUINT16 layoutId, 
													 GBUINT16 *pGBInputMode, 
													 GBUINT16 *pLang, 
													 GBUINT16 *pSubLang)
{
	GBLPCLAYOUTCONSTINFO pLayoutConstInfo;

	PT_Assert(pSkinData != NULL);
	PT_Assert(pGBInputMode != NULL);
	PT_Assert(pLang != NULL);
	PT_Assert(pSubLang != NULL);

	pLayoutConstInfo = CLayoutInfo_GetLayoutConstInfo(pSkinData, layoutId);
	*pGBInputMode = pLayoutConstInfo->ltIMMode;
	*pLang = pLayoutConstInfo->ltIMLang;
	*pSubLang = pLayoutConstInfo->ltIMSubLang;
	return GBIME_OK;
}
//���ݰ�ť��ֵ�ж��Ƿ��ǹ��ܼ�
GBBOOL CLayoutInfo_ThisKeyValueIsFunctionKey(GBUINT16 iLayoutKeyValue)
{
	//�����ֵʱ
	if ((iLayoutKeyValue > GBEvent_None)&&(iLayoutKeyValue < GBKEY_VK_A_UPPER))
	{
		if (((iLayoutKeyValue >= GBKEY_0) && (iLayoutKeyValue <= GBKEY_9))//���ּ�ʱ
			||((iLayoutKeyValue >= GBKEY_A) && (iLayoutKeyValue <= GBKEY_Z)))//��ĸ��ʱ
		{
			return GBFalse;
		}
		return GBTrue;
	}
	else//��������̼�ֵʱ
	{
		if (((iLayoutKeyValue >= GBKEY_VK_A_UPPER)&&(iLayoutKeyValue <= GBKEY_VK_10_NUMBERKEY)))
		{
			return GBFalse;
		}
		else if(iLayoutKeyValue >= GBKEY_VK_V9_SYMBOL_ADD 
			&& iLayoutKeyValue <= GBKEY_VK_V9_SYMBOL_FULL_STOP)
		{
			return GBFalse;
		}
		return GBTrue;
	}
}
//��ȡ��ѡ��߶�
GBINT16 CLayoutInfo_GetCandidateBoxHeight(GBLPCVOID pskinData,GBUINT16 layoutId,GBUINT16 layerId)
{
	GBINT16 iCandidateBoxHeight = 0;
	GBLPCLAYOUTCONSTINFO pLayoutConstInfo;
	PLAYOUTCONSTLAYER pFirstLayer,pOtherLayer;
	//�õ�ָ��ͼ��Ŀ�ʼλ��
	pLayoutConstInfo = CLayoutInfo_GetLayoutConstInfo(pskinData,layoutId);
	if (pLayoutConstInfo != NULL)
	{
		
		if ((layerId >= pLayoutConstInfo->lyLayerCount)//�����ڸò��ʱ
			|| (pLayoutConstInfo->lyLayerCount == 1))//ֻ��һ��ʱ��û�к�ѡ��
		{
			return 0;
		}
		if (layerId == FirstLayer)
		{
			pFirstLayer = CLayoutInfo_GetLayerConstInfo(pskinData,layoutId,FirstLayer);
			pOtherLayer = CLayoutInfo_GetLayerConstInfo(pskinData,layoutId,SecondLayer);
		}
		else
		{
			pFirstLayer = CLayoutInfo_GetLayerConstInfo(pskinData,layoutId,FirstLayer);
			pOtherLayer = CLayoutInfo_GetLayerConstInfo(pskinData,layoutId,layerId);
		}
		if (pFirstLayer == NULL || pOtherLayer == NULL)
		{
			return 0;
		}
		//�������Ǻ�ѡ��߶�
		iCandidateBoxHeight = pOtherLayer->lyPos.bottom - pFirstLayer->lyPos.bottom;
		return iCandidateBoxHeight;
	}
	return iCandidateBoxHeight;
}

/*!
 * \brief ��Layout��Ϣ�л�ȡLayoutID
 * \param pltInfo 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-9 17:24:48
*/
GBUINT16 CLayoutInfo_GetLayoutID(PLayoutInfo pltInfo)
{
	PT_Assert(pltInfo != NULL);
	return pltInfo->pltcData->ltID;
}

/*!
 * \brief ���ݻ���������Ŀ���л�LayoutID
 * \param pltInfo 
 * \param dir 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-24 14:19:32
*/
GBUINT16 CLayoutInfo_GetSlidingSwitchLayoutID(PLayoutInfo pltInfo, GBSlidingDirection dir)
{
	GBUINT16 switchLayoutID;

	PT_Assert(pltInfo != NULL);

	switch (dir)
	{
	case SLIDING_DIRECTION_UP:///<��
		switchLayoutID = pltInfo->pltcData->ltUPSwtID;
		break;
	case SLIDING_DIRECTION_DOWN:///<��
		switchLayoutID = pltInfo->pltcData->ltDownSwtID;
		break;
	case SLIDING_DIRECTION_LEFT:///<��
		switchLayoutID = pltInfo->pltcData->ltLeftSwtID;
		break;
	case SLIDING_DIRECTION_RIGHT:///<��
		switchLayoutID = pltInfo->pltcData->ltRightSwtID;
		break;
	default:
		PT_Assert(0); // �Ƿ�����
		break;
	}

	return switchLayoutID;
}

/*!
 * \brief ��ȡLayoutID��Ӧ�ļ�������
 * \param layoutID 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-29 9:56:33
*/
KEYBOARD_TYPE CLayoutInfo_GetKeyboardTypeByLayoutID(GBUINT16 layoutID)
{
	KEYBOARD_TYPE keyboardType = KEYBOARD_VK_NUMPAD;
	
	switch (layoutID)
	{
	case LayoutIdSP9JPinYin: //����9��ƴ��
	case LayoutIdSP9JStroke: //����9���ʻ�
	case LayoutIdSP9JLowerEnglish: //����9��СдӢ��
	case LayoutIdSP9JUpEnglish: //����9����дӢ��
	case LayoutIdSP9Jabc: //����9��Сд��ĸ
	case LayoutIdSP9JABC: //����9����д��ĸ
	case LayoutIdSP9JNumber: //����9������
	case LayoutIdSP9JSymbol: //����9������
		keyboardType = KEYBOARD_VK_NUMPAD;
		break;	
	case LayoutIdSP26JPinYin: //����26��ƴ��
	case LayoutIdSP26JStroke: //����26���ʻ�
	case LayoutIdSP26JLowerEnglish: //����26��СдӢ��
	case LayoutIdSP26JUpEnglish: //����26����дӢ��
	case LayoutIdSP26Jabc: //����26��Сд��ĸ
	case LayoutIdSP26JABC: //����26����д��ĸ
	case LayoutIdSP26JNumber: //����26������
	case LayoutIdSP26JSymbol: //����26������
		keyboardType = KEYBOARD_VK_QWERTY;
		break;
	case LayoutIdWL9JPinYin:
	case LayoutIdWL9JStroke:
	case LayoutIdWL9JEnglish:
	case LayoutIdWL9JABC:
	case LayoutIdWL9Jabc:
	case LayoutIdWL9JNumber:
		keyboardType = KEYBOARD_KB_NUMPAD;
		break;
	case LayoutIdWL26JPinYin:
	case LayoutIdWL26JStroke:
	case LayoutIdWL26JEnglish:
	case LayoutIdWL26JABC:
	case LayoutIdWL26Jabc:
	case LayoutIdWL26JNumber:
		keyboardType = KEYBOARD_KB_QWERTY;
		break;
	default:
		//PT_Assert(0); // �Ƿ�����
		break;
	}
	
	return keyboardType;
}

/*!
 * \brief ��ȡСдLayoutID��Ӧ�Ĵ�дLayoutID
 * \param lowerLayoutID 
 * \return 
 * \note Ѱ�Ҳ�����������
 * \example
 * \author weizhiping
 * \date 2010-10-21 17:23:09
*/
GBUINT16 CLayoutInfo_GetUpperLayoutID(GBUINT16 lowerLayoutID)
{
	GBUINT16 upperLayoutID = lowerLayoutID;
	GBUINT16 i;

	for (i = 0; i < PT_ARRAY_SIZE(GBUpperLowerLayoutIDMapping); i++)
	{
		if (GBUpperLowerLayoutIDMapping[i].lowerLayoutID == lowerLayoutID)
		{
			upperLayoutID = GBUpperLowerLayoutIDMapping[i].upperLayoutID;
			break;
		}
	}

	return upperLayoutID;
}

/*!
 * \brief ��ȡ��дLayoutID��Ӧ��СдLayoutID
 * \param upperLayoutID 
 * \return 
 * \note Ѱ�Ҳ�����������
 * \example
 * \author weizhiping
 * \date 2010-10-21 17:23:09
*/
GBUINT16 CLayoutInfo_GetLowerLayoutID(GBUINT16 upperLayoutID)
{
	GBUINT16 lowerLayoutID = upperLayoutID;
	GBUINT16 i;

	for (i = 0; i < PT_ARRAY_SIZE(GBUpperLowerLayoutIDMapping); i++)
	{
		if (GBUpperLowerLayoutIDMapping[i].upperLayoutID == upperLayoutID)
		{
			lowerLayoutID = GBUpperLowerLayoutIDMapping[i].lowerLayoutID;
			break;
		}
	}

	return lowerLayoutID;
}

/*!
 * \brief ��ȡָ����ֵ�İ�ť����
 * \param pskinData 
 * \param layoutId 
 * \param layerId 
 * \param iKeyValue 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-10-20 10:13:47
*/
GBWCHAR * CLayoutInfo_GetBottonKeyName(GBLPCVOID pskinData, GBUINT16 layoutId, GBUINT16 layerId, GBUINT16 iKeyValue)
{
	PBUTTONCONSTDATA pButtonConstData;
	PLAYOUTCONSTLAYER pLayerConstInfo;
	GBUINT16 iCount;
	GBWCHAR *pButtonKeyName = NULL;

	// ȡ�ò���Ϣ
	pLayerConstInfo = CLayoutInfo_GetLayerConstInfo(pskinData,layoutId,layerId);
	
	// ȡ�ò��Ӧ�İ�ť����
	pButtonConstData = CLayoutInfo_GetLayerConstButtons(pskinData,layoutId,layerId);

	if (pLayerConstInfo == NULL || pButtonConstData == NULL)
	{
		return NULL;
	}

	for (iCount = 0; iCount < pLayerConstInfo->lyBtCount; iCount++)
	{
		if (pButtonConstData[iCount].btKey == iKeyValue)
		{
			pButtonKeyName = pButtonConstData[iCount].btKeyName;
			return pButtonKeyName;
		}
	}

	return NULL;
}

/*!
 * \brief ����ָ��Layout��ť��״̬(Ӧ�ò�����ô˽ӿڽ���ָ����ť)
 * \param pLayoutInfo 
 * \param iKeyValue 
 * \param btStatus 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-12-8 9:48:48
*/
GBBOOL CLayoutInfo_SetButtonStatus(PLayoutInfo pLayoutInfo, GBUINT16 iKeyValue, GBUINT16 btStatus)
{
	PLayoutChgtLayer    pltLayerData;
	GBUINT16			iCount, iLayer;

	// ��������Layer���ҵ�Ŀ��button
	for (iLayer = 0; iLayer < pLayoutInfo->lyLayerCount; iLayer++)
	{
		pltLayerData = &pLayoutInfo->pltLayerData[iLayer];		
		if (pltLayerData == NULL)
		{
			continue; // �ò�������
		}
		
		for (iCount = 0; iCount < pltLayerData->lyBtCount; iCount++)
		{
			if (pltLayerData->plycBtData[iCount].btKey == iKeyValue)
			{
				pltLayerData->plyBtChgData[iCount].btStatus = btStatus;
				break;
			}
		}
	}	
	
	return GBTrue;
}