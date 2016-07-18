#ifndef __IME_COMMON_DATA_
#define __IME_COMMON_DATA_
#include "gbtype.h"
#include "gbdef.h"
#include "CLayoutInfo.h"

/*!
 *- brief guobi ime common data
 */
typedef struct __tag_IMECommonData
{
	GBBOOL				bInited;
	GBLPCVOID           pskinData;
	GBLPCVOID           pconfData;
#ifdef GBIME_CFG_CHS_SUPPORT // 中文支持
	GBLPCVOID           pimChnData;
#endif
#ifdef GBIME_CFG_EN_SUPPORT // 英文支持
	GBLPCVOID           pimAlpData;
#endif
	GBLPCVOID           pimEngineData;	
#if defined(__GBIME_HANDWRITE__) // 手写引擎支持
	GBLPCVOID           phwEngineData;
#endif
#ifdef GBIME_CFG_UDB_SUPPORT // 自造词支持
	GBLPCVOID			masterUDBData;
	GBLPCVOID			slaveUDBData;
#endif

	SkinData            skinInfo;
}IMECommonData, *PIMECommonData;

#endif //__IME_COMMON_DATA_
