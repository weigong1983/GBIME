#include "CProductSetting.h"
#include "CGBEngine.h"
#include "GB_Global.h"


GBINT SelectProduct(GBINT productID)
{
	GBINT i = 0;
	GBINT j = 0;
	GBINT count;
	GBINT configCount;
	const GBProductData * pTempProductData = productData;
	const GBEngineOptionData * pTempConfigData = NULL;

	count = GetProductDataCount();

	for (i = 0; i < count; i++)
	{
		if (pTempProductData[i].productID != productID)
		{
			continue;
		}
		configCount = GetProductConfigCount(i);
		pTempConfigData = pTempProductData[i].configData;
		for (j = 0; j < configCount; j++)
		{
			if (!pTempConfigData[j].id && !pTempConfigData[j].type && !pTempConfigData[j].flag && !pTempConfigData[j].value)
			{
				break;
			}
			GBEngine_SetEngineOption(Global_GetEnineInstance(), pTempConfigData[j].type, pTempConfigData[j].id, pTempConfigData[j].value);
		}
		FreeConfig();
		//¼ÓÔØÅäÖÃÎÄ¼þ
		LoadConfigFile(productID);
		break;
	}
	return 1;
}

GBPCWCHAR GetProductConfigFilePath(GBINT productID)
{
	GBINT i = 0;
	GBINT count;
	GBPCWCHAR pFilePath = NULL;
	const GBProductData *  pTempProductData = productData;

	count = GetProductDataCount();

	for (i = 0; i < count; i++)
	{
		if (pTempProductData[i].productID != productID)
		{
			continue;
		}
		pFilePath = pTempProductData[i].pConfigFileName;
	}
	return pFilePath;
}

GBProductStyle* GetProductStyle(GBINT productID)
{
	GBINT i = 0;
	GBINT count;
	const GBProductData * pTempProductData = productData;
	GBProductStyle* pProductStyle = NULL;

	count = GetProductDataCount();

	for (i = 0; i < count; i++)
	{
		if (pTempProductData[i].productID != productID)
		{
			continue;
		}

		pProductStyle = (GBProductStyle *)&pTempProductData[i].productData;
	}
	return pProductStyle;
}

