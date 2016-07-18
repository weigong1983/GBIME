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
* Purpose      :  ��װMTKƽ̨IMC��IMM��IMUI��PEN�����뷨���༭����صĺ������ṩ��MTKIME���ʹ��(���������װAPI��: CGBEngine.h)
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

#if __MTK__ > 0 // MTKƽ̨����ͷ�ļ�
	#include "Imc.h"
	#include "Timerevents.h"
#endif

#if __MTK__ > 0 
///<ƽ̨����Ķ�ʱ��ID
#define MULTITAP_TIMER_ID					GBIME_MULTITAP_TIMER				///<Multitap��ʱ��ID
#define HANDWRITE_RECOGNIZE_TIMER			GBIME_HW_RECOGNIZE_TIMER			///<��дʶ��ʼ��ʱ��ID
#define HANDWRITECANDIDATE_CONFIRM_TIMER    GBIME_HW_CANDIDATE_CONFIRM_TIMER	///<��дʶ���ѡȷ��������ʱ��ID
#endif

typedef void (*GbFuncPtr)(void);

/*!
 * \brief ����ƽ̨���뷨ģʽ��ȡĿ��Layout ID
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
 * \brief ����Ŀ���л�Layout ��ID�ҵ���Ӧ��ƽ̨���뷨ģʽ
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
 * \brief ���ݵ�ǰLayout ID��ȡ��һ��Layout ID
 * \param nCurLayoutID
 * \param pNextLayoutID 
 * \return 
 * \note ��mapCount > 1ʱ������ѡ��������뷨����ȱʡ���뷨Ӧ��ָ����LayoutID
 *\example
 * \author ZhaoKun
 * \date 2010-10-25 16:24:17
 */
GBIMEReturn GBPA_GetNextLayoutIDByCurLayoutID(GBUINT16 nCurLayoutID, GBUINT16 *pNextLayoutID);

#if __MTK__ > 0 // MTKƽ̨�ο�����
/*!
 * \brief ������ѡ
 * \param GBUACHAR 
 * \return void
 */
GBIMEGLOBAL void GBPA_PenConfirmHandwritingSelection(void);

/*!
 * \brief multitap����ĸup����
 * \param mmi_imc_key_custom_value,GBU16 
 * \return GBI
 * \ע�⣺mmi_imc_key_multitap_alphabetic_up_handler��imc_key����static�ģ�Ҫ�Ѻ��Ĵ���copy�����
 */
GBIMEGLOBAL GBI GBPA_KeyMultitapAlphabeticUpHandler(mmi_imc_key_custom_value key_code, GBU16 event_type);

/*!
 * \brief multitap����ĸdown����
 * \param mmi_imc_key_custom_value,GBU16 
 * \return GBI
 * \ע�⣺mmi_imc_key_multitap_alphabetic_down_handler��imc_key����static�ģ�Ҫ�Ѻ��Ĵ���copy�����
 */
GBIMEGLOBAL GBI GBPA_KeyMultitapAlphabeticDownHandler(mmi_imc_key_custom_value key_code, GBU16 event_type);

/*!
 * \brief ȫ����д
 * \param void
 * \return void
 * \ע�⣺
 */
GBIMEGLOBAL void GBPA_EditorEnableFullScreenHandwriting(void);

/*!
 * \brief ����״̬
 * \param mmi_imc_state_enum
 * \return void
 * \ע�⣺
 */
GBIMEGLOBAL void GBPA_SetState( mmi_imc_state_enum state);

/*!
 * \brief ������д
 * \param 
 * \return void
 * \ע�⣺
 */
GBIMEGLOBAL GBU32 GBPA_ClearHandWritingStroke(void);

/*!
* \brief ��������Ļ
* \param GBWCHAR, GbFuncPtr,GbFuncPtr,GBPVOID
* \param rect 
* \return GBUACHAR
*/
GBIMEGLOBAL GBUACHAR GBPA_EntryNewScreen(GBWCHAR newscrnID, GbFuncPtr newExitHandler, GbFuncPtr newEntryHandler, GBPVOID flag);

#endif // __MTK__  // MTKƽ̨�ο�����
#endif // __PLATFORM_ADAPTER__
