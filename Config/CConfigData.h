/***************************************************************************
* File Name    :  CConfigData.h
* Corporation  :  Guobi Technology Ltd. GuangZhou China
* Copyright    :  Copyright Guobi Technology Ltd. And Subject to Confidentiality 
*                 Restrictions. This document is the confidential and proprietary 
*                 property of Guobi Technology Ltd.It is neither the document                  
*                 itself nor any part of it can be shared with any party  without 
*                 the written  permission. 
* Author       :  ZhaoKun
* Date         :  2010-08-23
* Purpose      :  配置数据读取文件
* Version      :  V1.0
* Last Changed List
* Name         |   date   |  Discription
****************************************************************************/
#ifndef __CONFIGUREDATA_H_
#define __CONFIGUREDATA_H_

#include "gbtype.h"
#include "gbdef.h"
#include "gbim.h"
#ifdef WIN32
#define _CRT_SECURE_NO_DEPRECATE
#endif
#include "stdio.h"
#include "stdlib.h"

#include "PlatformTools.h"

//#if __MTK__ == 0
#include "GBIMEResDef.h"
//#endif


#define GB_MAX_FILE_NAME_LEN	(256)

#define SIGNATURE "CFG"

#ifndef CONFIG_VERSION
#define CONFIG_VERSION      0x00000001
#endif

#define CONFIG_STRUCTURE_RESERVED_LEN		32	///< reserved data length
#define CONFIG_DATA_VALUE_MAX_SIZE			16

#define STR(id)	#id
#define STR_W(id)	L###id // 解决字符串拼接问题
#define FSIZ( type, field ) sizeof( ((type *) 0)->field )	///<获取结构体变量sizeof


enum ConfigTypeIDEnum
{
	COMMON_CONFIG = 0x100	//<公共配置类型，累加于EngineIDEnum
	,HANDWRITING_CONFIG
	,DEFAULT_IM_CONFIG
};

enum ConfigValueTypeEnum
{
	CONFIG_VALUE_TYPE_ENUM_START
	,LIST_TYPE		//列表选项
	,RADIO_TYPE		//<单选框
	,CHECK_TYPE		//<复选框
	,INT_TYPE		//<整型或bool类型
	,STRING_TYPE	//<字符串类型
	,ONLY_TEXT_DLG_TYPE	//<只读文本窗口类型

	,CONFIG_VALUE_TYPE_ENUM_END
};

enum ProductEnum
{
	// 词语产品
	PRODUCE_NUMPAD_1
	,PRODUCE_NUMPAD_2
	,PRODUCE_NUMPAD_3
	,PRODUCE_NUMPAD_4

	,PRODUCE_QWERTY_1
	,PRODUCE_QWERTY_2
	,PRODUCE_QWERTY_3
	,PRODUCE_QWERTY_4

	,PRODUCE_HALF_QWERTY_1
	,PRODUCE_HALF_QWERTY_2
	,PRODUCE_HALF_QWERTY_3
	,PRODUCE_HALF_QWERTY_4

	// 单字产品
	,PRODUCE_NUMPAD_5
	,PRODUCE_NUMPAD_6
	,PRODUCE_NUMPAD_7
	,PRODUCE_NUMPAD_8

	,PRODUCE_QWERTY_5
	,PRODUCE_QWERTY_6
	,PRODUCE_QWERTY_7
	,PRODUCE_QWERTY_8

	,PRODUCE_HALF_QWERTY_5
	,PRODUCE_HALF_QWERTY_6
	,PRODUCE_HALF_QWERTY_7
	,PRODUCE_HALF_QWERTY_8

	,PRODUCE_NUMPAD_9
	,PRODUCE_NUMPAD_10

	,PRODUCE_NUMPAD_11
	,PRODUCE_QWERTY_9

	,PRODUCE_EXPLICIT_1
	,PRODUCE_EXPLICIT_2
	,PRODUCE_EXPLICIT_3
	,PRODUCE_EXPLICIT_4

	,PRODUCE_EXPLICIT_5

//	,CURRENT_ENGINE_CONFIG

	,PRODUCE_MAX
};


//配置文件.config中使用到，记录配置信息
typedef struct _tag_ConfigData
{
	GBINT		configID;
	GBINT		configTypeID;		///< EngineIDEnum和ConfigTypeIDEnum
	GBINT		menuStrId;
	GBUINT16	configValueType;	///< ConfigValueTypeEnum
	GBUINT16	enable;	///< 是否显示
	GBWCHAR		szValue[CONFIG_DATA_VALUE_MAX_SIZE];
	GBINT		chdCount;
	struct _tag_ConfigData*	chdConfigData;
}GBConfigData,*pGBConfigData;


//和语言区分,参见GBLanguageEnum
enum lang_config_
{
	COMMON_CONFIG_LANG = GBL_Last + 1
	,HELP_CONFIG_LANG
	,HANDWRITING_CONFIG_LANG
	,HIDE_IME_CONFIG_LANG
	,ABOUT_CONFIG_LANG
	,OTHER_CONFIG_LANG
};

/** \brief 描述数据在文件中偏移和大小的结构体 */
typedef struct _tag_DataOffset
{
	GBUINT32		offset ;			///<data start offset in entire file
	GBUINT16		menuStrID;					
	GBUINT16		menuType;					
	GBUINT16		lang;
	GBUINT16		subLang;
	GBUINT16		capacity ;			///<最大有效的配置个数
	GBUINT16		itemNumber ;		///<已经使用的配置个数
} DataOffset;

/** \brief 火星文数据头 */
typedef struct _tag_cfg_header
{
	//main header
	GBACHAR				cfg[4] ;		///< "CFG\x00"
	GBACHAR				hVersion ;		///< header version
	GBACHAR				version1 ;		///< data version number 1
	GBACHAR				version2 ;		///< data version number 1
	GBACHAR				version3 ;		///< data version number 1
	GBUINT16			productID;		///<产品ID
	GBUINT16			langCount;
	GBUINT32			totalSize ;	///< total size of entire file
	DataOffset			commonConfig[16]; ///< 公共类配置

	GBACHAR				ReservedData[CONFIG_STRUCTURE_RESERVED_LEN] ; ///< reserved data for future use
}CFG_HEADER, * pCFG_HEADER;

/** \brief 火星文数据尾 */
typedef struct _tag_cfg_tail
{
	GBINT					lang;
	GBINT					subLang;
	GBINT					menuStrId;
	GBINT					menuType;
	GBINT					configCount;
	GBConfigData			configData[16];
}CFG_TAILS,*pCFG_TAILS;

/** \brief 文本信息结构 */
typedef struct _tag_cfg_text_info
{
	GBUINT16				textStrId;	//字符串id
	GBPCWCHAR				pszText;	//字符串内容
}CFG_TEXT_INFO, *pCFG_TEXT_INFO;

extern const unsigned long g_szHelpFeatures[];
extern const unsigned long g_szSelectInputMode[];
extern const unsigned long g_szSwitchingAndHiddenKeyboard[];
extern const unsigned long g_szSmartPinyinInput[];
extern const unsigned long g_szSmartStrokeInput[];
extern const unsigned long g_szSmartEngInput[];
extern const unsigned long g_szSymbolInput[];
extern const unsigned long g_szHandWritingInput[];
extern const unsigned long g_szSetting[];
extern const CFG_TEXT_INFO Text_info[];

/*!
 * \brief 通过字符串ID获取文本信息，当前菜单必须是ONLY_TEXT_DLG_TYPE类型
 * \return  
 * \note 参见CTextInfo.c文件
 * \example
 * \author ZhaoKun
 * \date
 */
extern GBPCWCHAR GetTextInfo(GBUINT16 strID);


/*!
 * \brief 释放配置缓存
 * \return  
 * \note 
 * \example
 * \author ZhaoKun
 * \date
 */
extern GBINT FreeConfig(void);

/*!
 * \brief 设置全部引擎配置
 * \return  
 * \note 
 * \example
 * \author ZhaoKun
 * \date
 */
extern GBINT SetConfigFile(void);

/*!
 * \brief 通过字符串id设置引擎配置
 * \param menuStrId 
 * \param pValue
 * \param valueCount 当pValue为STRING_TYPE类型时可以
 * \return  
 * \note 
 * \example
 * \author ZhaoKun
 * \date
 */
extern GBINT SetEngineForStrID(GBINT menuStrId, void* pValue, GBINT valueCount);

/*!
 * \brief 获取当前产品ID
 * \param 
 * \return  
 * \note 
 * \example
 * \author ZhaoKun
 * \date
 */
extern GBINT GetCurProductID(void);

/*!
 * \brief 通过文件路径加载配置文件
 * \param (in) pFilePath 配置文件路径
 * \return  
 * \note 
 * \example
 * \author ZhaoKun
 * \date
 */
extern GBINT LoadConfigFileForPath(GBPWCHAR pFilePath);

/*!
 * \brief 通过产品id加载配置文件
 * \param (in) productID 产品id
 * \return  
 * \note 
 * \example
 * \author ZhaoKun
 * \date
 */
extern GBINT LoadConfigFile(GBINT productID);

/*!
 * \brief 通过配置文件路径保存配置设置
 * \param (in) filePath 配置文件路径
 * \return  
 * \note 
 * \example
 * \author ZhaoKun
 * \date
 */
extern GBINT SaveConfigFileForPath(GBPWCHAR filePath);

/*!
 * \brief 取配置文件路径
 * \return  
 * \note 
 * \example
 * \author ZhaoKun
 * \date
 */
extern GBPWCHAR GetConfigFilePath(void);

/*!
 * \brief 通过产品id保存配置设置到文件
 * \param (in) productID 产品ID	
 * \return  
 * \note 
 * \example
 * \author ZhaoKun
 * \date
 */
extern GBINT SaveConfigFile(GBINT productID);

/*!
 * \brief 根据菜单字符串id设置缓存的配置数据
 * \param (in) menuStrId 菜单字符串id	
 * \param (in) pValue 配置值
 * \param (in) valueCount STRING_TYPE时可用，pValue长度
 * \return  
 * \note 
 * \example
 * \author ZhaoKun
 * \date
 */
extern GBINT SetConfigValueForMenuStrID(GBINT menuStrId, void* pValue, GBINT valueCount);

/*!
 * \brief 根据语言，子语言，配置id和配置类型设置缓存的配置数据
 * \param (in) lang 语言	
 * \param (in) subLang 子语言
 * \param (in) configID 配置id
 * \param (in) configType 配置类型
 * \param (in) pValue 配置数据
 * \param (in) valueCount STRING_TYPE时可用，pValue长度
 * \return  
 * \note 
 * \example
 * \author ZhaoKun
 * \date
 */
extern GBINT SetConfigValue(GBINT lang, GBINT subLang, GBINT configID, GBINT configType, void* pValue, GBINT valueCount);

/*!
 * \brief 根据当前选择的字符串id，获取子菜单的字符串id、值、个数
 * \param (in) curStrID 当前菜单字符串ID	
 * \param (out) pChdMenuStrID 子菜单id
 * \param (out) pChdMenuValue 子菜单值
 * \param (out) pChdMenuCount 子菜单id个数
 * \return  返回菜单类型，详见ConfigValueTypeEnum
 * \note 
 * \example
 * \author ZhaoKun
 * \date
 */
extern GBINT GetConfigStrID(GBUINT16 curStrID, GBUINT16* pChdMenuStrID, void* pChdMenuValue, GBUINT16 *pChdMenuCount);

/*!
 * \brief 获取当前菜单下所有配置的菜单id和状态
 * \param (in) lang 语言	
 * \param (in) subLang 子语言
 * \param (out) pMenuStrID 菜单字符串id
 * \param (out) pMenuValue 菜单状态
 * \param (out) pMenuCount 菜单个数
 * \return  
 * \note buf必须 < MAX_SUB_MENUS
 * \example
 * \author ZhaoKun
 * \date
 */
extern GBINT GetConfigMenuInfo(GBINT lang, GBINT subLang, GBUINT16* pMenuStrID, void* pMenuValue, GBUINT16 *pMenuCount);

extern GBPACHAR GetProductString(GBINT productID);

extern GBPWCHAR GetProductStringW(GBINT productID);

/*!
 * \brief 把字符串转换成整型，同_wtoi
 * \param str
 * \return
 * \note 
 * \author ZhaoKun
 * \date 2010年9月25日
 */
extern GBINT gbwtoi(GBLPCWCHAR str);

/*!
 * \brief 把整型转换成字符串
 * \param number
 * \param str 需要有足够大的空间，函数不做空间检查
 * \param radix 转换的形式，该函数实现只针对10进制
 * \return
 * \note 
 * \author ZhaoKun
 * \date 2010年9月25日
 */
extern GBPWCHAR gbitow(GBINT number, GBLPWCHAR str, GBINT radix );

/*!
 * \brief 通过字符串ID获取当前菜单的类型
 * \param curStrID
 * \return	参见ConfigValueTypeEnum
 * \note 
 * \author ZhaoKun
 * \date 2010年9月27日
 */
extern GBINT GetCurMenuTypeToStrID(GBUINT16 curStrID);

#endif