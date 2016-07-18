/***************************************************************************
* File Name    :  GBIMEDef.h
* Corporation  :  Guobi Technology Ltd. GuangZhou China
* Copyright    :  Copyright Guobi Technology Ltd. And Subject to Confidentiality 
*                 Restrictions. This document is the confidential and proprietary 
*                 property of Guobi Technology Ltd.It is neither the document                  
*                 itself nor any part of it can be shared with any party  without 
*                 the written  permission. 
* Author       :  weizhiping
* Date         :  2010-06-08
* Purpose      :  输入法框架外部常量定义
* Version      :  V1.0
* Last Changed List
* Name         |   date   |  Discription
****************************************************************************/
#ifndef __GBIME_DEF_H_
#define __GBIME_DEF_H_

#include "gbtype.h"
#include "Gbwchar.h"
#include "gbdef.h"

#define GBIME_CFG_STATIC_DB					1	// 使用静态数据
#define GBIME_CFG_UDB_SUPPORT				1	// UDB自造词数据模块
//#define GBIME_CFG_HANDWRITE_SUPPORT			1	// 手写支持(改成平台宏控制)
#define GBIME_CFG_CHS_SUPPORT   			1   // 中文支持
#define GBIME_CFG_EN_SUPPORT    			1   // 英文支持
#define GBIME_CFG_SLIDING_SWITCH_SUPPORT	1   // 滑屏切换开关

#ifndef GB_MICRO_OPTIMIZATION
#define GB_MICRO_OPTIMIZATION       1
#endif

#ifndef GBIMEAPI
#ifdef GBIMPORT
#define GBIMEAPI  __declspec(dllimport)
#else
#define GBIMEAPI	extern
#endif
#endif

/*!
 * \brief 编译单元内部使用的函数
 */
#define GBIMELOCAL		static
/*!
 * \brief 全局使用，但不对外公布的函数
 */
#define GBIMEGLOBAL 
/*!
 * \brief 对外公布的函数
 */
#define GBIMEEXPORT 

#define GBIME_MAX_DB_COUNT						50	///<最大语言数据个数
#define GBIME_DEF_MAX_CAND_WORD				    9	///<默认最大候选词个数 < GB_MAX_CANDIDATES
#define GBIME_DEF_CHN_MAX_CAND_WORD				9	///<默认最大候选词个数 < GB_MAX_CANDIDATES
#define GBIME_DEF_SYLLABLE_MAX_CAND_WORD		9	///<默认最大候选词个数 < GB_MAX_CANDIDATES
#define GBIME_DEF_ENGINSH_MAX_CAND_WORD			9	///<默认最大候选词个数 < GB_MAX_CANDIDATES
	
///<输入法引擎参数配置(重要)
#ifdef GBIME_CFG_UDB_SUPPORT
	#define GBIME_UDB_UPDATE_FREQ		 				20 ///<更新频率，数据的修改次数大于这个值得时候，引擎就会调用fpUpdateFunction，一般来说这个值建议为10
#endif/*GBIME_CFG_UDB_SUPPORT*/
#define GBIME_FILENAME_LEN 						256

/*!
 * \brief GB功能配置选项宏定义
 */
#define GBIME_INPUT_CAND_ROW_COUNT	 			1 ///<必须小于 <= MAX_CANDIDATES / GB_CN_CANDIDATE_COUNT
#define GBIME_ASSOCIATE_CAND_ROW_COUNT			1 ///<必须小于 <= MAX_CANDIDATES / GB_CN_CANDIDATE_COUNT
#define GBIME_SWITCH_KEY						GBKEY_AMB_SPECIAL_B ///<GB无缝切换按键
#define GBIME_INTERPUNCTION_KEY					GBKEY_AMB_SPECIAL_A ///<GB标点符号按键
#define GBIME_EMBEDDED_INTERPUNCTION_KEY		GBKEY_1 ///<智能标点符号按键
#define GBIME_INTERPUNCTION_CAND_DESIRE_ROW		2 ///<GB标点符号键候选行数
#define GBIME_INTERPUNCTION_CAND_COUNT_PER_ROW	(GBIME_DEF_MAX_CAND_WORD - 1) ///<GB标点符号键每行候选个数

/*!
 * \brief 快捷标点符号、表情符号相关常量
 */
#define GBIME_SYMBOL_MAX_LEN					3
#define GBIME_FACE_MAX_LEN						8
#define GBIME_SPLIT_CHAR						'\t' // 表情分隔串
#define GBIME_SPLIT_STR							L"\t" // 表情分隔串(方便组合成表情符号串)

/*!
 * \brief UI相关常量
 */
#if defined(__MMI_MAINLCD_240X320__) || defined(__MMI_MAINLCD_240X400__)
	#define GBIME_VERTICAL_SPACING 					8  ///<垂直方向显示间隔(动态计算字符高度垂直居中无效替代方案)
	#define GBIME_SYLLABLE_SPACING 					8  ///<音节栏字间隔
	#define GBIME_CANDIDATE_SPACING 				8  ///<候选栏字间隔
	#define GBIME_ARROW_WIDTH						25
	#define GBIME_CAND_AREA_WIDTH  					(PT_GetLcdWidth() - GBIME_ARROW_WIDTH * 2) ///<国笔候选框的宽度
	#define GBIME_SYLLABLE_DISPLAY_START_X			GBIME_ARROW_WIDTH  // 拼音显示的开始位置X坐标
	#define GBIME_SYLLABLE_LINE_WIDTH				GBIME_CAND_AREA_WIDTH // 拼音栏宽度
	#define GBIME_SYLLABLE_LINE_HEIGHT				21  // 拼音栏高度
	#define GBIME_CANDIDATE_DISPLAY_START_X			GBIME_ARROW_WIDTH  // 候选显示的开始位置的X坐标
	#define GBIME_CANDIDATE_LINE_WIDTH				GBIME_CAND_AREA_WIDTH	// 候选栏宽度
	#define GBIME_CANDIDATE_LINE_HEIGHT				23  // 候选栏高度
	#define GBIME_SYLLABLE_FONT_SIZE				PT_FONT_SIZE_SMALL // 音节字体大小
	#define GBIME_SYLLABLE_NORMAL_FONT_COLOR		PT_COLOR_BLACK  // 没有选择中的候选拼音颜色
	#define GBIME_SYLLABLE_SELECTED_FONT_COLOR		PT_COLOR_BLUE    // 选择中的的拼音的颜色
	#define GBIME_CANDIDATE_FONT_SIZE				PT_FONT_SIZE_SMALL // 候选字体大小
	#define GBIME_CANDIDATE_NORMAL_FONT_COLOR		PT_COLOR_BLACK  // 没有选择中的候选汉字颜色
	#define GBIME_CANDIDATE_SELECTED_FONT_COLOR		PT_COLOR_BLUE    // 选择中的候选汉字颜色
	#define GBIME_STROKE_FONT_SIZE					PT_FONT_SIZE_SMALL // 笔划字体大小
	#define GBIME_STROKE_COLOR						PT_COLOR_BLACK    // 笔划的颜色
	#define GBIME_COMPONENT_FONT_SIZE				PT_FONT_SIZE_SMALL // 快捷部首字体大小
	#define GBIME_COMPONENT_COLOR					PT_COLOR_BLACK  // 快捷部首的颜色
	#define GBIME_ENGLISH_INPUTSTRING_COLOR			PT_COLOR_BLUE	// 9键英文输入的数字串的颜色
	#define GBIME_OUTPUT_STRING_COLOR				PT_COLOR_BLUE	// 输出串字体颜色
	#define GBIME_COMP_WIDTH						14 ///<国笔部首的宽度
	#define GBIME_COMP_HEIGHT						14 ///<国笔部首的高度
#elif defined(__MMI_MAINLCD_176X220__) //LCD 176宽x220高
	#define GBIME_VERTICAL_SPACING 					8  ///<垂直方向显示间隔(动态计算字符高度垂直居中无效替代方案)
	#define GBIME_SYLLABLE_SPACING 					8  ///<音节栏字间隔
	#define GBIME_CANDIDATE_SPACING 				8  ///<候选栏字间隔
	#define GBIME_ARROW_WIDTH						25
	#define GBIME_CAND_AREA_WIDTH  					(PT_GetLcdWidth() - GBIME_ARROW_WIDTH * 2) ///<国笔候选框的宽度
	#define GBIME_SYLLABLE_DISPLAY_START_X			GBIME_ARROW_WIDTH  // 拼音显示的开始位置X坐标
	#define GBIME_SYLLABLE_LINE_WIDTH				GBIME_CAND_AREA_WIDTH // 拼音栏宽度
	#define GBIME_SYLLABLE_LINE_HEIGHT				21  // 拼音栏高度
	#define GBIME_CANDIDATE_DISPLAY_START_X			GBIME_ARROW_WIDTH  // 候选显示的开始位置的X坐标
	#define GBIME_CANDIDATE_LINE_WIDTH				GBIME_CAND_AREA_WIDTH	// 候选栏宽度
	#define GBIME_CANDIDATE_LINE_HEIGHT				23  // 候选栏高度
	#define GBIME_SYLLABLE_FONT_SIZE				PT_FONT_SIZE_SMALL // 音节字体大小
	#define GBIME_SYLLABLE_NORMAL_FONT_COLOR		PT_COLOR_BLACK  // 没有选择中的候选拼音颜色
	#define GBIME_SYLLABLE_SELECTED_FONT_COLOR		PT_COLOR_BLUE    // 选择中的的拼音的颜色
	#define GBIME_CANDIDATE_FONT_SIZE				PT_FONT_SIZE_SMALL // 候选字体大小
	#define GBIME_CANDIDATE_NORMAL_FONT_COLOR		PT_COLOR_BLACK  // 没有选择中的候选汉字颜色
	#define GBIME_CANDIDATE_SELECTED_FONT_COLOR		PT_COLOR_BLUE    // 选择中的候选汉字颜色
	#define GBIME_STROKE_FONT_SIZE					PT_FONT_SIZE_SMALL // 笔划字体大小
	#define GBIME_STROKE_COLOR						PT_COLOR_BLACK    // 笔划的颜色
	#define GBIME_COMPONENT_FONT_SIZE				PT_FONT_SIZE_SMALL // 快捷部首字体大小
	#define GBIME_COMPONENT_COLOR					PT_COLOR_BLACK  // 快捷部首的颜色
	#define GBIME_ENGLISH_INPUTSTRING_COLOR			PT_COLOR_BLUE	// 9键英文输入的数字串的颜色
	#define GBIME_OUTPUT_STRING_COLOR				PT_COLOR_BLUE	// 输出串字体颜色
	#define GBIME_COMP_WIDTH						14 ///<国笔部首的宽度
	#define GBIME_COMP_HEIGHT						14 ///<国笔部首的高度
#elif defined(__MMI_MAINLCD_128X160__)   //LCD 128宽x160高 LCD 128X128
	#define GBIME_VERTICAL_SPACING 					8  ///<垂直方向显示间隔(动态计算字符高度垂直居中无效替代方案)
	#define GBIME_SYLLABLE_SPACING 					8  ///<音节栏字间隔
	#define GBIME_CANDIDATE_SPACING 				8  ///<候选栏字间隔
	#define GBIME_ARROW_WIDTH						25
	#define GBIME_CAND_AREA_WIDTH  					(PT_GetLcdWidth() - GBIME_ARROW_WIDTH * 2) ///<国笔候选框的宽度
	#define GBIME_SYLLABLE_DISPLAY_START_X			GBIME_ARROW_WIDTH  // 拼音显示的开始位置X坐标
	#define GBIME_SYLLABLE_LINE_WIDTH				GBIME_CAND_AREA_WIDTH // 拼音栏宽度
	#define GBIME_SYLLABLE_LINE_HEIGHT				21  // 拼音栏高度
	#define GBIME_CANDIDATE_DISPLAY_START_X			GBIME_ARROW_WIDTH  // 候选显示的开始位置的X坐标
	#define GBIME_CANDIDATE_LINE_WIDTH				GBIME_CAND_AREA_WIDTH	// 候选栏宽度
	#define GBIME_CANDIDATE_LINE_HEIGHT				23  // 候选栏高度
	#define GBIME_SYLLABLE_FONT_SIZE				PT_FONT_SIZE_SMALL // 音节字体大小
	#define GBIME_SYLLABLE_NORMAL_FONT_COLOR		PT_COLOR_BLACK  // 没有选择中的候选拼音颜色
	#define GBIME_SYLLABLE_SELECTED_FONT_COLOR		PT_COLOR_BLUE    // 选择中的的拼音的颜色
	#define GBIME_CANDIDATE_FONT_SIZE				PT_FONT_SIZE_SMALL // 候选字体大小
	#define GBIME_CANDIDATE_NORMAL_FONT_COLOR		PT_COLOR_BLACK  // 没有选择中的候选汉字颜色
	#define GBIME_CANDIDATE_SELECTED_FONT_COLOR		PT_COLOR_BLUE    // 选择中的候选汉字颜色
	#define GBIME_STROKE_FONT_SIZE					PT_FONT_SIZE_SMALL // 笔划字体大小
	#define GBIME_STROKE_COLOR						PT_COLOR_BLACK    // 笔划的颜色
	#define GBIME_COMPONENT_FONT_SIZE				PT_FONT_SIZE_SMALL // 快捷部首字体大小
	#define GBIME_COMPONENT_COLOR					PT_COLOR_BLACK  // 快捷部首的颜色
	#define GBIME_ENGLISH_INPUTSTRING_COLOR			PT_COLOR_BLUE	// 9键英文输入的数字串的颜色
	#define GBIME_OUTPUT_STRING_COLOR				PT_COLOR_BLUE	// 输出串字体颜色
	#define GBIME_COMP_WIDTH						14 ///<国笔部首的宽度
	#define GBIME_COMP_HEIGHT						14 ///<国笔部首的高度
#else
	#define GBIME_VERTICAL_SPACING 					8  ///<垂直方向显示间隔(动态计算字符高度垂直居中无效替代方案)
	#define GBIME_SYLLABLE_SPACING 					8  ///<音节栏字间隔
	#define GBIME_CANDIDATE_SPACING 				8  ///<候选栏字间隔
	#define GBIME_ARROW_WIDTH						33
	#define GBIME_CAND_AREA_WIDTH  					(PT_GetLcdWidth() - GBIME_ARROW_WIDTH * 2) ///<国笔候选框的宽度
	#define GBIME_SYLLABLE_DISPLAY_START_X			GBIME_ARROW_WIDTH  // 拼音显示的开始位置X坐标
	#define GBIME_SYLLABLE_LINE_WIDTH				GBIME_CAND_AREA_WIDTH // 拼音栏宽度
	#define GBIME_SYLLABLE_LINE_HEIGHT				30  // 拼音栏高度
	#define GBIME_CANDIDATE_DISPLAY_START_X			GBIME_ARROW_WIDTH  // 候选显示的开始位置的X坐标
	#define GBIME_CANDIDATE_LINE_WIDTH				GBIME_CAND_AREA_WIDTH	// 候选栏宽度
	#define GBIME_CANDIDATE_LINE_HEIGHT				33  // 候选栏高度
	#define GBIME_SYLLABLE_FONT_SIZE				PT_FONT_SIZE_DEFAULT // 音节字体大小
	#define GBIME_SYLLABLE_NORMAL_FONT_COLOR		PT_COLOR_BLACK  // 没有选择中的候选拼音颜色
	#define GBIME_SYLLABLE_SELECTED_FONT_COLOR		PT_COLOR_BLUE    // 选择中的的拼音的颜色
	#define GBIME_CANDIDATE_FONT_SIZE				PT_FONT_SIZE_DEFAULT // 候选字体大小
	#define GBIME_CANDIDATE_NORMAL_FONT_COLOR		PT_COLOR_BLACK  // 没有选择中的候选汉字颜色
	#define GBIME_CANDIDATE_SELECTED_FONT_COLOR		PT_COLOR_BLUE    // 选择中的候选汉字颜色
	#define GBIME_STROKE_FONT_SIZE					PT_FONT_SIZE_DEFAULT // 笔划字体大小
	#define GBIME_STROKE_COLOR						PT_COLOR_BLACK    // 笔划的颜色
	#define GBIME_COMPONENT_FONT_SIZE				PT_FONT_SIZE_SMALL // 快捷部首字体大小
	#define GBIME_COMPONENT_COLOR					PT_COLOR_BLACK  // 快捷部首的颜色
	#define GBIME_ENGLISH_INPUTSTRING_COLOR			PT_COLOR_BLUE	// 9键英文输入的数字串的颜色
	#define GBIME_OUTPUT_STRING_COLOR				PT_COLOR_BLUE	// 输出串字体颜色
	#define GBIME_COMP_WIDTH						14 ///<国笔部首的宽度
	#define GBIME_COMP_HEIGHT						14 ///<国笔部首的高度
#endif

/*!
 * \brief 其他
 */
#define GBIME_SYMBOL_COMMA_CHAR					0xFF0C // 逗号字符‘，’
#define GBIME_SYMBOL_FULL_STOP_CHN_CHAR			0x3002 // 句号字符‘。’
#define GBIME_SYMBOL_FULL_STOP_ALP_CHAR			0x002e // 句号字符‘.’

typedef struct __tag_GBIMEPoint
{
	GBINT16		x;
	GBINT16		y;
} GBIMEPoint, * PGBIMEPoint;

typedef struct __Tag_GBRect{
	GBINT16 left;
	GBINT16 top;
	GBINT16 right;
	GBINT16 bottom;
}GBRECT, * PGBRECT; ///< vc sizeof == 8

// 平台按键、触摸屏事件定义
enum GBPEventEnum{
	GBPEvent_None = 0

	// 按键事件
	,GBPEVT_KEY_DOWN
	,GBPEVT_KEY_UP
	,GBPEVT_KEY_LONG_PRESS
	,GBPEVT_KEY_REPEAT

	// 触摸屏事件
	,GBPEVT_PEN_DOWN
	,GBPEVT_PEN_MOVE
	,GBPEVT_PEN_UP
	,GBPEVT_NUM
};

/*!
* \brief GBIME 框架API的返回值定义
*/
enum GBIMEReturnEnum{
	GBIME_OK
	,GBIME_CONSUMED						///<这个事件已经被输入法框架处理，外部无需处理
	,GBIME_UNINITIALIZED				///<框架没有初始化
	,GBIME_FAILED
	,GBIME_IGNORE						///<忽略处理
	,GBIME_INVALID_PARAM				///<参数非法
	,GBIME_NOT_SUPPORT_INPUT_MODE		///<输入法框架不支持的输入模式(GBIMEInputMethodMap没有相应的映射项)
	,GBIME_NOT_SUPPORT_LAYOUT_ID		///<输入法框架不支持的Layout ID(GBIMEInputMethodMap没有相应的映射项)
	,GBIME_LAYOUT_NOEXIST				///<Layout不存在
	,GBIME_LAYOUT_NOTMATCH				///<Layout不匹配
	,GBIME_LAYOUT_CONVERT_MSG_FAIL		///<Layout转换消息失败
	,GBIME_EVENT_GROUP_IS_FULL			///<事件组满(无法继续添加)
	,GBIME_EVENT_GROUP_IS_NULL			///<事件组为空(无任何事件需要处理)
	,GBIME_EVENT_GROUP_NOT_EXIST		///<系统事件组表中不存在要查找的事件组
	,GBIME_OPEN_FILE_ERROR				///<开启文件失败
	,GBIME_NOT_ENGINE_CONFIG_ID			///<没有找到该引擎配置ID
};
typedef GBINT32	GBIMEReturn;

#endif //__GBIME_DEF_H_
