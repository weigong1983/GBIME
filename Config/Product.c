#include "CConfigData.h"
#include "CProductSetting.h"
#include "GBIME.h"


const GBProductData productData[] = 
{
///��Ʒ1
	{
		PRODUCE_NUMPAD_1
		,L"Number+����(����+����)+Alp(����)"
		,L"PRODUCE_NUMPAD_1.config"
		,{GBCL_ROW_KEYBD, 1, GBCL_ROW_KEYBD, 1}
		,{
			{GBCFG_PINYIN_FUZZY_H_F,	GB_ENGINE_CHINESE,	0,	0}	//f = h
			,{GBCFG_PINYIN_FUZZY_K_G,	GB_ENGINE_CHINESE,	0,	0}	//k = g
			,{GBCFG_PINYIN_FUZZY_N_L,	GB_ENGINE_CHINESE,	0,	0}	//n = l
			,{GBCFG_PINYIN_FUZZY_R_L,	GB_ENGINE_CHINESE,	0,	0}	//r = l
			,{GBCFG_PINYIN_FUZZY_NG_N,	GB_ENGINE_CHINESE,	0,	0}	//en=eng an=ang in=ing
			,{GBCFG_PINYIN_FUZZY_XH_X,	GB_ENGINE_CHINESE,	0,	0}	//s=sh c=ch z=zh
			,{GBCFG_OUT_ZI_SYLLABLE,	GB_ENGINE_CHINESE,	0,	0}	//chn-ֻ����ֵ�ƴ��
			,{GBCFG_TRANSLATE_INPUT_STRING,	GB_ENGINE_CHINESE,	0,	0}	//chn-�������ַ���תΪƴ����
			,{GBCFG_PY_1_MATCH_FUNC,	GB_ENGINE_CHINESE,	0,	0}	//chn-ƴ����ȷƥ��
			,{GBCFG_ADD_TRANSLATE_INPUT_STRING_TO_SYLLABLE,	GB_ENGINE_CHINESE,	0,	0}	//chn-��ת������ַ�������ƴ�������б�
			,{GBCFG_CLOSE_PHRASE,	GB_ENGINE_CHINESE,	0,	0}	//chn-�رմ��﹦��
			,{GBCFG_ONLY_ENTIRE_PHRASE_CAND,	GB_ENGINE_CHINESE,	0,	0}	//chn-ֻƥ�������Ĵ���,��ƥ����ܵ���
			,{GBCFG_CLOSE_ASSOC_PHRASE,	GB_ENGINE_CHINESE,	0,	0}	//chn-�رմ�������
			,{GBCFG_FIND_ONLY_FULL_MATCHING,	GB_ENGINE_CHINESE,	0,	0}	//chn-ֻƥ�������Ĵʣ����Ҳ�ƥ����ܵ���
			,{GBCFG_ASSOCIATE_INTEPCT,	GB_ENGINE_CHINESE,	1,	1}	//chn-֧�����ܱ��Ԥ��
			,{GBCFG_ASSOC_NOT_FILL_HIGHEST_FREQ_WORD,	GB_ENGINE_CHINESE,	0,	0}	//chn-�����ʱ������Ƶ��
			,{GBCFG_SET_PHRASE_SPLIT_CHAR,	GB_ENGINE_CHINESE,	0,	0}	//chn-���÷ָ����ŵ���ʾ�ַ�
			,{GBCFG_SET_PHRASE_LEN_LIMIT,	GB_ENGINE_CHINESE,	0,	0}	//���ô������󳤶�����
			,{GBCFG_CLOSE_COMPONENT,	GB_ENGINE_CHINESE,	0,	0}	//chn-�رղ��׹���
			,{GBCFG_INIT_GET_HIGHEST_FREQ_WORD,	GB_ENGINE_CHINESE,	0,	0}	//chn-��ȡĬ�ϸ�Ƶ��
			,{GBCFG_NOT_ALLOW_USELESS_STROKE,	GB_ENGINE_CHINESE,	1,	1}	//chn-�ʻ��ͺ������뷨�£����볬���������������ѡ
			,{GBCFG_SET_STROKE_WILDCHAR_SHOW,	GB_ENGINE_CHINESE,	0,	0}	//���ñʻ�ͨ�������
			,{GBCFG_CHARSET_SET,	GB_ENGINE_CHINESE,	1,	0}	//�����ַ��� 
			,{GBALP_AUTO_CORRECTION,	GB_ENGINE_ALPHABETIC,	0,	0}	//alp-��Explict/Qwerty���ܾ�����
			,{GBALP_CLOSE_AUTO_COMPLETION,	GB_ENGINE_ALPHABETIC,	0,	0}	//alp-�ر�number�����Լ�half-qwert�����Զ���ɹ���
			,{GBALP_INPUTSTRING_IS_FIRST_CANDIDATE,	GB_ENGINE_ALPHABETIC,	0,	0}	//alp-Explicit���봮��Ϊ��һ����ѡ
			,{GBALP_QWERTY_OUTPUT_DEFAULT_STRING,	GB_ENGINE_ALPHABETIC,	0,	0}	//alp-�޺�ѡƥ��ʱ��Qwerty���봮��Ӧ��Ĭ���ַ���Ϊ��һ����ѡ 
			,{GBALP_FIRST_KEY_ASSOC,	GB_ENGINE_ALPHABETIC,	0,	0}	//alp-Explicitģʽ��Qwertyÿ������ʼ�Զ����
			,{GBALP_ASSOC_NO_DERIVE,	GB_ENGINE_ALPHABETIC,	1,	1}	//alp-�����������
			,{GBALP_KEEP_ALL_HEADER,	GB_ENGINE_ALPHABETIC,	0,	0}	//alp-��ȡ���ǵ��ʵ�header
			,{GBALP_SET_AVOID_PATENT_SYMBOL,	GB_ENGINE_ALPHABETIC,	0,	0}	//alp-���ù��ר����ʹ�õķ���
			,{GBALP_SET_EMBEDDED_INTERPUNCTION_KEY,	GB_ENGINE_ALPHABETIC,	0,	0}	//alp-������������ʱ��ʹ�õ����ܱ�㰴��
			,{GBFO_Chn_All_Num_Can_Continuously_Input,	GB_ENGINE_FUNCTION,	1,	1}	//chn-�������ּ���֧��0-9������������
			,{GBFO_Alp_All_Num_Can_Continuously_Input,	GB_ENGINE_FUNCTION,	1,	1}	//alf-Ӣ�����ּ���֧��0-9������������
			,{GBFO_Chn_Display_Input_Key_Sequence,	GB_ENGINE_FUNCTION,	1,	1}	//��������������ʾ����
			,{GBFO_Alp_Up_To_Input_Key_Sequence,	GB_ENGINE_FUNCTION,	1,	1}	//alp-�򿪰��ϼ���������
			,{GBFO_Chn_Up_To_Input_Key_Sequence,	GB_ENGINE_FUNCTION,	1,	1}	//chn-�򿪰��ϼ���������
			,{GBFO_Chn_No_Response_When_Key_No_Cand,	GB_ENGINE_FUNCTION,	0,	0}	//chn-�޺�ѡʱ�����ΰ�������
			,{GBFO_Alp_No_Response_When_Key_No_Cand,	GB_ENGINE_FUNCTION,	0,	0}	//alp-�޺�ѡʱ�����ΰ�������
			,{GBFO_Alp_Add_Space_After_Cand_Confirmed,	GB_ENGINE_FUNCTION,	0,	0}	//alp-����һ����ѡ���Զ���ӿո�
			,{GBFO_Alp_UAW,	GB_ENGINE_FUNCTION,	1,	1}	//alp-֧����������
			,{GBFO_OK_Enter_Selected,	GB_ENGINE_SYNTAX,	0,	0}	//chn-����ƴ�����뷨:��û�н����ѡ״̬���棬OK������ѡ��״̬
			,{GBFO_Chn_Syntex_Key_Sequence_Edit,	GB_ENGINE_SYNTAX,	0,	0}	//chn-����ƴ�����뷨: ֧�ְ��������޸�
			,{GBFO_Chn_Switch_Alp,	GB_ENGINE_FUNCTION,	1,	1}	//chn-���������뷨�У�ʹ����Ӣ�л��Ĺ���
			,{GBFO_API_Stop_Log,	GB_ENGINE_FUNCTION,	1,	1}	//��ʱ�ر�Log����
			,{GBFO_Alp_UAW_Confirm,	GB_ENGINE_FUNCTION,	0,	0}	//alp-��Alp Predictive/HalfQwerty ģʽ�´�����ʵ�ȷ������ 
			,{GBFO_Multi_Tap_Show_Cand_Wnd,	GB_ENGINE_FUNCTION,	1,	0}	//alp-��MultiTap״̬������Ȼ�򿪺�ѡ����
			,{GBFO_Scroll_Cand_By_Row,	GB_ENGINE_FUNCTION,	1,	1}	//һ��һ�еĶԺ�ѡ���з�ҳ
			,{GBFO_Tch_Scr,	GB_ENGINE_FUNCTION,	0,	0}	//alp-�����������£����ṩ��ѡ���ֱ��
			,{GBFO_Alp_Paging_By_Lower,	GB_ENGINE_FUNCTION,	0,	0}	//alp-����Сд��ѡ������з�ҳ
		}
	}
///��Ʒ2
	,{
		PRODUCE_NUMPAD_11
		,L"Number+��ͳ����(����+����)+Alp(inline)"
		,L"PRODUCE_NUMPAD_11.config"
		,{GBCL_ROW_KEYBD, 1, GBCL_SINGLE_CAND, 1}
		,{
			{GBCFG_PINYIN_FUZZY_H_F,	GB_ENGINE_CHINESE,	0,	0}	//f = h
			,{GBCFG_PINYIN_FUZZY_K_G,	GB_ENGINE_CHINESE,	0,	0}	//k = g
			,{GBCFG_PINYIN_FUZZY_N_L,	GB_ENGINE_CHINESE,	0,	0}	//n = l
			,{GBCFG_PINYIN_FUZZY_R_L,	GB_ENGINE_CHINESE,	0,	0}	//r = l
			,{GBCFG_PINYIN_FUZZY_NG_N,	GB_ENGINE_CHINESE,	0,	0}	//en=eng an=ang in=ing
			,{GBCFG_PINYIN_FUZZY_XH_X,	GB_ENGINE_CHINESE,	0,	0}	//s=sh c=ch z=zh
			,{GBCFG_OUT_ZI_SYLLABLE,	GB_ENGINE_CHINESE,	0,	0}	//chn-ֻ����ֵ�ƴ��
			,{GBCFG_TRANSLATE_INPUT_STRING,	GB_ENGINE_CHINESE,	0,	0}	//chn-�������ַ���תΪƴ����
			,{GBCFG_PY_1_MATCH_FUNC,	GB_ENGINE_CHINESE,	0,	0}	//chn-ƴ����ȷƥ��
			,{GBCFG_ADD_TRANSLATE_INPUT_STRING_TO_SYLLABLE,	GB_ENGINE_CHINESE,	0,	0}	//chn-��ת������ַ�������ƴ�������б�
			,{GBCFG_CLOSE_PHRASE,	GB_ENGINE_CHINESE,	0,	0}	//chn-�رմ��﹦��
			,{GBCFG_ONLY_ENTIRE_PHRASE_CAND,	GB_ENGINE_CHINESE,	0,	0}	//chn-ֻƥ�������Ĵ���,��ƥ����ܵ���
			,{GBCFG_CLOSE_ASSOC_PHRASE,	GB_ENGINE_CHINESE,	0,	0}	//chn-�رմ�������
			,{GBCFG_FIND_ONLY_FULL_MATCHING,	GB_ENGINE_CHINESE,	0,	0}	//chn-ֻƥ�������Ĵʣ����Ҳ�ƥ����ܵ���
			,{GBCFG_ASSOCIATE_INTEPCT,	GB_ENGINE_CHINESE,	1,	1}	//chn-֧�����ܱ��Ԥ��
			,{GBCFG_ASSOC_NOT_FILL_HIGHEST_FREQ_WORD,	GB_ENGINE_CHINESE,	0,	0}	//chn-�����ʱ������Ƶ��
			,{GBCFG_SET_PHRASE_SPLIT_CHAR,	GB_ENGINE_CHINESE,	0,	0}	//chn-���÷ָ����ŵ���ʾ�ַ�
			,{GBCFG_SET_PHRASE_LEN_LIMIT,	GB_ENGINE_CHINESE,	0,	0}	//���ô������󳤶�����
			,{GBCFG_CLOSE_COMPONENT,	GB_ENGINE_CHINESE,	0,	0}	//chn-�رղ��׹���
			,{GBCFG_INIT_GET_HIGHEST_FREQ_WORD,	GB_ENGINE_CHINESE,	0,	0}	//chn-��ȡĬ�ϸ�Ƶ��
			,{GBCFG_NOT_ALLOW_USELESS_STROKE,	GB_ENGINE_CHINESE,	1,	1}	//chn-�ʻ��ͺ������뷨�£����볬���������������ѡ
			,{GBCFG_SET_STROKE_WILDCHAR_SHOW,	GB_ENGINE_CHINESE,	0,	0}	//���ñʻ�ͨ�������
			,{GBCFG_CHARSET_SET,	GB_ENGINE_CHINESE,	1,	0}	//�����ַ��� 
			,{GBALP_AUTO_CORRECTION,	GB_ENGINE_ALPHABETIC,	0,	0}	//alp-��Explict/Qwerty���ܾ�����
			,{GBALP_CLOSE_AUTO_COMPLETION,	GB_ENGINE_ALPHABETIC,	0,	0}	//alp-�ر�number�����Լ�half-qwert�����Զ���ɹ���
			,{GBALP_INPUTSTRING_IS_FIRST_CANDIDATE,	GB_ENGINE_ALPHABETIC,	0,	0}	//alp-Explicit���봮��Ϊ��һ����ѡ
			,{GBALP_QWERTY_OUTPUT_DEFAULT_STRING,	GB_ENGINE_ALPHABETIC,	0,	0}	//alp-�޺�ѡƥ��ʱ��Qwerty���봮��Ӧ��Ĭ���ַ���Ϊ��һ����ѡ 
			,{GBALP_FIRST_KEY_ASSOC,	GB_ENGINE_ALPHABETIC,	0,	0}	//alp-Explicitģʽ��Qwertyÿ������ʼ�Զ����
			,{GBALP_ASSOC_NO_DERIVE,	GB_ENGINE_ALPHABETIC,	1,	1}	//alp-�����������
			,{GBALP_KEEP_ALL_HEADER,	GB_ENGINE_ALPHABETIC,	0,	0}	//alp-��ȡ���ǵ��ʵ�header
			,{GBALP_SET_AVOID_PATENT_SYMBOL,	GB_ENGINE_ALPHABETIC,	0,	0}	//alp-���ù��ר����ʹ�õķ���
			,{GBALP_SET_EMBEDDED_INTERPUNCTION_KEY,	GB_ENGINE_ALPHABETIC,	0,	0}	//alp-������������ʱ��ʹ�õ����ܱ�㰴��
			,{GBFO_Chn_All_Num_Can_Continuously_Input,	GB_ENGINE_FUNCTION,	1,	1}	//chn-�������ּ���֧��0-9������������
			,{GBFO_Alp_All_Num_Can_Continuously_Input,	GB_ENGINE_FUNCTION,	1,	1}	//alf-Ӣ�����ּ���֧��0-9������������
			,{GBFO_Chn_Display_Input_Key_Sequence,	GB_ENGINE_FUNCTION,	1,	1}	//��������������ʾ����
			,{GBFO_Alp_Up_To_Input_Key_Sequence,	GB_ENGINE_FUNCTION,	0,	0}	//alp-�򿪰��ϼ���������
			,{GBFO_Chn_Up_To_Input_Key_Sequence,	GB_ENGINE_FUNCTION,	0,	0}	//chn-�򿪰��ϼ���������
			,{GBFO_Chn_No_Response_When_Key_No_Cand,	GB_ENGINE_FUNCTION,	0,	0}	//chn-�޺�ѡʱ�����ΰ�������
			,{GBFO_Alp_No_Response_When_Key_No_Cand,	GB_ENGINE_FUNCTION,	0,	0}	//alp-�޺�ѡʱ�����ΰ�������
			,{GBFO_Alp_Add_Space_After_Cand_Confirmed,	GB_ENGINE_FUNCTION,	0,	0}	//alp-����һ����ѡ���Զ���ӿո�
			,{GBFO_Alp_UAW,	GB_ENGINE_FUNCTION,	1,	1}	//alp-֧����������
			,{GBFO_OK_Enter_Selected,	GB_ENGINE_SYNTAX,	0,	0}	//chn-����ƴ�����뷨:��û�н����ѡ״̬���棬OK������ѡ��״̬
			,{GBFO_Chn_Syntex_Key_Sequence_Edit,	GB_ENGINE_SYNTAX,	0,	0}	//chn-����ƴ�����뷨: ֧�ְ��������޸�
			,{GBFO_Chn_Switch_Alp,	GB_ENGINE_FUNCTION,	0,	0}	//chn-���������뷨�У�ʹ����Ӣ�л��Ĺ���
			,{GBFO_API_Stop_Log,	GB_ENGINE_FUNCTION,	1,	1}	//��ʱ�ر�Log����
			,{GBFO_Alp_UAW_Confirm,	GB_ENGINE_FUNCTION,	0,	0}	//alp-��Alp Predictive/HalfQwerty ģʽ�´�����ʵ�ȷ������ 
			,{GBFO_Multi_Tap_Show_Cand_Wnd,	GB_ENGINE_FUNCTION,	1,	0}	//alp-��MultiTap״̬������Ȼ�򿪺�ѡ����
			,{GBFO_Scroll_Cand_By_Row,	GB_ENGINE_FUNCTION,	1,	1}	//һ��һ�еĶԺ�ѡ���з�ҳ
			,{GBFO_Tch_Scr,	GB_ENGINE_FUNCTION,	0,	0}	//alp-�����������£����ṩ��ѡ���ֱ��
			,{GBFO_Alp_Paging_By_Lower,	GB_ENGINE_FUNCTION,	0,	0}	//alp-����Сд��ѡ������з�ҳ
		}
	}
///��Ʒ3
	,{
		PRODUCE_EXPLICIT_1
		,L"Explicit+����(����+����)+Alp(����)"
		,L"PRODUCE_EXPLICIT_1.config"
		,{GBCL_ROW_KEYBD, 1, GBCL_ROW_KEYBD, 1}
		,{
			{GBCFG_PINYIN_FUZZY_H_F,	GB_ENGINE_CHINESE,	0,	0}	//f = h
			,{GBCFG_PINYIN_FUZZY_K_G,	GB_ENGINE_CHINESE,	0,	0}	//k = g
			,{GBCFG_PINYIN_FUZZY_N_L,	GB_ENGINE_CHINESE,	0,	0}	//n = l
			,{GBCFG_PINYIN_FUZZY_R_L,	GB_ENGINE_CHINESE,	0,	0}	//r = l
			,{GBCFG_PINYIN_FUZZY_NG_N,	GB_ENGINE_CHINESE,	0,	0}	//en=eng an=ang in=ing
			,{GBCFG_PINYIN_FUZZY_XH_X,	GB_ENGINE_CHINESE,	0,	0}	//s=sh c=ch z=zh
			,{GBCFG_OUT_ZI_SYLLABLE,	GB_ENGINE_CHINESE,	0,	0}	//chn-ֻ����ֵ�ƴ��
			,{GBCFG_TRANSLATE_INPUT_STRING,	GB_ENGINE_CHINESE,	0,	0}	//chn-�������ַ���תΪƴ����
			,{GBCFG_PY_1_MATCH_FUNC,	GB_ENGINE_CHINESE,	0,	0}	//chn-ƴ����ȷƥ��
			,{GBCFG_ADD_TRANSLATE_INPUT_STRING_TO_SYLLABLE,	GB_ENGINE_CHINESE,	0,	0}	//chn-��ת������ַ�������ƴ�������б�
			,{GBCFG_CLOSE_PHRASE,	GB_ENGINE_CHINESE,	0,	0}	//chn-�رմ��﹦��
			,{GBCFG_ONLY_ENTIRE_PHRASE_CAND,	GB_ENGINE_CHINESE,	0,	0}	//chn-ֻƥ�������Ĵ���,��ƥ����ܵ���
			,{GBCFG_CLOSE_ASSOC_PHRASE,	GB_ENGINE_CHINESE,	0,	0}	//chn-�رմ�������
			,{GBCFG_FIND_ONLY_FULL_MATCHING,	GB_ENGINE_CHINESE,	0,	0}	//chn-ֻƥ�������Ĵʣ����Ҳ�ƥ����ܵ���
			,{GBCFG_ASSOCIATE_INTEPCT,	GB_ENGINE_CHINESE,	1,	1}	//chn-֧�����ܱ��Ԥ��
			,{GBCFG_ASSOC_NOT_FILL_HIGHEST_FREQ_WORD,	GB_ENGINE_CHINESE,	0,	0}	//chn-�����ʱ������Ƶ��
			,{GBCFG_SET_PHRASE_SPLIT_CHAR,	GB_ENGINE_CHINESE,	0,	0}	//chn-���÷ָ����ŵ���ʾ�ַ�
			,{GBCFG_SET_PHRASE_LEN_LIMIT,	GB_ENGINE_CHINESE,	0,	0}	//���ô������󳤶�����
			,{GBCFG_CLOSE_COMPONENT,	GB_ENGINE_CHINESE,	0,	0}	//chn-�رղ��׹���
			,{GBCFG_INIT_GET_HIGHEST_FREQ_WORD,	GB_ENGINE_CHINESE,	0,	0}	//chn-��ȡĬ�ϸ�Ƶ��
			,{GBCFG_NOT_ALLOW_USELESS_STROKE,	GB_ENGINE_CHINESE,	1,	1}	//chn-�ʻ��ͺ������뷨�£����볬���������������ѡ
			,{GBCFG_SET_STROKE_WILDCHAR_SHOW,	GB_ENGINE_CHINESE,	0,	0}	//���ñʻ�ͨ�������
			,{GBCFG_CHARSET_SET,	GB_ENGINE_CHINESE,	1,	0}	//�����ַ��� 
			,{GBALP_AUTO_CORRECTION,	GB_ENGINE_ALPHABETIC,	0,	0}	//alp-��Explict/Qwerty���ܾ�����
			,{GBALP_CLOSE_AUTO_COMPLETION,	GB_ENGINE_ALPHABETIC,	0,	0}	//alp-�ر�number�����Լ�half-qwert�����Զ���ɹ���
			,{GBALP_INPUTSTRING_IS_FIRST_CANDIDATE,	GB_ENGINE_ALPHABETIC,	1,	0}	//alp-Explicit���봮��Ϊ��һ����ѡ
			,{GBALP_QWERTY_OUTPUT_DEFAULT_STRING,	GB_ENGINE_ALPHABETIC,	0,	0}	//alp-�޺�ѡƥ��ʱ��Qwerty���봮��Ӧ��Ĭ���ַ���Ϊ��һ����ѡ 
			,{GBALP_FIRST_KEY_ASSOC,	GB_ENGINE_ALPHABETIC,	1,	0}	//alp-Explicitģʽ��Qwertyÿ������ʼ�Զ����
			,{GBALP_ASSOC_NO_DERIVE,	GB_ENGINE_ALPHABETIC,	1,	0}	//alp-�����������
			,{GBALP_KEEP_ALL_HEADER,	GB_ENGINE_ALPHABETIC,	0,	0}	//alp-��ȡ���ǵ��ʵ�header
			,{GBALP_SET_AVOID_PATENT_SYMBOL,	GB_ENGINE_ALPHABETIC,	0,	0}	//alp-���ù��ר����ʹ�õķ���
			,{GBALP_SET_EMBEDDED_INTERPUNCTION_KEY,	GB_ENGINE_ALPHABETIC,	GBIME_EMBEDDED_INTERPUNCTION_KEY,	0}	//alp-������������ʱ��ʹ�õ����ܱ�㰴��
			,{GBFO_Chn_All_Num_Can_Continuously_Input,	GB_ENGINE_FUNCTION,	0,	0}	//chn-�������ּ���֧��0-9������������
			,{GBFO_Alp_All_Num_Can_Continuously_Input,	GB_ENGINE_FUNCTION,	0,	0}	//alf-Ӣ�����ּ���֧��0-9������������
			,{GBFO_Chn_Display_Input_Key_Sequence,	GB_ENGINE_FUNCTION,	0,	0}	//��������������ʾ����
			,{GBFO_Alp_Up_To_Input_Key_Sequence,	GB_ENGINE_FUNCTION,	1,	1}	//alp-�򿪰��ϼ���������
			,{GBFO_Chn_Up_To_Input_Key_Sequence,	GB_ENGINE_FUNCTION,	1,	1}	//chn-�򿪰��ϼ���������
			,{GBFO_Chn_No_Response_When_Key_No_Cand,	GB_ENGINE_FUNCTION,	0,	0}	//chn-�޺�ѡʱ�����ΰ�������
			,{GBFO_Alp_No_Response_When_Key_No_Cand,	GB_ENGINE_FUNCTION,	0,	0}	//alp-�޺�ѡʱ�����ΰ�������
			,{GBFO_Alp_Add_Space_After_Cand_Confirmed,	GB_ENGINE_FUNCTION,	0,	0}	//alp-����һ����ѡ���Զ���ӿո�
			,{GBFO_Alp_UAW,	GB_ENGINE_FUNCTION,	1,	1}	//alp-֧����������
			,{GBFO_OK_Enter_Selected,	GB_ENGINE_SYNTAX,	0,	0}	//chn-����ƴ�����뷨:��û�н����ѡ״̬���棬OK������ѡ��״̬
			,{GBFO_Chn_Syntex_Key_Sequence_Edit,	GB_ENGINE_SYNTAX,	0,	0}	//chn-����ƴ�����뷨: ֧�ְ��������޸�
			,{GBFO_Chn_Switch_Alp,	GB_ENGINE_FUNCTION,	0,	0}	//chn-���������뷨�У�ʹ����Ӣ�л��Ĺ���
			,{GBFO_API_Stop_Log,	GB_ENGINE_FUNCTION,	1,	1}	//��ʱ�ر�Log����
			,{GBFO_Alp_UAW_Confirm,	GB_ENGINE_FUNCTION,	0,	0}	//alp-��Alp Predictive/HalfQwerty ģʽ�´�����ʵ�ȷ������ 
			,{GBFO_Multi_Tap_Show_Cand_Wnd,	GB_ENGINE_FUNCTION,	1,	0}	//alp-��MultiTap״̬������Ȼ�򿪺�ѡ����
			,{GBFO_Scroll_Cand_By_Row,	GB_ENGINE_FUNCTION,	1,	1}	//һ��һ�еĶԺ�ѡ���з�ҳ
			,{GBFO_Tch_Scr,	GB_ENGINE_FUNCTION,	0,	0}	//alp-�����������£����ṩ��ѡ���ֱ��
			,{GBFO_Alp_Paging_By_Lower,	GB_ENGINE_FUNCTION,	0,	0}	//alp-����Сд��ѡ������з�ҳ
		}
	}
///��Ʒ4
	,{
		PRODUCE_EXPLICIT_5
		,L"Explicit+������ ����(����+����)+Alp(����)"
		,L"PRODUCE_EXPLICIT_5.config"
		,{GBCL_ROW_KEYBD, 1, GBCL_ROW_KEYBD, 1}
		,{
			{GBCFG_PINYIN_FUZZY_H_F,	GB_ENGINE_CHINESE,	0,	0}	//f = h
			,{GBCFG_PINYIN_FUZZY_K_G,	GB_ENGINE_CHINESE,	0,	0}	//k = g
			,{GBCFG_PINYIN_FUZZY_N_L,	GB_ENGINE_CHINESE,	0,	0}	//n = l
			,{GBCFG_PINYIN_FUZZY_R_L,	GB_ENGINE_CHINESE,	0,	0}	//r = l
			,{GBCFG_PINYIN_FUZZY_NG_N,	GB_ENGINE_CHINESE,	0,	0}	//en=eng an=ang in=ing
			,{GBCFG_PINYIN_FUZZY_XH_X,	GB_ENGINE_CHINESE,	0,	0}	//s=sh c=ch z=zh
			,{GBCFG_OUT_ZI_SYLLABLE,	GB_ENGINE_CHINESE,	0,	0}	//chn-ֻ����ֵ�ƴ��
			,{GBCFG_TRANSLATE_INPUT_STRING,	GB_ENGINE_CHINESE,	0,	0}	//chn-�������ַ���תΪƴ����
			,{GBCFG_PY_1_MATCH_FUNC,	GB_ENGINE_CHINESE,	0,	0}	//chn-ƴ����ȷƥ��
			,{GBCFG_ADD_TRANSLATE_INPUT_STRING_TO_SYLLABLE,	GB_ENGINE_CHINESE,	0,	0}	//chn-��ת������ַ�������ƴ�������б�
			,{GBCFG_CLOSE_PHRASE,	GB_ENGINE_CHINESE,	0,	0}	//chn-�رմ��﹦��
			,{GBCFG_ONLY_ENTIRE_PHRASE_CAND,	GB_ENGINE_CHINESE,	0,	0}	//chn-ֻƥ�������Ĵ���,��ƥ����ܵ���
			,{GBCFG_CLOSE_ASSOC_PHRASE,	GB_ENGINE_CHINESE,	0,	0}	//chn-�رմ�������
			,{GBCFG_FIND_ONLY_FULL_MATCHING,	GB_ENGINE_CHINESE,	0,	0}	//chn-ֻƥ�������Ĵʣ����Ҳ�ƥ����ܵ���
			,{GBCFG_ASSOCIATE_INTEPCT,	GB_ENGINE_CHINESE,	1,	1}	//chn-֧�����ܱ��Ԥ��
			,{GBCFG_ASSOC_NOT_FILL_HIGHEST_FREQ_WORD,	GB_ENGINE_CHINESE,	0,	0}	//chn-�����ʱ������Ƶ��
			,{GBCFG_SET_PHRASE_SPLIT_CHAR,	GB_ENGINE_CHINESE,	0,	0}	//chn-���÷ָ����ŵ���ʾ�ַ�
			,{GBCFG_SET_PHRASE_LEN_LIMIT,	GB_ENGINE_CHINESE,	0,	0}	//���ô������󳤶�����
			,{GBCFG_CLOSE_COMPONENT,	GB_ENGINE_CHINESE,	0,	0}	//chn-�رղ��׹���
			,{GBCFG_INIT_GET_HIGHEST_FREQ_WORD,	GB_ENGINE_CHINESE,	0,	0}	//chn-��ȡĬ�ϸ�Ƶ��
			,{GBCFG_NOT_ALLOW_USELESS_STROKE,	GB_ENGINE_CHINESE,	1,	1}	//chn-�ʻ��ͺ������뷨�£����볬���������������ѡ
			,{GBCFG_SET_STROKE_WILDCHAR_SHOW,	GB_ENGINE_CHINESE,	0,	0}	//���ñʻ�ͨ�������
			,{GBCFG_CHARSET_SET,	GB_ENGINE_CHINESE,	1,	0}	//�����ַ��� 
			,{GBALP_AUTO_CORRECTION,	GB_ENGINE_ALPHABETIC,	0,	0}	//alp-��Explict/Qwerty���ܾ�����
			,{GBALP_CLOSE_AUTO_COMPLETION,	GB_ENGINE_ALPHABETIC,	0,	0}	//alp-�ر�number�����Լ�half-qwert�����Զ���ɹ���
			,{GBALP_INPUTSTRING_IS_FIRST_CANDIDATE,	GB_ENGINE_ALPHABETIC,	1,	0}	//alp-Explicit���봮��Ϊ��һ����ѡ
			,{GBALP_QWERTY_OUTPUT_DEFAULT_STRING,	GB_ENGINE_ALPHABETIC,	0,	0}	//alp-�޺�ѡƥ��ʱ��Qwerty���봮��Ӧ��Ĭ���ַ���Ϊ��һ����ѡ 
			,{GBALP_FIRST_KEY_ASSOC,	GB_ENGINE_ALPHABETIC,	1,	0}	//alp-Explicitģʽ��Qwertyÿ������ʼ�Զ����
			,{GBALP_ASSOC_NO_DERIVE,	GB_ENGINE_ALPHABETIC,	1,	0}	//alp-�����������
			,{GBALP_KEEP_ALL_HEADER,	GB_ENGINE_ALPHABETIC,	0,	0}	//alp-��ȡ���ǵ��ʵ�header
			,{GBALP_SET_AVOID_PATENT_SYMBOL,	GB_ENGINE_ALPHABETIC,	0,	0}	//alp-���ù��ר����ʹ�õķ���
			,{GBALP_SET_EMBEDDED_INTERPUNCTION_KEY,	GB_ENGINE_ALPHABETIC,	GBIME_EMBEDDED_INTERPUNCTION_KEY,	0}	//alp-������������ʱ��ʹ�õ����ܱ�㰴��
			,{GBFO_Chn_All_Num_Can_Continuously_Input,	GB_ENGINE_FUNCTION,	0,	0}	//chn-�������ּ���֧��0-9������������
			,{GBFO_Alp_All_Num_Can_Continuously_Input,	GB_ENGINE_FUNCTION,	0,	0}	//alf-Ӣ�����ּ���֧��0-9������������
			,{GBFO_Chn_Display_Input_Key_Sequence,	GB_ENGINE_FUNCTION,	0,	0}	//��������������ʾ����
			,{GBFO_Alp_Up_To_Input_Key_Sequence,	GB_ENGINE_FUNCTION,	1,	1}	//alp-�򿪰��ϼ���������
			,{GBFO_Chn_Up_To_Input_Key_Sequence,	GB_ENGINE_FUNCTION,	1,	1}	//chn-�򿪰��ϼ���������
			,{GBFO_Chn_No_Response_When_Key_No_Cand,	GB_ENGINE_FUNCTION,	0,	0}	//chn-�޺�ѡʱ�����ΰ�������
			,{GBFO_Alp_No_Response_When_Key_No_Cand,	GB_ENGINE_FUNCTION,	0,	0}	//alp-�޺�ѡʱ�����ΰ�������
			,{GBFO_Alp_Add_Space_After_Cand_Confirmed,	GB_ENGINE_FUNCTION,	0,	0}	//alp-����һ����ѡ���Զ���ӿո�
			,{GBFO_Alp_UAW,	GB_ENGINE_FUNCTION,	1,	1}	//alp-֧����������
			,{GBFO_OK_Enter_Selected,	GB_ENGINE_SYNTAX,	0,	0}	//chn-����ƴ�����뷨:��û�н����ѡ״̬���棬OK������ѡ��״̬
			,{GBFO_Chn_Syntex_Key_Sequence_Edit,	GB_ENGINE_SYNTAX,	0,	0}	//chn-����ƴ�����뷨: ֧�ְ��������޸�
			,{GBFO_Chn_Switch_Alp,	GB_ENGINE_FUNCTION,	0,	0}	//chn-���������뷨�У�ʹ����Ӣ�л��Ĺ���
			,{GBFO_API_Stop_Log,	GB_ENGINE_FUNCTION,	1,	1}	//��ʱ�ر�Log����
			,{GBFO_Alp_UAW_Confirm,	GB_ENGINE_FUNCTION,	0,	0}	//alp-��Alp Predictive/HalfQwerty ģʽ�´�����ʵ�ȷ������ 
			,{GBFO_Multi_Tap_Show_Cand_Wnd,	GB_ENGINE_FUNCTION,	1,	0}	//alp-��MultiTap״̬������Ȼ�򿪺�ѡ����
			,{GBFO_Scroll_Cand_By_Row,	GB_ENGINE_FUNCTION,	1,	1}	//һ��һ�еĶԺ�ѡ���з�ҳ
			,{GBFO_Tch_Scr,	GB_ENGINE_FUNCTION,	0,	0}	//alp-�����������£����ṩ��ѡ���ֱ��
			,{GBFO_Alp_Paging_By_Lower,	GB_ENGINE_FUNCTION,	0,	0}	//alp-����Сд��ѡ������з�ҳ
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