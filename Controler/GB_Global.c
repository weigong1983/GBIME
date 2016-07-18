/***************************************************************************
* File Name    :  CConfigure.c
* Corporation  :  Guobi Technology Ltd. GuangZhou China
* Copyright    :  Copyright Guobi Technology Ltd. And Subject to Confidentiality 
*                 Restrictions. This document is the confidential and proprietary 
*                 property of Guobi Technology Ltd.It is neither the document                  
*                 itself nor any part of it can be shared with any party  without 
*                 the written  permission. 
* Author       :  haj
* Date         :  2010-06-08
* Purpose      :  Global data and its operator
* Version      :  V1.0
* Last Changed List
* Name         |   date   |  Discription
****************************************************************************/
#include "GB_Global.h"
#include "CIMEInterface.h"
#include "PlatformTools.h"
#include "PlatformAdapter.h"


extern const unsigned long V5LayoutDataArray[];
CIMEInterface g_imeInterface;

// ��������
#ifdef GBIME_CFG_STATIC_DB // ʹ�þ�̬����
	#ifndef _C166
	#define _huge
	#endif //_C166

	#ifdef GBIME_CFG_CHS_SUPPORT // ����֧��
	//#include "gb_sc.c"
	extern const unsigned long _huge prv_scDataArray[];
	#endif/*GBIME_CFG_CHS_SUPPORT*/

	#ifdef GBIME_CFG_EN_SUPPORT // Ӣ��֧��
	extern const unsigned long _huge prv_enDataArray[];
	#endif/*GBIME_CFG_EN_SUPPORT*/

#endif // GBIME_CFG_STATIC_DB

#ifdef GBIME_CFG_UDB_SUPPORT // ���������
	#define MASTER_UDB_SIZE (8 * 1024)
	static unsigned long _master_udb_buffer[MASTER_UDB_SIZE]; // ��UDB buffer�ռ�
	#define SLAVE_UDB_SIZE  (2 * 1024)
	static unsigned long _slave_udb_buffer[SLAVE_UDB_SIZE]; // ��UDB buffer�ռ�
#endif/*GBIME_CFG_UDB_SUPPORT*/

#if defined(__GBIME_HANDWRITE__) // ��д����
	extern const unsigned long _huge prv_HWDataArray[];
	//extern const unsigned long _huge prv_BigHWDataArray[]; // ��ʱ�����ô�˵�����
#endif/*defined(__GBIME_HANDWRITE__)*/

/*!
 * \ �ֲ����ܺ�������
 */
GBIMELOCAL GBINT Global_GetLtObjectArrayFreeIndex(void);
GBIMELOCAL GBINT Global_GetLtObjectArrayRemoveIndex(void);

/*!
 * \brief ��ʼ�����뷨���ȫ������bufferָ��
 * \param void 
 * \return 
 */
static void Global_InitComIMEData(void)
{
	PCIMEInterface pIMEInterface = Global_GetIMEInterface();

	pIMEInterface->comIMEData.pskinData = V5LayoutDataArray;
	pIMEInterface->comIMEData.pconfData = NULL;
#ifdef GBIME_CFG_CHS_SUPPORT // ����֧��
	pIMEInterface->comIMEData.pimChnData = prv_scDataArray;
#endif
#ifdef GBIME_CFG_EN_SUPPORT // Ӣ��֧��
	pIMEInterface->comIMEData.pimAlpData = prv_enDataArray;
#endif
	pIMEInterface->comIMEData.pimEngineData = NULL;
#if defined(__GBIME_HANDWRITE__) // ��д����֧��
	pIMEInterface->comIMEData.phwEngineData = prv_HWDataArray;
#endif
#ifdef GBIME_CFG_UDB_SUPPORT // �����֧��
	pIMEInterface->comIMEData.masterUDBData = _master_udb_buffer;
	pIMEInterface->comIMEData.slaveUDBData = _slave_udb_buffer;
#endif
}

/*!
 * \brief ��ʼ�����뷨���ȫ�ֱ����ӿڶ���
 * \param void 
 * \return 
 */
void Global_InitIMEInterface(void)
{
	PCIMEInterface pIMEInterface = Global_GetIMEInterface();

	// ����ṹ��
	memset(pIMEInterface, 0, sizeof(CIMEInterface));

	// ��ʼ�����ͨ�õ�ȫ������buffer
	Global_InitComIMEData();

	pIMEInterface->pltCtl = (CLayoutControl *)PT_Malloc(sizeof(CLayoutControl));
	PT_Assert(pIMEInterface->pltCtl != NULL);
	memset(pIMEInterface->pltCtl, 0, sizeof(CLayoutControl));

	pIMEInterface->pConfig = (CConfig *)PT_Malloc(sizeof(CConfig));
	PT_Assert(pIMEInterface->pConfig != NULL);
	memset(pIMEInterface->pConfig, 0, sizeof(CConfig));

#if defined(__GBIME_HANDWRITE__)
	pIMEInterface->pGBHWEngine = (GBHWEngine *)PT_Malloc(sizeof(GBHWEngine));
	PT_Assert(pIMEInterface->pGBHWEngine != NULL);
	memset(pIMEInterface->pGBHWEngine, 0, sizeof(GBHWEngine));
#endif

	// ��ʼ��Ƥ������
	CLayoutInfo_SkinInitData(&pIMEInterface->comIMEData.skinInfo, pIMEInterface->comIMEData.pskinData);

	pIMEInterface->bFrameworkInited = GBTrue;
	pIMEInterface->latestChnNumLayoutID = LayoutIdEnd;
	pIMEInterface->latestChnQwertyLayoutID = LayoutIdEnd;
	pIMEInterface->latestAlpNumLayoutID = LayoutIdEnd;
	pIMEInterface->latestAlpQwertyLayoutID = LayoutIdEnd;
}

/*!
 * \brief �������뷨���ȫ�ֱ����ӿڶ���
 * \param void 
 * \return 
 */
void Global_ReleaseIMEInterface(void)
{
	PCIMEInterface pIMEInterface = Global_GetIMEInterface();
	GBINT32 idx = 0;
	PCLayoutAbstract pltDest = NULL;

	// �ͷ�Layout��������Ķ�̬�ڴ�
	for (idx=0; idx < MAX_LEAVE_LAYOUT_OBJECT_NUM; idx++)
	{
		pltDest = (PCLayoutAbstract)pIMEInterface->ltObjectArray[idx];
		if (pltDest != NULL)
		{
			// �������������ͷ�Ƥ����������Ķ�̬�ڴ�
			if (pltDest->m_pfcRelease != NULL)
			{
				pltDest->m_pfcRelease(pltDest, &pltDest->m_ltData);
			}
			// �ͷ���ͼ��
			//�ô������ͷ�pltDest->m_hltView��������õ�
			//if (pltDest->m_hltView != NULL)
			//{
			//	PT_Free(pltDest->m_hltView);
			//	pltDest->m_hltView = NULL;
			//}
			// �ͷ���������
			PT_Free(pltDest);
			pltDest = NULL;
		}
	}

	// �ͷ�������������Ķ�̬�ڴ�
	if (pIMEInterface->pltCtl != NULL)
	{
		PT_Free(pIMEInterface->pltCtl);
		pIMEInterface->pltCtl = NULL;
	}
	if (pIMEInterface->pConfig!= NULL)
	{
		PT_Free(pIMEInterface->pConfig);
		pIMEInterface->pConfig = NULL;
	}
#if defined(__GBIME_HANDWRITE__)
	if (pIMEInterface->pGBHWEngine!= NULL)
	{
		PT_Free(pIMEInterface->pGBHWEngine);
		pIMEInterface->pGBHWEngine = NULL;
	}
#endif

	// ����ṹ��
	memset(pIMEInterface, 0, sizeof(CIMEInterface));
}

/*!
 * \brief ��ܽӿڶ����Ƿ��ʼ��
 * \param void 
 * \return 
 */
GBBOOL Global_IMEInterfaceIsInited(void)
{
	return Global_GetIMEInterface()->bFrameworkInited;
}

/*!
 * \brief ��ȡIMEģ������״̬
 * \param void 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-8-11 9:33:50
*/
GBAPI GBBOOL Global_IMEIsConnected(void)
{
	return Global_GetIMEInterface()->bIMEConnected;
}

/*!
 * \brief ����IMEģ������״̬
 * \param GBBOOL 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-8-11 9:33:50
*/
GBAPI void Global_SetIMEConnectedStatus(GBBOOL status)
{
	Global_GetIMEInterface()->bIMEConnected = status;
}

/*!
 * \brief Layout �Ķ����Ƿ��Ѿ�������
 * \param pltCurObject 
 * \return 
 */
GBINT32 Global_LyoutObjectIsLeaveByObject(PCLayoutAbstract pltCurObject)
{
	PCIMEInterface pIMEInterface = Global_GetIMEInterface();
	GBINT32 ltObjIndex = 0;

	PT_Assert(Global_GetIMEInterface()->bFrameworkInited);
	PT_Assert(pltCurObject);

	for (ltObjIndex=0; ltObjIndex < MAX_LEAVE_LAYOUT_OBJECT_NUM; ltObjIndex++)
	{
		if (pIMEInterface->ltObjectArray[ltObjIndex] == pltCurObject)
		{
			return 1;
		}
	}

	return -1;
}

/*!
 * \brief ���浱ǰLayout����
 * \param pltCurObject 
 * \return 
 */
PCLayoutAbstract Global_SaveLayoutObject(PCLayoutAbstract pltCurObject)
{
	PCIMEInterface pIMEInterface = Global_GetIMEInterface();
	GBINT ltObjIndex = 0;

	PT_Assert(Global_GetIMEInterface()->bFrameworkInited);
	PT_Assert(pltCurObject);

	if (pltCurObject->m_pfcIsNeedLeave(pltCurObject)
		&& Global_LyoutObjectIsLeaveByObject(pltCurObject) !=  1)
	{
		GBINT saveIndex = Global_GetLtObjectArrayFreeIndex();
		
		// �洢�ռ�δ��
		if (saveIndex != MAX_LEAVE_LAYOUT_OBJECT_NUM)
		{
			pIMEInterface->ltObjectArray[saveIndex] = pltCurObject;
			// ���µ�ǰ�������һ�������ָ�����ü�ʹ��ʱ��
			Global_SetCurrentLayoutObject(pltCurObject);
			return pltCurObject;
		}
		else // ���������ʷLayout�����������滻���ϵ�(�ϴ�ʹ��ʱ������)
		{
			GBINT removeIndex = Global_GetLtObjectArrayRemoveIndex();
			PCLayoutAbstract pRemoveLtObject = pIMEInterface->ltObjectArray[removeIndex];

			// �ͷ�removeIndex��Ӧ��Layout����
			if (pRemoveLtObject != NULL 
				&& pRemoveLtObject->m_pfcRelease != NULL)
			{
				pRemoveLtObject->m_pfcRelease(pRemoveLtObject, &pRemoveLtObject->m_ltData);
				PT_Free(pRemoveLtObject);
				pRemoveLtObject = NULL;			
			}

			// �Ƴ���ĸÿ��ж���ָ������ָ��ǰ����
			pIMEInterface->ltObjectArray[removeIndex] = pltCurObject;
			// ���µ�ǰ�������һ�������ָ�����ü�ʹ��ʱ��
			Global_SetCurrentLayoutObject(pltCurObject);
			return pltCurObject;
		}
	}

	return NULL;
}

/*!
 * \brief ���õ�ǰ�����Layout����
 * \param pltCurObject 
 * \return ���ص�ǰLayout ����
 */
void Global_SetCurrentLayoutObject(PCLayoutAbstract pltCurObject)
{
	PCIMEInterface pIMEInterface = Global_GetIMEInterface();
	GBINT ltObjIndex = 0;
	PCLayoutAbstract pTempLtObject = NULL;
	
	PT_Assert(Global_GetIMEInterface()->bFrameworkInited);
	PT_Assert(pltCurObject);

	// ��ǰ�������Ŀ�����ֱ�ӷ���
	if (pltCurObject == pIMEInterface->pltCurObject)
	{
		return ;
	}

	pIMEInterface->pltPreObject = pIMEInterface->pltCurObject;
	pIMEInterface->pltCurObject = pltCurObject;
	// ��¼��ǰLayout�����ʹ��ʱ��
	pltCurObject->lastUseTime = PT_GetCurrentTime();

}

/*!
 * \brief Ҫ��ʼ����Layout ID�Ƿ񱻱�����CIMEInterface::ltObjectArray
 * \param ltID 
 * \return ���ر�����������Layout Object ָ��
 */
PCLayoutAbstract Global_LayoutObjectIsLeaveByID(GBUINT16 ltID)
{
	GBINT32 isMatch = 0;
	GBINT32 ltObjIndex = 0;
	PCIMEInterface pIMEInterface = Global_GetIMEInterface();
	PCLayoutAbstract pTempObject = NULL;

	PT_Assert(Global_GetIMEInterface()->bFrameworkInited);

	for (ltObjIndex=0; ltObjIndex < MAX_LEAVE_LAYOUT_OBJECT_NUM; ltObjIndex++)
	{
		pTempObject = pIMEInterface->ltObjectArray[ltObjIndex];

		if (pTempObject != NULL && pTempObject->m_pfcIsMatchLtObjAndLtID != NULL)
		{
			if (pTempObject->m_pfcIsMatchLtObjAndLtID(pTempObject, ltID) == 1)
			{
				return pTempObject;
			}
		}
	}

	return NULL;
}

/*!
 * \brief ���ص�ǰLayout Object
 * \param void 
 * \return 
 */
PCLayoutAbstract Global_GetCurrentLayoutObject(void)
{
	PT_Assert(Global_GetIMEInterface()->bFrameworkInited);
	return Global_GetIMEInterface()->pltCurObject;
}

/*!
 * \brief ����ǰһ��Layout Object
 * \param void 
 * \return 
 */
PCLayoutAbstract Global_GetPrevLayoutObject(void)
{
	PT_Assert(Global_GetIMEInterface()->bFrameworkInited);
	return Global_GetIMEInterface()->pltPreObject;
}

/*!
 * \brief ��ȡLayout���ڼ���״̬
 * \param void 
 * \return 
 */
GBBOOL Global_GetLayoutWinActiveStatus(void)
{
	PT_Assert(Global_GetIMEInterface()->bFrameworkInited);
	return Global_GetIMEInterface()->layoutWinActiveStatus;
}

/*!
 * \brief ����Layout���ڼ���״̬
 * \param status 
 * \return 
 */
void Global_SetLayoutWinActiveStatus(GBBOOL status)
{
	PT_Assert(Global_GetIMEInterface()->bFrameworkInited);
	Global_GetIMEInterface()->layoutWinActiveStatus = status;
}

/*!
 * \brief ��ȡ��Сд״̬
 * \param void 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-10-22 10:04:44
*/
GBShiftCapStatus Global_GetShiftCapStatus(void)
{
	PT_Assert(Global_GetIMEInterface()->bFrameworkInited);
	return Global_GetIMEInterface()->shiftCapStatus;
}

/*!
 * \brief ���ô�Сд״̬
 * \param status 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-10-22 10:04:55
*/
void Global_SetShiftCapStatus(GBShiftCapStatus status)
{
	PT_Assert(Global_GetIMEInterface()->bFrameworkInited);
	Global_GetIMEInterface()->shiftCapStatus = status;
}

/*!
 * \brief �������뷨Layout���ڵ���ʼ��ʾλ��(�����½��������Ļ�����꣬��Ϊ���Ͻǻ����ź�ѡ��߶ȱ仯)
 * \param pWinPos 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-21 11:28:23
 */
GBIMEReturn Global_SetWinPos(GBIMEPoint *pWinPos)
{
	PT_Assert(Global_GetIMEInterface()->bFrameworkInited);
	PT_Assert(pWinPos != NULL);

	if (!(pWinPos->x >= 0 && pWinPos->x < PT_GetLcdWidth())
		|| !(pWinPos->y >= 0 && pWinPos->y < PT_GetLcdHeight()))
	{
		return GBIME_INVALID_PARAM;
	}

	Global_GetIMEInterface()->layoutWinStartPos.x = pWinPos->x;
	Global_GetIMEInterface()->layoutWinStartPos.y = pWinPos->y;

	return GBIME_OK;
}

/*!
 * \brief ��ȡ���뷨Layout���ڵ���ʼ��ʾλ��(�����½��������Ļ�����꣬��Ϊ���Ͻǻ����ź�ѡ��߶ȱ仯)
 * \param pWinPos 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-21 11:28:37
 */
GBIMEReturn Global_GetWinPos(GBIMEPoint *pWinPos)
{
	PT_Assert(Global_GetIMEInterface()->bFrameworkInited);
	PT_Assert(pWinPos != NULL);

	pWinPos->x = Global_GetIMEInterface()->layoutWinStartPos.x;
	pWinPos->y = Global_GetIMEInterface()->layoutWinStartPos.y;

	return GBIME_OK;
}

/*!
 * \brief ��ȡ��������ָ��
 * \return 
 * \autor huanjin
 * \data 2010-8-5
 */
GBLPCVOID Global_GetConfigData(void)
{
	PT_Assert(Global_GetIMEInterface()->bFrameworkInited);
	return Global_GetIMEInterface()->comIMEData.pconfData;
}

/*!
 * \brief ��ȡ������������ָ��
 * \return 
 * \autor huanjin
 * \data 2010-8-5
 */
GBLPCVOID Global_GetChnLangData(void)
{
#ifdef GBIME_CFG_CHS_SUPPORT // ����֧��
	PT_Assert(Global_GetIMEInterface()->bFrameworkInited);
	return Global_GetIMEInterface()->comIMEData.pimChnData;
#else
	return 0x0;
#endif
}

/*!
 * \brief ��ȡAlp��������ָ��
 * \return 
 * \autor huanjin
 * \data 2010-8-5
 */
GBLPCVOID Global_GetAlpLangData(void)
{
#ifdef GBIME_CFG_EN_SUPPORT // ����֧��
	PT_Assert(Global_GetIMEInterface()->bFrameworkInited);
	return Global_GetIMEInterface()->comIMEData.pimAlpData;
#else
	return 0x0;
#endif
}

/*!
 * \brief ��ȡ���뷨��������ָ��
 * \return 
 * \autor huanjin
 * \data 2010-8-5
 */
GBLPCVOID Global_GetIMEngineData(void)
{
	PT_Assert(Global_GetIMEInterface()->bFrameworkInited);
	return Global_GetIMEInterface()->comIMEData.pimEngineData;
}

/*!
 * \brief ��ȡ��д���뷨��������ָ��
 * \return 
 * \autor huanjin
 * \data 2010-8-5
 */
GBLPCVOID Global_GetHWEngineData(void)
{
#if defined(__GBIME_HANDWRITE__)
	PT_Assert(Global_GetIMEInterface()->bFrameworkInited);
	return Global_GetIMEInterface()->comIMEData.phwEngineData;
#else
	return 0x0;
#endif
}

/*!
 * \brief ��ȡ��UDB����ָ��
 * \return 
 * \autor huanjin
 * \data 2010-8-5
 */
GBLPCVOID Global_GetMasterUDBData(void)
{
#ifdef GBIME_CFG_UDB_SUPPORT
	PT_Assert(Global_GetIMEInterface()->bFrameworkInited);
	return Global_GetIMEInterface()->comIMEData.masterUDBData;
#else
	return 0x0;
#endif
}

/*!
 * \brief ��ȡ��UDB����ָ��
 * \return 
 * \autor huanjin
 * \data 2010-8-5
 */
GBLPCVOID Global_GetSlaveUDBData(void)
{
#ifdef GBIME_CFG_UDB_SUPPORT
	PT_Assert(Global_GetIMEInterface()->bFrameworkInited);
	return Global_GetIMEInterface()->comIMEData.slaveUDBData;
#else
	return 0x0;
#endif
}

/*!
 * \brief ��ȡ��UDB���ݴ�С
 * \return 
 * \autor huanjin
 * \data 2010-8-5
 */
unsigned long Global_GetMasterUDBDataSize(void)
{
#ifdef GBIME_CFG_UDB_SUPPORT
	PT_Assert(Global_GetIMEInterface()->bFrameworkInited);
	return MASTER_UDB_SIZE;
#else
	return 0x0;
#endif
}

/*!
 * \brief ��ȡ��UDB���ݴ�С
 * \return 
 * \autor huanjin
 * \data 2010-8-5
 */
unsigned long Global_GetSlaveUDBDataSize(void)
{
#ifdef GBIME_CFG_UDB_SUPPORT
	PT_Assert(Global_GetIMEInterface()->bFrameworkInited);
	return SLAVE_UDB_SIZE;
#else
	return 0x0;
#endif
}

/*!
 * \brief ��ȡ��д���뷨��������
 * \return 
 * \autor huanjin
 * \data 2010-8-5
 */
PSkinData Global_GetSkinInfoData(void)
{
	PT_Assert(Global_GetIMEInterface()->bFrameworkInited);
	return &Global_GetIMEInterface()->comIMEData.skinInfo;
}

/*!
 * \brief ��ȡ�������LayoutID(��: ���������뷨)
 * \param void 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-14 14:25:23
*/
GBUINT16 Global_GetLatestMasterLayoutID(void)
{
	PT_Assert(Global_GetIMEInterface()->bFrameworkInited);
	return Global_GetIMEInterface()->latestMasterLayoutID;
}

/*!
 * \brief ��ȡ���������LayoutID
 * \param void 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-14 14:25:23
*/
GBUINT16 Global_GetLatestChnLayoutID(void)
{
	KEYBOARD_TYPE keyboardType = GBConfig_GetKeyboardType(Global_GetConfigInstance());
	
	PT_Assert(Global_GetIMEInterface()->bFrameworkInited);
	
	switch (keyboardType)
	{
	case KEYBOARD_VK_NUMPAD:
		// ��һ�ε���ʹ��Ĭ��ֵ
		if (LayoutIdEnd == Global_GetIMEInterface()->latestChnNumLayoutID)
		{
			Global_GetIMEInterface()->latestChnNumLayoutID = 
				GBConfig_GetDefLatestChnLayoutID(Global_GetConfigInstance(), KEYBOARD_VK_NUMPAD);
		}
		return Global_GetIMEInterface()->latestChnNumLayoutID;
	case KEYBOARD_VK_QWERTY:
		// ��һ�ε���ʹ��Ĭ��ֵ
		if (LayoutIdEnd == Global_GetIMEInterface()->latestChnQwertyLayoutID)
		{
			Global_GetIMEInterface()->latestChnQwertyLayoutID = 
				GBConfig_GetDefLatestChnLayoutID(Global_GetConfigInstance(), KEYBOARD_VK_QWERTY);
		}
		return Global_GetIMEInterface()->latestChnQwertyLayoutID;
	default: // �������̲�֧��
		return LayoutIdEnd;
	}
	
	return LayoutIdEnd;
}

/*!
 * \brief ��ȡ���������LayoutID
 * \param void 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-14 14:25:23
*/
GBUINT16 Global_GetLatestAlpLayoutID(void)
{
	KEYBOARD_TYPE keyboardType = GBConfig_GetKeyboardType(Global_GetConfigInstance());

	PT_Assert(Global_GetIMEInterface()->bFrameworkInited);

	switch (keyboardType)
	{
	case KEYBOARD_VK_NUMPAD:
		// ��һ�ε���ʹ��Ĭ��ֵ
		if (LayoutIdEnd == Global_GetIMEInterface()->latestAlpNumLayoutID)
		{
			Global_GetIMEInterface()->latestAlpNumLayoutID = 
				GBConfig_GetDefLatestAlpLayoutID(Global_GetConfigInstance(), KEYBOARD_VK_NUMPAD);
		}
		return Global_GetIMEInterface()->latestAlpNumLayoutID;
	case KEYBOARD_VK_QWERTY:
		// ��һ�ε���ʹ��Ĭ��ֵ
		if (LayoutIdEnd == Global_GetIMEInterface()->latestAlpQwertyLayoutID)
		{
			Global_GetIMEInterface()->latestAlpQwertyLayoutID = 
				GBConfig_GetDefLatestAlpLayoutID(Global_GetConfigInstance(), KEYBOARD_VK_QWERTY);
		}
		return Global_GetIMEInterface()->latestAlpQwertyLayoutID;
	default: // �������̲�֧��
		return LayoutIdEnd;
	}

	return LayoutIdEnd;
}

/*!
 * \brief ���������LayoutID(����: ��Layout������Layout��Ӣ��Layout)
 * \param layoutID 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-14 14:28:37
*/
void Global_SaveLatestLayoutID(GBUINT16 layoutID)
{
	PT_Assert(Global_GetIMEInterface()->bFrameworkInited);

	// ��¼��Layout ID
	if (layoutID != Global_GetIMEInterface()->latestMasterLayoutID)
	{
		switch (layoutID)
		{
		case LayoutIdSP9JPinYin:
		case LayoutIdSP9JStroke:
		case LayoutIdSP9JLowerEnglish:
		case LayoutIdSP9JUpEnglish:
		case LayoutIdSP9Jabc:
		case LayoutIdSP9JABC:
		case LayoutIdSP26JPinYin:
		case LayoutIdSP26JStroke:
		case LayoutIdSP26JLowerEnglish:
		case LayoutIdSP26JUpEnglish:
		case LayoutIdSP26Jabc:
		case LayoutIdSP26JABC:
// 		case LayoutIdWL9JPinYin:
// 		case LayoutIdWL9JStroke:
// 		case LayoutIdWL9JEnglish:
// 		case LayoutIdWL9Jabc:
// 		case LayoutIdWL9JABC:
// 		case LayoutIdWL9JNumber:
// 		case LayoutIdWL26JPinYin:
// 		case LayoutIdWL26JStroke:
// 		case LayoutIdWL26JEnglish:
// 		case LayoutIdWL26Jabc:
// 		case LayoutIdWL26JABC:
// 		case LayoutIdWL26JNumber:
			Global_GetIMEInterface()->latestMasterLayoutID = layoutID;
		default:
			break;
		}
	}

	// ��¼�������ּ���Layout ID
	if (layoutID != Global_GetIMEInterface()->latestChnNumLayoutID)
	{
		switch (layoutID)
		{
		case LayoutIdSP9JPinYin:
		case LayoutIdSP9JStroke:
			Global_GetIMEInterface()->latestChnNumLayoutID = layoutID;
		default:
			break;
		}
	}

	// ��¼����Qwerty����Layout ID
	if (layoutID != Global_GetIMEInterface()->latestChnQwertyLayoutID)
	{
		switch (layoutID)
		{
		case LayoutIdSP26JPinYin:
		case LayoutIdSP26JStroke:
			Global_GetIMEInterface()->latestChnQwertyLayoutID = layoutID;
		default:
			break;
		}
	}

	// ��¼�������ּ���Layout ID
	if (layoutID != Global_GetIMEInterface()->latestAlpNumLayoutID)
	{
		switch (layoutID)
		{
		case LayoutIdSP9JLowerEnglish:
		case LayoutIdSP9JUpEnglish:
		case LayoutIdSP9Jabc:
		case LayoutIdSP9JABC:
			Global_GetIMEInterface()->latestAlpNumLayoutID = layoutID;
		default:
			break;
		}
	}

	// ��¼����Qwerty����Layout ID
	if (layoutID != Global_GetIMEInterface()->latestAlpQwertyLayoutID)
	{
		switch (layoutID)
		{
		case LayoutIdSP26JLowerEnglish:
		case LayoutIdSP26JUpEnglish:
		case LayoutIdSP26Jabc:
		case LayoutIdSP26JABC:
			Global_GetIMEInterface()->latestAlpQwertyLayoutID = layoutID;
		default:
			break;
		}
	}
}

/*!
 * \brief ���ݿ����Ϣת����������¼���
 * \param convertType 
 * \param wParam 
 * \param lParam 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-10-29 9:10:20
 */
GBIMEReturn Global_FillEventGroup(GBIMEConvertTypeEnum convertType, GBINT wParam, GBINT lParam)
{
	PGBIMEEventGroup pImeEventGroup = &Global_GetIMEInterface()->sysImeEventGroup;
	GBIMEReturn ret;
	GBU8 i;

	PT_Assert(!Global_GetIMEInterface()->sysImeEventGroupLock);

	// ��ֹͬʱ�����������
	if (!Global_GetIMEInterface()->sysImeEventGroupLock)
	{	
		Global_GetIMEInterface()->sysImeEventGroupLock = GBTrue;
		if ((ret = GBU_GetEventGroupByConvertType(convertType, pImeEventGroup)) != GBIME_OK)
		{
			return ret;
		}
		
		for (i=0; i<pImeEventGroup->eventNum; i++)
		{
			pImeEventGroup->eventGroup[i].wParam = wParam;
			pImeEventGroup->eventGroup[i].lParam = lParam;
		}
	}

	Global_GetIMEInterface()->sysImeEventGroupLock = GBFalse;

	return GBIME_OK;
}

/*!
 * \brief ���ݿ����Ϣת�������������¼���
 * \param convertType 
 * \param wParam 
 * \param lParam 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-10-29 9:10:20
 */
GBIMEReturn Global_FillPostEventGroup(GBIMEConvertTypeEnum convertType, GBINT wParam, GBINT lParam)
{
	PGBIMEEventGroup pImeEventGroup = &Global_GetIMEInterface()->sysImePostEventGroup;
	GBIMEReturn ret;
	GBU8 i;

	PT_Assert(!Global_GetIMEInterface()->sysImePostEventGroupLock);

	// ��ֹͬʱ�����������
	if (!Global_GetIMEInterface()->sysImePostEventGroupLock)
	{	
		Global_GetIMEInterface()->sysImePostEventGroupLock = GBTrue;
		if ((ret = GBU_GetEventGroupByConvertType(convertType, pImeEventGroup)) != GBIME_OK)
		{
			return ret;
		}
		
		for (i=0; i<pImeEventGroup->eventNum; i++)
		{
			pImeEventGroup->eventGroup[i].wParam = wParam;
			pImeEventGroup->eventGroup[i].lParam = lParam;
		}
	}	

	Global_GetIMEInterface()->sysImePostEventGroupLock = GBFalse;

	return GBIME_OK;
}

/*!
 * \brief �����¼��飬��������δ������䴦�������¼�
 * \param void 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-10-29 9:10:26
 */
GBIMEReturn Global_ResetEventGroup(void)
{
	memset(&Global_GetIMEInterface()->sysImeEventGroup, 0, sizeof(GBIMEEventGroup));
	memset(&Global_GetIMEInterface()->sysImePostEventGroup, 0, sizeof(GBIMEEventGroup));
	Global_GetIMEInterface()->sysImeEventGroupLock = GBFalse;
	Global_GetIMEInterface()->sysImePostEventGroupLock = GBFalse;
	return GBIME_OK;
}

/*!
 * \brief ��ȡ�����Ϣ�¼���
 * \param void 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-10-29 9:10:31
 */
PGBIMEEventGroup Global_GetIMEEventGroup(void)
{
	return &Global_GetIMEInterface()->sysImeEventGroup;
}

/*!
 * \brief ��ȡ�����Ϣ�����¼���
 * \param void 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-10-29 9:10:31
 */
PGBIMEEventGroup Global_GetIMEPostEventGroup(void)
{
	return &Global_GetIMEInterface()->sysImePostEventGroup;
}

/*!
 * \brief ������������л���������̱��
 * \param flag 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-10-29 18:21:12
 */
void Global_SetVk2KbSwitchFlag(GBBOOL flag)
{
	PT_Assert(Global_GetIMEInterface()->bFrameworkInited);
	Global_GetIMEInterface()->bVk2KbSwitchFlag = flag;
}

/*!
 * \brief ��ȡ��������л���������̱��
 * \param void 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-10-29 18:21:29
 */
GBBOOL Global_GetVk2KbSwitchFlag(void)
{
	PT_Assert(Global_GetIMEInterface()->bFrameworkInited);
	return Global_GetIMEInterface()->bVk2KbSwitchFlag;
}

/*!
 * \brief ������������л����������ʱ�û����µ���������ֵ
 * \param keyValue 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-10-29 18:22:07
 */
void Global_SetVk2KbSwitchEngineKeyValue(GBINT keyValue)
{
	PT_Assert(Global_GetIMEInterface()->bFrameworkInited);
	Global_GetIMEInterface()->Vk2KbSwitchEngineKeyValue = keyValue;
}

/*!
 * \brief ��ȡ��������л����������ʱ�û����µ���������ֵ
 * \param oid 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-10-29 18:22:10
 */
GBINT Global_GetVk2KbSwitchEngineKeyValue(void)
{
	PT_Assert(Global_GetIMEInterface()->bFrameworkInited);
	return Global_GetIMEInterface()->Vk2KbSwitchEngineKeyValue;
}

/*!
 * \brief ����Layout��ʾ���(��ʼ���趨==��Ļ���)
 * \param newWidth 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-10-31 12:58:27
 */
void Global_SetLayoutWinWidth(GBINT newWidth)
{
	PT_Assert(Global_GetIMEInterface()->bFrameworkInited);
	Global_GetIMEInterface()->layoutWinWidth = newWidth;
}

/*!
 * \brief ��ȡLayout��ʾ���
 * \param void 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-10-31 12:58:44
 */
GBINT Global_GetLayoutWinWidth(void)
{
	PT_Assert(Global_GetIMEInterface()->bFrameworkInited);
	return Global_GetIMEInterface()->layoutWinWidth;
}

/*!
 * \brief ���û�ȡLayout��ʾ�߶�(��̬����)
 * \param newHeight 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-10-31 12:58:27
 */
void Global_SetLayoutWinHeight(GBINT newHeight)
{
	PT_Assert(Global_GetIMEInterface()->bFrameworkInited);
	Global_GetIMEInterface()->layoutWinHeight = newHeight;
}

/*!
 * \brief ��ȡLayout��ʾ�߶�
 * \param void 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-10-31 12:58:44
 */
GBINT Global_GetLayoutWinHeight(void)
{
	PT_Assert(Global_GetIMEInterface()->bFrameworkInited);
	return Global_GetIMEInterface()->layoutWinHeight;
}


/*!
 * \brief ��ȡ���е�Layout���󱣴������±�
 * \param void 
 * \return 
 * \note ��� ltObjIndex == MAX_LEAVE_LAYOUT_OBJECT_NUM��ʾ��������Ҫ����Ӧ����
 * \example
 * \author weizhiping
 * \date 2010-12-9 18:52:39
*/
GBIMELOCAL GBINT Global_GetLtObjectArrayFreeIndex(void)
{
	PCIMEInterface pIMEInterface = Global_GetIMEInterface();
	GBINT ltObjIndex = 0;
	
	PT_Assert(Global_GetIMEInterface()->bFrameworkInited);

	for (ltObjIndex = 0; ltObjIndex < MAX_LEAVE_LAYOUT_OBJECT_NUM; ltObjIndex++)
	{
		if (pIMEInterface->ltObjectArray[ltObjIndex] == NULL)
		{
			break;
		}
	}
	
	return ltObjIndex;
}

/*!
 * \brief ������Ҫ�Ƴ���Layout�����±�(�ϴ�ʹ��ʱ�������)
 * \param void 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-12-9 19:19:44
*/
GBIMELOCAL GBINT Global_GetLtObjectArrayRemoveIndex(void)
{
	PCIMEInterface pIMEInterface = Global_GetIMEInterface();
	GBINT ltObjIndex = 0;
	GBINT removeIndex = 0;
	GBINT oldestUseTime = pIMEInterface->ltObjectArray[0]->lastUseTime;
	PCLayoutAbstract pTempLtObject = NULL;
	
	PT_Assert(Global_GetIMEInterface()->bFrameworkInited);

	for (ltObjIndex = 1; ltObjIndex < MAX_LEAVE_LAYOUT_OBJECT_NUM; ltObjIndex++)
	{
		pTempLtObject = pIMEInterface->ltObjectArray[ltObjIndex];
		if (pTempLtObject->lastUseTime < oldestUseTime)
		{
			oldestUseTime = pTempLtObject->lastUseTime;
			removeIndex = ltObjIndex;
		}
	}
	
	return removeIndex;
}