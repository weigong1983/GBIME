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
#ifdef GBIME_CFG_CHS_SUPPORT // ����֧��
	GBLPCVOID           pimChnData;
#endif
#ifdef GBIME_CFG_EN_SUPPORT // Ӣ��֧��
	GBLPCVOID           pimAlpData;
#endif
	GBLPCVOID           pimEngineData;	
#if defined(__GBIME_HANDWRITE__) // ��д����֧��
	GBLPCVOID           phwEngineData;
#endif
#ifdef GBIME_CFG_UDB_SUPPORT // �����֧��
	GBLPCVOID			masterUDBData;
	GBLPCVOID			slaveUDBData;
#endif

	SkinData            skinInfo;
}IMECommonData, *PIMECommonData;

#endif //__IME_COMMON_DATA_
