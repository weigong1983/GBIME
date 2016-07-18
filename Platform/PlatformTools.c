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
* Purpose      :  封装MTK平台的屏幕模板操作、字符操作、内存操作、文件访问、按键消息回调注册/注销、字符串显示等工具函数
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
 * \defgroup 绘图相关API
 */

/*!
 * \brief 将常量定义的字号大小转换成MTK平台的相应字体类型(内部使用)
 * \param font 参见PT_FontSize
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
//用于测试时打印log
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
 * \brief 打印语句
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
 * \brief 打印语句
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
	//vswprintf(g_trace_bufW, sizeof(g_trace_bufW), fmt, trace_p); // VS2008 需要四个参数
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
//  * \brief 将宽字符Unicode字符串转换成Ansi字符串【只用于ASCII码字符的转换，显示LOG用】
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
 * \brief 启动平台定时器
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
 * \brief 停止平台定时器
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
 * \brief 平台GUI简易定时器启动函数(不带定时器ID)
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
 * \brief 平台GUI简易定时器取消函数(不带定时器ID)
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
 * \brief 获取系统开机到现在的毫秒数
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
 * \brief 计算 Unicode  类型的字符串占多少点宽度
 * \param pParam 参见GBGlobalPageOption.pGetStringWidthParam的设置
 * \param str 源字符串
 * \return 此Unicode字符串的像素宽度
 * \note 调用范例
 	GBGlobalPageOption pageOption;
 	GBGetPageOption(pGbis, &pageOption);
	pageOption.fpGetWStringWidth = PT_GetStrWidth;
	GBSetPageOption(pGbis, &pageOption);
 * \author weizhiping
 * \date 2009年12月11日
 */
int PT_GetStrWidth(void * pParam, GBLPCWCHAR str)
{
	return PT_GetStrWidthEx(str, GBIME_CANDIDATE_FONT_SIZE); // GBV5引擎计算候选字符宽度默认采用候选字体，否则和显示时不一致
	//return gui_get_string_width((UI_string_type)str);
}

/*!
 * \brief 获取平台LCD宽度
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
 * \brief 获取平台LCD高度
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
 * \brief 平台打印字符串函数
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
 * \brief 绘制字符串
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
		PT_PrintLogInfo(GBLOG_PLATFORM, ("*****PT_DrawString：%s\r\n", buf));
	}
#endif
}

/*!
 * \brief 笔画绘制函数(考虑小字号无法显示偏旁“乛”“小”“竹”的处理)
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
#if 1//def __GB2312__ //  偏旁“乛”“小”“竹”在代码区显示为“口”
		if(pStrokeText[i] == 0x4E5B) // 乛( 0x4E5B)
		{
			gui_measure_image(get_image((U16)(IMG_GB_STROKE_5)), &strokeWidth, &strokeHeight);
			gui_show_image(strokeLeft, strokeTop, get_image((U16)(IMG_GB_STROKE_5)));		             
		}
		else if(pStrokeText[i] == 0xE822) //⺌ (0xE822)
		{
			gui_measure_image(get_image((U16)(IMG_GB_COMP_XIAO)), &strokeWidth, &strokeHeight);
			gui_show_image(strokeLeft, strokeTop, get_image((U16)(IMG_GB_COMP_XIAO)));		             
		}
		else if(pStrokeText[i] == 0xE836) //⺮（0xE836) 
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
 * \brief 在指定位置，根据图片ID显示图片
 * \param iLeftX 显示图片的左上角X坐标
 * \param iTopY 显示图片的左上角Y坐标
 * \param imageID 图片ID
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
	PT_PrintLogInfo(GBLOG_PLATFORM, ("****PT_DrawImage：iLeftX=%d, iTopY=%d, imageID=%d\n", iLeftX, iTopY, imageID));
#endif
}
/*!
 * \brief 将图片中某一指定的区域显示到屏幕上指定的区域
 * \param imageID 图片ID
 * \param display_rect_ptr 屏幕上的显示区域
 * \param image_rect_ptr 图片中指定的区域
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
 * \brief 填充矩形区域
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
 * \brief 画矩形区域
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
 * \brief 画点
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
 * \brief 画一条直线
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
 * \brief 画一条水平线
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
 * \brief 画一条垂直线
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
 * \brief 绘制带样式的垂直线条
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
 * \brief 画一条指定粗细宽度的直线
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
 * \brief 画一条指定粗细宽度的直线(可指定裁剪区，裁剪区之外不绘制)
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
 

    //初始点向上或向左方向需要增加的线宽
    //half_width = (STROKE_WIDTH>>1);
    half_width = (widthInPixels >> 1);
    //初始点向下或向右方向需要增加的线宽
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

    //判断线条是否在裁减区
    if (x1 > clip_right || x2 < clip_left || y1 > clip_bottom || y2 < clip_top)
    {
        return;
    }

    dx = x2 - x1;
    dy = y2 - y1;

    //设置符合裁减区的数据
    x1 = MAX((GBINT16)x1, (GBINT16)clip_left);
    x2 = MIN((GBINT16)x2, (GBINT16)clip_right);
    y1 = MAX((GBINT16)y1, (GBINT16)clip_top);
    y2 = MIN((GBINT16)y2, (GBINT16)clip_bottom);

    if (dx > dy)
    {
        //设置线条的最小坐标
        if (y1 < clip_top + half_width)
        {
            y1 = clip_top + half_width;
        }
        else if (y1 + next_width > clip_bottom)
        {
            y1 = clip_bottom - next_width;
        }

        //设置线条的最大坐标
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
        //设置线条的最小坐标
        if (x1 < clip_left + half_width)
        {
            x1 = clip_left + half_width;
        }
        else if (x1 + next_width > clip_right )
        {
            x1 = clip_right - next_width;
        }

        //设置线条的最大坐标
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
 * \defgroup 工具函数
 */

/*!
* \brief 判断一个点坐标是否落在指定矩形区域内
* \param point 
* \param rect 
* \return 
*/
#if 0 // 宏替换优化
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
 * \brief 刷新屏幕指定区域
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
* \brief 清屏
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
* \brief get string for given id 获取字符串
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
* \brief 获取字符串长度
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
* \brief 获得字符高度
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
 * \brief 获得指定字体的字符串宽度
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
 * \brief 获得指定字体的笔画字符串宽度
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

#if 1//def __GB2312__ //  包含偏旁“乛”“小”“竹”直接按图片宽度算单个笔画长度
	while (i < strLen)
	{
		if(pText[i] == 0x4E5B) // 乛( 0x4E5B)
		{
			gui_measure_image(get_image((U16)(IMG_GB_STROKE_5)), &strokeWidth, &strokeHeight);
			strokeImgTotalWidth += strokeWidth + nInterval;		             
		}
		else if(pText[i] == 0xE822) //⺌ (0xE822)
		{
			gui_measure_image(get_image((U16)(IMG_GB_COMP_XIAO)), &strokeWidth, &strokeHeight);
			strokeImgTotalWidth += strokeWidth + nInterval;             
		}
		else if(pText[i] == 0xE836) //⺮（0xE836) 
		{
			gui_measure_image(get_image((U16)(IMG_GB_COMP_ZHU)), &strokeWidth, &strokeHeight);
	        strokeImgTotalWidth += strokeWidth + nInterval;
		}
		else // 非贴图的笔画显示串过滤出来单独计算宽度
		{
			tempString[j++] = pText[i];
		}

		i++;
	}
#endif
	
	// 该笔画串的实际显示宽度 = 非贴图的笔画显示串宽度 + 贴图字符串宽度
	tempString[j] = 0;
	strokeTotalWidth = PT_GetStrWidthEx(tempString, font);
	strokeTotalWidth += strokeImgTotalWidth;

	return strokeTotalWidth;
}

/*!
 * \brief 获得指定字体的字符高度
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
 * \brief 获取某单个字符串宽和高
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

	// ASCII 字符获取字体实际宽高
	if (c >= 0 && c < 0x80)
	{
		gui_measure_character((UI_character_type)c, (S32 *)pWidth, (S32 *)pHeight);
	} 
	else // 非ASCII采用定宽高
	{
		switch (font)
		{
		case PT_FONT_SIZE_SMALL: //小号字体
			*pWidth = 14;
			*pHeight = 14;
			break;
		case PT_FONT_SIZE_LARGE: //大号字体
			*pWidth = 24;
			*pHeight = 24;
			break;
		case PT_FONT_SIZE_MEDIUM: //中号字体
		case PT_FONT_SIZE_DEFAULT: //默认字体
		default:
			*pWidth = 20;
			*pHeight = 20;
			break;	
		}
	}
	
#endif
}

/*!
* \brief 获取图片id
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
* \brief 获取图片宽和高
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
* \brief 显示图片
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
* \brief 设置文本输出的开始坐标
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
* \brief 输出长度为N的文本
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
* \brief 设置文本颜色
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
* \brief 设置字体
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
 * \brief 检测文件夹是否存在，如果不存在则创建之
 * \param
 * \return
 * \note 
 * \author weizhiping
 * \date 2010年2月21日
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
* \brief 锁层
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
* \brief 解锁层
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
* \brief 文本剪裁区域压栈
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
* \brief 文本剪裁区域出栈
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
* \brief 获取当前活动的屏幕ID
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
* \brief 创建新屏幕
* \param scrnid		  [IN]		屏幕ID
* \param menu_item_id [IN]		菜单ID
* \param guiBuffer    [IN]		Buffer holding history data
* \param nNumofItem   [IN]		Stores no of children in the submenu
* \param nDispAttribute   [IN]  Stores display attribue
* \param nStrItemList     [OUT]  Stores the strings id of submenus returned 
* \param ItemType		  [OUT]	子菜单列表
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
* \brief 列表菜单屏幕
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
* \brief 复选菜单屏幕
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
* \brief 单选菜单屏幕
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
* \brief 创建一个只读文本屏幕
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
* \brief 退出只读文本屏幕
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
 * \brief 设置左软键对应的回调函数
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
* \brief 设置右软键对应的回调函数
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
* \brief 注册高亮函数
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
* \brief 设置按键回调
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
* \brief 设置修改Checkbox后完成的回调
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
 * \brief 显示弹出式消息框(一定时间间隔后自动返回上级界面)
 * \param nMessageStringID 提示消息字符串ID
 * \param popupDuration 提示时长(毫秒)
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
 * \brief 弹出保存提示框
 * \param pfLeftSoftkeyCallback 左软键回调函数
 * \param pfRightSoftkeyCallback 右软键回调函数
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
 * \brief 创建触摸区
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
								GBIMEPoint startPos,		// 起始位置
								GBUINT16 iRegionWidth,		// 区域总宽度
								GBUINT16 iRegionHeight,		// 区域总高度
								GBPCWCHAR *ppCandidates,	// 候选指针
								GBUINT16 iCandidateCount,	// 候选个数
								PT_FontSize font,			// 字体
								PT_TextAlign textAlign)		// 文本对齐
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

	// 计算候选总宽度，从而设定恰当的水平间隔
	for (i = 0; i < iCandidateCount; i++)
	{
		pCand = ppCandidates[i];

		// 对所有候选字符做一次过滤，将回车、空格等非可见字符做特殊处理 - weizhiping/2010/10/28
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
			break; // 输入串过长时截断处理
		}
	}

	// 动态计算得到恰当的候选间隔像素个数
	pTouchRegion->nHorizontalSpacing = (GBUINT16) ((iRegionWidth - iCandTotalWidth) / (iCandidateCount * 2));
	remainSpacing = (GBUINT16) ((iRegionWidth - iCandTotalWidth) % (iCandidateCount * 2));
	pTouchRegion->nVerticalSpacing = (GBUINT16) ((iRegionHeight - PT_GetStrHeightEx(ppCandidates[0], font)) / 2);
	//pTouchRegion->nVerticalSpacing = GBIME_VERTICAL_SPACING; // 动态计算居然不居中(???)

	// 根据文本对齐标记设置文本坐标位置要增加的偏移量
	switch (textAlign)
	{
	case PT_TEXT_ALIGN_LEFT: //居左
		iTextOffsetX = 0;
		iTextOffsetY = pTouchRegion->nVerticalSpacing;
		break;
	case PT_TEXT_ALIGN_CENTER: //居中
	case PT_TEXT_ALIGN_RIGHT: //居右			
	default:
		iTextOffsetX = pTouchRegion->nHorizontalSpacing;
		iTextOffsetY = pTouchRegion->nVerticalSpacing;
		break;
	}	

	// 填充区域信息
	for (i = 0; i < iAvailableCount; i++)
	{
		// 剩余的空白间隔追加给最后一个候选字符串，避免出现无效点击区域
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
 * \brief 检测触摸区被点中的单元索引
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
 * \brief 编辑栏内容处理(右对齐排版处理)
 * \param x 
 * \param y 
 * \param limitWidth 可以绘制的像素宽度限制
 * \param drawString 
 * \param font 
 * \param clr 
 * \return 实际画了多少个汉字
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
 * \brief 绘制笔画字符串(右对齐排版处理)
 * \param x 
 * \param y 
 * \param limitWidth 可以绘制的像素宽度限制
 * \param drawString 
 * \param font 
 * \param clr 
 * \return 实际画了多少个汉字
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
 * \brief 将符号表中的符号转换成符号字符串形式
 * \param pSymbolString 存放转换后的符号字符串
 * \param symbolTable 存放转换后的符号字符串
 * \param symbolNum 符号数目
 * \return 转换是否成功
 * \note 
 * \author weizhiping
 * \date 2010年2月1日
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
 * \brief 将全屏表情符号表中的表情转换成表情字符串形式
 * \param pFaceString 存放转换后的符号字符串
 * \param faceTable 表情符号表
 * \param faceNum 表情符号数目
 * \return 转换是否成功
 * \note 
 * \author weizhiping
 * \date 2010年3月11日
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
			mmi_wcscat(pTempString, GBIME_SPLIT_STR); // 添加一个表情分隔符
		}
	}
#endif
	return GBTrue;
}
//#endif // __MTK__

/*!
 * \brief 求两个点之间距离的近似值
 * \param disX X坐标差值
 * \param disY Y坐标差值
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
 * \brief WideChar到MultiByte之间的转换
 * \param mbs MultiByte
 * \param wcs WideChar
 * \param mbslen MultiByte长度
 * \autor lidasheng
 * \return 
 */
GBINT PT_wcstombs(GBPACHAR mbs, GBPCWCHAR wcs, GBINT mbslen)
{
#ifdef __GBIME_LOG__
	const char * pStart = mbs;
	const char * pEnd = mbs + mbslen;
	char * pTempMbs = mbs;

	/*! 5 ： mbs至少要留有2个字的空间*/
	while(*wcs && pTempMbs + 5 < pEnd)
	{
		/*! 0x4E00：unicode值从4e00开始。
			20902 ：共有20902个unicode值
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
				sprintf(pTempMbs, "？");
			}
			else if (*wcs == 0xff01)
			{
				sprintf(pTempMbs, "！");
			}
			else if (*wcs == 0xff61)
			{
				sprintf(pTempMbs, "。");
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
* \brief MultiByte到WideChar之间的转换
* \param mbs MultiByte
* \param wcs WideChar
* \param wcslen WideChar长度
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
 * \brief 使能可视化显示空格、回车等符号功能
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
 * \brief 禁用可视化显示空格、回车等符号功能
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
 * \brief 对特殊字符的切换，主要针对空格、回车、笔画
 * \param pOldStr 原字符串
 * \param pNewStr 
 * \return 
 * \note 该函数不对传入参数进行字节检查，传入时需要注意,pOldStr必须是字符串。pNewStr空间必须足够
 *\example
 * \author Zhaokun
 * modify by weizhiping:  该函数不累加候选字每个字符的宽度，因为矢量字库下会和整个候选计算出来的宽度不一致
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
		case 0x0020: // 空格
			pNewStr[i] = 0x0086;
			break;
		case 0x000A: // 回车
			pNewStr[i] = 0x0085;
			break;
		case 0xE836://竹字头
			pNewStr[i] = 0x2EAE;
			break;
		case 0xE822://小字头
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
 * \brief 获取部首串长度(GB2312无法显示字符用定长替代)
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
		case 0x4E5B: // 乛
		case 0xE836: //竹字头					
		case 0xE822: //小字头
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