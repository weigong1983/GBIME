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
* Purpose      :  ��װ������д����
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
 * \brief ��ʼ��������д���뷨����
 * \param phwData ��д���뷨�������� 
 * \return 
 * \note �����κι�����д����API֮ǰ������ִ�иú���
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

	// �ǵó�ʼ��
	memset(pGBHWEngine, 0, sizeof(GBHWEngine));
	ret = GBHWNew(lang_data,0);
	PT_Assert(ret == 0);
	pGBHWEngine->open_flags = GBTrue;
	return GBIME_OK;
}

/*!
 * \brief ��д�����˳�(��ʱֻ���������ñ���)
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
 * \brief ��д��������
 * \param pGBHWEngine 
 * \return 
 * \note �������ʶ���ѡ
 * \example
 * \author weizhiping
 * \date 2010-9-7 16:37:29
*/
void GBHW_Reset(PGBHWEngine pGBHWEngine)
{
	GBINT i;

	//pGBHWEngine->dwRange = 0x0; ʶ��Χ�������
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
 * \brief ���ù�����д���뷨�����ʶ��Χ
 * \param pGBHWEngine
 * \param dwRange GBHW_OPT_RANGE_XXX �����
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
 * \brief ��ȡ������д���뷨�����ʶ��Χ
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
int GBHW_SetOption(int index, GBUINT32 value)
{
	return GBHWSetOption(index, value);
}

/*!
 * \brief �ж���д�����Ƿ��
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
 * \brief ������д��ѡ
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

	// ���º�ѡ��ر���
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
 * \brief ����ʶ�����ĸ���
 * \param pGBHWEngine
 * \return ����ʶ�����ĸ���
 * \note �������GBHW_UpdateCandidates������д��ѡ
 * \author haj
 * \date 2010-8-5
 */
GBU16 GBHW_GetCandCount(PGBHWEngine pGBHWEngine)
{
	PT_Assert(GBHW_IsOpened(pGBHWEngine));
	return pGBHWEngine->cand_count;
}

/*!
 * \brief ������д��ѡBufferָ��
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
 * \brief �����������غ�ѡ
 * \param pGBHWEngine
 * \param idx ��ѡ����
 * \return
 * \note �����ȵ���GBHW_UpdateCandidates������д��ѡ
 * \author haj
 * \date 2010-8-5
 */
GBPCWCHAR GBHW_GetCandByIndex(PGBHWEngine pGBHWEngine, GBU16 idx)
{
	PT_Assert(GBHW_IsOpened(pGBHWEngine));
	
	if (!(idx >= 0 && idx < GBHW_GetCandCount(pGBHWEngine)))
	{
		PT_PrintLogInfo(GBLOG_HANDWRITE, ("GBHW_GetCandByIndex �Ƿ�����: [%d] GBHW_GetCandCount=[%d]\r\n", idx, GBHW_GetCandCount(pGBHWEngine)));
		return NULL;
	}
	
	return (GBPCWCHAR)pGBHWEngine->pCandidates[idx];
}

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
void GBHW_RecognizeFinishedCallback(GBPCWCHAR pCandBuffer, GBU16 cand_count)
{
	GBIMEEvent imeEvent;
	GBPCWCHAR pCand = NULL;
	
	// ������д��ѡ
	GBHW_UpdateCandidates(Global_GetHWEnineInstance(), pCandBuffer, cand_count);
	
	// ��ȡ��һ����ѡ�������
	pCand = GBHW_GetCandByIndex(Global_GetHWEnineInstance(), 0);
	
	// ����������һ�ᣬ���潫ʶ��Ϊ"ɾ��"�������ɾ���������������һ���ַ�
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
 * \brief ������д���ͻ�ȡ��д����ʶ��Χ����
 * \param handwriteType 
 * \param dwRange ���
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
 * \brief ��д��Ϣ������
 * \param pIMEEvent 
 * \return 
 */
GBIMEReturn GBHW_HandleMessage(PGBIMEEvent pIMEEvent)
{
	GBIMEReturn ret = GBIME_OK;

	if (pIMEEvent->nType != GBIME_EVENT_TYPE_HANDWRITE)
	{
		return GBIME_IGNORE; // ����д�¼�������
	}

	switch (pIMEEvent->nEvent)
	{
	case GBIME_EVENT_HANDWRITE_ENGINE_INIT: ///<��д�����ʼ��
		// ��ʼ����д�������д�켣������
		//ret = GBHW_Initialize(Global_GetHWEnineInstance(), Global_GetHWEngineData());
		//GBHW_SetRange(Global_GetHWEnineInstance(), GBHW_OPT_RANGE_DEFAULT);
		break;
	case GBIME_EVENT_HANDWRITE_ENGINE_EXIT: ///<��д�����˳�
		// ������д�������д�켣������
		//GBHW_Terminate(Global_GetHWEnineInstance());
		break;
	case GBIME_EVENT_HANDWRITE_DRAW_CANDIDATE: ///<������дʶ������ĺ�ѡ
	case GBIME_EVENT_HANDWRITE_DRAW_ASSOCWORD: ///<���������ַ���
		{
			PCLayoutAbstract pltObj = Global_GetCurrentLayoutObject();
			
			// Layout��Ϣ����
			if (pltObj->m_pfcHandleEvent != NULL)
			{
				pltObj->m_pfcHandleEvent(pltObj, pIMEEvent, CLayoutControl_GetEngineOutputInfo());
			}
		}
		break;
	case GBIME_EVENT_HANDWRITE_SET_RECOGNIZE_RANGE: ///<����ʶ��Χ
		{
			GBU32 dwRange;
			GBIMEHandwriteTypeEnum handwriteType = pIMEEvent->wParam;
			GBHW_ConvertHandwriteTypeToRange(handwriteType, &dwRange);
			GBHW_SetRange(Global_GetHWEnineInstance(), dwRange);
		}		 
		break;
	default:
		PT_Assert(0); // �Ƿ��¼�
	}

	return ret;
}

/*!
 * \brief ��ȡͬ����״̬
 * \param pGBHWEngine 
 * \return 
 */
GBBOOL GBHW_GetHomonymStatus(PGBHWEngine pGBHWEngine)
{
	PT_Assert(GBHW_IsOpened(pGBHWEngine));
	return pGBHWEngine->bHomonymEnable;
}

/*!
 * \brief ����ͬ����״̬
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
 * \brief ��ȡӢ��ʶ��״̬
 * \param pGBHWEngine 
 * \return 
 */
GBBOOL GBHW_GetEnglishStatus(PGBHWEngine pGBHWEngine)
{
	PT_Assert(GBHW_IsOpened(pGBHWEngine));
	return pGBHWEngine->bEnglishEnable;
}

/*!
 * \brief ����Ӣ��ʶ��״̬
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
 * \brief ��ȡ����ʶ��״̬
 * \param pGBHWEngine 
 * \return 
 */
GBBOOL GBHW_GetNumberStatus(PGBHWEngine pGBHWEngine)
{
	PT_Assert(GBHW_IsOpened(pGBHWEngine));
	return pGBHWEngine->bNumberEnable;
}

/*!
 * \brief ��������ʶ��״̬
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
* \defgroup ����API�Ƿ�װ��MTKƽ̨��дģ����õ�
*/

/*!
 * \brief ��ʼ��������д����(��װ��MTKƽ̨����)
 * \param hw_type 
 * \return 
 * \note һ�㽨�鿪��ʱ���ʼ��
 * \example
 * \author weizhiping
 * \date 2010-12-1 17:46:13
*/
void mmi_ime_hand_writing_initialize(mmi_imc_pen_handwriting_type_enum hw_type)
{
	// ��ʼ����д����
	GBHW_Initialize(Global_GetHWEnineInstance(), Global_GetHWEngineData());
	GBHW_SetRange(Global_GetHWEnineInstance(), GBHW_OPT_RANGE_DEFAULT);
}

/*!
 * \brief ��д�����˳�
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
 * \brief ���õײ���ϯ����õ���ѡ
 * \param query_param 
 * \param result_param 
 * \return 
 * \note 
 * \example ��imc_pen.c�б�����
 * \author weizhiping
 * \date 2010-12-1 17:49:03
*/
S32 mmi_ime_hand_writing_get_candidates(mmi_ime_hand_writing_query_param_p query_param,
										mmi_ime_hand_writing_result_param_p result_param)
{
    S32 num = 0;
	unsigned long recognizeRange = GBHW_GetRange(Global_GetHWEnineInstance());

    num = GBHWRecognize((short*)(query_param->stroke_buffer),   /*IN  �û�����ıʼ���ע�����Ϊ:-1,0,-1,-1*/
    				 result_param->candidate_array ,			/*OUT �õ���ѡ����ڸ�buffe��r*/
    				 query_param->desired_cnt,					/*IN Ҫ��õ�ʶ���ѡ����*/
    				 recognizeRange);							/*ʶ��Χ*/
    result_param->result_cnt = num;
    return num;
}

/*!
 * \brief ��ѯ��дʶ������
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
 * \brief ��ӷָ�������д��ѡ��������Ĭ��Ϊ������'\0'��
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
 * \brief ���ݴ����ȡ��ѡ
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