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
* Purpose      :  ����ʵ���ļ�
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
// ��������Layout����

#if defined(__GBIME_VK__) // �����������
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
#endif // �����������

#if defined(__GBIME_HANDWRITE__)
#include "CLayoutHandwrite.h"
#include "CLayoutHandwriteFullScreen.h"
#include "CLayoutHandwriteMultiBlock.h"
#endif

#if defined(__GBIME_KB__) // �����������
#include "CLayoutKB.h"
#include "CLayoutKB9Pinyin.h"
#include "CLayoutKB9Stroke.h"
#include "CLayoutKB9MultiLang.h"
#include "CLayoutKB9Number.h"
#endif // �����������

// ��������Layout����

#ifdef Layout_Test //Layout������Ҫ��̬����
typedef struct _tag_LayoutObject{
	GBUINT16 ltID;
	PCLayoutAbstract pltObject;
}LayoutObjectArray, * PLayoutObjectArray;

#define MAX_LAYOUT_OBJECT_NUM   20
LayoutObjectArray g_ltObjectArray[MAX_LAYOUT_OBJECT_NUM];
#endif

/*!
* \brief Layout �Ƿ��Ѿ�ʵ������
* \param id 
* \return 
* \note 
* \example
* \author fanbo
* \date 2010-06-08
*/
GBINT32 CFactory_HasExist(PSkinData pskData, GBUINT16 ltID)
{
#ifdef Layout_Test //Layout������Ҫ��̬����
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
* \brief ����һ��Layoutʵ��
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
#ifdef Layout_Test //Layout������Ҫ��̬����
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
		//����������
		PT_Assert(0);
		return NULL;
	}
#endif
	ltType = CLayoutInfo_GetLayoutTypeByID(pskData->pskcData, ltID);
	switch (ltType) // ����Layout���͵��ö�Ӧ��Layout����ʵ����ռ�õ��ֽڴ�С
	{
#if defined(__GBIME_HANDWRITE__)
	case LayoutHandwriteFullScreen:	///<ȫƴ��д
		ltSize = sizeof(CLayoutHandwriteFullScreen);
		break;
	case LayoutHandwriteMultiBlock:  ///<������д����ʱ�������г�����ࣩ
		ltSize = sizeof(CLayoutHandwriteMultiBlock);
		break;
#endif
#if defined(__GBIME_VK__) // �����������
	case LayoutVK9Pinyin: ///<����9��--ƴ����
		ltSize = sizeof(CLayoutVK9Pinyin);
		break;
	case LayoutVK9Stroke: ///<����9��--�ʻ���
		ltSize = sizeof(CLayoutVK9Stroke);
		break;
	case LayoutVK9MultiLang: ///<����9��--��������
		ltSize = sizeof(CLayoutVK9MultiLang);
		break;
	case LayoutVK26Pinyin: ///<����ȫ����ƴ����
		ltSize = sizeof(CLayoutVK26Pinyin);
		break;
	case LayoutVK26Stroke: ///<����ȫ���̱ʻ���
		ltSize = sizeof(CLayoutVK26Stroke);
		break;
	case LayoutVK26MultiLang: ///<����ȫ���̶�������
		ltSize = sizeof(CLayoutVK26MultiLang);
		break;
	case LayoutVKNumber: ///<��ֱֵ��������--����
		ltSize = sizeof(CLayoutVKNumber);
		break;
	case LayoutVKSymbol: ///<��ֱֵ��������--����
		ltSize = sizeof(CLayoutVKSymbol);
		break;
	case LayoutVKCopyPaste: ///<����ճ������ʱ�����������뷨��Ʒ���빦�ܣ��Ժ������ƻ�����
		//break; // not support yet!!!
	case LayoutVKProductConfig: ///<��Ʒ���ã���ʱ������Ŀǰ��ƽ̨�˵���ʽ���ò�Ʒ����
		//break; // not support yet!!!
#endif // �����������

#if defined(__GBIME_KB__) // �����������
	case LayoutKB9SingleRow: ///<����9������
		ltSize = sizeof(CLayoutKB);
		break;
	case LayoutKB26SingleRow: ///<����Qwerty����
		//break; // not support yet!!!
	case LayoutKB9TwinRow: ///<����9��˫��
		//break; // not support yet!!!
	case LayoutKB26TwinRow: ///<����Qwerty˫��
		//break; // not support yet!!!
	case LayoutFullScreenSymbol: ///<ȫƴ������
		//break; // not support yet!!!
#endif // �����������

	default:
		//PT_Assert(0);
		break;
	}

	pltTempObj = (PCLayoutAbstract)PT_Malloc(ltSize);
	
	return pltTempObj;
}

/*!
* \brief ����һ��Layoutʵ��
* \param id 
* \return 
* \note 
* \example
* \author fanbo
* \date 2010-06-08
*/
GBINT32 CFactory_ReleaseLayoutObject(GBINT32 id)
{
#ifdef Layout_Test //Layout������Ҫ��̬����
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
* \brief ����Layoutʵ��
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
		switch (ltType) // ����Layout���͵��ö�Ӧ�Ĺ��캯������Layout����ʵ��
		{
	#if defined(__GBIME_HANDWRITE__)
		case LayoutHandwriteFullScreen:	///<ȫƴ��д
			pltCurObject = CLayoutHandwriteFullScreen__Construct((PCLayoutHandwriteFullScreen)pltObject,
			 	CLayoutInfo_GetLayoutDataByID(pcomIMEData->pskinData, ltID),
			 	pcomIMEData->pimEngineData, pcomIMEData->pimEngineData);
 			break;
		case LayoutHandwriteMultiBlock:  ///<������д����ʱ�������г�����ࣩ
			pltCurObject = CLayoutHandwriteMultiBlock__Construct((PCLayoutHandwriteMultiBlock)pltObject,
				CLayoutInfo_GetLayoutDataByID(pcomIMEData->pskinData, ltID),
				pcomIMEData->pimEngineData, pcomIMEData->pimEngineData);
 			break;
	#endif
	#if defined(__GBIME_VK__) // �����������
		case LayoutVK9Pinyin: ///<����9��--ƴ����
			pltCurObject = CLayoutVK9Pinyin__Construct((PCLayoutVK9Pinyin)pltObject,
				CLayoutInfo_GetLayoutDataByID(pcomIMEData->pskinData, ltID),
				pcomIMEData->pimEngineData, pcomIMEData->pimEngineData);
			break;
		case LayoutVK9Stroke: ///<����9��--�ʻ���
			pltCurObject = CLayoutVK9Stroke__Construct((PCLayoutVK9Stroke)pltObject,
				CLayoutInfo_GetLayoutDataByID(pcomIMEData->pskinData, ltID),
				pcomIMEData->pimEngineData, pcomIMEData->pimEngineData);
			break;
		case LayoutVK9MultiLang: ///<����9��--��������
			pltCurObject = CLayoutVK9MultiLang__Construct((PCLayoutVK9MultiLang)pltObject,
				CLayoutInfo_GetLayoutDataByID(pcomIMEData->pskinData, ltID),
				pcomIMEData->pimEngineData, pcomIMEData->pimEngineData);
			break;
		case LayoutVK26Pinyin: ///<����ȫ����ƴ����
			pltCurObject = CLayoutVK26Pinyin__Construct((PCLayoutVK26Pinyin)pltObject,
				CLayoutInfo_GetLayoutDataByID(pcomIMEData->pskinData, ltID),
				pcomIMEData->pimEngineData, pcomIMEData->pimEngineData);
			break;
		case LayoutVK26Stroke: ///<����ȫ���̱ʻ���
			pltCurObject = CLayoutVK26Stroke__Construct((PCLayoutVK26Stroke)pltObject,
				CLayoutInfo_GetLayoutDataByID(pcomIMEData->pskinData, ltID),
				pcomIMEData->pimEngineData, pcomIMEData->pimEngineData);
			break;
		case LayoutVK26MultiLang: ///<����ȫ���̶�������
			pltCurObject = CLayoutVK26MultiLang__Construct((PCLayoutVK26MultiLang)pltObject,
				CLayoutInfo_GetLayoutDataByID(pcomIMEData->pskinData, ltID),
				pcomIMEData->pimEngineData, pcomIMEData->pimEngineData);
			break;
		case LayoutVKNumber: ///<��ֱֵ��������--����
			pltCurObject = CLayoutVKNumber__Construct((PCLayoutVKNumber)pltObject,
				CLayoutInfo_GetLayoutDataByID(pcomIMEData->pskinData, ltID),
				pcomIMEData->pimEngineData, pcomIMEData->pimEngineData);
			break;
		case LayoutVKSymbol: ///<��ֱֵ��������--����
			pltCurObject = CLayoutVKSymbol__Construct((PCLayoutVKSymbol)pltObject,
				CLayoutInfo_GetLayoutDataByID(pcomIMEData->pskinData, ltID),
				pcomIMEData->pimEngineData, pcomIMEData->pimEngineData);
			break;
		case LayoutVKCopyPaste: ///<����ճ������ʱ�����������뷨��Ʒ���빦�ܣ��Ժ������ƻ�����
			break;
		case LayoutVKProductConfig: ///<��Ʒ���ã���ʱ������Ŀǰ��ƽ̨�˵���ʽ���ò�Ʒ����
			break;
	#endif // �����������

	#if defined(__GBIME_KB__) // �����������
		case LayoutKB9SingleRow: ///<����9������
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
		case LayoutKB26SingleRow: ///<����Qwerty����
				switch(ltID)
				{
					//26��ģ����ȱ
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
		case LayoutKB9TwinRow: ///<����9��˫��
			break;
		case LayoutKB26TwinRow: ///<����Qwerty˫��
			break;
		case LayoutFullScreenSymbol: ///<ȫƴ������
			break;
	#endif // �����������

		default:
			//PT_Assert(0);
			break;
		}
	}

	return pltCurObject;
}
