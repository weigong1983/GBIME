/***************************************************************************
* File Name    :  PlatformTools.c
* Corporation  :  Guobi Technology Ltd. GuangZhou China
* Copyright    :  Copyright Guobi Technology Ltd. And Subject to Confidentiality 
*                 Restrictions. This document is the confidential and proprietary 
*                 property of Guobi Technology Ltd.It is neither the document                  
*                 itself nor any part of it can be shared with any party  without 
*                 the written  permission. 
* Author       :  weizhiping
* Date         :  2010-06-08
* Purpose      :  ��װMTKƽ̨����Ļģ��������ַ��������ڴ�������ļ����ʡ�������Ϣ�ص�ע��/ע�����ַ�����ʾ�ȹ��ߺ���
* Version      :  V1.0
* Last Changed List
* Name         |   date   |  Discription
****************************************************************************/

#include "PlatformTools.h"
#include "GBIMEResDef.h"

#define GBMAX_SUB_MENUS               (60)
#define GBMAX_SUBMENU_CHARACTERS      (60)
#define GBMAX_SUB_MENU_SIZE           (60*ENCODING_LENGTH + ENCODING_LENGTH)

#ifdef __GBIME_LOG__
extern GBUACHAR uni2gb[][2];
#endif

#if __MTK__ > 0

extern void wgui_hide_right_softkey(void);
extern void wgui_hide_left_softkey(void);
extern void redraw_EMS_inputbox(void);
extern void redraw_EMS_inputbox(void);
extern MMI_BOOL EMS_inputbox_direct_input(UI_character_type c);
extern void EMS_inputbox_delete_all(void);
extern void register_EMS_inputbox_keys(void);
extern void wgui_set_EMS_inputbox_RSK(void);
extern void wgui_inline_edit_register_BACK_function(void);

/*!
 * \defgroup ��ͼ���API
 */

/*!
 * \brief ������������ֺŴ�Сת����MTKƽ̨����Ӧ��������(�ڲ�ʹ��)
 * \param font �μ�PT_FontSize
 * \return 
 */
GBIMELOCAL const UI_font_type PT_GetMtkFontType(PT_FontSize font)
{
    switch(font)
    {		
    case PT_FONT_SIZE_SMALL:
        return &MMI_small_font;		
    case PT_FONT_SIZE_LARGE:
        return &MMI_large_font;
	case PT_FONT_SIZE_DEFAULT:
	case PT_FONT_SIZE_MEDIUM:
	default:
		return &MMI_medium_font;
	}
}
#endif // __MTK__

static char g_trace_buf[512];
static wchar_t g_trace_bufW[512];

#if __GBIME_LOG_TO_FILE__ > 0
FILE*	g_test_log_file = NULL;
//���ڲ���ʱ��ӡlog
GBINT PT_OpenFile(char* filePath)
{
	if (g_test_log_file)
	{
		return 0;
	}

	g_test_log_file = fopen(filePath, "w+");

	if (!g_test_log_file)
	{
		return -1;
	}
	return 1;
}

void PT_CloseFile()
{
	if (g_test_log_file)
	{
		fclose(g_test_log_file);
		g_test_log_file = NULL;
	}
}
#endif
/*!
 * \brief ��ӡ���
 * \param fmt 
 * \param ... 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-8-11 20:12:10
*/
void PT_Print(const char * fmt, ...)
{
	va_list trace_p;
	va_start(trace_p, fmt);
	vsprintf(g_trace_buf, fmt, trace_p);
	va_end(trace_p);
#if __MTK__ > 0
	kal_print(g_trace_buf);
	kal_prompt_trace(MOD_GBIME, g_trace_buf);
#elif __GBIME_LOG_TO_FILE__ > 0
	if (g_test_log_file)
	{
		fprintf(g_test_log_file, g_trace_buf);
	}
#else
	printf(g_trace_buf);
#endif
}

/*!
 * \brief ��ӡ���
 * \param fmt 
 * \param ... 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-8-11 20:12:10
*/
void PT_PrintW(const wchar_t * fmt, ...)
{
	va_list trace_p;
	va_start(trace_p, fmt);
#if __MTK__ > 0
	//vswprintf(g_trace_bufW, fmt, trace_p);
	//vswprintf(g_trace_bufW, sizeof(g_trace_bufW), fmt, trace_p); // VS2008 ��Ҫ�ĸ�����
#else
	vswprintf(g_trace_bufW, sizeof(g_trace_bufW), fmt, trace_p);
#endif
	va_end(trace_p);
#if __MTK__ > 0
	kal_print(g_trace_buf);
	kal_prompt_trace(MOD_GBIME, g_trace_buf);
#elif __GBIME_LOG_TO_FILE__ > 0
	if (g_test_log_file)
	{
		fwprintf(g_test_log_file, g_trace_bufW);
	}
#else
	wprintf(g_trace_bufW);
#endif
}

// /*!
//  * \brief �����ַ�Unicode�ַ���ת����Ansi�ַ�����ֻ����ASCII���ַ���ת������ʾLOG�á�
//  * \param pSrcUniString 
//  * \param pDesAnsiString 
//  * \return 
//  * \note 
//  * \example
//  * \author weizhiping
//  * \date 2010-8-17 17:12:19
// */
// void PT_UnicodeToAnsi(GBPCWCHAR pSrcUniString, GBUINT8 *pDesAnsiString)
// {
// 	GBUINT16 iCount = 0;
// 
// 	while (pSrcUniString[iCount])
// 	{
// 		pDesAnsiString[iCount] = (GBUINT8)(pSrcUniString[iCount] & 0x00FF);
// 		iCount++;
// 	}
// 	pDesAnsiString[iCount] = '\0';
// }

/*!
 * \brief ����ƽ̨��ʱ��
 * \param timerid 
 * \param delay 
 * \param funcPtr 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-8-17 11:05:51
*/
void PT_StartTimer(GBU16 timerid, GBU32 delay, TIMER_CALLBACK funcPtr)
{
#if __MTK__ > 0
	StartTimer((U16)timerid, (U32)delay, (FuncPtr)funcPtr);
#elif __GBIME_LOG__
	PT_PrintLogInfo(GBLOG_PLATFORM, ("****PT_StartTimer:timerid=%d, delay=%d, funcPtr=0x%x\n", timerid, delay, funcPtr))
#endif
}

/*!
 * \brief ֹͣƽ̨��ʱ��
 * \param timerid 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-8-17 11:08:31
*/
void PT_StopTimer(GBU16 timerid)
{
#if __MTK__ > 0
	StopTimer((U16)timerid);
#elif __GBIME_LOG__
	PT_PrintLogInfo(GBLOG_PLATFORM, ("****PT_StopTimer:timerid=%d\n", timerid))
#endif
}

/*!
 * \brief ƽ̨GUI���׶�ʱ����������(������ʱ��ID)
 * \param delay 
 * \param callback 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-10-8 17:10:33
*/
void PT_GUIStartTimer(GBINT delay, TIMER_CALLBACK callback)
{
#if __MTK__ > 0
	gui_start_timer(delay, callback);
#endif
}

/*!
 * \brief ƽ̨GUI���׶�ʱ��ȡ������(������ʱ��ID)
 * \param callback 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-10-8 17:10:30
*/
void PT_GUICancelTimer(TIMER_CALLBACK callback)
{
#if __MTK__ > 0
	gui_cancel_timer(callback);
#endif
}

/*!
 * \brief ��ȡϵͳ���������ڵĺ�����
 * \param void 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-24 9:20:38
*/
GBU32 PT_GetCurrentTime(void)
{
#if __MTK__ > 0
	kal_uint32 ticks;
	kal_get_time(&ticks);
	return (GBU32)kal_ticks_to_milli_secs(ticks);
#endif
	return 0;
}

/*!
 * \brief ���� Unicode  ���͵��ַ���ռ���ٵ���
 * \param pParam �μ�GBGlobalPageOption.pGetStringWidthParam������
 * \param str Դ�ַ���
 * \return ��Unicode�ַ��������ؿ��
 * \note ���÷���
 	GBGlobalPageOption pageOption;
 	GBGetPageOption(pGbis, &pageOption);
	pageOption.fpGetWStringWidth = PT_GetStrWidth;
	GBSetPageOption(pGbis, &pageOption);
 * \author weizhiping
 * \date 2009��12��11��
 */
int PT_GetStrWidth(void * pParam, GBLPCWCHAR str)
{
	return PT_GetStrWidthEx(str, GBIME_CANDIDATE_FONT_SIZE); // GBV5��������ѡ�ַ����Ĭ�ϲ��ú�ѡ���壬�������ʾʱ��һ��
	//return gui_get_string_width((UI_string_type)str);
}

/*!
 * \brief ��ȡƽ̨LCD���
 * \param void 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-21 11:14:07
 */
GBIMEGLOBAL GBINT PT_GetLcdWidth(void)
{
	return UI_DEVICE_WIDTH;
}

/*!
 * \brief ��ȡƽ̨LCD�߶�
 * \param void 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-21 11:13:49
 */
GBIMEGLOBAL GBINT PT_GetLcdHeight(void)
{
	return UI_DEVICE_HEIGHT;
}

/*!
 * \brief ƽ̨��ӡ�ַ�������
 * \param x 
 * \param y 
 * \param pText 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-11-30 9:44:35
*/
void PT_PrintText(GBINT x, GBINT y,  GBPCWCHAR pText)
{
	UI_string_type pStrText = (UI_string_type)pText;
#if defined(__MMI_VECTOR_FONT_SUPPORT__)
	S32 baseline;
	baseline = gui_get_string_height(pStrText) - gui_get_string_height(pStrText) / 9;
	mmi_fe_show_string_baseline(x, y, (U8*)pStrText, baseline, 0);
#else
	gui_move_text_cursor(x, y);
    gui_print_text(pStrText);
#endif
}

/*!
 * \brief �����ַ���
 * \param str 
 * \param font 
 * \param clr 
 * \param left 
 * \param top 
 * \return 
 */
void PT_DrawString( GBPCWCHAR str, PT_FontSize font, PT_COLOR clr, GBINT left, GBINT top)
{
#if __MTK__ > 0
	//color backupSystemColor = gui_get_text_color();

	PT_SetFont(font);
	PT_SetTextColor(clr);
	//gui_move_text_cursor((S32)left, (S32)top);
	//gui_print_text((UI_string_type)str);
	PT_PrintText(left, top, str);
	//gui_set_text_color(backupSystemColor);
#elif __GBIME_LOG__
	{
		GBACHAR buf[64] = {0};
		PT_wcstombs(buf, str, sizeof(buf));
		PT_PrintLogInfo(GBLOG_PLATFORM, ("*****PT_DrawString��%s\r\n", buf));
	}
#endif
}

/*!
 * \brief �ʻ����ƺ���(����С�ֺ��޷���ʾƫ�ԡ��^����С�����񡱵Ĵ���)
 * \param pStrokeText 
 * \param left 
 * \param top 
 * \param font 
 * \param clr 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-12-2 16:33:33
 */
void PT_PrintStrokeText(GBPCWCHAR pStrokeText, GBINT left, GBINT top, PT_FontSize font, PT_COLOR clr)
{
	GBINT i;
	GBINT strokeLeft = left;   
	GBINT strokeTop = top;
	GBINT strokeWidth;
	GBINT strokeHeight;
	GBINT nStrokeNum = wcslen(pStrokeText);
	GBINT nStrokeStringWidth = gui_get_string_width((UI_string_type)pStrokeText);
	GBINT nInterval = 2;
	UI_character_type stroke_text[2] = {0};

	PT_SetFont(font);
	PT_SetTextColor(clr);

	for(i=0; i<nStrokeNum; i++)
	{
#if 1//def __GB2312__ //  ƫ�ԡ��^����С�������ڴ�������ʾΪ���ڡ�
		if(pStrokeText[i] == 0x4E5B) // �^( 0x4E5B)
		{
			gui_measure_image(get_image((U16)(IMG_GB_STROKE_5)), &strokeWidth, &strokeHeight);
			gui_show_image(strokeLeft, strokeTop, get_image((U16)(IMG_GB_STROKE_5)));		             
		}
		else if(pStrokeText[i] == 0xE822) //�] (0xE822)
		{
			gui_measure_image(get_image((U16)(IMG_GB_COMP_XIAO)), &strokeWidth, &strokeHeight);
			gui_show_image(strokeLeft, strokeTop, get_image((U16)(IMG_GB_COMP_XIAO)));		             
		}
		else if(pStrokeText[i] == 0xE836) //�q��0xE836) 
		{
			gui_measure_image(get_image((U16)(IMG_GB_COMP_ZHU)), &strokeWidth, &strokeHeight);
			gui_show_image(strokeLeft, strokeTop, get_image((U16)(IMG_GB_COMP_ZHU)));		             
		}
		else
#endif
		{
			stroke_text[0] = pStrokeText[i];
			strokeWidth = PT_GetStrWidthEx(stroke_text, font);
			PT_PrintText(strokeLeft, strokeTop, stroke_text);
		}

		strokeLeft += strokeWidth + nInterval;
	}
}

/*!
 * \brief ��ָ��λ�ã�����ͼƬID��ʾͼƬ
 * \param iLeftX ��ʾͼƬ�����Ͻ�X����
 * \param iTopY ��ʾͼƬ�����Ͻ�Y����
 * \param imageID ͼƬID
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-12 11:48:39
 */
void PT_DrawImage(GBINT iLeftX, GBINT iTopY, GBU16 imageID)
{
#if __MTK__ > 0
	//gui_measure_image();
	gui_show_image(iLeftX, iTopY, get_image((U16)(imageID)));
#elif __GBIME_LOG__
	PT_PrintLogInfo(GBLOG_PLATFORM, ("****PT_DrawImage��iLeftX=%d, iTopY=%d, imageID=%d\n", iLeftX, iTopY, imageID));
#endif
}
/*!
 * \brief ��ͼƬ��ĳһָ����������ʾ����Ļ��ָ��������
 * \param imageID ͼƬID
 * \param display_rect_ptr ��Ļ�ϵ���ʾ����
 * \param image_rect_ptr ͼƬ��ָ��������
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-12 11:49:10
 */
void PT_DrawImageWithClip(GBU16 imageID, GBRECT *display_rect_ptr, GBRECT *image_rect_ptr)
{
#if __MTK__ > 0
	S32 iLeftX, iTopY;
    PU8  image_ptr = (PU8)get_image((U16)imageID);

	PT_Assert(display_rect_ptr != NULL && image_rect_ptr != NULL);
	PT_Assert(image_ptr != NULL);

	gui_push_clip();
	gui_set_clip(display_rect_ptr->left, display_rect_ptr->top, display_rect_ptr->right, display_rect_ptr->bottom);
	iLeftX = display_rect_ptr->left - image_rect_ptr->left;
	iTopY  = display_rect_ptr->top - image_rect_ptr->top;
    gui_show_image(iLeftX, iTopY, image_ptr);
	gui_pop_clip();
#elif __GBIME_LOG__
	PT_PrintLogInfo(GBLOG_PLATFORM, ("****PT_DrawImageWithClip:imageID=%d, display_rect_ptr=%d,%d,%d,%d image_rect_ptr=%d,%d,%d,%d\n"
									 , imageID
									 ,display_rect_ptr->bottom, display_rect_ptr->left, display_rect_ptr->right, display_rect_ptr->top
									 ,image_rect_ptr->bottom, image_rect_ptr->left, image_rect_ptr->right, image_rect_ptr->top));
#endif
}

/*!
 * \brief ����������
 * \param left 
 * \param top 
 * \param right 
 * \param bottom 
 * \param clr 
 * \return 
 */
void PT_FillRect(GBI16 left, GBI16 top, GBI16 right, GBI16 bottom, PT_COLOR clr)
{
#if __MTK__ > 0
	gui_fill_rectangle((S32)left, (S32)top, (S32)right, (S32)bottom, clr);
#endif
}

/*!
 * \brief ����������
 * \param left 
 * \param top 
 * \param right 
 * \param bottom 
 * \param clr 
 * \return 
 */
void PT_DrawRect(GBI16 left, GBI16 top, GBI16 right, GBI16 bottom, PT_COLOR clr)
{
#if __MTK__ > 0
	gui_draw_rectangle((S32)left, (S32)top, (S32)right, (S32)bottom, clr);
#endif
}

/*!
 * \brief ����
 * \param x 
 * \param y 
 * \param clr 
 * \return 
 */
void PT_DrawPoint(GBU16 x, GBU16 y,  PT_COLOR clr)
{
#if __MTK__ > 0
	gui_line((S32)x, (S32)y, (S32)x, (S32)y, clr);
#endif
}

/*!
 * \brief ��һ��ֱ��
 * \param x1 
 * \param y1 
 * \param x2 
 * \param y2 
 * \param clr 
 * \return 
 */
void PT_DrawLine(GBU16 x1, GBU16 y1, GBU16 x2, GBU16 y2, PT_COLOR clr)
{
#if __MTK__ > 0
	gui_line((S32)x1, (S32)y1, (S32)x2, (S32)y2, clr);
#endif
}

/*!
 * \brief ��һ��ˮƽ��
 * \param x1 
 * \param x2 
 * \param y 
 * \param clr 
 * \return 
 */
void PT_DrawHorizontalLine(GBU16 x1, GBU16 x2, GBU16 y, PT_COLOR clr)
{
#if __MTK__ > 0
	gui_draw_horizontal_line((S32)x1, (S32)x2, (S32)y, clr);
#endif
}

/*!
 * \brief ��һ����ֱ��
 * \param y1 
 * \param y2 
 * \param x 
 * \param clr 
 * \return 
 */
void PT_DrawVerticalLine(GBU16 y1, GBU16 y2, GBU16 x, PT_COLOR clr)
{
#if __MTK__ > 0
	gui_draw_vertical_line((S32)y1, (S32)y2, (S32)x, clr);
#endif
}

/*!
 * \brief ���ƴ���ʽ�Ĵ�ֱ����
 * \param y1 
 * \param y2 
 * \param x 
 * \param clr 
 * \param lineType 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-8-23 11:39:39
*/
void PT_DrawVerticalLineType(GBU16 y1, GBU16 y2, GBU16 x, PT_COLOR clr, PT_LINT_TYPE lineType)
{
#if __MTK__ > 0

	S32 type_line;

	switch (lineType)
	{
	case PT_LINT_TYPE_SOLID:
		type_line = 0;
		break;
	case PT_LINT_TYPE_DOTTED:
		type_line = 1;
		break;
	case PT_LINT_TYPE_DASHED:
		type_line = 2;
		break;
	default:
		PT_Assert(0);
		break;
	}

	gui_draw_vert_line_type((S32)y1, (S32)y2, (S32)x, clr, type_line);
#endif
}

/*!
 * \brief ��һ��ָ����ϸ��ȵ�ֱ��
 * \param x1 
 * \param y1 
 * \param x2 
 * \param y2 
 * \param clr 
 * \param widthInPixelswidth in pixels
 * \return 
 */
void PT_DrawThickLine(GBU16 x1, GBU16 y1, GBU16 x2, GBU16 y2, PT_COLOR clr, GBU16 widthInPixels)
{
#if __MTK__ > 0
	gui_wline((S32)x1, (S32)y1, (S32)x2, (S32)y2, clr, (S32)widthInPixels);
#endif
}

/*!
 * \brief ��һ��ָ����ϸ��ȵ�ֱ��(��ָ���ü������ü���֮�ⲻ����)
 * \param x1 
 * \param y1 
 * \param x2 
 * \param y2 
 * \param clip_left 
 * \param clip_top 
 * \param clip_right 
 * \param clip_bottom 
 * \param clr 
 * \param widthInPixelswidth in pixels
 * \return 
 */
void PT_DrawThickLineEx( GBU16 x1, 
						 GBU16 y1, 
						 GBU16 x2, 
						 GBU16 y2, 
						 GBU16 clip_left,
						 GBU16 clip_top,
						 GBU16 clip_right,
						 GBU16 clip_bottom,						 
						 PT_COLOR clr, 
						 GBU16 widthInPixels )
{
    GBU16 dx = 0, dy = 0;
    GBU16 half_width = 0, next_width = 0;
    GBBOOL is_swap_x = GBFalse, is_swap_y = GBFalse;
 

    //��ʼ�����ϻ���������Ҫ���ӵ��߿�
    //half_width = (STROKE_WIDTH>>1);
    half_width = (widthInPixels >> 1);
    //��ʼ�����»����ҷ�����Ҫ���ӵ��߿�
    //next_width = (STROKE_WIDTH - half_width - 1);
    next_width = (widthInPixels - half_width - 1);

    if (x1 > x2)
    {
        SWAP_UINT16(x1, x2);
        is_swap_x = GBTrue;
    }

    if (y1 > y2)
    {
        SWAP_UINT16(y1, y2);
        is_swap_y = GBTrue;
    }

    //�ж������Ƿ��ڲü���
    if (x1 > clip_right || x2 < clip_left || y1 > clip_bottom || y2 < clip_top)
    {
        return;
    }

    dx = x2 - x1;
    dy = y2 - y1;

    //���÷��ϲü���������
    x1 = MAX((GBINT16)x1, (GBINT16)clip_left);
    x2 = MIN((GBINT16)x2, (GBINT16)clip_right);
    y1 = MAX((GBINT16)y1, (GBINT16)clip_top);
    y2 = MIN((GBINT16)y2, (GBINT16)clip_bottom);

    if (dx > dy)
    {
        //������������С����
        if (y1 < clip_top + half_width)
        {
            y1 = clip_top + half_width;
        }
        else if (y1 + next_width > clip_bottom)
        {
            y1 = clip_bottom - next_width;
        }

        //�����������������
        if (y2 < clip_top + half_width)
        {
            y2 = clip_top + half_width;
        }
        else if (y2 + next_width > clip_bottom)
        {
            y2 = clip_bottom - next_width;
        }
    }
    else
    {
        //������������С����
        if (x1 < clip_left + half_width)
        {
            x1 = clip_left + half_width;
        }
        else if (x1 + next_width > clip_right )
        {
            x1 = clip_right - next_width;
        }

        //�����������������
        if (x2 < clip_left + half_width)
        {
            x2 = clip_left + half_width;
        }
        else if (x2 + next_width > clip_right)
        {
            x2 = clip_right - next_width;
        }
    }

    if (is_swap_x)
    {
        SWAP_UINT16(x1, x2);
    }

    if (is_swap_y)
    {
        SWAP_UINT16(y1, y2);
    }

    PT_DrawThickLine(x1, y1, x2, y2, clr, widthInPixels ); 
}


/*!
 * \defgroup ���ߺ���
 */

/*!
* \brief �ж�һ���������Ƿ�����ָ������������
* \param point 
* \param rect 
* \return 
*/
#if 0 // ���滻�Ż�
GBBOOL PT_PointIsInRect(GBIMEPoint point, GBRECT rect)
{
	if (point.x >= rect.left && point.x <= rect.right 
		&& point.y >= rect.top && point.y <= rect.bottom)
    {
        return GBTrue;
    }
    else
    {
        return GBFalse;
    }
}
#endif

/*!
 * \brief ˢ����Ļָ������
 * \param left 
 * \param top 
 * \param right 
 * \param bottom 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-21 14:13:20
 */
void PT_Invalidate(GBINT left, GBINT top, GBINT right, GBINT bottom)
{
#if __MTK__ > 0
	gui_BLT_double_buffer(left, top, right, bottom);
#endif
}

/*!
* \brief ����
* \param  void
* \return void
*/
void PT_ClearScreen(void)
{
#if __MTK__ > 0
	clear_screen();
#endif
}

/*!
* \brief get string for given id ��ȡ�ַ���
* \param GBU16
* \return GBPCU16
*/
GBPCU16 PT_GetString(GBU16 id)
{
#if __MTK__ > 0
	return get_string(id);
#endif
	return NULL;
}

/*!
* \brief ��ȡ�ַ�������
* \param GBPCU16
* \return GBINT
*/
GBINT PT_GetStrLen(GBWCHAR *text)
{
#if __MTK__ > 0
	return gui_strlen((UI_string_type)text);
#endif
	return 1;
}

/*!
* \brief ����ַ��߶�
* \param GBPCU16
* \return GBINT
*/
GBINT PT_GetStrHeight(GBLPCWCHAR text)
{
#if __MTK__ > 0
	return gui_get_string_height((UI_string_type)text);
#endif
	return 1;
}

/*!
 * \brief ���ָ��������ַ������
 * \param text 
 * \param font 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-8-23 19:22:10
*/
GBINT PT_GetStrWidthEx(GBLPCWCHAR text, PT_FontSize font)
{
#if __MTK__ > 0
	PT_SetFont(font);
	return gui_get_string_width((UI_string_type)text);
#endif
	return 1;
}

/*!
 * \brief ���ָ������ıʻ��ַ������
 * \param text 
 * \param font 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2011-1-5 13:51:22
 */
GBINT PT_GetStrokeStrWidth(GBLPCWCHAR text, PT_FontSize font)
{
	GBINT i = 0;
	GBINT j = 0;
	GBLPWCHAR pText;
	GBINT strLen;
	GBINT strokeWidth = 0;
	GBINT strokeHeight = 0;
	GBINT strokeImgTotalWidth = 0;
	GBWCHAR tempString[32] = {0};
	GBINT strokeTotalWidth = 0;
	GBINT nInterval = 2;

	if (text == NULL)
	{
		return 0;
	}

	pText = text;
	strLen = wcslen(text);

#if 1//def __GB2312__ //  ����ƫ�ԡ��^����С������ֱ�Ӱ�ͼƬ����㵥���ʻ�����
	while (i < strLen)
	{
		if(pText[i] == 0x4E5B) // �^( 0x4E5B)
		{
			gui_measure_image(get_image((U16)(IMG_GB_STROKE_5)), &strokeWidth, &strokeHeight);
			strokeImgTotalWidth += strokeWidth + nInterval;		             
		}
		else if(pText[i] == 0xE822) //�] (0xE822)
		{
			gui_measure_image(get_image((U16)(IMG_GB_COMP_XIAO)), &strokeWidth, &strokeHeight);
			strokeImgTotalWidth += strokeWidth + nInterval;             
		}
		else if(pText[i] == 0xE836) //�q��0xE836) 
		{
			gui_measure_image(get_image((U16)(IMG_GB_COMP_ZHU)), &strokeWidth, &strokeHeight);
	        strokeImgTotalWidth += strokeWidth + nInterval;
		}
		else // ����ͼ�ıʻ���ʾ�����˳�������������
		{
			tempString[j++] = pText[i];
		}

		i++;
	}
#endif
	
	// �ñʻ�����ʵ����ʾ��� = ����ͼ�ıʻ���ʾ����� + ��ͼ�ַ������
	tempString[j] = 0;
	strokeTotalWidth = PT_GetStrWidthEx(tempString, font);
	strokeTotalWidth += strokeImgTotalWidth;

	return strokeTotalWidth;
}

/*!
 * \brief ���ָ��������ַ��߶�
 * \param text 
 * \param font 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-8-23 19:22:10
*/
GBINT PT_GetStrHeightEx(GBLPCWCHAR text, PT_FontSize font)
{
#if __MTK__ > 0
	PT_SetFont(font);
	return gui_get_string_height((UI_string_type)text);
#endif
	return 1;
}	

/*!
 * \brief ��ȡĳ�����ַ�����͸�
 * \param c 
 * \param pWidth 
 * \param pHeight 
 * \param font 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-10-12 18:45:10
*/
void PT_MeasureCharacter(GBWCHAR c, GBINT *pWidth,GBINT *pHeight, PT_FontSize font)
{
#if __MTK__ > 0
	PT_Assert(pWidth != NULL);
	PT_Assert(pHeight != NULL);

	// ASCII �ַ���ȡ����ʵ�ʿ��
	if (c >= 0 && c < 0x80)
	{
		gui_measure_character((UI_character_type)c, (S32 *)pWidth, (S32 *)pHeight);
	} 
	else // ��ASCII���ö����
	{
		switch (font)
		{
		case PT_FONT_SIZE_SMALL: //С������
			*pWidth = 14;
			*pHeight = 14;
			break;
		case PT_FONT_SIZE_LARGE: //�������
			*pWidth = 24;
			*pHeight = 24;
			break;
		case PT_FONT_SIZE_MEDIUM: //�к�����
		case PT_FONT_SIZE_DEFAULT: //Ĭ������
		default:
			*pWidth = 20;
			*pHeight = 20;
			break;	
		}
	}
	
#endif
}

/*!
* \brief ��ȡͼƬid
* \param GBU16
* \return GBPUACHAR
*/
GBPUACHAR PT_GetImage(GBU16 i)
{
#if __MTK__ > 0
	return get_image(i);
#endif
	return NULL;
}

/*!
* \brief ��ȡͼƬ��͸�
* \param GBPWCHAR
* \param GBPINT
* \param GBPINT
* \return void
*/
void PT_MeasureImage(GBPCHAR i,GBPINT width,GBPINT height)
{
#if __MTK__ > 0
	gui_measure_image((PU8)i, width, height);
#endif
}

/*!
* \brief ��ʾͼƬ
* \param GBINT,GBINT,GBUINT8
* \return void
*/
void PT_ShowImage(GBINT x,GBINT y,GBUINT8 *i)
{
#if __MTK__ > 0
	gui_show_image(x, y, (PU8)i);
#endif
}

/*!
* \brief �����ı�����Ŀ�ʼ����
* \param GBINT
* \param GBINT
* \return void
*/
void PT_MoveTextCursor(GBINT x,GBINT y)
{
#if __MTK__ > 0
	gui_move_text_cursor(x,y);
#endif
}

/*!
* \brief �������ΪN���ı�
* \param GBPUSHORT
* \param GBI
* \return void
*/
void PT_PrintTextN(GBPUSHORT text,GBI n)
{
#if __MTK__ > 0
	gui_print_text_n(text,n); 
#endif
}

/*!
* \brief �����ı���ɫ
* \param PT_COLOR
* \return void
*/
void PT_SetTextColor(PT_COLOR clr)
{
#if __MTK__ > 0
	gui_set_text_color(clr); 
#endif
}

/*!
* \brief ��������
* \param PT_FontSize
* \return void
*/
void PT_SetFont(PT_FontSize font)
{
#if __MTK__ > 0
	gui_set_font(PT_GetMtkFontType(font));
#endif
}

/*!
 * \brief ����ļ����Ƿ���ڣ�����������򴴽�֮
 * \param
 * \return
 * \note 
 * \author weizhiping
 * \date 2010��2��21��
 */
GBBOOL PT_CheckFolder(GBPCWCHAR pFoldPath)
{
#if __MTK__ > 0
    FS_HANDLE file_handle;
    S32 result = 0;

    file_handle = FS_Open((U16 *)pFoldPath, FS_OPEN_DIR | FS_READ_ONLY);

    if (file_handle < 0)
    {
        result = FS_CreateDir((U16 *)pFoldPath);
    }
    else
    {
        FS_Close(file_handle);
    }
	
    if (result < 0)
    {
        return GBFalse;
    }
#endif
    return GBTrue;
}

/*!
* \brief ����
* \param void
* \return void
*/
void PT_LayerLock(void)
{
#if __MTK__ > 0
	gdi_layer_lock_frame_buffer();
#endif
}

/*!
* \brief ������
* \param void
* \return void
*/
void PT_LayerUnLock(void)
{
#if __MTK__ > 0
	gdi_layer_unlock_frame_buffer();
//	gui_BLT_double_buffer(x, y, width, height);
#endif
}

/*!
* \brief �ı���������ѹջ
* \param GBINT x
* \param GBINT y
* \param GBINT width
* \param GBINT height
* \return void
*/
void PT_PushClip(GBINT x,GBINT y,GBINT width,GBINT height)
{
#if __MTK__ > 0
	gui_push_clip();
	gui_push_text_clip();
	gui_set_clip(x, y, width, height);
#endif
}

/*!
* \brief �ı����������ջ
* \param void
* \return void
*/
void PT_PopClip(void)
{
#if __MTK__ > 0
	gui_pop_text_clip(); 
	gui_pop_clip();
#endif
}

/*!
* \brief ��ȡ��ǰ�����ĻID
* \param void
* \return GBINT
*/
GBINT PT_GetActiveScreenId(void)
{
#if __MTK__ > 0
	return GetActiveScreenId();
#endif
	return 1;
}


/*!
* \brief ��������Ļ
* \param scrnid		  [IN]		��ĻID
* \param menu_item_id [IN]		�˵�ID
* \param guiBuffer    [IN]		Buffer holding history data
* \param nNumofItem   [IN]		Stores no of children in the submenu
* \param nDispAttribute   [IN]  Stores display attribue
* \param nStrItemList     [OUT]  Stores the strings id of submenus returned 
* \param ItemType		  [OUT]	�Ӳ˵��б�
* \return void
*/
void PT_CreateNewScreen(GBUINT16 scrnid,
						GBUINT16 menu_item_id,
						GBUINT8 *guiBuffer,
						GBUINT16 *nNumofItem,
						GBUINT16 *nDispAttribute,
						GBUINT16 *nStrItemList,
						GBUINT8 **ItemType)
{
#if __MTK__ > 0
	/* 2 Get current screen to gui buffer  for history purposes */
    guiBuffer = GetCurrGuiBuffer(scrnid);
	
    if(nNumofItem)
		*nNumofItem = GetNumOfChild_Ext(menu_item_id);
	
    /* 3. Get attribute of menu to be displayed */
    *nDispAttribute = (GBUINT16)GetDispAttributeOfItem(menu_item_id);
	
    /* 4. Retrieve string ids in sequence of given menu item to be displayed */
    /* diamond, 2005/07/01 add _Ext to menu item functions */
	if(nStrItemList)
		GetSequenceStringIds_Ext(menu_item_id, nStrItemList);
	
    /* 5 Set current parent id */
    SetParentHandler(menu_item_id);
	
    /* 6 Register highlight handler to be called in menu screen */
    RegisterHighlightHandler(ExecuteCurrHiliteHandler);
	
    /* 8 Construct the Hints List */
    ConstructHintsList(menu_item_id, ItemType);
#endif
}

/*!
* \brief �б�˵���Ļ
 * \param title                       [IN]        Title for the screen
 * \param title_icon                  [IN]        Icon displayed with the title
 * \param left_softkey                [IN]        Left softkey label
 * \param left_softkey_icon           [IN]        Left softkey icon
 * \param right_softkey               [IN]        Right softkey label
 * \param right_softkey_icon          [IN]        Right softkey icon
 * \param number_of_items             [IN]        Number of items in the menu
 * \param list_of_items               [IN]        Array of items
 * \param list_of_icons               [IN]        Array of icons
 * \param list_of_descriptions        [IN]        Array of Pop up description strings
 * \param flags                       [IN]        Flags (see explanation below)
 * \param highlighted_item            [IN]        Default item to be highlighted (if there is no history)
 * \param history_buffer              [IN]        History buffer
* \return void
*/
void PT_ShowListScreen(
        GBUINT16 title,
        GBUINT16 title_icon,
        GBUINT16 left_softkey,
        GBUINT16 left_softkey_icon,
        GBUINT16 right_softkey,
        GBUINT16 right_softkey_icon,
        GBINT number_of_items,
        GBUINT16 *list_of_items,
        GBUINT16 *list_of_icons,
        GBUINT8 **list_of_descriptions,
        GBINT flags,
        GBINT highlighted_item,
        GBUINT8 *history_buffer)
{
#if __MTK__ > 0
	GBUINT8 count;
	U8 *subMenuData[GBMAX_SUB_MENUS];

	memset(subMenuData, 0, sizeof(subMenuData));
	for (count = 0; count < number_of_items; count++)
    {
        subMenuData[count] = (U8 *)GetString(list_of_items[count]);
    }
    ShowCategory353Screen(
        (GBUINT8*) GetString(title),
        title_icon,
        left_softkey,
        left_softkey_icon,
        right_softkey,
        right_softkey_icon,
        number_of_items,
        (U8 **)subMenuData,
        list_of_icons,
        list_of_descriptions,
        flags,
        highlighted_item,
        history_buffer);
#endif
}

/*!
* \brief ��ѡ�˵���Ļ
 * \param  title                       [IN]        Title for the screen
 * \param  title_icon                  [IN]        Icon displayed with the title
 * \param  number_of_items             [IN]        Number of items in the menu
 * \param  list_of_items               [IN]        Array of items
 * \param  list_of_states              [IN]        Item state list
 * \param  highlighted_item            [IN]        Default item to be highlighted (if there is no history)
 * \param  history_buffer              [IN]        History buffer
* \return void
*/
void PT_ReelectionFrameScreen(
		GBUINT16 title,
		GBUINT16 title_icon,
		GBINT	 number_of_items,
		GBUINT16 *list_of_items,
		GBUINT8 *list_of_states,
		GBINT	 highlighted_item,
		GBUINT8	 *history_buffer)
{
#if __MTK__ > 0
	GBUINT8 count;
	U8 *subMenuData[GBMAX_SUB_MENUS];


	memset(subMenuData, 0, sizeof(subMenuData));	
	for (count = 0; count < number_of_items; count++)
    {
		subMenuData[count] = (U8 *)GetString(list_of_items[count]);
        //mmi_ucs2ncpy(subMenuData[count], GetString(list_of_items[count]), MAX_SUBMENU_CHARACTERS);
		//list_of_states[count] = GetConfigMenuDataState(list_of_items[count]);
    }

	//	SetCheckboxToggleRightSoftkeyFunctions(GBSettiingSaveConfirm, GoBackHistory);
	ShowCategory140Screen(
         title,
         title_icon,
         number_of_items,
         (U8 **)subMenuData,
         list_of_states,
         highlighted_item,
         history_buffer);
#endif
}

/*!
* \brief ��ѡ�˵���Ļ
 * \param title                   [IN]        Title for the screen
 * \param title_icon              [IN]        Icon shown with the title
 * \param left_softkey            [IN]        Left softkey label
 * \param left_softkey_icon       [IN]        Icon for the left softkey
 * \param right_softkey           [IN]        Right softkey label
 * \param right_softkey_icon      [IN]        Icon for the right softkey
 * \param number_of_items         [IN]        Number of items
 * \param list_of_items           [IN]        List of text items
 * \param highlighted_item        [IN]        Zero based index of the highlighed item (used if there is no history)
 * \param history_buffer          [IN]        History buffer
* \return void
*/
void PT_ShanSelectionsFrameScreen(
		  GBUINT16 title,
		  GBUINT16 title_icon,
		  GBUINT16 left_softkey,
		  GBUINT16 left_softkey_icon,
		  GBUINT16 right_softkey,
		  GBUINT16 right_softkey_icon,
		  GBINT number_of_items,
		  GBUINT16 *list_of_items,
		  GBUINT16 highlighted_item,
		  GBUINT8 *history_buffer)  
{
#if __MTK__ > 0

	GBUINT8 count;
	U8 *subMenuData[GBMAX_SUB_MENUS];

//	RegisterHighlightHandler(highlight_guobi_productconfiguration);

	memset(subMenuData, 0, sizeof(subMenuData));
	for (count = 0; count < number_of_items; count++)
    {
        subMenuData[count] = (U8 *)GetString(list_of_items[count]);
    }

	 ShowCategory36Screen(
		   title,
		   title_icon,
		   left_softkey,
		   left_softkey_icon,
		   right_softkey,
		   right_softkey_icon,
		   number_of_items,
		   (U8 **)subMenuData,
		   highlighted_item,
		   history_buffer);
#endif
}

/*!
* \brief ����һ��ֻ���ı���Ļ
 * \param title                   [IN]        Title for the screen
 * \param title_icon              [IN]        Icon shown with the title
 * \param left_softkey            [IN]        Left softkey label
 * \param left_softkey_icon       [IN]        Icon for the Left softkey
 * \param right_softkey           [IN]        Right softkey label
 * \param right_softkey_icon      [IN]        Icon for the right softkey
 * \param buffer                  [IN]        Buffer the input box should use
 * \param buffer_size             [IN]        Size of the buffer
 * \param history_buffer          [IN]        History buffer
 * \return void
*/
void PT_ReadOnlyTextFrameScreen(
								GBU16 title,
								GBU16 title_icon,
								GBU16 left_softkey,
								GBU16 left_softkey_icon,
								GBU16 right_softkey,
								GBU16 right_softkey_icon,
								GBU8 *buffer,
								GBINT buffer_length,
								GBU8 *history_buffer)
{	
#if __MTK__ > 0
	ShowCategory74Screen(
		title,
		title_icon,
		left_softkey,
		left_softkey_icon,
		right_softkey,
		right_softkey_icon,		
		buffer,
		buffer_length,
        history_buffer);
#endif
}

/*!
* \brief �˳�ֻ���ı���Ļ
 * \return void
*/
void PT_ExitReadOnlyScreen(void)
{
#if __MTK__ > 0
	ExitCategory74Screen();
#endif
}

//#if __MTK__ > 0
/*!
 * \brief �����������Ӧ�Ļص�����
 *  SetLeftSoftkeyFunction
 * DESCRIPTION
 *  Sets the function that is called when a particular softkey event occurs
 * \param f [IN]        Is the function that is called
 * \param k [IN]        Is the key event (up, down...)
 * \return void
*/
void PT_SetLeftSoftkeyFunction(void (*f) (void), MMI_key_event_type k)
{
#if __MTK__ > 0
	SetLeftSoftkeyFunction(f, k); 
#endif
}

/*!
* \brief �����������Ӧ�Ļص�����
 *  SetRightSoftkeyFunction
 * DESCRIPTION
 *  Sets the function that is called when a particular softkey event occurs
 * PARAMETERS
 * \param  f       [IN]        Is the function that is called
 * \param  k       [IN]        Is the key event (up, down...)
 * \return void
*/
void PT_SetRightSoftkeyFunction(void (*f) (void), MMI_key_event_type k)
{
#if __MTK__ > 0
	SetRightSoftkeyFunction(f, k);
#endif
}
/*!
* \brief ע���������
 *  RegisterHighlightHandler
 * DESCRIPTION
 *  Registers the highlight handler to be called in menu screens
 * \param  f           [IN]        Is a pointer to a highlight handler function
 * \param  This(?)     [IN]        Is a zero based index of the highlighted menuitem.
 * \return void
*/
void PT_RegisterHighlightHandler(void (*f) (GBINT item_index))
{
#if __MTK__ > 0
	RegisterHighlightHandler(f);
#endif
}

/*!
* \brief ���ð����ص�
 *  SetKeyHandler
 * DESCRIPTION
 *  This is used to sets the curent screen key handlers
 * PARAMETERS
 * \param  funcPtr     :[IN] Function to be executed on occurrence of the event.
 * \param  keyCode     :[IN] Key to which this event is associated.
 * \param  keyType     :[IN] KEY_EVENT_UP, KEY_EVENT_DOWN, event occurs when the key
 *                   is pressed or released
 * \return void
*/
void PT_SetKeyHandler(FuncPtr funcPtr,GBU16 keyCode,GBU16 keyType)
{
#if __MTK__ > 0
	SetKeyHandler(funcPtr, keyCode, keyType);
#endif
}
//#endif

/*!
* \brief �����޸�Checkbox����ɵĻص�
 *  SetCheckboxToggleRightSoftkeyFunctions
 * DESCRIPTION
 *  SEt the RSK function phandlers of check box screen
 * PARAMETERS
 * \param  done_function       [IN]        
 * \param  back_function       [IN]        
 * \param  S32(?)              [IN]        H:- index of highlight item
 * \return void
*/
void PT_SetCheckboxToggleRightSoftkeyFunctions(void (*done_function) (void), void (*back_function) (void))
{
#if __MTK__ > 0
	SetCheckboxToggleRightSoftkeyFunctions(done_function, back_function); 
#endif
}

/*!
 * \brief ��ʾ����ʽ��Ϣ��(һ��ʱ�������Զ������ϼ�����)
 * \param nMessageStringID ��ʾ��Ϣ�ַ���ID
 * \param popupDuration ��ʾʱ��(����)
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-8-20 11:03:00
*/
void PT_DisplayPopupMessageBox(GBU16 nMessageStringID, GBU32 popupDuration)
{
#if __MTK__ > 0
	RedrawCategoryFunction();
	DisplayPopup((PU8) GetString(nMessageStringID), (U16)IMG_GLOBAL_ACTIVATED, TRUE, popupDuration, (U8)SUCCESS_TONE);
    DeleteNHistory(1);
#endif
}

/*!
 * \brief ����������ʾ��
 * \param pfLeftSoftkeyCallback ������ص�����
 * \param pfRightSoftkeyCallback ������ص�����
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-8-20 11:08:21
*/
void PT_DisplayConfirmMessageBox(void (*pfLeftSoftkeyCallback) (void), void (*pfRightSoftkeyCallback) (void))
{
#if __MTK__ > 0
	DisplayConfirm(
        STR_GLOBAL_YES,
        IMG_GLOBAL_YES,
        STR_GLOBAL_NO,
        IMG_GLOBAL_NO,
        get_string(STR_GLOBAL_SAVE_ASK),
        IMG_GLOBAL_QUESTION,
        WARNING_TONE);

    SetLeftSoftkeyFunction(pfLeftSoftkeyCallback, KEY_EVENT_UP);
    SetRightSoftkeyFunction(pfRightSoftkeyCallback, KEY_EVENT_UP);
#endif
}

/*!
 * \brief ����������
 * \param pTouchRegion 
 * \param startPos 
 * \param iRegionWidth 
 * \param iRegionHeight 
 * \param ppCandidates 
 * \param iCandidateCount 
 * \param font 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-8-23 20:11:17
*/
GBBOOL PT_CreateTouchRegion(PGBIMETouchRegion pTouchRegion,
								GBIMEPoint startPos,		// ��ʼλ��
								GBUINT16 iRegionWidth,		// �����ܿ��
								GBUINT16 iRegionHeight,		// �����ܸ߶�
								GBPCWCHAR *ppCandidates,	// ��ѡָ��
								GBUINT16 iCandidateCount,	// ��ѡ����
								PT_FontSize font,			// ����
								PT_TextAlign textAlign)		// �ı�����
{
	GBUINT16 i;
	GBUINT16 left,top,right,bottom;
	GBPCWCHAR pCand = NULL;
	GBUINT16 iCandWidth = 0;
	GBUINT16 iCandWidthArray[GBIME_DEF_MAX_CAND_WORD];
	GBUINT16 iCandTotalWidth = 0;
	GBUINT16 iLimitWidth = 0;
	GBUINT16 iAvailableCount = 0;
	GBUINT16 iTextOffsetX = 0;
	GBUINT16 iTextOffsetY = 0;
	GBWCHAR pNewCand[GB_CAND_BUFFER_LEN];
	GBUINT16 remainSpacing = 0;

	memset(pTouchRegion, 0, sizeof(GBIMETouchRegion));
	memset(iCandWidthArray, 0, sizeof(iCandWidthArray));

	if (iCandidateCount == 0 || iCandidateCount > GBIME_DEF_MAX_CAND_WORD)
	{
		return GBFalse; // too much candidates
	}

	left	= startPos.x;
	top		= startPos.y;
	right	= startPos.x;
	bottom	= top + iRegionHeight;

	// �����ѡ�ܿ�ȣ��Ӷ��趨ǡ����ˮƽ���
	for (i = 0; i < iCandidateCount; i++)
	{
		pCand = ppCandidates[i];

		// �����к�ѡ�ַ���һ�ι��ˣ����س����ո�ȷǿɼ��ַ������⴦�� - weizhiping/2010/10/28
		memset(pNewCand, 0, GB_CAND_BUFFER_LEN);
		PT_ProprietarySymbolSwitch(pCand, pNewCand);
		iCandWidthArray[i] = PT_GetStrWidthEx(pCand, font);
		iCandTotalWidth += iCandWidthArray[i];
		iAvailableCount++;
		if (iCandTotalWidth >= iRegionWidth)
		{
			//return GBFalse; // exceed region max width
			iCandTotalWidth = iRegionWidth;
			iCandWidthArray[i] = iRegionWidth;
			break; // ���봮����ʱ�ضϴ���
		}
	}

	// ��̬����õ�ǡ���ĺ�ѡ������ظ���
	pTouchRegion->nHorizontalSpacing = (GBUINT16) ((iRegionWidth - iCandTotalWidth) / (iCandidateCount * 2));
	remainSpacing = (GBUINT16) ((iRegionWidth - iCandTotalWidth) % (iCandidateCount * 2));
	pTouchRegion->nVerticalSpacing = (GBUINT16) ((iRegionHeight - PT_GetStrHeightEx(ppCandidates[0], font)) / 2);
	//pTouchRegion->nVerticalSpacing = GBIME_VERTICAL_SPACING; // ��̬�����Ȼ������(???)

	// �����ı������������ı�����λ��Ҫ���ӵ�ƫ����
	switch (textAlign)
	{
	case PT_TEXT_ALIGN_LEFT: //����
		iTextOffsetX = 0;
		iTextOffsetY = pTouchRegion->nVerticalSpacing;
		break;
	case PT_TEXT_ALIGN_CENTER: //����
	case PT_TEXT_ALIGN_RIGHT: //����			
	default:
		iTextOffsetX = pTouchRegion->nHorizontalSpacing;
		iTextOffsetY = pTouchRegion->nVerticalSpacing;
		break;
	}	

	// ���������Ϣ
	for (i = 0; i < iAvailableCount; i++)
	{
		// ʣ��Ŀհ׼��׷�Ӹ����һ����ѡ�ַ��������������Ч�������
		if (remainSpacing > 0 && i == iAvailableCount - 1)
		{
			right = (GBUINT16 )(left + iCandWidthArray[i] + 2 * pTouchRegion->nHorizontalSpacing + remainSpacing);
			iTextOffsetX += remainSpacing / 2;
		}
		else
		{
			right = (GBUINT16 )(left + iCandWidthArray[i] + 2 * pTouchRegion->nHorizontalSpacing);
		}

		pTouchRegion->candidateRectArray[i].left = left;
		pTouchRegion->candidateRectArray[i].top = top;
		pTouchRegion->candidateRectArray[i].right = right;
		pTouchRegion->candidateRectArray[i].bottom = bottom;

		pTouchRegion->candidatePosArray[i].x = left + iTextOffsetX;
		pTouchRegion->candidatePosArray[i].y = top + iTextOffsetY;

		left = right;
		pTouchRegion->candidateNum++;
	}

	return GBTrue;
}

/*!
 * \brief ��ⴥ���������еĵ�Ԫ����
 * \param pTouchRegion 
 * \param hitPoint 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-8-23 20:10:36
*/
GBINT PT_TouchRegionHitTest(PGBIMETouchRegion pTouchRegion, GBIMEPoint hitPoint)
{
	GBINT i;

	if (pTouchRegion->candidateNum == 0)
	{
		return -1;
	}

	for (i = 0; i < pTouchRegion->candidateNum; i++)
	{
		if (PT_PointIsInRect(hitPoint, pTouchRegion->candidateRectArray[i]))
		{
			return i;
		}
	}

	return -1;
}

/*!
 * \brief �༭�����ݴ���(�Ҷ����Ű洦��)
 * \param x 
 * \param y 
 * \param limitWidth ���Ի��Ƶ����ؿ������
 * \param drawString 
 * \param font 
 * \param clr 
 * \return ʵ�ʻ��˶��ٸ�����
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-8-31 10:01:34
*/
GBU16 PT_RightAlignDrawString(GBINT x, GBINT y, GBINT limitWidth, GBPCWCHAR drawString, PT_FontSize font, PT_COLOR clr)
{
	GBINT widthLeft = limitWidth;
	GBINT drawStringWidth = 0;
#if __MTK__ > 0
	GBINT len;
	GBPWCHAR pTextStart;

	if(drawString == NULL || *drawString == 0)
	{
		return 0;
	}

	PT_SetFont(font);
	PT_SetTextColor(clr);
	len = wcslen(drawString);

	pTextStart = (GBPWCHAR)drawString;
	drawStringWidth = PT_GetStrWidthEx(drawString, font);
	while(widthLeft <= drawStringWidth && pTextStart < (drawString + len))
	{
		pTextStart++;
		drawStringWidth = PT_GetStrWidthEx(pTextStart, font);
	}
	PT_PrintText(x, y, pTextStart);
#endif
	//return limitWidth - widthLeft;
	return drawStringWidth;
}

/*!
 * \brief ���Ʊʻ��ַ���(�Ҷ����Ű洦��)
 * \param x 
 * \param y 
 * \param limitWidth ���Ի��Ƶ����ؿ������
 * \param drawString 
 * \param font 
 * \param clr 
 * \return ʵ�ʻ��˶��ٸ�����
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-8-31 10:01:34
*/
GBU16 PT_RightAlignDrawStrokeString(GBINT x, GBINT y, GBINT limitWidth, GBPCWCHAR drawString, PT_FontSize font, PT_COLOR clr)
{
	GBINT widthLeft = limitWidth;
	GBINT drawStringWidth = 0;
#if __MTK__ > 0
	GBINT len;
	GBPWCHAR pTextStart;

	if(drawString == NULL || *drawString == 0)
	{
		return 0;
	}

	len = wcslen(drawString);
	pTextStart = (GBPWCHAR)drawString;
	drawStringWidth = PT_GetStrokeStrWidth(drawString, font);
	while(widthLeft <= drawStringWidth && pTextStart < (drawString + len))
	{
		pTextStart++;
		drawStringWidth = PT_GetStrokeStrWidth(pTextStart, font);
	}
	PT_PrintStrokeText(pTextStart, x, y, font, clr);
#endif
	//return limitWidth - widthLeft;
	return drawStringWidth;
}

/*!
 * \brief �����ű��еķ���ת���ɷ����ַ�����ʽ
 * \param pSymbolString ���ת����ķ����ַ���
 * \param symbolTable ���ת����ķ����ַ���
 * \param symbolNum ������Ŀ
 * \return ת���Ƿ�ɹ�
 * \note 
 * \author weizhiping
 * \date 2010��2��1��
 */
GBBOOL PT_ConvertSymbolString(GBPWCHAR pSymbolString, const GBWCHAR ** symbolTable, GBINT symbolNum)
{
#if __MTK__ > 0
	GBINT i;
	UI_string_type pTempString = pSymbolString;

	if (!pSymbolString || !symbolTable || symbolNum == 0)
	{
		return GBFalse;
	}
    
	pTempString[0] = 0;

	for (i = 0; i < symbolNum; i++)
	{
		if (i == 0)
		{
			mmi_wcscpy(pTempString, symbolTable[i]);
		}
		else
		{
			mmi_wcscat(pTempString, symbolTable[i]);
		}
	}
	
#endif
	return GBTrue;
}

/*!
 * \brief ��ȫ��������ű��еı���ת���ɱ����ַ�����ʽ
 * \param pFaceString ���ת����ķ����ַ���
 * \param faceTable ������ű�
 * \param faceNum ���������Ŀ
 * \return ת���Ƿ�ɹ�
 * \note 
 * \author weizhiping
 * \date 2010��3��11��
 */
GBBOOL PT_ConvertFaceString(GBPWCHAR pFaceString, const GBWCHAR ** faceTable, GBINT faceNum)
{
#if __MTK__ > 0
	GBINT i;
	UI_string_type pTempString = pFaceString;

	if (!pFaceString)
	{
		return GBFalse;
	}

	pTempString[0] = 0;

	for (i = 0; i < faceNum; i++)
	{
		if (i == 0)
		{
			mmi_wcscpy(pTempString, faceTable[i]);
		}
		else
		{
			mmi_wcscat(pTempString, faceTable[i]);
		}
		
		if (i < faceNum - 1)
		{
			mmi_wcscat(pTempString, GBIME_SPLIT_STR); // ���һ������ָ���
		}
	}
#endif
	return GBTrue;
}
//#endif // __MTK__

/*!
 * \brief ��������֮�����Ľ���ֵ
 * \param disX X�����ֵ
 * \param disY Y�����ֵ
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-24 16:10:36
*/
GBINT PT_GetPointDistance(GBINT disX, GBINT disY)
{
	GBINT mn = 0;
	
	disX = ABS(disX);
	disY = ABS(disY);
	mn = MIN(disX, disY);
	
	return(disX + disY - (mn >> 1) - (mn >> 2) + (mn >> 4));
}

/*!
 * \brief WideChar��MultiByte֮���ת��
 * \param mbs MultiByte
 * \param wcs WideChar
 * \param mbslen MultiByte����
 * \autor lidasheng
 * \return 
 */
GBINT PT_wcstombs(GBPACHAR mbs, GBPCWCHAR wcs, GBINT mbslen)
{
#ifdef __GBIME_LOG__
	const char * pStart = mbs;
	const char * pEnd = mbs + mbslen;
	char * pTempMbs = mbs;

	/*! 5 �� mbs����Ҫ����2���ֵĿռ�*/
	while(*wcs && pTempMbs + 5 < pEnd)
	{
		/*! 0x4E00��unicodeֵ��4e00��ʼ��
			20902 ������20902��unicodeֵ
			*/
		if(*wcs >= 0x4E00 && *wcs < 0x4E00 + 20902)
		{	
			memcpy(pTempMbs, (unsigned char *)uni2gb+(*wcs - 0x4E00)*2, 2);
			pTempMbs[2] = 0;
			++wcs;
		} 
		else if(*wcs > 127)
		{
#ifdef WIN32
			sprintf(pTempMbs, "\\h%x", *wcs++);
#else
			/*!
			*/
			if(*wcs == 0xff1f)
			{
				sprintf(pTempMbs, "��");
			}
			else if (*wcs == 0xff01)
			{
				sprintf(pTempMbs, "��");
			}
			else if (*wcs == 0xff61)
			{
				sprintf(pTempMbs, "��");
			}
			else
			{
				sprintf(pTempMbs, "\\h%x", *wcs);
			}
			wcs++;
#endif
		}
		else
		{
			sprintf(pTempMbs, "%c", *wcs++);
		}
		pTempMbs += strlen(pTempMbs);
	}
	*pTempMbs = 0;
	return (GBINT)(pTempMbs - pStart);
#else
	return 0;
#endif
}

/*!
* \brief MultiByte��WideChar֮���ת��
* \param mbs MultiByte
* \param wcs WideChar
* \param wcslen WideChar����
* \autor zhaokun
* \return 
*/
GBINT PT_mbstowcs(unsigned short *wcs, char *mbs,int wcslen)
{
#ifdef __GBIME_LOG__
	const unsigned short* pStart = wcs;
	char* uni2gbTemp = (char*)uni2gb;
	char word[3] = {0};
	strncpy(word,mbs,2);
	word[2] = 0;
	while(*mbs && wcs+1 < pStart + wcslen)
	{
		uni2gbTemp = (char*)strstr((char*)uni2gbTemp,word);
		if(uni2gbTemp != NULL
			&& (uni2gbTemp - (char*)uni2gb) % 2 == 0)
		{
			*wcs =(unsigned short) (0x4E00 + (uni2gbTemp - (char*)uni2gb)/2);
			wcs[1] = 0;
			mbs += 2;
			uni2gbTemp = (char*)uni2gb;
			strncpy(word,mbs,2);
			word[2] = 0;
			wcs++;
		}
		else
		{
			uni2gbTemp++;
		}
	}
	*wcs = 0;
	return (GBINT)(wcs - pStart);
#else
	return 0;
#endif
}	

/*!
 * \brief ʹ�ܿ��ӻ���ʾ�ո񡢻س��ȷ��Ź���
 * \param void 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-10-28 17:22:49
 */
void PT_EnableProprietarySymbolDisplay(void)
{
#if __MTK__ > 0
	mmi_fe_enable_proprietary_symbol_display();
#endif // __MTK__
}

/*!
 * \brief ���ÿ��ӻ���ʾ�ո񡢻س��ȷ��Ź���
 * \param void 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-10-28 17:22:49
 */
void PT_DisableProprietarySymbolDisplay(void)
{
#if __MTK__ > 0
	mmi_fe_disable_proprietary_symbol_display();
#endif // __MTK__
}

/*!
 * \brief �������ַ����л�����Ҫ��Կո񡢻س����ʻ�
 * \param pOldStr ԭ�ַ���
 * \param pNewStr 
 * \return 
 * \note �ú������Դ�����������ֽڼ�飬����ʱ��Ҫע��,pOldStr�������ַ�����pNewStr�ռ�����㹻
 *\example
 * \author Zhaokun
 * modify by weizhiping:  �ú������ۼӺ�ѡ��ÿ���ַ��Ŀ�ȣ���Ϊʸ���ֿ��»��������ѡ��������Ŀ�Ȳ�һ��
 * \date 2010-10-28 17:22:49
 */
void PT_ProprietarySymbolSwitch(GBPCWCHAR pOldStr, GBPWCHAR pNewStr)
{
	GBINT i = 0;

	if(!pOldStr || !pNewStr)
	{
		return ;
	}

	while(pOldStr[i])
	{
		switch (pOldStr[i])
		{
		case 0x0020: // �ո�
			pNewStr[i] = 0x0086;
			break;
		case 0x000A: // �س�
			pNewStr[i] = 0x0085;
			break;
		case 0xE836://����ͷ
			pNewStr[i] = 0x2EAE;
			break;
		case 0xE822://С��ͷ
			pNewStr[i] = 0x2E8C;
			break;
		default:
			pNewStr[i] = pOldStr[i];
			break;
		}
		i++;
	}
	pNewStr[i] = 0;
}

/*!
 * \brief ��ȡ���״�����(GB2312�޷���ʾ�ַ��ö������)
 * \param pComponentString 
 * \param font 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-12-8 15:43:12
*/
GBINT PT_GetComponentStringWidth(GBPCWCHAR pComponentString, PT_FontSize font)
{
	GBINT i = 0;
	GBINT nWidth = 0;
	GBWCHAR comStr[2] = {0};
	
	if(!pComponentString)
	{
		return 0;
	}
	
	while(pComponentString[i])
	{
		switch (pComponentString[i])
		{
		case 0x4E5B: // �^
		case 0xE836: //����ͷ					
		case 0xE822: //С��ͷ
			nWidth += GBIME_COMP_WIDTH;
			break;
		default:
			comStr[0] = pComponentString[i];
			nWidth += PT_GetStrWidthEx(comStr, font);
			break;
		}
		i++;
	}
	
	return nWidth;
}