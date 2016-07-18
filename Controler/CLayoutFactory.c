/***************************************************************************
* File Name    :  CFactory.c
* Corporation  :  Guobi Technology Ltd. GuangZhou China
* Copyright    :  Copyright Guobi Technology Ltd. And Subject to Confidentiality 
*                 Restrictions. This document is the confidential and proprietary 
*                 property of Guobi Technology Ltd.It is neither the document                  
*                 itself nor any part of it can be shared with any party  without 
*                 the written  permission. 
* Author       :  fanbo
* Date         :  2010-06-08
* Purpose      :  工厂实现文件
* Version      :  V1.0
* Last Changed List
* Name         |   date   |  Discription
****************************************************************************/
#include "PlatformTools.h"
#include "GB_Global.h"
#include "IMECommonData.h"
#include "CLayoutControl.h"
#include "CLayoutInfo.h"
#include "CLayoutAbstract.h"
// 包含所有Layout类型

#if defined(__GBIME_VK__) // 国笔虚拟键盘
#include "CLayoutVK.h"
#include "CLayoutVKSymbol.h"
#include "CLayoutVK.h"
#include "CLayoutVK9MultiLang.h"
#include "CLayoutVK9Pinyin.h"
#include "CLayoutVK9Stroke.h"
#include "CLayoutVK26MultiLang.h"
#include "CLayoutVK26Pinyin.h"
#include "CLayoutVK26Stroke.h"
#include "CLayoutVKNumber.h"
#include "CLayoutVKSymbol.h"
#endif // 国笔虚拟键盘

#if defined(__GBIME_HANDWRITE__)
#include "CLayoutHandwrite.h"
#include "CLayoutHandwriteFullScreen.h"
#include "CLayoutHandwriteMultiBlock.h"
#endif

#if defined(__GBIME_KB__) // 国笔物理键盘
#include "CLayoutKB.h"
#include "CLayoutKB9Pinyin.h"
#include "CLayoutKB9Stroke.h"
#include "CLayoutKB9MultiLang.h"
#include "CLayoutKB9Number.h"
#endif // 国笔物理键盘

// 包含所有Layout类型

#ifdef Layout_Test //Layout对象需要动态创建
typedef struct _tag_LayoutObject{
	GBUINT16 ltID;
	PCLayoutAbstract pltObject;
}LayoutObjectArray, * PLayoutObjectArray;

#define MAX_LAYOUT_OBJECT_NUM   20
LayoutObjectArray g_ltObjectArray[MAX_LAYOUT_OBJECT_NUM];
#endif

/*!
* \brief Layout 是否已经实例化了
* \param id 
* \return 
* \note 
* \example
* \author fanbo
* \date 2010-06-08
*/
GBINT32 CFactory_HasExist(PSkinData pskData, GBUINT16 ltID)
{
#ifdef Layout_Test //Layout对象需要动态创建
	int i=0;
	for (i=0; i < pskData->ltCount; i++)
	{
		if (pskData->pskLtData[i].pltcData->ltID == ltID)
		{
			return GBOK;
		}
	}
#endif
	return GB_Failed;
}

/*!
* \brief 增加一个Layout实例
* \param id 
* \return 
* \note 
* \example
* \author fanbo
* \date 2010-06-08
*/
PCLayoutAbstract CFactory_AddLayoutObject(PSkinData pskData, GBUINT16 ltID)
{
	PCLayoutAbstract pltTempObj = NULL;
	GBUINT16 ltType = 0;
	GBUINT16 ltSize = 0;
#ifdef Layout_Test //Layout对象需要动态创建
	int i=0;

	for (i=0; i < pskData->ltCount; i++)
	{
		if (pskData->pskLtData[i].pltcData->ltID == 0)
		{
			break;
		}
	}
	if (i >= pskData->ltCount)
	{
		//超过最大个数
		PT_Assert(0);
		return NULL;
	}
#endif
	ltType = CLayoutInfo_GetLayoutTypeByID(pskData->pskcData, ltID);
	switch (ltType) // 根据Layout类型调用对应的Layout对象实例所占用的字节大小
	{
#if defined(__GBIME_HANDWRITE__)
	case LayoutHandwriteFullScreen:	///<全拼手写
		ltSize = sizeof(CLayoutHandwriteFullScreen);
		break;
	case LayoutHandwriteMultiBlock:  ///<格子手写（暂时不做：市场需求多）
		ltSize = sizeof(CLayoutHandwriteMultiBlock);
		break;
#endif
#if defined(__GBIME_VK__) // 国笔虚拟键盘
	case LayoutVK9Pinyin: ///<虚拟9键--拼音类
		ltSize = sizeof(CLayoutVK9Pinyin);
		break;
	case LayoutVK9Stroke: ///<虚拟9键--笔画类
		ltSize = sizeof(CLayoutVK9Stroke);
		break;
	case LayoutVK9MultiLang: ///<虚拟9键--多语言类
		ltSize = sizeof(CLayoutVK9MultiLang);
		break;
	case LayoutVK26Pinyin: ///<虚拟全键盘拼音类
		ltSize = sizeof(CLayoutVK26Pinyin);
		break;
	case LayoutVK26Stroke: ///<虚拟全键盘笔画类
		ltSize = sizeof(CLayoutVK26Stroke);
		break;
	case LayoutVK26MultiLang: ///<虚拟全键盘多语言类
		ltSize = sizeof(CLayoutVK26MultiLang);
		break;
	case LayoutVKNumber: ///<键值直接上屏类--数字
		ltSize = sizeof(CLayoutVKNumber);
		break;
	case LayoutVKSymbol: ///<键值直接上屏类--符号
		ltSize = sizeof(CLayoutVKSymbol);
		break;
	case LayoutVKCopyPaste: ///<复制粘贴（暂时不做：非输入法产品必须功能，以后再做计划。）
		//break; // not support yet!!!
	case LayoutVKProductConfig: ///<产品配置（暂时不做：目前用平台菜单方式配置产品。）
		//break; // not support yet!!!
#endif // 国笔虚拟键盘

#if defined(__GBIME_KB__) // 国笔物理键盘
	case LayoutKB9SingleRow: ///<物理9键单行
		ltSize = sizeof(CLayoutKB);
		break;
	case LayoutKB26SingleRow: ///<物理Qwerty单行
		//break; // not support yet!!!
	case LayoutKB9TwinRow: ///<物理9键双行
		//break; // not support yet!!!
	case LayoutKB26TwinRow: ///<物理Qwerty双行
		//break; // not support yet!!!
	case LayoutFullScreenSymbol: ///<全拼标点符号
		//break; // not support yet!!!
#endif // 国笔物理键盘

	default:
		//PT_Assert(0);
		break;
	}

	pltTempObj = (PCLayoutAbstract)PT_Malloc(ltSize);
	
	return pltTempObj;
}

/*!
* \brief 增加一个Layout实例
* \param id 
* \return 
* \note 
* \example
* \author fanbo
* \date 2010-06-08
*/
GBINT32 CFactory_ReleaseLayoutObject(GBINT32 id)
{
#ifdef Layout_Test //Layout对象需要动态创建
	int i=0;

	for (i=0; i < MAX_LAYOUT_OBJECT_NUM; i++)
	{
		if (g_ltObjectArray[i].ltID == id)
		{
			break;
		}
	}
	if (i >= MAX_LAYOUT_OBJECT_NUM)
	{
		return GB_Failed;
	}
	g_ltObjectArray[i].pltObject->m_pfcRelease(&g_ltObjectArray[i].pltObject->m_ltData);

	g_ltObjectArray[i].ltID = 0;
	PT_Free(g_ltObjectArray[i].pltObject);
	g_ltObjectArray[i].pltObject = NULL;
#endif
	return GBOK;
}

/*!
* \brief 创建Layout实例
* \param id 
* \return 
* \note 
* \example
* \author fanbo
* \date 2010-06-08
*/
PCLayoutAbstract CFactory_CreateInstance(PIMECommonData pcomIMEData, GBUINT16 ltID)
{
	PCLayoutAbstract pltObject = NULL;
	PCLayoutAbstract pltCurObject = NULL;
	GBUINT16 ltType = 0;

	if (CFactory_HasExist(Global_GetSkinInfoData(), ltID) == GBOK)
	{
		return NULL;
	}
	pltObject = CFactory_AddLayoutObject(Global_GetSkinInfoData(), ltID);
	if (NULL != pltObject)
	{
		ltType = CLayoutInfo_GetLayoutTypeByID(pcomIMEData->pskinData, ltID);
		switch (ltType) // 根据Layout类型调用对应的构造函数创建Layout对象实例
		{
	#if defined(__GBIME_HANDWRITE__)
		case LayoutHandwriteFullScreen:	///<全拼手写
			pltCurObject = CLayoutHandwriteFullScreen__Construct((PCLayoutHandwriteFullScreen)pltObject,
			 	CLayoutInfo_GetLayoutDataByID(pcomIMEData->pskinData, ltID),
			 	pcomIMEData->pimEngineData, pcomIMEData->pimEngineData);
 			break;
		case LayoutHandwriteMultiBlock:  ///<格子手写（暂时不做：市场需求多）
			pltCurObject = CLayoutHandwriteMultiBlock__Construct((PCLayoutHandwriteMultiBlock)pltObject,
				CLayoutInfo_GetLayoutDataByID(pcomIMEData->pskinData, ltID),
				pcomIMEData->pimEngineData, pcomIMEData->pimEngineData);
 			break;
	#endif
	#if defined(__GBIME_VK__) // 国笔虚拟键盘
		case LayoutVK9Pinyin: ///<虚拟9键--拼音类
			pltCurObject = CLayoutVK9Pinyin__Construct((PCLayoutVK9Pinyin)pltObject,
				CLayoutInfo_GetLayoutDataByID(pcomIMEData->pskinData, ltID),
				pcomIMEData->pimEngineData, pcomIMEData->pimEngineData);
			break;
		case LayoutVK9Stroke: ///<虚拟9键--笔画类
			pltCurObject = CLayoutVK9Stroke__Construct((PCLayoutVK9Stroke)pltObject,
				CLayoutInfo_GetLayoutDataByID(pcomIMEData->pskinData, ltID),
				pcomIMEData->pimEngineData, pcomIMEData->pimEngineData);
			break;
		case LayoutVK9MultiLang: ///<虚拟9键--多语言类
			pltCurObject = CLayoutVK9MultiLang__Construct((PCLayoutVK9MultiLang)pltObject,
				CLayoutInfo_GetLayoutDataByID(pcomIMEData->pskinData, ltID),
				pcomIMEData->pimEngineData, pcomIMEData->pimEngineData);
			break;
		case LayoutVK26Pinyin: ///<虚拟全键盘拼音类
			pltCurObject = CLayoutVK26Pinyin__Construct((PCLayoutVK26Pinyin)pltObject,
				CLayoutInfo_GetLayoutDataByID(pcomIMEData->pskinData, ltID),
				pcomIMEData->pimEngineData, pcomIMEData->pimEngineData);
			break;
		case LayoutVK26Stroke: ///<虚拟全键盘笔画类
			pltCurObject = CLayoutVK26Stroke__Construct((PCLayoutVK26Stroke)pltObject,
				CLayoutInfo_GetLayoutDataByID(pcomIMEData->pskinData, ltID),
				pcomIMEData->pimEngineData, pcomIMEData->pimEngineData);
			break;
		case LayoutVK26MultiLang: ///<虚拟全键盘多语言类
			pltCurObject = CLayoutVK26MultiLang__Construct((PCLayoutVK26MultiLang)pltObject,
				CLayoutInfo_GetLayoutDataByID(pcomIMEData->pskinData, ltID),
				pcomIMEData->pimEngineData, pcomIMEData->pimEngineData);
			break;
		case LayoutVKNumber: ///<键值直接上屏类--数字
			pltCurObject = CLayoutVKNumber__Construct((PCLayoutVKNumber)pltObject,
				CLayoutInfo_GetLayoutDataByID(pcomIMEData->pskinData, ltID),
				pcomIMEData->pimEngineData, pcomIMEData->pimEngineData);
			break;
		case LayoutVKSymbol: ///<键值直接上屏类--符号
			pltCurObject = CLayoutVKSymbol__Construct((PCLayoutVKSymbol)pltObject,
				CLayoutInfo_GetLayoutDataByID(pcomIMEData->pskinData, ltID),
				pcomIMEData->pimEngineData, pcomIMEData->pimEngineData);
			break;
		case LayoutVKCopyPaste: ///<复制粘贴（暂时不做：非输入法产品必须功能，以后再做计划。）
			break;
		case LayoutVKProductConfig: ///<产品配置（暂时不做：目前用平台菜单方式配置产品。）
			break;
	#endif // 国笔虚拟键盘

	#if defined(__GBIME_KB__) // 国笔物理键盘
		case LayoutKB9SingleRow: ///<物理9键单行
			switch(ltID)
			{
			case LayoutIdWL9JPinYin:
				pltCurObject = CLayoutKB9Pinyin__Construct((PCLayoutKB9Pinyin)pltObject,
					CLayoutInfo_GetLayoutDataByID(pcomIMEData->pskinData, ltID),
					pcomIMEData->pimEngineData, pcomIMEData->pimEngineData);
				break;
			case LayoutIdWL9JStroke:
				pltCurObject = CLayoutKB9Stroke__Construct((PCLayoutKB9Stroke)pltObject,
					CLayoutInfo_GetLayoutDataByID(pcomIMEData->pskinData, ltID),
					pcomIMEData->pimEngineData, pcomIMEData->pimEngineData);
				break;
			case LayoutIdWL9JEnglish:
			case LayoutIdWL9Jabc:
			case LayoutIdWL9JABC:
				pltCurObject = CLayoutKB9MultiLang__Construct((PCLayoutKB9MultiLang)pltObject,
					CLayoutInfo_GetLayoutDataByID(pcomIMEData->pskinData, ltID),
					pcomIMEData->pimEngineData, pcomIMEData->pimEngineData);
				break;
			case LayoutIdWL9JNumber:
				pltCurObject = CLayoutKB9Number__Construct((PCLayoutKB9Number)pltObject,
					CLayoutInfo_GetLayoutDataByID(pcomIMEData->pskinData, ltID),
					pcomIMEData->pimEngineData, pcomIMEData->pimEngineData);
				break;
			default:
				pltCurObject = CLayoutKB9Pinyin__Construct((PCLayoutKB9Pinyin)pltObject,
					CLayoutInfo_GetLayoutDataByID(pcomIMEData->pskinData, ltID),
					pcomIMEData->pimEngineData, pcomIMEData->pimEngineData);
			}			
			break;
		case LayoutKB26SingleRow: ///<物理Qwerty单行
				switch(ltID)
				{
					//26键模块暂缺
				case LayoutIdWL26JPinYin:
					pltCurObject = CLayoutKB9Pinyin__Construct((PCLayoutKB9Pinyin)pltObject,
						CLayoutInfo_GetLayoutDataByID(pcomIMEData->pskinData, ltID),
						pcomIMEData->pimEngineData, pcomIMEData->pimEngineData);
					break;
				case LayoutIdWL26JStroke:
					pltCurObject = CLayoutKB9Stroke__Construct((PCLayoutKB9Stroke)pltObject,
						CLayoutInfo_GetLayoutDataByID(pcomIMEData->pskinData, ltID),
						pcomIMEData->pimEngineData, pcomIMEData->pimEngineData);
					break;
				case LayoutIdWL26JEnglish:
				case LayoutIdWL26Jabc:
				case LayoutIdWL26JABC:
					pltCurObject = CLayoutKB9MultiLang__Construct((PCLayoutKB9MultiLang)pltObject,
						CLayoutInfo_GetLayoutDataByID(pcomIMEData->pskinData, ltID),
						pcomIMEData->pimEngineData, pcomIMEData->pimEngineData);
					break;
				case LayoutIdWL26JNumber:
					pltCurObject = CLayoutKB9Number__Construct((PCLayoutKB9Number)pltObject,
						CLayoutInfo_GetLayoutDataByID(pcomIMEData->pskinData, ltID),
						pcomIMEData->pimEngineData, pcomIMEData->pimEngineData);
					break;
				default:
					pltCurObject = CLayoutKB9Pinyin__Construct((PCLayoutKB9Pinyin)pltObject,
						CLayoutInfo_GetLayoutDataByID(pcomIMEData->pskinData, ltID),
						pcomIMEData->pimEngineData, pcomIMEData->pimEngineData);
				}
				
				break;
		case LayoutKB9TwinRow: ///<物理9键双行
			break;
		case LayoutKB26TwinRow: ///<物理Qwerty双行
			break;
		case LayoutFullScreenSymbol: ///<全拼标点符号
			break;
	#endif // 国笔物理键盘

		default:
			//PT_Assert(0);
			break;
		}
	}

	return pltCurObject;
}
