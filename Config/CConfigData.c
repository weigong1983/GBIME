/***************************************************************************
* File Name    :  CConfigData.h
* Corporation  :  Guobi Technology Ltd. GuangZhou China
* Copyright    :  Copyright Guobi Technology Ltd. And Subject to Confidentiality 
*                 Restrictions. This document is the confidential and proprietary 
*                 property of Guobi Technology Ltd.It is neither the document                  
*                 itself nor any part of it can be shared with any party  without 
*                 the written  permission. 
* Author       :  ZhaoKun
* Date         :  2010-08-23
* Purpose      :  配置数据读取文件
* Version      :  V1.0
* Last Changed List
* Name         |   date   |  Discription
****************************************************************************/
#include "CConfigData.h"
#include "GB_Global.h"
#include "CGBEngine.h"
#if __MTK__ > 0
#include "GlobalMenuItems.h"
#include "CommonScreens.h"

#endif

#ifdef WIN32
#define _CRT_SECURE_NO_DEPRECATE
#include "stdio.h"
#include "wchar.h"
#endif

CFG_HEADER	g_cfg_header;
CFG_TAILS	g_cfg_tails[16] = {0};
GBUINT16	g_cfgTailCount;
GBINT		g_curProductID = -1;
GBWCHAR		g_configFilePath[GB_MAX_FILE_NAME_LEN] = {0};


/*!
 * \brief 释放配置缓存
 * \return  
 * \note 
 * \example
 * \author ZhaoKun
 * \date
 */
GBINT FreeConfig(void)
{
	GBINT i = 0;
	GBINT menuIndex = 0;
	pGBConfigData pConfigData = NULL;

	for (i = 0; i < g_cfgTailCount; i++)
	{
		for (menuIndex = 0; menuIndex < g_cfg_tails[i].configCount; menuIndex++)
		{
			pConfigData = g_cfg_tails[i].configData;
			if (pConfigData[menuIndex].configValueType != INT_TYPE
				&& pConfigData[menuIndex].configValueType != STRING_TYPE
				&& pConfigData[menuIndex].chdConfigData)
			{
				PT_Free(pConfigData[menuIndex].chdConfigData);
				pConfigData[menuIndex].chdConfigData = NULL;
			}

		}

	}
	memset(g_cfg_tails, 0, sizeof(g_cfg_tails));
	g_cfgTailCount = 0;
	g_curProductID = -1;
	memset(g_configFilePath, 0, sizeof(g_configFilePath));
	return 1;
}
/*!
 * \brief 设置全部引擎配置
 * \return  
 * \note 
 * \example
 * \author ZhaoKun
 * \date
 */
GBINT SetConfigFile(void)
{
	GBINT i = 0;
	GBINT j = 0;
	GBINT nChdCount = 0;
	GBINT configCount;
	GBConfigData* pConfigData = NULL;
	GBConfigData* pChdConfigData = NULL;
	GBU32 value = 0;

	for (i = 0; i < g_cfgTailCount; i++)
	{
		configCount = g_cfg_tails[i].configCount;
		for (j = 0; j < configCount; j++)
		{
			pConfigData = &g_cfg_tails[i].configData[j];

			for (nChdCount = 0; nChdCount < pConfigData->chdCount; nChdCount++)
			{
				pChdConfigData = &pConfigData->chdConfigData[nChdCount];
				switch (pChdConfigData->configTypeID)
				{
				case GB_ENGINE_CHINESE:
				case GB_ENGINE_ALPHABETIC:
				case GB_ENGINE_SYNTAX:
				case GB_ENGINE_FUNCTION:
#if GB_SUPPORT_KOREAN > 0
				case GB_ENGINE_KOREAN:
#endif				
					value = gbwtoi(pChdConfigData->szValue);

					GBEngine_SetEngineOption(Global_GetEnineInstance(),
						pChdConfigData->configTypeID, pChdConfigData->configID, value);
					break;
				case COMMON_CONFIG:
					break;	
				default:
					break;
				}		
			}
			if (!pConfigData->chdCount)
			{
				switch (pConfigData->configTypeID)
				{
				case GB_ENGINE_CHINESE:
				case GB_ENGINE_ALPHABETIC:
				case GB_ENGINE_SYNTAX:
				case GB_ENGINE_FUNCTION:
#if GB_SUPPORT_KOREAN > 0
				case GB_ENGINE_KOREAN:
#endif				
					value = gbwtoi(pConfigData->szValue);
					
					GBEngine_SetEngineOption(Global_GetEnineInstance(),
						pConfigData->configTypeID, pConfigData->configID, value);
					break;
				case COMMON_CONFIG:
					break;	
				default:
					break;
				}		
			}
		}
	}
	return 1;
}
/*!
 * \brief 通过字符串id设置引擎配置
 * \param menuStrId 
 * \param pValue
 * \param valueCount 当pValue为STRING_TYPE类型时可以
 * \return  
 * \note 
 * \example
 * \author ZhaoKun
 * \date
 */
GBINT SetEngineForStrID(GBINT menuStrId, void* pValue, GBINT valueCount)
{
	GBINT i = 0;
	GBINT j = 0;
	GBINT nChdCount = 0;
	GBConfigData* pConfigData = NULL;
	GBConfigData* pChdConfigData = NULL;
	GBINT configCount;
	GBINT isFind = 0;
	GBU32 value = 0;

	if (pValue == NULL)
	{
		return -1;
	}
	for (i = 0; i < g_cfgTailCount; i++)
	{
		configCount = g_cfg_tails[i].configCount;

		for (j = 0; j < configCount; j++)
		{
			pConfigData = &g_cfg_tails[i].configData[j];

			for (nChdCount = 0; nChdCount < pConfigData->chdCount; nChdCount++)
			{
				pChdConfigData = &pConfigData->chdConfigData[nChdCount];
				if (pChdConfigData->menuStrId != menuStrId - GBIME_BASE)
				{
					continue;
				}
				isFind = 1;
				switch (pChdConfigData->configTypeID)
				{
				case GB_ENGINE_CHINESE:
				case GB_ENGINE_ALPHABETIC:
				case GB_ENGINE_SYNTAX:
				case GB_ENGINE_FUNCTION:
#if GB_SUPPORT_KOREAN > 0
				case GB_ENGINE_KOREAN:
#endif
					if (pChdConfigData->configValueType == INT_TYPE)
					{
						value = *(GBUINT8 *)pValue;
					} 
					else if (pChdConfigData->configValueType == STRING_TYPE)
					{
						value = gbwtoi(pChdConfigData->szValue);

					}
					GBEngine_SetEngineOption(Global_GetEnineInstance(),
						pChdConfigData->configTypeID, pChdConfigData->configID, value);								
					break;
				case COMMON_CONFIG:
					break;	
				default:
					break;
				}
				break;
			}
			if (!pConfigData->chdCount)
			{
				if (pConfigData->menuStrId != menuStrId - GBIME_BASE)
				{
					continue;
				}
				isFind = 1;
				switch (pConfigData->configTypeID)
				{
				case GB_ENGINE_CHINESE:
				case GB_ENGINE_ALPHABETIC:
				case GB_ENGINE_SYNTAX:
				case GB_ENGINE_FUNCTION:
#if GB_SUPPORT_KOREAN > 0
				case GB_ENGINE_KOREAN:
#endif
					if (pConfigData->configValueType == INT_TYPE)
					{
						value = *(GBUINT8 *)pValue;
					} 
					else if (pConfigData->configValueType == STRING_TYPE)
					{
						value = gbwtoi(pConfigData->szValue);
						
					}
					GBEngine_SetEngineOption(Global_GetEnineInstance(),
						pConfigData->configTypeID, pConfigData->configID, value);								
					break;
				case COMMON_CONFIG:
					break;	
				default:
					break;
				}
			}
			
		}
		if (isFind)
		{
			break;
		}
	}
	return 1;
}
/*!
 * \brief 获取当前产品ID
 * \param 
 * \return  
 * \note 
 * \example
 * \author ZhaoKun
 * \date
 */
GBINT GetCurProductID(void)
{
	return g_curProductID;
}
/*!
 * \brief 通过文件路径加载配置文件
 * \param (in) pFilePath 配置文件路径
 * \return  
 * \note 
 * \example
 * \author ZhaoKun
 * \date
 */
GBINT LoadConfigFileForPath(GBPWCHAR pFilePath)
{
	PT_FILE_HANDLE file;
	GBINT i = 0;
	GBINT j = 0;
	GBINT offset = 0;
	pGBConfigData pConfigData;
	pGBConfigData pLangConfigData;
	GBINT langSize = 0;
	GBINT length = 0;
	GBINT menuCount = 0;

	file = PT_FS_Open(pFilePath, PT_FS_READ);
	
	if(!file)
		return -1;

	PT_wcscpy(g_configFilePath, pFilePath);

	PT_FS_Read(file, &g_cfg_header, sizeof(CFG_HEADER), &length);
	
	if(strcmp(g_cfg_header.cfg, SIGNATURE))
	{
		return -2;
	}

	g_cfgTailCount = g_cfg_header.langCount;

	for (i = 0; i < g_cfg_header.langCount; i++)
	{
		offset = sizeof(CFG_HEADER) + g_cfg_header.commonConfig[i].offset;

		PT_FS_Seek(file, offset, SEEK_SET);		

		g_cfg_tails[i].lang = g_cfg_header.commonConfig[i].lang;
		g_cfg_tails[i].subLang = g_cfg_header.commonConfig[i].subLang;
		g_cfg_tails[i].configCount = g_cfg_header.commonConfig[i].itemNumber;
		g_cfg_tails[i].menuStrId = g_cfg_header.commonConfig[i].menuStrID;
		g_cfg_tails[i].menuType = g_cfg_header.commonConfig[i].menuType;

		langSize = g_cfg_header.commonConfig[i].itemNumber * sizeof(GBConfigData);

		//在没有子菜单的情况下
		if (!langSize)
		{
			continue;
		}
		pLangConfigData = (pGBConfigData)PT_Malloc(langSize);

		PT_FS_Read(file, pLangConfigData, langSize, &length);

		for (j = 0, menuCount = 0; j < g_cfg_header.commonConfig[i].itemNumber; menuCount++, j++)
		{
			pConfigData = &g_cfg_tails[i].configData[menuCount];
			memcpy(pConfigData, &pLangConfigData[j], sizeof(GBConfigData));
			if (pLangConfigData[j].configValueType != INT_TYPE
				&& pLangConfigData[j].configValueType != STRING_TYPE
				&& pLangConfigData[j].configValueType != ONLY_TEXT_DLG_TYPE
				&& pConfigData->chdCount != 0)
			{
				pConfigData->chdConfigData = PT_Malloc(pConfigData->chdCount * sizeof(GBConfigData));
				memcpy(pConfigData->chdConfigData, &pLangConfigData[j + 1], pConfigData->chdCount * sizeof(GBConfigData));
				j += pConfigData->chdCount; 
				g_cfg_tails[i].configCount -= pConfigData->chdCount;
			}
		}
		PT_Free(pLangConfigData);
		pLangConfigData = NULL;
	}

	PT_FS_Close(file);

	SetConfigFile();

	return 1;
}
/*!
 * \brief 通过产品id加载配置文件
 * \param (in) productID 产品id
 * \return  
 * \note 
 * \example
 * \author ZhaoKun
 * \date
 */
GBINT LoadConfigFile(GBINT productID)
{
	GBWCHAR filePath[128] = {0};

	if (g_curProductID == productID)
	{
		return 1;
	}

	g_curProductID = productID;

#if __MTK__ > 0
	PT_wsprintf(filePath, "%w%w.config", GB_SYSTEM_DIR, GetProductStringW(productID));
#else
	PT_wsprintf(filePath, L"%s%s.config", GB_SYSTEM_DIR, GetProductStringW(productID));
#endif

	return LoadConfigFileForPath(filePath);
}

/*!
 * \brief 通过配置文件路径保存配置设置
 * \param (in) filePath 配置文件路径
 * \return  
 * \note 
 * \example
 * \author ZhaoKun
 * \date
 */
GBINT SaveConfigFileForPath(GBPWCHAR filePath)
{
	GBINT length = 0;
	GBINT i = 0;
	GBINT j = 0;
	GBINT lang, subLang;
	GBINT offset;
	DataOffset *pDataOffset = NULL;
	GBConfigData* pConfigData = NULL;

	PT_FILE_HANDLE file;
	
	file = PT_FS_Open(filePath, PT_FS_READ_WRITE);
	if(!file)
		return -1;
	
	for (i = 0; i < g_cfgTailCount; i++)
	{
		lang = g_cfg_tails[i].lang;
		subLang = g_cfg_tails[i].subLang;
		
		for (j = 0; j < g_cfg_header.langCount; j++)
		{
			if (lang == g_cfg_header.commonConfig[j].lang 
				&& subLang == g_cfg_header.commonConfig[i].subLang)
			{
				pDataOffset = &g_cfg_header.commonConfig[j];
				break;
			}
		}
		if (!pDataOffset)
		{
			continue;
		}
		offset = pDataOffset->offset + sizeof(CFG_HEADER);

		PT_FS_Seek(file, offset, SEEK_SET);

		for (j = 0; j < g_cfg_tails[i].configCount; j++)
		{
			pConfigData = &g_cfg_tails[i].configData[j];
			PT_FS_Write(file, pConfigData, sizeof(GBConfigData), &length);
			offset += sizeof(GBConfigData);
			if (pConfigData->chdCount)
			{
				PT_FS_Write(file, pConfigData->chdConfigData, sizeof(GBConfigData) * pConfigData->chdCount, &length);			
				offset += (sizeof(GBConfigData) * pConfigData->chdCount);
			}			
		}
	}
	
	PT_FS_Close(file);

	return 1;
}
/*!
 * \brief 取配置文件路径
 * \return  
 * \note 
 * \example
 * \author ZhaoKun
 * \date
 */
GBPWCHAR GetConfigFilePath(void)
{
	return g_configFilePath;
}
/*!
 * \brief 通过产品id保存配置设置到文件
 * \param (in) productID 产品ID	
 * \return  
 * \note 
 * \example
 * \author ZhaoKun
 * \date
 */
GBINT SaveConfigFile(GBINT productID)
{
 	GBWCHAR filePath[128] = {0}; 

#if __MTK__ > 0
	PT_wsprintf(filePath, "%w%w.config",GB_SYSTEM_DIR, GetProductStringW(productID));
#else
	PT_wsprintf(filePath, L"%s%s.config",GB_SYSTEM_DIR, GetProductStringW(productID));
#endif
	
	return SaveConfigFileForPath(filePath);
}
/*!
 * \brief 根据菜单字符串id设置缓存的配置数据
 * \param (in) menuStrId 菜单字符串id	
 * \param (in) pValue 配置值
 * \param (in) valueCount STRING_TYPE时可用，pValue长度
 * \return  
 * \note 
 * \example
 * \author ZhaoKun
 * \date
 */
GBINT SetConfigValueForMenuStrID(GBINT menuStrId, void* pValue, GBINT valueCount)
{
	GBINT i = 0;
	GBINT j = 0;
	GBINT nChdCount;
	GBConfigData* pConfigData = NULL;
	GBConfigData* pChdConfigData = NULL;
	GBWCHAR szBuf[CONFIG_DATA_VALUE_MAX_SIZE] = {0};
	GBINT count;
	GBINT configCount;

	if (pValue == NULL)
	{
		return -1;
	}
	for (i = 0; i < g_cfgTailCount; i++)
	{
		configCount = g_cfg_tails[i].configCount;

		for (j = 0; j < configCount; j++)
		{
			pConfigData = &g_cfg_tails[i].configData[j];

			for (nChdCount = 0; nChdCount < pConfigData->chdCount; nChdCount++)
			{
				pChdConfigData = &pConfigData->chdConfigData[nChdCount];

				if (pChdConfigData->menuStrId != menuStrId - GBIME_BASE)
				{
					continue;
				}
				switch (pChdConfigData->configValueType)
				{
				case INT_TYPE:
					gbitow(*(GBUINT8 *)pValue, szBuf, 10);
					PT_wcscpy(pChdConfigData->szValue, szBuf);
					break;
				case STRING_TYPE:
					count = CONFIG_DATA_VALUE_MAX_SIZE > valueCount ? valueCount : CONFIG_DATA_VALUE_MAX_SIZE;
					PT_wcsncpy(pChdConfigData->szValue, (GBPWCHAR)pValue, count);
					pChdConfigData->szValue[count] = 0;
					break;
				default:
					break;
				}
				return 1;			
			}
			if (!pConfigData->chdCount 
				&& (pConfigData->configValueType == INT_TYPE
				|| pConfigData->configValueType == STRING_TYPE))
			{
				if (pConfigData->menuStrId != menuStrId - GBIME_BASE)
				{
					continue;
				}

				switch (pConfigData->configValueType)
				{
				case INT_TYPE:
					gbitow(*(GBUINT8 *)pValue, szBuf, 10);
					PT_wcscpy(pConfigData->szValue, szBuf);
					break;
				case STRING_TYPE:
					count = CONFIG_DATA_VALUE_MAX_SIZE > valueCount ? valueCount : CONFIG_DATA_VALUE_MAX_SIZE;
					PT_wcsncpy(pConfigData->szValue, (GBPWCHAR)pValue, count);
					pConfigData->szValue[count] = 0;
					break;
				default:
					break;
				}
			}
			
			
		}
	}
	return 1;
}
/*!
 * \brief 根据语言，子语言，配置id和配置类型设置缓存的配置数据
 * \param (in) lang 语言	
 * \param (in) subLang 子语言
 * \param (in) configID 配置id
 * \param (in) configType 配置类型
 * \param (in) pValue 配置数据
 * \param (in) valueCount STRING_TYPE时可用，pValue长度
 * \return  
 * \note 
 * \example
 * \author ZhaoKun
 * \date
 */
GBINT SetConfigValue(GBINT lang, GBINT subLang, GBINT configID, GBINT configType, void* pValue, GBINT valueCount)
{
	GBINT i = 0;
	GBINT j = 0;
	GBINT nChdCount = 0;
	GBConfigData* pConfigData = NULL;
	GBConfigData* pChdConfigData = NULL;
	GBINT configCount;
	GBWCHAR szBuf[CONFIG_DATA_VALUE_MAX_SIZE] = {0};
	GBINT count;

	if (pValue == NULL)
	{
		return -1;
	}
	for (i = 0; i < g_cfgTailCount; i++)
	{
		if (lang != g_cfg_tails[i].lang || subLang != g_cfg_tails[i].subLang)
		{
			continue;
		}
		pConfigData = g_cfg_tails[i].configData;
		configCount = g_cfg_tails[i].configCount;

		for (j = 0; j < configCount; j++)
		{
			pChdConfigData = pConfigData[j].chdConfigData;
			for (nChdCount = 0; nChdCount < pConfigData[j].chdCount; nChdCount++)
			{
				if (configID != pChdConfigData[nChdCount].configID || configType != pChdConfigData[nChdCount].configTypeID)
				{
					continue;
				}
				switch (pChdConfigData[nChdCount].configValueType)
				{
				case INT_TYPE:
					gbitow(*(GBUINT8 *)pValue, szBuf, 10);
					PT_wcscpy(pChdConfigData[nChdCount].szValue, szBuf);
					break;
				case STRING_TYPE:
					count = CONFIG_DATA_VALUE_MAX_SIZE > valueCount ? valueCount : CONFIG_DATA_VALUE_MAX_SIZE;
					PT_wcsncpy(pChdConfigData[nChdCount].szValue, (GBPWCHAR)pValue, count);
					pChdConfigData[nChdCount].szValue[count] = 0;
					break;
				default:
					break;
				}
				return 1;
			}			
		}
		break;
	}

	return 1;
}
/*!
 * \brief 根据当前选择的字符串id，获取子菜单的字符串id、值、个数
 * \param (in) curStrID 当前菜单字符串ID	
 * \param (out) pChdMenuStrID 子菜单id
 * \param (out) pChdMenuValue 子菜单值
 * \param (out) pChdMenuCount 子菜单id个数
 * \return  返回菜单类型，详见ConfigValueTypeEnum
 * \note 
 * \example
 * \author ZhaoKun
 * \date
 */
GBINT GetConfigStrID(GBUINT16 curStrID, GBUINT16* pChdMenuStrID, void* pChdMenuValue, GBUINT16 *pChdMenuCount)
{
	GBINT menuType = 0;
	GBINT langIndex = 0;
	GBINT chdCount;
	pGBConfigData pConfigData;
	GBUINT8 value;
	GBWCHAR *pStringMenuValue = (GBWCHAR *)pChdMenuValue;
	GBUINT8* pIntMenuValue = (GBUINT8*)pChdMenuValue;

	if (!pChdMenuCount || !pChdMenuStrID || !pChdMenuValue)
	{
		return -1;
	}
	*pChdMenuCount = 0;
	if (curStrID == 0)
	{
		for (langIndex = 0; langIndex < g_cfgTailCount; langIndex++)
		{
			pChdMenuStrID[*pChdMenuCount] = g_cfg_tails[langIndex].menuStrId + GBIME_BASE;
			(*pChdMenuCount)++;
		}
		return LIST_TYPE;
	}

	for (langIndex = 0; langIndex < g_cfgTailCount; langIndex++)
	{
		
		if (curStrID - GBIME_BASE != g_cfg_tails[langIndex].menuStrId)
		{
			pConfigData = g_cfg_tails[langIndex].configData;

			for (chdCount = 0; chdCount < g_cfg_tails[langIndex].configCount; chdCount++)
			{
				int j = 0;
				pGBConfigData pChdConfigData;
				if (curStrID - GBIME_BASE != pConfigData[chdCount].menuStrId)
				{
					continue;
				}
				pChdConfigData = pConfigData[chdCount].chdConfigData;
				menuType = pConfigData[chdCount].configValueType;

				for (j = 0; j < pConfigData[chdCount].chdCount; j++)
				{
					pChdMenuStrID[*pChdMenuCount] = pChdConfigData[j].menuStrId + GBIME_BASE;
					switch (pChdConfigData[j].configValueType)
					{
					case INT_TYPE:
						value = gbwtoi(pChdConfigData[j].szValue);
						pIntMenuValue[*pChdMenuCount] = value;
						break;
					case STRING_TYPE:
						PT_wcscpy(pStringMenuValue, pChdConfigData[j].szValue);
						pStringMenuValue += CONFIG_DATA_VALUE_MAX_SIZE;
						break;
					default:
						break;
					}
					(*pChdMenuCount)++;
				}
				break;
			}
			continue;
		}
		//找到ID
		menuType = g_cfg_tails[langIndex].menuType;
		pConfigData = g_cfg_tails[langIndex].configData;

		for (chdCount = 0; chdCount < g_cfg_tails[langIndex].configCount; chdCount++)
		{
			pChdMenuStrID[*pChdMenuCount] = pConfigData[chdCount].menuStrId + GBIME_BASE;
			switch (pConfigData[chdCount].configValueType)
			{
			case INT_TYPE:
				value = gbwtoi(pConfigData[chdCount].szValue);
				pIntMenuValue[*pChdMenuCount] = value;
				break;
			case STRING_TYPE:
				PT_wcscpy(pStringMenuValue, pConfigData[chdCount].szValue);
				pStringMenuValue += CONFIG_DATA_VALUE_MAX_SIZE;
				break;
			default:
				break;
			}
			(*pChdMenuCount)++;
		}
		break;
	}

	return menuType;
}
/*!
 * \brief 获取当前菜单下所有配置的菜单id和状态
 * \param (in) lang 语言	
 * \param (in) subLang 子语言
 * \param (out) pMenuStrID 菜单字符串id
 * \param (out) pMenuValue 菜单状态
 * \param (out) pMenuCount 菜单个数
 * \return  
 * \note buf必须 < MAX_SUB_MENUS
 * \example
 * \author ZhaoKun
 * \date
 */
GBINT GetConfigMenuInfo(GBINT lang, GBINT subLang, GBUINT16* pMenuStrID, void* pMenuValue, GBUINT16 *pMenuCount)
{
	GBINT i = 0;
	GBINT j = 0;
	GBConfigData* pConfigData = NULL;
	GBINT configCount;
	GBUINT8 value;
	GBWCHAR *pStringMenuValue = (GBWCHAR *)pMenuValue;
	GBUINT8* pIntMenuValue = (GBUINT8*)pMenuValue;

	if (!pMenuCount || !pMenuStrID || !pMenuValue)
	{
		return -1;
	}

	*pMenuCount = 0;

	for (i = 0; i < g_cfgTailCount; i++)
	{
		if (lang != g_cfg_tails[i].lang || subLang != g_cfg_tails[i].subLang)
		{
			continue;
		}
		
		configCount = g_cfg_tails[i].configCount;
		pConfigData = g_cfg_tails[i].configData;

		for (j = 0; j < configCount; j++)
		{
			//如果大于菜单最大值则退出
			if (j >= MAX_SUB_MENUS)
			{
				break;
			}
			if (!pConfigData[j].enable)
			{
				//不需要显示该配置选项
				continue;
			}
			
			pMenuStrID[*pMenuCount] = pConfigData[j].menuStrId + GBIME_BASE;

			switch (pConfigData[j].configValueType)
			{
			case INT_TYPE:
				value = gbwtoi(pConfigData[j].szValue);
				pIntMenuValue[*pMenuCount] = value;
				break;
			case STRING_TYPE:
				PT_wcscpy(pStringMenuValue, pConfigData[j].szValue);
				pStringMenuValue += CONFIG_DATA_VALUE_MAX_SIZE;
				break;
			default:
				break;
			}

			(*pMenuCount)++;
		}
		break;
	}
	return 1;
}

GBPACHAR GetProductString(GBINT productID)
{
	GBPACHAR name;

	switch (productID)
	{
	case PRODUCE_NUMPAD_1:
		name = STR(PRODUCE_NUMPAD_1);
		break;
	case PRODUCE_NUMPAD_2:
		name = STR(PRODUCE_NUMPAD_2);
		break;
	case PRODUCE_NUMPAD_3:
		name = STR(PRODUCE_NUMPAD_3);
		break;
	case PRODUCE_NUMPAD_4:
		name = STR(PRODUCE_NUMPAD_4);
		break;
	case PRODUCE_QWERTY_1:
		name = STR(PRODUCE_QWERTY_1);
		break;
	case PRODUCE_QWERTY_2:
		name = STR(PRODUCE_QWERTY_2);
		break;
	case PRODUCE_QWERTY_3:
		name = STR(PRODUCE_QWERTY_3);
		break;
	case PRODUCE_QWERTY_4:
		name = STR(PRODUCE_QWERTY_4);
		break;
	case PRODUCE_HALF_QWERTY_1:
		name = STR(PRODUCE_HALF_QWERTY_1);
		break;
	case PRODUCE_HALF_QWERTY_2:
		name = STR(PRODUCE_HALF_QWERTY_2);
		break;
	case PRODUCE_HALF_QWERTY_3:
		name = STR(PRODUCE_HALF_QWERTY_3);
		break;
	case PRODUCE_HALF_QWERTY_4:
		name = STR(PRODUCE_HALF_QWERTY_4);
		break;
	case PRODUCE_NUMPAD_5:
		name = STR(PRODUCE_NUMPAD_5);
		break;
	case PRODUCE_NUMPAD_6:
		name = STR(PRODUCE_NUMPAD_6);
		break;
	case PRODUCE_NUMPAD_7:
		name = STR(PRODUCE_NUMPAD_7);
		break;
	case PRODUCE_NUMPAD_8:
		name = STR(PRODUCE_NUMPAD_8);
		break;
	case PRODUCE_QWERTY_5:
		name = STR(PRODUCE_QWERTY_5);
		break;
	case PRODUCE_QWERTY_6:
		name = STR(PRODUCE_QWERTY_6);
		break;
	case PRODUCE_QWERTY_7:
		name = STR(PRODUCE_QWERTY_7);
		break;
	case PRODUCE_QWERTY_8:
		name = STR(PRODUCE_QWERTY_8);
		break;
	case PRODUCE_HALF_QWERTY_5:
		name = STR(PRODUCE_HALF_QWERTY_5);
		break;
	case PRODUCE_HALF_QWERTY_6:
		name = STR(PRODUCE_HALF_QWERTY_6);
		break;
	case PRODUCE_HALF_QWERTY_7:
		name = STR(PRODUCE_HALF_QWERTY_7);
		break;
	case PRODUCE_HALF_QWERTY_8:
		name = STR(PRODUCE_HALF_QWERTY_8);
		break;
	case PRODUCE_NUMPAD_9:
		name = STR(PRODUCE_NUMPAD_9);
		break;
	case PRODUCE_NUMPAD_10:
		name = STR(PRODUCE_NUMPAD_10);
		break;
	case PRODUCE_NUMPAD_11:
		name = STR(PRODUCE_NUMPAD_11);
		break;
	case PRODUCE_QWERTY_9:
		name = STR(PRODUCE_QWERTY_9);
		break;
	case PRODUCE_EXPLICIT_1:
		name = STR(PRODUCE_EXPLICIT_1);
		break;
	case PRODUCE_EXPLICIT_2:
		name = STR(PRODUCE_EXPLICIT_2);
		break;
	case PRODUCE_EXPLICIT_3:
		name = STR(PRODUCE_EXPLICIT_3);
		break;
	case PRODUCE_EXPLICIT_4:
		name = STR(PRODUCE_EXPLICIT_4);
		break;
	case PRODUCE_EXPLICIT_5:
		name  = STR(PRODUCE_EXPLICIT_5);
		break;
	default:
		name = NULL;
		break;
	}

	return name;
}

GBPWCHAR GetProductStringW(GBINT productID)
{
	GBPWCHAR name;

	switch (productID)
	{
	case PRODUCE_NUMPAD_1:
		name = STR_W(PRODUCE_NUMPAD_1);
		break;
	case PRODUCE_NUMPAD_2:
		name = STR_W(PRODUCE_NUMPAD_2);
		break;
	case PRODUCE_NUMPAD_3:
		name = STR_W(PRODUCE_NUMPAD_3);
		break;
	case PRODUCE_NUMPAD_4:
		name = STR_W(PRODUCE_NUMPAD_4);
		break;
	case PRODUCE_QWERTY_1:
		name = STR_W(PRODUCE_QWERTY_1);
		break;
	case PRODUCE_QWERTY_2:
		name = STR_W(PRODUCE_QWERTY_2);
		break;
	case PRODUCE_QWERTY_3:
		name = STR_W(PRODUCE_QWERTY_3);
		break;
	case PRODUCE_QWERTY_4:
		name = STR_W(PRODUCE_QWERTY_4);
		break;
	case PRODUCE_HALF_QWERTY_1:
		name = STR_W(PRODUCE_HALF_QWERTY_1);
		break;
	case PRODUCE_HALF_QWERTY_2:
		name = STR_W(PRODUCE_HALF_QWERTY_2);
		break;
	case PRODUCE_HALF_QWERTY_3:
		name = STR_W(PRODUCE_HALF_QWERTY_3);
		break;
	case PRODUCE_HALF_QWERTY_4:
		name = STR_W(PRODUCE_HALF_QWERTY_4);
		break;
	case PRODUCE_NUMPAD_5:
		name = STR_W(PRODUCE_NUMPAD_5);
		break;
	case PRODUCE_NUMPAD_6:
		name = STR_W(PRODUCE_NUMPAD_6);
		break;
	case PRODUCE_NUMPAD_7:
		name = STR_W(PRODUCE_NUMPAD_7);
		break;
	case PRODUCE_NUMPAD_8:
		name = STR_W(PRODUCE_NUMPAD_8);
		break;
	case PRODUCE_QWERTY_5:
		name = STR_W(PRODUCE_QWERTY_5);
		break;
	case PRODUCE_QWERTY_6:
		name = STR_W(PRODUCE_QWERTY_6);
		break;
	case PRODUCE_QWERTY_7:
		name = STR_W(PRODUCE_QWERTY_7);
		break;
	case PRODUCE_QWERTY_8:
		name = STR_W(PRODUCE_QWERTY_8);
		break;
	case PRODUCE_HALF_QWERTY_5:
		name = STR_W(PRODUCE_HALF_QWERTY_5);
		break;
	case PRODUCE_HALF_QWERTY_6:
		name = STR_W(PRODUCE_HALF_QWERTY_6);
		break;
	case PRODUCE_HALF_QWERTY_7:
		name = STR_W(PRODUCE_HALF_QWERTY_7);
		break;
	case PRODUCE_HALF_QWERTY_8:
		name = STR_W(PRODUCE_HALF_QWERTY_8);
		break;
	case PRODUCE_NUMPAD_9:
		name = STR_W(PRODUCE_NUMPAD_9);
		break;
	case PRODUCE_NUMPAD_10:
		name = STR_W(PRODUCE_NUMPAD_10);
		break;
	case PRODUCE_NUMPAD_11:
		name = STR_W(PRODUCE_NUMPAD_11);
		break;
	case PRODUCE_QWERTY_9:
		name = STR_W(PRODUCE_QWERTY_9);
		break;
	case PRODUCE_EXPLICIT_1:
		name = STR_W(PRODUCE_EXPLICIT_1);
		break;
	case PRODUCE_EXPLICIT_2:
		name = STR_W(PRODUCE_EXPLICIT_2);
		break;
	case PRODUCE_EXPLICIT_3:
		name = STR_W(PRODUCE_EXPLICIT_3);
		break;
	case PRODUCE_EXPLICIT_4:
		name = STR_W(PRODUCE_EXPLICIT_4);
		break;
	case PRODUCE_EXPLICIT_5:
		name = STR_W(PRODUCE_EXPLICIT_5);
		break;
	default:
		name = NULL;
		break;
	}

	return name;
}

GBINT gbwtoi(GBLPCWCHAR str)
{
	GBINT digval = 0;
	GBINT number = 0;
	GBLPCWCHAR p;
	GBWCHAR c;
	GBINT isNegative = 0;
	
	PT_Assert(str != NULL);
	
	p = str;
	
	c = *p++;
	
	if (c == L'-')
	{
		isNegative = 1;
		c = *p++;
	}
	else if (c == L'+')
		c = *p++;
	
	for (;;)
	{
		if (c == 0)
		{
			if (isNegative)
			{
				number = (int)(-number);
			}
			break;
		}
		if (c >= L'0' && c <= L'9')
		{
			digval = c - L'0';
			number = number * 10 + digval;
			c = *p++;
			continue;
		}
		number = 0;
		break;
	}
	return number;	
}
GBPWCHAR gbitow(GBINT number, GBLPWCHAR str, GBINT radix )
{
	GBINT is_neg = 0;
	GBPWCHAR p;
	GBPWCHAR firstdig;
	GBINT temp;
	GBINT digval = 0;
	
	PT_Assert(str != NULL);
	
	if (radix == 10 && number < 0)
	{
		is_neg = 1; 
	}
	
	p = str;
	firstdig = p;
	
	if (is_neg)
	{
		*p++ = L'-';
		number = (int)(-number);
	}
	
	do
	{
		digval = (int) (number % radix);
		number /= radix;       /* get next digit */
		
		*p++ = digval + L'0';
	}while(number > 0);
	
	*p-- = L'\0';
	
	do {
		temp = *p;
		*p = *firstdig;
		*firstdig = temp;   /* swap *p and *firstdig */
		--p;
		++firstdig;         /* advance to next two digits */
	} while (firstdig < p); /* repeat until halfway */
	
	return str;
}
/*!
 * \brief 判断字符串ID在当前菜单和子菜单中是否存在
 * \param (in) pConfigData	
 * \param (in) curStrID
 * \param (out) pMenuType 菜单类型
 * \return  存在返回1，不存在返回0
 * \example
 * \author ZhaoKun
 * \date
 */
GBINT IsHaveCurStrID(pGBConfigData pConfigData, GBUINT16 curStrID, GBINT* pMenuType)
{
	GBINT i = 0;
	if (!pConfigData)
	{
		return 0;
	}

	if (curStrID - GBIME_BASE == pConfigData->menuStrId)
	{
		if (pMenuType)
		{
			*pMenuType = pConfigData->configValueType;
		}
		return 1;
	}

	for (i = 0; i < pConfigData->chdCount; i++)
	{
		if(IsHaveCurStrID(&pConfigData->chdConfigData[i], curStrID, pMenuType))
		{
			return 1;
		}
	}
	return 0;
}
/*!
 * \brief 通过字符串ID获取当前菜单的类型
 * \param curStrID
 * \return	参见ConfigValueTypeEnum
 * \note 
 * \author ZhaoKun
 * \date 2010年9月27日
 */
GBINT GetCurMenuTypeToStrID(GBUINT16 curStrID)
{
	GBINT menuType = 0;
	GBINT langIndex = 0;
	GBINT i = 0;
	pGBConfigData pConfigData;

	for (langIndex = 0; langIndex < g_cfgTailCount; langIndex++)
	{
		if (curStrID - GBIME_BASE != g_cfg_tails[langIndex].menuStrId)
		{
			pConfigData = g_cfg_tails[langIndex].configData;
			for (i = 0; i < g_cfg_tails[langIndex].configCount; i++)
			{
				if (IsHaveCurStrID(&pConfigData[i], curStrID, &menuType))
				{
					return menuType;
				}
				continue;
			}
			continue;
		}
		menuType = g_cfg_tails[langIndex].menuType;
		break;
	}
	return menuType;
}

////////读生成的配置文件信息//////////////////////////////////////////////////////////////////
#if TEST > 0
GBINT ReadConfigFile(GBINT productID, pCFG_TAILS pCfg_tails, GBINT count)
{
	GBINT i = 0;
	GBINT j = 0;
	pGBConfigData pConfigData;
	GBACHAR filePath[128] = {0};
	void * pBuf = NULL;
	GBPACHAR pTemp = NULL;
	GBINT offset = 0;

	PT_FILE_HANDLE file;
	pGBConfigData pLangConfigData;
	GBINT length;
	GBINT langSize = 0;
	GBINT configCount = 0;
	GBINT menuCount = 0;


	sprintf(filePath, "..\\%s.config", GetProductString(productID));

	file = fopen(filePath, "r");
	if(!file)
		return -1;

	fread(&g_cfg_header, sizeof(CFG_HEADER), 1, file);

	if(strcmp(g_cfg_header.cfg, SIGNATURE))
	{
		printf("\nnot config file!\n");
		return -2;
	}

	if(1)
	{
		printf("\n=============file header================\n");
		printf("CFG_HEADER::cfg\t\t%s\n", g_cfg_header.cfg);
		printf("CFG_HEADER::hVersion\t\t%d\n",g_cfg_header.hVersion);
		printf("CFG_HEADER::version1\t\t%d\n",g_cfg_header.version1);
		printf("CFG_HEADER::version2\t\t%d\n",g_cfg_header.version2);
		printf("CFG_HEADER::version3\t\t%d\n",g_cfg_header.version3);
		printf("CFG_HEADER::productID\t\t%d\n",g_cfg_header.productID);
		printf("CFG_HEADER::langCount\t\t%d\n",g_cfg_header.langCount);
		printf("CFG_HEADER::totalSize\t\t%d\n",g_cfg_header.totalSize);
		for (i = 0; i < g_cfg_header.langCount; i++)
		{
			printf("CFG_HEADER::commonConfig[%d]\t\t\n", i);
			printf("                           offset:\t\t%d\n", g_cfg_header.commonConfig[i].offset);
			printf("                           menuStrID:\t\t%d\n",  g_cfg_header.commonConfig[i].menuStrID);
			printf("                           lang:\t\t%d\n",  g_cfg_header.commonConfig[i].lang);
			printf("                           subLang:\t\t%d\n",  g_cfg_header.commonConfig[i].subLang);
			printf("                           capacity:\t\t%d\n",  g_cfg_header.commonConfig[i].capacity);
			printf("                           itemNumber:\t\t%d\n", g_cfg_header.commonConfig[i].itemNumber);
		}
		printf("CFG_HEADER::ReservedData\t\t%s", g_cfg_header.ReservedData);
	}

	for (i = 0; i < g_cfg_header.langCount; i++)
	{
		offset = sizeof(CFG_HEADER) + g_cfg_header.commonConfig[i].offset;

		//fseek(file, offset, SEEK_SET);
		PT_FS_Seek(file, offset, SEEK_SET);		

		g_cfg_tails[i].lang = g_cfg_header.commonConfig[i].lang;
		g_cfg_tails[i].subLang = g_cfg_header.commonConfig[i].subLang;
		g_cfg_tails[i].configCount = g_cfg_header.commonConfig[i].itemNumber;
		g_cfg_tails[i].menuStrId = g_cfg_header.commonConfig[i].menuStrID;

		langSize = g_cfg_header.commonConfig[i].itemNumber * sizeof(GBConfigData);

		pLangConfigData = (pGBConfigData)PT_Malloc(langSize);

		PT_FS_Read(file, pLangConfigData, langSize, &length);

		for (j = 0, menuCount = 0; j < g_cfg_header.commonConfig[i].itemNumber; menuCount++, j++)
		{
			pConfigData = &g_cfg_tails[i].configData[menuCount];
			memcpy(pConfigData, &pLangConfigData[j], sizeof(GBConfigData));
			if (pLangConfigData[j].configValueType != INT_TYPE
				&& pLangConfigData[j].configValueType != STRING_TYPE)
			{
				pConfigData->chdConfigData = PT_Malloc(pConfigData->chdCount * sizeof(GBConfigData));
				memcpy(pConfigData->chdConfigData, &pLangConfigData[j + 1], pConfigData->chdCount * sizeof(GBConfigData));
				j += pConfigData->chdCount; 
				g_cfg_tails[i].configCount -= pConfigData->chdCount;
			}
			//pConfigData = &g_cfg_tails[i].configData[j];
		}
		PT_Free(pLangConfigData);
		pLangConfigData = NULL;
	}

	if(1)
	{
		pGBConfigData pMenuConfig;
		GBINT count;
		printf("\n=============file tails================\n");
		for (i = 0; i < g_cfg_header.langCount; i++)
		{
			printf("\t【%d】：%d %d\n", i + 1, g_cfg_tails[i].lang, g_cfg_tails[i].menuStrId);

			pMenuConfig = g_cfg_tails[i].configData;
			for (j = 0; j < g_cfg_tails[i].configCount; j++)
			{
				if (pMenuConfig[j].configValueType != INT_TYPE
					&& pMenuConfig[j].configValueType != STRING_TYPE)
				{
					printf("\t\t【%d】：%d \n", j + 1, pMenuConfig[j].menuStrId);
					for (count = 0; count < pMenuConfig[j].chdCount; count++)
					{
						printf("\t\t\t【X】：%d",pMenuConfig[j].chdConfigData[count].menuStrId);
						wprintf(L"===%s \n", pMenuConfig[j].chdConfigData[count].szValue);
					}
				}
				else
				{
					printf("\t\t【X】：%d",pMenuConfig[j].menuStrId);
					wprintf(L"===%s \n", pMenuConfig[j].szValue);
				}
			}
		}
	}

	fclose(file);
	return 1;
}
#endif