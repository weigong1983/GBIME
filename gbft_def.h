#ifndef __GB_PLATFORM_DEFINE_
#define __GB_PLATFORM_DEFINE_
#include "gbtype.h"

/*!
 * \brief 输入法框架事件集最大事件容量
 */
#define GBIME_EVT_GROUP_CAPACITY	10

/** \struct __tag_GBIMEEvent
	\brief event structure
 */
typedef struct __tag_GBIMEEvent
{
	GBUINT32	nType;	///< event type \see GBIMEEventType
	GBUINT32	nEvent; ///< event value
	GBINT32		wParam;	///< parameter 1
	GBINT32		lParam;	///< parameter 2
} GBIMEEvent, * PGBIMEEvent ;

/*!
 * \brief 一个平台事件通常解析为输入法框架内部相互关联的事件集
 */
typedef struct __tag_GBIMEEventGroup
{
	GBIMEEvent	eventGroup[GBIME_EVT_GROUP_CAPACITY];
	GBU8		eventNum;
} GBIMEEventGroup, * PGBIMEEventGroup ;

/*!
 * \brief 输入法框架事件类型定义
 */
enum GBIMEEventType {
	GBIME_EVENT_TYPE_FRAMEWORK				///<国笔输入法框架事件(事件值参考GBIMEFrameworkEventEnum)
	,GBIME_EVENT_TYPE_ENGINE				///<国笔引擎事件(事件值参考GBIMEEngineEventEnum)
	,GBIME_EVENT_TYPE_LAYOUT				///<Layout事件(事件值参考GBIMELayoutEventEnum)
	,GBIME_EVENT_TYPE_INPUT_METHOD			///<输入法事件(事件值参考GBEventEnum)
	,GBIME_EVENT_TYPE_CONFIG				///<配置事件(事件值参考GBEventEnum)
	,GBIME_EVENT_TYPE_HANDWRITE				///<国笔手写引擎事件(事件值参考GBIMEHandwriteEventEnum)
	,GBIME_EVENT_TYPE_NUM					///<事件类型数目
};

/*!
 * \brief 输入法框架原子事件定义
 */
enum GBIMEEventAtom {
	// 框架
	GBIME_EVENT_FRAMEWORK_INIT					///<框架初始化
	,GBIME_EVENT_FRAMEWORK_EXIT					///<框架退出

    // 引擎
	,GBIME_EVENT_ENGINE_INIT					///<引擎初始化
	,GBIME_EVENT_ENGINE_EXIT					///<引擎退出
	,GBIME_EVENT_ENGINE_RESET					///<引擎状态重置[切换输入法模式、关闭候选框或隐藏Layout时使用]
	,GBIME_EVENT_ENGINE_KEY						///<引擎按键事件，交给GBMMIHandleKeyEvent处理
	,GBIME_EVENT_ENGINE_GET_ASSOCWORD			///<获取联想字符串
	,GBIME_EVENT_ENGINE_GET_HOMONYMWORD			///<获取同音字

	// Layout
	,GBIME_EVENT_LAYOUT_CREATE					///<Layout 创建
	,GBIME_EVENT_LAYOUT_DESTROY					///<Layout 销毁
	,GBIME_EVENT_LAYOUT_PEN_DOWN				///<Layout 触摸笔按下
	,GBIME_EVENT_LAYOUT_PEN_UP					///<Layout 触摸笔弹起
	,GBIME_EVENT_LAYOUT_PEN_MOVE				///<Layout 触摸笔移动
	,GBIME_EVENT_LAYOUT_REFRESH					///<Layout 刷新
	,GBIME_EVENT_LAYOUT_ACTIVATE   				///<Layout 激活(首次进入编辑框界面时发送)
	,GBIME_EVENT_LAYOUT_DEACTIVATE				///<Layout 注销(断退出编辑框界面时发送)
	,GBIME_EVENT_LAYOUT_HIDE      				///<Layout 隐藏
	,GBIME_EVENT_LAYOUT_SHOW      				///<Layout 显示
	,GBIME_EVENT_LAYOUT_ENTER      				///<Layout 进入
	,GBIME_EVENT_LAYOUT_EXIT      				///<Layout 退出
	,GBIME_EVENT_LAYOUT_SWITCH      			///<Layout 切换
	,GBIME_EVENT_LAYOUT_REDRAW      			///<Layout 重绘

	// 输入法
	,GBIME_EVENT_INPUT_METHOD_SWITCH			///<切换输入法
	,GBIME_EVENT_INPUT_METHOD_SWITCH_NOTIFY		///<通知平台切换输入法（再完成GBIME框架内部的切换逻辑）

	// 配置
	,GBIME_EVENT_CONFIG_LOAD					///<加载配置
	,GBIME_EVENT_CONFIG_MENU					///<进入配置菜单
	,GBIME_EVENT_CONFIG_SET						///<设置配置
	,GBIME_EVENT_CONFIG_GET						///<获取配置
	,GBIME_EVENT_CONFIG_EXIT					///<卸载配置

	// 手写
	,GBIME_EVENT_HANDWRITE_ENGINE_INIT			///<手写引擎初始化
	,GBIME_EVENT_HANDWRITE_ENGINE_EXIT			///<手写引擎退出
	,GBIME_EVENT_HANDWRITE_DRAW_CANDIDATE		///<绘制手写识别出来的候选
	,GBIME_EVENT_HANDWRITE_DRAW_ASSOCWORD		///<绘制联想字符串
	,GBIME_EVENT_HANDWRITE_SET_RECOGNIZE_RANGE	///<设置识别范围

	// 物理按键
	,GBIME_EVENT_KEY_DOWN
	,GBIME_EVENT_KEY_UP
	,GBIME_EVENT_KEY_LONG_PRESS

	,GBIME_EVENT_NUM							///<事件数目
};

/*!
 * \brief 平台事件经过Layout m_pfcPreHandleEvent转换后的目标框架事件分类(通常一类事件会转换成一组相互关联的框架原子事件集合，然后交给框架消息处理接口依次处理)
 */
typedef enum {
	GBIME_CT_KEY_DOWN_REFRESH						///<Layout按钮按下， 仅作刷新
	,GBIME_CT_KEY_UP_REFRESH						///<Layout按钮弹起， 仅作刷新
	,GBIME_CT_NORMAL_KEY_DOWN						///<Layout按钮按下响应常规引擎按键事件，交给GBMMIHandleKeyEvent处理
	,GBIME_CT_NORMAL_KEY_UP							///<Layout按钮弹起响应常规引擎按键事件，交给GBMMIHandleKeyEvent处理
	,GBIME_CT_MOVE      							///<Layout 面板上面滑动
	,GBIME_CT_SWITCH      							///<Layout 切换(有可能是Layout滑屏切换)
	,GBIME_CT_SHOW      							///<Layout 显示（激活）
	,GBIME_CT_HIDE      							///<Layout 隐藏

	// 手写相关
	,GBIME_CT_HANDWRITE_DRAW_CANDIDATE				///<绘制手写识别出来的候选
	,GBIME_CT_HANDWRITE_DRAW_ASSOCWORD				///<绘制联想字符串
	,GBIME_CT_HANDWRITE_SET_RECOGNIZE_RANGE			///<设置识别范围

	// 配置相关
	,GBIME_CT_CONFIG_MENU							///<进入配置菜单界面

	// 物理按键相关
	,GBIME_CT_KEY_DOWN
	,GBIME_CT_KEY_UP
	,GBIME_CT_KEY_LONG_PRESS

	// 物理键盘和虚拟键盘相互切换
	,GBIME_CT_VK_2_KB_SWITCH						///<虚拟键盘 -> 物理键盘
	,GBIME_CT_KB_2_VK_SWITCH						///<物理键盘 -> 虚拟键盘

	,GBIME_CT_NUM
}GBIMEConvertTypeEnum;

typedef struct __tagGBIMEEventHeader
{
	GBUINT32	nType;	///< event type \see GBIMEEventType
	GBUINT32	nEvent; ///< event value
} GBIMEEventHeader;

typedef struct tagEventMap
{
	GBIMEConvertTypeEnum convertType;
	GBIMEEventHeader eventHeader[GBIME_EVT_GROUP_CAPACITY];
	GBU8 eventNum;
}GBIMEEventConvertMap;

/*!
 * \软键盘事件 start [新增新本类事件时，加在本类后面，不要打乱本类内部顺序]
 */
enum PLATFORM_EVENT_ENUM{
	
	GBKEY_VK_A_UPPER = 0x4100       ///<大写字母A
	, GBKEY_VK_B_UPPER                ///<大写字母B
	, GBKEY_VK_C_UPPER                ///<
	, GBKEY_VK_D_UPPER                ///<
	, GBKEY_VK_E_UPPER                ///<
	, GBKEY_VK_F_UPPER                ///<
	, GBKEY_VK_G_UPPER                ///<
	, GBKEY_VK_H_UPPER                ///<
	, GBKEY_VK_I_UPPER                ///<
	, GBKEY_VK_J_UPPER                ///<
	, GBKEY_VK_K_UPPER                ///<
	, GBKEY_VK_L_UPPER                ///<
	, GBKEY_VK_M_UPPER                ///<
	, GBKEY_VK_N_UPPER                ///<
	, GBKEY_VK_O_UPPER                ///<
	, GBKEY_VK_P_UPPER                ///<
	, GBKEY_VK_Q_UPPER                ///<
	, GBKEY_VK_R_UPPER                ///<
	, GBKEY_VK_S_UPPER                ///<
	, GBKEY_VK_T_UPPER                ///<
	, GBKEY_VK_U_UPPER                ///<
	, GBKEY_VK_V_UPPER                ///<
	, GBKEY_VK_W_UPPER                ///<
	, GBKEY_VK_X_UPPER                ///<
	, GBKEY_VK_Y_UPPER                ///<
	, GBKEY_VK_Z_UPPER                ///<

	// 符号键盘符号选择键
	, GBKEY_VK_1_NUMBERKEY              ///<键盘按键1
	, GBKEY_VK_2_NUMBERKEY              ///<键盘按键2
	, GBKEY_VK_3_NUMBERKEY              ///<键盘按键3
	, GBKEY_VK_4_NUMBERKEY              ///<键盘按键4
	, GBKEY_VK_5_NUMBERKEY              ///<键盘按键5
	, GBKEY_VK_6_NUMBERKEY              ///<键盘按键6
	, GBKEY_VK_7_NUMBERKEY              ///<键盘按键7
	, GBKEY_VK_8_NUMBERKEY              ///<键盘按键8
	, GBKEY_VK_9_NUMBERKEY              ///<键盘按键9
	, GBKEY_VK_10_NUMBERKEY             ///<键盘按键10
	, GBKEY_VK_11_NUMBERKEY             ///<键盘按键11
	, GBKEY_VK_12_NUMBERKEY             ///<键盘按键12

	//软键盘事件 end

	//function key  start [新增新本类事件时，加在本类后面，不要打乱本类内部顺序]
	, GBKEY_VK_PUNCTUATION = 0x4200           ///<快捷标点
	, GBKEY_VK_CAP                      ///<大小写切换键
	, GBKEY_VK_SYM_PAGEUP               ///<符号上翻页键
	, GBKEY_VK_SYM_PAGEDOWN             ///<符号下翻页键
	, GBKEY_VK_SBC_DBC_SWICH            ///<全半角切换键
	, GBKEY_VK_SYM_LOCK                 ///<符号锁定键
	, GBKEY_VK_SYM_comment              ///<普通符号
	, GBKEY_VK_SYM_EMOTION              ///<表情符号
	, GBKEY_VK_SYM_SPECIAL              ///<特殊符号
    , GBKEY_VK_SYM_MATH                 ///<数学符号
    , GBKEY_VK_SYM_CANDIDATE_AREA        ///<候选区
    , GBKEY_VK_SYM_INPUT_AREA            ///<输入区
	//handwrite key 
	, GBKEY_VK_V_HANDWRITE_CANDIDATE_AREA            ///<竖屏手写候选区
	, GBKEY_VK_V_HANDWRITE_ASSOCIATE_AREA            ///<竖屏手写联想区
	, GBKEY_VK_V_HANDWRITE_ALL                      ///<竖屏手写全部
	, GBKEY_VK_v_HANDWRITE_ENG                      ///<竖屏手写英文
	, GBKEY_VK_v_HANDWRITE_NUMBER                   ///<竖屏手写数字
	, GBKEY_VK_v_HANDWRITE_TONGYIN                   ///<竖屏手写同音
	, GBKEY_VK_BACKWORD								///<返回键
	, GBKEY_VK_LATEST_KEYBOARD						///<返回最近一个键盘

	//function key  end

	//switch key start [新增新本类事件时，加在本类后面，不要打乱本类内部顺序]
	, GBKEY_VK_V9_PINYIN  = 0x4300      ///<竖屏9键拼音
	, GBKEY_VK_V9_STROKE                ///<竖屏9键笔画
	, GBKEY_VK_V9_ENG_UPPER             ///<竖屏9键大写英文
	, GBKEY_VK_V9_ENG_LOWER             ///<竖屏9键小写英文
	, GBKEY_VK_V9_ABC_UPPER             ///<竖屏9键大写字母
	, GBKEY_VK_V9_ABC_LOWER             ///<竖屏9键小写字母
	, GBKEY_VK_V9_NUM                   ///<竖屏9键数字
	, GBKEY_VK_V9_SYM                   ///<竖屏9键符号
	, GBKEY_VK_V9_FS_HANDWRITE          ///<竖屏全屏手写
	, GBKEY_VK_V9_BOX_HANDWRITE         ///<竖屏方格手写
	, GBKEY_VK_SETTING                  ///<设置

	, GBKEY_VK_V26_PINYIN               ///<竖屏26键拼音
	, GBKEY_VK_V26_STROKE               ///<竖屏26键笔画
	, GBKEY_VK_V26_ENG_UPPER            ///<竖屏26键大写英文
	, GBKEY_VK_V26_ENG_LOWER            ///<竖屏26键小写英文
	, GBKEY_VK_V26_ABC_UPPER            ///<竖屏26键大写字母
	, GBKEY_VK_V26_ABC_LOWER            ///<竖屏26键小写字母
	, GBKEY_VK_V26_NUM                  ///<竖屏26键数字
	, GBKEY_VK_V26_SYM                  ///<竖屏26键符号

	, GBKEY_VK_H9_PINYIN                ///<横屏9键拼音
	, GBKEY_VK_H9_STROKE                ///<横屏9键笔画
	, GBKEY_VK_H9_ENG_UPPER             ///<横屏9键大写英文
	, GBKEY_VK_H9_ENG_LOWER             ///<横屏9键小写英文
	, GBKEY_VK_H9_ABC_UPPER             ///<横屏9键大写字母
	, GBKEY_VK_H9_ABC_LOWER             ///<横屏9键小写字母
	, GBKEY_VK_H9_NUM                   ///<横屏9键数字
	, GBKEY_VK_H9_SYM                   ///<横屏9键符号
	, GBKEY_VK_H9_FS_HANDWRITE          ///<横屏全屏手写
	, GBKEY_VK_H9_BOX_HANDWRITE         ///<横屏方格手写

	, GBKEY_VK_H26_PINYIN               ///<横屏26键拼音
	, GBKEY_VK_H26_STROKE               ///<横屏26键笔画
	, GBKEY_VK_H26_ENG_UPPER            ///<横屏26键大写英文
	, GBKEY_VK_H26_ENG_LOWER            ///<横屏26键小写英文
	, GBKEY_VK_H26_ABC_UPPER            ///<横屏26键大写字母
	, GBKEY_VK_H26_ABC_LOWER            ///<横屏26键小写字母
	, GBKEY_VK_H26_NUM                  ///<横屏26键数字
	, GBKEY_VK_H26_SYM                  ///<横屏26键符号

	//switch key end

	//数字9键模式的几个符号 start [新增新本类事件时，加在本类后面，不要打乱本类内部顺序]
	, GBKEY_VK_V9_SYMBOL_ADD = 0x4400	///<数字输入模式"+"号
	, GBKEY_VK_V9_SYMBOL_AT				///<数字输入模式"@"号
	, GBKEY_VK_V9_SYMBOL_DOLL			///<数字输入模式"$"号
	, GBKEY_VK_V9_SYMBOL_PERCENT        ///<数字输入模式"%"号s
	, GBKEY_VK_V9_SYMBOL_POINT			///<数字输入模式"."号
	, GBKEY_VK_V9_SYMBOL_SUBTRACT		///<数字输入模式"-"号
	, GBKEY_VK_V9_SYMBOL_COMMA			///<数字输入模式","号
	, GBKEY_VK_V9_PINYIN_SEPARATOR		///<拼音分隔符号"'"
	, GBKEY_VK_V9_SYMBOL_FULL_STOP		///<句号
	, GBKEY_VK_V9_SYMBOL_KEY			///<符号键,该键值比较特殊，对应多个符号，程序根据按下该按键的次数据来取得其对应的键值，
										///<键值保存在BUTTONCONSTDATA(关于该结构请参考:CLayoutInfo.h)结构的btKeyName数组成员中
	, GBKEY_VK_V9_SYMBOL_FACE_MULTIP_KEY  ///<表情符号(多键值)

	//数字9键模式的几个符号 end

	// 多语言新添加键值
	, GBKEY_VK_V9_UPPERCASE_FRENCH = 0x4500///<竖屏9键大写法文
	, GBKEY_VK_V9_LOWERCASE_FRENCH		///<竖屏9键小写法文
	, GBKEY_VK_V9_UPPERCASE_FRENCH_ABC	///<竖屏9键大写法文字母
	, GBKEY_VK_V9_LOWERCASE_FRENCH_ABC	///<竖屏9键小写法文字母
	
	, GBKEY_VK_V9_UPPERCASE_GERMAN		///<竖屏9键大写德文
	, GBKEY_VK_V9_LOWERCASE_GERMAN		///<竖屏9键小写德文
	, GBKEY_VK_V9_UPPERCASE_GERMAN_ABC	///<竖屏9键大写德文字母
	, GBKEY_VK_V9_LOWERCASE_GERMAN_ABC	///<竖屏9键小写德文字母
	
	, GBKEY_VK_V9_UPPERCASE_SPANISH		///<竖屏9键大写西班牙文
	, GBKEY_VK_V9_LOWERCASE_SPANISH		///<竖屏9键小写西班牙文
	, GBKEY_VK_V9_UPPERCASE_SPANISH_ABC	///<竖屏9键大写西班牙文字母
	, GBKEY_VK_V9_LOWERCASE_SPANISH_ABC	///<竖屏9键小写西班牙文字母
	
	, GBKEY_VK_V26_UPPERCASE_FRENCH		///<竖屏26键大写法文
	, GBKEY_VK_V26_LOWERCASE_FRENCH		///<竖屏26键小写法文
	, GBKEY_VK_V26_UPPERCASE_FRENCH_ABC	///<竖屏26键大写法文字母
	, GBKEY_VK_V26_LOWERCASE_FRENCH_ABC	///<竖屏26键小写法文字母
	
	, GBKEY_VK_V26_UPPERCASE_GERMAN		///<竖屏26键大写德文
	, GBKEY_VK_V26_LOWERCASE_GERMAN		///<竖屏26键小写德文
	, GBKEY_VK_V26_UPPERCASE_GERMAN_ABC	///<竖屏26键大写德文字母
	, GBKEY_VK_V26_LOWERCASE_GERMAN_ABC	///<竖屏26键小写德文字母
	
	, GBKEY_VK_V26_UPPERCASE_SPANISH	///<竖屏26键大写西班牙文
	, GBKEY_VK_V26_LOWERCASE_SPANISH	///<竖屏26键小写西班牙文	
	, GBKEY_VK_V26_UPPERCASE_SPANISH_ABC///<竖屏26键大写西班牙文字母
	, GBKEY_VK_V26_LOWERCASE_SPANISH_ABC///<竖屏26键小写西班牙文字母
	
	, GBKEY_VK_H9_UPPERCASE_FRENCH		///<横屏9键大写法文
	, GBKEY_VK_H9_LOWERCASE_FRENCH		///<横屏9键小写法文
	, GBKEY_VK_H9_UPPERCASE_FRENCH_ABC	///<横屏9键大写法文字母
	, GBKEY_VK_H9_LOWERCASE_FRENCH_ABC	///<横屏9键小写法文字母
	
	, GBKEY_VK_H9_UPPERCASE_GERMAN		///<横屏9键大写德文
	, GBKEY_VK_H9_LOWERCASE_GERMAN		///<横屏9键小写德文
	, GBKEY_VK_H9_UPPERCASE_GERMAN_ABC	///<横屏9键大写德文字母
	, GBKEY_VK_H9_LOWERCASE_GERMAN_ABC	///<横屏9键小写德文字母
	
	, GBKEY_VK_H9_UPPERCASE_SPANISH		///<横屏9键大写西班牙文
	, GBKEY_VK_H9_LOWERCASE_SPANISH		///<横屏9键小写西班牙文
	, GBKEY_VK_H9_UPPERCASE_SPANISH_ABC	///<横屏9键大写西班牙文字母
	, GBKEY_VK_H9_LOWERCASE_SPANISH_ABC	///<横屏9键小写西班牙文字母
	
	, GBKEY_VK_H26_UPPERCASE_FRENCH		///<横屏26键大写法文
	, GBKEY_VK_H26_LOWERCASE_FRENCH		///<横屏26键小写法文
	, GBKEY_VK_H26_UPPERCASE_FRENCH_ABC	///<横屏26键大写法文字母
	, GBKEY_VK_H26_LOWERCASE_FRENCH_ABC	///<横屏26键小写法文字母
	
	, GBKEY_VK_H26_UPPERCASE_GERMAN	///<横屏26键大写德文
	, GBKEY_VK_H26_LOWERCASE_GERMAN		///<横屏26键小写德文
	, GBKEY_VK_H26_UPPERCASE_GERMAN_ABC	///<横屏26键大写德文字母
	, GBKEY_VK_H26_LOWERCASE_GERMAN_ABC	///<横屏26键小写德文字母
	
	, GBKEY_VK_H26_UPPERCASE_SPANISH	///<横屏26键大写西班牙文
	, GBKEY_VK_H26_LOWERCASE_SPANISH	///<横屏26键小写西班牙文
	, GBKEY_VK_H26_UPPERCASE_SPANISH_ABC///<横屏26键大写西班牙文字母
	, GBKEY_VK_H26_LOWERCASE_SPANISH_ABC///<横屏26键小写西班牙文字母
	
	, GBKEY_VK_V_HANDWRITING_FRENCH		///<竖屏手写法文
	, GBKEY_VK_V_HANDWRITING_GERMAN		///<竖屏手写德文
	, GBKEY_VK_V_HANDWRITING_SPANISH	///<竖屏手写西班牙文
	
	, GBKEY_VK_H_HANDWRITING_FRENCH		///<横屏手写法文
	, GBKEY_VK_H_HANDWRITING_GERMAN		///<横屏手写德文
	, GBKEY_VK_H_HANDWRITING_SPANISH	///<横屏手写西班牙文
	
	, GBKEY_VK_SMART_ASSOCIATE			///<智能联想键
	, GBKEY_VK_CYCLE_PAGE_TURNING		///<循环翻页键
	, GBKEY_VK_LANG_SWITCH				///<语言切换键
	
}; //enum PLATFORM_EVENT_ENUM

#define LAYOUTVK_IS_NUM_KEY(key) ((key) >= GBKEY_0 && (key) <= GBKEY_9)
#define IS_ENGLISH_CHAR(ch)	 ((ch) >= 'a' && (ch) <= 'z' || (ch) >= 'A' && (ch) <= 'Z')

#endif //__GB_PLATFORM_DEFINE_