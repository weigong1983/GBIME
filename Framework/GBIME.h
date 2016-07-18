/***************************************************************************
* File Name    :  GBIME.h
* Corporation  :  Guobi Technology Ltd. GuangZhou China
* Copyright    :  Copyright Guobi Technology Ltd. And Subject to Confidentiality 
*                 Restrictions. This document is the confidential and proprietary 
*                 property of Guobi Technology Ltd.It is neither the document                  
*                 itself nor any part of it can be shared with any party  without 
*                 the written  permission. 
* Author       :  weizhiping
* Date         :  2010-06-08
* Purpose      :  ���뷨�������㣨�����ܣ��ṩAPI��ƽ̨���ã�ƽ̨����֪�����ʵ��ϸ�ڣ�
* Version      :  V1.0
* Last Changed List
* Name         |   date   |  Discription
****************************************************************************/
#ifndef __GBIME_H_
#define __GBIME_H_
#include "gbtype.h"
#include "GBIMEDef.h"
#include "CIMEInterface.h"
#include "CConfigure.h"

/*!
* \brief �������뷨��ܴ���
* \param void
* \return 
* \note 
* \example
* \author fanbo
* \date 2010-06-08
*/
GBIMEAPI GBIMEReturn GBIME_Create(void);

/*!
 * \brief �������뷨��ܺ�ƽ̨�����������(�л�����ѡĬ������ģʽ��������ӦLayout��ʾ���)
 * \param platformInputMode ƽ̨�������뷨ʱ��Ĭ�����뷨ģʽ
 * \return 
 * \note �����ȵ���GBIME_Create
 * \example
 * \author weizhiping
 * \date 2010-8-20 10:20:58
*/
GBIMEAPI GBIMEReturn GBIME_Connect(GBINT platformInputMode);

/*!
 * \brief �������뷨����л�����ģʽ
 * \param platformInputMode
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-8-20 10:20:58
*/
GBIMEAPI GBIMEReturn GBIME_SwitchInputMode(GBINT platformInputMode);

/*!
 * \brief �������뷨��ܺ�ƽ̨�������Ͽ�����
 * \param void 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-8-31 14:17:17
*/
GBIMEAPI GBIMEReturn GBIME_Disconnect(void);

/*!
* \brief �������뷨�������
* \return 
* \note 
* \example
* \author fanbo
* \date 2010-06-08
*/
GBIMEAPI GBIMEReturn GBIME_Destroy(void);

/*!
 * \brief ƽ̨���뷨������������´�����
 * \param key_code 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-22 14:09:23
 */
GBIMEAPI GBIMEReturn GBIME_KeyDownHandler(GBINT key_code);

/*!
 * \brief ƽ̨���뷨�����������������
 * \param key_code 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-22 14:09:23
 */
GBIMEAPI GBIMEReturn GBIME_KeyUpHandler(GBINT key_code);

/*!
 * \brief ƽ̨���뷨�������������������
 * \param key_code 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-22 14:09:23
 */
GBIMEAPI GBIMEReturn GBIME_KeyLongPressHandler(GBINT key_code);

/*!
 * \brief ƽ̨���뷨������������ٶ̰�������
 * \param key_code 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-22 14:09:23
 */
GBIMEAPI GBIMEReturn GBIME_KeyRepeatHandler(GBINT key_code);

/*!
 * \brief ƽ̨���뷨��ܴ��������´�����
 * \param x
 * \param y
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-22 14:11:58
*/
GBIMEAPI GBIMEReturn GBIME_PenDownHandler(GBINT x, GBINT y);

/*!
 * \brief ƽ̨���뷨��ܴ�������������
 * \param x
 * \param y
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-22 14:11:58
*/
GBIMEAPI GBIMEReturn GBIME_PenUpHandler(GBINT x, GBINT y);

/*!
 * \brief ƽ̨���뷨��ܴ�����Move������
 * \param x
 * \param y
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-22 14:11:58
*/
GBIMEAPI GBIMEReturn GBIME_PenMoveHandler(GBINT x, GBINT y);

/*!
 * \brief �������뷨���ǿ��ˢ����ʾ
 * \return 
 * \note 
 * \example
 * \author fanbo
 * \date 2010-06-08
 */
GBIMEAPI GBIMEReturn GBIME_Show(void);

/*!
 * \brief �������뷨�������
 * \return 
 * \note 
 * \example
 * \author fanbo
 * \date 2010-06-08
 */
GBIMEAPI GBIMEReturn GBIME_Hide(void);

/*!
 * \brief �������뷨���ǿ��ˢ����ʾ
 * \param void 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-17 11:17:16
*/
GBIMEAPI GBIMEReturn GBIME_Paint(void);

/*!
 * \brief �������뷨���ڵĵ���ʼ��ʾλ��(�����½��������Ļ�����꣬��Ϊ���Ͻǻ����ź�ѡ��߶ȱ仯)
 * \param pWinPos 
 * \note ���ڿ�ܳ�ʼ��ʱ���ã�Ҳ���Զ�̬����
 * \return 
 */
GBIMEAPI GBIMEReturn GBIME_SetWinPos(GBIMEPoint *pWinPos);

/*!
 * \brief ��ȡ���뷨���ڵ����Ͻ�λ��(���½���ʼ��ʾλ�� + Layout������ʾ�߶�)
 * \param pWinPos 
 * \note ��ܴ�����ʾ״̬����Ч
 * \return 
 */
GBIMEAPI GBIMEReturn GBIME_GetWinPos(GBIMEPoint *pWinPos);

/*!
 * \brief ��ȡ���뷨���ڵ���ʾ
 * \param void 
 * \note �߶ȷ����仯ʱ��Ҫ֪ͨMTKƽ̨�༭������ˢ�£�����ᵼ�������������뷨����ˢ�²�����
 * \return 
 */
GBIMEAPI GBINT GBIME_GetWinHeight(void);

/*!
 * \brief ��ȡ���뷨���ڵ���ʾ
 * \param void 
 * \note ��ȷ����仯ʱ��Ҫ֪ͨMTKƽ̨�༭������ˢ�£�����ᵼ�������������뷨����ˢ�²�����
 * \return 
 */
GBIMEAPI GBINT GBIME_GetWinWidth(void);

/*!
 * \brief �ж�������Ƿ�����Layout����������
 * \param point 
 * \return 
 */
GBIMEAPI GBBOOL GBIME_PointIsInLayoutRect(GBIMEPoint point);

/*!
 * \brief �ж����뷨����Ƿ���Ҫ����˴�������Ϣ
 * \param nPenEvent 
 * \param point 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-11-22 20:07:44
 */
 GBIMEAPI GBBOOL GBIME_ShouldHandlePenEvent(GBINT nPenEvent, GBIMEPoint point);

/*!
 * \brief ��ȡ��ǰ��������
 * \param void 
 * \return KEYBOARD_TYPE
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-10-28
*/
GBIMEAPI KEYBOARD_TYPE GBIME_GetKeyboardType(void);

#if __TEST_LOG_TO_FILE__ > 0
//����ʱʹ�ã�ר�Ŵ�ӡlog
GBIMEAPI GBINT GBIME_OpenFile(char* filePath);
GBIMEAPI void GBIME_CloseFile();
#endif

/*!
 * \brief ���ù������뷨����������
 * \param input_mode 
 * \note 
 * \return 
 */
//void GBIME_SetVirtualKeyboard(mmi_imm_input_mode_enum input_mode);
#endif //__GBIME_H_