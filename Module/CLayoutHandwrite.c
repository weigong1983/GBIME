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
* Purpose      :  ��д����
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

// ��д�켣��������غ���
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
	// ��ʼ����д�켣������
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
 * \brief ��д�켣��������Ϣ����
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
	case GBPEVT_PEN_DOWN: /* ���ʵ����¼�����ζ��һ���ʻ��Ŀ�ʼ*/
		{
			GBRECT  rcEdit;

			PT_PrintLogInfo(GBLOG_HANDWRITE, ("���ʵ����¼�����ζ��һ���ʻ��Ŀ�ʼ\r\n"));

			rcEdit.left = 0;
			rcEdit.top = 0;
			rcEdit.right = (GBINT16)PT_GetLcdWidth();
			rcEdit.bottom = (GBINT16)PT_GetLcdHeight();
			//GBPA_GetInputBoxEditRect(&rcEdit);
			//rcEdit.top = 0;

			/* 
			 * �ʻ��Ŀ�ʼ�����������
			 * 1. ����ڱ༭�ؼ�����ʾ��Χ�ڼ���
			 * 2. �����1�������㣬��ʱ���Ѿ�����Ҳ����
			 * ��ʱ������ζ��֮ǰ�Ѿ����������ɱʣ�������һ������������ﶼ����ν�ˣ�
			 * �������Ա�֤��д�������ԡ�
			 */
			if( PT_PointIsInRect(point,rcEdit) || pGBHWTrace->timer != 0x0 )
			{
				// �����д̬���رն�ʱ������Ϊ���ܼ�����
				if( pGBHWTrace->status == GBTRACE_Drawing )
					GBHWTrace_StopTimer(pGBHWTrace);
				// ����ǳ�ʼ״̬������Ԥ��̬
				else if( pGBHWTrace->status == GBTRACE_None )
					pGBHWTrace->status = GBTRACE_Ready;
				// �����ʶ��̬����λ������Ԥ��̬
				else if( pGBHWTrace->status == GBTRACE_Identified )
				{
					GBHWTrace_Reset(pGBHWTrace);
					pGBHWTrace->status = GBTRACE_Ready;
				}
				else // ������������״̬
				{
					PT_Assert(0);
				}

				// ��¼���
				pGBHWTrace->last_point = point;
				PT_HandwriteStrokeDown(point.x, point.y);
			}
		}
		break;	
	case GBPEVT_PEN_UP: /* �����ɿ��¼�����ζ��һ���ʻ��Ľ��� */
		{
			PT_PrintLogInfo(GBLOG_HANDWRITE, ("�����ɿ��¼�����ζ��һ���ʻ��Ľ���\r\n"));

			/* ���֮ǰ�����������������ƶ���*/
			if( pGBHWTrace->status == GBTRACE_Drawing )
			{
				// һ���ʻ�������־				
				GBHWTrace_AppendTrace(pGBHWTrace, (GBINT16)0xFFFF, (GBINT16)0x0000, GBFalse );
				// �򿪺ͱ༭�ؼ������Ķ�ʱ������ʱ���޵����ʶ�����л����ıʻ�
				GBHWTrace_StartTimer(pGBHWTrace);
			}
			/* ����������ǵ�һ���������ɿ�������û�л�����ֻ�ûָ���ʼ̬��*/
			else if( pGBHWTrace->status == GBTRACE_Ready )
			{
				pGBHWTrace->status = GBTRACE_None;
			}
			PT_HandwriteStrokeUp();
			//else // ����״̬������
		}
		break;	
	case GBPEVT_PEN_MOVE: /* ���ʵ�����Ļ�ƶ� */
		{
			PT_PrintLogInfo(GBLOG_HANDWRITE, ("���ʵ�����Ļ�ƶ�[%d, %d]\r\n", point.x, point.y));

			/*���ǴӺϷ�����ƶ������ģ����ǿ϶���������*/
			if( pGBHWTrace->status == GBTRACE_Ready || pGBHWTrace->status == GBTRACE_Drawing )
			{
				GBRECT  clip_rect;			
					
				clip_rect.left = 0;
				clip_rect.top = 0;
				clip_rect.right = (GBINT16)PT_GetLcdWidth();
				clip_rect.bottom = (GBINT16)PT_GetLcdHeight();
				
				// �����ƶ����涨����д��Χ��
				if(PT_PointIsInRect(point, clip_rect))
				{
					// �ɹ�����켣����
					if( GBHWTrace_AppendTrace(pGBHWTrace, point.x, point.y, GBFalse ) )
					{
						pGBHWTrace->status = GBTRACE_Drawing;
						//add for cr39385, LCD�Ļ��߻�ˢ��������������⣬����������⴦��
						clip_rect.top++;
						clip_rect.left++;
						clip_rect.right--;
						clip_rect.bottom--;
						PT_HandwriteStrokeMove(point.x, point.y);
						// ��¼���
						pGBHWTrace->last_point = point;
					}
				}
			}
		}
	break;	
	default:
		PT_PrintLogInfo(GBLOG_HANDWRITE, ("�켣�����������¼�\r\n"));
		return GBFalse;
	}

	return GBTrue;
}

// ��д�켣��������غ���
GBIMELOCAL void GBHWTrace_OnTimer(void)
{
	int recognizeCandCount = 0;
	PGBHWTrace pGBHWTrace = CLayoutHandwrite__GetHWTraceInstance();

	// ����ʻ�
	PT_HandwriteClearStroke();

	// �ʻ�ȫ������������ʶ��״̬
	GBHWTrace_AppendTrace(pGBHWTrace, (GBINT16)0xFFFF, (GBINT16)0xFFFF, GBTrue);
	pGBHWTrace->status = GBTRACE_Identified;

	// ������д����ʶ��ӿڻ�ȡ��ѡ
	recognizeCandCount = GBHW_Recognize(Global_GetHWEnineInstance(), 
										GBHWTrace_GetPointBuffer(pGBHWTrace), 
										GBHWTrace_GetTotalHits(pGBHWTrace));


	GBHW_RecognizeFinishedCallback();//modify by john for �����ʼ�


	// ��λ�켣	
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
 * \brief ��ʼ����д�켣������
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
 * \brief ��д�켣�������˳�
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
 * \brief ��д�켣������״̬��λ
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
 * \brief ��ȡ��д�켣�������б��������Ļ�����
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
 * \brief ��ȡ��д�켣�������б������Ч���������Ŀ
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