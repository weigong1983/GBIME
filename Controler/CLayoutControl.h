/***************************************************************************
* File Name    :  CLayoutControl.h
* Corporation  :  Guobi Technology Ltd. GuangZhou China
* Copyright    :  Copyright Guobi Technology Ltd. And Subject to Confidentiality 
*                 Restrictions. This document is the confidential and proprietary 
*                 property of Guobi Technology Ltd.It is neither the document                  
*                 itself nor any part of it can be shared with any party  without 
*                 the written  permission. 
* Author       :  fanbo
* Date         :  2010-06-08
* Purpose      :  Layout�ڲ��߼�����ͷ�ļ�
* Version      :  V1.0
* Last Changed List
* Name         |   date   |  Discription
****************************************************************************/
#ifndef __CLAYOUTCONTROL_H_
#define __CLAYOUTCONTROL_H_
#include "gbtype.h"
#include "GBIMEDef.h"
#include "gbft_def.h"
#include "CGBEngine.h"

// ��Layout��ͼ��ʾ��Ҫ���õĹ����������������֯�����½ṹ�壬Ȼ��֪ͨ��ͼˢ��
typedef struct tagEngineOutputInfo
{	
	GBPCWCHAR  						pInputString;///<������������
	/**< ��Ҫ�������ַ���, ָ����ַ�����ʱ���ʾû����Ҫ�������ַ�, �û��������֮������մ��ַ��� */
	GBWCHAR	*						pUpScreenStr;	
	//GBU32							nStatusFlag;	///<���浱ǰ��״̬
	GBBOOL							bCandWindOpen; ///<��ѡ�򼤻�״̬
	GBBOOL							bInputEmpty; ///<���浱ǰ�������Ƿ�Ϊ��
	
	// ��ѡ���
	GBU16 							nCandNum;		/**< ��ѡ���� */
	GBPCWCHAR 						*ppCandidates;	/** (output)��ѡ�ִʶ�ά����*/

	// �������
	GBU8 							nSyllableNum;/**< ��ǰҳƴ�����ڵĸ�����ƴ�������GBSetPageOption
												 ����ƴ���ķ�ҳ��Ϣ����ҳ�� �����Ҫ�������е�ƴ����ѡ��
												 ��ֻ��Ҫ��GBSetPageOption ��ƴ���ķ�ҳ����Ϊ����GB_MAX_SYLLABLES��ƴ������*/	
	GBU8 							nCurrentSyllableIndex;	/**< ��ǰѡ��ƴ�����±� = nSyllableIndex - nSyllablePageStartIndex */	
	GBPCWCHAR						*ppSyllables;/**< (GBMMI)ƴ������ ֻ�Ǹ��ݷ�ҳ��Ϣ, (GBAPI)���п��ܵ�ƴ��*/

	// �ʻ��������
	GBPWCHAR 						pStroke;/**< �ʻ������봮 */
	GBPWCHAR 						pComponents;/**< ���������봮 */
	GBPCWCHAR						pOutputString;			///<�������뷨����ʵ�ʱ���Ѿ�ѡ���˵Ĵ����Լ���������봮

	///(GBMMI)Ӧ����ʾΪInline Edit�ĺ�ѡ, һ����MultiTap��MultiTap Symbols�Լ��޺�ѡ���Alphabeticģʽ��Ч
	GBLPCWCHAR						pCompStr;///<����pAuxInfo->pOutputString + pAuxInfo->nSelZiCount;

	// ��ѡ���м�ͷ��ָʾ���
	GBBOOL							bUpScreenArrow;		///<������ͷ
	GBBOOL							bCandArrowUp;		///<��ѡ��ʾ �� ��ͷ
	GBBOOL							bCandArrowDown;		///<��ѡ��ʾ �� ��ͷ
	GBBOOL							bCandArrowLeft;		///<��ѡ��ʾ �� ��ͷ
	GBBOOL							bCandArrowRight;	///<��ѡ��ʾ �� ��ͷ
	GBBOOL							bSyllableArrowLeft; ///<Ambiguous Chinese only, ƴ����ҳ��ʾ���ͷ
	GBBOOL							bSyllableArrowRight;///<Ambiguous Chinese only, ƴ����ҳ��ʾ�Ҽ�ͷ

	//������������ڶ��к�ѡ�����, ��ȫ��������
	///ÿ���к�ѡ�Ŀ�ʼλ��, pRowStart ָ��һ������ΪGB_CAND_MAX_LINE_COUNT + 1��Byte����\ref GB_CAND_MAX_LINE_COUNT
	GBPBYTE							pRowStart;		 //GBMMIʹ��	
	GBI8							nSelCol;		///<��ǰѡ����� GBMMIʹ��
	GBI8							nSelRow;		///<��ǰѡ����� GBMMIʹ��
	GBU8							nRowCount;		///<��ǰҳ��Ч������ GBMMIʹ��

	// ״̬���
	GBBOOL							isSelectedStatus;	///<�Ƿ�Ϊѡ��״̬
	GBBOOL							isMultiTapInputMode; ///<�Ƿ�Mutitap����ģʽ
	GBBOOL							isSwitchNextInputMode;	//<�Ƿ�Ϊ�л�����ģʽ
	GBBOOL							isSymbolCandType;	///<�Ƿ�Ϊ����ģʽ
	//GBShiftCapStatus                shiftCapStatus; ��ܼ�¼��״̬
}EngineOutputInfo, *PEngineOutputInfo;

typedef struct _tag_CLayoutControl
{
	EngineOutputInfo engineOutputInfo; //  layout��ʾ��Ҫ�õ������������Ϣ
}CLayoutControl, *PCLayoutControl;

/*!
 * \brief Layout����ģ��������������Ϣ
 * \param void 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-07-28
 */
GBIMEReturn CLayoutControl_UpdateOutputInfo(void);

/*!
 * \brief ��ȡLayout����ģ��ά�������������Ϣ
 * \param void 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-07-28
 */
PEngineOutputInfo CLayoutControl_GetEngineOutputInfo(void);

/*!
 * \brief Layout��Ϣ������
 * \param pIMEEvent 
 * \return 
 */
GBIMEReturn CLayoutControl_HandleMessage(PGBIMEEvent pIMEEvent);

/*!
 * \brief ��ȡ��ѡ���Ƿ��
 * \param void 
 * \return 
 * \note 
 * \example
 * \author ZhaoKun
 * \date 2010-10-22
 */
GBBOOL CLayoutControl_IsOpenCandWind(void);

#endif //__CLAYOUTCONTROL_H_