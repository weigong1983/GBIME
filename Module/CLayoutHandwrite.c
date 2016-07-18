/***************************************************************************
* File Name    :  CLayoutHandwrite.c
* Corporation  :  Guobi Technology Ltd. GuangZhou China
* Copyright    :  Copyright Guobi Technology Ltd. And Subject to Confidentiality 
*                 Restrictions. This document is the confidential and proprietary 
*                 property of Guobi Technology Ltd.It is neither the document                  
*                 itself nor any part of it can be shared with any party  without 
*                 the written  permission. 
* Author       :  weizhiping
* Date         :  2010-08-07
* Purpose      :  手写键盘
* Version      :  V1.0
* Last Changed List
* Name         |   date   |  Discription
****************************************************************************/
#include "CLayoutHandwrite.h"
#include "CLayoutViewHandwrite.h"
#include "PlatformAdapter.h"
#include "GB_Global.h"
#include "GBIMEDef.h"

#if defined(__GBIME_HANDWRITE__)

GBIMELOCAL GBIMEReturn CLayoutHandwrite__Init(PCLayoutHandwrite pltHandwrite);
GBIMELOCAL GBIMEReturn CLayoutHandwrite__Release(GBLPVOID pltObj, PLayoutInfo pltUIData);
#if 0
GBIMELOCAL PGBHWTrace CLayoutHandwrite__GetHWTraceInstance(void);
GBIMELOCAL GBBOOL CLayoutHandwrite__HandleTraceEvent(GBLPVOID pltObj, GBINT nPlatformEvent, GBIMEPoint point);

// 手写轨迹管理器相关函数
GBIMELOCAL void GBHWTrace_Initialize(PGBHWTrace pGBHWTrace);
GBIMELOCAL void GBHWTrace_Terminate(PGBHWTrace pGBHWTrace);
GBIMELOCAL void GBHWTrace_Reset(PGBHWTrace pGBHWTrace);
GBIMELOCAL GBPINT16 GBHWTrace_GetPointBuffer(PGBHWTrace pGBHWTrace);
GBIMELOCAL GBU16 GBHWTrace_GetTotalHits(PGBHWTrace pGBHWTrace);
GBIMELOCAL void GBHWTrace_OnTimer(void);
GBIMELOCAL void GBHWTrace_StopTimer(PGBHWTrace pGBHWTrace);
GBIMELOCAL void GBHWTrace_StartTimer(PGBHWTrace pGBHWTrace);
GBIMELOCAL GBBOOL GBHWTrace_AppendTrace(PGBHWTrace pGBHWTrace, GBINT16 x, GBINT16 y, GBBOOL bAllEnd);
#endif

CLayoutAbstract * CLayoutHandwrite__Construct(PCLayoutHandwrite pltHandwrite, 
					      GBLPCVOID pltUIData,
					      GBLPCVOID pltIMData,
					      GBLPCVOID pltConfigData)
{	
	CLayoutAbstract__Construct(&pltHandwrite->m_ltParent, pltUIData, pltIMData, pltConfigData);
	CLayoutHandwrite__Init(pltHandwrite);
	return (CLayoutAbstract *)pltHandwrite;
}

GBIMELOCAL GBIMEReturn CLayoutHandwrite__Init(PCLayoutHandwrite pltHandwrite)
{
#if 0
	// 初始化手写轨迹管理器
	pltHandwrite->pGBHWTrace = (GBHWTrace *)PT_Malloc(sizeof(GBHWTrace));
	PT_Assert(pltHandwrite->pGBHWTrace != NULL);
	memset(pltHandwrite->pGBHWTrace, 0, sizeof(GBHWTrace));	
	GBHWTrace_Initialize(pltHandwrite->pGBHWTrace);
	pltHandwrite->pGBHWTrace->pfHWTraceHandleEvent = CLayoutHandwrite__HandleTraceEvent;	
#endif	
	pltHandwrite->m_ltParent.m_pfcRelease = CLayoutHandwrite__Release;
	return GBIME_OK;
}

GBIMELOCAL GBIMEReturn CLayoutHandwrite__Release(GBLPVOID pltObj, PLayoutInfo pltUIData)
{
	CLASE_THIS_POITER(CLayoutHandwrite, pltObj)
#if 0
	GBHWTrace_Terminate(pltThis->pGBHWTrace);		
	if (pltThis->pGBHWTrace != NULL)
	{
		PT_Free(pltThis->pGBHWTrace);
		pltThis->pGBHWTrace = NULL;
	}
#endif
	pltBase->m_pfcReleaseBase(pltObj, pltUIData);
		
	return GBIME_OK;
}

#if 0
GBIMELOCAL PGBHWTrace CLayoutHandwrite__GetHWTraceInstance(void)
{
	PCLayoutHandwrite pltThis = (PCLayoutHandwrite)Global_GetCurrentLayoutObject();
	return pltThis->pGBHWTrace;
}

/*!
 * \brief 手写轨迹管理器消息处理
 * \param pGBHWTrace 
 * \param pIMEEvent 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-8-13 17:41:11
*/
GBIMELOCAL GBBOOL CLayoutHandwrite__HandleTraceEvent(GBLPVOID pltObj, GBINT nPlatformEvent, GBIMEPoint point)
{
	CLASE_THIS_POITER(CLayoutHandwrite, pltObj)
	PGBHWTrace pGBHWTrace = pltThis->pGBHWTrace;

	if(!pGBHWTrace->enable)
	{
		return GBFalse;
	}

	switch(nPlatformEvent)
	{
	case GBPEVT_PEN_DOWN: /* 触笔点下事件，意味着一个笔画的开始*/
		{
			GBRECT  rcEdit;

			PT_PrintLogInfo(GBLOG_HANDWRITE, ("触笔点下事件，意味着一个笔画的开始\r\n"));

			rcEdit.left = 0;
			rcEdit.top = 0;
			rcEdit.right = (GBINT16)PT_GetLcdWidth();
			rcEdit.bottom = (GBINT16)PT_GetLcdHeight();
			//GBPA_GetInputBoxEditRect(&rcEdit);
			//rcEdit.top = 0;

			/* 
			 * 笔画的开始满足的条件：
			 * 1. 落点在编辑控件的显示范围内即可
			 * 2. 如果第1条不满足，定时器已经打开了也可以
			 * 定时器打开意味着之前已经画过了若干笔，所以这一画的落点在哪里都无所谓了，
			 * 这样可以保证书写的连续性。
			 */
			if( PT_PointIsInRect(point,rcEdit) || pGBHWTrace->timer != 0x0 )
			{
				// 如果是写态，关闭定时器，因为可能继续画
				if( pGBHWTrace->status == GBTRACE_Drawing )
					GBHWTrace_StopTimer(pGBHWTrace);
				// 如果是初始状态，进入预备态
				else if( pGBHWTrace->status == GBTRACE_None )
					pGBHWTrace->status = GBTRACE_Ready;
				// 如果是识别态，复位，进入预备态
				else if( pGBHWTrace->status == GBTRACE_Identified )
				{
					GBHWTrace_Reset(pGBHWTrace);
					pGBHWTrace->status = GBTRACE_Ready;
				}
				else // 不可能是其他状态
				{
					PT_Assert(0);
				}

				// 记录落点
				pGBHWTrace->last_point = point;
				PT_HandwriteStrokeDown(point.x, point.y);
			}
		}
		break;	
	case GBPEVT_PEN_UP: /* 触笔松开事件，意味着一个笔画的结束 */
		{
			PT_PrintLogInfo(GBLOG_HANDWRITE, ("触笔松开事件，意味着一个笔画的结束\r\n"));

			/* 如果之前画过，即触笔曾经移动过*/
			if( pGBHWTrace->status == GBTRACE_Drawing )
			{
				// 一个笔画结束标志				
				GBHWTrace_AppendTrace(pGBHWTrace, (GBINT16)0xFFFF, (GBINT16)0x0000, GBFalse );
				// 打开和编辑控件关联的定时器，定时界限到达后识别所有划过的笔画
				GBHWTrace_StartTimer(pGBHWTrace);
			}
			/* 这种情况就是点一下又马上松开，根本没有画过，只好恢复初始态了*/
			else if( pGBHWTrace->status == GBTRACE_Ready )
			{
				pGBHWTrace->status = GBTRACE_None;
			}
			PT_HandwriteStrokeUp();
			//else // 其他状态不处理
		}
		break;	
	case GBPEVT_PEN_MOVE: /* 触笔点着屏幕移动 */
		{
			PT_PrintLogInfo(GBLOG_HANDWRITE, ("触笔点着屏幕移动[%d, %d]\r\n", point.x, point.y));

			/*不是从合法起点移动过来的，我们肯定不作处理*/
			if( pGBHWTrace->status == GBTRACE_Ready || pGBHWTrace->status == GBTRACE_Drawing )
			{
				GBRECT  clip_rect;			
					
				clip_rect.left = 0;
				clip_rect.top = 0;
				clip_rect.right = (GBINT16)PT_GetLcdWidth();
				clip_rect.bottom = (GBINT16)PT_GetLcdHeight();
				
				// 必须移动到规定的手写范围内
				if(PT_PointIsInRect(point, clip_rect))
				{
					// 成功加入轨迹数组
					if( GBHWTrace_AppendTrace(pGBHWTrace, point.x, point.y, GBFalse ) )
					{
						pGBHWTrace->status = GBTRACE_Drawing;
						//add for cr39385, LCD的画线或刷屏处理可能有问题，因此做了特殊处理
						clip_rect.top++;
						clip_rect.left++;
						clip_rect.right--;
						clip_rect.bottom--;
						PT_HandwriteStrokeMove(point.x, point.y);
						// 记录落点
						pGBHWTrace->last_point = point;
					}
				}
			}
		}
	break;	
	default:
		PT_PrintLogInfo(GBLOG_HANDWRITE, ("轨迹管理器忽略事件\r\n"));
		return GBFalse;
	}

	return GBTrue;
}

// 手写轨迹管理器相关函数
GBIMELOCAL void GBHWTrace_OnTimer(void)
{
	int recognizeCandCount = 0;
	PGBHWTrace pGBHWTrace = CLayoutHandwrite__GetHWTraceInstance();

	// 清除笔画
	PT_HandwriteClearStroke();

	// 笔画全部结束，进入识别状态
	GBHWTrace_AppendTrace(pGBHWTrace, (GBINT16)0xFFFF, (GBINT16)0xFFFF, GBTrue);
	pGBHWTrace->status = GBTRACE_Identified;

	// 调用手写引擎识别接口获取候选
	recognizeCandCount = GBHW_Recognize(Global_GetHWEnineInstance(), 
										GBHWTrace_GetPointBuffer(pGBHWTrace), 
										GBHWTrace_GetTotalHits(pGBHWTrace));


	GBHW_RecognizeFinishedCallback();//modify by john for 残留笔迹


	// 复位轨迹	
	GBHWTrace_Reset(pGBHWTrace);
}
GBIMELOCAL void GBHWTrace_StopTimer(PGBHWTrace pGBHWTrace)
{
    if( pGBHWTrace->timer != 0x0 )
	{
		PT_StopTimer(pGBHWTrace->timer);
		pGBHWTrace->timer = 0x0;
	}
}
GBIMELOCAL void GBHWTrace_StartTimer(PGBHWTrace pGBHWTrace)
{
    GBHWTrace_StopTimer(pGBHWTrace);
#if __MTK__ > 0
	pGBHWTrace->timer = HANDWRITE_RECOGNIZE_TIMER;
	PT_StartTimer(pGBHWTrace->timer, pGBHWTrace->timeout, (FuncPtr)GBHWTrace_OnTimer);	
#endif
}
GBIMELOCAL GBBOOL GBHWTrace_AppendTrace(PGBHWTrace pGBHWTrace, GBINT16 x, GBINT16 y, GBBOOL bAllEnd)
{
	GBINT idx = pGBHWTrace->total_hits * 2;

	if( bAllEnd == GBFalse )
	{
		if( pGBHWTrace->total_hits < GBIME_DEF_HW_MAX_TRACE_PER_WORD )
		{
			pGBHWTrace->traceBuffer[idx++] = x;
			pGBHWTrace->traceBuffer[idx++] = y;
			pGBHWTrace->total_hits++;
			return GBTrue;
		}
	}
	else
	{
		pGBHWTrace->traceBuffer[idx++] = x;
		pGBHWTrace->traceBuffer[idx++] = y;
	}

	return GBFalse;
}

/*!
 * \brief 初始化手写轨迹管理器
 * \param pGBHWTrace 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-8-6 14:02:17
 */
GBIMELOCAL void GBHWTrace_Initialize(PGBHWTrace pGBHWTrace)
{
	pGBHWTrace->enable      = GBTrue;
	pGBHWTrace->pixel       = HW_PIXEL_DEFAULT;
	pGBHWTrace->color       = HW_COLOR_DEFAULT;
	pGBHWTrace->timeout     = HW_SPEED_DEFAULT; 
	pGBHWTrace->total_hits = 0;
	pGBHWTrace->timer      = 0x0;
	GBHWTrace_Reset(pGBHWTrace);
}

/*!
 * \brief 手写轨迹管理器退出
 * \param pGBHWTrace 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-8-6 14:02:17
 */
GBIMELOCAL void GBHWTrace_Terminate(PGBHWTrace pGBHWTrace)
{	
	pGBHWTrace->enable = GBFalse;
	GBHWTrace_Reset(pGBHWTrace);
}

/*!
 * \brief 手写轨迹管理器状态复位
 * \param pGBHWTrace 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-8-6 14:02:17
 */
GBIMELOCAL void GBHWTrace_Reset(PGBHWTrace pGBHWTrace)
{
	GBHWTrace_StopTimer(pGBHWTrace);
	pGBHWTrace->status = GBTRACE_None;
	pGBHWTrace->total_hits = 0;
	memset(pGBHWTrace->traceBuffer, 0x0, sizeof(pGBHWTrace->traceBuffer));
}

/*!
 * \brief 获取手写轨迹管理器中保存坐标点的缓冲区
 * \param pGBHWTrace 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-8-6 14:02:17
 */
GBIMELOCAL GBPINT16 GBHWTrace_GetPointBuffer(PGBHWTrace pGBHWTrace)
{
	PT_Assert(pGBHWTrace->status == GBTRACE_Identified);
	return pGBHWTrace->traceBuffer;
}

/*!
 * \brief 获取手写轨迹管理器中保存的有效触摸点击数目
 * \param pGBHWTrace 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-8-6 14:02:17
 */
GBIMELOCAL GBU16 GBHWTrace_GetTotalHits(PGBHWTrace pGBHWTrace)
{
	PT_Assert(pGBHWTrace->status == GBTRACE_Identified);
	return pGBHWTrace->total_hits;
}
#endif

#endif // defined(__GBIME_HANDWRITE__)