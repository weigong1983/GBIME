/***************************************************************************
* File Name    :  PlatformAdapter.h
* Corporation  :  Guobi Technology Ltd. GuangZhou China
* Copyright    :  Copyright Guobi Technology Ltd. And Subject to Confidentiality 
*                 Restrictions. This document is the confidential and proprietary 
*                 property of Guobi Technology Ltd.It is neither the document                  
*                 itself nor any part of it can be shared with any party  without 
*                 the written  permission. 
* Author       :  weizhiping
* Date         :  2010-06-08
* Purpose      :  封装MTK平台IMC、IMM、IMUI、PEN等输入法及编辑器相关的函数，提供给MTKIME框架使用(国笔引擎封装API在: CGBEngine.h)
* Version      :  V1.0
* Last Changed List
* Name         |   date   |  Discription
****************************************************************************/
#ifndef __PLATFORM_ADAPTER__
#define __PLATFORM_ADAPTER__

#include "gbtype.h"
#include "GBIMEDef.h"
#include "gbft_def.h"
#include "CIMEInterface.h"

#ifndef __MTK__
#define __MTK__           1
#endif

#if __MTK__ > 0 // MTK平台依赖头文件
	#include "Imc.h"
	#include "Timerevents.h"
#endif

#if __MTK__ > 0 
///<平台定义的定时器ID
#define MULTITAP_TIMER_ID					GBIME_MULTITAP_TIMER				///<Multitap定时器ID
#define HANDWRITE_RECOGNIZE_TIMER			GBIME_HW_RECOGNIZE_TIMER			///<手写识别开始定时器ID
#define HANDWRITECANDIDATE_CONFIRM_TIMER    GBIME_HW_CANDIDATE_CONFIRM_TIMER	///<手写识别候选确认上屏定时器ID
#endif

typedef void (*GbFuncPtr)(void);

/*!
 * \brief 根据平台输入法模式获取目标Layout ID
 * \param platformInputMode
 * \param pLayoutID 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-8-4 16:24:17
 */
GBIMEReturn GBPA_GetLayoutIDByPlatformInputMode(GBINT platformInputMode, GBUINT16 *pLayoutID);

/*!
 * \brief 根据目标切换Layout 的ID找到对应的平台输入法模式
 * \param layoutID 
 * \param pPlatformInputMode 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-8-4 16:24:17
 */
GBIMEReturn GBPA_GetPlatformInputModeByLayoutID(GBUINT16 layoutID, GBINT *pPlatformInputMode);

/*!
 * \brief 根据当前Layout ID获取下一个Layout ID
 * \param nCurLayoutID
 * \param pNextLayoutID 
 * \return 
 * \note 当mapCount > 1时，优先选择国笔输入法配置缺省输入法应该指定的LayoutID
 *\example
 * \author ZhaoKun
 * \date 2010-10-25 16:24:17
 */
GBIMEReturn GBPA_GetNextLayoutIDByCurLayoutID(GBUINT16 nCurLayoutID, GBUINT16 *pNextLayoutID);

#if __MTK__ > 0 // MTK平台参考函数
/*!
 * \brief 高亮候选
 * \param GBUACHAR 
 * \return void
 */
GBIMEGLOBAL void GBPA_PenConfirmHandwritingSelection(void);

/*!
 * \brief multitap的字母up处理
 * \param mmi_imc_key_custom_value,GBU16 
 * \return GBI
 * \注意：mmi_imc_key_multitap_alphabetic_up_handler在imc_key中是static的，要把核心代码copy到这里。
 */
GBIMEGLOBAL GBI GBPA_KeyMultitapAlphabeticUpHandler(mmi_imc_key_custom_value key_code, GBU16 event_type);

/*!
 * \brief multitap的字母down处理
 * \param mmi_imc_key_custom_value,GBU16 
 * \return GBI
 * \注意：mmi_imc_key_multitap_alphabetic_down_handler在imc_key中是static的，要把核心代码copy到这里。
 */
GBIMEGLOBAL GBI GBPA_KeyMultitapAlphabeticDownHandler(mmi_imc_key_custom_value key_code, GBU16 event_type);

/*!
 * \brief 全屏手写
 * \param void
 * \return void
 * \注意：
 */
GBIMEGLOBAL void GBPA_EditorEnableFullScreenHandwriting(void);

/*!
 * \brief 设置状态
 * \param mmi_imc_state_enum
 * \return void
 * \注意：
 */
GBIMEGLOBAL void GBPA_SetState( mmi_imc_state_enum state);

/*!
 * \brief 清理手写
 * \param 
 * \return void
 * \注意：
 */
GBIMEGLOBAL GBU32 GBPA_ClearHandWritingStroke(void);

/*!
* \brief 进入新屏幕
* \param GBWCHAR, GbFuncPtr,GbFuncPtr,GBPVOID
* \param rect 
* \return GBUACHAR
*/
GBIMEGLOBAL GBUACHAR GBPA_EntryNewScreen(GBWCHAR newscrnID, GbFuncPtr newExitHandler, GbFuncPtr newEntryHandler, GBPVOID flag);

#endif // __MTK__  // MTK平台参考函数
#endif // __PLATFORM_ADAPTER__
