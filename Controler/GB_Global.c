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

// 语言数据
#ifdef GBIME_CFG_STATIC_DB // 使用静态数据
	#ifndef _C166
	#define _huge
	#endif //_C166

	#ifdef GBIME_CFG_CHS_SUPPORT // 中文支持
	//#include "gb_sc.c"
	extern const unsigned long _huge prv_scDataArray[];
	#endif/*GBIME_CFG_CHS_SUPPORT*/

	#ifdef GBIME_CFG_EN_SUPPORT // 英文支持
	extern const unsigned long _huge prv_enDataArray[];
	#endif/*GBIME_CFG_EN_SUPPORT*/

#endif // GBIME_CFG_STATIC_DB

#ifdef GBIME_CFG_UDB_SUPPORT // 自造词数据
	#define MASTER_UDB_SIZE (8 * 1024)
	static unsigned long _master_udb_buffer[MASTER_UDB_SIZE]; // 主UDB buffer空间
	#define SLAVE_UDB_SIZE  (2 * 1024)
	static unsigned long _slave_udb_buffer[SLAVE_UDB_SIZE]; // 辅UDB buffer空间
#endif/*GBIME_CFG_UDB_SUPPORT*/

#if defined(__GBIME_HANDWRITE__) // 手写数据
	extern const unsigned long _huge prv_HWDataArray[];
	//extern const unsigned long _huge prv_BigHWDataArray[]; // 暂时不采用大端的数据
#endif/*defined(__GBIME_HANDWRITE__)*/

/*!
 * \ 局部功能函数声明
 */
GBIMELOCAL GBINT Global_GetLtObjectArrayFreeIndex(void);
GBIMELOCAL GBINT Global_GetLtObjectArrayRemoveIndex(void);

/*!
 * \brief 初始化输入法框架全局数据buffer指针
 * \param void 
 * \return 
 */
static void Global_InitComIMEData(void)
{
	PCIMEInterface pIMEInterface = Global_GetIMEInterface();

	pIMEInterface->comIMEData.pskinData = V5LayoutDataArray;
	pIMEInterface->comIMEData.pconfData = NULL;
#ifdef GBIME_CFG_CHS_SUPPORT // 中文支持
	pIMEInterface->comIMEData.pimChnData = prv_scDataArray;
#endif
#ifdef GBIME_CFG_EN_SUPPORT // 英文支持
	pIMEInterface->comIMEData.pimAlpData = prv_enDataArray;
#endif
	pIMEInterface->comIMEData.pimEngineData = NULL;
#if defined(__GBIME_HANDWRITE__) // 手写引擎支持
	pIMEInterface->comIMEData.phwEngineData = prv_HWDataArray;
#endif
#ifdef GBIME_CFG_UDB_SUPPORT // 自造词支持
	pIMEInterface->comIMEData.masterUDBData = _master_udb_buffer;
	pIMEInterface->comIMEData.slaveUDBData = _slave_udb_buffer;
#endif
}

/*!
 * \brief 初始化输入法框架全局变量接口对象
 * \param void 
 * \return 
 */
void Global_InitIMEInterface(void)
{
	PCIMEInterface pIMEInterface = Global_GetIMEInterface();

	// 清零结构体
	memset(pIMEInterface, 0, sizeof(CIMEInterface));

	// 初始化框架通用的全局数据buffer
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

	// 初始化皮肤数据
	CLayoutInfo_SkinInitData(&pIMEInterface->comIMEData.skinInfo, pIMEInterface->comIMEData.pskinData);

	pIMEInterface->bFrameworkInited = GBTrue;
	pIMEInterface->latestChnNumLayoutID = LayoutIdEnd;
	pIMEInterface->latestChnQwertyLayoutID = LayoutIdEnd;
	pIMEInterface->latestAlpNumLayoutID = LayoutIdEnd;
	pIMEInterface->latestAlpQwertyLayoutID = LayoutIdEnd;
}

/*!
 * \brief 销毁输入法框架全局变量接口对象
 * \param void 
 * \return 
 */
void Global_ReleaseIMEInterface(void)
{
	PCIMEInterface pIMEInterface = Global_GetIMEInterface();
	GBINT32 idx = 0;
	PCLayoutAbstract pltDest = NULL;

	// 释放Layout对象申请的动态内存
	for (idx=0; idx < MAX_LEAVE_LAYOUT_OBJECT_NUM; idx++)
	{
		pltDest = (PCLayoutAbstract)pIMEInterface->ltObjectArray[idx];
		if (pltDest != NULL)
		{
			// 调用析构函数释放皮肤数据申请的动态内存
			if (pltDest->m_pfcRelease != NULL)
			{
				pltDest->m_pfcRelease(pltDest, &pltDest->m_ltData);
			}
			// 释放视图类
			//该处不能释放pltDest->m_hltView，外面会用到
			//if (pltDest->m_hltView != NULL)
			//{
			//	PT_Free(pltDest->m_hltView);
			//	pltDest->m_hltView = NULL;
			//}
			// 释放自身引用
			PT_Free(pltDest);
			pltDest = NULL;
		}
	}

	// 释放其他对象申请的动态内存
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

	// 清零结构体
	memset(pIMEInterface, 0, sizeof(CIMEInterface));
}

/*!
 * \brief 框架接口对象是否初始化
 * \param void 
 * \return 
 */
GBBOOL Global_IMEInterfaceIsInited(void)
{
	return Global_GetIMEInterface()->bFrameworkInited;
}

/*!
 * \brief 获取IME模块连接状态
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
 * \brief 设置IME模块连接状态
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
 * \brief Layout 的对象是否已经被保留
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
 * \brief 保存当前Layout对象
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
		
		// 存储空间未满
		if (saveIndex != MAX_LEAVE_LAYOUT_OBJECT_NUM)
		{
			pIMEInterface->ltObjectArray[saveIndex] = pltCurObject;
			// 更新当前对象和上一个对象的指针引用及使用时间
			Global_SetCurrentLayoutObject(pltCurObject);
			return pltCurObject;
		}
		else // 超过最大历史Layout保留个数，替换最老的(上次使用时间最早)
		{
			GBINT removeIndex = Global_GetLtObjectArrayRemoveIndex();
			PCLayoutAbstract pRemoveLtObject = pIMEInterface->ltObjectArray[removeIndex];

			// 释放removeIndex对应的Layout对象
			if (pRemoveLtObject != NULL 
				&& pRemoveLtObject->m_pfcRelease != NULL)
			{
				pRemoveLtObject->m_pfcRelease(pRemoveLtObject, &pRemoveLtObject->m_ltData);
				PT_Free(pRemoveLtObject);
				pRemoveLtObject = NULL;			
			}

			// 移除后的该空闲对象指针重新指向当前对象
			pIMEInterface->ltObjectArray[removeIndex] = pltCurObject;
			// 更新当前对象和上一个对象的指针引用及使用时间
			Global_SetCurrentLayoutObject(pltCurObject);
			return pltCurObject;
		}
	}

	return NULL;
}

/*!
 * \brief 设置当前激活的Layout对象
 * \param pltCurObject 
 * \return 返回当前Layout 对象
 */
void Global_SetCurrentLayoutObject(PCLayoutAbstract pltCurObject)
{
	PCIMEInterface pIMEInterface = Global_GetIMEInterface();
	GBINT ltObjIndex = 0;
	PCLayoutAbstract pTempLtObject = NULL;
	
	PT_Assert(Global_GetIMEInterface()->bFrameworkInited);
	PT_Assert(pltCurObject);

	// 当前对象就是目标对象，直接返回
	if (pltCurObject == pIMEInterface->pltCurObject)
	{
		return ;
	}

	pIMEInterface->pltPreObject = pIMEInterface->pltCurObject;
	pIMEInterface->pltCurObject = pltCurObject;
	// 记录当前Layout对象的使用时间
	pltCurObject->lastUseTime = PT_GetCurrentTime();

}

/*!
 * \brief 要初始化的Layout ID是否被保留在CIMEInterface::ltObjectArray
 * \param ltID 
 * \return 返回被保留下来的Layout Object 指针
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
 * \brief 返回当前Layout Object
 * \param void 
 * \return 
 */
PCLayoutAbstract Global_GetCurrentLayoutObject(void)
{
	PT_Assert(Global_GetIMEInterface()->bFrameworkInited);
	return Global_GetIMEInterface()->pltCurObject;
}

/*!
 * \brief 返回前一个Layout Object
 * \param void 
 * \return 
 */
PCLayoutAbstract Global_GetPrevLayoutObject(void)
{
	PT_Assert(Global_GetIMEInterface()->bFrameworkInited);
	return Global_GetIMEInterface()->pltPreObject;
}

/*!
 * \brief 获取Layout窗口激活状态
 * \param void 
 * \return 
 */
GBBOOL Global_GetLayoutWinActiveStatus(void)
{
	PT_Assert(Global_GetIMEInterface()->bFrameworkInited);
	return Global_GetIMEInterface()->layoutWinActiveStatus;
}

/*!
 * \brief 设置Layout窗口激活状态
 * \param status 
 * \return 
 */
void Global_SetLayoutWinActiveStatus(GBBOOL status)
{
	PT_Assert(Global_GetIMEInterface()->bFrameworkInited);
	Global_GetIMEInterface()->layoutWinActiveStatus = status;
}

/*!
 * \brief 获取大小写状态
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
 * \brief 设置大小写状态
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
 * \brief 设置输入法Layout窗口的起始显示位置(即左下角相对于屏幕的坐标，因为左上角会随着候选框高度变化)
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
 * \brief 获取输入法Layout窗口的起始显示位置(即左下角相对于屏幕的坐标，因为左上角会随着候选框高度变化)
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
 * \brief 获取配置数据指针
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
 * \brief 获取中文语言数据指针
 * \return 
 * \autor huanjin
 * \data 2010-8-5
 */
GBLPCVOID Global_GetChnLangData(void)
{
#ifdef GBIME_CFG_CHS_SUPPORT // 中文支持
	PT_Assert(Global_GetIMEInterface()->bFrameworkInited);
	return Global_GetIMEInterface()->comIMEData.pimChnData;
#else
	return 0x0;
#endif
}

/*!
 * \brief 获取Alp语言数据指针
 * \return 
 * \autor huanjin
 * \data 2010-8-5
 */
GBLPCVOID Global_GetAlpLangData(void)
{
#ifdef GBIME_CFG_EN_SUPPORT // 中文支持
	PT_Assert(Global_GetIMEInterface()->bFrameworkInited);
	return Global_GetIMEInterface()->comIMEData.pimAlpData;
#else
	return 0x0;
#endif
}

/*!
 * \brief 获取输入法引擎数据指针
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
 * \brief 获取手写输入法引擎数据指针
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
 * \brief 获取主UDB数据指针
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
 * \brief 获取辅UDB数据指针
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
 * \brief 获取主UDB数据大小
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
 * \brief 获取辅UDB数据大小
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
 * \brief 获取手写输入法引擎数据
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
 * \brief 获取最近的主LayoutID(如: 主语言输入法)
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
 * \brief 获取最近的中文LayoutID
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
		// 第一次调用使用默认值
		if (LayoutIdEnd == Global_GetIMEInterface()->latestChnNumLayoutID)
		{
			Global_GetIMEInterface()->latestChnNumLayoutID = 
				GBConfig_GetDefLatestChnLayoutID(Global_GetConfigInstance(), KEYBOARD_VK_NUMPAD);
		}
		return Global_GetIMEInterface()->latestChnNumLayoutID;
	case KEYBOARD_VK_QWERTY:
		// 第一次调用使用默认值
		if (LayoutIdEnd == Global_GetIMEInterface()->latestChnQwertyLayoutID)
		{
			Global_GetIMEInterface()->latestChnQwertyLayoutID = 
				GBConfig_GetDefLatestChnLayoutID(Global_GetConfigInstance(), KEYBOARD_VK_QWERTY);
		}
		return Global_GetIMEInterface()->latestChnQwertyLayoutID;
	default: // 其他键盘不支持
		return LayoutIdEnd;
	}
	
	return LayoutIdEnd;
}

/*!
 * \brief 获取最近的拉丁LayoutID
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
		// 第一次调用使用默认值
		if (LayoutIdEnd == Global_GetIMEInterface()->latestAlpNumLayoutID)
		{
			Global_GetIMEInterface()->latestAlpNumLayoutID = 
				GBConfig_GetDefLatestAlpLayoutID(Global_GetConfigInstance(), KEYBOARD_VK_NUMPAD);
		}
		return Global_GetIMEInterface()->latestAlpNumLayoutID;
	case KEYBOARD_VK_QWERTY:
		// 第一次调用使用默认值
		if (LayoutIdEnd == Global_GetIMEInterface()->latestAlpQwertyLayoutID)
		{
			Global_GetIMEInterface()->latestAlpQwertyLayoutID = 
				GBConfig_GetDefLatestAlpLayoutID(Global_GetConfigInstance(), KEYBOARD_VK_QWERTY);
		}
		return Global_GetIMEInterface()->latestAlpQwertyLayoutID;
	default: // 其他键盘不支持
		return LayoutIdEnd;
	}

	return LayoutIdEnd;
}

/*!
 * \brief 保存最近的LayoutID(包括: 主Layout、中文Layout、英文Layout)
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

	// 记录主Layout ID
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

	// 记录中文数字键盘Layout ID
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

	// 记录中文Qwerty键盘Layout ID
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

	// 记录拉丁数字键盘Layout ID
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

	// 记录拉丁Qwerty键盘Layout ID
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
 * \brief 根据框架消息转换类型填充事件组
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

	// 防止同时调用填充代码段
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
 * \brief 根据框架消息转换类型填充后处理事件组
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

	// 防止同时调用填充代码段
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
 * \brief 重置事件组，避免由于未重新填充处理到残留事件
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
 * \brief 获取框架消息事件组
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
 * \brief 获取框架消息后处理事件组
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
 * \brief 设置虚拟键盘切换到物理键盘标记
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
 * \brief 获取虚拟键盘切换到物理键盘标记
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
 * \brief 设置虚拟键盘切换到物理键盘时用户按下的物理按键键值
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
 * \brief 获取虚拟键盘切换到物理键盘时用户按下的物理按键键值
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
 * \brief 设置Layout显示宽度(初始化设定==屏幕宽度)
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
 * \brief 获取Layout显示宽度
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
 * \brief 设置获取Layout显示高度(动态更新)
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
 * \brief 获取Layout显示高度
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
 * \brief 获取空闲的Layout对象保存数组下标
 * \param void 
 * \return 
 * \note 如果 ltObjIndex == MAX_LEAVE_LAYOUT_OBJECT_NUM表示已满，需要做相应处理
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
 * \brief 返回需要移除的Layout对象下标(上次使用时间最早的)
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