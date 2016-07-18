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
* Purpose      :  ��װMTKƽ̨����Ļģ��������ַ��������ڴ�������ļ����ʡ�������Ϣ�ص�ע��/ע�����ַ�����ʾ�ȹ��ߺ���, ����ֱ������ƽ̨����
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
	#define __GBIME_LOG__			1 // ���뷨���Log��ӡ����
	//#define __GBIME_LOG_TO_FILE__	1 // ���뷨���Logд���ļ�
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

// ƽ̨�޹������С����
typedef enum
{
    PT_FONT_SIZE_DEFAULT,	//Ĭ������
	PT_FONT_SIZE_SMALL,		//С������ 
	PT_FONT_SIZE_MEDIUM,	//�к�����
	PT_FONT_SIZE_LARGE,		//�������
	PT_FONT_SIZE_TOTAL
} PT_FontSize;

// �ı����붨��
typedef enum
{
    PT_TEXT_ALIGN_LEFT		//����
	,PT_TEXT_ALIGN_CENTER	//����
	,PT_TEXT_ALIGN_RIGHT	//����
} PT_TextAlign;

// ������ɫ����
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
	#define PT_COLOR_BLUE			gui_color(0, 0, 200)// 255) ������ɫ
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

// �������Ͷ���
typedef enum {	
	PT_LINT_TYPE_SOLID
	,PT_LINT_TYPE_DOTTED
	,PT_LINT_TYPE_DASHED
}PT_LINT_TYPE;

/*!
 * \ ����Logģ��ö�٣�ʵ�ַ�ģ��Log��ӡ����
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

#define DBG_LVL ALL_LVL // �˴�������Ҫ��ע��log������Ϣ

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
#else// �ر�Debug��Ϣ
	#define PT_PrintLogInfo(lvl, debug_info)
#endif // __GBIME_LOG__


// ����������
#ifndef PT_ARRAY_SIZE
#define PT_ARRAY_SIZE(a)				(sizeof(a) / sizeof((a)[0])) 	// ��ȡ����Ԫ����Ŀ
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

// �ַ����������
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
	// �ڴ����
	#define PT_Malloc               OslMalloc
	#define PT_Free                 OslMfree	
	// ����
	#define PT_Assert(expr)			MMI_ASSERT(expr)
	// �ļ�����
	#define GB_SYSTEM_DIR      		L"c:\\@gb\\" // ����ϵͳ�ļ���
	#define PT_FILE_HANDLE          FS_HANDLE
	#define PT_FS_READ           	FS_READ_ONLY
	#define PT_FS_READ_WRITE		FS_READ_WRITE | FS_CREATE
	#define PT_FS_WRITE          	FS_READ_WRITE|FS_CREATE_ALWAYS
	#define PT_FS_Open(x,y)        	FS_Open((U16 *)x,y)
	#define PT_FS_Close(x)         	{ FS_Commit(x); FS_Close(x); }
	#define PT_FS_Write(FileHandle, DataPtr, Length, Written) FS_Write(FileHandle,DataPtr,Length,(UINT *)Written)
	#define PT_FS_Read(FileHandle,DataPtr,Length,Read)  FS_Read(FileHandle,DataPtr,Length,(UINT *)Read) 
	#define PT_FS_Seek(FileHandle, Offset, Whence)	FS_Seek(FileHandle, Offset, Whence)

	// ��ӡ
	//#define PT_Print				kal_print
	#define PT_wsprintf				kal_wsprintf
	#define PT_sprintf				kal_sprintf
	#define PT_COLOR				color
#else //Win32
	#include <Windows.h>
	// �ڴ����
	#define PT_Malloc               malloc
	#define PT_Free                 free
	// ����
	#define PT_Assert(expr)			assert(expr)
	// �ļ�����
	#define PT_FILE_HANDLE          FILE *
	#define PT_FS_READ           	L"r"
	#define PT_FS_READ_WRITE		L"rw"
	#define PT_FS_WRITE          	L"w"
	#define PT_FS_Open(x,y)        	_wfopen((unsigned short *)x,y)
	#define PT_FS_Close(x)         	{ fflush(x); fclose(x); }
	#define PT_FS_Write(FileHandle, DataPtr, Length, Written) fwrite(DataPtr,Length,0,FileHandle)
	#define PT_FS_Read(FileHandle,DataPtr,Length,Read)  fread(DataPtr,1,Length,FileHandle)
	#define PT_FS_Seek(FileHandle, Offset, Whence)	fseek(FileHandle, Offset, Whence)

	// ��ӡ
	#define PT_wsprintf				wsprintf
	#define PT_sprintf				sprintf

	#define PT_COLOR				void*
		
	#define UI_DEVICE_WIDTH			320
	#define UI_DEVICE_HEIGHT		480

	#define MMI_key_event_type	GBINT
	typedef void (*FuncPtr) (void);

#endif // __MTK__

/*!
 * \brief �����������
 */
typedef struct _tagGBTouchRegion {
		GBRECT		candidateRectArray[GBIME_DEF_MAX_CAND_WORD * 2];	///<��ѡ��Ӧ�ľ���������
		GBIMEPoint  candidatePosArray[GBIME_DEF_MAX_CAND_WORD * 2];		///<��ѡ�ַ�����Ӧ�Ļ�������λ������
		GBINT		candidateNum;
		GBUINT16	nHorizontalSpacing;
		GBUINT16	nVerticalSpacing;
} GBIMETouchRegion, * PGBIMETouchRegion;

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
void PT_Print(const char * fmt, ...);

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
void PT_PrintW(const wchar_t * fmt, ...);

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
// void PT_UnicodeToAnsi(GBPCWCHAR pSrcUniString, GBUINT8 *pDesAnsiString);

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
void PT_StartTimer(GBU16 timerid, GBU32 delay, TIMER_CALLBACK funcPtr);

/*!
 * \brief ֹͣƽ̨��ʱ��
 * \param timerid 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-8-17 11:08:31
*/
void PT_StopTimer(GBU16 timerid);

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
void PT_GUIStartTimer(GBINT delay, TIMER_CALLBACK callback);

/*!
 * \brief ƽ̨GUI���׶�ʱ��ȡ������(������ʱ��ID)
 * \param callback 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-10-8 17:10:30
*/
void PT_GUICancelTimer(TIMER_CALLBACK callback);

/*!
 * \brief ��ȡϵͳ���������ڵĺ�����
 * \param void 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-24 9:20:38
*/
GBU32 PT_GetCurrentTime(void);

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
int PT_GetStrWidth(void * pParam, GBLPCWCHAR str);

/*!
 * \brief ��ȡƽ̨LCD���
 * \param void 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-21 11:14:07
 */
GBIMEGLOBAL GBINT PT_GetLcdWidth(void);

/*!
 * \brief ��ȡƽ̨LCD�߶�
 * \param void 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-21 11:13:49
 */
GBIMEGLOBAL GBINT PT_GetLcdHeight(void);

/*!
 * \defgroup ͼƬ��ʾAPI
 */

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
GBIMEGLOBAL void PT_PrintText(GBINT x, GBINT y,  GBPCWCHAR pText);

/*!
 * \brief �����ַ���
 * \param str 
 * \param font
 * \param clr 
 * \param left 
 * \param top 
 * \return 
 */
GBIMEGLOBAL void PT_DrawString( GBPCWCHAR str, PT_FontSize font, PT_COLOR clr, GBINT left, GBINT top);

/*!
 * \brief �ʻ����ƺ���(����С�ֺ��޷���ʾƫ�ԡ��^����С""�񡰵Ĵ���)
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
GBIMEGLOBAL void PT_DrawImage(GBINT iLeftX, GBINT iTopY, GBU16 imageID);

/*!
 * \brief ��ͼƬ��ĳһָ����������ʾ����Ļ��ָ��������
 * \param imageID ͼƬID
 * \param display_rect_ptr ��Ļ��ָ��������
 * \param image_rect_ptr ͼƬ��ָ��������
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-7-12 11:49:10
 */
GBIMEGLOBAL void PT_DrawImageWithClip(GBU16 imageID, GBRECT *display_rect_ptr, GBRECT *image_rect_ptr);

/*!
 * \brief ����������
 * \param left 
 * \param top 
 * \param right 
 * \param bottom 
 * \param clr 
 * \return 
 */
GBIMEGLOBAL void PT_FillRect(GBI16 left, GBI16 top, GBI16 right, GBI16 bottom, PT_COLOR clr);

/*!
 * \brief ����������
 * \param left 
 * \param top 
 * \param right 
 * \param bottom 
 * \param clr 
 * \return 
 */
GBIMEGLOBAL void PT_DrawRect(GBI16 left, GBI16 top, GBI16 right, GBI16 bottom, PT_COLOR clr);

/*!
 * \brief ����
 * \param x 
 * \param y 
 * \param clr 
 * \return 
 */
GBIMEGLOBAL void PT_DrawPoint(GBU16 x, GBU16 y,  PT_COLOR clr);

/*!
 * \brief ��һ��ֱ��
 * \param x1 
 * \param y1 
 * \param x2 
 * \param y2 
 * \param clr 
 * \return 
 */
GBIMEGLOBAL void PT_DrawLine(GBU16 x1, GBU16 y1, GBU16 x2, GBU16 y2, PT_COLOR clr);

/*!
 * \brief ��һ��ˮƽ��
 * \param x1 
 * \param y1 
 * \param x2 
 * \param y2 
 * \param clr 
 * \return 
 */
void PT_DrawHorizontalLine(GBU16 x1, GBU16 x2, GBU16 y, PT_COLOR clr);
/*!
 * \brief ��һ����ֱ��
 * \param x1 
 * \param y1 
 * \param x2 
 * \param y2 
 * \param clr 
 * \return 
 */
void PT_DrawVerticalLine(GBU16 y1, GBU16 y2, GBU16 x, PT_COLOR clr);

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
void PT_DrawVerticalLineType(GBU16 y1, GBU16 y2, GBU16 x, PT_COLOR clr, PT_LINT_TYPE lineType);

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
void PT_DrawThickLine(GBU16 x1, GBU16 y1, GBU16 x2, GBU16 y2, PT_COLOR clr, GBU16 widthInPixels);

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
						 GBU16 widthInPixels );

/*!
 * \defgroup ���ߺ���
 */

 /*!
 * \brief �ж�һ���������Ƿ�����ָ������������
 * \param point 
 * \param rect 
 * \return 
 */
//GBIMEGLOBAL GBBOOL PT_PointIsInRect(GBIMEPoint point, GBRECT rect);
#define PT_PointIsInRect(pt, rect)  (pt.x >= rect.left && pt.x <= rect.right \
									&& pt.y >= rect.top && pt.y <= rect.bottom)


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
void PT_Invalidate(GBINT left, GBINT top, GBINT right, GBINT bottom);

/*!
* \brief ����
* \param  void
* \return void
*/
void PT_ClearScreen(void);

/*!
 * \brief ����/����ƽ̨��д����
 */
#define PT_DisableHandwriting	mmi_imc_disable_handwriting
#define PT_EnableHandwriting	mmi_imc_enable_handwriting

/*!
* \brief get string for given id ��ȡ�ַ���
* \param GBU16
* \return GBPCU16
*/
GBPCU16 PT_GetString(GBU16 id);

/*!
* \brief ��ȡ�ַ�������
* \param GBPCU16
* \return GBINT
*/
GBINT PT_GetStrLen(GBWCHAR * text);

/*!
* \brief ����ַ��߶�
* \param GBPCU16
* \return GBINT
*/
GBINT PT_GetStrHeight(GBLPCWCHAR text);

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
GBINT PT_GetStrWidthEx(GBLPCWCHAR text, PT_FontSize font);

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
GBINT PT_GetStrHeightEx(GBLPCWCHAR text, PT_FontSize font);

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
 * \date 2010-10-12 18:44:50
*/
void PT_MeasureCharacter(GBWCHAR c, GBINT *pWidth,GBINT *pHeight, PT_FontSize font);

/*!
* \brief ��ȡͼƬid
* \param GBU16
* \return GBPUACHAR
*/
GBPUACHAR PT_GetImage(GBU16 i);

/*!
* \brief ��ȡͼƬ��͸�
* \param GBPWCHAR
* \param GBPINT
* \param GBPINT
* \return void
*/
void PT_MeasureImage(GBPWCHAR i,GBPINT width,GBPINT height);

/*!
* \brief ��ʾͼƬ
* \param GBINT,GBINT,GBUINT8
* \return void
*/
void PT_ShowImage(GBINT x,GBINT y, GBUINT8 *i);

/*!
* \brief ��ȡ�ñ༭������
* \param void
* \return GBINT
*/
GBINT PT_GetSingleLineInputBoxTextLength(void);

/*!
* \brief ��ȡ�������ı�����
* \param void
* \return GBUINT16
*/
GBUINT16 PT_GetEMSInputBoxTextLen(void);

/*!
* \brief �����ı�����Ŀ�ʼ����
* \param GBINT
* \param GBINT
* \return void
*/
void PT_MoveTextCursor(GBINT x,GBINT y);

/*!
* \brief �������ΪN���ı�
* \param GBPUSHORT
* \param GBI
* \return void
*/
void PT_PrintTextN(GBPUSHORT text,GBI n);

/*!
* \brief �����ı���ɫ
* \param GBINT
* \return void
*/
void PT_SetTextColor(PT_COLOR clr);

/*!
* \brief ��������
* \param PT_FontSize
* \return void
*/
void PT_SetFont(PT_FontSize font);

/*!
 * \brief ����ļ����Ƿ���ڣ�����������򴴽�֮
 * \param
 * \return
 * \note 
 * \author weizhiping
 * \date 2010��2��21��
 */
GBBOOL PT_CheckFolder(GBPCWCHAR pFoldPath);

/*!
* \brief ����
* \param void
* \return void
*/
void PT_LayerLock(void);

/*!
* \brief ������
* \param void
* \return void
*/
void PT_LayerUnLock(void);

/*!
* \brief �ı���������ѹջ
* \param GBINT x
* \param GBINT y
* \param GBINT width
* \param GBINT height
* \return void
*/
void PT_PushClip(GBINT x,GBINT y,GBINT width,GBINT height);

/*!
* \brief �ı����������ջ
* \param void
* \return void
*/
void PT_PopClip(void);

/*!
* \brief ��ȡ��ǰ�༭״̬
* \param void
* \return GBINT
*/
GBINT PT_GetCurrEditorStyle(void);

/*!
* \brief ��ȡ��ǰ�����ĻID
* \param void
* \return GBINT
*/
GBINT PT_GetActiveScreenId(void);

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
GBIMEGLOBAL void PT_CreateNewScreen(GBUINT16 scrnid,
						GBUINT16 menu_item_id,
						GBUINT8 *guiBuffer,
						GBUINT16 *nNumofItem,
						GBUINT16 *nDispAttribute,
						GBUINT16 *nStrItemList,
						GBUINT8 **ItemType);

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
		GBUINT8	 *history_buffer);

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
		  GBUINT8 *history_buffer);

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
								GBU8 *history_buffer);

/*!
* \brief �˳�ֻ���ı���Ļ
 * \return void
*/
void PT_ExitReadOnlyScreen(void);

/*!
 * \brief �����������Ӧ�Ļص�����
 *  SetLeftSoftkeyFunction
 * DESCRIPTION
 *  Sets the function that is called when a particular softkey event occurs
 * \param f [IN]        Is the function that is called
 * \param k [IN]        Is the key event (up, down...)
 * \return void
*/
void PT_SetLeftSoftkeyFunction(void (*f) (void), MMI_key_event_type k);

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
void PT_SetRightSoftkeyFunction(void (*f) (void), MMI_key_event_type k);

/*!
* \brief ע���������
 *  RegisterHighlightHandler
 * DESCRIPTION
 *  Registers the highlight handler to be called in menu screens
 * \param  f           [IN]        Is a pointer to a highlight handler function
 * \param  This(?)     [IN]        Is a zero based index of the highlighted menuitem.
 * \return void
*/
void PT_RegisterHighlightHandler(void (*f) (GBINT item_index));

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
void PT_SetKeyHandler(FuncPtr funcPtr,GBU16 keyCode,GBU16 keyType);

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
void PT_SetCheckboxToggleRightSoftkeyFunctions(void (*done_function) (void), void (*back_function) (void));

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
void PT_DisplayPopupMessageBox(GBU16 nMessageStringID, GBU32 popupDuration);

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
void PT_DisplayConfirmMessageBox(void (*pfLeftSoftkeyCallback) (void), void (*pfRightSoftkeyCallback) (void));

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
								PT_TextAlign textAlign);	// �ı�����

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
GBINT PT_TouchRegionHitTest(PGBIMETouchRegion pTouchRegion, GBIMEPoint hitPoint);

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
GBU16 PT_RightAlignDrawString(GBINT x, GBINT y, GBINT limitWidth, GBPCWCHAR drawString, PT_FontSize font, PT_COLOR clr);

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
GBU16 PT_RightAlignDrawStrokeString(GBINT x, GBINT y, GBINT limitWidth, GBPCWCHAR drawString, PT_FontSize font, PT_COLOR clr);

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
GBBOOL PT_ConvertSymbolString(GBPWCHAR pSymbolString, const GBWCHAR ** symbolTable, GBINT symbolNum);

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
GBBOOL PT_ConvertFaceString(GBPWCHAR pFaceString, const GBWCHAR ** faceTable, GBINT faceNum);

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
GBINT PT_GetPointDistance(GBINT disX, GBINT disY);

/*!
 * \brief WideChar��MultiByte֮���ת��
 * \param mbs MultiByte
 * \param wcs WideChar
 * \param mbslen MultiByte����
 * \autor lidasheng
 * \return 
 */
GBINT PT_wcstombs(GBPACHAR mbs, GBPCWCHAR wcs, GBINT mbslen);

/*!
* \brief MultiByte��WideChar֮���ת��
* \param mbs MultiByte
* \param wcs WideChar
* \param wcslen WideChar����
* \autor zhaokun
* \return 
*/
GBINT PT_mbstowcs(unsigned short *wcs, char *mbs,int wcslen);

/*!
 * \brief ʹ�ܿ��ӻ���ʾ�ո񡢻س��ȷ��Ź���
 * \param void 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-10-28 17:22:49
 */
void PT_EnableProprietarySymbolDisplay(void);

/*!
 * \brief ���ÿ��ӻ���ʾ�ո񡢻س��ȷ��Ź���
 * \param void 
 * \return 
 * \note 
 *\example
 * \author weizhiping
 * \date 2010-10-28 17:22:49
 */
void PT_DisableProprietarySymbolDisplay(void);

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
void PT_ProprietarySymbolSwitch(GBPCWCHAR pOldStr, GBPWCHAR pNewStr);

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
GBINT PT_GetComponentStringWidth(GBPCWCHAR pComponentString, PT_FontSize font);

#if __GBIME_LOG_TO_FILE__ > 0
GBINT PT_OpenFile(char* filePath);
void PT_CloseFile();
#endif
#endif //__PLATFORM_TOOLS_H_