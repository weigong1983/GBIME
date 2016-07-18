/***************************************************************************
* File Name    :  GBIMEDef.h
* Corporation  :  Guobi Technology Ltd. GuangZhou China
* Copyright    :  Copyright Guobi Technology Ltd. And Subject to Confidentiality 
*                 Restrictions. This document is the confidential and proprietary 
*                 property of Guobi Technology Ltd.It is neither the document                  
*                 itself nor any part of it can be shared with any party  without 
*                 the written  permission. 
* Author       :  weizhiping
* Date         :  2010-06-08
* Purpose      :  ���뷨����ⲿ��������
* Version      :  V1.0
* Last Changed List
* Name         |   date   |  Discription
****************************************************************************/
#ifndef __GBIME_DEF_H_
#define __GBIME_DEF_H_

#include "gbtype.h"
#include "Gbwchar.h"
#include "gbdef.h"

#define GBIME_CFG_STATIC_DB					1	// ʹ�þ�̬����
#define GBIME_CFG_UDB_SUPPORT				1	// UDB���������ģ��
//#define GBIME_CFG_HANDWRITE_SUPPORT			1	// ��д֧��(�ĳ�ƽ̨�����)
#define GBIME_CFG_CHS_SUPPORT   			1   // ����֧��
#define GBIME_CFG_EN_SUPPORT    			1   // Ӣ��֧��
#define GBIME_CFG_SLIDING_SWITCH_SUPPORT	1   // �����л�����

#ifndef GB_MICRO_OPTIMIZATION
#define GB_MICRO_OPTIMIZATION       1
#endif

#ifndef GBIMEAPI
#ifdef GBIMPORT
#define GBIMEAPI  __declspec(dllimport)
#else
#define GBIMEAPI	extern
#endif
#endif

/*!
 * \brief ���뵥Ԫ�ڲ�ʹ�õĺ���
 */
#define GBIMELOCAL		static
/*!
 * \brief ȫ��ʹ�ã��������⹫���ĺ���
 */
#define GBIMEGLOBAL 
/*!
 * \brief ���⹫���ĺ���
 */
#define GBIMEEXPORT 

#define GBIME_MAX_DB_COUNT						50	///<����������ݸ���
#define GBIME_DEF_MAX_CAND_WORD				    9	///<Ĭ������ѡ�ʸ��� < GB_MAX_CANDIDATES
#define GBIME_DEF_CHN_MAX_CAND_WORD				9	///<Ĭ������ѡ�ʸ��� < GB_MAX_CANDIDATES
#define GBIME_DEF_SYLLABLE_MAX_CAND_WORD		9	///<Ĭ������ѡ�ʸ��� < GB_MAX_CANDIDATES
#define GBIME_DEF_ENGINSH_MAX_CAND_WORD			9	///<Ĭ������ѡ�ʸ��� < GB_MAX_CANDIDATES
	
///<���뷨�����������(��Ҫ)
#ifdef GBIME_CFG_UDB_SUPPORT
	#define GBIME_UDB_UPDATE_FREQ		 				20 ///<����Ƶ�ʣ����ݵ��޸Ĵ����������ֵ��ʱ������ͻ����fpUpdateFunction��һ����˵���ֵ����Ϊ10
#endif/*GBIME_CFG_UDB_SUPPORT*/
#define GBIME_FILENAME_LEN 						256

/*!
 * \brief GB��������ѡ��궨��
 */
#define GBIME_INPUT_CAND_ROW_COUNT	 			1 ///<����С�� <= MAX_CANDIDATES / GB_CN_CANDIDATE_COUNT
#define GBIME_ASSOCIATE_CAND_ROW_COUNT			1 ///<����С�� <= MAX_CANDIDATES / GB_CN_CANDIDATE_COUNT
#define GBIME_SWITCH_KEY						GBKEY_AMB_SPECIAL_B ///<GB�޷��л�����
#define GBIME_INTERPUNCTION_KEY					GBKEY_AMB_SPECIAL_A ///<GB�����Ű���
#define GBIME_EMBEDDED_INTERPUNCTION_KEY		GBKEY_1 ///<���ܱ����Ű���
#define GBIME_INTERPUNCTION_CAND_DESIRE_ROW		2 ///<GB�����ż���ѡ����
#define GBIME_INTERPUNCTION_CAND_COUNT_PER_ROW	(GBIME_DEF_MAX_CAND_WORD - 1) ///<GB�����ż�ÿ�к�ѡ����

/*!
 * \brief ��ݱ����š����������س���
 */
#define GBIME_SYMBOL_MAX_LEN					3
#define GBIME_FACE_MAX_LEN						8
#define GBIME_SPLIT_CHAR						'\t' // ����ָ���
#define GBIME_SPLIT_STR							L"\t" // ����ָ���(������ϳɱ�����Ŵ�)

/*!
 * \brief UI��س���
 */
#if defined(__MMI_MAINLCD_240X320__) || defined(__MMI_MAINLCD_240X400__)
	#define GBIME_VERTICAL_SPACING 					8  ///<��ֱ������ʾ���(��̬�����ַ��߶ȴ�ֱ������Ч�������)
	#define GBIME_SYLLABLE_SPACING 					8  ///<�������ּ��
	#define GBIME_CANDIDATE_SPACING 				8  ///<��ѡ���ּ��
	#define GBIME_ARROW_WIDTH						25
	#define GBIME_CAND_AREA_WIDTH  					(PT_GetLcdWidth() - GBIME_ARROW_WIDTH * 2) ///<���ʺ�ѡ��Ŀ��
	#define GBIME_SYLLABLE_DISPLAY_START_X			GBIME_ARROW_WIDTH  // ƴ����ʾ�Ŀ�ʼλ��X����
	#define GBIME_SYLLABLE_LINE_WIDTH				GBIME_CAND_AREA_WIDTH // ƴ�������
	#define GBIME_SYLLABLE_LINE_HEIGHT				21  // ƴ�����߶�
	#define GBIME_CANDIDATE_DISPLAY_START_X			GBIME_ARROW_WIDTH  // ��ѡ��ʾ�Ŀ�ʼλ�õ�X����
	#define GBIME_CANDIDATE_LINE_WIDTH				GBIME_CAND_AREA_WIDTH	// ��ѡ�����
	#define GBIME_CANDIDATE_LINE_HEIGHT				23  // ��ѡ���߶�
	#define GBIME_SYLLABLE_FONT_SIZE				PT_FONT_SIZE_SMALL // ���������С
	#define GBIME_SYLLABLE_NORMAL_FONT_COLOR		PT_COLOR_BLACK  // û��ѡ���еĺ�ѡƴ����ɫ
	#define GBIME_SYLLABLE_SELECTED_FONT_COLOR		PT_COLOR_BLUE    // ѡ���еĵ�ƴ������ɫ
	#define GBIME_CANDIDATE_FONT_SIZE				PT_FONT_SIZE_SMALL // ��ѡ�����С
	#define GBIME_CANDIDATE_NORMAL_FONT_COLOR		PT_COLOR_BLACK  // û��ѡ���еĺ�ѡ������ɫ
	#define GBIME_CANDIDATE_SELECTED_FONT_COLOR		PT_COLOR_BLUE    // ѡ���еĺ�ѡ������ɫ
	#define GBIME_STROKE_FONT_SIZE					PT_FONT_SIZE_SMALL // �ʻ������С
	#define GBIME_STROKE_COLOR						PT_COLOR_BLACK    // �ʻ�����ɫ
	#define GBIME_COMPONENT_FONT_SIZE				PT_FONT_SIZE_SMALL // ��ݲ��������С
	#define GBIME_COMPONENT_COLOR					PT_COLOR_BLACK  // ��ݲ��׵���ɫ
	#define GBIME_ENGLISH_INPUTSTRING_COLOR			PT_COLOR_BLUE	// 9��Ӣ����������ִ�����ɫ
	#define GBIME_OUTPUT_STRING_COLOR				PT_COLOR_BLUE	// �����������ɫ
	#define GBIME_COMP_WIDTH						14 ///<���ʲ��׵Ŀ��
	#define GBIME_COMP_HEIGHT						14 ///<���ʲ��׵ĸ߶�
#elif defined(__MMI_MAINLCD_176X220__) //LCD 176��x220��
	#define GBIME_VERTICAL_SPACING 					8  ///<��ֱ������ʾ���(��̬�����ַ��߶ȴ�ֱ������Ч�������)
	#define GBIME_SYLLABLE_SPACING 					8  ///<�������ּ��
	#define GBIME_CANDIDATE_SPACING 				8  ///<��ѡ���ּ��
	#define GBIME_ARROW_WIDTH						25
	#define GBIME_CAND_AREA_WIDTH  					(PT_GetLcdWidth() - GBIME_ARROW_WIDTH * 2) ///<���ʺ�ѡ��Ŀ��
	#define GBIME_SYLLABLE_DISPLAY_START_X			GBIME_ARROW_WIDTH  // ƴ����ʾ�Ŀ�ʼλ��X����
	#define GBIME_SYLLABLE_LINE_WIDTH				GBIME_CAND_AREA_WIDTH // ƴ�������
	#define GBIME_SYLLABLE_LINE_HEIGHT				21  // ƴ�����߶�
	#define GBIME_CANDIDATE_DISPLAY_START_X			GBIME_ARROW_WIDTH  // ��ѡ��ʾ�Ŀ�ʼλ�õ�X����
	#define GBIME_CANDIDATE_LINE_WIDTH				GBIME_CAND_AREA_WIDTH	// ��ѡ�����
	#define GBIME_CANDIDATE_LINE_HEIGHT				23  // ��ѡ���߶�
	#define GBIME_SYLLABLE_FONT_SIZE				PT_FONT_SIZE_SMALL // ���������С
	#define GBIME_SYLLABLE_NORMAL_FONT_COLOR		PT_COLOR_BLACK  // û��ѡ���еĺ�ѡƴ����ɫ
	#define GBIME_SYLLABLE_SELECTED_FONT_COLOR		PT_COLOR_BLUE    // ѡ���еĵ�ƴ������ɫ
	#define GBIME_CANDIDATE_FONT_SIZE				PT_FONT_SIZE_SMALL // ��ѡ�����С
	#define GBIME_CANDIDATE_NORMAL_FONT_COLOR		PT_COLOR_BLACK  // û��ѡ���еĺ�ѡ������ɫ
	#define GBIME_CANDIDATE_SELECTED_FONT_COLOR		PT_COLOR_BLUE    // ѡ���еĺ�ѡ������ɫ
	#define GBIME_STROKE_FONT_SIZE					PT_FONT_SIZE_SMALL // �ʻ������С
	#define GBIME_STROKE_COLOR						PT_COLOR_BLACK    // �ʻ�����ɫ
	#define GBIME_COMPONENT_FONT_SIZE				PT_FONT_SIZE_SMALL // ��ݲ��������С
	#define GBIME_COMPONENT_COLOR					PT_COLOR_BLACK  // ��ݲ��׵���ɫ
	#define GBIME_ENGLISH_INPUTSTRING_COLOR			PT_COLOR_BLUE	// 9��Ӣ����������ִ�����ɫ
	#define GBIME_OUTPUT_STRING_COLOR				PT_COLOR_BLUE	// �����������ɫ
	#define GBIME_COMP_WIDTH						14 ///<���ʲ��׵Ŀ��
	#define GBIME_COMP_HEIGHT						14 ///<���ʲ��׵ĸ߶�
#elif defined(__MMI_MAINLCD_128X160__)   //LCD 128��x160�� LCD 128X128
	#define GBIME_VERTICAL_SPACING 					8  ///<��ֱ������ʾ���(��̬�����ַ��߶ȴ�ֱ������Ч�������)
	#define GBIME_SYLLABLE_SPACING 					8  ///<�������ּ��
	#define GBIME_CANDIDATE_SPACING 				8  ///<��ѡ���ּ��
	#define GBIME_ARROW_WIDTH						25
	#define GBIME_CAND_AREA_WIDTH  					(PT_GetLcdWidth() - GBIME_ARROW_WIDTH * 2) ///<���ʺ�ѡ��Ŀ��
	#define GBIME_SYLLABLE_DISPLAY_START_X			GBIME_ARROW_WIDTH  // ƴ����ʾ�Ŀ�ʼλ��X����
	#define GBIME_SYLLABLE_LINE_WIDTH				GBIME_CAND_AREA_WIDTH // ƴ�������
	#define GBIME_SYLLABLE_LINE_HEIGHT				21  // ƴ�����߶�
	#define GBIME_CANDIDATE_DISPLAY_START_X			GBIME_ARROW_WIDTH  // ��ѡ��ʾ�Ŀ�ʼλ�õ�X����
	#define GBIME_CANDIDATE_LINE_WIDTH				GBIME_CAND_AREA_WIDTH	// ��ѡ�����
	#define GBIME_CANDIDATE_LINE_HEIGHT				23  // ��ѡ���߶�
	#define GBIME_SYLLABLE_FONT_SIZE				PT_FONT_SIZE_SMALL // ���������С
	#define GBIME_SYLLABLE_NORMAL_FONT_COLOR		PT_COLOR_BLACK  // û��ѡ���еĺ�ѡƴ����ɫ
	#define GBIME_SYLLABLE_SELECTED_FONT_COLOR		PT_COLOR_BLUE    // ѡ���еĵ�ƴ������ɫ
	#define GBIME_CANDIDATE_FONT_SIZE				PT_FONT_SIZE_SMALL // ��ѡ�����С
	#define GBIME_CANDIDATE_NORMAL_FONT_COLOR		PT_COLOR_BLACK  // û��ѡ���еĺ�ѡ������ɫ
	#define GBIME_CANDIDATE_SELECTED_FONT_COLOR		PT_COLOR_BLUE    // ѡ���еĺ�ѡ������ɫ
	#define GBIME_STROKE_FONT_SIZE					PT_FONT_SIZE_SMALL // �ʻ������С
	#define GBIME_STROKE_COLOR						PT_COLOR_BLACK    // �ʻ�����ɫ
	#define GBIME_COMPONENT_FONT_SIZE				PT_FONT_SIZE_SMALL // ��ݲ��������С
	#define GBIME_COMPONENT_COLOR					PT_COLOR_BLACK  // ��ݲ��׵���ɫ
	#define GBIME_ENGLISH_INPUTSTRING_COLOR			PT_COLOR_BLUE	// 9��Ӣ����������ִ�����ɫ
	#define GBIME_OUTPUT_STRING_COLOR				PT_COLOR_BLUE	// �����������ɫ
	#define GBIME_COMP_WIDTH						14 ///<���ʲ��׵Ŀ��
	#define GBIME_COMP_HEIGHT						14 ///<���ʲ��׵ĸ߶�
#else
	#define GBIME_VERTICAL_SPACING 					8  ///<��ֱ������ʾ���(��̬�����ַ��߶ȴ�ֱ������Ч�������)
	#define GBIME_SYLLABLE_SPACING 					8  ///<�������ּ��
	#define GBIME_CANDIDATE_SPACING 				8  ///<��ѡ���ּ��
	#define GBIME_ARROW_WIDTH						33
	#define GBIME_CAND_AREA_WIDTH  					(PT_GetLcdWidth() - GBIME_ARROW_WIDTH * 2) ///<���ʺ�ѡ��Ŀ��
	#define GBIME_SYLLABLE_DISPLAY_START_X			GBIME_ARROW_WIDTH  // ƴ����ʾ�Ŀ�ʼλ��X����
	#define GBIME_SYLLABLE_LINE_WIDTH				GBIME_CAND_AREA_WIDTH // ƴ�������
	#define GBIME_SYLLABLE_LINE_HEIGHT				30  // ƴ�����߶�
	#define GBIME_CANDIDATE_DISPLAY_START_X			GBIME_ARROW_WIDTH  // ��ѡ��ʾ�Ŀ�ʼλ�õ�X����
	#define GBIME_CANDIDATE_LINE_WIDTH				GBIME_CAND_AREA_WIDTH	// ��ѡ�����
	#define GBIME_CANDIDATE_LINE_HEIGHT				33  // ��ѡ���߶�
	#define GBIME_SYLLABLE_FONT_SIZE				PT_FONT_SIZE_DEFAULT // ���������С
	#define GBIME_SYLLABLE_NORMAL_FONT_COLOR		PT_COLOR_BLACK  // û��ѡ���еĺ�ѡƴ����ɫ
	#define GBIME_SYLLABLE_SELECTED_FONT_COLOR		PT_COLOR_BLUE    // ѡ���еĵ�ƴ������ɫ
	#define GBIME_CANDIDATE_FONT_SIZE				PT_FONT_SIZE_DEFAULT // ��ѡ�����С
	#define GBIME_CANDIDATE_NORMAL_FONT_COLOR		PT_COLOR_BLACK  // û��ѡ���еĺ�ѡ������ɫ
	#define GBIME_CANDIDATE_SELECTED_FONT_COLOR		PT_COLOR_BLUE    // ѡ���еĺ�ѡ������ɫ
	#define GBIME_STROKE_FONT_SIZE					PT_FONT_SIZE_DEFAULT // �ʻ������С
	#define GBIME_STROKE_COLOR						PT_COLOR_BLACK    // �ʻ�����ɫ
	#define GBIME_COMPONENT_FONT_SIZE				PT_FONT_SIZE_SMALL // ��ݲ��������С
	#define GBIME_COMPONENT_COLOR					PT_COLOR_BLACK  // ��ݲ��׵���ɫ
	#define GBIME_ENGLISH_INPUTSTRING_COLOR			PT_COLOR_BLUE	// 9��Ӣ����������ִ�����ɫ
	#define GBIME_OUTPUT_STRING_COLOR				PT_COLOR_BLUE	// �����������ɫ
	#define GBIME_COMP_WIDTH						14 ///<���ʲ��׵Ŀ��
	#define GBIME_COMP_HEIGHT						14 ///<���ʲ��׵ĸ߶�
#endif

/*!
 * \brief ����
 */
#define GBIME_SYMBOL_COMMA_CHAR					0xFF0C // �����ַ�������
#define GBIME_SYMBOL_FULL_STOP_CHN_CHAR			0x3002 // ����ַ�������
#define GBIME_SYMBOL_FULL_STOP_ALP_CHAR			0x002e // ����ַ���.��

typedef struct __tag_GBIMEPoint
{
	GBINT16		x;
	GBINT16		y;
} GBIMEPoint, * PGBIMEPoint;

typedef struct __Tag_GBRect{
	GBINT16 left;
	GBINT16 top;
	GBINT16 right;
	GBINT16 bottom;
}GBRECT, * PGBRECT; ///< vc sizeof == 8

// ƽ̨�������������¼�����
enum GBPEventEnum{
	GBPEvent_None = 0

	// �����¼�
	,GBPEVT_KEY_DOWN
	,GBPEVT_KEY_UP
	,GBPEVT_KEY_LONG_PRESS
	,GBPEVT_KEY_REPEAT

	// �������¼�
	,GBPEVT_PEN_DOWN
	,GBPEVT_PEN_MOVE
	,GBPEVT_PEN_UP
	,GBPEVT_NUM
};

/*!
* \brief GBIME ���API�ķ���ֵ����
*/
enum GBIMEReturnEnum{
	GBIME_OK
	,GBIME_CONSUMED						///<����¼��Ѿ������뷨��ܴ����ⲿ���账��
	,GBIME_UNINITIALIZED				///<���û�г�ʼ��
	,GBIME_FAILED
	,GBIME_IGNORE						///<���Դ���
	,GBIME_INVALID_PARAM				///<�����Ƿ�
	,GBIME_NOT_SUPPORT_INPUT_MODE		///<���뷨��ܲ�֧�ֵ�����ģʽ(GBIMEInputMethodMapû����Ӧ��ӳ����)
	,GBIME_NOT_SUPPORT_LAYOUT_ID		///<���뷨��ܲ�֧�ֵ�Layout ID(GBIMEInputMethodMapû����Ӧ��ӳ����)
	,GBIME_LAYOUT_NOEXIST				///<Layout������
	,GBIME_LAYOUT_NOTMATCH				///<Layout��ƥ��
	,GBIME_LAYOUT_CONVERT_MSG_FAIL		///<Layoutת����Ϣʧ��
	,GBIME_EVENT_GROUP_IS_FULL			///<�¼�����(�޷��������)
	,GBIME_EVENT_GROUP_IS_NULL			///<�¼���Ϊ��(���κ��¼���Ҫ����)
	,GBIME_EVENT_GROUP_NOT_EXIST		///<ϵͳ�¼�����в�����Ҫ���ҵ��¼���
	,GBIME_OPEN_FILE_ERROR				///<�����ļ�ʧ��
	,GBIME_NOT_ENGINE_CONFIG_ID			///<û���ҵ�����������ID
};
typedef GBINT32	GBIMEReturn;

#endif //__GBIME_DEF_H_
