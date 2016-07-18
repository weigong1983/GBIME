/***************************************************************************
* File Name    :  CLayoutControl.h
* Corporation  :  Guobi Technology Ltd. GuangZhou China
* Copyright    :  Copyright Guobi Technology Ltd. And Subject to Confidentiality 
*                 Restrictions. This document is the confidential and proprietary 
*                 property of Guobi Technology Ltd.It is neither the document                  
*                 itself nor any part of it can be shared with any party  without 
*                 the written  permission. 
* Author       :  fanbo
* Date         :  2010-06-08
* Purpose      :  Layout内部逻辑控制头文件
* Version      :  V1.0
* Last Changed List
* Name         |   date   |  Discription
****************************************************************************/
#ifndef __CLAYOUTCONTROL_H_
#define __CLAYOUTCONTROL_H_
#include "gbtype.h"
#include "GBIMEDef.h"
#include "gbft_def.h"
#include "CGBEngine.h"

// 将Layout视图显示需要引用的国笔引擎输出内容组织到如下结构体，然后通知视图刷新
typedef struct tagEngineOutputInfo
{	
	GBPCWCHAR  						pInputString;///<引擎输入序列
	/**< 需要上屏的字符串, 指向空字符串的时候表示没有需要上屏的字符, 用户上屏完成之后负责清空此字符串 */
	GBWCHAR	*						pUpScreenStr;	
	//GBU32							nStatusFlag;	///<保存当前的状态
	GBBOOL							bCandWindOpen; ///<候选框激活状态
	GBBOOL							bInputEmpty; ///<引擎当前的输入是否为空
	
	// 候选相关
	GBU16 							nCandNum;		/**< 候选个数 */
	GBPCWCHAR 						*ppCandidates;	/** (output)候选字词二维数组*/

	// 音节相关
	GBU8 							nSyllableNum;/**< 当前页拼音音节的个数，拼音会根据GBSetPageOption
												 里面拼音的分页信息来分页， 如果需要返回所有的拼音候选，
												 则只需要把GBSetPageOption 中拼音的分页设置为返回GB_MAX_SYLLABLES个拼音即可*/	
	GBU8 							nCurrentSyllableIndex;	/**< 当前选择拼音的下标 = nSyllableIndex - nSyllablePageStartIndex */	
	GBPCWCHAR						*ppSyllables;/**< (GBMMI)拼音音节 只是根据翻页信息, (GBAPI)所有可能的拼音*/

	// 笔画部首相关
	GBPWCHAR 						pStroke;/**< 笔画区内码串 */
	GBPWCHAR 						pComponents;/**< 部首区内码串 */
	GBPCWCHAR						pOutputString;			///<中文输入法自造词的时候已经选择了的词语以及后面的输入串

	///(GBMMI)应当显示为Inline Edit的候选, 一般在MultiTap、MultiTap Symbols以及无候选框的Alphabetic模式有效
	GBLPCWCHAR						pCompStr;///<等于pAuxInfo->pOutputString + pAuxInfo->nSelZiCount;

	// 候选框中箭头的指示标记
	GBBOOL							bUpScreenArrow;		///<上屏箭头
	GBBOOL							bCandArrowUp;		///<候选显示 上 箭头
	GBBOOL							bCandArrowDown;		///<候选显示 下 箭头
	GBBOOL							bCandArrowLeft;		///<候选显示 左 箭头
	GBBOOL							bCandArrowRight;	///<候选显示 右 箭头
	GBBOOL							bSyllableArrowLeft; ///<Ambiguous Chinese only, 拼音翻页显示左箭头
	GBBOOL							bSyllableArrowRight;///<Ambiguous Chinese only, 拼音翻页显示右箭头

	//以下输出仅限于多行候选的情况, 如全屏标点符号
	///每行行候选的开始位置, pRowStart 指向一个长度为GB_CAND_MAX_LINE_COUNT + 1的Byte数组\ref GB_CAND_MAX_LINE_COUNT
	GBPBYTE							pRowStart;		 //GBMMI使用	
	GBI8							nSelCol;		///<当前选择的列 GBMMI使用
	GBI8							nSelRow;		///<当前选择的行 GBMMI使用
	GBU8							nRowCount;		///<当前页有效的行数 GBMMI使用

	// 状态相关
	GBBOOL							isSelectedStatus;	///<是否为选择状态
	GBBOOL							isMultiTapInputMode; ///<是否Mutitap输入模式
	GBBOOL							isSwitchNextInputMode;	//<是否为切换输入模式
	GBBOOL							isSymbolCandType;	///<是否为符号模式
	//GBShiftCapStatus                shiftCapStatus; 框架记录此状态
}EngineOutputInfo, *PEngineOutputInfo;

typedef struct _tag_CLayoutControl
{
	EngineOutputInfo engineOutputInfo; //  layout显示需要用到的引擎输出信息
}CLayoutControl, *PCLayoutControl;

/*!
 * \brief Layout控制模块更新引擎输出信息
 * \param void 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-07-28
 */
GBIMEReturn CLayoutControl_UpdateOutputInfo(void);

/*!
 * \brief 获取Layout控制模块维护的引擎输出信息
 * \param void 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-07-28
 */
PEngineOutputInfo CLayoutControl_GetEngineOutputInfo(void);

/*!
 * \brief Layout消息处理函数
 * \param pIMEEvent 
 * \return 
 */
GBIMEReturn CLayoutControl_HandleMessage(PGBIMEEvent pIMEEvent);

/*!
 * \brief 获取候选框是否打开
 * \param void 
 * \return 
 * \note 
 * \example
 * \author ZhaoKun
 * \date 2010-10-22
 */
GBBOOL CLayoutControl_IsOpenCandWind(void);

#endif //__CLAYOUTCONTROL_H_