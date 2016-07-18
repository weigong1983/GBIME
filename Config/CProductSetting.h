#ifndef __CPRODUCT_SETTING_H__
#define __CPRODUCT_SETTING_H__

#include "CConfigData.h"
#include "ConfigDef.h"

#define ENGINE_CONFIG_MAX_COUNT 64

/** \brief 产品样式 */
typedef struct _tag_ProductStyle
{
	GBINT	Chn_candListType;	//中文候选行类型
	GBINT	Chn_nCandRowCount;	//中文候选行数
	GBINT	Alp_candListType;	//英文候选行类型
	GBINT	Alp_nCandRowCount;	//英文候选行数
}GBProductStyle;

//对产品需要一个.c文件，记录引擎的44个配置
typedef struct _tag_product_data
{
	GBINT				productID;			///<产品ID，ProductEnum
	GBPCWCHAR			productName;		///<产品名
	GBPCWCHAR			pConfigFileName;	///<配置文件名
	GBProductStyle		productData;		///<产品表现形式,记录产品的候选类型和行数
	GBEngineOptionData	configData[ENGINE_CONFIG_MAX_COUNT]; 
}GBProductData,*pGBProductData;


//<产品数据
extern const GBProductData productData[];
extern GBINT GetProductDataCount(void);
extern GBINT GetProductConfigCount(GBINT index);



extern GBINT SelectProduct(GBINT productID);
extern GBPCWCHAR GetProductConfigFilePath(GBINT productID);
extern GBProductStyle* GetProductStyle(GBINT productID);


#endif