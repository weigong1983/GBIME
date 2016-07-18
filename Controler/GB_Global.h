#ifndef __CONTROLER_H
#define __CONTROLER_H
#include "gbtype.h"
#include "gbdef.h"
#include "GBIMEDef.h"
#include "CLayoutInfo.h"
#include "IMECommonData.h"
#include "CIMEInterface.h"

extern CIMEInterface g_imeInterface;

#define Global_GetIMEInterface()             (&g_imeInterface)

/*!
 * \brief ��ȡ��������ʵ��������
 * \param void 
 * \return 
 */
#define Global_GetEnineInstance()            (&g_imeInterface.gbIMEngine)

/*!
 * \brief ��ȡ������д����ʵ��������
 * \param void 
 * \return 
 */
#if defined(__GBIME_HANDWRITE__)
#define Global_GetHWEnineInstance()		(g_imeInterface.pGBHWEngine)
#endif

/*!
 * \brief ��ȡLayout����ʵ��������
 * \param void 
 * \return 
 */
#define Global_GetLayoutCtl()				(g_imeInterface.pltCtl)

/*!
 * \brief ��ȡ����ʵ��������
 * \param void 
 * \return 
 */
#define Global_GetConfigInstance()		(g_imeInterface.pConfig)

/*!
 * \brief ��ȡͨ������ʵ��������
 * \param void 
 * \return 
 */
#define Global_GetIMEData()				(&g_imeInterface.comIMEData)

/*!
 * \brief ��ʼ�����뷨���ȫ�ֱ����ӿڶ���
 * \param void 
 * \return 
 */
void Global_InitIMEInterface(void);

/*!
 * \brief �������뷨���ȫ�ֱ����ӿڶ���
 * \param void 
 * \return 
 */
void Global_ReleaseIMEInterface(void);

/*!
 * \brief ��ܽӿڶ����Ƿ��ʼ��
 * \param void 
 * \return 
 */
GBBOOL Global_IMEInterfaceIsInited(void);

/*!
 * \brief ��ȡIMEģ������״̬
 * \param void 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-8-11 9:33:50
*/
GBBOOL Global_IMEIsConnected(void);

/*!
 * \brief ����IMEģ������״̬
 * \param GBBOOL 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-8-11 9:33:50
*/
void Global_SetIMEConnectedStatus(GBBOOL status);

/*!
 * \brief ���õ�ǰ�����Layout����
 * \param pltCurObject 
 * \return ���ص�ǰLayout ����
 */
void Global_SetCurrentLayoutObject(PCLayoutAbstract pltCurObject);

/*!
 * \brief Layout �Ķ����Ƿ��Ѿ�������
 * \param pltCurObject 
 * \return 
 */
GBINT32 Global_LyoutObjectIsLeaveByObject(PCLayoutAbstract pltCurObject);

/*!
 * \brief ����һ��Layout ����
 * \param pltCurObject 
 * \return 
 */
PCLayoutAbstract Global_SaveLayoutObject(PCLayoutAbstract pltCurObject);

/*!
 * \brief Ҫ��ʼ����Layout ID�Ƿ񱻱�����CIMEInterface::ltObjectArray
 * \param pIMEInterface 
 * \param ltID 
 * \return ���ر�����������Layout Object ָ��
 */
PCLayoutAbstract Global_LayoutObjectIsLeaveByID(GBUINT16 ltID);

/*!
 * \brief ���ص�ǰLayout Object
 * \param void 
 * \return 
 */
PCLayoutAbstract Global_GetCurrentLayoutObject(void);

/*!
 * \brief ����ǰһ��Layout Object
 * \param void 
 * \return 
 */
PCLayoutAbstract Global_GetPrevLayoutObject(void);

/*!
 * \brief ��ȡLayout���ڼ���״̬
 * \param void 
 * \return 
 */
GBBOOL Global_GetLayoutWinActiveStatus(void);

/*!
 * \brief ����Layout���ڼ���״̬
 * \param status 
 * \return 
 */
void Global_SetLayoutWinActiveStatus(GBBOOL status);

/*!
 * \brief ��ȡ��Сд״̬
 * \param void 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-10-22 10:04:44
*/
GBShiftCapStatus Global_GetShiftCapStatus(void);

/*!
 * \brief ���ô�Сд״̬
 * \param status 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-10-22 10:04:55
*/
void Global_SetShiftCapStatus(GBShiftCapStatus status);

/*!
 * \brief �������뷨Layout���ڵ���ʼ��ʾλ��(�����½��������Ļ�����꣬��Ϊ���Ͻǻ����ź�ѡ��߶ȱ仯)
 * \param pWinPos 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-21 11:28:23
 */
GBIMEReturn Global_SetWinPos(GBIMEPoint *pWinPos);

/*!
 * \brief ��ȡ���뷨Layout���ڵ���ʼ��ʾλ��(�����½��������Ļ�����꣬��Ϊ���Ͻǻ����ź�ѡ��߶ȱ仯)
 * \param pWinPos 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-21 11:28:37
 */
GBIMEReturn Global_GetWinPos(GBIMEPoint *pWinPos);

/*!
 * \brief ��ȡƤ������ָ��
 * \return 
 * \autor huanjin
 * \data 2010-8-5
 */
#define Global_GetSkinData()	(g_imeInterface.comIMEData.pskinData)

/*!
 * \brief ��ȡ��������ָ��
 * \return 
 * \autor huanjin
 * \data 2010-8-5
 */
GBLPCVOID Global_GetConfigData(void);

/*!
 * \brief ��ȡ������������ָ��
 * \return 
 * \autor huanjin
 * \data 2010-8-5
 */
GBLPCVOID Global_GetChnLangData(void);

/*!
 * \brief ��ȡAlp��������ָ��
 * \return 
 * \autor huanjin
 * \data 2010-8-5
 */
GBLPCVOID Global_GetAlpLangData(void);

/*!
 * \brief ��ȡ���뷨��������ָ��
 * \return 
 * \autor huanjin
 * \data 2010-8-5
 */
GBLPCVOID Global_GetIMEngineData(void);

/*!
 * \brief ��ȡ��д���뷨��������ָ��
 * \return 
 * \autor huanjin
 * \data 2010-8-5
 */
GBLPCVOID Global_GetHWEngineData(void);

/*!
 * \brief ��ȡ��UDB����ָ��
 * \return 
 * \autor huanjin
 * \data 2010-8-5
 */
GBLPCVOID Global_GetMasterUDBData(void);

/*!
 * \brief ��ȡ��UDB����ָ��
 * \return 
 * \autor huanjin
 * \data 2010-8-5
 */
GBLPCVOID Global_GetSlaveUDBData(void);

/*!
 * \brief ��ȡ��UDB���ݴ�С
 * \return 
 * \autor huanjin
 * \data 2010-8-5
 */
unsigned long Global_GetMasterUDBDataSize(void);

/*!
 * \brief ��ȡ��UDB���ݴ�С
 * \return 
 * \autor huanjin
 * \data 2010-8-5
 */
unsigned long Global_GetSlaveUDBDataSize(void);

/*!
 * \brief ��ȡ��д���뷨��������
 * \return 
 * \autor huanjin
 * \data 2010-8-5
 */
PSkinData Global_GetSkinInfoData(void);

/*!
 * \brief ��ȡ�������LayoutID(��: ���������뷨)
 * \param void 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-14 14:25:23
*/
GBUINT16 Global_GetLatestMasterLayoutID(void);

/*!
 * \brief ��ȡ���������LayoutID
 * \param void 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-14 14:25:23
*/
GBUINT16 Global_GetLatestChnLayoutID(void);

/*!
 * \brief ��ȡ���������LayoutID
 * \param void 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-14 14:25:23
*/
GBUINT16 Global_GetLatestAlpLayoutID(void);

/*!
 * \brief ���������LayoutID(����: ��Layout������Layout��Ӣ��Layout)
 * \param layoutID 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-14 14:28:37
*/
void Global_SaveLatestLayoutID(GBUINT16 layoutID);

/*!
 * \brief ���ݿ����Ϣת����������¼���
 * \param convertType 
 * \param wParam 
 * \param lParam 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-10-29 9:10:20
 */
GBIMEReturn Global_FillEventGroup(GBIMEConvertTypeEnum convertType, GBINT wParam, GBINT lParam);

/*!
 * \brief ���ݿ����Ϣת�������������¼���
 * \param convertType 
 * \param wParam 
 * \param lParam 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-10-29 9:10:20
 */
GBIMEReturn Global_FillPostEventGroup(GBIMEConvertTypeEnum convertType, GBINT wParam, GBINT lParam);

/*!
 * \brief �����¼��飬��������δ������䴦�������¼�
 * \param void 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-10-29 9:10:26
 */
GBIMEReturn Global_ResetEventGroup(void);

/*!
 * \brief ���ݿ����Ϣת����������¼���
 * \param void 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-10-29 9:10:31
 */
PGBIMEEventGroup Global_GetIMEEventGroup(void);

/*!
 * \brief ��ȡ�����Ϣ�����¼���
 * \param void 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-10-29 9:10:31
 */
PGBIMEEventGroup Global_GetIMEPostEventGroup(void);

/*!
 * \brief ������������л���������̱��
 * \param flag 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-10-29 18:21:12
 */
void Global_SetVk2KbSwitchFlag(GBBOOL flag);

/*!
 * \brief ��ȡ��������л���������̱��
 * \param void 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-10-29 18:21:29
 */
GBBOOL Global_GetVk2KbSwitchFlag(void);

/*!
 * \brief ������������л����������ʱ�û����µ���������ֵ
 * \param keyValue 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-10-29 18:22:07
 */
void Global_SetVk2KbSwitchEngineKeyValue(GBINT keyValue);

/*!
 * \brief ��ȡ��������л����������ʱ�û����µ���������ֵ
 * \param oid 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-10-29 18:22:10
 */
GBINT Global_GetVk2KbSwitchEngineKeyValue(void);

/*!
 * \brief ����Layout��ʾ���(��ʼ���趨==��Ļ���)
 * \param newWidth 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-10-31 12:58:27
 */
void Global_SetLayoutWinWidth(GBINT newWidth);

/*!
 * \brief ��ȡLayout��ʾ���
 * \param void 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-10-31 12:58:44
 */
GBINT Global_GetLayoutWinWidth(void);

/*!
 * \brief ���û�ȡLayout��ʾ�߶�(��̬����)
 * \param newHeight 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-10-31 12:58:27
 */
void Global_SetLayoutWinHeight(GBINT newHeight);

/*!
 * \brief ��ȡLayout��ʾ�߶�
 * \param void 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-10-31 12:58:44
 */
GBINT Global_GetLayoutWinHeight(void);

#endif //__CONTROLER_H
