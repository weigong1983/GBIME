/***************************************************************************
* File Name    :  CSwitchLayout.c
* Corporation  :  Guobi Technology Ltd. GuangZhou China
* Copyright    :  Copyright Guobi Technology Ltd. And Subject to Confidentiality 
*                 Restrictions. This document is the confidential and proprietary 
*                 property of Guobi Technology Ltd.It is neither the document                  
*                 itself nor any part of it can be shared with any party  without 
*                 the written  permission. 
* Author       :  fanbo
* Date         :  2010-06-08
* Purpose      :  切换Layout实现文件
* Version      :  V1.0
* Last Changed List
* Name         |   date   |  Discription
****************************************************************************/
#include "CSwitchLayout.h"
#include "GB_Global.h"

/*!
* \brief 切换Layout
* \param tartLayoutID 目标Layout ID 
* \return 
* \note 
* \example
* \author fanbo
* \date 2010-06-08
*/
GBINT32 CSwitchLayout_Switch(GBUINT16 tartLayoutID)
{
	PCLayoutAbstract pltCurrentObject = Global_GetCurrentLayoutObject();
	PCLayoutAbstract pltTargetObject = NULL;
	GBU16 layoutWidth = 0;

	// 检查LayoutID是否有效
	PT_Assert(IS_VALID_LAYOUT_ID(tartLayoutID));

	// 目标Layout就是当前Layout对象(从选项菜单返回时出现此状况)
// 	if (pltCurrentObject != NULL && tartLayoutID == GET_LAYOUT_ID(pltCurrentObject))
// 	{
// 		return GBIME_OK;
// 	}

    // 前置处理
	switch (tartLayoutID)
	{
	case LayoutIdSwitchPreObj: // 返回上一个Layout
		pltTargetObject = Global_GetPrevLayoutObject();
		break;
	case LayoutIdSwitchLatestObj: // 返回最近Layout
		tartLayoutID = Global_GetLatestMasterLayoutID();
		pltTargetObject = Global_LayoutObjectIsLeaveByID(tartLayoutID);
		break;
	case LayoutIdSwitchLatestChnObj: // 返回最近中文Layout
		tartLayoutID = Global_GetLatestChnLayoutID();
		pltTargetObject = Global_LayoutObjectIsLeaveByID(tartLayoutID);
		break;
	case LayoutIdSwitchLatestAlpObj: // 返回最近拉丁Layout
		tartLayoutID = Global_GetLatestAlpLayoutID();
		pltTargetObject = Global_LayoutObjectIsLeaveByID(tartLayoutID);
		break;
	default:
		pltTargetObject = Global_LayoutObjectIsLeaveByID(tartLayoutID);
		break;
	}

	// 挪到m_pfcInitVariable中统一清除
// 	// 切换到新的的Layout之前重置当前Layout的MultiTap定时器
// 	if (pltCurrentObject != NULL)
// 	{
// 		if (pltCurrentObject->m_pfcKBMultiTapTimerCallBack != NULL)
// 		{
// 			pltCurrentObject->m_pfcKBMultiTapTimerCallBack();
// 		}
// 		if (pltCurrentObject->m_pfcOnMultitapTimer != NULL)
// 		{
// 			pltCurrentObject->m_pfcOnMultitapTimer();
// 		}
// 	}	

	if (NULL == pltTargetObject)
	{
		// 目标切换Layout对象不存在,需要重新创建
		pltTargetObject = CFactory_CreateInstance(Global_GetIMEData(), tartLayoutID);
		if (NULL != pltTargetObject)
		{
			Global_SaveLayoutObject(pltTargetObject);
		}
		else
		{
			return GBIME_FAILED; // 创建Layout失败
		}
	}
	else
	{
		Global_SetCurrentLayoutObject(pltTargetObject);
	}
	
	// 重新设置标点符号功能键
	if (pltTargetObject->m_pfcSetInterpunctionKey != NULL)
	{
		pltTargetObject->m_pfcSetInterpunctionKey(pltTargetObject);
	}
	
#if defined(__GBIME_HANDWRITE__) // 手写Layout启用MTK平台手写功能
	if (IS_HANDWRITE_LAYOUT(tartLayoutID))
	{
		PT_EnableHandwriting();
	}
	else
	{
		PT_DisableHandwriting();
	}
#endif

	// 保存最近的LayoutID
	Global_SaveLatestLayoutID(tartLayoutID);
	
	// 根据Layout实际宽度动态计算国笔候选框的宽度【需要考虑2个方向按钮宽度】
	layoutWidth = pltTargetObject->m_pfcGetLayoutWidth(Global_GetCurrentLayoutObject(), tartLayoutID);
	GBEngine_SetPageToalWidthOption(Global_GetEnineInstance(), (GBU16)(layoutWidth - GBIME_ARROW_WIDTH * 2 - 4));
	//GBEngine_SetCandRowCount(Global_GetEnineInstance(), GBIME_INPUT_CAND_ROW_COUNT, GBIME_ASSOCIATE_CAND_ROW_COUNT); // 设置候选行数
	return GBIME_OK;
}