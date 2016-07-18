/***************************************************************************
* File Name    :  CConfigure.h
* Corporation  :  Guobi Technology Ltd. GuangZhou China
* Copyright    :  Copyright Guobi Technology Ltd. And Subject to Confidentiality 
*                 Restrictions. This document is the confidential and proprietary 
*                 property of Guobi Technology Ltd.It is neither the document                  
*                 itself nor any part of it can be shared with any party  without 
*                 the written  permission. 
* Author       :  fanbo
* Date         :  2010-06-08
* Purpose      :  ����ͷ�ļ�
* Version      :  V1.0
* Last Changed List
* Name         |   date   |  Discription
****************************************************************************/
#ifndef __CONFIGURE_H_
#define __CONFIGURE_H_
#include "gbtype.h"
//#include "CEngineConfig.h"
#include "CLayoutConfigAbstract.h"

typedef enum
{
	TYPE_PEN
	,TYPE_PENCIL
	,TYPE_SYSTEM
	,COLOR_RED
	,COLOR_GREEN
	,COLOR_BLUE
	,COLOR_YELLOW
	,COLOR_PURPLE
	,COLOR_GRAY
	,COLOR_BLACK
	,WIDTH_LARGE
	,WIDTH_MEDIUM
	,WIDTH_SMALL
	,SPEED_SLOW
	,SPEED_MEDIUM
	,SPEED_FAST
}HANDWRITING_ENUM;

/* �������������
1.	ÿ��Layout���Ӧһ��Layout��ͼ�ࡢLayout������
2.	Layout������������Ƿ�֧�ֻ����л�(�������)����ӦDOWN����UP�¼���
3.	Layout�л��б����?
	(1)	��ЩLayout���������еģ���Щ���ǣ��磺ABC, 123�ɸ��õ�������֣�
	(2)	��Ҫ��ȷ���ǣ���ʵ�ڱ���ʱ�Ѿ���������ЩLayout�������.cʵ�ִ���μӱ��룬��������֧����ЩLayout��
	(3)	��ͨ��һ��Ĭ�����ñ���д�ֻ�֧�ֵ�ÿ�����ֵġ�Ĭ��Layout�л��б���
	(4)	ϵͳ��һ�ο������ص��ڴ�ȫ�����õ��ǡ�Ĭ��Layout�л��б���֮������ڴ��ж�̬�޸ģ����磺�ر�/��/��������
	(5)	�л���Ϣ������IDΪ��ʶ�洢���ļ�ϵͳ�У���ʽ�磺langID: CHN; layoutType: layoutKB status: ON/OFF, ��
4. ÿ��Layout������Ϣ��¼һ��
*/

/*!
 * \brief �����ܽṹ�� 
 */
typedef struct _tagCCConfig
{
	GBBOOL					bHasInit;							///<���������Ƿ��ѳ�ʼ��
//	CEngineConfig			engineConfig;						///<���������������

	// Layoutȫ������
	KEYBOARD_TYPE			m_keyboardType;						///<�������
	GBUINT16				m_preferLayoutID;					///<��ǰ���������µ�ȱʡLayoutID
	GBConfigRespondKeyType	m_respondKeyType;					///<��Ӧ�������(�������»��ǵ���ʱ��Ӧ)
	GBConfigSwitchType		m_bSupportSlidingSwitchLayout;		///<��������Ƿ�֧�ֻ����л�Layout	
}CConfig, *PCConfig;

/*!
 * \brief ���ع�����������
 * \param  pConfig
 * \return 
 * \note 
 * \example
 * \author fanbo
 * \date
 */
GBIMEReturn  GBConfig_Load(CConfig *pConfig);

/*!
 * \brief ж�ع�����������
 * \param  pConfig
 * \return 
 * \note 
 * \example
 * \author fanbo
 * \date
 */
GBIMEReturn  GBConfig_Unload(CConfig *pConfig);

/*!
 * \brief ������Ϣ������
 * \param pIMEEvent 
 * \return 
 */
GBIMEReturn GBConfig_HandleMessage(PGBIMEEvent pIMEEvent);

/*!
 * \brief ���ü������
 * \param pConfig 
 * \param keyboardType 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-8-19 20:18:24
*/
GBIMEReturn  GBConfig_SetKeyboardType(CConfig *pConfig, KEYBOARD_TYPE keyboardType);

/*!
 * \brief ���ü������
 * \param pConfig 
 * \param keyboardType 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-8-19 20:18:24
*/
KEYBOARD_TYPE  GBConfig_GetKeyboardType(CConfig *pConfig);

/*!
 * \brief ��ȡȱʡLayoutID
 * \param pConfig 
 * \return ȱʡLayoutID
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-8-19 20:18:24
*/
GBUINT16 GBConfig_GetPreferLayoutID(CConfig *pConfig);

/*!
 * \brief ��ȡĬ�ϵ��������LayoutID
 * \param pConfig 
 * \param kbType 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-12-8 10:11:25
*/
GBUINT16 GBConfig_GetDefLatestAlpLayoutID(CConfig *pConfig, KEYBOARD_TYPE kbType);

/*!
 * \brief Ĭ�ϵ��������LayoutID
 * \param pConfig 
 * \param kbType 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-11-22 19:22:10
 */
GBUINT16 GBConfig_GetDefLatestChnLayoutID(CConfig *pConfig, KEYBOARD_TYPE kbType);

#endif //__CONFIGURE_H_