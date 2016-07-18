/***************************************************************************
* File Name    :  GuobiIMEngine.c
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
#include "CGBEngine.h"
#include "GB_Global.h"
#include "PlatformAdapter.h"
#include "PlatformTools.h"
#include "CHWEngine.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
// 本地函数实现
///////////////////////////////////////////////////////////////////////////////////////////////////
/*!
 * \brief 本地封装的按键事件处理函数(调用GBMMI按键处理函数之前需要做状态检查)
 * \param pEngine 
 * \param evt 
 * \param param 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-22 10:57:08
 */
static GBU32 _input_key_event(PCGBEngine pEngine, GBINT evt, GBU32 param)
{
	PT_Assert( GBEngine_IsReadyInput(pEngine) );
	return GBMMIHandleKeyEvent(&pEngine->g_gbis, evt, param);
}

/*!
 * \brief 引擎全局变量初始化(将变量设置为初始状态值)
 * \param pEngine 
 * \return 
 */
static void GBEngine_GlobalVariableInit(PCGBEngine pEngine)
{
	memset( &(pEngine->g_ldbs), 0x0,sizeof(pEngine->g_ldbs) );
	memset( &(pEngine->g_gbis), 0x0, sizeof(pEngine->g_gbis) );
	memset( &(pEngine->g_aux), 0x0, sizeof(pEngine->g_aux) );

	pEngine->g_nDataCount = 0;
	pEngine->engine_opened = 0;
	pEngine->input_mode_is_valid = 0;
	pEngine->master_udb_loaded = 0;
	pEngine->slave_udb_loaded = 0;
	pEngine->master_udb_id = 0;
	pEngine->slave_udb_id = 0;
//	pEngine->isSwitchNextInputMode = GBFalse;
}


/*!
 * \brief 设置log日志
 * \param pOEMData 用于将日志保存到文件
 * \param format 日志格式
 * \param p1 
 * \param p2 
 * \param p3 
 * \param p4 
 * \return 
 * \note 
 * \example
 * \author fanbo
 * \date 2010-06-08
 */
static GBINT setLogRecorder(GBLPVOID pOEMData, GBPCACHAR format, GBU32 p1, GBU32 p2, GBU32 p3, GBU32 p4)
{
#ifdef _DEBUG
	//fprintf(stderr, format, p1, p2, p3, p4);
#endif
	return 1;
}

/*!
 * \brief 设置国笔引擎页面配置
 * \param pEngine
 * \return see GBRETURN
 * \note 
 * \author weizhiping
 * \date 2010年4月21日
 */
static GBRETURN GBEngine_SetPageOption(PCGBEngine pEngine)
{
	GBGlobalPageOption pageOption;
	GBRETURN ret;

	PT_Assert( GBEngine_IsOpened(pEngine) == 0 );

	// 获取页面配置，修改并设置到引擎内部
	ret = GBGetPageOption(&pEngine->g_gbis, &pageOption);
	pageOption.fpGetWStringWidth = PT_GetStrWidth;

	// 中文
	pageOption.mChnCandPageOption.nMaxCandCountPerPage = GBIME_DEF_CHN_MAX_CAND_WORD;
	pageOption.mChnCandPageOption.nTotalWidthInPixel = GBIME_CAND_AREA_WIDTH;
	//pageOption.mChnCandPageOption.nCharWidthInPixel = 18;
	pageOption.mChnCandPageOption.nSplitWidthInPixel = GBIME_CANDIDATE_SPACING;

	// Ambiguous中文音节
	pageOption.mSyllablePageOption.nMaxCandCountPerPage = GBIME_DEF_SYLLABLE_MAX_CAND_WORD;
	pageOption.mSyllablePageOption.nTotalWidthInPixel = GBIME_CAND_AREA_WIDTH;
	pageOption.mSyllablePageOption.nSplitWidthInPixel = GBIME_SYLLABLE_SPACING;

	// 英文
	pageOption.mPageOption.nMaxCandCountPerPage = GBIME_DEF_ENGINSH_MAX_CAND_WORD;
	pageOption.mPageOption.nTotalWidthInPixel = GBIME_CAND_AREA_WIDTH;
	pageOption.mPageOption.nSplitWidthInPixel = GBIME_CANDIDATE_SPACING;
	pageOption.mPageOption.nCharWidthInPixel = 0;

	ret = GBSetPageOption(&pEngine->g_gbis, &pageOption);
	return ret;
}

/*!
 * \brief 设置国笔引擎页面配置的显示页面宽度
 * \param pEngine
 * \return see GBRETURN
 * \note 
 * \author ZhaoKun
 * \date 2010年10月20日
 */
GBRETURN GBEngine_SetPageToalWidthOption(PCGBEngine pEngine, GBU16 nTotalWidth)
{
	GBGlobalPageOption pageOption;
	GBRETURN ret;

	if( GBEngine_IsOpened(pEngine) == 0 )
	{
		return GBFalse;
	}
	// 获取页面配置，修改并设置到引擎内部
	ret = GBGetPageOption(&pEngine->g_gbis, &pageOption);
	// 中文
	pageOption.mChnCandPageOption.nTotalWidthInPixel = nTotalWidth;
	// Ambiguous中文音节
	pageOption.mSyllablePageOption.nTotalWidthInPixel = nTotalWidth;
	// 英文
	pageOption.mPageOption.nTotalWidthInPixel = nTotalWidth;
	ret = GBSetPageOption(&pEngine->g_gbis, &pageOption);
	return ret;
}

static GBBOOL GBEngine_LoadData(PCGBEngine pEngine, GBACHAR *pDataOrPath)
{
	GBUINT32 dataSize = 0;
	char header_data[256];

#ifndef GBIME_CFG_STATIC_DB
	PT_FILE_HANDLE fp = NULL;

	fp = PT_FS_Open(pDataOrPath, "rb");
	if(!fp)
	{
		return GBFalse;
	}
	if(PT_FS_Read(header_data, sizeof(header_data), 1, fp) == 1)
	{
		if(GBGetDataLanguage(header_data, &pEngine->g_ldbs[pEngine->g_nDataCount].nLang, 
			&pEngine->g_ldbs[pEngine->g_nDataCount].nSubLang, &dataSize) == GBOK)
		{
			pEngine->g_ldbs[pEngine->g_nDataCount].pData = (GBLPCVOID)PT_Malloc(dataSize);
			PT_FS_Seek(fp, 0, SEEK_SET);
			if(PT_FS_Read(fp, (GBU8*)pEngine->g_ldbs[pEngine->g_nDataCount].pData, 1, dataSize) != dataSize)
			{
				return GBFalse;
			}
			PT_FS_Close(fp);
			pEngine->g_nDataCount++;
			return GBTrue;
		}
	}
	PT_FS_Close(fp);
#else
	memcpy(header_data, pDataOrPath, sizeof(header_data));
	if(GBGetDataLanguage(header_data, &pEngine->g_ldbs[pEngine->g_nDataCount].nLang, 
		&pEngine->g_ldbs[pEngine->g_nDataCount].nSubLang, &dataSize) == GBOK)
	{			
		pEngine->g_ldbs[pEngine->g_nDataCount++].pData = pDataOrPath;
		return GBTrue;
	}	
#endif
	return GBFalse;
}

/*!
 * \brief 读入语言主数据
 * \param pEngine 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-19 20:38:37
 */
static GBRETURN GBEngine_LoadMDB(PCGBEngine pEngine)
{
	GBRETURN ret = GBOK;

#ifdef  GBIME_CFG_STATIC_DB
	#ifdef GBIME_CFG_CHS_SUPPORT // 中文支持
	ret = GBEngine_LoadData(pEngine, (GBACHAR *)Global_GetChnLangData());
	#endif

	#ifdef GBIME_CFG_EN_SUPPORT // 英文支持
	ret = GBEngine_LoadData(pEngine, (GBACHAR *)Global_GetAlpLangData());
	#endif
#else
	#ifdef GBIME_CFG_CHS_SUPPORT // 中文支持
	ret = GBEngine_LoadData(pEngine, GBIME_DB_FILE_SC);
	#endif

	#ifdef GBIME_CFG_EN_SUPPORT // 英文支持
	ret = GBEngine_LoadData(pEngine, GBIME_DB_FILE_EN);
	#endif
#endif //GBIME_CFG_STATIC_DB

	return ret;
}

/*!
 * \brief 根据语言ID取得对应语言的UDB文件全路径
 * \param filePath 
 * \param language_id 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-8-31 15:13:57
*/
static void GBEngine_GetUDBFilePathByLang(GBPWCHAR filePath, GBINT language_id)
{
#if __MTK__ > 0
	PT_wsprintf((GBPWCHAR)filePath, "%w%08x.udb", GB_SYSTEM_DIR, language_id);
#else
	PT_wsprintf((GBPWCHAR)filePath, L"%s%08x.udb", GB_SYSTEM_DIR, language_id);
#endif
}

/*!
 * \brief 读取UDB数据
 * \param language_id 语言id
 * \param pRAM 用于存储UDB数据的内存块首地址
 * \param pRAMsize 用于存储UDB数据的内存块大小
 * \return
 * \note
 * \author weizhiping
 * \date 2009年12月11日
 */
static void GBEngine_ReadUDBData(long language_id, unsigned long *pRAM, size_t pRAMsize)
{
#ifdef GBIME_CFG_UDB_SUPPORT
	PT_FILE_HANDLE fp;
	GBUINT readLen = 0;
	GBWCHAR udbFileName[GBIME_FILENAME_LEN];

	GBEngine_GetUDBFilePathByLang(udbFileName, language_id);
	
	memset(pRAM, 0, pRAMsize);
	fp = PT_FS_Open(udbFileName, PT_FS_READ_WRITE);
	if(fp > 0)
	{
		PT_FS_Read(fp, pRAM, pRAMsize, &readLen);
		if(readLen != pRAMsize)
		{
			memset(pRAM, 0, pRAMsize);
		}
		PT_FS_Close(fp);
	}
#endif
}

/*!
 * \brief UDB更新函数, 引擎内部自动调用(如断开UDB模块时)
 * \param nUDBID UDB id
 * \param pData UDB数据块首地址
 * \param len UDB数据块大小
 * \param pOEMUpdateParam 调用GBUDBSetUpdateParam设置的参数pOEMUpdateParam(此处为language_id)
 * \return
 * \note 
 * \author weizhiping
 * \date 2009年12月11日
 */
static void GBEngine_UpdateUDB(GBI32 nUDBID, GBLPVOID pData, GBU32 len, GBLPVOID pOEMUpdateParam)
{
#ifdef GBIME_CFG_UDB_SUPPORT
	GBU32 write_len = 0;
	PT_FILE_HANDLE fp;
	GBWCHAR udbFileName[GBIME_FILENAME_LEN];
	
	// 根据语言ID参数转换成对应的udb文件名称
	GBEngine_GetUDBFilePathByLang(udbFileName, (GBINT)pOEMUpdateParam);

	fp = PT_FS_Open(udbFileName, PT_FS_WRITE);
	if(fp > 0)
	{
		PT_FS_Write(fp, pData, len, &write_len);
		PT_FS_Close(fp);
	}
#endif
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// 引擎API封装函数实现
///////////////////////////////////////////////////////////////////////////////////////////////////
/*!
 * \brief 初始化国笔输入法引擎
 * \param pEngine 
 * \return 
 * \note 调用任何国笔引擎API之前必须先执行该函数
 *\example
 * \author weizhiping
 * \date 2010-7-22 11:06:32
 */
GBRETURN GBEngine_Initialize(PCGBEngine pEngine)
{
	GBU8  i=0;
	GBRETURN ret = GBOK;

	PT_Assert( GBEngine_IsOpened(pEngine) == 0 );

	/* 检查国笔系统文件夹是否存在 */
	PT_CheckFolder(GB_SYSTEM_DIR);

	/* 读取引擎功能选项配置文件然后设置引擎相关选项状态 */
	//GB_ReadConfig();

	/* 加载语言数据 */
	GBEngine_LoadMDB(pEngine);
	PT_Assert( pEngine->g_nDataCount > 0 );

	/* 检查语言类型*/
	for(i=0; i<pEngine->g_nDataCount;++i)
	{
		GBU32 data_size = 0x0;
		ret = GBGetDataLanguage( pEngine->g_ldbs[i].pData, 
			                     &(pEngine->g_ldbs[i].nLang), 
								 &(pEngine->g_ldbs[i].nSubLang), 
								 &data_size );
		PT_Assert( ret == GBOK );
	}

	/* 总初始化 */
	pEngine->g_gbis.pAuxInfo = &(pEngine->g_aux);
	pEngine->g_gbis.engine_private_buffer_size = sizeof(pEngine->g_gbis.engine_private_buffer);
	ret = GBInitialize( &(pEngine->g_gbis), pEngine->g_ldbs, pEngine->g_nDataCount );
	PT_Assert( ret == GBOK );

	/* 设置输入法语言 */
	ret = GBSetCurLanguage( &(pEngine->g_gbis), pEngine->g_ldbs[0].nLang, pEngine->g_ldbs[0].nSubLang );
	PT_Assert( ret == GBOK );

	/* 设置候选类型和行数 */
	ret = GBMMISetDefaultCandListType(&(pEngine->g_gbis), GBCL_ROW_KEYBD);
	PT_Assert( ret == GBOK );
	GBMMISetCandRowCount(&(pEngine->g_gbis), GBCS_INPUT, 		1);
	GBMMISetCandRowCount(&(pEngine->g_gbis), GBCS_ASSOCIATE, 	1);
	
	/* 分页设置 */
	ret = GBEngine_SetPageOption(pEngine);
	PT_Assert( ret == GBOK );

	/* 设置引擎内部Log信息输出回调 */
	ret = GBAttachLogRecorder(&pEngine->g_gbis, setLogRecorder, NULL);
	PT_Assert( ret == GBOK );
	
	/* 加载UDB */
	#ifdef GBIME_CFG_UDB_SUPPORT	
	ret = GBEngine_LoadUDB(pEngine);
	PT_Assert( ret == GBOK );
	#endif

	// 加载产品配置
	//GBSetSelectedProduce(gb_config_info.gb_product_id);
	//loadProduceConfig();	
	//GBSetBusinessConfig(gb_config_info.gb_product_id, GB_ENGINE_COUNT, GBTrue);	
	//GBUpdateAllEngineOption();

	// 用户配置
	//GB_SetUserEngineOption();

	/* 打开成功 */
	pEngine->engine_opened = 1;
	return ret;
}

/*!
 * \brief 关闭国笔输入法引擎，所有绑定到引擎的MDB, UDB将被断开 
 * \param pEngine 
 * \return 
 * \note 如果需要继续调用引擎API，需要重新调用GBEngine_Initialize
 *\example
 * \author weizhiping
 * \date 2010-7-22 11:01:39
 */
GBRETURN GBEngine_Terminate(PCGBEngine pEngine)
{
	GBRETURN ret = GBOK;
	PT_Assert( GBEngine_IsOpened(pEngine) );

	GBEngine_UnLoadUDB(pEngine);
	ret = GBTerminate(&(pEngine->g_gbis));
	GBEngine_GlobalVariableInit(pEngine);
	return ret;
}

/*!
 * \brief 国笔输入法引擎GBMMI状态重置
 * \param pEngine 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-22 16:21:02
 */
GBRETURN GBEngine_Reset(PCGBEngine pEngine)
{
	GBRETURN ret = GBOK;
	PT_Assert(GBEngine_IsOpened(pEngine));

	ret = GBEngine_UnLoadUDB(pEngine);
	ret = GBMMIReset(&(pEngine->g_gbis));
	return ret;
}

/*!
 * \brief 判断国笔输入法引擎是否打开
 * \param pEngine 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-22 11:23:31
 */
GBBOOL GBEngine_IsOpened(PCGBEngine pEngine)
{
	return ( GBIsInited(&(pEngine->g_gbis)) == GBOK && pEngine->engine_opened );
}

/*!
 * \brief 输入法引擎消息处理函数
 * \param pIMEEvent 
 * \return 
 */
GBIMEReturn GBEngine_HandleMessage(PGBIMEEvent pIMEEvent)
{
	GBIMEReturn ret = GBIME_OK;
	PCLayoutAbstract pltObj = Global_GetCurrentLayoutObject();

	if (pIMEEvent->nType != GBIME_EVENT_TYPE_ENGINE
		|| GBInputBox_PlatformHandleKeyMode()) // 完全交给平台处理
	{
		return GBIME_IGNORE; // 非输入法引擎事件，忽略
	}

	switch (pIMEEvent->nEvent)
	{
	case GBIME_EVENT_ENGINE_INIT:
		ret = GBEngine_Initialize(Global_GetEnineInstance());
		//GBEngine_SwitchInputMode(GBIM_Pinyin, GBL_Chinese, GBL_Sub_Not_Spec); // 测试引擎
		break;		
	case GBIME_EVENT_ENGINE_EXIT:
		ret = GBEngine_Terminate(Global_GetEnineInstance());
		break;
	case GBIME_EVENT_ENGINE_RESET: // 引擎状态重置
		ret = GBEngine_Reset(Global_GetEnineInstance());
		ret = CLayoutControl_UpdateOutputInfo();
		break;
	case GBIME_EVENT_ENGINE_KEY:		
		if (Global_GetVk2KbSwitchFlag()) // 采用虚拟键盘切换到物理键盘用户所按中按键的键值
		{
			pIMEEvent->wParam = Global_GetVk2KbSwitchEngineKeyValue();
			Global_SetVk2KbSwitchFlag(GBFalse);
			Global_SetVk2KbSwitchEngineKeyValue(GBEvent_None);
			//GBInputBox_SetAllIMEFuncKey();
		} 
		else if (pltObj->bSendEngineKeyEvent) // 采用Layout预处理转换之后的键值参数
		{
			pIMEEvent->wParam = pltObj->engineKeyEvent;
			pIMEEvent->lParam = pltObj->engineEventParam;
		}

		// 国笔GBMMI按键处理函数(参数wParam为GBEventEnum中的值)
		ret = GBEngine_SendKeyEvent(Global_GetEnineInstance(), pIMEEvent->wParam, pIMEEvent->lParam);
		ret = CLayoutControl_UpdateOutputInfo();
		break;
	case GBIME_EVENT_ENGINE_GET_ASSOCWORD: ///<获取联想字符串
		{
			GBLPCWCHAR pConfirmedWord = (GBLPCWCHAR)pIMEEvent->wParam;

			// 重置引擎
			GBEngine_Reset(Global_GetEnineInstance());

		#if defined(__GBIME_HANDWRITE__)
			if (GBHW_GetEnglishStatus(Global_GetHWEnineInstance())/* && IS_ENGLISH_CHAR(pConfirmedWord[0]) 不要联想出中文*/)
			{
				ret = GBEngine_SwitchInputMode(Global_GetEnineInstance(), GBIM_Explicit_Alphabetic, GBL_English, GBL_Sub_Not_Spec);
				ret = GBEngine_SetEngineOption(Global_GetEnineInstance(), GB_ENGINE_ALPHABETIC,	
						GBALP_FIRST_KEY_ASSOC, 1);
				ret = GBEngine_SetInputString(Global_GetEnineInstance(), pConfirmedWord);
			}
			else
		#endif
			{
				ret = GBEngine_SwitchInputMode(Global_GetEnineInstance(), GBIM_Pinyin, GBL_Chinese, GBL_Sub_Not_Spec);
				ret = GBEngine_GetAssocWord(Global_GetEnineInstance(), pConfirmedWord);
			}
			ret = CLayoutControl_UpdateOutputInfo();
		}
		break;
	case GBIME_EVENT_ENGINE_GET_HOMONYMWORD: ///<获取同音字
		{
			GBLPCWCHAR pConfirmedWord = (GBLPCWCHAR)pIMEEvent->wParam;
			GBWCHAR pInputBuffer[GB_INPUT_BUFFER_LEN];

			ret = GBEngine_Word2CodesByInputmod(Global_GetEnineInstance(), GBIM_Qwerty_Pinyin, 
						pConfirmedWord[0], 0, pInputBuffer);
			ret = GBEngine_SwitchInputMode(Global_GetEnineInstance(), GBIM_Qwerty_Pinyin, GBL_Chinese, GBL_Sub_Not_Spec);
			ret = GBEngine_SetInputString(Global_GetEnineInstance(), pInputBuffer);
			ret = CLayoutControl_UpdateOutputInfo();
		}
		break;
	default:
		PT_Assert(0); // 非法事件
	}

	return ret;
}

/*!
 * \brief 切换国笔输入法引擎输入模式及语言环境
 * \param pEngine
 * \param input_mode 
 * \param nLangID 
 * \param nSubLangID 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-8-4 15:24:19
 */
GBRETURN GBEngine_SwitchInputMode(PCGBEngine pEngine, GBINT input_mode, GBINT nLangID, GBINT nSubLangID)
{
	pGBInputStruct pGBIS = &pEngine->g_gbis;
	GBRETURN ret;
	int candListType = GBCL_ROW_KEYBD;
	GBBOOL bStarSymbol = GBTrue;

	PT_Assert( GBEngine_IsOpened(pEngine) );

	// 先Reset在重新初始化
	ret = GBEngine_Reset(pEngine);

	ret = GBSetCurLanguage(pGBIS, nLangID, nSubLangID);
	ret = GBSetInputMode(pGBIS, input_mode);
	ret = GBMMIUnregisterInterpunctionKey(pGBIS, GBIME_INTERPUNCTION_KEY);

	switch(input_mode)
	{
	case GBIM_Qwerty_Jyutping:
	case GBIM_Explicit_Bopomofo:
	case GBIM_Qwerty_Pinyin:
	case GBIM_Half_Qwerty_Pinyin:
		candListType = GBCL_ROW_KEYBD;
		bStarSymbol = GBTrue;
		break;
	case GBIM_Predictive:
		if(pEngine->g_gbis.nLang == GBL_English
			|| pEngine->g_gbis.nLang == GBL_French
			|| pEngine->g_gbis.nLang == GBL_German)
		{
			//if (gb_config_info.gb_config_switch.basic.smart_english_inline_edit)
			{
				//candListType = GBCL_SINGLE_CAND;
			}
			//else
			{
				candListType = GBCL_ROW_KEYBD;
			}
			bStarSymbol = GBTrue;
			//#ifdef __USE_GB_MULTITAP__
			//GBMultitap_SetInterpunctionKeyFlag(GBTrue, GBTrue);
			//#endif
		}
		else
		{
			candListType = GBCL_ROW_KEYBD;
			bStarSymbol = GBTrue;
		}
		break;
	case GBIM_Qwerty_Alphabetic:
	case GBIM_Half_Qwerty_Alphabetic:
		//if (gb_config_info.gb_config_switch.basic.smart_english_inline_edit)
		//{
		//	candListType = GBCL_SINGLE_CAND;
		//}
		//else
		{
			candListType = GBCL_ROW_KEYBD;
		}
		break;
	case GBIM_Pinyin:
	case GBIM_Jyutping:
		candListType = GBCL_ROW_KEYBD;
		bStarSymbol = GBTrue;
		//#ifdef __USE_GB_MULTITAP__
		//GBMultitap_SetInterpunctionKeyFlag(GBTrue, GBTrue);
		//#endif
		break;
	case GBIM_Stroke:
		candListType = GBCL_ROW_KEYBD;
		bStarSymbol = GBTrue;
		GBEngine_UnregisterInterpunctionKey(pEngine, '1');
		GBEngine_UnregisterInterpunctionKey(pEngine, '0');
		break;
	case GBIM_Number:
		break;
	}

	// 设置候选列表类型
	ret = GBMMISetDefaultCandListType(pGBIS, candListType);
	
// 	// 设置候选行数
// 	if (1)
// 	{
// 		GBMMISetCandRowCount(pGBIS, GBCS_INPUT, 		ML_INPUT_CAND_ROW_COUNT);
// 		GBMMISetCandRowCount(pGBIS, GBCS_ASSOCIATE, 	ML_ASSOCIATE_CAND_ROW_COUNT);
// 	}
// 	else
// 	{
// 		GBMMISetCandRowCount(pGBIS, GBCS_INPUT,			SL_INPUT_CAND_ROW_COUNT);
// 		GBMMISetCandRowCount(pGBIS, GBCS_ASSOCIATE,		SL_ASSOCIATE_CAND_ROW_COUNT);
// 	}

//#ifdef __USE_GB_MULTITAP__
//	GBMultitap_SetInterpunctionKey(); // 0、1键不注册标点符号功能键，采用GBV4处理方式
//#endif

	// 注册为默认常规功能(输入区有输入时，按此键，输入简拼分隔符)	
	if(bStarSymbol)
	{
		GBMMIRegisterInterpunctionKeyEx(pGBIS, GBIME_INTERPUNCTION_KEY, NULL, 0, GBCL_ROW_MULTITAP, 
			GBIME_INTERPUNCTION_CAND_DESIRE_ROW, GBIME_INTERPUNCTION_CAND_COUNT_PER_ROW, 0);
	}

	// 无缝切换键
	//if (gb_config_info.gb_config_switch.basic.seamlessSwich)
	if (1)
	{
		GBMMISetSwitchOption(pGBIS, GBIME_SWITCH_KEY, GBL_English, GBL_Sub_Not_Spec, GBIM_Predictive);
	}
	else
	{
		GBMMISetSwitchOption(pGBIS, 0, GBL_English, GBL_Sub_Not_Spec, GBIM_Predictive);
	}

	// 根据引擎配置设置相应功能选项
	//GB_SetUserEngineOption();

	/* 重新加载UDB */
#ifdef GBIME_CFG_UDB_SUPPORT	
	ret = GBEngine_LoadUDB(pEngine);
	PT_Assert( ret == GBOK );
#endif

	pEngine->input_mode_is_valid = 1;	
	return ret;
}
/*!
 * \brief 判断引擎是否准备好接收用户输入了
 * \param pEngine 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-22 11:27:12
 */
GBBOOL GBEngine_IsReadyInput(PCGBEngine pEngine)
{
	PT_Assert( GBEngine_IsOpened(pEngine) );
	return ( pEngine->input_mode_is_valid==1);
}

/*!
 * \brief 发送按键消息给引擎处理
 * \param pEngine 
 * \param evt 
 * \param param 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-22 11:29:07
 */
GBU32 GBEngine_SendKeyEvent(PCGBEngine pEngine, GBINT evt, GBU32 param )
{
	GBU32 ret = GB_NOT_HANDLED;
		
	switch( evt )
	{
	case GBKEY_OK:
	case GBKEY_BACK:

	case GBKEY_LEFT:
	case GBKEY_RIGHT:
	case GBKEY_UP:
	case GBKEY_DOWN:

	case GBKEY_0:
	case GBKEY_1:
	case GBKEY_2:
	case GBKEY_3:
	case GBKEY_4:
	case GBKEY_5:
	case GBKEY_6:
	case GBKEY_7:
	case GBKEY_8:
	case GBKEY_9:

	case GBKEY_A:
	case GBKEY_B:
	case GBKEY_C:
	case GBKEY_D:
	case GBKEY_E:
	case GBKEY_F:
	case GBKEY_G:
	case GBKEY_H:
	case GBKEY_I:
	case GBKEY_J:
	case GBKEY_K:
	case GBKEY_L:
	case GBKEY_M:
	case GBKEY_N:
	case GBKEY_O:
	case GBKEY_P:
	case GBKEY_Q:
	case GBKEY_R:
	case GBKEY_S:
	case GBKEY_T:
	case GBKEY_U:
	case GBKEY_V:
	case GBKEY_W:
	case GBKEY_X:
	case GBKEY_Y:
	case GBKEY_Z:
	case GBKEY_AMB_SPECIAL_A: ///< 在中文模式下面输入分隔符，在Alphabetic改变大小写
	case GBKEY_AMB_SPECIAL_B: ///< 默认不处理，可以设置标点符号
	case GBKEY_AMB_SPECIAL_E:
		ret = _input_key_event(pEngine, evt,param);
		break;
// 	case GBKEY_AMB_SPECIAL_B:
// 
// 		ret = _input_key_event(pEngine, evt,param);
// 		if(GBE_API_WRONG_STRING_IN_WRONG_STATUS == ret)
// 		{
// 			pEngine->isSwitchNextInputMode = GBTrue;
// 		}
// 		break;
	case GBEVENT_SELECT_SYLLABLE_PAGED:
		GBEngine_select_syll_in_page(pEngine, (GBU16)param);
		break;
	case GBEVENT_SELECT_CANDIDATE:
		GBEngine_SelCandidateByIndex(pEngine, param);
		break;	
	case GBKEY_NEXT_PAGE_CANDIATE:
		GBEngine_GetNextPageCandidate(pEngine);
		//printf("**************GBEngine_SendKeyEvent() GBKEY_NEXT_PAGE_CANDIATE\n");
		break;
	case GBKEY_PREV_PAGE_CANDIATE:
		GBEngine_GetPrevPageCandidate(pEngine);
		//printf("**************GBEngine_SendKeyEvent() GBKEY_PREV_PAGE_CANDIATE\n");
		break;
	case GBKEY_NEXT_PAGE_SYLLABLE:
		GBEngine_GetNextSyllPage(pEngine);
		break;
	case GBKEY_PREV_PAGE_SYLLABLE:
		GBEngine_GetPrevSyllPage(pEngine);
		break;
	case GBKEY_NEXT_SYLLABLE:
		GBEngine_GetNextSyllable(pEngine);
		break;
	case GBKEY_PREV_SYLLABLE:
		GBEngine_GetPrevSyllPage(pEngine);
		break;
	default:
		//PT_Assert(0);
		break;
	}
	return ret;
}

/*!
 * \brief 获取引擎当前输入语言
 * \param pEngine 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-8-5 17:07:52
 */
GBLANG GBEngine_GetCurLang(PCGBEngine pEngine)
{
	return pEngine->g_gbis.nLang;
}


/*!
 * \brief 获取引擎当前输入模式
 * \param pEngine 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-8-5 17:07:52
 */
GBU8 GBEngine_GetInputMode(PCGBEngine pEngine)
{
	return pEngine->g_gbis.nInputMode;
}

/*!
 * \brief 是否常规按键
 * \param pEngine 
 * \param event 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-8-5 17:07:13
 */
GBBOOL GBEngine_IsNormalKey(PCGBEngine pEngine, GBINT event)
{
	if (((event) >= GBKEY_BACK&& (event) <= GBKEY_SPACE)
		|| ((event) >= GBKEY_0 && (event) <= GBKEY_9)
		|| ((event) >= GBKEY_A && (event) <= GBKEY_Z)
		|| ((event) == GBKEY_OK)
		|| ((event) >= GBKEY_LEFT&& (event) <= GBKEY_DOWN)
		|| ((event) == GBIME_SWITCH_KEY)
		|| ((event) == GBIME_INTERPUNCTION_KEY)
		|| ((event) >= GBKEY_BPMF_B && (event) <= GBKEY_BPMF_IU))
	{
		return GBTrue;
	}
	return GBFalse;
}

/*!
 * \brief 当前状态下面是否应该处理上下左右OK和删除等功能按键【判断国笔候选框是否激活】
 * \param pEngine 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-8-5 17:07:05
 */
GBBOOL GBEngine_IsActive(PCGBEngine pEngine)
{
	if (pEngine->g_gbis.outputInfo.nStatusFlag & GBSF_OpenCand || pEngine->g_gbis.pAuxInfo->pCompStr != NULL)
	{
		return GBTrue;
	}
	return GBFalse;
}

/*!
 * \brief 是否可以上屏
 * \param pEngine 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-8-5 17:06:54
 */
GBBOOL GBEngine_CanUpScreen(PCGBEngine pEngine)
{
	if (pEngine->g_gbis.outputInfo.pUpScreenStr && pEngine->g_gbis.outputInfo.pUpScreenStr[0])
	{
		return GBTrue;
	}
	return GBFalse;
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// 以下操作仅针对INPUT_MODE_IS_AMB_CHN( 处于这种输入模式下才可能有多个音节候选)
///////////////////////////////////////////////////////////////////////////////////////////////////

/*!
 * \brief 输入模式是否是模糊的中文输入法，模糊的中文输入法才存在拼音的候选
 * \param pEngine 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-22 16:39:37
 */
GBBOOL GBEngine_Is_AMB_Chn(PCGBEngine pEngine)
{
	PT_Assert( GBEngine_IsReadyInput(pEngine) );
	if(INPUT_MODE_IS_AMB_CHN( pEngine->g_gbis.nInputMode))
		return GBTrue;
	else
		return GBFalse;	
}

/*!
 * \brief 当前状态下是否可能存在音节候选
 * \param pEngine 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-22 16:39:37
 */
GBBOOL GBEngine_HaveSyllable(PCGBEngine pEngine)
{
	if(!GBEngine_Is_AMB_Chn(pEngine))
		return 0;
	return GBIS_HaveSyllableSelection(&(pEngine->g_gbis));
}

/*!
* \brief 获取当前页拼音音节的个数，拼音会根据GBSetPageOption
*  里面拼音的分页信息来分页， 如果需要返回所有的拼音候选，
*  则只需要把GBSetPageOption 中拼音的分页设置为返回GB_MAX_SYLLABLES个拼音即可
 * \param pEngine 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-22 16:41:19
 */
GBU8 GBEngine_GetSyllableNum(PCGBEngine pEngine)
{
	if(!GBEngine_Is_AMB_Chn(pEngine))
		return 0;
	return pEngine->g_gbis.pAuxInfo->nSyllableNum;
}

/*!
 * \brief 获取拼音音节buffer
 * \param pEngine 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-22 16:43:35
 */
GBPCWCHAR * GBEngine_GetSyllableBuffer(PCGBEngine pEngine)
{
	return pEngine->g_gbis.pAuxInfo->pSyllables;
}

/*!
 * \brief 存在上一页音节
 * \param pEngine 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-22 16:48:17
 */
GBU8 GBEngine_HavePrevSyllPage(PCGBEngine pEngine)
{
	if(!GBEngine_HaveSyllable(pEngine))
		return 0;
	return ( pEngine->g_gbis.pAuxInfo->nSyllablePageStartIndex > 0 );
		     
}

/*!
 * \brief 存在下一页音节
 * \param pEngine 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-22 16:48:17
 */
GBU8 GBEngine_HaveNextSyllPage(PCGBEngine pEngine)
{
	if(!GBEngine_HaveSyllable(pEngine))
		return 0;
	return (GBU8)(GBIS_HaveMoreSyllable(&(pEngine->g_gbis)));
}

/*!
 * \brief 进入音节翻页模式
 * \param pEngine 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-8-5 17:06:15
 */
static GBU8 _enter_syll_page_mode(PCGBEngine pEngine)
{
	if(GBEngine_HaveSyllable(pEngine))
	{
		GBU16 cur_state = GBEngine_GetCurStatus(pEngine);
		
		/* 以下状态可以直接选择音节 */
		if( cur_state == GBIMS_INPUT ||	
			cur_state == GBIMS_CHN_UAW   )
			return 1;
		
		/* 以下状态需要先回滚到之前的态 */
		if( cur_state == GBIMS_SELECTED )
		{
			_input_key_event(pEngine, GBKEY_BACK,0); // 发BACK，不是发UP !!
			
			#ifdef GBIMF_DEF_DEBUG
			cur_state = GBEngine_GetCurStatus();
			PT_Assert( cur_state == GBIMS_INPUT || cur_state == GBIMS_CHN_UAW );
			#endif
			
			return 1;
		}
	}
	return 0;
}

/*!
 * \brief 获取上一页音节
 * \param pEngine 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-22 16:48:17
 */
void GBEngine_GetPrevSyllPage(PCGBEngine pEngine)
{
	if( _enter_syll_page_mode(pEngine) && GBEngine_HavePrevSyllPage(pEngine) )
		_input_key_event(pEngine, GBKEY_PREV_PAGE_SYLLABLE, 0 );
}

/*!
 * \brief 获取下一页音节
 * \param pEngine 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-22 16:48:17
 */
void GBEngine_GetNextSyllPage(PCGBEngine pEngine)
{
	if( _enter_syll_page_mode(pEngine) && GBEngine_HaveNextSyllPage(pEngine) )
		_input_key_event(pEngine, GBKEY_NEXT_PAGE_SYLLABLE, 0 );
}

/*!
 * \brief 存在上一个音节
 * \param pEngine 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-22 16:48:17
 */
GBU8 GBEngine_HavePrevSyllable(PCGBEngine pEngine)
{
	if(!GBEngine_HaveSyllable(pEngine))
		return 0;
	if( pEngine->g_gbis.pAuxInfo->nSyllablePageStartIndex > 0 ||
		pEngine->g_gbis.pAuxInfo->nSyllableIndex > pEngine->g_gbis.pAuxInfo->nSyllablePageStartIndex )
		return 1;
	else
		return 0;
}

/*!
 * \brief 存在下一个音节
 * \param pEngine 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-22 16:48:17
 */
GBU8 GBEngine_HaveNextSyllable(PCGBEngine pEngine)
{
	GBINT iSyllableCount = 0;
	GBRETURN ret = GBOK;
	if(!GBEngine_HaveSyllable(pEngine))
		return 0;
	ret = GBGetAllSyllableCount( &(pEngine->g_gbis), &iSyllableCount);
	PT_Assert( ret == GBOK );
	if( pEngine->g_gbis.pAuxInfo->nSyllableIndex < ((GBU8)iSyllableCount) )
		return 1;
	else
		return 0;
}

/*!
 * \brief 获取上一个音节
 * \param pEngine 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-22 16:48:17
 */
void GBEngine_GetPrevSyllable(PCGBEngine pEngine)
{
	if( _enter_syll_page_mode(pEngine) && GBEngine_HavePrevSyllable(pEngine) )
		_input_key_event(pEngine, GBKEY_PREV_SYLLABLE, 0 );
}

/*!
 * \brief 获取下一个音节
 * \param pEngine 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-22 16:48:17
 */
void GBEngine_GetNextSyllable(PCGBEngine pEngine)
{
	if( _enter_syll_page_mode(pEngine) && GBEngine_HaveNextSyllable(pEngine) )
		_input_key_event(pEngine, GBKEY_NEXT_SYLLABLE, 0 );
}

/*!
 * \brief 选中指定索引的拼音音节
 * \param pEngine 
 * \param idx 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-22 16:48:17
 */
void GBEngine_select_syll_in_page(PCGBEngine pEngine, GBU16 idx )
{
	if(_enter_syll_page_mode(pEngine))
	{
		PT_Assert(idx >=0 && idx <= GBEngine_GetSyllableNum(pEngine));
		_input_key_event(pEngine, GBEVENT_SELECT_SYLLABLE_PAGED, idx);
	}
}

/*!
 * \brief 获取当前音节索引
 * \param pEngine 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-22 16:59:26
 */
GBU8 GBEngine_GetCurrentSyllableIndex(PCGBEngine pEngine)
{
	return pEngine->g_gbis.pAuxInfo->nSyllableIndex - pEngine->g_gbis.pAuxInfo->nSyllablePageStartIndex;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// 以下操作仅针对候选
///////////////////////////////////////////////////////////////////////////////////////////////////

/*!
 * \brief 存在候选
 * \param pEngine 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-22 17:01:04
 */
GBBOOL GBEngine_HaveCandidate(PCGBEngine pEngine)
{
	PT_Assert( GBEngine_IsReadyInput(pEngine) );
	if( pEngine->g_gbis.outputInfo.nCandNum > 0 )
		return GBTrue;
	else
		return GBFalse;
}

/*!
 * \brief 获取候选总个数
 * \param pEngine 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-22 17:01:16
 */
GBU16 GBEngine_GetCandNum(PCGBEngine pEngine)
{
	PT_Assert( GBEngine_IsReadyInput(pEngine) );
	return pEngine->g_gbis.outputInfo.nCandNum;
}

/*!
 * \brief 获取候选字Buffer
 * \param pEngine 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-22 17:01:16
 */
GBPCWCHAR * GBEngine_GetCandidateBuffer(PCGBEngine pEngine)
{
	return pEngine->g_gbis.outputInfo.pCandidates;
}

/*!
 * \brief 是否存在上一页候选
 * \param pEngine 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-22 17:01:16
 */
GBU8 GBEngine_HavePrevCandPage(PCGBEngine pEngine)
{
	if( GBEngine_GetCurStatus(pEngine) == GBIMS_ALP_UAW_NO_MORE_CAND )
		return 1;
	else if( GBHavePrevPage( &(pEngine->g_gbis) ) == GBOK )
		return 2;
	else
		return 0;
}

/*!
 * \brief 是否存在下一页候选
 * \param pEngine 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-22 17:01:16
 */
GBU8 GBEngine_HaveNextCandPage(PCGBEngine pEngine)
{
	if( GBHaveNextPage( &(pEngine->g_gbis) ) == GBOK )
		return 1;
	else
		return 0;
}

/*!
 * \brief 进入翻页模式
 * \param pEngine 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-8-5 17:03:38
 */
static GBU8 _enter_cand_page_mode(PCGBEngine pEngine)
{
// 	if(GBEngine_HaveCandidate(pEngine))
// 	{
// 		GBU16 cur_state = GBEngine_GetCurStatus(pEngine);
// 		
// 		/* 以下状态可以直接选择候选 */
// 		if( cur_state == GBIMS_SELECTED )
// 			return 1;
// 
// 		/* 以下状态需要前进到SELECT态 */
// 		if( cur_state == GBIMS_INPUT     || 
// 			cur_state == GBIMS_CHN_UAW   ||
// 			cur_state == GBIMS_ALP_UAW   || 
// 			cur_state == GBIMS_ASSOCIATE ||
// 			cur_state == GBIMS_MULTITAP_INTERPUNCTION )
// 		{
// 			_input_key_event(pEngine, GBKEY_DOWN,0);
// 			
// 			#ifdef GBIMF_DEF_DEBUG
// 			PT_Assert( GBEngine_GetCurStatus() == GBIMS_SELECTED );
// 			#endif
// 			
// 			return 1;
// 		}
// 	}
// 	return 0;
	return 1; // 不需要进入选择状态，否则翻页后直接变成数字选择候选而不是继续输入
}

/*!
 * \brief 获取上一页候选
 * \param pEngine 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-22 17:01:16
 */
void GBEngine_GetPrevPageCandidate(PCGBEngine pEngine)
{
	GBU8 ret = GBEngine_HavePrevCandPage(pEngine);
/*	GBINT nStartIndex = 0;*/

	if( ret != 0 )
	{
		//nStartIndex = pEngine->g_gbis.outputInfo.nFirstCandIndex - 1;
		_input_key_event(pEngine, GBKEY_PREV_PAGE_CANDIATE, 0 );
		//GBPrevPageCandidate(&pEngine->g_gbis, nStartIndex);
	}
}

/*!
 * \brief 获取下一页候选
 * \param pEngine 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-22 17:01:16
 */
void GBEngine_GetNextPageCandidate(PCGBEngine pEngine)
{
// 	GBINT nStartIndex = 0;
// 	GBRETURN ret = GBOK;

	if( GBEngine_HaveNextCandPage(pEngine) != 0 )
	{
		if( _enter_cand_page_mode(pEngine) )
			_input_key_event(pEngine, GBKEY_NEXT_PAGE_CANDIATE, 0 );
// 		nStartIndex = pEngine->g_gbis.outputInfo.nCandNum + pEngine->g_gbis.outputInfo.nFirstCandIndex;
// 		ret = GBNextPageCandidate(&pEngine->g_gbis, nStartIndex);
	}
}

/*!
 * \brief 存在上一个候选
 * \param pEngine 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-22 17:01:16
 */
GBBOOL GBEngine_HavePrevCandidate(PCGBEngine pEngine)
{
	if(!GBEngine_HaveCandidate(pEngine))
		return GBFalse;
	if( pEngine->g_gbis.outputInfo.nFirstCandIndex != 0xFFFF && 
		( pEngine->g_gbis.outputInfo.nFirstCandIndex >0 ||
		  pEngine->g_gbis.pAuxInfo->nSelCol > pEngine->g_gbis.outputInfo.nFirstCandIndex ) )
		return GBTrue;
	else
		return GBFalse;
}

/*!
 * \brief 存在下一个候选
 * \param pEngine 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-22 17:01:16
 */
GBBOOL GBEngine_HaveNextCandidate(PCGBEngine pEngine)
{
	GBINT iCandCount = 0;
	GBRETURN ret = GBOK;

	if( GBEngine_HaveCandidate(pEngine) == 0 )
		return 0;
	ret = GBGetCandidatesCount(&(pEngine->g_gbis),&iCandCount);
	PT_Assert( ret == GBOK );
	if( pEngine->g_gbis.outputInfo.nFirstCandIndex != 0xFFFF && 
		( (pEngine->g_gbis.outputInfo.nFirstCandIndex+ pEngine->g_gbis.pAuxInfo->nSelCol) < ((GBU16)iCandCount-1) ) )
		return 1;
	else
		return 0;
}

/*!
 * \brief 获取上一个候选
 * \param pEngine 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-22 17:01:16
 */
void GBEngine_GetPrevCandidate(PCGBEngine pEngine)
{
	if( _enter_cand_page_mode(pEngine) && GBEngine_HavePrevCandidate(pEngine) )
		_input_key_event(pEngine, GBKEY_PREV_CANDIDATE, 0 );
}

/*!
 * \brief 获取下一个候选
 * \param pEngine 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-22 17:01:16
 */
void GBEngine_GetNextCandidate(PCGBEngine pEngine)
{
	if( _enter_cand_page_mode(pEngine) && GBEngine_HaveNextCandidate(pEngine) )
		_input_key_event(pEngine, GBKEY_NEXT_CANDIDATE, 0 );
}

/*!
 * \brief 选择当前候选分页中的指定下标的候选
 * \param pEngine
 * \param idx 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-22 17:01:16
 */
void GBEngine_SelCandidateByIndex(PCGBEngine pEngine, GBU32 idx)
{
	GBU16 cur_state = GBEngine_GetCurStatus(pEngine);

	if( cur_state == GBIMS_ALP_UAW_NO_MORE_CAND ||
	    cur_state == GBIMS_ALP_UAW_BEGIN        ||
	    cur_state == GBIMS_ALP_UAW_Confirm         )
	{
		_input_key_event(pEngine, GBKEY_OK,0);
	}
	else if( _enter_cand_page_mode(pEngine) )
	{
#if 0 // 多行候选时返回的索引参见引擎说明【GBEVENT_SELECT_CANDIDATE：param参数的低16位表示选择的列，高16位表示选择的行】
		GBU16 move_step=0;
		if (!( idx >=0 && idx < GBEngine_GetCandNum(pEngine)))
		{
			return ; // 参数出错了!!!
		}
		
		if( idx < pEngine->g_gbis.pAuxInfo->nSelCol )
		{
			for(move_step=pEngine->g_gbis.pAuxInfo->nSelCol; move_step>idx;--move_step)
				_input_key_event(pEngine, GBKEY_PREV_CANDIDATE,0);
		}
		else if( idx > pEngine->g_gbis.pAuxInfo->nSelCol )
		{
			for(move_step=pEngine->g_gbis.pAuxInfo->nSelCol;move_step<idx;++move_step)
				_input_key_event(pEngine, GBKEY_NEXT_CANDIDATE,0);
		}
#endif				
		_input_key_event(pEngine, GBEVENT_SELECT_CANDIDATE, idx);
	}
}

/*!
 * \brief 获取联想字符串(目前只支持中文)
 * \param pEngine 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-22 17:01:16
 */
GBRETURN GBEngine_GetAssocWord(PCGBEngine pEngine, GBLPCWCHAR pConfirmedWord )
{
	GBRETURN ret = GBOK;
	
	GBMMIReset(&(pEngine->g_gbis));
	ret = GBGetAssocWord(&(pEngine->g_gbis), pConfirmedWord, GBTrue );
	return ret;
}

/*!
 * \brief 获取笔画区内码串
 * \param pEngine 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-22 17:01:16
 */
GBWCHAR * GBEngine_GetStorkeString(PCGBEngine pEngine)
{
	GBChnGetComponents(&(pEngine->g_gbis));
	return pEngine->g_gbis.pAuxInfo->wStroke;
}

/*!
 * \brief 获取部首区内码串
 * \param pEngine 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-22 17:01:16
 */
GBWCHAR * GBEngine_GetComponentsString(PCGBEngine pEngine)
{
	GBChnGetComponents(&(pEngine->g_gbis));
	return pEngine->g_gbis.pAuxInfo->wComponents;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// 引擎交互缓冲区访问
///////////////////////////////////////////////////////////////////////////////////////////////////
/*!
 * \brief 获取需要上屏的字符串
 * \param pEngine 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-22 17:01:16
 */
GBWCHAR * GBEngine_GetUpScreenString(PCGBEngine pEngine)
{
	PT_Assert( GBEngine_IsOpened(pEngine) );
	return pEngine->g_gbis.outputInfo.pUpScreenStr;
}

/*!
 * \brief 清空需要上屏的字符串
 * \param pEngine 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-22 17:01:16
 */
void GBEngine_ClearUpScreenString(PCGBEngine pEngine)
{
	PT_Assert( GBEngine_IsOpened(pEngine) );
	if( pEngine->g_gbis.outputInfo.pUpScreenStr != NULL )
		pEngine->g_gbis.outputInfo.pUpScreenStr[0] = 0x0;
}

/*!
 * \brief 获取输出串（中文输入法自造词的时候已经选择了的词语以及后面的输入串）
 * \param pEngine 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-22 17:01:16
 */
GBPCWCHAR GBEngine_GetOutputString(PCGBEngine pEngine)
{
	PT_Assert( GBEngine_IsReadyInput(pEngine) );
	return pEngine->g_gbis.pAuxInfo->pOutputString;
}

/*!
 * \brief 获取输入序列
 * \param pEngine 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-22 17:01:16
 */
GBPCWCHAR GBEngine_GetInputString(PCGBEngine pEngine)
{
	return pEngine->g_gbis.inputString;
}

/*!
 * \brief 获取输入模式的具体状态,在GBMMI的API里面输出
 * \param pEngine 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-22 17:01:16
 */
GBU16 GBEngine_GetCurStatus(PCGBEngine pEngine)
{
	PT_Assert( GBEngine_IsReadyInput(pEngine) );
	return pEngine->g_gbis.pAuxInfo->nStatus;
}

/*!
 * \brief 是否可按上键上屏
 * \param pEngine 
 * \return 
 * \note 
 *\example
 * \author Zhaokun
 * \date 2010-10-21 17:01:16
 */
GBBOOL GBEngine_GetUpScreenArrowFlag(PCGBEngine pEngine)
{
	GBBOOL bDrawFlag = GBFalse;
	
	PT_Assert( GBEngine_IsReadyInput(pEngine) );

	if(!GBEngine_IsSelectedStatus(pEngine) 
		&& (pEngine->g_gbis.pAuxInfo->nArrowFlags & GBTA_Up) == GBTA_Up
		&& !(pEngine->g_gbis.pAuxInfo->nStatus == GBIMS_ALP_UAW_NO_MORE_CAND))
	{
		bDrawFlag = GBTrue;
	}
	return bDrawFlag;
}

/*!
 * \brief 是否显示候选上箭头
 * \param pEngine 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-8-9 17:01:16
 */
GBBOOL GBEngine_GetCandUpArrowFlag(PCGBEngine pEngine)
{
	GBBOOL bDrawFlag = GBFalse;

	PT_Assert( GBEngine_IsReadyInput(pEngine) );

	if (GBEngine_IsAssociateStatus(pEngine))
	{
		bDrawFlag = (GBBOOL)(GBEngine_HavePrevCandPage(pEngine) != 0);
	} 
	else
	{
		bDrawFlag = (GBBOOL)(GBEngine_HavePrevCandPage(pEngine) != 0 
			                 && ((pEngine->g_gbis.pAuxInfo->nArrowFlags & GBTA_Up) == GBTA_Up));
	}

	return bDrawFlag;
}

/*!
 * \brief 是否显示候选下箭头
 * \param pEngine 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-8-9 17:01:16
 */
GBBOOL GBEngine_GetCandDownArrowFlag(PCGBEngine pEngine)
{
	GBBOOL bDrawFlag = GBFalse;
	
	PT_Assert( GBEngine_IsReadyInput(pEngine) );
	
	if (GBEngine_HaveNextCandPage(pEngine) 
		&& ((pEngine->g_gbis.pAuxInfo->nArrowFlags & GBTA_Down) == GBTA_Down))
	{
		bDrawFlag = GBTrue;
	}
	
	return bDrawFlag;
}

/*!
 * \brief 是否显示候选左箭头
 * \param pEngine 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-22 17:01:16
 */
GBBOOL GBEngine_GetCandLeftArrowFlag(PCGBEngine pEngine)
{
	PT_Assert( GBEngine_IsReadyInput(pEngine) );
	return (GBBOOL)((pEngine->g_gbis.pAuxInfo->nArrowFlags & GBTA_Left) == GBTA_Left);
}

/*!
 * \brief 是否显示候选右箭头
 * \param pEngine 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-8-9 17:01:16
 */
GBBOOL GBEngine_GetCandRightArrowFlag(PCGBEngine pEngine)
{
	PT_Assert( GBEngine_IsReadyInput(pEngine) );
	return (GBBOOL)((pEngine->g_gbis.pAuxInfo->nArrowFlags & GBTA_Right) == GBTA_Right);
}

/*!
 * \brief 是否显示音节左箭头
 * \param pEngine 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-8-9 17:01:16
 */
GBBOOL GBEngine_GetSyllableLeftArrowFlag(PCGBEngine pEngine)
{
	GBBOOL bDrawFlag = GBFalse;
	
	PT_Assert( GBEngine_IsReadyInput(pEngine) );
	
	if (pEngine->g_gbis.pAuxInfo->nSyllablePageStartIndex > 0
		&& ((pEngine->g_gbis.pAuxInfo->nArrowFlags & GBTA_Syllable_Left) == GBTA_Syllable_Left))
	{
		bDrawFlag = GBTrue;
	}
	
	return bDrawFlag;
}

/*!
 * \brief 是否显示音节右箭头
 * \param pEngine 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-8-9 17:01:16
 */
GBBOOL GBEngine_GetSyllableRightArrowFlag(PCGBEngine pEngine)
{
	GBBOOL bDrawFlag = GBFalse;
	
	PT_Assert( GBEngine_IsReadyInput(pEngine) );
	
	if (GBIS_HaveMoreSyllable(&pEngine->g_gbis)
		&& ((pEngine->g_gbis.pAuxInfo->nArrowFlags & GBTA_Syllable_Right) == GBTA_Syllable_Right))
	{
		bDrawFlag = GBTrue;
	}
	
	return bDrawFlag;
}

/*!
 * \brief 设置国笔输入法引擎的候选类型，会影响用户输出候选的方式
 * \param pEngine 
 * \param nCandType 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-8-17 10:18:54
*/
GBRETURN GBEngine_SetCandType(PCGBEngine pEngine, GBINT nCandType)
{
	PT_Assert( GBEngine_IsOpened(pEngine) );
	return GBSetCandType(&pEngine->g_gbis, nCandType);
}

/*!
 * \brief 设置引擎配置
 * \param pEngine 
 * \param engine_id 
 * \param config 
 * \param option 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-8-17 11:26:03
*/
GBRETURN GBEngine_SetEngineOption(PCGBEngine pEngine, GBINT engine_id, GBU32 config, GBU32 option)
{
	PT_Assert( GBEngine_IsOpened(pEngine) );
	return GBSetEngineOption(&pEngine->g_gbis, engine_id, config, option);
}

/*!
 * \brief 重新绑定UDB(在切换语言和输入法时调用)
 * \param pEngine
 * \return
 * \note 如果之前UDB Buffer已经设置为其他的语言，那么需要保存之前数据，
 * 同时将同一个UDB Buffer使用到新的语言的自造上面，
 * 这样做法会节约RAM空间，但是会导致一个副作用，就是在无缝切换之后
 * 切换到的语言没有自造词里面的候选。
 * 也可以申请两个UDB Buffer，给主语言以及切换的语言分别一个UDB Buffer。这样可以
 * 解决上面的问题，但是会增加RAM空间。
 * \author weizhiping
 * \date 2009年12月11日
 */
GBRETURN GBEngine_LoadUDB(PCGBEngine pEngine)
{
#ifdef GBIME_CFG_UDB_SUPPORT
	GBRETURN ret = GBOK;
	GBINT language_id = 0;
	
	// 先卸载，保存数据
	GBEngine_UnLoadUDB(pEngine);

	// 绑定当前语言UDB
	language_id = (pEngine->g_gbis.nLang << 16) + pEngine->g_gbis.nSubLang;	
	GBEngine_ReadUDBData(language_id, (unsigned long *)Global_GetMasterUDBData(), Global_GetMasterUDBDataSize());
	if ((ret = GBUDBAttach(&pEngine->g_gbis, (unsigned long *)Global_GetMasterUDBData(), 
		 Global_GetMasterUDBDataSize(), pEngine->g_gbis.nLang, pEngine->g_gbis.nSubLang, &pEngine->master_udb_id)) 
		 != GBOK)
	{
		return ret;
	}
	if ((ret = GBUDBSetUpdateParam(&pEngine->g_gbis, pEngine->master_udb_id, GBIME_UDB_UPDATE_FREQ, 
							  GBEngine_UpdateUDB, (GBLPVOID)language_id)) != GBOK)
	{
		return ret;
	}
	pEngine->master_udb_loaded = GBTrue;

	// 中文情况下需要辅助UDB
	if (!GBIS_Is_Alp_IM(&pEngine->g_gbis)
		&& pEngine->g_gbis.nLang != GBL_English)//Zhaokun, 物理Number输入模式时，语言也为GBL_English，出现重复加载udb
	{
		language_id = (GBL_English << 16) + 0;
		GBEngine_ReadUDBData(language_id, (unsigned long *)Global_GetSlaveUDBData(), Global_GetSlaveUDBDataSize());
		
		if ((ret = GBUDBAttach(&pEngine->g_gbis, (unsigned long *)Global_GetSlaveUDBData(), Global_GetSlaveUDBDataSize(), 
					GBL_English, 0, &pEngine->slave_udb_id)) != GBOK)
		{
			return ret;
		}
		if ((ret = GBUDBSetUpdateParam(&pEngine->g_gbis, pEngine->slave_udb_id, GBIME_UDB_UPDATE_FREQ, 
					GBEngine_UpdateUDB, (GBLPVOID)language_id)) != GBOK)
		{
			return ret;
		}
		pEngine->slave_udb_loaded = GBTrue;
	}

	return ret;
#else
	return GBOK;
#endif
}

/*!
 * \brief 卸载UDB
 * \param pEngine 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-8-31 14:32:28
*/
GBRETURN GBEngine_UnLoadUDB(PCGBEngine pEngine)
{
	if (pEngine->master_udb_loaded)
	{
		if(pEngine->master_udb_id != 0)
		{
			GBUDBDetach(&pEngine->g_gbis, pEngine->master_udb_id);	
			pEngine->master_udb_id = 0;
		}
		pEngine->master_udb_loaded = GBFalse;
	}
	
	if (pEngine->slave_udb_loaded)
	{
		if(pEngine->slave_udb_id != 0)
		{
			GBUDBDetach(&pEngine->g_gbis, pEngine->slave_udb_id);	
			pEngine->slave_udb_id = 0;
		}	
		pEngine->slave_udb_loaded = GBFalse;
	}

	return GBOK;
}

/*!
 * \brief 设置国笔输入法引擎标点符号输入选项
 * \param pEngine 
 * \param cKey 
 * \param pSymbols 
 * \param cSplit 
 * \param nCandListType 
 * \param nCandDesireRow 
 * \param nCandCountPerRow 
 * \param nOption 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-8-31 18:17:17
*/
GBRETURN GBEngine_RegisterInterpunctionKeyEx(PCGBEngine pEngine,
							GBINT cKey,
							GBLPCWCHAR pSymbols,
							GBINT cSplit,
							GBINT nCandListType,
							GBINT nCandDesireRow,
							GBINT nCandCountPerRow,
							GBUINT nOption)
{
	PT_Assert( GBEngine_IsOpened(pEngine) );
	return GBMMIRegisterInterpunctionKeyEx(&pEngine->g_gbis, 
											cKey,
											pSymbols,
											cSplit,
											nCandListType,
											nCandDesireRow,
											nCandCountPerRow,
											nOption);
}

/*!
 * \brief 注销国笔输入法引擎标点符号功能键
 * \param pEngine 
 * \param cKey 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-1 15:34:12
*/
GBRETURN GBEngine_UnregisterInterpunctionKey(PCGBEngine pEngine, GBINT cKey)
{
	PT_Assert( GBEngine_IsOpened(pEngine) );
	return GBMMIUnregisterInterpunctionKey(&pEngine->g_gbis, cKey);
}


/*!
 * \brief 设置国笔输入法引擎候选类型
 * \param pEngine 
 * \param Lang
 * \param  Chn_nCandRowCount
 * \param  Chn_candListType
 * \param  Alp_nCandRowCount
 * \param  Alp_candListType 
 * \return 
 * \note 
 * \example
 * \author Zhaokun
 * \date 2010-9-3 15:34:12
*/
GBRETURN GBEngine_SetCandStyle(PCGBEngine pEngine,
						   GBINT Lang, 
						   GBINT Chn_nCandRowCount,
						   GBINT Chn_candListType,
						   GBINT Alp_nCandRowCount,
						   GBINT Alp_candListType)
{
	GBRETURN ret = GBOK;
	GBINT nCandRowCount = 0;
	GBINT candListType = 0;
	
	switch (Lang)
	{
	case GBL_Chinese:
		nCandRowCount = Chn_nCandRowCount;
		candListType = Chn_candListType;
		break;
	case GBL_English:
		nCandRowCount = Alp_nCandRowCount;
		candListType = Alp_candListType;
		break;
	default:
		nCandRowCount = Alp_nCandRowCount;
		candListType = Alp_candListType;
		break;
	}
	
	GBMMISetDefaultCandListType(&pEngine->g_gbis, candListType);
	GBMMISetCandRowCount(&pEngine->g_gbis, GBCS_INPUT, nCandRowCount);
	GBMMISetCandRowCount(&pEngine->g_gbis, GBCS_ASSOCIATE, nCandRowCount + 1);
	return ret;
}

/*!
 * \brief 判断国笔输入法引擎当前是否处于中文输入模式
 * \param pEngine 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-2 10:16:27
*/
GBBOOL GBEngine_IsChineseInputMode(PCGBEngine pEngine)
{
	PT_Assert( GBEngine_IsOpened(pEngine) );
	return (GBBOOL)(GBIS_Is_Chn_IM(&pEngine->g_gbis));
}

/*!
 * \brief 判断国笔输入法引擎当前是否处于Alp输入模式
 * \param pEngine 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-2 10:15:56
*/
GBBOOL GBEngine_IsAlpInputMode(PCGBEngine pEngine)
{
	PT_Assert( GBEngine_IsOpened(pEngine) );
	return (GBBOOL)(GBIS_Is_Alp_IM(&pEngine->g_gbis));
}

/*!
 * \brief 判断国笔输入法引擎当前是否处于MultiTap输入模式
 * \param pEngine 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-2 10:19:59
*/
GBBOOL GBEngine_IsMultiTapStatus(PCGBEngine pEngine)
{
	PT_Assert( GBEngine_IsOpened(pEngine) );
	return (GBBOOL)(pEngine->g_gbis.outputInfo.nStatusFlag & GBSF_MultiTap);
}

/*!
 * \brief 判断国笔输入法引擎当前是否使用标点符号输入框类型
 * \param pEngine 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-2 10:19:59
*/
GBBOOL GBEngine_IsSymbolCandType(PCGBEngine pEngine)
{
	PT_Assert( GBEngine_IsOpened(pEngine) );
	return (GBBOOL)(pEngine->g_gbis.nCandType == GBCT_Symbol);
}

/*!
 * \brief 判断国笔输入法引擎当前是否处于初始化状态(刚进入编辑器或每次输入结束时候的状态
 * \param pEngine 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-2 10:19:59
*/
GBBOOL GBEngine_IsInitialStatus(PCGBEngine pEngine)
{
	PT_Assert( GBEngine_IsOpened(pEngine) );
	return (GBBOOL)(pEngine->g_gbis.pAuxInfo->nStatus == GBIMS_INITIAL);
}

/*!
 * \brief 判断国笔输入法引擎当前是否处于输入状态
 * \param pEngine 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-2 10:19:59
*/
GBBOOL GBEngine_IsInputStatus(PCGBEngine pEngine)
{
	PT_Assert( GBEngine_IsOpened(pEngine) );
	return (GBBOOL)(pEngine->g_gbis.pAuxInfo->nStatus == GBIMS_INPUT);
}

/*!
 * \brief 判断国笔输入法引擎当前是否处于候选择状态
 * \param pEngine 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-2 10:19:59
*/
GBBOOL GBEngine_IsSelectedStatus(PCGBEngine pEngine)
{
	PT_Assert( GBEngine_IsOpened(pEngine) );
	return (GBBOOL)GBIS_IS_CAND_SELECTED(&pEngine->g_gbis);
}

/*!
 * \brief 判断国笔输入法引擎当前是否处于联想模式
 * \param pEngine 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-2 10:16:41
*/
GBBOOL GBEngine_IsAssociateStatus(PCGBEngine pEngine)
{
	PT_Assert( GBEngine_IsOpened(pEngine) );
	return (GBBOOL)(pEngine->g_gbis.nCandType == GBCT_Associate);
}

/*!
 * \brief 判断国笔输入法引擎当前是否处于外文自造词模式
 * \param pEngine 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-2 10:19:59
*/
GBBOOL GBEngine_IsAlpUAWStatus(PCGBEngine pEngine)
{
	PT_Assert( GBEngine_IsOpened(pEngine) );
	return (GBBOOL)(pEngine->g_gbis.pAuxInfo->nStatus == GBIMS_ALP_UAW_BEGIN 
		|| pEngine->g_gbis.pAuxInfo->nStatus == GBIMS_ALP_UAW 
		|| pEngine->g_gbis.pAuxInfo->nStatus == GBIMS_ALP_UAW_Confirm);
}

/*!
 * \brief 判断国笔输入法引擎当前是否处于外文自造词开始状态
 * \param pEngine 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-2 10:19:59
*/
GBBOOL GBEngine_IsAlpUAWBeginStatus(PCGBEngine pEngine)
{
	PT_Assert( GBEngine_IsOpened(pEngine) );
	return (GBBOOL)(pEngine->g_gbis.pAuxInfo->nStatus == GBIMS_ALP_UAW_BEGIN);
}

/*!
 * \brief 判断国笔输入法引擎当前是否处于外文自造词过程状态
 * \param pEngine 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-2 10:19:59
*/
GBBOOL GBEngine_IsAlpUAWProcessingStatus(PCGBEngine pEngine)
{
	PT_Assert( GBEngine_IsOpened(pEngine) );
	return (GBBOOL)(pEngine->g_gbis.pAuxInfo->nStatus == GBIMS_ALP_UAW);
}

/*!
 * \brief 判断国笔输入法引擎当前是否处于外文自造词没有其他候选的时候按下翻状态
 * \param pEngine 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-2 10:19:59
*/
GBBOOL GBEngine_IsAlpUAWNoMoreCandStatus(PCGBEngine pEngine)
{
	PT_Assert( GBEngine_IsOpened(pEngine) );
	return (GBBOOL)(pEngine->g_gbis.pAuxInfo->nStatus == GBIMS_ALP_UAW_NO_MORE_CAND);
}

/*!
 * \brief 判断国笔输入法引擎当前是否处于外文自造词结束，用户确认加入，这个状态可以关闭
 * \param pEngine 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-2 10:19:59
*/
GBBOOL GBEngine_IsAlpUAWConfirmStatus(PCGBEngine pEngine)
{
	PT_Assert( GBEngine_IsOpened(pEngine) );
	return (GBBOOL)(pEngine->g_gbis.pAuxInfo->nStatus == GBIMS_ALP_UAW_Confirm);
}

/*!
 * \brief 
 * \param pGBIS 
 * \param nInputmod 
 * \param isn 
 * \param indexMulPron 
 * \param pBuff 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-6 17:56:40
*/
GBRETURN GBEngine_Word2CodesByInputmod(PCGBEngine pEngine, int nInputmod, unsigned short isn,
											GBINT indexMulPron, GBPWCHAR pBuff)
{
	PT_Assert( GBEngine_IsOpened(pEngine) );
	return GBWord2CodesByInputmod(&pEngine->g_gbis, nInputmod, isn,	indexMulPron, pBuff);
}

/*!
 * \brief 直接更新输入串获取候选
 * \param pEngine 
 * \param pInputString 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-7 8:51:43
*/
GBRETURN GBEngine_SetInputString(PCGBEngine pEngine, GBLPCWCHAR pInputString)
{
	PT_Assert( GBEngine_IsOpened(pEngine) );
	return GBSetInputString(&pEngine->g_gbis, pInputString);
}

/*!
 * \brief 获取行索引起始位置指针 GBMMI使用
 * \param pEngine 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-2 10:19:59
*/
GBPBYTE GBEngine_GetRowStart(PCGBEngine pEngine)
{
	PT_Assert( GBEngine_IsOpened(pEngine) );
	return pEngine->g_gbis.pAuxInfo->pRowStart;
}
/*!
 * \brief 获取当前选择的列 GBMMI使用
 * \param pEngine 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-2 10:19:59
*/
GBI8 GBEngine_GetSelCol(PCGBEngine pEngine)
{
	PT_Assert( GBEngine_IsOpened(pEngine) );
	return pEngine->g_gbis.pAuxInfo->nSelCol;
}
/*!
 * \brief 获取当前选择的行 GBMMI使用
 * \param pEngine 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-2 10:19:59
*/
GBI8 GBEngine_GetSelRow(PCGBEngine pEngine)
{
	PT_Assert( GBEngine_IsOpened(pEngine) );
	return pEngine->g_gbis.pAuxInfo->nSelRow;
}
/*!
 * \brief 获取当前页有效的行数 GBMMI使用
 * \param pEngine 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-2 10:19:59
*/
GBU8 GBEngine_GetRowCount(PCGBEngine pEngine)
{
	PT_Assert( GBEngine_IsOpened(pEngine) );
	return pEngine->g_gbis.pAuxInfo->nRowCount;
}

/*!
 * \brief 判断国笔输入法引擎当前是否处于MultiTap输入模式
 * \param pEngine 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-2 10:19:59
*/
GBBOOL GBEngine_IsMultiTapInputMode(PCGBEngine pEngine)
{
	PT_Assert( GBEngine_IsOpened(pEngine) );
	return (GBBOOL)GBIS_IsMultiTapInputMode(&pEngine->g_gbis);
}

/*!
 * \brief 返回引擎大小写状态标记
 * \param pEngine 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-14 17:58:24
*/
GBShiftCapStatus GBEngine_GetShiftCapStatus(PCGBEngine pEngine)
{
	PT_Assert( GBEngine_IsOpened(pEngine) );

	if (pEngine->g_gbis.outputInfo.nStatusFlag & GBSF_Shift)
	{
		return GBStatus_Shift;
	}
	else if (pEngine->g_gbis.outputInfo.nStatusFlag & GBSF_Caplock)
	{
		return GBStatus_Caplock;
	}
	else
	{
		return GBStatus_Normal;
	}
}

/*!
 * \brief 退出选择状态[数字上屏时需要确保输入栏处于焦点选择状态]
 * \param pEngine 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-20 14:14:11
*/
GBRETURN GBEngine_ExitSelectedStatus(PCGBEngine pEngine)
{
	GBU16 cur_state = GBEngine_GetCurStatus(pEngine);
	GBRETURN ret = GBOK;
	
	PT_Assert( GBEngine_IsOpened(pEngine) );
	
	if( cur_state == GBIMS_SELECTED )
	{
		ret = _input_key_event(pEngine, GBKEY_BACK, 0);
	}
	return ret;
}

/*!
 * \brief 设置国笔输入法引擎的控制键状态，仅对alphabetic语言有效
 * \param pEngine 
 * \param nCandType 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-8-17 10:18:54
*/
GBRETURN GBEngine_SetShiftCap(PCGBEngine pEngine, GBINT bShiftDown, GBINT bCapsLockToggle)
{
	PT_Assert( GBEngine_IsOpened(pEngine) );
	return GBSetShiftCap(&pEngine->g_gbis, bShiftDown, bCapsLockToggle);
}

/*!
 * \brief 判断是否是可以切换输入模式状态
 * \param pEngine 
 * \return 
 * \note 用于物理键盘
 * \example
 * \author Zhaokun
 * \date 2010-10-23 14:18:54
*/
GBBOOL GBEngine_IsNeedSwitchInputModeState(PCGBEngine pEngine)
{
	GBBOOL ret = GBFalse;
	PT_Assert( GBEngine_IsOpened(pEngine) );

	if(GBIS_IS_CAND_OPEN(&pEngine->g_gbis) && pEngine->g_gbis.nCandType != GBCT_Associate)
	{
		ret = GBFalse;
	}
	else
	{
		ret = GBTrue;
	}	

	return ret;
}

/*!
 * \brief 国笔MultiTap确认回调函数
 * \param void 
 * \return 
 * \note 
 * \example
 * \author Zhaokun
 * \date 2010-10-23 14:18:54
*/
void GBEngine_GBRequestTimer_CallBack(void)
{
	_input_key_event(Global_GetEnineInstance(), GBKEY_OK, 0);
	//PT_GUICancelTimer(GBEngine_GBRequestTimer_CallBack);	
}

/*!
 * \brief 国笔MultiTap定时器启动函数
 * \param pOEMPrivateData 
 * \param nTimerType 
 * \return 
 * \note 
 * \example
 * \author Zhaokun
 * \date 2010-10-23 14:18:54
*/
GBU32 GBEngine_GBRequestTimer_t(GBLPVOID pOEMPrivateData, GBINT nTimerType)
{
	TIMER_CALLBACK callBack = (TIMER_CALLBACK)pOEMPrivateData;

	PT_StartTimer(MULTITAP_TIMER_ID, 1000, callBack);
	return MULTITAP_TIMER_ID;
}

/*!
 * \brief 国笔MultiTap定时器停止函数
 * \param pOEMPrivateData 
 * \param nTimerID 
 * \return 
 * \note 
 * \example
 * \author Zhaokun
 * \date 2010-10-23 14:18:54
*/
GBU32 GBEngine_GBRequestKillTimer_t(GBLPVOID pOEMPrivateData, GBU32 nTimerID)
{
	TIMER_CALLBACK callBack = (TIMER_CALLBACK)pOEMPrivateData;
	PCGBEngine pEngine = Global_GetEnineInstance();
	PCLayoutAbstract pltPreObject = Global_GetCurrentLayoutObject();
// 	if(pEngine->g_gbis.outputInfo.pUpScreenStr)
// 	{
// 		pEngine->g_gbis.outputInfo.pUpScreenStr[0] = 0;
// 	}
// 	if(!pEngine->g_gbis.pAuxInfo->pCompStr)
// 	{
// 		pltPreObject->bMultitapTimerStartFlag = GBFalse;
// 		pltPreObject->multitapReplaceCandLength = 0;
// 	}
	
// 	GBInputBox_ClearHilight();
// 	GBInputBox_Redraw();
	PT_StopTimer((GBU16)nTimerID);
	
	return GBOK;
}

/*!
 * \brief 国笔MultiTap定时器绑定函数
 * \param pEngine 
 * \param requestTimer 
 * \param killTimer 
 * \param pOEMPrivateData 
 * \return 
 * \note 
 * \example
 * \author Zhaokun
 * \date 2010-10-23 14:18:54
*/
GBRETURN GBEngine_AttachTimerFunction(PCGBEngine pEngine, 
									  fpGBRequestTimer_t requestTimer,
									  fpGBRequestKillTimer_t killTimer,
									  GBLPVOID pOEMPrivateData)
{
	PT_Assert( GBEngine_IsOpened(pEngine) );
	return GBMMIAttachTimerFunction(&pEngine->g_gbis, requestTimer, killTimer, pOEMPrivateData);
}

/*!
 * \brief 设置候选框候选行数
 * \param pEngine 
 * \param nInputCandRowCount 输入状态下候选行数
 * \param nAssociateCandRowCount 联想状态下候选行数
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-11-11 19:37:20
 */
GBRETURN GBEngine_SetCandRowCount(PCGBEngine pEngine, GBINT nInputCandRowCount, GBINT nAssociateCandRowCount)
{
	GBRETURN ret;
	ret = GBMMISetCandRowCount(&pEngine->g_gbis, GBCS_INPUT, nInputCandRowCount);
	ret = GBMMISetCandRowCount(&pEngine->g_gbis, GBCS_ASSOCIATE, nAssociateCandRowCount);
	return ret;
}

/*!
 * \brief 检查引擎当前的输入是否为空
 * \param 
 * \return GBTrue/GBFalse
 * \note
 * \author weizhiping
 * \date 2009年12月11日
 */
GBBOOL GBEngine_IsInputEmpty(PCGBEngine pEngine)
{
	if(pEngine->g_gbis.pAuxInfo->pOutputString == NULL
	   || pEngine->g_gbis.pAuxInfo->pOutputString[0] == 0)
	{
		return GBTrue;
	}
	else
	{
		return GBFalse;
	}
}