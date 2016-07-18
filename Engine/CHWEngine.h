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
* Purpose      :  ��װ������д����
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
 * \brief ��д���Ͷ���
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
 * \brief ��д����ɾ�������ַ�
 */
#define GBIME_HANDWRITE_DELETE_CTL_CHAR	0x000B

typedef struct
{
	GBBOOL  	open_flags;								///<��д����򿪱�־		
	GBU32  		dwRange;								///<��д����ʶ��Χ����
	GBBOOL 		bHomonymEnable; 						///<��дʶ��Χ����Ϊͬ��
	GBBOOL      bEnglishEnable;						    ///<��дʶ��Χ����ΪӢ��
	GBBOOL      bNumberEnable;							///<��дʶ��Χ����Ϊ����
	//GBWCHAR  	cand_buf[GBIME_DEF_MAX_CAND_WORD][2]; 	///<��д�����ѡ������
	GBPCWCHAR  	pCandidates[GBIME_DEF_MAX_CAND_WORD]; 	///<��д�����ѡ������ָ��
	GBU16  		cand_count;								///<��д�����ѡ����	
}GBHWEngine, *PGBHWEngine;

/*!
 * \brief ��ʼ��������д���뷨����
 * \param pGBHWEngine 
 * \param lang_data ��д���뷨�������� 
 * \return 
 * \note �����κι�����д����API֮ǰ������ִ�иú���
 * \example
 * \author weizhiping
 * \date 2010-9-7 16:40:01
*/
GBIMEReturn GBHW_Initialize(PGBHWEngine pGBHWEngine, const unsigned long * lang_data);

/*!
 * \brief ��д�����˳�(��ʱֻ���������ñ���)
 * \param pGBHWEngine
 * \return 
 * \note
 * \author haj
 * \date 2010-8-5
 */
void GBHW_Terminate(PGBHWEngine pGBHWEngine);

/*!
 * \brief ��д��������
 * \param pGBHWEngine 
 * \return 
 * \note �������ʶ���ѡ
 * \example
 * \author weizhiping
 * \date 2010-9-7 16:37:29
*/
void GBHW_Reset(PGBHWEngine pGBHWEngine);

/*!
 * \brief ���ù�����д���뷨�����ʶ��Χ
 * \param pGBHWEngine
 * \param dwRange GBHW_OPT_RANGE_XXX �����
 * \return 
 * \note
 * \author weizhiping
 * \date 2010-8-10
 */
GBIMEReturn GBHW_SetRange(PGBHWEngine pGBHWEngine, GBU32 dwRange);

/*!
 * \brief ����ʶ������ʶ�����
 * \param index ��Χ��GB_HW_Option_Index�����ֵ
 * \param value ����index���岻ͬ����ϸ�����ÿһ��index��˵��
 * \return ���indexΪ�Ƿ�ֵ���򷵻�GBHW_Err_Option_Invalid_Index 
 * -    �ɹ�����GBHW_OK
 * -    ��������ֵ��鿴ÿһ������ֵ��˵��
 * \note
 * \author haj
 * \date 2010-8-5
 */
int GBHW_SetOption(int index, GBUINT32 value);

/*!
 * \brief �ж���д�����Ƿ��
 * \param pGBHWEngine
 * \return
 * \note
 * \author haj
 * \date 2010-8-5
 */
GBBOOL GBHW_IsOpened(const PGBHWEngine pGBHWEngine);

/*!
 * \brief ����ʶ�����ĸ���
 * \param pGBHWEngine
 * \return ����ʶ�����ĸ���
 * \note
 * \author haj
 * \date 2010-8-5
 */
GBU16 GBHW_GetCandCount(PGBHWEngine pGBHWEngine);

/*!
 * \brief ������д��ѡBufferָ��
 * \param pGBHWEngine
 * \return
 * \note
 * \author haj
 * \date 2010-8-5
 */
GBPCWCHAR * GBHW_GetCandBuffer(PGBHWEngine pGBHWEngine);

/*!
 * \brief �����������غ�ѡ
 * \param pGBHWEngine
 * \param idx ��ѡ����
 * \return
 * \note
 * \author haj
 * \date 2010-8-5
 */
GBPCWCHAR GBHW_GetCandByIndex(PGBHWEngine pGBHWEngine, GBU16 idx);

/*!
 * \brief ��д����ʶ������ص�(������Ϣ֪ͨ��дLayout������д����ʶ������ĺ�ѡ)
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
 * \brief ������д���ͻ�ȡ��д����ʶ��Χ����
 * \param handwriteType 
 * \param dwRange ���
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-8-12 15:10:47
*/
GBIMEReturn GBHW_ConvertHandwriteTypeToRange(GBIMEHandwriteTypeEnum handwriteType, GBU32 *dwRange);

/*!
 * \brief ��д��Ϣ������
 * \param pIMEEvent 
 * \return 
 */
GBIMEReturn GBHW_HandleMessage(PGBIMEEvent pIMEEvent);

/*!
 * \brief ��ȡͬ����״̬
 * \param pGBHWEngine 
 * \return 
 */
GBBOOL GBHW_GetHomonymStatus(PGBHWEngine pGBHWEngine);

/*!
 * \brief ����ͬ����״̬
 * \param pGBHWEngine 
 * \param status 
 * \return 
 */
GBIMEReturn GBHW_SetHomonymStatus(PGBHWEngine pGBHWEngine, GBBOOL status);

/*!
 * \brief ��ȡӢ��ʶ��״̬
 * \param pGBHWEngine 
 * \return 
 */
GBBOOL GBHW_GetEnglishStatus(PGBHWEngine pGBHWEngine);

/*!
 * \brief ����Ӣ��ʶ��״̬
 * \param pGBHWEngine 
 * \param status 
 * \return 
 */
GBIMEReturn GBHW_SetEnglishStatus(PGBHWEngine pGBHWEngine, GBBOOL status);

/*!
 * \brief ��ȡ����ʶ��״̬
 * \param pGBHWEngine 
 * \return 
 */
GBBOOL GBHW_GetNumberStatus(PGBHWEngine pGBHWEngine);

/*!
 * \brief ��������ʶ��״̬
 * \param pGBHWEngine 
 * \param status 
 * \return 
 */
GBIMEReturn GBHW_SetNumberStatus(PGBHWEngine pGBHWEngine, GBBOOL status);

#endif

#endif //__GUOBIHANDWRITE_H_
