/***************************************************************************
* File Name    :  CLayoutViewHandwrite.C
* Corporation  :  Guobi Technology Ltd. GuangZhou China
* Copyright    :  Copyright Guobi Technology Ltd. And Subject to Confidentiality 
*                 Restrictions. This document is the confidential and proprietary 
*                 property of Guobi Technology Ltd.It is neither the document                  
*                 itself nor any part of it can be shared with any party  without 
*                 the written  permission. 
* Author       :  weizhiping
* Date         :  2010-08-07
* Purpose      :  ÊÖÐ´¼üÅÌÊÓÍ¼Àà
* Version      :  V1.0
* Last Changed List
* Name         |   date   |  Discription
****************************************************************************/
#include "CLayoutViewHandwrite.h"
#include "CLayoutHandwrite.h"
#include "PlatformAdapter.h"
#include "PlatformTools.h"
#include "GB_Global.h"
#include "CHWEngine.h"
#include "GBIMEResDef.h"

#if defined(__GBIME_HANDWRITE__)

CLayoutViewAbstract * CLayoutViewHandwrite__Construct(PCLayoutViewHandwrite pltViewHandwrite)
{
	CLayoutViewAbstract__Construct(&pltViewHandwrite->m_ltParent);
	CLayoutViewHandwrite__Init(pltViewHandwrite);
	return (CLayoutViewAbstract *)pltViewHandwrite;
}

GBIMEReturn CLayoutViewHandwrite__Init(PCLayoutViewHandwrite pltViewHandwrite)
{
	return GBIME_OK;
}

#endif // defined(__GBIME_HANDWRITE__)