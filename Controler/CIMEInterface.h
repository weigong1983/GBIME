/***************************************************************************
* File Name    :  CIMEInterface.h
* Corporation  :  Guobi Technology Ltd. GuangZhou China
* Copyright    :  Copyright Guobi Technology Ltd. And Subject to Confidentiality 
*                 Restrictions. This document is the confidential and proprietary 
*                 property of Guobi Technology Ltd.It is neither the document                  
*                 itself nor any part of it can be shared with any party  without 
*                 the written  permission. 
* Author       :  fanbo
* Date         :  2010-06-08
* Purpose      :  ��Ϣ����ͷ�ļ�
* Version      :  V1.0
* Last Changed List
* Name         |   date   |  Discription
****************************************************************************/
#ifndef __CMESSAGE_H_
#define __CMESSAGE_H_
#include "gbtype.h"
#include "gbdef.h"
#include "IMECommonData.h"
#include "CLayoutFactory.h"
#include "CLayoutControl.h"
#include "CSwitchLayout.h"
#include "CSwitchInputMethod.h"
#include "CConfigure.h"
#include "CGBEngine.h"
#include "CHWEngine.h"

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * \ �������ڴ��е����Layout��Ŀ[�����Ͳ���Ҫ��Ƶ���л���ʱ�򷴸�����]
 */
#define MAX_LEAVE_LAYOUT_OBJECT_NUM   10

typedef struct __tag_CIMEInterface
{
	GBBOOL				bFrameworkInited; ///<����Ƿ��ʼ��
	GBBOOL				bIMEConnected; ///<MTKƽ̨���뷨ģ���Ƿ�����(mmi_imc_connect/mmi_imc_disconnect����)
	IMECommonData       comIMEData;

	GBBOOL				layoutWinActiveStatus; ///<Layout�����Ƿ��ڼ���״̬
	GBIMEPoint			layoutWinStartPos; ///< layout���ڵ���ʼ��ʾλ��(�����½��������Ļ�����꣬��Ϊ���Ͻǻ����ź�ѡ��߶ȱ仯)
	GBINT				layoutWinWidth;///<Layout��ʾ���(��ʼ���趨==��Ļ���)
	GBINT				layoutWinHeight;///<Layout��ʾ�߶�(��̬����)

	GBShiftCapStatus	shiftCapStatus;///<��Сд״̬���(һ����ѭ���ʵ�ֵı�֤��ѡ��Layout���̴�Сдһ�µķ���) 

	GBBOOL				bVk2KbSwitchFlag;///<��������л���������̱��
	GBINT				Vk2KbSwitchEngineKeyValue;///<��������л����������ʱ�û����µ���������ֵ

	GBUINT16			latestMasterLayoutID;///<�������LayoutID(��: ���������뷨)
	GBUINT16			latestChnNumLayoutID;///<������������ּ���LayoutID
	GBUINT16			latestChnQwertyLayoutID;///<���������Qwerty����LayoutID
	GBUINT16			latestAlpNumLayoutID;///<������������ּ���LayoutID
	GBUINT16			latestAlpQwertyLayoutID;///<���������Qwerty����LayoutID
	PCLayoutAbstract    pltCurObject;
	PCLayoutAbstract    pltPreObject;
	PCLayoutAbstract    ltObjectArray[MAX_LEAVE_LAYOUT_OBJECT_NUM];
	CLayoutControl		*pltCtl;
	CSwitchLayout		*pschLt;
	CSWitchInputMethod	*pschIM;
	CFactory			*pfactory;
	CConfig				*pConfig;
	CGBEngine			gbIMEngine; // ʹ��ϵͳ�Ķ�̬�ڴ�������2KB
#if defined(__GBIME_HANDWRITE__)
	PGBHWEngine 		pGBHWEngine;
#endif
	GBIMEEventGroup		sysImeEventGroup;//<��ƽ̨�¼�����Layout�������gbIMEEventConvertMapȡ�õ��¼���
	GBIMEEventGroup		sysImePostEventGroup;//<Layout��������в����ĺ����¼���
	GBBOOL				sysImeEventGroupLock;///<��ֹ����ط�ͬʱ���sysImeEventGroup
	GBBOOL				sysImePostEventGroupLock;///<��ֹ����ط�ͬʱ���sysImePostEventGroup
}CIMEInterface, *PCIMEInterface;

/*!
 * \brief ��ʼ����ܽӿڶ���
 * \param void 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-8-11 9:33:50
*/
GBAPI void Interface_InitIME(void);

/*!
 * \brief �жϿ�ܽӿڶ����Ƿ��Ѿ���ʼ��
 * \param void 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-8-11 9:33:50
*/
GBAPI GBBOOL Interface_IsIMEInited(void);

/*!
 * \brief ��ȡIMEģ������״̬
 * \param void 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-8-11 9:33:50
*/
GBAPI GBBOOL Interface_IsIMEConnected(void);

/*!
 * \brief ����IMEģ������״̬
 * \param GBBOOL 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-8-11 9:33:50
*/
GBAPI void Interface_SetIMEConnectedStatus(GBBOOL status);

/*!
 * \brief ��Ϣ������(�ɾ���Layoutʵ��������ƽ̨��Ϣת��ΪIME�����Ϣ)
 * \param nPlatformEvent 
 * \param wParam 
 * \param lParam 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-6-22 14:58:51
 */
GBAPI GBINT32 Handle_GBIMEInterface(GBINT nPlatformEvent, GBINT wParam, GBINT lParam);

/*!
 * \brief IME �����Ϣ������
 * \param pIMEEvent 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-6-22 14:58:27
 */
GBAPI GBINT32 Handle_GBIMEMessage(PGBIMEEvent pIMEEvent);

/*!
 * \brief ��ȡ��������ʵ������ָ��
 * \param void 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-26 15:31:24
 */
PCGBEngine Interface_GetEnineInstance(void);

/*!
 * \brief �������뷨Layout���ڵ���ʼ��ʾλ��(�����½��������Ļ�����꣬��Ϊ���Ͻǻ����ź�ѡ��߶ȱ仯)
 * \param pWinPos 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-21 11:28:23
 */
GBIMEReturn Interface_SetWinPos(GBIMEPoint *pWinPos);

/*!
 * \brief ��ȡ���뷨Layout���ڵ����Ͻ�λ��(���½���ʼ��ʾλ�� + Layout������ʾ�߶�)
 * \param pWinPos 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-21 11:28:37
 */
GBIMEReturn Interface_GetWinPos(GBIMEPoint *pWinPos);

/*!
 * \brief �������뷨���ڵ���ʾ�߶�
 * \param newHeight 
 * \note �߶ȷ����仯ʱ��Ҫ֪ͨMTKƽ̨�༭������ˢ�£�����ᵼ�������������뷨����ˢ�²�����
 * \return 
 */
void Interface_SetWinHeight(GBINT newHeight);

/*!
 * \brief ��ȡ���뷨���ڵ���ʾ�߶�
 * \param void 
 * \note �߶ȷ����仯ʱ��Ҫ֪ͨMTKƽ̨�༭������ˢ�£�����ᵼ�������������뷨����ˢ�²�����
 * \return 
 */
GBINT Interface_GetWinHeight(void);

/*!
 * \brief ��ȡ���뷨���ڵ���ʾ���
 * \param newWidth 
 * \note ��ȷ����仯ʱ��Ҫ֪ͨMTKƽ̨�༭������ˢ�£�����ᵼ�������������뷨����ˢ�²�����
 * \return 
 */
void Interface_SetWinWidth(GBINT newWidth);

/*!
 * \brief ��ȡ���뷨���ڵ���ʾ���
 * \param void 
 * \note ��ȷ����仯ʱ��Ҫ֪ͨMTKƽ̨�༭������ˢ�£�����ᵼ�������������뷨����ˢ�²�����
 * \return 
 */
GBINT Interface_GetWinWidth(void);

/*!
 * \brief ��ȡLayout���ڼ���״̬
 * \param void 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-8-11 11:45:39
*/
GBBOOL Interface_GetLayoutWinActiveStatus(void);

/*!
 * \brief ����Layout���ڼ���״̬
 * \param status 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-8-11 11:45:39
*/
void Interface_SetLayoutWinActiveStatus(GBBOOL status);

#ifdef __cplusplus
};
#endif
#endif //__CMESSAGE_H_
