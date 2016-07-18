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
* Purpose      :  封装国笔引擎
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
	///<虚拟全键盘Layout键值映射到LayouID
	GBKEY_VK_V26_PINYIN, LayoutIdSP26JPinYin
		,GBKEY_VK_V26_STROKE, LayoutIdSP26JStroke
		,GBKEY_VK_V9_NUM, LayoutIdSP9JNumber
		,GBKEY_VK_V9_FS_HANDWRITE, LayoutIdSPQuanPingShouXiue
		,GBKEY_VK_V26_ENG_LOWER, LayoutIdSP26JLowerEnglish		
		,GBKEY_VK_V26_ABC_LOWER, LayoutIdSP26Jabc
		,GBKEY_VK_V26_ENG_UPPER, LayoutIdSP26JUpEnglish
		,GBKEY_VK_V26_ABC_UPPER, LayoutIdSP26JABC
		,GBKEY_VK_V9_SYM, LayoutIdSP9JSymbol
		
		///<虚拟九键Layout键值映射到LayouID
		,GBKEY_VK_V9_PINYIN, LayoutIdSP9JPinYin
		,GBKEY_VK_V9_STROKE, LayoutIdSP9JStroke
		,GBKEY_VK_V9_NUM, LayoutIdSP9JNumber
		,GBKEY_VK_V9_FS_HANDWRITE, LayoutIdSPQuanPingShouXiue
		,GBKEY_VK_V9_ENG_LOWER, LayoutIdSP9JLowerEnglish
		,GBKEY_VK_V9_ABC_LOWER, LayoutIdSP9Jabc
		,GBKEY_VK_V9_ENG_UPPER, LayoutIdSP9JUpEnglish
		,GBKEY_VK_V9_ABC_LOWER, LayoutIdSP9JABC
		,GBKEY_VK_V9_SYM, LayoutIdSP9JSymbol
		
		///<按返回键切换到上一个Layout
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
* \brief 根据配置或缺省配置
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
	///< 皮肤数据中没有这个Layout ID
	return NULL;
}

/*!
* \brief 获得layout 类型
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
* \brief 获得layout 类型
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
* \brief 获得layout 数据指针
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
* \brief 获得layer 数据指针
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
* \brief 初始化layout 数据
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
* \brief 释放layout 数据
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

	if (pltInfo->pltLayerData != NULL) // 超过最大Layout保存数目这里存在内存异常(增加判断保护)
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

	// 清零Layout信息
	memset(pltInfo, 0, sizeof(LayoutInfo));
	return GBIME_OK;
}
GBINT32 CLayoutInfo_SkinReleaseData(PSkinData pskInfo)
{
#ifdef Layout_Test //Layout对象需要动态创建
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
* \brief 初始化layout 数据
* \param id 
* \return 
* \note 
* \example
* \author haj
* \date 2010-06-08
*/
GBINT32 CLayoutInfo_SkinInitData(PSkinData pskInfo, GBPCVOID pskcData)
{
#ifdef Layout_Test //Layout对象需要动态创建
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
#ifdef Layout_Test //Layout对象需要动态创建
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
功能：
从皮肤数据中，根据图层ID取图层信息
参数：
pskinData是指向皮肤数据的指针
layoutId是图层ID
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
功能：
从皮肤数据中，取图层中某一层的信息
参数：
pskinData是指向皮肤数据的指针
layoutId是图层ID
layerId是层ID
************************************************************************/
PLAYOUTCONSTLAYER CLayoutInfo_GetLayerConstInfo(GBLPCVOID pskinData,GBUINT16 layoutId,GBUINT16 layerId)

{
	GBLPCLAYOUTCONSTINFO pLayoutConstInfo;
	GBUINT16 iLayerTotalCount;
	GBUINT16 iCount;
	GBUINT16 iButtonOffsetSum;
	GBUINT8 * pVoid;
	PLAYOUTCONSTLAYER pLayer;
	//得到指向图层的开始位置
	pLayoutConstInfo = CLayoutInfo_GetLayoutConstInfo(pskinData,layoutId);
	if (pLayoutConstInfo)
	{
		//判断参数是否有效
		if ((layerId >= FirstLayer) && (layerId <= FourthLayer))
		{
			;
		}
		else
		{
			layerId = FirstLayer;
		}
		//得到图层的层数
		iLayerTotalCount = pLayoutConstInfo->lyLayerCount;
		pVoid = (GBUINT8 *)pLayoutConstInfo;
		//所有层按钮的总偏移量
		iButtonOffsetSum = 0;
		//一个图层可能有多个层[1---3层]
		for (iCount=0;iCount < iLayerTotalCount;iCount++)
		{
			//指向第iCount个层的数据开始处
			pLayer=(PLAYOUTCONSTLAYER)(pVoid + sizeof(LAYOUTCONSTINFO) + iCount*sizeof(LAYOUTCONSTLAYER)+iButtonOffsetSum);
			//累加按钮偏移量
			iButtonOffsetSum+=(sizeof(BUTTONCONSTDATA)*pLayer->lyBtCount);
			//找到对应的层时
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
功能：
从皮肤数据中，图层中某一层的按钮信息
参数：
pskinData是指向皮肤数据的指针
layoutId是图层ID
layerId是层ID
************************************************************************/
PBUTTONCONSTDATA CLayoutInfo_GetLayerConstButtons(GBLPCVOID pskinData,GBUINT16 layoutId,GBUINT16 layerId)
{
	PLAYOUTCONSTLAYER pLayerConstInfo;
	GBUINT8 * pVoid;
	PBUTTONCONSTDATA pButtons;
	pLayerConstInfo = CLayoutInfo_GetLayerConstInfo(pskinData,layoutId,layerId);
	if (pLayerConstInfo)
	{
		//指向层的开始位置
		pVoid = (GBUINT8 *)pLayerConstInfo;
		//层信息后面就是按钮数据
		pButtons = (PBUTTONCONSTDATA)(pVoid + sizeof(LAYOUTCONSTLAYER));
		return pButtons;
	}
	else
	{
		return NULL;
	}
}

/************************************************************************
功能：
将虚拟键盘的键值转化为引擎的键值
参数：

************************************************************************/
GBUINT16 CLayoutInfo_ConversionLayoutKeyToEngineKey(GBUINT16 iLayoutKeyValue)
{
	GBUINT16 iEngineKeyValue;
	iEngineKeyValue = GBEvent_None;
	//GBKEY_VK_A_UPPER是虚拟键盘键值定义的开始，小于该值时是引擎事件值，无需转换
	if ((iLayoutKeyValue > GBEvent_None)&&(iLayoutKeyValue < GBKEY_VK_A_UPPER))
	{
		return iLayoutKeyValue;
	}
	else//是虚拟键盘键值时
	{
		//大写字母
		if ((iLayoutKeyValue >= GBKEY_VK_A_UPPER)&&(iLayoutKeyValue <= GBKEY_VK_Z_UPPER))
		{
			//引擎不处理大写
			iEngineKeyValue = 'a' + iLayoutKeyValue - GBKEY_VK_A_UPPER;
			//在这里将引擎输出改为大写
		}
		else if ((iLayoutKeyValue >= GBKEY_VK_1_NUMBERKEY)&&(iLayoutKeyValue <= GBKEY_VK_10_NUMBERKEY))//键盘按键
		{
			iEngineKeyValue = '0' + (iLayoutKeyValue - GBKEY_VK_1_NUMBERKEY +1)%10;
		}
		//Note:haj add 貌似有问题，GBKEY_VK_BACKWORD 是一个UI级键值，怎么会转换为GBKEY_RETURN 引擎的键值呢。
		else if (iLayoutKeyValue == GBKEY_VK_BACKWORD)//返回键
		{
			iEngineKeyValue = GBKEY_RETURN;
		}
		else if (iLayoutKeyValue == GBKEY_VK_V9_SYMBOL_ADD)//数字输入模式"+"号
		{
			iEngineKeyValue = '+';
		}
		else if (iLayoutKeyValue == GBKEY_VK_V9_SYMBOL_AT)//数字输入模式"@"号
		{
			iEngineKeyValue = '@';
		}
		else if (iLayoutKeyValue == GBKEY_VK_V9_SYMBOL_DOLL)//数字输入模式"$"号
		{
			iEngineKeyValue = '$';
		}
		else if (iLayoutKeyValue == GBKEY_VK_V9_SYMBOL_PERCENT)//数字输入模式"%"号
		{
			iEngineKeyValue = '%';
		}
		else if (iLayoutKeyValue == GBKEY_VK_V9_SYMBOL_POINT)//数字输入模式"."号
		{
			iEngineKeyValue = '.';
		}
		else if (iLayoutKeyValue == GBKEY_VK_V9_SYMBOL_SUBTRACT)//数字输入模式"-"号
		{
			iEngineKeyValue = '-';
		}
		else if (iLayoutKeyValue == GBKEY_VK_V9_SYMBOL_COMMA)//数字输入模式","号
		{
			iEngineKeyValue = ',';
		}
		else if (iLayoutKeyValue == GBKEY_VK_V9_PINYIN_SEPARATOR)//拼音分隔符号
		{
			iEngineKeyValue = ',';
		}
		else if (iLayoutKeyValue == GBKEY_VK_V9_SYMBOL_FULL_STOP)//句号
		{
			iEngineKeyValue = 0x3002;
		}
		
	}

	return iEngineKeyValue;
}
/************************************************************************
功能：
取得候选框图的高度[候选框也是一个layout]
参数：

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
功能：
取得候选框图的宽度[候选框也是一个layout]
参数：

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
功能：
根据按钮键值,取得按钮在图层中的坐标
参数：

************************************************************************/
GBBOOL CLayoutInfo_GetBottonRectByKey(GBLPCVOID pskinData,GBUINT16 layoutId,GBUINT16 layerId,GBUINT16 iKeyValue,PGBRECT pRect)
{
	PBUTTONCONSTDATA pButtonConstData;
	PLAYOUTCONSTLAYER pLayerConstInfo;
	GBUINT16 iCount;
	//取得层信息
	pLayerConstInfo = CLayoutInfo_GetLayerConstInfo(pskinData,layoutId,layerId);
	//取得层对应的按钮数据
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
 * \brief 根据LayoutID获取国笔切换输入模式切换需要的信息
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
//根据按钮键值判断是否是功能键
GBBOOL CLayoutInfo_ThisKeyValueIsFunctionKey(GBUINT16 iLayoutKeyValue)
{
	//引擎键值时
	if ((iLayoutKeyValue > GBEvent_None)&&(iLayoutKeyValue < GBKEY_VK_A_UPPER))
	{
		if (((iLayoutKeyValue >= GBKEY_0) && (iLayoutKeyValue <= GBKEY_9))//数字键时
			||((iLayoutKeyValue >= GBKEY_A) && (iLayoutKeyValue <= GBKEY_Z)))//字母键时
		{
			return GBFalse;
		}
		return GBTrue;
	}
	else//是虚拟键盘键值时
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
//获取候选框高度
GBINT16 CLayoutInfo_GetCandidateBoxHeight(GBLPCVOID pskinData,GBUINT16 layoutId,GBUINT16 layerId)
{
	GBINT16 iCandidateBoxHeight = 0;
	GBLPCLAYOUTCONSTINFO pLayoutConstInfo;
	PLAYOUTCONSTLAYER pFirstLayer,pOtherLayer;
	//得到指向图层的开始位置
	pLayoutConstInfo = CLayoutInfo_GetLayoutConstInfo(pskinData,layoutId);
	if (pLayoutConstInfo != NULL)
	{
		
		if ((layerId >= pLayoutConstInfo->lyLayerCount)//不存在该层号时
			|| (pLayoutConstInfo->lyLayerCount == 1))//只有一层时，没有候选框
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
		//两层相差即是候选框高度
		iCandidateBoxHeight = pOtherLayer->lyPos.bottom - pFirstLayer->lyPos.bottom;
		return iCandidateBoxHeight;
	}
	return iCandidateBoxHeight;
}

/*!
 * \brief 从Layout信息中获取LayoutID
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
 * \brief 根据滑屏方向获得目标切换LayoutID
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
	case SLIDING_DIRECTION_UP:///<上
		switchLayoutID = pltInfo->pltcData->ltUPSwtID;
		break;
	case SLIDING_DIRECTION_DOWN:///<下
		switchLayoutID = pltInfo->pltcData->ltDownSwtID;
		break;
	case SLIDING_DIRECTION_LEFT:///<左
		switchLayoutID = pltInfo->pltcData->ltLeftSwtID;
		break;
	case SLIDING_DIRECTION_RIGHT:///<右
		switchLayoutID = pltInfo->pltcData->ltRightSwtID;
		break;
	default:
		PT_Assert(0); // 非法方向
		break;
	}

	return switchLayoutID;
}

/*!
 * \brief 获取LayoutID对应的键盘类型
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
	case LayoutIdSP9JPinYin: //竖屏9键拼音
	case LayoutIdSP9JStroke: //竖屏9键笔画
	case LayoutIdSP9JLowerEnglish: //竖屏9键小写英文
	case LayoutIdSP9JUpEnglish: //竖屏9键大写英文
	case LayoutIdSP9Jabc: //竖屏9键小写字母
	case LayoutIdSP9JABC: //竖屏9键大写字母
	case LayoutIdSP9JNumber: //竖屏9键数字
	case LayoutIdSP9JSymbol: //竖屏9键符号
		keyboardType = KEYBOARD_VK_NUMPAD;
		break;	
	case LayoutIdSP26JPinYin: //竖屏26键拼音
	case LayoutIdSP26JStroke: //竖屏26键笔画
	case LayoutIdSP26JLowerEnglish: //竖屏26键小写英文
	case LayoutIdSP26JUpEnglish: //竖屏26键大写英文
	case LayoutIdSP26Jabc: //竖屏26键小写字母
	case LayoutIdSP26JABC: //竖屏26键大写字母
	case LayoutIdSP26JNumber: //竖屏26键数字
	case LayoutIdSP26JSymbol: //竖屏26键符号
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
		//PT_Assert(0); // 非法键盘
		break;
	}
	
	return keyboardType;
}

/*!
 * \brief 获取小写LayoutID对应的大写LayoutID
 * \param lowerLayoutID 
 * \return 
 * \note 寻找不到返回自身
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
 * \brief 获取大写LayoutID对应的小写LayoutID
 * \param upperLayoutID 
 * \return 
 * \note 寻找不到返回自身
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
 * \brief 获取指定键值的按钮名称
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

	// 取得层信息
	pLayerConstInfo = CLayoutInfo_GetLayerConstInfo(pskinData,layoutId,layerId);
	
	// 取得层对应的按钮数据
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
 * \brief 设置指定Layout按钮的状态(应用层可利用此接口禁用指定按钮)
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

	// 遍历所有Layer，找到目标button
	for (iLayer = 0; iLayer < pLayoutInfo->lyLayerCount; iLayer++)
	{
		pltLayerData = &pLayoutInfo->pltLayerData[iLayer];		
		if (pltLayerData == NULL)
		{
			continue; // 该层无数据
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