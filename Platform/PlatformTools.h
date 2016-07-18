/***************************************************************************
* File Name    :  PlatformTools.h
* Corporation  :  Guobi Technology Ltd. GuangZhou China
* Copyright    :  Copyright Guobi Technology Ltd. And Subject to Confidentiality 
*                 Restrictions. This document is the confidential and proprietary 
*                 property of Guobi Technology Ltd.It is neither the document                  
*                 itself nor any part of it can be shared with any party  without 
*                 the written  permission. 
* Author       :  weizhiping
* Date         :  2010-06-08
* Purpose      :  封装MTK平台的屏幕模板操作、字符操作、内存操作、文件访问、按键消息回调注册/注销、字符串显示等工具函数, 避免直接引用平台函数
* Version      :  V1.0
* Last Changed List
* Name         |   date   |  Discription
****************************************************************************/

#ifndef __PLATFORM_TOOLS_H_
#define __PLATFORM_TOOLS_H_

#ifndef __MTK__
#define __MTK__           1
#endif

#ifndef __GBIME_LOG__
	#define __GBIME_LOG__			1 // 输入法框架Log打印开关
	//#define __GBIME_LOG_TO_FILE__	1 // 输入法框架Log写入文件
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#if defined(WIN32)
//#include <assert.h>
#endif

#include "GBIMEDef.h"

#if __MTK__ > 0
#include "MMI_include.h"
#include "ImeGprot.h"
#include "Wgui_categories_util.h"
#include "KeyBrd_Int.h"
#include "gui_ems.h"
#include "gui_inputs.h"
#include "mteGprot.h"
#include "Gui.h"
#include "wgui_inputs.h"
#include "fs_func.h"
#include "EventsGprot_Int.h"
#include "wgui_categories_list.h"
#include "GlobalConstants.h"
#include "CommonScreens.h"
#include "SettingProfile.h"

typedef void (*GbFuncPtr)(void);
extern single_line_input_box MMI_singleline_inputbox;
extern UI_EMS_input_box MMI_EMS_inputbox;
extern void mmi_imc_disable_handwriting();
extern void mmi_imc_enable_handwriting();
#endif

// 平台无关字体大小定义
typedef enum
{
    PT_FONT_SIZE_DEFAULT,	//默认字体
	PT_FONT_SIZE_SMALL,		//小号字体 
	PT_FONT_SIZE_MEDIUM,	//中号字体
	PT_FONT_SIZE_LARGE,		//大号字体
	PT_FONT_SIZE_TOTAL
} PT_FontSize;

// 文本对齐定义
typedef enum
{
    PT_TEXT_ALIGN_LEFT		//居左
	,PT_TEXT_ALIGN_CENTER	//居中
	,PT_TEXT_ALIGN_RIGHT	//居右
} PT_TextAlign;

// 常用颜色定义
#if __MTK__ > 0
	#define PT_COLOR_LIGHT_GREY		UI_COLOR_LIGHT_GREY
	#define PT_COLOR_WHITE			UI_COLOR_WHITE
	#define PT_COLOR_DARK_GREY		UI_COLOR_DARK_GREY
	#define PT_COLOR_BLACK			UI_COLOR_BLACK
	#define PT_COLOR_NONE			UI_COLOR_NONE
	#define PT_COLOR_RED			UI_COLOR_RED
	#define PT_COLOR_3D_FILLER		UI_COLOR_3D_FILLER
	#define PT_COLOR_GREY			UI_COLOR_GREY
	#define PT_COLOR_GREEN			UI_COLOR_GREEN
	#define PT_COLOR_BLUE			gui_color(0, 0, 200)// 255) 诡异蓝色
#else
	#define PT_COLOR_LIGHT_GREY		0
	#define PT_COLOR_WHITE			0
	#define PT_COLOR_DARK_GREY		0
	#define PT_COLOR_BLACK			0
	#define PT_COLOR_NONE			0
	#define PT_COLOR_RED			0
	#define PT_COLOR_3D_FILLER		0
	#define PT_COLOR_GREY			0
	#define PT_COLOR_GREEN			0
	#define PT_COLOR_BLUE			0
#endif

// 线条类型定义
typedef enum {	
	PT_LINT_TYPE_SOLID
	,PT_LINT_TYPE_DOTTED
	,PT_LINT_TYPE_DASHED
}PT_LINT_TYPE;

/*!
 * \ 国笔Log模块枚举，实现分模块Log打印管理
 */
typedef enum __tag_GBIME_Module{
	GBLOG_LAYOUTABSTRUCT             = 0X0001
	,GBLOG_LAYOUTINFO                = 0X0002
	,GBLOG_LAYOUTVIEW                = 0X0004
	,GBLOG_LAYOUTCONTROL             = 0X0008
	,GBLOG_LAYOUTSWTICH              = 0X0010
	,GBLOG_INPUTMETHODSWITCH         = 0X0020
	,GBLOG_IMEINTERFACE              = 0X0040
	,GBLOG_CONFIGURE                 = 0X0080
	,GBLOG_ENGINE					 = 0X0100
	,GBLOG_HANDWRITE                 = 0X0200
	,GBLOG_PLATFORM                  = 0X0400
	,GBLOG_UTILS					 = 0X0800
}PT_GBIME_MODULE;

#define ALL_LVL   (GBLOG_LAYOUTABSTRUCT  | GBLOG_LAYOUTINFO   | GBLOG_LAYOUTVIEW \
				   | GBLOG_LAYOUTCONTROL | GBLOG_LAYOUTSWTICH | GBLOG_INPUTMETHODSWITCH \
				   | GBLOG_IMEINTERFACE  | GBLOG_CONFIGURE    | GBLOG_ENGINE \
				   | GBLOG_HANDWRITE	 | GBLOG_PLATFORM	  | GBLOG_UTILS)

#define DBG_LVL ALL_LVL // 此处设置需要关注的log级别信息

#ifdef __GBIME_LOG__
	#define PT_PrintLogInfo(lvl, debug_info)     \
	{\
		if((lvl) & DBG_LVL) {\
			PT_Print debug_info;\
    		}\
	}
	/*!Sample code
	PT_PrintLogInfo(LAYOUTINFO, ("GBIME MODULE:LAYOUTINFO"));
	 */
#else// 关闭Debug信息
	#define PT_PrintLogInfo(lvl, debug_info)
#endif // __GBIME_LOG__


// 数组操作相关
#ifndef PT_ARRAY_SIZE
#define PT_ARRAY_SIZE(a)				(sizeof(a) / sizeof((a)[0])) 	// 获取数组元素数目
#endif

#ifndef CLEAR_ARRAY
#define CLEAR_ARRAY(a)	(memset((a), 0, sizeof(a)))
#endif

#ifndef MAX
#define MAX(a,b) (a > b) ? a : b
#endif 
#ifndef MIN
#define MIN(a,b) (a < b) ? a : b
#endif 
#ifndef ABS
#define ABS(x) ((x) >= 0 ? (x) : -(x))
#endif

#define SWAP_UINT16(x1, x2) {GBU16 temp; temp = (x1); (x1) = (x2); (x2) = temp;}

// 字符串操作相关
#define PT_wcscat 			wcscat 
#define PT_wcschr 			wcschr 
#define PT_wcscmp 			wcscmp 
#define PT_wcscpy 			wcscpy 
#define PT_wcscspn			wcscspn
#define PT_wcslen 			wcslen 
#define PT_wcsncmp			wcsncmp
#define PT_wcsncpy			wcsncpy
#define PT_wcsspn 			wcsspn 
#define PT_wcsncat			wcsncat
#define PT_wcsrev			wcsrev	
#define PT_wcspbrk			wcspbrk

typedef void (*TIMER_CALLBACK) (void);

#if __MTK__ > 0
	// 内存操作
	#define PT_Malloc               OslMalloc
	#define PT_Free                 OslMfree	
	// 断言
	#define PT_Assert(expr)			MMI_ASSERT(expr)
	// 文件操作
	#define GB_SYSTEM_DIR      		L"c:\\@gb\\" // 国笔系统文件夹
	#define PT_FILE_HANDLE          FS_HANDLE
	#define PT_FS_READ           	FS_READ_ONLY
	#define PT_FS_READ_WRITE		FS_READ_WRITE | FS_CREATE
	#define PT_FS_WRITE          	FS_READ_WRITE|FS_CREATE_ALWAYS
	#define PT_FS_Open(x,y)        	FS_Open((U16 *)x,y)
	#define PT_FS_Close(x)         	{ FS_Commit(x); FS_Close(x); }
	#define PT_FS_Write(FileHandle, DataPtr, Length, Written) FS_Write(FileHandle,DataPtr,Length,(UINT *)Written)
	#define PT_FS_Read(FileHandle,DataPtr,Length,Read)  FS_Read(FileHandle,DataPtr,Length,(UINT *)Read) 
	#define PT_FS_Seek(FileHandle, Offset, Whence)	FS_Seek(FileHandle, Offset, Whence)

	// 打印
	//#define PT_Print				kal_print
	#define PT_wsprintf				kal_wsprintf
	#define PT_sprintf				kal_sprintf
	#define PT_COLOR				color
#else //Win32
	#include <Windows.h>
	// 内存操作
	#define PT_Malloc               malloc
	#define PT_Free                 free
	// 断言
	#define PT_Assert(expr)			assert(expr)
	// 文件操作
	#define PT_FILE_HANDLE          FILE *
	#define PT_FS_READ           	L"r"
	#define PT_FS_READ_WRITE		L"rw"
	#define PT_FS_WRITE          	L"w"
	#define PT_FS_Open(x,y)        	_wfopen((unsigned short *)x,y)
	#define PT_FS_Close(x)         	{ fflush(x); fclose(x); }
	#define PT_FS_Write(FileHandle, DataPtr, Length, Written) fwrite(DataPtr,Length,0,FileHandle)
	#define PT_FS_Read(FileHandle,DataPtr,Length,Read)  fread(DataPtr,1,Length,FileHandle)
	#define PT_FS_Seek(FileHandle, Offset, Whence)	fseek(FileHandle, Offset, Whence)

	// 打印
	#define PT_wsprintf				wsprintf
	#define PT_sprintf				sprintf

	#define PT_COLOR				void*
		
	#define UI_DEVICE_WIDTH			320
	#define UI_DEVICE_HEIGHT		480

	#define MMI_key_event_type	GBINT
	typedef void (*FuncPtr) (void);

#endif // __MTK__

/*!
 * \brief 触摸区域对象
 */
typedef struct _tagGBTouchRegion {
		GBRECT		candidateRectArray[GBIME_DEF_MAX_CAND_WORD * 2];	///<候选对应的矩形区数组
		GBIMEPoint  candidatePosArray[GBIME_DEF_MAX_CAND_WORD * 2];		///<候选字符串对应的绘制坐标位置数组
		GBINT		candidateNum;
		GBUINT16	nHorizontalSpacing;
		GBUINT16	nVerticalSpacing;
} GBIMETouchRegion, * PGBIMETouchRegion;

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
void PT_Print(const char * fmt, ...);

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
void PT_PrintW(const wchar_t * fmt, ...);

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
// void PT_UnicodeToAnsi(GBPCWCHAR pSrcUniString, GBUINT8 *pDesAnsiString);

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
void PT_StartTimer(GBU16 timerid, GBU32 delay, TIMER_CALLBACK funcPtr);

/*!
 * \brief 停止平台定时器
 * \param timerid 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-8-17 11:08:31
*/
void PT_StopTimer(GBU16 timerid);

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
void PT_GUIStartTimer(GBINT delay, TIMER_CALLBACK callback);

/*!
 * \brief 平台GUI简易定时器取消函数(不带定时器ID)
 * \param callback 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-10-8 17:10:30
*/
void PT_GUICancelTimer(TIMER_CALLBACK callback);

/*!
 * \brief 获取系统开机到现在的毫秒数
 * \param void 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-24 9:20:38
*/
GBU32 PT_GetCurrentTime(void);

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
int PT_GetStrWidth(void * pParam, GBLPCWCHAR str);

/*!
 * \brief 获取平台LCD宽度
 * \param void 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-21 11:14:07
 */
GBIMEGLOBAL GBINT PT_GetLcdWidth(void);

/*!
 * \brief 获取平台LCD高度
 * \param void 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-21 11:13:49
 */
GBIMEGLOBAL GBINT PT_GetLcdHeight(void);

/*!
 * \defgroup 图片显示API
 */

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
GBIMEGLOBAL void PT_PrintText(GBINT x, GBINT y,  GBPCWCHAR pText);

/*!
 * \brief 绘制字符串
 * \param str 
 * \param font
 * \param clr 
 * \param left 
 * \param top 
 * \return 
 */
GBIMEGLOBAL void PT_DrawString( GBPCWCHAR str, PT_FontSize font, PT_COLOR clr, GBINT left, GBINT top);

/*!
 * \brief 笔画绘制函数(考虑小字号无法显示偏旁“乛”“小""竹“的处理)
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
void PT_PrintStrokeText(GBPCWCHAR pStrokeText, GBINT left, GBINT top, PT_FontSize font, PT_COLOR clr);

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
GBIMEGLOBAL void PT_DrawImage(GBINT iLeftX, GBINT iTopY, GBU16 imageID);

/*!
 * \brief 将图片中某一指定的区域显示到屏幕上指定的区域
 * \param imageID 图片ID
 * \param display_rect_ptr 屏幕上指定的区域
 * \param image_rect_ptr 图片中指定的区域
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-12 11:49:10
 */
GBIMEGLOBAL void PT_DrawImageWithClip(GBU16 imageID, GBRECT *display_rect_ptr, GBRECT *image_rect_ptr);

/*!
 * \brief 填充矩形区域
 * \param left 
 * \param top 
 * \param right 
 * \param bottom 
 * \param clr 
 * \return 
 */
GBIMEGLOBAL void PT_FillRect(GBI16 left, GBI16 top, GBI16 right, GBI16 bottom, PT_COLOR clr);

/*!
 * \brief 画矩形区域
 * \param left 
 * \param top 
 * \param right 
 * \param bottom 
 * \param clr 
 * \return 
 */
GBIMEGLOBAL void PT_DrawRect(GBI16 left, GBI16 top, GBI16 right, GBI16 bottom, PT_COLOR clr);

/*!
 * \brief 画点
 * \param x 
 * \param y 
 * \param clr 
 * \return 
 */
GBIMEGLOBAL void PT_DrawPoint(GBU16 x, GBU16 y,  PT_COLOR clr);

/*!
 * \brief 画一条直线
 * \param x1 
 * \param y1 
 * \param x2 
 * \param y2 
 * \param clr 
 * \return 
 */
GBIMEGLOBAL void PT_DrawLine(GBU16 x1, GBU16 y1, GBU16 x2, GBU16 y2, PT_COLOR clr);

/*!
 * \brief 画一条水平线
 * \param x1 
 * \param y1 
 * \param x2 
 * \param y2 
 * \param clr 
 * \return 
 */
void PT_DrawHorizontalLine(GBU16 x1, GBU16 x2, GBU16 y, PT_COLOR clr);
/*!
 * \brief 画一条垂直线
 * \param x1 
 * \param y1 
 * \param x2 
 * \param y2 
 * \param clr 
 * \return 
 */
void PT_DrawVerticalLine(GBU16 y1, GBU16 y2, GBU16 x, PT_COLOR clr);

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
void PT_DrawVerticalLineType(GBU16 y1, GBU16 y2, GBU16 x, PT_COLOR clr, PT_LINT_TYPE lineType);

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
void PT_DrawThickLine(GBU16 x1, GBU16 y1, GBU16 x2, GBU16 y2, PT_COLOR clr, GBU16 widthInPixels);

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
						 GBU16 widthInPixels );

/*!
 * \defgroup 工具函数
 */

 /*!
 * \brief 判断一个点坐标是否落在指定矩形区域内
 * \param point 
 * \param rect 
 * \return 
 */
//GBIMEGLOBAL GBBOOL PT_PointIsInRect(GBIMEPoint point, GBRECT rect);
#define PT_PointIsInRect(pt, rect)  (pt.x >= rect.left && pt.x <= rect.right \
									&& pt.y >= rect.top && pt.y <= rect.bottom)


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
void PT_Invalidate(GBINT left, GBINT top, GBINT right, GBINT bottom);

/*!
* \brief 清屏
* \param  void
* \return void
*/
void PT_ClearScreen(void);

/*!
 * \brief 开启/禁用平台手写功能
 */
#define PT_DisableHandwriting	mmi_imc_disable_handwriting
#define PT_EnableHandwriting	mmi_imc_enable_handwriting

/*!
* \brief get string for given id 获取字符串
* \param GBU16
* \return GBPCU16
*/
GBPCU16 PT_GetString(GBU16 id);

/*!
* \brief 获取字符串长度
* \param GBPCU16
* \return GBINT
*/
GBINT PT_GetStrLen(GBWCHAR * text);

/*!
* \brief 获得字符高度
* \param GBPCU16
* \return GBINT
*/
GBINT PT_GetStrHeight(GBLPCWCHAR text);

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
GBINT PT_GetStrWidthEx(GBLPCWCHAR text, PT_FontSize font);

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
GBINT PT_GetStrHeightEx(GBLPCWCHAR text, PT_FontSize font);

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
 * \date 2010-10-12 18:44:50
*/
void PT_MeasureCharacter(GBWCHAR c, GBINT *pWidth,GBINT *pHeight, PT_FontSize font);

/*!
* \brief 获取图片id
* \param GBU16
* \return GBPUACHAR
*/
GBPUACHAR PT_GetImage(GBU16 i);

/*!
* \brief 获取图片宽和高
* \param GBPWCHAR
* \param GBPINT
* \param GBPINT
* \return void
*/
void PT_MeasureImage(GBPWCHAR i,GBPINT width,GBPINT height);

/*!
* \brief 显示图片
* \param GBINT,GBINT,GBUINT8
* \return void
*/
void PT_ShowImage(GBINT x,GBINT y, GBUINT8 *i);

/*!
* \brief 获取该编辑栏长度
* \param void
* \return GBINT
*/
GBINT PT_GetSingleLineInputBoxTextLength(void);

/*!
* \brief 获取输入栏文本长度
* \param void
* \return GBUINT16
*/
GBUINT16 PT_GetEMSInputBoxTextLen(void);

/*!
* \brief 设置文本输出的开始坐标
* \param GBINT
* \param GBINT
* \return void
*/
void PT_MoveTextCursor(GBINT x,GBINT y);

/*!
* \brief 输出长度为N的文本
* \param GBPUSHORT
* \param GBI
* \return void
*/
void PT_PrintTextN(GBPUSHORT text,GBI n);

/*!
* \brief 设置文本颜色
* \param GBINT
* \return void
*/
void PT_SetTextColor(PT_COLOR clr);

/*!
* \brief 设置字体
* \param PT_FontSize
* \return void
*/
void PT_SetFont(PT_FontSize font);

/*!
 * \brief 检测文件夹是否存在，如果不存在则创建之
 * \param
 * \return
 * \note 
 * \author weizhiping
 * \date 2010年2月21日
 */
GBBOOL PT_CheckFolder(GBPCWCHAR pFoldPath);

/*!
* \brief 锁层
* \param void
* \return void
*/
void PT_LayerLock(void);

/*!
* \brief 解锁层
* \param void
* \return void
*/
void PT_LayerUnLock(void);

/*!
* \brief 文本剪裁区域压栈
* \param GBINT x
* \param GBINT y
* \param GBINT width
* \param GBINT height
* \return void
*/
void PT_PushClip(GBINT x,GBINT y,GBINT width,GBINT height);

/*!
* \brief 文本剪裁区域出栈
* \param void
* \return void
*/
void PT_PopClip(void);

/*!
* \brief 获取当前编辑状态
* \param void
* \return GBINT
*/
GBINT PT_GetCurrEditorStyle(void);

/*!
* \brief 获取当前活动的屏幕ID
* \param void
* \return GBINT
*/
GBINT PT_GetActiveScreenId(void);

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
GBIMEGLOBAL void PT_CreateNewScreen(GBUINT16 scrnid,
						GBUINT16 menu_item_id,
						GBUINT8 *guiBuffer,
						GBUINT16 *nNumofItem,
						GBUINT16 *nDispAttribute,
						GBUINT16 *nStrItemList,
						GBUINT8 **ItemType);

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
 * \param string_submenu		[IN]        SubMenu of strings
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
        GBUINT8 *history_buffer);

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
		GBUINT8	 *history_buffer);

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
		  GBUINT8 *history_buffer);

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
								GBU8 *history_buffer);

/*!
* \brief 退出只读文本屏幕
 * \return void
*/
void PT_ExitReadOnlyScreen(void);

/*!
 * \brief 设置左软键对应的回调函数
 *  SetLeftSoftkeyFunction
 * DESCRIPTION
 *  Sets the function that is called when a particular softkey event occurs
 * \param f [IN]        Is the function that is called
 * \param k [IN]        Is the key event (up, down...)
 * \return void
*/
void PT_SetLeftSoftkeyFunction(void (*f) (void), MMI_key_event_type k);

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
void PT_SetRightSoftkeyFunction(void (*f) (void), MMI_key_event_type k);

/*!
* \brief 注册高亮函数
 *  RegisterHighlightHandler
 * DESCRIPTION
 *  Registers the highlight handler to be called in menu screens
 * \param  f           [IN]        Is a pointer to a highlight handler function
 * \param  This(?)     [IN]        Is a zero based index of the highlighted menuitem.
 * \return void
*/
void PT_RegisterHighlightHandler(void (*f) (GBINT item_index));

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
void PT_SetKeyHandler(FuncPtr funcPtr,GBU16 keyCode,GBU16 keyType);

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
void PT_SetCheckboxToggleRightSoftkeyFunctions(void (*done_function) (void), void (*back_function) (void));

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
void PT_DisplayPopupMessageBox(GBU16 nMessageStringID, GBU32 popupDuration);

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
void PT_DisplayConfirmMessageBox(void (*pfLeftSoftkeyCallback) (void), void (*pfRightSoftkeyCallback) (void));

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
								PT_TextAlign textAlign);	// 文本对齐

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
GBINT PT_TouchRegionHitTest(PGBIMETouchRegion pTouchRegion, GBIMEPoint hitPoint);

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
GBU16 PT_RightAlignDrawString(GBINT x, GBINT y, GBINT limitWidth, GBPCWCHAR drawString, PT_FontSize font, PT_COLOR clr);

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
GBU16 PT_RightAlignDrawStrokeString(GBINT x, GBINT y, GBINT limitWidth, GBPCWCHAR drawString, PT_FontSize font, PT_COLOR clr);

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
GBBOOL PT_ConvertSymbolString(GBPWCHAR pSymbolString, const GBWCHAR ** symbolTable, GBINT symbolNum);

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
GBBOOL PT_ConvertFaceString(GBPWCHAR pFaceString, const GBWCHAR ** faceTable, GBINT faceNum);

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
GBINT PT_GetPointDistance(GBINT disX, GBINT disY);

/*!
 * \brief WideChar到MultiByte之间的转换
 * \param mbs MultiByte
 * \param wcs WideChar
 * \param mbslen MultiByte长度
 * \autor lidasheng
 * \return 
 */
GBINT PT_wcstombs(GBPACHAR mbs, GBPCWCHAR wcs, GBINT mbslen);

/*!
* \brief MultiByte到WideChar之间的转换
* \param mbs MultiByte
* \param wcs WideChar
* \param wcslen WideChar长度
* \autor zhaokun
* \return 
*/
GBINT PT_mbstowcs(unsigned short *wcs, char *mbs,int wcslen);

/*!
 * \brief 使能可视化显示空格、回车等符号功能
 * \param void 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-10-28 17:22:49
 */
void PT_EnableProprietarySymbolDisplay(void);

/*!
 * \brief 禁用可视化显示空格、回车等符号功能
 * \param void 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-10-28 17:22:49
 */
void PT_DisableProprietarySymbolDisplay(void);

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
void PT_ProprietarySymbolSwitch(GBPCWCHAR pOldStr, GBPWCHAR pNewStr);

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
GBINT PT_GetComponentStringWidth(GBPCWCHAR pComponentString, PT_FontSize font);

#if __GBIME_LOG_TO_FILE__ > 0
GBINT PT_OpenFile(char* filePath);
void PT_CloseFile();
#endif
#endif //__PLATFORM_TOOLS_H_