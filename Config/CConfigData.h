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
* Purpose      :  �������ݶ�ȡ�ļ�
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
#define STR_W(id)	L###id // ����ַ���ƴ������
#define FSIZ( type, field ) sizeof( ((type *) 0)->field )	///<��ȡ�ṹ�����sizeof


enum ConfigTypeIDEnum
{
	COMMON_CONFIG = 0x100	//<�����������ͣ��ۼ���EngineIDEnum
	,HANDWRITING_CONFIG
	,DEFAULT_IM_CONFIG
};

enum ConfigValueTypeEnum
{
	CONFIG_VALUE_TYPE_ENUM_START
	,LIST_TYPE		//�б�ѡ��
	,RADIO_TYPE		//<��ѡ��
	,CHECK_TYPE		//<��ѡ��
	,INT_TYPE		//<���ͻ�bool����
	,STRING_TYPE	//<�ַ�������
	,ONLY_TEXT_DLG_TYPE	//<ֻ���ı���������

	,CONFIG_VALUE_TYPE_ENUM_END
};

enum ProductEnum
{
	// �����Ʒ
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

	// ���ֲ�Ʒ
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


//�����ļ�.config��ʹ�õ�����¼������Ϣ
typedef struct _tag_ConfigData
{
	GBINT		configID;
	GBINT		configTypeID;		///< EngineIDEnum��ConfigTypeIDEnum
	GBINT		menuStrId;
	GBUINT16	configValueType;	///< ConfigValueTypeEnum
	GBUINT16	enable;	///< �Ƿ���ʾ
	GBWCHAR		szValue[CONFIG_DATA_VALUE_MAX_SIZE];
	GBINT		chdCount;
	struct _tag_ConfigData*	chdConfigData;
}GBConfigData,*pGBConfigData;


//����������,�μ�GBLanguageEnum
enum lang_config_
{
	COMMON_CONFIG_LANG = GBL_Last + 1
	,HELP_CONFIG_LANG
	,HANDWRITING_CONFIG_LANG
	,HIDE_IME_CONFIG_LANG
	,ABOUT_CONFIG_LANG
	,OTHER_CONFIG_LANG
};

/** \brief �����������ļ���ƫ�ƺʹ�С�Ľṹ�� */
typedef struct _tag_DataOffset
{
	GBUINT32		offset ;			///<data start offset in entire file
	GBUINT16		menuStrID;					
	GBUINT16		menuType;					
	GBUINT16		lang;
	GBUINT16		subLang;
	GBUINT16		capacity ;			///<�����Ч�����ø���
	GBUINT16		itemNumber ;		///<�Ѿ�ʹ�õ����ø���
} DataOffset;

/** \brief ����������ͷ */
typedef struct _tag_cfg_header
{
	//main header
	GBACHAR				cfg[4] ;		///< "CFG\x00"
	GBACHAR				hVersion ;		///< header version
	GBACHAR				version1 ;		///< data version number 1
	GBACHAR				version2 ;		///< data version number 1
	GBACHAR				version3 ;		///< data version number 1
	GBUINT16			productID;		///<��ƷID
	GBUINT16			langCount;
	GBUINT32			totalSize ;	///< total size of entire file
	DataOffset			commonConfig[16]; ///< ����������

	GBACHAR				ReservedData[CONFIG_STRUCTURE_RESERVED_LEN] ; ///< reserved data for future use
}CFG_HEADER, * pCFG_HEADER;

/** \brief ����������β */
typedef struct _tag_cfg_tail
{
	GBINT					lang;
	GBINT					subLang;
	GBINT					menuStrId;
	GBINT					menuType;
	GBINT					configCount;
	GBConfigData			configData[16];
}CFG_TAILS,*pCFG_TAILS;

/** \brief �ı���Ϣ�ṹ */
typedef struct _tag_cfg_text_info
{
	GBUINT16				textStrId;	//�ַ���id
	GBPCWCHAR				pszText;	//�ַ�������
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
 * \brief ͨ���ַ���ID��ȡ�ı���Ϣ����ǰ�˵�������ONLY_TEXT_DLG_TYPE����
 * \return  
 * \note �μ�CTextInfo.c�ļ�
 * \example
 * \author ZhaoKun
 * \date
 */
extern GBPCWCHAR GetTextInfo(GBUINT16 strID);


/*!
 * \brief �ͷ����û���
 * \return  
 * \note 
 * \example
 * \author ZhaoKun
 * \date
 */
extern GBINT FreeConfig(void);

/*!
 * \brief ����ȫ����������
 * \return  
 * \note 
 * \example
 * \author ZhaoKun
 * \date
 */
extern GBINT SetConfigFile(void);

/*!
 * \brief ͨ���ַ���id������������
 * \param menuStrId 
 * \param pValue
 * \param valueCount ��pValueΪSTRING_TYPE����ʱ����
 * \return  
 * \note 
 * \example
 * \author ZhaoKun
 * \date
 */
extern GBINT SetEngineForStrID(GBINT menuStrId, void* pValue, GBINT valueCount);

/*!
 * \brief ��ȡ��ǰ��ƷID
 * \param 
 * \return  
 * \note 
 * \example
 * \author ZhaoKun
 * \date
 */
extern GBINT GetCurProductID(void);

/*!
 * \brief ͨ���ļ�·�����������ļ�
 * \param (in) pFilePath �����ļ�·��
 * \return  
 * \note 
 * \example
 * \author ZhaoKun
 * \date
 */
extern GBINT LoadConfigFileForPath(GBPWCHAR pFilePath);

/*!
 * \brief ͨ����Ʒid���������ļ�
 * \param (in) productID ��Ʒid
 * \return  
 * \note 
 * \example
 * \author ZhaoKun
 * \date
 */
extern GBINT LoadConfigFile(GBINT productID);

/*!
 * \brief ͨ�������ļ�·��������������
 * \param (in) filePath �����ļ�·��
 * \return  
 * \note 
 * \example
 * \author ZhaoKun
 * \date
 */
extern GBINT SaveConfigFileForPath(GBPWCHAR filePath);

/*!
 * \brief ȡ�����ļ�·��
 * \return  
 * \note 
 * \example
 * \author ZhaoKun
 * \date
 */
extern GBPWCHAR GetConfigFilePath(void);

/*!
 * \brief ͨ����Ʒid�����������õ��ļ�
 * \param (in) productID ��ƷID	
 * \return  
 * \note 
 * \example
 * \author ZhaoKun
 * \date
 */
extern GBINT SaveConfigFile(GBINT productID);

/*!
 * \brief ���ݲ˵��ַ���id���û������������
 * \param (in) menuStrId �˵��ַ���id	
 * \param (in) pValue ����ֵ
 * \param (in) valueCount STRING_TYPEʱ���ã�pValue����
 * \return  
 * \note 
 * \example
 * \author ZhaoKun
 * \date
 */
extern GBINT SetConfigValueForMenuStrID(GBINT menuStrId, void* pValue, GBINT valueCount);

/*!
 * \brief �������ԣ������ԣ�����id�������������û������������
 * \param (in) lang ����	
 * \param (in) subLang ������
 * \param (in) configID ����id
 * \param (in) configType ��������
 * \param (in) pValue ��������
 * \param (in) valueCount STRING_TYPEʱ���ã�pValue����
 * \return  
 * \note 
 * \example
 * \author ZhaoKun
 * \date
 */
extern GBINT SetConfigValue(GBINT lang, GBINT subLang, GBINT configID, GBINT configType, void* pValue, GBINT valueCount);

/*!
 * \brief ���ݵ�ǰѡ����ַ���id����ȡ�Ӳ˵����ַ���id��ֵ������
 * \param (in) curStrID ��ǰ�˵��ַ���ID	
 * \param (out) pChdMenuStrID �Ӳ˵�id
 * \param (out) pChdMenuValue �Ӳ˵�ֵ
 * \param (out) pChdMenuCount �Ӳ˵�id����
 * \return  ���ز˵����ͣ����ConfigValueTypeEnum
 * \note 
 * \example
 * \author ZhaoKun
 * \date
 */
extern GBINT GetConfigStrID(GBUINT16 curStrID, GBUINT16* pChdMenuStrID, void* pChdMenuValue, GBUINT16 *pChdMenuCount);

/*!
 * \brief ��ȡ��ǰ�˵����������õĲ˵�id��״̬
 * \param (in) lang ����	
 * \param (in) subLang ������
 * \param (out) pMenuStrID �˵��ַ���id
 * \param (out) pMenuValue �˵�״̬
 * \param (out) pMenuCount �˵�����
 * \return  
 * \note buf���� < MAX_SUB_MENUS
 * \example
 * \author ZhaoKun
 * \date
 */
extern GBINT GetConfigMenuInfo(GBINT lang, GBINT subLang, GBUINT16* pMenuStrID, void* pMenuValue, GBUINT16 *pMenuCount);

extern GBPACHAR GetProductString(GBINT productID);

extern GBPWCHAR GetProductStringW(GBINT productID);

/*!
 * \brief ���ַ���ת�������ͣ�ͬ_wtoi
 * \param str
 * \return
 * \note 
 * \author ZhaoKun
 * \date 2010��9��25��
 */
extern GBINT gbwtoi(GBLPCWCHAR str);

/*!
 * \brief ������ת�����ַ���
 * \param number
 * \param str ��Ҫ���㹻��Ŀռ䣬���������ռ���
 * \param radix ת������ʽ���ú���ʵ��ֻ���10����
 * \return
 * \note 
 * \author ZhaoKun
 * \date 2010��9��25��
 */
extern GBPWCHAR gbitow(GBINT number, GBLPWCHAR str, GBINT radix );

/*!
 * \brief ͨ���ַ���ID��ȡ��ǰ�˵�������
 * \param curStrID
 * \return	�μ�ConfigValueTypeEnum
 * \note 
 * \author ZhaoKun
 * \date 2010��9��27��
 */
extern GBINT GetCurMenuTypeToStrID(GBUINT16 curStrID);

#endif