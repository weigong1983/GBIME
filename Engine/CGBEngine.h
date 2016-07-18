/***************************************************************************
* File Name    :  GuobiIMEngine.h
* Corporation  :  Guobi Technology Ltd. GuangZhou China
* Copyright    :  Copyright Guobi Technology Ltd. And Subject to Confidentiality 
*                 Restrictions. This document is the confidential and proprietary 
*                 property of Guobi Technology Ltd.It is neither the document                  
*                 itself nor any part of it can be shared with any party  without 
*                 the written  permission. 
* Author       :  fanbo
* Date         :  2010-06-08
* Purpose      :  ��װ��������
* Version      :  V1.0
* Last Changed List
* Name         |   date   |  Discription
****************************************************************************/
#ifndef __GUOBIIMENGINE_H_
#define __GUOBIIMENGINE_H_
#include "gbapi.h"
#include "gbdef.h"
#include "gbim.h"
#include "gbtype.h"
#include "GBIMEDef.h"
#include "gbft_def.h"
#if __MTK__ == 0
#include "GBIMEResDef.h"
#endif
/*! ��ǰ����״̬ */
typedef enum 
{
	GBStatus_Normal
	,GBStatus_Shift
	,GBStatus_Caplock
}GBShiftCapStatus;

/*!
 * \brief ��������ȫ������
 */
typedef struct __tagGBEngine
{
	GBInputStruct		g_gbis;					///���뷨״̬�����Լ����ݽ�������
	GBAuxInfo			g_aux;					///��������ṹ��
	GBLDB				g_ldbs[GBIME_MAX_DB_COUNT];	///<��������
	GBU16				g_nDataCount;			///<�������ݸ���
	GBBOOL				engine_opened;			///<�����Ƿ��
	GBBOOL				input_mode_is_valid;	///<����ģʽ�Ƿ�Ϸ�(�Ϸ����ʾ����������)	
//	GBBOOL				isSwitchNextInputMode;	///<�Ƿ��л�����һ������ģʽ

	// UDB ���
	GBI32				master_udb_id;			///<��UDB ID
	GBI32				slave_udb_id;			///<��UDB ID
	GBBOOL				master_udb_loaded;		///<��UDB�Ƿ����
	GBBOOL				slave_udb_loaded;		///<��UDB�Ƿ����
} CGBEngine, *PCGBEngine;

#ifndef GBIME_CFG_STATIC_DB // �ⲿ�����ļ�·��
	#define GBIME_DB_FILE_EN "./Engine/gb_en.ebd"
	#define GBIME_DB_FILE_SC "./Engine/gb_sc.ebd"
#endif // GBIME_CFG_STATIC_DB

/*!
 * \brief ��ʼ���������뷨����
 * \param pEngine 
 * \return 
 * \note �����κι�������API֮ǰ������ִ�иú���
 *\example
 * \author weizhiping
 * \date 2010-7-22 11:06:32
 */
GBRETURN GBEngine_Initialize(PCGBEngine pEngine);

/*!
 * \brief �رչ������뷨���棬���а󶨵������MDB, UDB�����Ͽ� 
 * \param pEngine 
 * \return 
 * \note �����Ҫ������������API����Ҫ���µ���GBEngine_Initialize
 *\example
 * \author weizhiping
 * \date 2010-7-22 11:01:39
 */
GBRETURN GBEngine_Terminate(PCGBEngine pEngine);

/*!
 * \brief �������뷨����GBMMI״̬����
 * \param pEngine 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-22 16:21:02
 */
GBRETURN GBEngine_Reset(PCGBEngine pEngine);

/*!
 * \brief �жϹ������뷨�����Ƿ��
 * \param pEngine 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-22 11:23:31
 */
GBBOOL GBEngine_IsOpened(PCGBEngine pEngine);

/*!
 * \brief ���뷨������Ϣ������
 * \param pIMEEvent 
 * \return 
 */
GBIMEReturn GBEngine_HandleMessage(PGBIMEEvent pIMEEvent);

/*!
 * \brief �л��������뷨��������ģʽ�����Ի���
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
GBRETURN GBEngine_SwitchInputMode(PCGBEngine pEngine, GBINT input_mode, GBINT nLangID, GBINT nSubLangID);

/*!
 * \brief �ж������Ƿ�׼���ý����û�������
 * \param pEngine 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-22 11:27:12
 */
GBBOOL GBEngine_IsReadyInput(PCGBEngine pEngine);

/*!
 * \brief ���Ͱ�����Ϣ�����洦��
 * \param pEngine 
 * \param evt 
 * \param param 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-22 11:29:07
 */
GBU32 GBEngine_SendKeyEvent(PCGBEngine pEngine, GBINT evt, GBU32 param );

/*!
 * \brief ��ȡ���浱ǰ��������
 * \param pEngine 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-8-5 17:07:52
 */
GBLANG GBEngine_GetCurLang(PCGBEngine pEngine);

/*!
 * \brief ��ȡ���浱ǰ����ģʽ
 * \param pEngine 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-8-5 17:07:52
 */
GBU8 GBEngine_GetInputMode(PCGBEngine pEngine);

/*!
 * \brief �Ƿ񳣹水��
 * \param pEngine 
 * \param event 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-8-5 17:07:13
 */
GBBOOL GBEngine_IsNormalKey(PCGBEngine pEngine, GBINT event);

/*!
 * \brief ��ǰ״̬�����Ƿ�Ӧ�ô�����������OK��ɾ���ȹ��ܰ������жϹ��ʺ�ѡ���Ƿ񼤻
 * \param pEngine 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-8-5 17:07:05
 */
GBBOOL GBEngine_IsActive(PCGBEngine pEngine);

/*!
 * \brief �Ƿ��������
 * \param pEngine 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-8-5 17:06:54
 */
GBBOOL GBEngine_CanUpScreen(PCGBEngine pEngine);


///////////////////////////////////////////////////////////////////////////////////////////////////
// ���²��������INPUT_MODE_IS_AMB_CHN( ������������ģʽ�²ſ����ж�����ں�ѡ)
///////////////////////////////////////////////////////////////////////////////////////////////////

/*!
 * \brief ����ģʽ�Ƿ���ģ�����������뷨��ģ�����������뷨�Ŵ���ƴ���ĺ�ѡ
 * \param pEngine 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-22 16:39:37
 */
GBBOOL GBEngine_Is_AMB_Chn(PCGBEngine pEngine);

/*!
 * \brief ��ǰ״̬���Ƿ���ܴ������ں�ѡ
 * \param pEngine 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-22 16:39:37
 */
GBBOOL GBEngine_HaveSyllable(PCGBEngine pEngine);

/*!
* \brief ��ȡ��ǰҳƴ�����ڵĸ�����ƴ�������GBSetPageOption
*  ����ƴ���ķ�ҳ��Ϣ����ҳ�� �����Ҫ�������е�ƴ����ѡ��
*  ��ֻ��Ҫ��GBSetPageOption ��ƴ���ķ�ҳ����Ϊ����GB_MAX_SYLLABLES��ƴ������
 * \param pEngine 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-22 16:41:19
 */
GBU8 GBEngine_GetSyllableNum(PCGBEngine pEngine);

/*!
 * \brief ��ȡƴ������buffer
 * \param pEngine 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-22 16:43:35
 */
GBPCWCHAR * GBEngine_GetSyllableBuffer(PCGBEngine pEngine);

/*!
 * \brief ������һҳ����
 * \param pEngine 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-22 16:48:17
 */
GBU8 GBEngine_HavePrevSyllPage(PCGBEngine pEngine);

/*!
 * \brief ������һҳ����
 * \param pEngine 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-22 16:48:17
 */
GBU8 GBEngine_HaveNextSyllPage(PCGBEngine pEngine);

/*!
 * \brief ��ȡ��һҳ����
 * \param pEngine 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-22 16:48:17
 */
void GBEngine_GetPrevSyllPage(PCGBEngine pEngine);

/*!
 * \brief ��ȡ��һҳ����
 * \param pEngine 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-22 16:48:17
 */
void GBEngine_GetNextSyllPage(PCGBEngine pEngine);

/*!
 * \brief ������һ������
 * \param pEngine 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-22 16:48:17
 */
GBU8 GBEngine_HavePrevSyllable(PCGBEngine pEngine);

/*!
 * \brief ������һ������
 * \param pEngine 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-22 16:48:17
 */
GBU8 GBEngine_HaveNextSyllable(PCGBEngine pEngine);

/*!
 * \brief ��ȡ��һ������
 * \param pEngine 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-22 16:48:17
 */
void GBEngine_GetPrevSyllable(PCGBEngine pEngine);

/*!
 * \brief ��ȡ��һ������
 * \param pEngine 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-22 16:48:17
 */
void GBEngine_GetNextSyllable(PCGBEngine pEngine);

/*!
 * \brief ѡ��ָ��������ƴ������
 * \param pEngine 
 * \param idx 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-22 16:48:17
 */
void GBEngine_select_syll_in_page(PCGBEngine pEngine, GBU16 idx );

/*!
 * \brief ��ȡ��ǰ��������
 * \param pEngine 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-22 16:59:26
 */
GBU8 GBEngine_GetCurrentSyllableIndex(PCGBEngine pEngine);

///////////////////////////////////////////////////////////////////////////////////////////////////
// ���²�������Ժ�ѡ
///////////////////////////////////////////////////////////////////////////////////////////////////

/*!
 * \brief ���ں�ѡ
 * \param pEngine 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-22 17:01:04
 */
GBBOOL GBEngine_HaveCandidate(PCGBEngine pEngine);

/*!
 * \brief ��ȡ��ѡ�ܸ���
 * \param pEngine 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-22 17:01:16
 */
GBU16 GBEngine_GetCandNum(PCGBEngine pEngine);

/*!
 * \brief ��ȡ��ѡ��Buffer
 * \param pEngine 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-22 17:01:16
 */
GBPCWCHAR * GBEngine_GetCandidateBuffer(PCGBEngine pEngine);

/*!
 * \brief �Ƿ������һҳ��ѡ
 * \param pEngine 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-22 17:01:16
 */
GBU8 GBEngine_HavePrevCandPage(PCGBEngine pEngine);

/*!
 * \brief �Ƿ������һҳ��ѡ
 * \param pEngine 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-22 17:01:16
 */
GBU8 GBEngine_HaveNextCandPage(PCGBEngine pEngine);

/*!
 * \brief ��ȡ��һҳ��ѡ
 * \param pEngine 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-22 17:01:16
 */
void GBEngine_GetPrevPageCandidate(PCGBEngine pEngine);

/*!
 * \brief ��ȡ��һҳ��ѡ
 * \param pEngine 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-22 17:01:16
 */
void GBEngine_GetNextPageCandidate(PCGBEngine pEngine);

/*!
 * \brief ������һ����ѡ
 * \param pEngine 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-22 17:01:16
 */
GBBOOL GBEngine_HavePrevCandidate(PCGBEngine pEngine);

/*!
 * \brief ������һ����ѡ
 * \param pEngine 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-22 17:01:16
 */
GBBOOL GBEngine_HaveNextCandidate(PCGBEngine pEngine);

/*!
 * \brief ��ȡ��һ����ѡ
 * \param pEngine 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-22 17:01:16
 */
void GBEngine_GetPrevCandidate(PCGBEngine pEngine);

/*!
 * \brief ��ȡ��һ����ѡ
 * \param pEngine 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-22 17:01:16
 */
void GBEngine_GetNextCandidate(PCGBEngine pEngine);

/*!
 * \brief ѡ��ǰ��ѡ��ҳ�е�ָ���±�ĺ�ѡ
 * \param pEngine
 * \param idx 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-22 17:01:16
 */
void GBEngine_SelCandidateByIndex(PCGBEngine pEngine, GBU32 idx);

/*!
 * \brief ��ȡ�����ַ���(Ŀǰֻ֧������)
 * \param pEngine 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-22 17:01:16
 */
GBRETURN GBEngine_GetAssocWord(PCGBEngine pEngine, GBLPCWCHAR pConfirmedWord );

/*!
 * \brief ��ȡ�ʻ������봮
 * \param pEngine 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-22 17:01:16
 */
GBWCHAR * GBEngine_GetStorkeString(PCGBEngine pEngine);

/*!
 * \brief ��ȡ���������봮
 * \param pEngine 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-22 17:01:16
 */
GBWCHAR * GBEngine_GetComponentsString(PCGBEngine pEngine);

///////////////////////////////////////////////////////////////////////////////////////////////////
// ���潻������������
///////////////////////////////////////////////////////////////////////////////////////////////////
/*!
 * \brief ��ȡ��Ҫ�������ַ���
 * \param pEngine 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-22 17:01:16
 */
GBWCHAR * GBEngine_GetUpScreenString(PCGBEngine pEngine);

/*!
 * \brief �����Ҫ�������ַ���
 * \param pEngine 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-22 17:01:16
 */
void GBEngine_ClearUpScreenString(PCGBEngine pEngine);

/*!
 * \brief ��ȡ��������������뷨����ʵ�ʱ���Ѿ�ѡ���˵Ĵ����Լ���������봮��
 * \param pEngine 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-22 17:01:16
 */
GBPCWCHAR GBEngine_GetOutputString(PCGBEngine pEngine);

/*!
 * \brief ��ȡ��������
 * \param pEngine 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-22 17:01:16
 */
GBPCWCHAR GBEngine_GetInputString(PCGBEngine pEngine);

/*!
 * \brief ��ȡ����ģʽ�ľ���״̬,��GBMMI��API�������
 * \param pEngine 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-22 17:01:16
 */
GBU16 GBEngine_GetCurStatus(PCGBEngine pEngine);

/*!
 * \brief �Ƿ�ɰ��ϼ�����
 * \param pEngine 
 * \return 
 * \note 
 *\example
 * \author Zhaokun
 * \date 2010-10-21 17:01:16
 */
GBBOOL GBEngine_GetUpScreenArrowFlag(PCGBEngine pEngine);

/*!
 * \brief �Ƿ���ʾ��ѡ�ϼ�ͷ
 * \param pEngine 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-8-9 17:01:16
 */
GBBOOL GBEngine_GetCandUpArrowFlag(PCGBEngine pEngine);

/*!
 * \brief �Ƿ���ʾ��ѡ�¼�ͷ
 * \param pEngine 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-8-9 17:01:16
 */
GBBOOL GBEngine_GetCandDownArrowFlag(PCGBEngine pEngine);

/*!
 * \brief �Ƿ���ʾ��ѡ���ͷ
 * \param pEngine 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-22 17:01:16
 */
GBBOOL GBEngine_GetCandLeftArrowFlag(PCGBEngine pEngine);

/*!
 * \brief �Ƿ���ʾ��ѡ�Ҽ�ͷ
 * \param pEngine 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-8-9 17:01:16
 */
GBBOOL GBEngine_GetCandRightArrowFlag(PCGBEngine pEngine);

/*!
 * \brief �Ƿ���ʾ�������ͷ
 * \param pEngine 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-8-9 17:01:16
 */
GBBOOL GBEngine_GetSyllableLeftArrowFlag(PCGBEngine pEngine);

/*!
 * \brief �Ƿ���ʾ�����Ҽ�ͷ
 * \param pEngine 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-8-9 17:01:16
 */
GBBOOL GBEngine_GetSyllableRightArrowFlag(PCGBEngine pEngine);

/*!
 * \brief ���ù������뷨����ĺ�ѡ���ͣ���Ӱ���û������ѡ�ķ�ʽ
 * \param pEngine 
 * \param nCandType 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-8-17 10:18:54
*/
GBRETURN GBEngine_SetCandType(PCGBEngine pEngine, GBINT nCandType);

/*!
 * \brief ������������
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
GBRETURN GBEngine_SetEngineOption(PCGBEngine pEngine, GBINT engine_id, GBU32 config, GBU32 option);

/*!
 * \brief ���°�UDB(���л����Ժ����뷨ʱ����)
 * \param pEngine
 * \return
 * \note ���֮ǰUDB Buffer�Ѿ�����Ϊ���������ԣ���ô��Ҫ����֮ǰ���ݣ�
 * ͬʱ��ͬһ��UDB Bufferʹ�õ��µ����Ե��������棬
 * �����������ԼRAM�ռ䣬���ǻᵼ��һ�������ã��������޷��л�֮��
 * �л���������û�����������ĺ�ѡ��
 * Ҳ������������UDB Buffer�����������Լ��л������Էֱ�һ��UDB Buffer����������
 * �����������⣬���ǻ�����RAM�ռ䡣
 * \author weizhiping
 * \date 2009��12��11��
 */
GBRETURN GBEngine_LoadUDB(PCGBEngine pEngine);

/*!
 * \brief ж��UDB
 * \param pEngine 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-8-31 14:32:28
*/
GBRETURN GBEngine_UnLoadUDB(PCGBEngine pEngine);

/*!
 * \brief ���ù������뷨�������������ѡ��
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
							GBUINT nOption);

/*!
 * \brief ע���������뷨��������Ź��ܼ�
 * \param pEngine 
 * \param cKey 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-1 15:34:12
*/
GBRETURN GBEngine_UnregisterInterpunctionKey(PCGBEngine pEngine, GBINT cKey);



/*!
 * \brief ���ù������뷨�����ѡ����
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
						   GBINT Alp_candListType);


/*!
 * \brief �жϹ������뷨���浱ǰ�Ƿ�����������ģʽ
 * \param pEngine 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-2 10:16:27
*/
GBBOOL GBEngine_IsChineseInputMode(PCGBEngine pEngine);

/*!
 * \brief �жϹ������뷨���浱ǰ�Ƿ���Alp����ģʽ
 * \param pEngine 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-2 10:15:56
*/
GBBOOL GBEngine_IsAlpInputMode(PCGBEngine pEngine);

/*!
 * \brief �жϹ������뷨���浱ǰ�Ƿ���MultiTap����ģʽ
 * \param pEngine 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-2 10:19:59
*/
GBBOOL GBEngine_IsMultiTapStatus(PCGBEngine pEngine);

/*!
 * \brief �жϹ������뷨���浱ǰ�Ƿ�ʹ�ñ��������������
 * \param pEngine 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-2 10:19:59
*/
GBBOOL GBEngine_IsSymbolCandType(PCGBEngine pEngine);

/*!
 * \brief �жϹ������뷨���浱ǰ�Ƿ��ڳ�ʼ��״̬(�ս���༭����ÿ���������ʱ���״̬
 * \param pEngine 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-2 10:19:59
*/
GBBOOL GBEngine_IsInitialStatus(PCGBEngine pEngine);

/*!
 * \brief �жϹ������뷨���浱ǰ�Ƿ�������״̬
 * \param pEngine 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-2 10:19:59
*/
GBBOOL GBEngine_IsInputStatus(PCGBEngine pEngine);

/*!
 * \brief �жϹ������뷨���浱ǰ�Ƿ��ں�ѡ��״̬
 * \param pEngine 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-2 10:19:59
*/
GBBOOL GBEngine_IsSelectedStatus(PCGBEngine pEngine);

/*!
 * \brief �жϹ������뷨���浱ǰ�Ƿ�������ģʽ
 * \param pEngine 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-2 10:16:41
*/
GBBOOL GBEngine_IsAssociateStatus(PCGBEngine pEngine);

/*!
 * \brief �жϹ������뷨���浱ǰ�Ƿ������������ģʽ
 * \param pEngine 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-2 10:19:59
*/
GBBOOL GBEngine_IsAlpUAWStatus(PCGBEngine pEngine);

/*!
 * \brief �жϹ������뷨���浱ǰ�Ƿ�����������ʿ�ʼ״̬
 * \param pEngine 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-2 10:19:59
*/
GBBOOL GBEngine_IsAlpUAWBeginStatus(PCGBEngine pEngine);

/*!
 * \brief �жϹ������뷨���浱ǰ�Ƿ�����������ʹ���״̬
 * \param pEngine 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-2 10:19:59
*/
GBBOOL GBEngine_IsAlpUAWProcessingStatus(PCGBEngine pEngine);

/*!
 * \brief �жϹ������뷨���浱ǰ�Ƿ������������û��������ѡ��ʱ���·�״̬
 * \param pEngine 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-2 10:19:59
*/
GBBOOL GBEngine_IsAlpUAWNoMoreCandStatus(PCGBEngine pEngine);

/*!
 * \brief �жϹ������뷨���浱ǰ�Ƿ�����������ʽ������û�ȷ�ϼ��룬���״̬���Թر�
 * \param pEngine 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-2 10:19:59
*/
GBBOOL GBEngine_IsAlpUAWConfirmStatus(PCGBEngine pEngine);

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
											GBINT indexMulPron, GBPWCHAR pBuff);

/*!
 * \brief ֱ�Ӹ������봮��ȡ��ѡ
 * \param pEngine 
 * \param pInputString 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-7 8:51:43
*/
GBRETURN GBEngine_SetInputString(PCGBEngine pEngine, GBLPCWCHAR pInputString);

/*!
 * \brief ��ȡ��������ʼλ��ָ�� GBMMIʹ��
 * \param pEngine 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-2 10:19:59
*/
GBPBYTE GBEngine_GetRowStart(PCGBEngine pEngine);

/*!
 * \brief ��ȡ��ǰѡ����� GBMMIʹ��
 * \param pEngine 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-2 10:19:59
*/
GBI8 GBEngine_GetSelCol(PCGBEngine pEngine);

/*!
 * \brief ��ȡ��ǰѡ����� GBMMIʹ��
 * \param pEngine 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-2 10:19:59
*/
GBI8 GBEngine_GetSelRow(PCGBEngine pEngine);

/*!
 * \brief ��ȡ��ǰҳ��Ч������ GBMMIʹ��
 * \param pEngine 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-2 10:19:59
*/
GBU8 GBEngine_GetRowCount(PCGBEngine pEngine);

/*!
 * \brief �жϹ������뷨���浱ǰ�Ƿ���MultiTap����ģʽ
 * \param pEngine 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-2 10:19:59
*/
GBBOOL GBEngine_IsMultiTapInputMode(PCGBEngine pEngine);

/*!
 * \brief ���������Сд״̬���
 * \param pEngine 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-14 17:58:24
*/
GBShiftCapStatus GBEngine_GetShiftCapStatus(PCGBEngine pEngine);

/*!
 * \brief �˳�ѡ��״̬[��������ʱ��Ҫȷ�����������ڽ���ѡ��״̬]
 * \param pEngine 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-20 14:14:11
*/
GBRETURN GBEngine_ExitSelectedStatus(PCGBEngine pEngine);

/*!
 * \brief ���ù������뷨����Ŀ��Ƽ�״̬������alphabetic������Ч
 * \param pEngine 
 * \param nCandType 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-8-17 10:18:54
*/
GBRETURN GBEngine_SetShiftCap(PCGBEngine pEngine, GBINT bShiftDown, GBINT bCapsLockToggle);

/*!
 * \brief ���ù�������ҳ�����õ���ʾҳ����
 * \param pEngine
 * \return see GBRETURN
 * \note 
 * \author ZhaoKun
 * \date 2010��10��20��
 */
GBRETURN GBEngine_SetPageToalWidthOption(PCGBEngine pEngine, GBU16 nTotalWidth);

/*!
 * \brief �ж��Ƿ��ǿ����л�����ģʽ״̬
 * \param pEngine 
 * \return 
 * \note �����������
 * \example
 * \author Zhaokun
 * \date 2010-10-23 14:18:54
*/
GBBOOL GBEngine_IsNeedSwitchInputModeState(PCGBEngine pEngine);

/*!
 * \brief ����MultiTapȷ�ϻص�����
 * \param void 
 * \return 
 * \note 
 * \example
 * \author Zhaokun
 * \date 2010-10-23 14:18:54
*/
void GBEngine_GBRequestTimer_CallBack(void);

/*!
 * \brief ����MultiTap��ʱ����������
 * \param pOEMPrivateData 
 * \param nTimerType 
 * \return 
 * \note 
 * \example
 * \author Zhaokun
 * \date 2010-10-23 14:18:54
*/
GBU32 GBEngine_GBRequestTimer_t(GBLPVOID pOEMPrivateData, GBINT nTimerType);

/*!
 * \brief ����MultiTap��ʱ��ֹͣ����
 * \param pOEMPrivateData 
 * \param nTimerID 
 * \return 
 * \note 
 * \example
 * \author Zhaokun
 * \date 2010-10-23 14:18:54
*/
GBU32 GBEngine_GBRequestKillTimer_t(GBLPVOID pOEMPrivateData, GBU32 nTimerID);

/*!
 * \brief ����MultiTap��ʱ���󶨺���
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
									  GBLPVOID pOEMPrivateData);

/*!
 * \brief ���ú�ѡ���ѡ����
 * \param pEngine 
 * \param nInputCandRowCount ����״̬�º�ѡ����
 * \param nAssociateCandRowCount ����״̬�º�ѡ����
 * \return 
 * \note 
 *\example
 */
GBRETURN GBEngine_SetCandRowCount(PCGBEngine pEngine, GBINT nInputCandRowCount, GBINT nAssociateCandRowCount);

/*!
 * \brief ������浱ǰ�������Ƿ�Ϊ��
 * \param 
 * \return GBTrue/GBFalse
 * \note
 * \author weizhiping
 * \date 2009��12��11��
 */
GBBOOL GBEngine_IsInputEmpty(PCGBEngine pEngine);

#endif //__GUOBIIMENGINE_H_
