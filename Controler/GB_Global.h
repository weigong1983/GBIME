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
 * \brief 获取国笔引擎实例对象句柄
 * \param void 
 * \return 
 */
#define Global_GetEnineInstance()            (&g_imeInterface.gbIMEngine)

/*!
 * \brief 获取国笔手写引擎实例对象句柄
 * \param void 
 * \return 
 */
#if defined(__GBIME_HANDWRITE__)
#define Global_GetHWEnineInstance()		(g_imeInterface.pGBHWEngine)
#endif

/*!
 * \brief 获取Layout控制实例对象句柄
 * \param void 
 * \return 
 */
#define Global_GetLayoutCtl()				(g_imeInterface.pltCtl)

/*!
 * \brief 获取配置实例对象句柄
 * \param void 
 * \return 
 */
#define Global_GetConfigInstance()		(g_imeInterface.pConfig)

/*!
 * \brief 获取通用数据实例对象句柄
 * \param void 
 * \return 
 */
#define Global_GetIMEData()				(&g_imeInterface.comIMEData)

/*!
 * \brief 初始化输入法框架全局变量接口对象
 * \param void 
 * \return 
 */
void Global_InitIMEInterface(void);

/*!
 * \brief 销毁输入法框架全局变量接口对象
 * \param void 
 * \return 
 */
void Global_ReleaseIMEInterface(void);

/*!
 * \brief 框架接口对象是否初始化
 * \param void 
 * \return 
 */
GBBOOL Global_IMEInterfaceIsInited(void);

/*!
 * \brief 获取IME模块连接状态
 * \param void 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-8-11 9:33:50
*/
GBBOOL Global_IMEIsConnected(void);

/*!
 * \brief 设置IME模块连接状态
 * \param GBBOOL 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-8-11 9:33:50
*/
void Global_SetIMEConnectedStatus(GBBOOL status);

/*!
 * \brief 设置当前激活的Layout对象
 * \param pltCurObject 
 * \return 返回当前Layout 对象
 */
void Global_SetCurrentLayoutObject(PCLayoutAbstract pltCurObject);

/*!
 * \brief Layout 的对象是否已经被保留
 * \param pltCurObject 
 * \return 
 */
GBINT32 Global_LyoutObjectIsLeaveByObject(PCLayoutAbstract pltCurObject);

/*!
 * \brief 保留一个Layout 对象
 * \param pltCurObject 
 * \return 
 */
PCLayoutAbstract Global_SaveLayoutObject(PCLayoutAbstract pltCurObject);

/*!
 * \brief 要初始化的Layout ID是否被保留在CIMEInterface::ltObjectArray
 * \param pIMEInterface 
 * \param ltID 
 * \return 返回被保留下来的Layout Object 指针
 */
PCLayoutAbstract Global_LayoutObjectIsLeaveByID(GBUINT16 ltID);

/*!
 * \brief 返回当前Layout Object
 * \param void 
 * \return 
 */
PCLayoutAbstract Global_GetCurrentLayoutObject(void);

/*!
 * \brief 返回前一个Layout Object
 * \param void 
 * \return 
 */
PCLayoutAbstract Global_GetPrevLayoutObject(void);

/*!
 * \brief 获取Layout窗口激活状态
 * \param void 
 * \return 
 */
GBBOOL Global_GetLayoutWinActiveStatus(void);

/*!
 * \brief 设置Layout窗口激活状态
 * \param status 
 * \return 
 */
void Global_SetLayoutWinActiveStatus(GBBOOL status);

/*!
 * \brief 获取大小写状态
 * \param void 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-10-22 10:04:44
*/
GBShiftCapStatus Global_GetShiftCapStatus(void);

/*!
 * \brief 设置大小写状态
 * \param status 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-10-22 10:04:55
*/
void Global_SetShiftCapStatus(GBShiftCapStatus status);

/*!
 * \brief 设置输入法Layout窗口的起始显示位置(即左下角相对于屏幕的坐标，因为左上角会随着候选框高度变化)
 * \param pWinPos 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-21 11:28:23
 */
GBIMEReturn Global_SetWinPos(GBIMEPoint *pWinPos);

/*!
 * \brief 获取输入法Layout窗口的起始显示位置(即左下角相对于屏幕的坐标，因为左上角会随着候选框高度变化)
 * \param pWinPos 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-21 11:28:37
 */
GBIMEReturn Global_GetWinPos(GBIMEPoint *pWinPos);

/*!
 * \brief 获取皮肤数据指针
 * \return 
 * \autor huanjin
 * \data 2010-8-5
 */
#define Global_GetSkinData()	(g_imeInterface.comIMEData.pskinData)

/*!
 * \brief 获取配置数据指针
 * \return 
 * \autor huanjin
 * \data 2010-8-5
 */
GBLPCVOID Global_GetConfigData(void);

/*!
 * \brief 获取中文语言数据指针
 * \return 
 * \autor huanjin
 * \data 2010-8-5
 */
GBLPCVOID Global_GetChnLangData(void);

/*!
 * \brief 获取Alp语言数据指针
 * \return 
 * \autor huanjin
 * \data 2010-8-5
 */
GBLPCVOID Global_GetAlpLangData(void);

/*!
 * \brief 获取输入法引擎数据指针
 * \return 
 * \autor huanjin
 * \data 2010-8-5
 */
GBLPCVOID Global_GetIMEngineData(void);

/*!
 * \brief 获取手写输入法引擎数据指针
 * \return 
 * \autor huanjin
 * \data 2010-8-5
 */
GBLPCVOID Global_GetHWEngineData(void);

/*!
 * \brief 获取主UDB数据指针
 * \return 
 * \autor huanjin
 * \data 2010-8-5
 */
GBLPCVOID Global_GetMasterUDBData(void);

/*!
 * \brief 获取辅UDB数据指针
 * \return 
 * \autor huanjin
 * \data 2010-8-5
 */
GBLPCVOID Global_GetSlaveUDBData(void);

/*!
 * \brief 获取主UDB数据大小
 * \return 
 * \autor huanjin
 * \data 2010-8-5
 */
unsigned long Global_GetMasterUDBDataSize(void);

/*!
 * \brief 获取辅UDB数据大小
 * \return 
 * \autor huanjin
 * \data 2010-8-5
 */
unsigned long Global_GetSlaveUDBDataSize(void);

/*!
 * \brief 获取手写输入法引擎数据
 * \return 
 * \autor huanjin
 * \data 2010-8-5
 */
PSkinData Global_GetSkinInfoData(void);

/*!
 * \brief 获取最近的主LayoutID(如: 主语言输入法)
 * \param void 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-14 14:25:23
*/
GBUINT16 Global_GetLatestMasterLayoutID(void);

/*!
 * \brief 获取最近的中文LayoutID
 * \param void 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-14 14:25:23
*/
GBUINT16 Global_GetLatestChnLayoutID(void);

/*!
 * \brief 获取最近的拉丁LayoutID
 * \param void 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-14 14:25:23
*/
GBUINT16 Global_GetLatestAlpLayoutID(void);

/*!
 * \brief 保存最近的LayoutID(包括: 主Layout、中文Layout、英文Layout)
 * \param layoutID 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-14 14:28:37
*/
void Global_SaveLatestLayoutID(GBUINT16 layoutID);

/*!
 * \brief 根据框架消息转换类型填充事件组
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
 * \brief 根据框架消息转换类型填充后处理事件组
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
 * \brief 重置事件组，避免由于未重新填充处理到残留事件
 * \param void 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-10-29 9:10:26
 */
GBIMEReturn Global_ResetEventGroup(void);

/*!
 * \brief 根据框架消息转换类型填充事件组
 * \param void 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-10-29 9:10:31
 */
PGBIMEEventGroup Global_GetIMEEventGroup(void);

/*!
 * \brief 获取框架消息后处理事件组
 * \param void 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-10-29 9:10:31
 */
PGBIMEEventGroup Global_GetIMEPostEventGroup(void);

/*!
 * \brief 设置虚拟键盘切换到物理键盘标记
 * \param flag 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-10-29 18:21:12
 */
void Global_SetVk2KbSwitchFlag(GBBOOL flag);

/*!
 * \brief 获取虚拟键盘切换到物理键盘标记
 * \param void 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-10-29 18:21:29
 */
GBBOOL Global_GetVk2KbSwitchFlag(void);

/*!
 * \brief 设置虚拟键盘切换到物理键盘时用户按下的物理按键键值
 * \param keyValue 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-10-29 18:22:07
 */
void Global_SetVk2KbSwitchEngineKeyValue(GBINT keyValue);

/*!
 * \brief 获取虚拟键盘切换到物理键盘时用户按下的物理按键键值
 * \param oid 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-10-29 18:22:10
 */
GBINT Global_GetVk2KbSwitchEngineKeyValue(void);

/*!
 * \brief 设置Layout显示宽度(初始化设定==屏幕宽度)
 * \param newWidth 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-10-31 12:58:27
 */
void Global_SetLayoutWinWidth(GBINT newWidth);

/*!
 * \brief 获取Layout显示宽度
 * \param void 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-10-31 12:58:44
 */
GBINT Global_GetLayoutWinWidth(void);

/*!
 * \brief 设置获取Layout显示高度(动态更新)
 * \param newHeight 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-10-31 12:58:27
 */
void Global_SetLayoutWinHeight(GBINT newHeight);

/*!
 * \brief 获取Layout显示高度
 * \param void 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-10-31 12:58:44
 */
GBINT Global_GetLayoutWinHeight(void);

#endif //__CONTROLER_H
