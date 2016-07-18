/***************************************************************************
* File Name    :  CHWEngine.h
* Corporation  :  Guobi Technology Ltd. GuangZhou China
* Copyright    :  Copyright Guobi Technology Ltd. And Subject to Confidentiality 
*                 Restrictions. This document is the confidential and proprietary 
*                 property of Guobi Technology Ltd.It is neither the document                  
*                 itself nor any part of it can be shared with any party  without 
*                 the written  permission. 
* Author       :  huanjin
* Date         :  2010-8-4
* Purpose      :  封装国笔手写引擎
* Version      :  V1.0
* Last Changed List
* Name         |   date   |  Discription
****************************************************************************/
#ifndef __GUOBIHANDWRITE_H_
#define __GUOBIHANDWRITE_H_
#include "gbhw.h"
#include "gbtype.h"
#include "GBIMEDef.h"
#include "gbft_def.h"

#if defined(__GBIME_HANDWRITE__)

typedef int (*pfnGetWStringWidth_t)(void * pParam, GBLPCWCHAR pWString);

/*!
 * \brief 手写类型定义
 */
typedef enum {
	GBIME_HANDWRITE_TYPE_NONE = 0,
	GBIME_HANDWRITE_TYPE_TRADITIONAL_CHINESE,
	GBIME_HANDWRITE_TYPE_SIMPLIFIED_CHINESE,
	GBIME_HANDWRITE_TYPE_UPPER_CASE_ENGLISH,
	GBIME_HANDWRITE_TYPE_LOWER_CASE_ENGLISH,
	GBIME_HANDWRITE_TYPE_NUMBER,
	GBIME_HANDWRITE_TYPE_PUNCTUATION,
	GBIME_HANDWRITE_TYPE_ALL,
	GBIME_HANDWRITE_TYPE_DEFAULT
}GBIMEHandwriteTypeEnum;

/*!
 * \brief 手写引擎删除控制字符
 */
#define GBIME_HANDWRITE_DELETE_CTL_CHAR	0x000B

typedef struct
{
	GBBOOL  	open_flags;								///<手写引擎打开标志		
	GBU32  		dwRange;								///<手写引擎识别范围设置
	GBBOOL 		bHomonymEnable; 						///<手写识别范围设置为同音
	GBBOOL      bEnglishEnable;						    ///<手写识别范围设置为英文
	GBBOOL      bNumberEnable;							///<手写识别范围设置为数字
	//GBWCHAR  	cand_buf[GBIME_DEF_MAX_CAND_WORD][2]; 	///<手写引擎候选缓冲区
	GBPCWCHAR  	pCandidates[GBIME_DEF_MAX_CAND_WORD]; 	///<手写引擎候选缓冲区指针
	GBU16  		cand_count;								///<手写引擎候选个数	
}GBHWEngine, *PGBHWEngine;

/*!
 * \brief 初始化国笔手写输入法引擎
 * \param pGBHWEngine 
 * \param lang_data 手写输入法引擎数据 
 * \return 
 * \note 调用任何国笔手写引擎API之前必须先执行该函数
 * \example
 * \author weizhiping
 * \date 2010-9-7 16:40:01
*/
GBIMEReturn GBHW_Initialize(PGBHWEngine pGBHWEngine, const unsigned long * lang_data);

/*!
 * \brief 手写引擎退出(暂时只需用来重置变量)
 * \param pGBHWEngine
 * \return 
 * \note
 * \author haj
 * \date 2010-8-5
 */
void GBHW_Terminate(PGBHWEngine pGBHWEngine);

/*!
 * \brief 手写引擎重置
 * \param pGBHWEngine 
 * \return 
 * \note 用于清除识别候选
 * \example
 * \author weizhiping
 * \date 2010-9-7 16:37:29
*/
void GBHW_Reset(PGBHWEngine pGBHWEngine);

/*!
 * \brief 设置国笔手写输入法引擎的识别范围
 * \param pGBHWEngine
 * \param dwRange GBHW_OPT_RANGE_XXX 的组合
 * \return 
 * \note
 * \author weizhiping
 * \date 2010-8-10
 */
GBIMEReturn GBHW_SetRange(PGBHWEngine pGBHWEngine, GBU32 dwRange);

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
int GBHW_SetOption(int index, GBUINT32 value);

/*!
 * \brief 判断手写引擎是否打开
 * \param pGBHWEngine
 * \return
 * \note
 * \author haj
 * \date 2010-8-5
 */
GBBOOL GBHW_IsOpened(const PGBHWEngine pGBHWEngine);

/*!
 * \brief 返回识别结果的个数
 * \param pGBHWEngine
 * \return 返回识别结果的个数
 * \note
 * \author haj
 * \date 2010-8-5
 */
GBU16 GBHW_GetCandCount(PGBHWEngine pGBHWEngine);

/*!
 * \brief 根据手写候选Buffer指针
 * \param pGBHWEngine
 * \return
 * \note
 * \author haj
 * \date 2010-8-5
 */
GBPCWCHAR * GBHW_GetCandBuffer(PGBHWEngine pGBHWEngine);

/*!
 * \brief 根据索引返回候选
 * \param pGBHWEngine
 * \param idx 候选索引
 * \return
 * \note
 * \author haj
 * \date 2010-8-5
 */
GBPCWCHAR GBHW_GetCandByIndex(PGBHWEngine pGBHWEngine, GBU16 idx);

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
void GBHW_RecognizeFinishedCallback(GBPCWCHAR  pCandBuffer, GBU16 cand_count);

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
GBIMEReturn GBHW_ConvertHandwriteTypeToRange(GBIMEHandwriteTypeEnum handwriteType, GBU32 *dwRange);

/*!
 * \brief 手写消息处理函数
 * \param pIMEEvent 
 * \return 
 */
GBIMEReturn GBHW_HandleMessage(PGBIMEEvent pIMEEvent);

/*!
 * \brief 获取同音字状态
 * \param pGBHWEngine 
 * \return 
 */
GBBOOL GBHW_GetHomonymStatus(PGBHWEngine pGBHWEngine);

/*!
 * \brief 设置同音字状态
 * \param pGBHWEngine 
 * \param status 
 * \return 
 */
GBIMEReturn GBHW_SetHomonymStatus(PGBHWEngine pGBHWEngine, GBBOOL status);

/*!
 * \brief 获取英文识别状态
 * \param pGBHWEngine 
 * \return 
 */
GBBOOL GBHW_GetEnglishStatus(PGBHWEngine pGBHWEngine);

/*!
 * \brief 设置英文识别状态
 * \param pGBHWEngine 
 * \param status 
 * \return 
 */
GBIMEReturn GBHW_SetEnglishStatus(PGBHWEngine pGBHWEngine, GBBOOL status);

/*!
 * \brief 获取数字识别状态
 * \param pGBHWEngine 
 * \return 
 */
GBBOOL GBHW_GetNumberStatus(PGBHWEngine pGBHWEngine);

/*!
 * \brief 设置数字识别状态
 * \param pGBHWEngine 
 * \param status 
 * \return 
 */
GBIMEReturn GBHW_SetNumberStatus(PGBHWEngine pGBHWEngine, GBBOOL status);

#endif

#endif //__GUOBIHANDWRITE_H_
