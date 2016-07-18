/***************************************************************************
* File Name    :  CHWEngine.c
* Corporation  :  Guobi Technology Ltd. GuangZhou China
* Copyright    :  Copyright Guobi Technology Ltd. And Subject to Confidentiality 
*                 Restrictions. This document is the confidential and proprietary 
*                 property of Guobi Technology Ltd.It is neither the document                  
*                 itself nor any part of it can be shared with any party  without 
*                 the written  permission. 
* Author       :  weizhiping
* Date         :  2010-06-08
* Purpose      :  封装国笔手写引擎
* Version      :  V1.0
* Last Changed List
* Name         |   date   |  Discription
****************************************************************************/
#include "CHWEngine.h"
#include "PlatformTools.h"
#include "CIMEInterface.h"
#include "gbft_def.h"
#include "GB_Global.h"

#if __MTK__ > 0
#include "MMI_features.h"
#include "kal_non_specific_general_types.h"
#include "gui_data_types.h"
#include "ImeGprot.h"
#include "Handwriting_engine.h"
#endif

#if defined(__GBIME_HANDWRITE__)

static GBBOOL GBHW_UpdateCandidates(PGBHWEngine pGBHWEngine, GBPCWCHAR pCandBuffer, GBU16 cand_count);

/*!
 * \brief 初始化国笔手写输入法引擎
 * \param phwData 手写输入法引擎数据 
 * \return 
 * \note 调用任何国笔手写引擎API之前必须先执行该函数
 * \author haj
 * \date 2010-8-5
 */
GBIMEReturn GBHW_Initialize(PGBHWEngine pGBHWEngine, const unsigned long * lang_data)
{
	GBIMEReturn ret;
	
	if (GBHW_IsOpened(pGBHWEngine))
	{
		return GBIME_OK;
	}	

	// 记得初始化
	memset(pGBHWEngine, 0, sizeof(GBHWEngine));
	ret = GBHWNew(lang_data,0);
	PT_Assert(ret == 0);
	pGBHWEngine->open_flags = GBTrue;
	return GBIME_OK;
}

/*!
 * \brief 手写引擎退出(暂时只需用来重置变量)
 * \param pGBHWEngine
 * \return 
 * \note
 * \author haj
 * \date 2010-8-5
 */
void GBHW_Terminate(PGBHWEngine pGBHWEngine)
{
	pGBHWEngine->cand_count= 0;
	pGBHWEngine->open_flags = 0x0;
	pGBHWEngine->dwRange = 0x0;
}

/*!
 * \brief 手写引擎重置
 * \param pGBHWEngine 
 * \return 
 * \note 用于清除识别候选
 * \example
 * \author weizhiping
 * \date 2010-9-7 16:37:29
*/
void GBHW_Reset(PGBHWEngine pGBHWEngine)
{
	GBINT i;

	//pGBHWEngine->dwRange = 0x0; 识别范围不能清除
	//pGBHWEngine->bHomonymEnable = GBFalse;
	//pGBHWEngine->bEnglishEnable = GBFalse;
	//memset(pGBHWEngine->cand_buf, 0, sizeof(pGBHWEngine->cand_buf));
	pGBHWEngine->cand_count= 0;

	for (i=0; i<GBIME_DEF_MAX_CAND_WORD; i++)
	{
		pGBHWEngine->pCandidates[i] = NULL;
	}	
}

/*!
 * \brief 设置国笔手写输入法引擎的识别范围
 * \param pGBHWEngine
 * \param dwRange GBHW_OPT_RANGE_XXX 的组合
 * \return 
 * \note
 * \author weizhiping
 * \date 2010-8-10
 */
GBIMEReturn GBHW_SetRange(PGBHWEngine pGBHWEngine, GBU32 dwRange)
{
	PT_Assert(GBHW_IsOpened(pGBHWEngine));
	pGBHWEngine->dwRange = dwRange;
	return GBIME_OK;
}

/*!
 * \brief 获取国笔手写输入法引擎的识别范围
 * \param pGBHWEngine
 * \return 
 * \note
 * \author weizhiping
 * \date 2010-8-10
 */
GBU32 GBHW_GetRange(PGBHWEngine pGBHWEngine)
{
	PT_Assert(GBHW_IsOpened(pGBHWEngine));
	return pGBHWEngine->dwRange;
}

/*!
 * \brief 设置识别引擎识别参数
 * \param index 范围是GB_HW_Option_Index里面的值
 * \param value 根据index定义不同，详细请参照每一个index的说明
 * \return 如果index为非法值，则返回GBHW_Err_Option_Invalid_Index 
 * -    成功返回GBHW_OK
 * -    其他返回值请查看每一个错误值的说明
 * \note
 * \author haj
 * \date 2010-8-5
 */
int GBHW_SetOption(int index, GBUINT32 value)
{
	return GBHWSetOption(index, value);
}

/*!
 * \brief 判断手写引擎是否打开
 * \param pGBHWEngine
 * \return
 * \note
 * \author haj
 * \date 2010-8-5
 */
GBBOOL GBHW_IsOpened(const PGBHWEngine pGBHWEngine)
{
	return pGBHWEngine->open_flags;
}

/*!
 * \brief 更新手写候选
 * \param pGBHWEngine 
 * \param pCandBuffer 
 * \param cand_count 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-12-1 14:52:41
*/
static GBBOOL GBHW_UpdateCandidates(PGBHWEngine pGBHWEngine, GBPCWCHAR pCandBuffer, GBU16 cand_count)
{
	GBU16 i = 0;
	GBINT cand_len = 0;
	GBPCWCHAR pTemp = pCandBuffer;

	if (pCandBuffer == NULL || cand_count == 0)
	{
		return GBFalse;
	}

	// 更新候选相关变量
	pGBHWEngine->cand_count = cand_count;
	for(i = 0; i < cand_count; ++i)
	{		
		pGBHWEngine->pCandidates[i] = pTemp;
		cand_len = wcslen(pTemp);
		pTemp += (cand_len + 1);
	}

	return GBTrue;
}

/*!
 * \brief 返回识别结果的个数
 * \param pGBHWEngine
 * \return 返回识别结果的个数
 * \note 必须调用GBHW_UpdateCandidates更新手写候选
 * \author haj
 * \date 2010-8-5
 */
GBU16 GBHW_GetCandCount(PGBHWEngine pGBHWEngine)
{
	PT_Assert(GBHW_IsOpened(pGBHWEngine));
	return pGBHWEngine->cand_count;
}

/*!
 * \brief 根据手写候选Buffer指针
 * \param pGBHWEngine
 * \return
 * \note
 * \author haj
 * \date 2010-8-5
 */
GBPCWCHAR * GBHW_GetCandBuffer(PGBHWEngine pGBHWEngine)
{
	PT_Assert(GBHW_IsOpened(pGBHWEngine));
	return pGBHWEngine->pCandidates;
}

/*!
 * \brief 根据索引返回候选
 * \param pGBHWEngine
 * \param idx 候选索引
 * \return
 * \note 必须先调用GBHW_UpdateCandidates更新手写候选
 * \author haj
 * \date 2010-8-5
 */
GBPCWCHAR GBHW_GetCandByIndex(PGBHWEngine pGBHWEngine, GBU16 idx)
{
	PT_Assert(GBHW_IsOpened(pGBHWEngine));
	
	if (!(idx >= 0 && idx < GBHW_GetCandCount(pGBHWEngine)))
	{
		PT_PrintLogInfo(GBLOG_HANDWRITE, ("GBHW_GetCandByIndex 非法索引: [%d] GBHW_GetCandCount=[%d]\r\n", idx, GBHW_GetCandCount(pGBHWEngine)));
		return NULL;
	}
	
	return (GBPCWCHAR)pGBHWEngine->pCandidates[idx];
}

/*!
 * \brief 手写引擎识别结束回调(发送消息通知手写Layout绘制手写引擎识别出来的候选)
 * \param pCandBuffer 
 * \param cand_count 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-12-1 14:55:46
*/
void GBHW_RecognizeFinishedCallback(GBPCWCHAR pCandBuffer, GBU16 cand_count)
{
	GBIMEEvent imeEvent;
	GBPCWCHAR pCand = NULL;
	
	// 更新手写候选
	GBHW_UpdateCandidates(Global_GetHWEnineInstance(), pCandBuffer, cand_count);
	
	// 获取第一个候选的联想词
	pCand = GBHW_GetCandByIndex(Global_GetHWEnineInstance(), 0);
	
	// 在屏上向左划一横，引擎将识别为"删除"命令，可以删除输入上屏的最后一个字符
	if(pCand != NULL && *pCand == GBIME_HANDWRITE_DELETE_CTL_CHAR)
	{
		GBInputBox_DeleteChar();
	}
	else
	{
		imeEvent.nType = GBIME_EVENT_TYPE_HANDWRITE;
		imeEvent.nEvent = GBIME_EVENT_HANDWRITE_DRAW_CANDIDATE;
		Handle_GBIMEMessage(&imeEvent);

		if (pCand != NULL)
		{
		
			if (GBHW_GetHomonymStatus(Global_GetHWEnineInstance()))
			{
				imeEvent.nType = GBIME_EVENT_TYPE_ENGINE;
				imeEvent.nEvent = GBIME_EVENT_ENGINE_GET_HOMONYMWORD;
				imeEvent.wParam = (GBINT)pCand;
				Handle_GBIMEMessage(&imeEvent);
			}
			else
			{
				imeEvent.nType = GBIME_EVENT_TYPE_ENGINE;
				imeEvent.nEvent = GBIME_EVENT_ENGINE_GET_ASSOCWORD;
				imeEvent.wParam = (GBINT)pCand;
				Handle_GBIMEMessage(&imeEvent);
			}

			imeEvent.nType = GBIME_EVENT_TYPE_HANDWRITE;
			imeEvent.nEvent = GBIME_EVENT_HANDWRITE_DRAW_ASSOCWORD;
			Handle_GBIMEMessage(&imeEvent);
		}
	}

	imeEvent.nType = GBIME_EVENT_TYPE_LAYOUT;
	imeEvent.nEvent = GBIME_EVENT_LAYOUT_REFRESH;
	Handle_GBIMEMessage(&imeEvent);

}

/*!
 * \brief 根据手写类型获取手写引擎识别范围参数
 * \param handwriteType 
 * \param dwRange 输出
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-8-12 15:10:47
*/
GBIMEReturn GBHW_ConvertHandwriteTypeToRange(GBIMEHandwriteTypeEnum handwriteType, GBU32 *dwRange)
{
	switch (handwriteType)
	{
	case GBIME_HANDWRITE_TYPE_SIMPLIFIED_CHINESE:
		*dwRange = GBHW_OPT_RANGE_DEFAULT | GBHW_OPT_RANGE_CONTROL_CHAR;     
		break;
	case GBIME_HANDWRITE_TYPE_TRADITIONAL_CHINESE:
		*dwRange = GBHW_OPT_RANGE_DEFAULT | GBHW_OPT_RANGE_CONTROL_CHAR;
		break;
	case GBIME_HANDWRITE_TYPE_LOWER_CASE_ENGLISH:
		*dwRange = GBHW_OPT_RANGE_ASCII | GBHW_OPT_RANGE_CONTROL_CHAR;
		break;
	case GBIME_HANDWRITE_TYPE_UPPER_CASE_ENGLISH:
		*dwRange = GBHW_OPT_RANGE_ASCII | GBHW_OPT_RANGE_CONTROL_CHAR;
		break;
	case GBIME_HANDWRITE_TYPE_NUMBER:
		*dwRange = GBHW_OPT_RANGE_NUMBER | GBHW_OPT_RANGE_CONTROL_CHAR;
		break;
	case GBIME_HANDWRITE_TYPE_PUNCTUATION:
		*dwRange = GBHW_OPT_RANGE_PUNCTUATION;
		break;
	case GBIME_HANDWRITE_TYPE_ALL:
		*dwRange = GBHW_OPT_RANGE_ALL;
		break;
	default:
		*dwRange = GBHW_OPT_RANGE_DEFAULT;
		break;
	}

	return GBIME_OK;
}

/*!
 * \brief 手写消息处理函数
 * \param pIMEEvent 
 * \return 
 */
GBIMEReturn GBHW_HandleMessage(PGBIMEEvent pIMEEvent)
{
	GBIMEReturn ret = GBIME_OK;

	if (pIMEEvent->nType != GBIME_EVENT_TYPE_HANDWRITE)
	{
		return GBIME_IGNORE; // 非手写事件，忽略
	}

	switch (pIMEEvent->nEvent)
	{
	case GBIME_EVENT_HANDWRITE_ENGINE_INIT: ///<手写引擎初始化
		// 初始化手写引擎和手写轨迹管理器
		//ret = GBHW_Initialize(Global_GetHWEnineInstance(), Global_GetHWEngineData());
		//GBHW_SetRange(Global_GetHWEnineInstance(), GBHW_OPT_RANGE_DEFAULT);
		break;
	case GBIME_EVENT_HANDWRITE_ENGINE_EXIT: ///<手写引擎退出
		// 销毁手写引擎和手写轨迹管理器
		//GBHW_Terminate(Global_GetHWEnineInstance());
		break;
	case GBIME_EVENT_HANDWRITE_DRAW_CANDIDATE: ///<绘制手写识别出来的候选
	case GBIME_EVENT_HANDWRITE_DRAW_ASSOCWORD: ///<绘制联想字符串
		{
			PCLayoutAbstract pltObj = Global_GetCurrentLayoutObject();
			
			// Layout消息处理
			if (pltObj->m_pfcHandleEvent != NULL)
			{
				pltObj->m_pfcHandleEvent(pltObj, pIMEEvent, CLayoutControl_GetEngineOutputInfo());
			}
		}
		break;
	case GBIME_EVENT_HANDWRITE_SET_RECOGNIZE_RANGE: ///<设置识别范围
		{
			GBU32 dwRange;
			GBIMEHandwriteTypeEnum handwriteType = pIMEEvent->wParam;
			GBHW_ConvertHandwriteTypeToRange(handwriteType, &dwRange);
			GBHW_SetRange(Global_GetHWEnineInstance(), dwRange);
		}		 
		break;
	default:
		PT_Assert(0); // 非法事件
	}

	return ret;
}

/*!
 * \brief 获取同音字状态
 * \param pGBHWEngine 
 * \return 
 */
GBBOOL GBHW_GetHomonymStatus(PGBHWEngine pGBHWEngine)
{
	PT_Assert(GBHW_IsOpened(pGBHWEngine));
	return pGBHWEngine->bHomonymEnable;
}

/*!
 * \brief 设置同音字状态
 * \param pGBHWEngine 
 * \param status 
 * \return 
 */
GBIMEReturn GBHW_SetHomonymStatus(PGBHWEngine pGBHWEngine, GBBOOL status)
{
	PT_Assert(GBHW_IsOpened(pGBHWEngine));
	pGBHWEngine->bHomonymEnable = status;
	return GBIME_OK;
}

/*!
 * \brief 获取英文识别状态
 * \param pGBHWEngine 
 * \return 
 */
GBBOOL GBHW_GetEnglishStatus(PGBHWEngine pGBHWEngine)
{
	PT_Assert(GBHW_IsOpened(pGBHWEngine));
	return pGBHWEngine->bEnglishEnable;
}

/*!
 * \brief 设置英文识别状态
 * \param pGBHWEngine 
 * \param status 
 * \return 
 */
GBIMEReturn GBHW_SetEnglishStatus(PGBHWEngine pGBHWEngine, GBBOOL status)
{
	PT_Assert(GBHW_IsOpened(pGBHWEngine));
	pGBHWEngine->bEnglishEnable = status;
	return GBIME_OK;
}

/*!
 * \brief 获取数字识别状态
 * \param pGBHWEngine 
 * \return 
 */
GBBOOL GBHW_GetNumberStatus(PGBHWEngine pGBHWEngine)
{
	PT_Assert(GBHW_IsOpened(pGBHWEngine));
	return pGBHWEngine->bNumberEnable;
}

/*!
 * \brief 设置数字识别状态
 * \param pGBHWEngine 
 * \param status 
 * \return 
 */
GBIMEReturn GBHW_SetNumberStatus(PGBHWEngine pGBHWEngine, GBBOOL status)
{
	PT_Assert(GBHW_IsOpened(pGBHWEngine));
	pGBHWEngine->bNumberEnable = status;
	return GBIME_OK;
}

/*!
* \defgroup 如下API是封装给MTK平台手写模块调用的
*/

/*!
 * \brief 初始化国笔手写引擎(封装给MTK平台调用)
 * \param hw_type 
 * \return 
 * \note 一般建议开机时候初始化
 * \example
 * \author weizhiping
 * \date 2010-12-1 17:46:13
*/
void mmi_ime_hand_writing_initialize(mmi_imc_pen_handwriting_type_enum hw_type)
{
	// 初始化手写引擎
	GBHW_Initialize(Global_GetHWEnineInstance(), Global_GetHWEngineData());
	GBHW_SetRange(Global_GetHWEnineInstance(), GBHW_OPT_RANGE_DEFAULT);
}

/*!
 * \brief 手写引擎退出
 * \param void 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-12-1 17:48:39
*/
void mmi_ime_hand_writing_deinitialize(void)
{
	GBHW_Terminate(Global_GetHWEnineInstance());
}

/*!
 * \brief 调用底层首席引擎得到候选
 * \param query_param 
 * \param result_param 
 * \return 
 * \note 
 * \example 在imc_pen.c中被调用
 * \author weizhiping
 * \date 2010-12-1 17:49:03
*/
S32 mmi_ime_hand_writing_get_candidates(mmi_ime_hand_writing_query_param_p query_param,
										mmi_ime_hand_writing_result_param_p result_param)
{
    S32 num = 0;
	unsigned long recognizeRange = GBHW_GetRange(Global_GetHWEnineInstance());

    num = GBHWRecognize((short*)(query_param->stroke_buffer),   /*IN  用户输入的笔迹，注意结束为:-1,0,-1,-1*/
    				 result_param->candidate_array ,			/*OUT 得到候选后放在该buffe中r*/
    				 query_param->desired_cnt,					/*IN 要求得到识别候选个数*/
    				 recognizeRange);							/*识别范围*/
    result_param->result_cnt = num;
    return num;
}

/*!
 * \brief 查询手写识别类型
 * \param h_w_type 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-12-1 17:52:08
*/
MMI_BOOL mmi_ime_hand_writing_hw_type_query(mmi_imc_pen_handwriting_type_enum h_w_type)
{
    return MMI_TRUE;
}

/*!
 * \brief 添加分隔符到手写候选缓冲区【默认为结束符'\0'】
 * \param destination_buffer 
 * \param source_buffer 
 * \param num 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-12-1 17:50:42
*/
void  mmi_ime_hand_writing_add_separator_to_candidate_buffer(UI_character_type * destination_buffer, UI_character_type * source_buffer, S32 num)
{
    S32 i = 0, j = 0;
    
    while (i < num && source_buffer[i] != 0)
    {
        destination_buffer[j] = source_buffer[i];
        destination_buffer[j + 1] = 0; /* Default separator is '\0' */
        i++;
        j += 2;
    }

    destination_buffer[j] = 0;
}

#if defined(__MMI_HANDWRITING_PHRASE__)
/*!
 * \brief 根据词语获取候选
 * \param query 
 * \param result 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-12-1 17:51:50
*/
S32 mmi_ime_hand_writing_get_candidates_by_phrase(
    mmi_ime_hand_writing_phrase_query_param_p query, 
    mmi_ime_hand_writing_phrase_result_param_p result)
{
    result.result_cnt = 0;
    result.next_page = MMI_FALSE;
    return 0;
}
#endif

#endif // defined(__GBIME_HANDWRITE__)