#include "CConfigData.h"
#include "CProductSetting.h"
#include "GBIME.h"


const GBProductData productData[] = 
{
///产品1
	{
		PRODUCE_NUMPAD_1
		,L"Number+中文(词语+二行)+Alp(二行)"
		,L"PRODUCE_NUMPAD_1.config"
		,{GBCL_ROW_KEYBD, 1, GBCL_ROW_KEYBD, 1}
		,{
			{GBCFG_PINYIN_FUZZY_H_F,	GB_ENGINE_CHINESE,	0,	0}	//f = h
			,{GBCFG_PINYIN_FUZZY_K_G,	GB_ENGINE_CHINESE,	0,	0}	//k = g
			,{GBCFG_PINYIN_FUZZY_N_L,	GB_ENGINE_CHINESE,	0,	0}	//n = l
			,{GBCFG_PINYIN_FUZZY_R_L,	GB_ENGINE_CHINESE,	0,	0}	//r = l
			,{GBCFG_PINYIN_FUZZY_NG_N,	GB_ENGINE_CHINESE,	0,	0}	//en=eng an=ang in=ing
			,{GBCFG_PINYIN_FUZZY_XH_X,	GB_ENGINE_CHINESE,	0,	0}	//s=sh c=ch z=zh
			,{GBCFG_OUT_ZI_SYLLABLE,	GB_ENGINE_CHINESE,	0,	0}	//chn-只输出字的拼音
			,{GBCFG_TRANSLATE_INPUT_STRING,	GB_ENGINE_CHINESE,	0,	0}	//chn-将输入字符串转为拼音串
			,{GBCFG_PY_1_MATCH_FUNC,	GB_ENGINE_CHINESE,	0,	0}	//chn-拼音精确匹配
			,{GBCFG_ADD_TRANSLATE_INPUT_STRING_TO_SYLLABLE,	GB_ENGINE_CHINESE,	0,	0}	//chn-将转换后的字符串加入拼音音节列表
			,{GBCFG_CLOSE_PHRASE,	GB_ENGINE_CHINESE,	0,	0}	//chn-关闭词语功能
			,{GBCFG_ONLY_ENTIRE_PHRASE_CAND,	GB_ENGINE_CHINESE,	0,	0}	//chn-只匹配完整的词语,并匹配可能的字
			,{GBCFG_CLOSE_ASSOC_PHRASE,	GB_ENGINE_CHINESE,	0,	0}	//chn-关闭词语联想
			,{GBCFG_FIND_ONLY_FULL_MATCHING,	GB_ENGINE_CHINESE,	0,	0}	//chn-只匹配完整的词，并且不匹配可能的字
			,{GBCFG_ASSOCIATE_INTEPCT,	GB_ENGINE_CHINESE,	1,	1}	//chn-支持智能标点预测
			,{GBCFG_ASSOC_NOT_FILL_HIGHEST_FREQ_WORD,	GB_ENGINE_CHINESE,	0,	0}	//chn-联想的时候不填充高频字
			,{GBCFG_SET_PHRASE_SPLIT_CHAR,	GB_ENGINE_CHINESE,	0,	0}	//chn-设置分隔符号的显示字符
			,{GBCFG_SET_PHRASE_LEN_LIMIT,	GB_ENGINE_CHINESE,	0,	0}	//设置词语的最大长度限制
			,{GBCFG_CLOSE_COMPONENT,	GB_ENGINE_CHINESE,	0,	0}	//chn-关闭部首功能
			,{GBCFG_INIT_GET_HIGHEST_FREQ_WORD,	GB_ENGINE_CHINESE,	0,	0}	//chn-获取默认高频字
			,{GBCFG_NOT_ALLOW_USELESS_STROKE,	GB_ENGINE_CHINESE,	1,	1}	//chn-笔画和韩文输入法下，输入超长串后不允许出来候选
			,{GBCFG_SET_STROKE_WILDCHAR_SHOW,	GB_ENGINE_CHINESE,	0,	0}	//设置笔画通配符符号
			,{GBCFG_CHARSET_SET,	GB_ENGINE_CHINESE,	1,	0}	//设置字符集 
			,{GBALP_AUTO_CORRECTION,	GB_ENGINE_ALPHABETIC,	0,	0}	//alp-打开Explict/Qwerty智能纠错功能
			,{GBALP_CLOSE_AUTO_COMPLETION,	GB_ENGINE_ALPHABETIC,	0,	0}	//alp-关闭number键盘以及half-qwert键盘自动完成功能
			,{GBALP_INPUTSTRING_IS_FIRST_CANDIDATE,	GB_ENGINE_ALPHABETIC,	0,	0}	//alp-Explicit输入串作为第一个候选
			,{GBALP_QWERTY_OUTPUT_DEFAULT_STRING,	GB_ENGINE_ALPHABETIC,	0,	0}	//alp-无候选匹配时，Qwerty输入串对应的默认字符作为第一个候选 
			,{GBALP_FIRST_KEY_ASSOC,	GB_ENGINE_ALPHABETIC,	0,	0}	//alp-Explicit模式和Qwerty每个键开始自动完成
			,{GBALP_ASSOC_NO_DERIVE,	GB_ENGINE_ALPHABETIC,	1,	1}	//alp-不输出派生词
			,{GBALP_KEEP_ALL_HEADER,	GB_ENGINE_ALPHABETIC,	0,	0}	//alp-获取不是单词的header
			,{GBALP_SET_AVOID_PATENT_SYMBOL,	GB_ENGINE_ALPHABETIC,	0,	0}	//alp-设置规避专利所使用的符号
			,{GBALP_SET_EMBEDDED_INTERPUNCTION_KEY,	GB_ENGINE_ALPHABETIC,	0,	0}	//alp-设置外文输入时所使用的智能标点按键
			,{GBFO_Chn_All_Num_Can_Continuously_Input,	GB_ENGINE_FUNCTION,	1,	1}	//chn-中文数字键盘支持0-9按键连续输入
			,{GBFO_Alp_All_Num_Can_Continuously_Input,	GB_ENGINE_FUNCTION,	1,	1}	//alf-英文数字键盘支持0-9按键连续输入
			,{GBFO_Chn_Display_Input_Key_Sequence,	GB_ENGINE_FUNCTION,	1,	1}	//中文输入区中显示数字
			,{GBFO_Alp_Up_To_Input_Key_Sequence,	GB_ENGINE_FUNCTION,	1,	1}	//alp-打开按上键上屏功能
			,{GBFO_Chn_Up_To_Input_Key_Sequence,	GB_ENGINE_FUNCTION,	1,	1}	//chn-打开按上键上屏功能
			,{GBFO_Chn_No_Response_When_Key_No_Cand,	GB_ENGINE_FUNCTION,	0,	0}	//chn-无候选时，屏蔽按键输入
			,{GBFO_Alp_No_Response_When_Key_No_Cand,	GB_ENGINE_FUNCTION,	0,	0}	//alp-无候选时，屏蔽按键输入
			,{GBFO_Alp_Add_Space_After_Cand_Confirmed,	GB_ENGINE_FUNCTION,	0,	0}	//alp-上屏一个候选后，自动添加空格
			,{GBFO_Alp_UAW,	GB_ENGINE_FUNCTION,	1,	1}	//alp-支持添加自造词
			,{GBFO_OK_Enter_Selected,	GB_ENGINE_SYNTAX,	0,	0}	//chn-整句拼音输入法:在没有进入候选状态下面，OK键进入选择状态
			,{GBFO_Chn_Syntex_Key_Sequence_Edit,	GB_ENGINE_SYNTAX,	0,	0}	//chn-整句拼音输入法: 支持按键序列修改
			,{GBFO_Chn_Switch_Alp,	GB_ENGINE_FUNCTION,	1,	1}	//chn-在中文输入法中，使用中英切换的功能
			,{GBFO_API_Stop_Log,	GB_ENGINE_FUNCTION,	1,	1}	//临时关闭Log功能
			,{GBFO_Alp_UAW_Confirm,	GB_ENGINE_FUNCTION,	0,	0}	//alp-在Alp Predictive/HalfQwerty 模式下打开自造词的确认流程 
			,{GBFO_Multi_Tap_Show_Cand_Wnd,	GB_ENGINE_FUNCTION,	1,	0}	//alp-在MultiTap状态下面仍然打开候选窗口
			,{GBFO_Scroll_Cand_By_Row,	GB_ENGINE_FUNCTION,	1,	1}	//一行一行的对候选进行翻页
			,{GBFO_Tch_Scr,	GB_ENGINE_FUNCTION,	0,	0}	//alp-触摸屏输入下，不提供候选数字编号
			,{GBFO_Alp_Paging_By_Lower,	GB_ENGINE_FUNCTION,	0,	0}	//alp-按照小写候选排序进行分页
		}
	}
///产品2
	,{
		PRODUCE_NUMPAD_11
		,L"Number+传统中文(单字+二行)+Alp(inline)"
		,L"PRODUCE_NUMPAD_11.config"
		,{GBCL_ROW_KEYBD, 1, GBCL_SINGLE_CAND, 1}
		,{
			{GBCFG_PINYIN_FUZZY_H_F,	GB_ENGINE_CHINESE,	0,	0}	//f = h
			,{GBCFG_PINYIN_FUZZY_K_G,	GB_ENGINE_CHINESE,	0,	0}	//k = g
			,{GBCFG_PINYIN_FUZZY_N_L,	GB_ENGINE_CHINESE,	0,	0}	//n = l
			,{GBCFG_PINYIN_FUZZY_R_L,	GB_ENGINE_CHINESE,	0,	0}	//r = l
			,{GBCFG_PINYIN_FUZZY_NG_N,	GB_ENGINE_CHINESE,	0,	0}	//en=eng an=ang in=ing
			,{GBCFG_PINYIN_FUZZY_XH_X,	GB_ENGINE_CHINESE,	0,	0}	//s=sh c=ch z=zh
			,{GBCFG_OUT_ZI_SYLLABLE,	GB_ENGINE_CHINESE,	0,	0}	//chn-只输出字的拼音
			,{GBCFG_TRANSLATE_INPUT_STRING,	GB_ENGINE_CHINESE,	0,	0}	//chn-将输入字符串转为拼音串
			,{GBCFG_PY_1_MATCH_FUNC,	GB_ENGINE_CHINESE,	0,	0}	//chn-拼音精确匹配
			,{GBCFG_ADD_TRANSLATE_INPUT_STRING_TO_SYLLABLE,	GB_ENGINE_CHINESE,	0,	0}	//chn-将转换后的字符串加入拼音音节列表
			,{GBCFG_CLOSE_PHRASE,	GB_ENGINE_CHINESE,	0,	0}	//chn-关闭词语功能
			,{GBCFG_ONLY_ENTIRE_PHRASE_CAND,	GB_ENGINE_CHINESE,	0,	0}	//chn-只匹配完整的词语,并匹配可能的字
			,{GBCFG_CLOSE_ASSOC_PHRASE,	GB_ENGINE_CHINESE,	0,	0}	//chn-关闭词语联想
			,{GBCFG_FIND_ONLY_FULL_MATCHING,	GB_ENGINE_CHINESE,	0,	0}	//chn-只匹配完整的词，并且不匹配可能的字
			,{GBCFG_ASSOCIATE_INTEPCT,	GB_ENGINE_CHINESE,	1,	1}	//chn-支持智能标点预测
			,{GBCFG_ASSOC_NOT_FILL_HIGHEST_FREQ_WORD,	GB_ENGINE_CHINESE,	0,	0}	//chn-联想的时候不填充高频字
			,{GBCFG_SET_PHRASE_SPLIT_CHAR,	GB_ENGINE_CHINESE,	0,	0}	//chn-设置分隔符号的显示字符
			,{GBCFG_SET_PHRASE_LEN_LIMIT,	GB_ENGINE_CHINESE,	0,	0}	//设置词语的最大长度限制
			,{GBCFG_CLOSE_COMPONENT,	GB_ENGINE_CHINESE,	0,	0}	//chn-关闭部首功能
			,{GBCFG_INIT_GET_HIGHEST_FREQ_WORD,	GB_ENGINE_CHINESE,	0,	0}	//chn-获取默认高频字
			,{GBCFG_NOT_ALLOW_USELESS_STROKE,	GB_ENGINE_CHINESE,	1,	1}	//chn-笔画和韩文输入法下，输入超长串后不允许出来候选
			,{GBCFG_SET_STROKE_WILDCHAR_SHOW,	GB_ENGINE_CHINESE,	0,	0}	//设置笔画通配符符号
			,{GBCFG_CHARSET_SET,	GB_ENGINE_CHINESE,	1,	0}	//设置字符集 
			,{GBALP_AUTO_CORRECTION,	GB_ENGINE_ALPHABETIC,	0,	0}	//alp-打开Explict/Qwerty智能纠错功能
			,{GBALP_CLOSE_AUTO_COMPLETION,	GB_ENGINE_ALPHABETIC,	0,	0}	//alp-关闭number键盘以及half-qwert键盘自动完成功能
			,{GBALP_INPUTSTRING_IS_FIRST_CANDIDATE,	GB_ENGINE_ALPHABETIC,	0,	0}	//alp-Explicit输入串作为第一个候选
			,{GBALP_QWERTY_OUTPUT_DEFAULT_STRING,	GB_ENGINE_ALPHABETIC,	0,	0}	//alp-无候选匹配时，Qwerty输入串对应的默认字符作为第一个候选 
			,{GBALP_FIRST_KEY_ASSOC,	GB_ENGINE_ALPHABETIC,	0,	0}	//alp-Explicit模式和Qwerty每个键开始自动完成
			,{GBALP_ASSOC_NO_DERIVE,	GB_ENGINE_ALPHABETIC,	1,	1}	//alp-不输出派生词
			,{GBALP_KEEP_ALL_HEADER,	GB_ENGINE_ALPHABETIC,	0,	0}	//alp-获取不是单词的header
			,{GBALP_SET_AVOID_PATENT_SYMBOL,	GB_ENGINE_ALPHABETIC,	0,	0}	//alp-设置规避专利所使用的符号
			,{GBALP_SET_EMBEDDED_INTERPUNCTION_KEY,	GB_ENGINE_ALPHABETIC,	0,	0}	//alp-设置外文输入时所使用的智能标点按键
			,{GBFO_Chn_All_Num_Can_Continuously_Input,	GB_ENGINE_FUNCTION,	1,	1}	//chn-中文数字键盘支持0-9按键连续输入
			,{GBFO_Alp_All_Num_Can_Continuously_Input,	GB_ENGINE_FUNCTION,	1,	1}	//alf-英文数字键盘支持0-9按键连续输入
			,{GBFO_Chn_Display_Input_Key_Sequence,	GB_ENGINE_FUNCTION,	1,	1}	//中文输入区中显示数字
			,{GBFO_Alp_Up_To_Input_Key_Sequence,	GB_ENGINE_FUNCTION,	0,	0}	//alp-打开按上键上屏功能
			,{GBFO_Chn_Up_To_Input_Key_Sequence,	GB_ENGINE_FUNCTION,	0,	0}	//chn-打开按上键上屏功能
			,{GBFO_Chn_No_Response_When_Key_No_Cand,	GB_ENGINE_FUNCTION,	0,	0}	//chn-无候选时，屏蔽按键输入
			,{GBFO_Alp_No_Response_When_Key_No_Cand,	GB_ENGINE_FUNCTION,	0,	0}	//alp-无候选时，屏蔽按键输入
			,{GBFO_Alp_Add_Space_After_Cand_Confirmed,	GB_ENGINE_FUNCTION,	0,	0}	//alp-上屏一个候选后，自动添加空格
			,{GBFO_Alp_UAW,	GB_ENGINE_FUNCTION,	1,	1}	//alp-支持添加自造词
			,{GBFO_OK_Enter_Selected,	GB_ENGINE_SYNTAX,	0,	0}	//chn-整句拼音输入法:在没有进入候选状态下面，OK键进入选择状态
			,{GBFO_Chn_Syntex_Key_Sequence_Edit,	GB_ENGINE_SYNTAX,	0,	0}	//chn-整句拼音输入法: 支持按键序列修改
			,{GBFO_Chn_Switch_Alp,	GB_ENGINE_FUNCTION,	0,	0}	//chn-在中文输入法中，使用中英切换的功能
			,{GBFO_API_Stop_Log,	GB_ENGINE_FUNCTION,	1,	1}	//临时关闭Log功能
			,{GBFO_Alp_UAW_Confirm,	GB_ENGINE_FUNCTION,	0,	0}	//alp-在Alp Predictive/HalfQwerty 模式下打开自造词的确认流程 
			,{GBFO_Multi_Tap_Show_Cand_Wnd,	GB_ENGINE_FUNCTION,	1,	0}	//alp-在MultiTap状态下面仍然打开候选窗口
			,{GBFO_Scroll_Cand_By_Row,	GB_ENGINE_FUNCTION,	1,	1}	//一行一行的对候选进行翻页
			,{GBFO_Tch_Scr,	GB_ENGINE_FUNCTION,	0,	0}	//alp-触摸屏输入下，不提供候选数字编号
			,{GBFO_Alp_Paging_By_Lower,	GB_ENGINE_FUNCTION,	0,	0}	//alp-按照小写候选排序进行分页
		}
	}
///产品3
	,{
		PRODUCE_EXPLICIT_1
		,L"Explicit+中文(词语+二行)+Alp(二行)"
		,L"PRODUCE_EXPLICIT_1.config"
		,{GBCL_ROW_KEYBD, 1, GBCL_ROW_KEYBD, 1}
		,{
			{GBCFG_PINYIN_FUZZY_H_F,	GB_ENGINE_CHINESE,	0,	0}	//f = h
			,{GBCFG_PINYIN_FUZZY_K_G,	GB_ENGINE_CHINESE,	0,	0}	//k = g
			,{GBCFG_PINYIN_FUZZY_N_L,	GB_ENGINE_CHINESE,	0,	0}	//n = l
			,{GBCFG_PINYIN_FUZZY_R_L,	GB_ENGINE_CHINESE,	0,	0}	//r = l
			,{GBCFG_PINYIN_FUZZY_NG_N,	GB_ENGINE_CHINESE,	0,	0}	//en=eng an=ang in=ing
			,{GBCFG_PINYIN_FUZZY_XH_X,	GB_ENGINE_CHINESE,	0,	0}	//s=sh c=ch z=zh
			,{GBCFG_OUT_ZI_SYLLABLE,	GB_ENGINE_CHINESE,	0,	0}	//chn-只输出字的拼音
			,{GBCFG_TRANSLATE_INPUT_STRING,	GB_ENGINE_CHINESE,	0,	0}	//chn-将输入字符串转为拼音串
			,{GBCFG_PY_1_MATCH_FUNC,	GB_ENGINE_CHINESE,	0,	0}	//chn-拼音精确匹配
			,{GBCFG_ADD_TRANSLATE_INPUT_STRING_TO_SYLLABLE,	GB_ENGINE_CHINESE,	0,	0}	//chn-将转换后的字符串加入拼音音节列表
			,{GBCFG_CLOSE_PHRASE,	GB_ENGINE_CHINESE,	0,	0}	//chn-关闭词语功能
			,{GBCFG_ONLY_ENTIRE_PHRASE_CAND,	GB_ENGINE_CHINESE,	0,	0}	//chn-只匹配完整的词语,并匹配可能的字
			,{GBCFG_CLOSE_ASSOC_PHRASE,	GB_ENGINE_CHINESE,	0,	0}	//chn-关闭词语联想
			,{GBCFG_FIND_ONLY_FULL_MATCHING,	GB_ENGINE_CHINESE,	0,	0}	//chn-只匹配完整的词，并且不匹配可能的字
			,{GBCFG_ASSOCIATE_INTEPCT,	GB_ENGINE_CHINESE,	1,	1}	//chn-支持智能标点预测
			,{GBCFG_ASSOC_NOT_FILL_HIGHEST_FREQ_WORD,	GB_ENGINE_CHINESE,	0,	0}	//chn-联想的时候不填充高频字
			,{GBCFG_SET_PHRASE_SPLIT_CHAR,	GB_ENGINE_CHINESE,	0,	0}	//chn-设置分隔符号的显示字符
			,{GBCFG_SET_PHRASE_LEN_LIMIT,	GB_ENGINE_CHINESE,	0,	0}	//设置词语的最大长度限制
			,{GBCFG_CLOSE_COMPONENT,	GB_ENGINE_CHINESE,	0,	0}	//chn-关闭部首功能
			,{GBCFG_INIT_GET_HIGHEST_FREQ_WORD,	GB_ENGINE_CHINESE,	0,	0}	//chn-获取默认高频字
			,{GBCFG_NOT_ALLOW_USELESS_STROKE,	GB_ENGINE_CHINESE,	1,	1}	//chn-笔画和韩文输入法下，输入超长串后不允许出来候选
			,{GBCFG_SET_STROKE_WILDCHAR_SHOW,	GB_ENGINE_CHINESE,	0,	0}	//设置笔画通配符符号
			,{GBCFG_CHARSET_SET,	GB_ENGINE_CHINESE,	1,	0}	//设置字符集 
			,{GBALP_AUTO_CORRECTION,	GB_ENGINE_ALPHABETIC,	0,	0}	//alp-打开Explict/Qwerty智能纠错功能
			,{GBALP_CLOSE_AUTO_COMPLETION,	GB_ENGINE_ALPHABETIC,	0,	0}	//alp-关闭number键盘以及half-qwert键盘自动完成功能
			,{GBALP_INPUTSTRING_IS_FIRST_CANDIDATE,	GB_ENGINE_ALPHABETIC,	1,	0}	//alp-Explicit输入串作为第一个候选
			,{GBALP_QWERTY_OUTPUT_DEFAULT_STRING,	GB_ENGINE_ALPHABETIC,	0,	0}	//alp-无候选匹配时，Qwerty输入串对应的默认字符作为第一个候选 
			,{GBALP_FIRST_KEY_ASSOC,	GB_ENGINE_ALPHABETIC,	1,	0}	//alp-Explicit模式和Qwerty每个键开始自动完成
			,{GBALP_ASSOC_NO_DERIVE,	GB_ENGINE_ALPHABETIC,	1,	0}	//alp-不输出派生词
			,{GBALP_KEEP_ALL_HEADER,	GB_ENGINE_ALPHABETIC,	0,	0}	//alp-获取不是单词的header
			,{GBALP_SET_AVOID_PATENT_SYMBOL,	GB_ENGINE_ALPHABETIC,	0,	0}	//alp-设置规避专利所使用的符号
			,{GBALP_SET_EMBEDDED_INTERPUNCTION_KEY,	GB_ENGINE_ALPHABETIC,	GBIME_EMBEDDED_INTERPUNCTION_KEY,	0}	//alp-设置外文输入时所使用的智能标点按键
			,{GBFO_Chn_All_Num_Can_Continuously_Input,	GB_ENGINE_FUNCTION,	0,	0}	//chn-中文数字键盘支持0-9按键连续输入
			,{GBFO_Alp_All_Num_Can_Continuously_Input,	GB_ENGINE_FUNCTION,	0,	0}	//alf-英文数字键盘支持0-9按键连续输入
			,{GBFO_Chn_Display_Input_Key_Sequence,	GB_ENGINE_FUNCTION,	0,	0}	//中文输入区中显示数字
			,{GBFO_Alp_Up_To_Input_Key_Sequence,	GB_ENGINE_FUNCTION,	1,	1}	//alp-打开按上键上屏功能
			,{GBFO_Chn_Up_To_Input_Key_Sequence,	GB_ENGINE_FUNCTION,	1,	1}	//chn-打开按上键上屏功能
			,{GBFO_Chn_No_Response_When_Key_No_Cand,	GB_ENGINE_FUNCTION,	0,	0}	//chn-无候选时，屏蔽按键输入
			,{GBFO_Alp_No_Response_When_Key_No_Cand,	GB_ENGINE_FUNCTION,	0,	0}	//alp-无候选时，屏蔽按键输入
			,{GBFO_Alp_Add_Space_After_Cand_Confirmed,	GB_ENGINE_FUNCTION,	0,	0}	//alp-上屏一个候选后，自动添加空格
			,{GBFO_Alp_UAW,	GB_ENGINE_FUNCTION,	1,	1}	//alp-支持添加自造词
			,{GBFO_OK_Enter_Selected,	GB_ENGINE_SYNTAX,	0,	0}	//chn-整句拼音输入法:在没有进入候选状态下面，OK键进入选择状态
			,{GBFO_Chn_Syntex_Key_Sequence_Edit,	GB_ENGINE_SYNTAX,	0,	0}	//chn-整句拼音输入法: 支持按键序列修改
			,{GBFO_Chn_Switch_Alp,	GB_ENGINE_FUNCTION,	0,	0}	//chn-在中文输入法中，使用中英切换的功能
			,{GBFO_API_Stop_Log,	GB_ENGINE_FUNCTION,	1,	1}	//临时关闭Log功能
			,{GBFO_Alp_UAW_Confirm,	GB_ENGINE_FUNCTION,	0,	0}	//alp-在Alp Predictive/HalfQwerty 模式下打开自造词的确认流程 
			,{GBFO_Multi_Tap_Show_Cand_Wnd,	GB_ENGINE_FUNCTION,	1,	0}	//alp-在MultiTap状态下面仍然打开候选窗口
			,{GBFO_Scroll_Cand_By_Row,	GB_ENGINE_FUNCTION,	1,	1}	//一行一行的对候选进行翻页
			,{GBFO_Tch_Scr,	GB_ENGINE_FUNCTION,	0,	0}	//alp-触摸屏输入下，不提供候选数字编号
			,{GBFO_Alp_Paging_By_Lower,	GB_ENGINE_FUNCTION,	0,	0}	//alp-按照小写候选排序进行分页
		}
	}
///产品4
	,{
		PRODUCE_EXPLICIT_5
		,L"Explicit+触摸屏 中文(词语+二行)+Alp(二行)"
		,L"PRODUCE_EXPLICIT_5.config"
		,{GBCL_ROW_KEYBD, 1, GBCL_ROW_KEYBD, 1}
		,{
			{GBCFG_PINYIN_FUZZY_H_F,	GB_ENGINE_CHINESE,	0,	0}	//f = h
			,{GBCFG_PINYIN_FUZZY_K_G,	GB_ENGINE_CHINESE,	0,	0}	//k = g
			,{GBCFG_PINYIN_FUZZY_N_L,	GB_ENGINE_CHINESE,	0,	0}	//n = l
			,{GBCFG_PINYIN_FUZZY_R_L,	GB_ENGINE_CHINESE,	0,	0}	//r = l
			,{GBCFG_PINYIN_FUZZY_NG_N,	GB_ENGINE_CHINESE,	0,	0}	//en=eng an=ang in=ing
			,{GBCFG_PINYIN_FUZZY_XH_X,	GB_ENGINE_CHINESE,	0,	0}	//s=sh c=ch z=zh
			,{GBCFG_OUT_ZI_SYLLABLE,	GB_ENGINE_CHINESE,	0,	0}	//chn-只输出字的拼音
			,{GBCFG_TRANSLATE_INPUT_STRING,	GB_ENGINE_CHINESE,	0,	0}	//chn-将输入字符串转为拼音串
			,{GBCFG_PY_1_MATCH_FUNC,	GB_ENGINE_CHINESE,	0,	0}	//chn-拼音精确匹配
			,{GBCFG_ADD_TRANSLATE_INPUT_STRING_TO_SYLLABLE,	GB_ENGINE_CHINESE,	0,	0}	//chn-将转换后的字符串加入拼音音节列表
			,{GBCFG_CLOSE_PHRASE,	GB_ENGINE_CHINESE,	0,	0}	//chn-关闭词语功能
			,{GBCFG_ONLY_ENTIRE_PHRASE_CAND,	GB_ENGINE_CHINESE,	0,	0}	//chn-只匹配完整的词语,并匹配可能的字
			,{GBCFG_CLOSE_ASSOC_PHRASE,	GB_ENGINE_CHINESE,	0,	0}	//chn-关闭词语联想
			,{GBCFG_FIND_ONLY_FULL_MATCHING,	GB_ENGINE_CHINESE,	0,	0}	//chn-只匹配完整的词，并且不匹配可能的字
			,{GBCFG_ASSOCIATE_INTEPCT,	GB_ENGINE_CHINESE,	1,	1}	//chn-支持智能标点预测
			,{GBCFG_ASSOC_NOT_FILL_HIGHEST_FREQ_WORD,	GB_ENGINE_CHINESE,	0,	0}	//chn-联想的时候不填充高频字
			,{GBCFG_SET_PHRASE_SPLIT_CHAR,	GB_ENGINE_CHINESE,	0,	0}	//chn-设置分隔符号的显示字符
			,{GBCFG_SET_PHRASE_LEN_LIMIT,	GB_ENGINE_CHINESE,	0,	0}	//设置词语的最大长度限制
			,{GBCFG_CLOSE_COMPONENT,	GB_ENGINE_CHINESE,	0,	0}	//chn-关闭部首功能
			,{GBCFG_INIT_GET_HIGHEST_FREQ_WORD,	GB_ENGINE_CHINESE,	0,	0}	//chn-获取默认高频字
			,{GBCFG_NOT_ALLOW_USELESS_STROKE,	GB_ENGINE_CHINESE,	1,	1}	//chn-笔画和韩文输入法下，输入超长串后不允许出来候选
			,{GBCFG_SET_STROKE_WILDCHAR_SHOW,	GB_ENGINE_CHINESE,	0,	0}	//设置笔画通配符符号
			,{GBCFG_CHARSET_SET,	GB_ENGINE_CHINESE,	1,	0}	//设置字符集 
			,{GBALP_AUTO_CORRECTION,	GB_ENGINE_ALPHABETIC,	0,	0}	//alp-打开Explict/Qwerty智能纠错功能
			,{GBALP_CLOSE_AUTO_COMPLETION,	GB_ENGINE_ALPHABETIC,	0,	0}	//alp-关闭number键盘以及half-qwert键盘自动完成功能
			,{GBALP_INPUTSTRING_IS_FIRST_CANDIDATE,	GB_ENGINE_ALPHABETIC,	1,	0}	//alp-Explicit输入串作为第一个候选
			,{GBALP_QWERTY_OUTPUT_DEFAULT_STRING,	GB_ENGINE_ALPHABETIC,	0,	0}	//alp-无候选匹配时，Qwerty输入串对应的默认字符作为第一个候选 
			,{GBALP_FIRST_KEY_ASSOC,	GB_ENGINE_ALPHABETIC,	1,	0}	//alp-Explicit模式和Qwerty每个键开始自动完成
			,{GBALP_ASSOC_NO_DERIVE,	GB_ENGINE_ALPHABETIC,	1,	0}	//alp-不输出派生词
			,{GBALP_KEEP_ALL_HEADER,	GB_ENGINE_ALPHABETIC,	0,	0}	//alp-获取不是单词的header
			,{GBALP_SET_AVOID_PATENT_SYMBOL,	GB_ENGINE_ALPHABETIC,	0,	0}	//alp-设置规避专利所使用的符号
			,{GBALP_SET_EMBEDDED_INTERPUNCTION_KEY,	GB_ENGINE_ALPHABETIC,	GBIME_EMBEDDED_INTERPUNCTION_KEY,	0}	//alp-设置外文输入时所使用的智能标点按键
			,{GBFO_Chn_All_Num_Can_Continuously_Input,	GB_ENGINE_FUNCTION,	0,	0}	//chn-中文数字键盘支持0-9按键连续输入
			,{GBFO_Alp_All_Num_Can_Continuously_Input,	GB_ENGINE_FUNCTION,	0,	0}	//alf-英文数字键盘支持0-9按键连续输入
			,{GBFO_Chn_Display_Input_Key_Sequence,	GB_ENGINE_FUNCTION,	0,	0}	//中文输入区中显示数字
			,{GBFO_Alp_Up_To_Input_Key_Sequence,	GB_ENGINE_FUNCTION,	1,	1}	//alp-打开按上键上屏功能
			,{GBFO_Chn_Up_To_Input_Key_Sequence,	GB_ENGINE_FUNCTION,	1,	1}	//chn-打开按上键上屏功能
			,{GBFO_Chn_No_Response_When_Key_No_Cand,	GB_ENGINE_FUNCTION,	0,	0}	//chn-无候选时，屏蔽按键输入
			,{GBFO_Alp_No_Response_When_Key_No_Cand,	GB_ENGINE_FUNCTION,	0,	0}	//alp-无候选时，屏蔽按键输入
			,{GBFO_Alp_Add_Space_After_Cand_Confirmed,	GB_ENGINE_FUNCTION,	0,	0}	//alp-上屏一个候选后，自动添加空格
			,{GBFO_Alp_UAW,	GB_ENGINE_FUNCTION,	1,	1}	//alp-支持添加自造词
			,{GBFO_OK_Enter_Selected,	GB_ENGINE_SYNTAX,	0,	0}	//chn-整句拼音输入法:在没有进入候选状态下面，OK键进入选择状态
			,{GBFO_Chn_Syntex_Key_Sequence_Edit,	GB_ENGINE_SYNTAX,	0,	0}	//chn-整句拼音输入法: 支持按键序列修改
			,{GBFO_Chn_Switch_Alp,	GB_ENGINE_FUNCTION,	0,	0}	//chn-在中文输入法中，使用中英切换的功能
			,{GBFO_API_Stop_Log,	GB_ENGINE_FUNCTION,	1,	1}	//临时关闭Log功能
			,{GBFO_Alp_UAW_Confirm,	GB_ENGINE_FUNCTION,	0,	0}	//alp-在Alp Predictive/HalfQwerty 模式下打开自造词的确认流程 
			,{GBFO_Multi_Tap_Show_Cand_Wnd,	GB_ENGINE_FUNCTION,	1,	0}	//alp-在MultiTap状态下面仍然打开候选窗口
			,{GBFO_Scroll_Cand_By_Row,	GB_ENGINE_FUNCTION,	1,	1}	//一行一行的对候选进行翻页
			,{GBFO_Tch_Scr,	GB_ENGINE_FUNCTION,	0,	0}	//alp-触摸屏输入下，不提供候选数字编号
			,{GBFO_Alp_Paging_By_Lower,	GB_ENGINE_FUNCTION,	0,	0}	//alp-按照小写候选排序进行分页
		}
	}
};


GBINT GetProductDataCount()
{
	return PT_ARRAY_SIZE(productData);
}

GBINT GetProductConfigCount(GBINT index)
{
	if (index >= GetProductDataCount())
	{
		return 0;
	}
	return PT_ARRAY_SIZE(productData[index].configData);
}