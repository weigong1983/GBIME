#ifndef __CPRODUCT_SETTING_H__
#define __CPRODUCT_SETTING_H__

#include "CConfigData.h"
#include "ConfigDef.h"

#define ENGINE_CONFIG_MAX_COUNT 64

/** \brief ��Ʒ��ʽ */
typedef struct _tag_ProductStyle
{
	GBINT	Chn_candListType;	//���ĺ�ѡ������
	GBINT	Chn_nCandRowCount;	//���ĺ�ѡ����
	GBINT	Alp_candListType;	//Ӣ�ĺ�ѡ������
	GBINT	Alp_nCandRowCount;	//Ӣ�ĺ�ѡ����
}GBProductStyle;

//�Բ�Ʒ��Ҫһ��.c�ļ�����¼�����44������
typedef struct _tag_product_data
{
	GBINT				productID;			///<��ƷID��ProductEnum
	GBPCWCHAR			productName;		///<��Ʒ��
	GBPCWCHAR			pConfigFileName;	///<�����ļ���
	GBProductStyle		productData;		///<��Ʒ������ʽ,��¼��Ʒ�ĺ�ѡ���ͺ�����
	GBEngineOptionData	configData[ENGINE_CONFIG_MAX_COUNT]; 
}GBProductData,*pGBProductData;


//<��Ʒ����
extern const GBProductData productData[];
extern GBINT GetProductDataCount(void);
extern GBINT GetProductConfigCount(GBINT index);



extern GBINT SelectProduct(GBINT productID);
extern GBPCWCHAR GetProductConfigFilePath(GBINT productID);
extern GBProductStyle* GetProductStyle(GBINT productID);


#endif