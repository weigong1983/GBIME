#ifndef __CLASS_LAYOUT_INFO_
#define __CLASS_LAYOUT_INFO_
#include "gbtype.h"
#include "GBIMEDef.h" // include GBRECT
#include "gbft_def.h"
//#define Layout_Test 

#define MAX_BUTTON_KEY_NAME_LEN		16 ///<(15 +1 )
#define MAX_BUTTON_COUNT			64

//lt->layout
//im->input method
//bt->buttom
//dir->direction
//swt->switch
//bk->backgound

typedef const GBRECT *LPCGBRECT;

typedef struct __Tag_GBSize{
	GBINT16 cx;
	GBINT16 cy;
}GBSIZE, * PGBSIZE; ///< vc sizeof == 4

typedef const GBSIZE *LPCGBSIZE;
typedef struct _GBImage
{
	GBRECT   rcPos;		///< rect(x,y)图片显示的左上角位置坐标，rect(w,h)图片大小
	GBUINT16 imageID;	///< 图片ID
}GBImage; ///< vc sizeof == 10

/*!
 *- Button Const Information
 */
typedef struct __tag_ButtonConstData
{
	GBRECT					btPos;								///<按键相对于输入法windows的坐标
	GBUINT16			    btKey;								///<键值
	GBImage					btNormal;						    ///<当前显示图片
	GBImage					btDown;							    ///<按下图片
	GBImage					btGrey;							    ///<灰色图片
	GBUINT16				btKeyName[MAX_BUTTON_KEY_NAME_LEN];	///<键名
}BUTTONCONSTDATA, * PBUTTONCONSTDATA; ///< vc sizeof == 82
typedef const BUTTONCONSTDATA _gb_far *         GBLPCBUTTONCONSTDATA;

/*!
 *- Button Varia & Const Information
 */
typedef struct __tag_ButtonChangedData
{
//	GBUINT16			    btFlag;								///<符号标记,是否就是输入符号   
// 	GBUACHAR			    btIsIcon;							///<是否显示该数组中的szKeyName，normalBmp，downBmp，greyBmp
 	GBUINT16			    btStatus;							///<按键状态，0为正常态，1为按下，2为灰色，3隐藏当前按键
	GBPCVOID                btpfcResponse;                      ///<button相应的响应函数指针
	GBPWCHAR                btpChgKeyName;					    ///<有些button文本是需要修改的，如笔画的部首，表情符号字符串
} BUTTONCHGDATA, *PBUTTONCHGDATA; //vc sizeof == 12

typedef const BUTTONCHGDATA _gb_far *         GBLPCBUTTONCHGDATA;

/*!
 *- Layout Information
 */
typedef struct __Tag_LayoutConstLayer{
	GBRECT					lyPos;								///<按键相对于输入法windows的坐标

	GBUINT16			    lyImageID;							///< Layer Image ID
	GBUINT16			    lyBKImageID;						///< Layer Background Image ID

	GBUINT16                __pack_1;
	GBUINT16			    lyBtCount; 
	//GBLPCBUTTONCONSTDATA    lypBtData;						    ///< 保存键盘上的所有按钮的数组
}LAYOUTCONSTLAYER, *PLAYOUTCONSTLAYER;

typedef const LAYOUTCONSTLAYER _gb_far *         GBLPCLAYOUTCONSTLAYER;

/*!
 *- Layout Information
 */
typedef struct __Tag_LayoutChgtLayer
{
	GBUINT16			    lyBtCount; 
	GBLPCLAYOUTCONSTLAYER   plycData;                           ///<
	GBLPCBUTTONCONSTDATA    plycBtData;						    ///< 保存键盘上的所有按钮的数组
	PBUTTONCHGDATA          plyBtChgData;
}LayoutChgtLayer, * PLayoutChgtLayer;


/*!
 *- Layout Const Information
 */
typedef struct __Tag_LayoutConstInfo
{
	GBUINT16 ltType;                                          ///< Layout type
	GBUINT16 ltID;                                            ///< Layout ID
	GBUINT16 ltDir;                                           ///< Layout direction
	GBUINT16 ltIMLang;                                        ///< 输入法语言类型。值对应gbim.h中GBLanguageEnum定义。
	GBUINT16 ltIMSubLang;                                     ///< 输入法子语言类型。值对应gbim.h中GBSubLanguageEnum定义。   
	GBUINT16 ltIMMode;                                        ///< 输入法类型键，值对应gbim.h中GBInputModeEnum定义。

	GBUINT16 ltScrSwtID;                                      ///< 横竖屏转换,转到相应的横屏类型 
	GBUINT16 ltUPSwtID;                                       ///< 向上键, 转到相应的Layout ID 
	GBUINT16 ltDownSwtID;                                     ///< 向下键, 转到相应的Layout ID  
	GBUINT16 ltLeftSwtID;                                     ///< 向左键, 转到相应的Layout ID 
	GBUINT16 ltRightSwtID;                                    ///< 向右键, 转到相应的Layout ID 

	GBUINT16 __pack_1;             
	GBUINT16 lyLayerCount;			                          ///< Layout layer Count
	//GBLPCLAYOUTCONSTLAYER ltLayerData;                        ///<
}LAYOUTCONSTINFO, *PLAYOUTCONSTINFO; ///< vc sizeof == ?

typedef const LAYOUTCONSTINFO _gb_far *         GBLPCLAYOUTCONSTINFO;

/*!
*- Layout Information
*/
typedef struct __Tag_LayoutInfo
{
	GBLPCLAYOUTCONSTINFO  pltcData;                        ///< Layout layer Const data
	GBUINT16			  lyLayerCount;                    ///< Layout layer Count
	PLayoutChgtLayer      pltLayerData;                    ///< Layout layer changed data

	GBUINT16			  ltCurLayer;                      ///< Current Layout layer
}LayoutInfo, * PLayoutInfo;

/** \brief 通过Layout ID 值获取对应Layout 数据指针*/
typedef GBLPCLAYOUTCONSTINFO (* FPGetLayoutDataByID)(GBLPCVOID pskinData, GBUINT16 ltID);
/** \brief 初始化皮肤数据 */
//typedef GBINT32 (* FPSkinInitData)(PSkinData pskInfo, GBPCVOID pskcData);

typedef struct __Tag_SkinData
{
	GBPCVOID              pskcData;
	GBUINT16              ltCount;
#ifdef Layout_Test //Layout对象需要动态创建
	PLayoutInfo           pskLtData;
#endif
	GBUINT16			  ltDefID;                      ///< input method type
	//GBUINT16			  ltCurID;                      ///< input method type
	///< im记录对应输入法,这个值对于"PPC输入法"是一个不完整的冗余数据,但是如果要整合实体键盘,可能会有用
	///< 对应输入法, PINYIN, STROKE, SMART_ENGLISH
	//GBUINT16 ltIMLang;                                        ///< 输入法语言类型。
	//GBUINT16 ltIMSub;                                         ///< 输入法类型键，比如，拼音，笔划等

}SkinData, * PSkinData; ///< vc sizeof == 776, MAX_BUTTON_COUNT==64

typedef const SkinData _gb_far *         GBLPCSkinData;

/*!
 *- Layout Data Offset
 */
typedef struct __Tag_LayoutBinaryDataOffset{
	GBUINT16 ltID;
	GBUINT16 ltHighOffset;
	GBUINT16 ltLowerOffset;
}LayoutBinaryDataOffset, PLayoutBinaryDataOffset;

typedef const LayoutBinaryDataOffset _gb_far *         GBLPCLayoutBinaryDataOffset;
/*!
 *- Layout Data Index Info
 */
typedef struct __Tag_LayoutBinaryDataIndexInfo
{
	GBUINT16 ltCount;
	GBUINT16 ltDefID;
	GBLPCLayoutBinaryDataOffset ltOffsetArray;
}LayoutBinaryDataIndexInfo, * PLayoutBinaryDataIndexInfo;

//Layout层号
typedef enum __tag_LayerIndex
{
	FirstLayer,
	SecondLayer,
	ThirdLayer,
	FourthLayer
}LayerIndex;
//按钮状态
typedef enum _ButtonStatusEnum
{
	ButtonStatusNomal,
	ButtonStatusDown,
	ButtonStatusUp,
	ButtonStatusMove,
	ButtonStatusGray,
	ButtonStatusHide
}ButtonStatusEnum;

//Layout类型
typedef enum _LayoutIdEnum
{
	LayoutIdSP9JPinYin			= 0	,//竖屏9键拼音
	LayoutIdSP9JStroke			= 1	,//竖屏9键笔画
	LayoutIdSP9JLowerEnglish	= 2	,//竖屏9键小写英文
	LayoutIdSP9Jabc				= 3	,//竖屏9键小写字母
	LayoutIdSP9JABC				= 4	,//竖屏9键大写字母
	LayoutIdSP9JNumber			= 5	,//竖屏9键数字
	LayoutIdSP9JSymbol			= 6	,//竖屏9键符号
	LayoutIdSP26JPinYin			= 7	,//竖屏26键拼音
	LayoutIdSP26JStroke			= 8	,//竖屏26键笔画
	LayoutIdSP26JLowerEnglish	= 9	,//竖屏26键小写英文
	LayoutIdSP26Jabc			= 10,//竖屏26键小写字母
	LayoutIdSP26JABC			= 11,//竖屏26键大写字母
	LayoutIdSP26JNumber			= 12,//竖屏26键数字
	LayoutIdSP26JSymbol			= 13,//竖屏26键符号
	LayoutIdHP9JPinYin			= 14,//横屏9键拼音
	LayoutIdHP9JStroke			= 15,//横屏9键笔画
	LayoutIdHP9JEnglish			= 16,//横屏9键英文
	LayoutIdHP9Jabc				= 17,//横屏9键小写字母
	LayoutIdHP9JABC				= 18,//横屏9键大写字母
	LayoutIdHP9JNumber			= 19,//横屏9键数字
	LayoutIdHP9JSymbol			= 20,//横屏9键符号
	LayoutIdHP26JPinYin			= 21,//横屏26键拼音
	LayoutIdHP26JStroke			= 22,//横屏26键笔画
	LayoutIdHP26JEnglish		= 23,//横屏26键英文
	LayoutIdHP26Jabc			= 24,//横屏26键小写字母
	LayoutIdHP26JABC			= 25,//横屏26键字母大写
	LayoutIdHP26JNumber			= 26,//横屏26键数字
	LayoutIdHP26JSymbol			= 27,//横屏26键符号
	LayoutIdSPGeZiShouXiue		= 28,//竖屏格子手写
	LayoutIdHPGeZiShouXiue		= 29,//横屏格子手写
	LayoutIdSPQuanPingShouXiue	= 30,//竖屏全屏手写
	LayoutIdHPQuanPingShouXiue	= 31,//横屏全屏手写
	LayoutIdOneRowCandidate		= 32,//一行候选栏
	LayoutIdTowRowCandidate		= 33,//二行候选栏
	LayoutIdSP9JUpEnglish		= 34,//竖屏9键大写英文
	LayoutIdSP26JUpEnglish		= 35,//竖屏26键大写英文

	LayoutIdWL9JPinYin			= 36,//	物理9键拼音
	LayoutIdWL9JStroke			= 37,//	物理9键笔画
	LayoutIdWL9JEnglish			= 38,//	物理9键英文
	LayoutIdWL26JPinYin			= 39,//	物理26键拼音
	LayoutIdWL26JStroke			= 40,//	物理26键笔画
	LayoutIdWL26JEnglish		= 41,//	物理26键英文
	LayoutIdWL9JABC				= 42,//	物理9键大写字母
	LayoutIdWL9Jabc				= 43,//	物理9键小写字母
	LayoutIdWL9JNumber 			= 44,//	物理9键数字
	LayoutIdWL26JABC			= 45,//	物理26键大写字母
	LayoutIdWL26Jabc			= 46,//	物理26键小写字母	
	LayoutIdWL26JNumber 		= 47,//	物理26键数字
	
	// 新增MTK平台应用专属Layout	
	LayoutIdPassword_V			= 48,//竖屏密码输入
	LayoutIdFloatNumber_V		= 49,//竖屏浮点数字
	LayoutIdPhoneNumber_V		= 50,//竖屏电话号码
// 	LayoutIdQuickSearchPinyin9_V= 51,//竖屏拼音9键快速查找
// 	LayoutIdQuickSearchPinyin26_V= 52,//竖屏拼音26键快速查找
// 	LayoutIdQuickSearchStroke9_V= 53,//竖屏笔画9键快速查找
// 	LayoutIdQuickSearch26Abc_V	= 54,//竖屏9键abc快速查找
// 	LayoutIdQuickSearch9Abc_V	= 55,//竖屏26键abc快速查找

	// 多语言新增LayoutID
	LayoutId_SP9_UPPERCASE_FRENCH = 60 // 竖屏9键大写法文
	,LayoutId_SP9_LOWERCASE_FRENCH = 61 // 竖屏9键小写法文
	,LayoutId_SP9_UPPERCASE_FRENCH_ABC = 62 // 竖屏9键大写法文字母
	,LayoutId_SP9_LOWERCASE_FRENCH_ABC = 63 // 竖屏9键小写法文字母
	
	,LayoutId_SP9_UPPERCASE_GERMAN = 64 // 竖屏9键大写德文
	,LayoutId_SP9_LOWERCASE_GERMAN = 65 // 竖屏9键小写德文
	,LayoutId_SP9_UPPERCASE_GERMAN_ABC = 66 // 竖屏9键大写德文字母
	,LayoutId_SP9_LOWERCASE_GERMAN_ABC = 67 // 竖屏9键小写德文字母
	
	,LayoutId_SP9_UPPERCASE_SPANISH = 68 // 竖屏9键大写西班牙文
	,LayoutId_SP9_LOWERCASE_SPANISH = 69 // 竖屏9键小写西班牙文
	,LayoutId_SP9_UPPERCASE_SPANISH_ABC = 70 // 竖屏9键大写西班牙文字母
	,LayoutId_SP9_LOWERCASE_SPANISH_ABC = 71 // 竖屏9键小写西班牙文字母
	
	,LayoutId_SP26_UPPERCASE_FRENCH = 72 // 竖屏26键大写法文
	,LayoutId_SP26_LOWERCASE_FRENCH = 73 // 竖屏26键小写法文
	,LayoutId_SP26_UPPERCASE_FRENCH_ABC = 74 // 竖屏26键大写法文字母
	,LayoutId_SP26_LOWERCASE_FRENCH_ABC = 75 // 竖屏26键小写法文字母
	
	,LayoutId_SP26_UPPERCASE_GERMAN = 76 // 竖屏26键大写德文
	,LayoutId_SP26_LOWERCASE_GERMAN = 77 // 竖屏26键小写德文
	,LayoutId_SP26_UPPERCASE_GERMAN_ABC = 78 // 竖屏26键大写德文字母
	,LayoutId_SP26_LOWERCASE_GERMAN_ABC = 79 // 竖屏26键小写德文字母
	
	,LayoutId_SP26_UPPERCASE_SPANISH = 80 // 竖屏26键大写西班牙文
	,LayoutId_SP26_LOWERCASE_SPANISH = 81 // 竖屏26键小写西班牙文
	,LayoutId_SP26_UPPERCASE_SPANISH_ABC = 82 // 竖屏26键大写西班牙文字母
	,LayoutId_SP26_LOWERCASE_SPANISH_ABC = 83 // 竖屏26键小写西班牙文字母
	
	,LayoutId_HP9_UPPERCASE_FRENCH = 84 // 横屏9键大写法文
	,LayoutId_HP9_LOWERCASE_FRENCH = 85 // 横屏9键小写法文
	,LayoutId_HP9_UPPERCASE_FRENCH_ABC = 86 // 横屏9键大写法文字母
	,LayoutId_HP9_LOWERCASE_FRENCH_ABC = 87 // 横屏9键小写法文字母
	
	,LayoutId_HP9_UPPERCASE_GERMAN = 88 // 横屏9键大写德文
	,LayoutId_HP9_LOWERCASE_GERMAN = 89 // 横屏9键小写德文
	,LayoutId_HP9_UPPERCASE_GERMAN_ABC = 90 // 横屏9键大写德文字母
	,LayoutId_HP9_LOWERCASE_GERMAN_ABC = 91 // 横屏9键小写德文字母
	
	,LayoutId_HP9_UPPERCASE_SPANISH = 92 // 横屏9键大写西班牙文
	,LayoutId_HP9_LOWERCASE_SPANISH = 93 // 横屏9键小写西班牙文
	,LayoutId_HP9_UPPERCASE_SPANISH_ABC = 94 // 横屏9键大写西班牙文字母
	,LayoutId_HP9_LOWERCASE_SPANISH_ABC = 95 // 横屏9键小写西班牙文字母
	
	,LayoutId_HP26_UPPERCASE_FRENCH = 96 // 横屏26键大写法文
	,LayoutId_HP26_LOWERCASE_FRENCH = 97 // 横屏26键小写法文
	,LayoutId_HP26_UPPERCASE_FRENCH_ABC = 98 // 横屏26键大写法文字母
	,LayoutId_HP26_LOWERCASE_FRENCH_ABC = 99 // 横屏26键小写法文字母
	
	,LayoutId_HP26_UPPERCASE_GERMAN = 100 // 横屏26键大写德文
	,LayoutId_HP26_LOWERCASE_GERMAN = 101 // 横屏26键小写德文
	,LayoutId_HP26_UPPERCASE_GERMAN_ABC = 102 // 横屏26键大写德文字母
	,LayoutId_HP26_LOWERCASE_GERMAN_ABC = 103 // 横屏26键小写德文字母
	
	,LayoutId_HP26_UPPERCASE_SPANISH = 104 // 横屏26键大写西班牙文
	,LayoutId_HP26_LOWERCASE_SPANISH = 105 // 横屏26键小写西班牙文
	,LayoutId_HP26_UPPERCASE_SPANISH_ABC = 106 // 横屏26键大写西班牙文字母
	,LayoutId_HP26_LOWERCASE_SPANISH_ABC = 107 // 横屏26键小写西班牙文字母
	
	,LayoutId_SP_HANDWRITING_FRENCH = 108 // 竖屏手写法文
	,LayoutId_SP_HANDWRITING_GERMAN = 109 // 竖屏手写德文
	,LayoutId_SP_HANDWRITING_SPANISH = 110 // 竖屏手写西班牙文
	
	,LayoutId_HP_HANDWRITING_FRENCH = 111 // 横屏手写法文
	,LayoutId_HP_HANDWRITING_GERMAN = 112 // 横屏手写德文
	,LayoutId_HP_HANDWRITING_SPANISH = 113, // 横屏手写西班牙文

	// 新增的LyoutID请添加到此行之前
	LayoutIdEnd						= 0x0FFF,

	// 皮肤工具定制的Layout切换指令
	LayoutIdSwitchPreObj			= 0x1000, ///<按返回键切换到上一个Layout
	LayoutIdSwitchLatestObj			= 0x1001, ///<切换到最近一个Layout
	LayoutIdSwitchLatestChnObj		= 0x1002, ///<切换到最近一个中文 Layout
	LayoutIdSwitchLatestAlpObj		= 0x1003, ///<切换到最近一个Alp Layout
	LayoutIdHide					= 0x1004  ///<隐藏Layout
}LayoutIdEnum;
///<start [haj add 2010-8-27]

/*!
 *- 检查LayoutID是否有效
 */
#define IS_VALID_LAYOUT_ID(layoutID) (layoutID >= LayoutIdSP9JPinYin && layoutID <= LayoutIdEnd) \
								     || (layoutID >= LayoutIdSwitchPreObj && layoutID <= LayoutIdHide)

enum __tag_LayoutType
{
	LayoutHandwriteFullScreen	///<全拼手写
	,LayoutHandwriteMultiBlock  ///<格子手写（暂时不做：市场需求多）
	
	,LayoutVK9Pinyin			///<虚拟9键--拼音类
	,LayoutVK9Stroke			///<虚拟9键--笔画类
	,LayoutVK9MultiLang			///<虚拟9键--多语言类
	
	,LayoutVK26Pinyin			///<虚拟全键盘拼音类
	,LayoutVK26Stroke			///<虚拟全键盘笔画类
	,LayoutVK26MultiLang		///<虚拟全键盘多语言类
	
	,LayoutVKNumber				///<键值直接上屏类--数字
	,LayoutVKSymbol				///<键值直接上屏类--符号
	
	,LayoutVKCopyPaste			///<复制粘贴（暂时不做：非输入法产品必须功能，以后再做计划。）
	,LayoutVKProductConfig		///<产品配置（暂时不做：目前用平台菜单方式配置产品。）
	
	,LayoutKB9SingleRow			///<物理9键单行
	,LayoutKB26SingleRow		///<物理Qwerty单行
	,LayoutKB9TwinRow			///<物理9键双行
	,LayoutKB26TwinRow			///<物理Qwerty双行
	,LayoutFullScreenSymbol		///<全拼标点符号
};

// 键盘类型
typedef enum _tagkeyboardType
{
	KEYBOARD_VK_NUMPAD		// 数字键盘
	,KEYBOARD_VK_QWERTY		// Qwerty键盘
	,KEYBOARD_KB_NUMPAD		//物理Number键盘
	,KEYBOARD_KB_QWERTY		//物理Qwerty键盘
	,KEYBOARD_NUM
}KEYBOARD_TYPE;

typedef struct __tag_LayoutKey2LayoutID_Mapping{
	GBUINT16 layoutKey;
	GBUINT16 layoutID;
}MAPPING_LAYOUTKEY2LAYOUTID, * PMAPPING_LAYOUTKEY2LAYOUTID;

typedef const MAPPING_LAYOUTKEY2LAYOUTID * PCMAPPING_LAYOUTKEY2LAYOUTID;

extern const MAPPING_LAYOUTKEY2LAYOUTID GBCMappingLayoutKey2ID[];
///<end [haj add 2010-8-27]

//触摸笔按下时所在的位置
typedef enum _PenPosition
{
	PenPositionInNothing,//不在处理范围中时
	PenPositionInSyllable,//点中章节时
	PenPositionInCandidate,//点中候选时
	PenPositionInKeyboardButton,//点中键盘上的按钮时
	PenPositionInSyllablePageDown,//点中拼音向下翻页
	PenPositionInSyllablePageUp,//点中拼音向上翻页
	PenPositionInCandidatePageDown,//点中候选向下翻页
	PenPositionInCandidatePageUp,//点中候选向上翻页
	PenPositionLeave//触摸笔离开按下位置
}PenPosition;

/*!
 * \brief 滑屏切换layout方向定义
 */
typedef enum {
	SLIDING_DIRECTION_UP		///<上
	,SLIDING_DIRECTION_DOWN		///<下
	,SLIDING_DIRECTION_LEFT		///<左
	,SLIDING_DIRECTION_RIGHT	///<右
}GBSlidingDirection;

/*!
 * \ 判断当前layoutID属于虚拟键盘
 */
#define IS_VK_LAYOUT(layoutID)	((layoutID) >=LayoutIdSP9JPinYin &&  (layoutID) <= LayoutIdSP26JUpEnglish)

/*!
 * \ 判断当前layoutID属于物理键盘
 */
#define IS_KB_LAYOUT(layoutID)	((layoutID) >=LayoutIdWL9JPinYin &&  (layoutID) <= LayoutIdWL26JNumber)

/*!
 * \ 判断当前layoutID为手写layout
 */
#define IS_HANDWRITE_LAYOUT(layoutID)	((layoutID) >=LayoutIdSPGeZiShouXiue &&  (layoutID) <= LayoutIdHPQuanPingShouXiue)

/*!
 * \ 判断是否物理数字键盘，此时不需要候选框
 */
#define IS_PHYSIC_NUMBER_LAYOUT(layoutID) (layoutID == LayoutIdWL9JNumber || layoutID == LayoutIdWL26JNumber)

/*!
 * \ 判断是否大写英文、字母型键盘
 */
#define IS_UPPERCASE_LAYOUT(layoutID) (layoutID == LayoutIdSP9JABC \
										|| layoutID == LayoutIdSP26JABC \
										|| layoutID == LayoutIdHP9JABC \
										|| layoutID == LayoutIdHP26JABC \
										|| layoutID == LayoutIdSP9JUpEnglish \
										|| layoutID == LayoutIdSP26JUpEnglish \
										|| layoutID == LayoutIdWL9JABC \
										|| layoutID == LayoutIdWL26JABC)

GBLPCLAYOUTCONSTINFO CLayoutInfo_GetLayoutDataByID(GBLPCVOID pskinData, GBUINT16 ltID);
GBUINT16 CLayoutInfo_GetLayoutTypeByID(GBLPCVOID pskinData, GBUINT16 ltID);
GBUINT16 CLayoutInfo_GetLayoutTypeFromLtData(GBLPCLAYOUTCONSTINFO pltData);
GBINT32 CLayoutInfo_SkinReleaseData(PSkinData pltInfo);
GBINT32 CLayoutInfo_LayoutInitData(PLayoutInfo pltInfo, GBLPCVOID pltData);
GBINT32 CLayoutInfo_LayoutInitDataEx(PLayoutInfo pltInfo, PSkinData pskInfo, GBUINT16 ltID);
GBINT32 CLayoutInfo_LayoutReleaseData(PLayoutInfo pltInfo);
GBINT32 CLayoutInfo_SkinReleaseData(PSkinData pskInfo);
GBINT32 CLayoutInfo_SkinInitData(PSkinData pskInfo, GBPCVOID pskcData);

GBLPCLAYOUTCONSTINFO CLayoutInfo_GetLayoutConstInfo(GBLPCVOID pskinData,GBUINT16 layoutId);
PLAYOUTCONSTLAYER CLayoutInfo_GetLayerConstInfo(GBLPCVOID pskinData,GBUINT16 layoutId,GBUINT16 layerId);
PBUTTONCONSTDATA CLayoutInfo_GetLayerConstButtons(GBLPCVOID pskinData,GBUINT16 layoutId,GBUINT16 layerId);
GBUINT16 CLayoutInfo_ConversionLayoutKeyToEngineKey(GBUINT16 iLayoutKeyValue);
GBUINT16 CLayoutInfo_GetLayoutHeight(GBLPCVOID pskinData,LayoutIdEnum layoutId);
GBUINT16 CLayoutInfo_GetLayoutWidth(GBLPCVOID pskinData,LayoutIdEnum layoutId);
GBBOOL CLayoutInfo_GetBottonRectByKey(GBLPCVOID pskinData,GBUINT16 layoutId,GBUINT16 layerId,GBUINT16 iKeyValue,PGBRECT pRect);
GBBOOL CLayoutInfo_ThisKeyValueIsFunctionKey(GBUINT16 iLayoutKeyValue);
GBINT16 CLayoutInfo_GetCandidateBoxHeight(GBLPCVOID pskinData,GBUINT16 layoutId,GBUINT16 layerId);

/*!
 * \brief 根据LayoutID获取国笔切换输入模式切换需要的信息
 * \param pSkinData 
 * \param layoutId 
 * \param pGBInputMode 
 * \param pLang 
 * \param pSubLang 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-8-20 12:55:50
*/
GBIMEReturn CLayoutInfo_GetGBInputModeInfoByLayoutID(GBLPCVOID pSkinData,
													 GBUINT16 layoutId, 
													 GBUINT16 *pGBInputMode, 
													 GBUINT16 *pLang, 
													 GBUINT16 *pSubLang);

/*!
 * \brief 从Layout信息中获取LayoutID
 * \param pltInfo 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-9 17:24:48
*/
GBUINT16 CLayoutInfo_GetLayoutID(PLayoutInfo pltInfo);

/*!
 * \brief 根据滑屏方向获得目标切换LayoutID
 * \param pltInfo 
 * \param dir 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-24 14:19:32
*/
GBUINT16 CLayoutInfo_GetSlidingSwitchLayoutID(PLayoutInfo pltInfo, GBSlidingDirection dir);

/*!
 * \brief 获取LayoutID对应的键盘类型
 * \param layoutID 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-29 9:56:33
*/
KEYBOARD_TYPE CLayoutInfo_GetKeyboardTypeByLayoutID(GBUINT16 layoutID);

/*!
 * \brief 获取小写LayoutID对应的大写LayoutID
 * \param lowerLayoutID 
 * \return 
 * \note 寻找不到返回自身
 * \example
 * \author weizhiping
 * \date 2010-10-21 17:23:09
*/
GBUINT16 CLayoutInfo_GetUpperLayoutID(GBUINT16 lowerLayoutID);

/*!
 * \brief 获取大写LayoutID对应的小写LayoutID
 * \param upperLayoutID 
 * \return 
 * \note 寻找不到返回自身
 * \example
 * \author weizhiping
 * \date 2010-10-21 17:23:09
*/
GBUINT16 CLayoutInfo_GetLowerLayoutID(GBUINT16 upperLayoutID);

/*!
 * \brief 获取指定键值的按钮名称
 * \param pskinData 
 * \param layoutId 
 * \param layerId 
 * \param iKeyValue 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-10-20 10:13:47
*/
GBWCHAR * CLayoutInfo_GetBottonKeyName(GBLPCVOID pskinData, GBUINT16 layoutId, GBUINT16 layerId, GBUINT16 iKeyValue);

/*!
 * \brief 设置指定Layout按钮的状态(应用层可利用此接口禁用指定按钮)
 * \param pLayoutInfo 
 * \param iKeyValue 
 * \param btStatus 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-12-8 9:48:48
*/
GBBOOL CLayoutInfo_SetButtonStatus(PLayoutInfo pLayoutInfo, GBUINT16 iKeyValue, GBUINT16 btStatus);

#endif //__CLASS_LAYOUT_INFO_