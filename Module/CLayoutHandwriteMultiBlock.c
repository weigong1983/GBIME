/***************************************************************************
* File Name    :  CLayoutHandwriteMultiBlock.c
* Corporation  :  Guobi Technology Ltd. GuangZhou China
* Copyright    :  Copyright Guobi Technology Ltd. And Subject to Confidentiality 
*                 Restrictions. This document is the confidential and proprietary 
*                 property of Guobi Technology Ltd.It is neither the document                  
*                 itself nor any part of it can be shared with any party  without 
*                 the written  permission. 
* Author       :  weizhiping
* Date         :  2010-09-08
* Purpose      :  ¸ñ×ÓÊÖÐ´¼üÅÌ
* Version      :  V1.0
* Last Changed List
* Name         |   date   |  Discription
****************************************************************************/
#include "CLayoutHandwriteMultiBlock.h"
#include "CLayoutViewHandwriteMultiBlock.h"
#include "PlatformAdapter.h"
#include "GB_Global.h"

#if defined(__GBIME_HANDWRITE__)

CLayoutAbstract * CLayoutHandwriteMultiBlock__Construct(PCLayoutHandwriteMultiBlock pltHandwriteMultiBlock, 
					      		GBLPCVOID pltUIData,
					      		GBLPCVOID pltIMData,
					      		GBLPCVOID pltConfigData)
{	
	CLayoutHandwrite__Construct(&pltHandwriteMultiBlock->m_ltParent, pltUIData, pltIMData, pltConfigData);
	//CLayoutHandwrite__Init(pltHandwriteMultiBlock);
	return (CLayoutAbstract *)pltHandwriteMultiBlock;
}

#endif // defined(__GBIME_HANDWRITE__)