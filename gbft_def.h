#ifndef __GB_PLATFORM_DEFINE_
#define __GB_PLATFORM_DEFINE_
#include "gbtype.h"

/*!
 * \brief ���뷨����¼�������¼�����
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
 * \brief һ��ƽ̨�¼�ͨ������Ϊ���뷨����ڲ��໥�������¼���
 */
typedef struct __tag_GBIMEEventGroup
{
	GBIMEEvent	eventGroup[GBIME_EVT_GROUP_CAPACITY];
	GBU8		eventNum;
} GBIMEEventGroup, * PGBIMEEventGroup ;

/*!
 * \brief ���뷨����¼����Ͷ���
 */
enum GBIMEEventType {
	GBIME_EVENT_TYPE_FRAMEWORK				///<�������뷨����¼�(�¼�ֵ�ο�GBIMEFrameworkEventEnum)
	,GBIME_EVENT_TYPE_ENGINE				///<���������¼�(�¼�ֵ�ο�GBIMEEngineEventEnum)
	,GBIME_EVENT_TYPE_LAYOUT				///<Layout�¼�(�¼�ֵ�ο�GBIMELayoutEventEnum)
	,GBIME_EVENT_TYPE_INPUT_METHOD			///<���뷨�¼�(�¼�ֵ�ο�GBEventEnum)
	,GBIME_EVENT_TYPE_CONFIG				///<�����¼�(�¼�ֵ�ο�GBEventEnum)
	,GBIME_EVENT_TYPE_HANDWRITE				///<������д�����¼�(�¼�ֵ�ο�GBIMEHandwriteEventEnum)
	,GBIME_EVENT_TYPE_NUM					///<�¼�������Ŀ
};

/*!
 * \brief ���뷨���ԭ���¼�����
 */
enum GBIMEEventAtom {
	// ���
	GBIME_EVENT_FRAMEWORK_INIT					///<��ܳ�ʼ��
	,GBIME_EVENT_FRAMEWORK_EXIT					///<����˳�

    // ����
	,GBIME_EVENT_ENGINE_INIT					///<�����ʼ��
	,GBIME_EVENT_ENGINE_EXIT					///<�����˳�
	,GBIME_EVENT_ENGINE_RESET					///<����״̬����[�л����뷨ģʽ���رպ�ѡ�������Layoutʱʹ��]
	,GBIME_EVENT_ENGINE_KEY						///<���水���¼�������GBMMIHandleKeyEvent����
	,GBIME_EVENT_ENGINE_GET_ASSOCWORD			///<��ȡ�����ַ���
	,GBIME_EVENT_ENGINE_GET_HOMONYMWORD			///<��ȡͬ����

	// Layout
	,GBIME_EVENT_LAYOUT_CREATE					///<Layout ����
	,GBIME_EVENT_LAYOUT_DESTROY					///<Layout ����
	,GBIME_EVENT_LAYOUT_PEN_DOWN				///<Layout �����ʰ���
	,GBIME_EVENT_LAYOUT_PEN_UP					///<Layout �����ʵ���
	,GBIME_EVENT_LAYOUT_PEN_MOVE				///<Layout �������ƶ�
	,GBIME_EVENT_LAYOUT_REFRESH					///<Layout ˢ��
	,GBIME_EVENT_LAYOUT_ACTIVATE   				///<Layout ����(�״ν���༭�����ʱ����)
	,GBIME_EVENT_LAYOUT_DEACTIVATE				///<Layout ע��(���˳��༭�����ʱ����)
	,GBIME_EVENT_LAYOUT_HIDE      				///<Layout ����
	,GBIME_EVENT_LAYOUT_SHOW      				///<Layout ��ʾ
	,GBIME_EVENT_LAYOUT_ENTER      				///<Layout ����
	,GBIME_EVENT_LAYOUT_EXIT      				///<Layout �˳�
	,GBIME_EVENT_LAYOUT_SWITCH      			///<Layout �л�
	,GBIME_EVENT_LAYOUT_REDRAW      			///<Layout �ػ�

	// ���뷨
	,GBIME_EVENT_INPUT_METHOD_SWITCH			///<�л����뷨
	,GBIME_EVENT_INPUT_METHOD_SWITCH_NOTIFY		///<֪ͨƽ̨�л����뷨�������GBIME����ڲ����л��߼���

	// ����
	,GBIME_EVENT_CONFIG_LOAD					///<��������
	,GBIME_EVENT_CONFIG_MENU					///<�������ò˵�
	,GBIME_EVENT_CONFIG_SET						///<��������
	,GBIME_EVENT_CONFIG_GET						///<��ȡ����
	,GBIME_EVENT_CONFIG_EXIT					///<ж������

	// ��д
	,GBIME_EVENT_HANDWRITE_ENGINE_INIT			///<��д�����ʼ��
	,GBIME_EVENT_HANDWRITE_ENGINE_EXIT			///<��д�����˳�
	,GBIME_EVENT_HANDWRITE_DRAW_CANDIDATE		///<������дʶ������ĺ�ѡ
	,GBIME_EVENT_HANDWRITE_DRAW_ASSOCWORD		///<���������ַ���
	,GBIME_EVENT_HANDWRITE_SET_RECOGNIZE_RANGE	///<����ʶ��Χ

	// ������
	,GBIME_EVENT_KEY_DOWN
	,GBIME_EVENT_KEY_UP
	,GBIME_EVENT_KEY_LONG_PRESS

	,GBIME_EVENT_NUM							///<�¼���Ŀ
};

/*!
 * \brief ƽ̨�¼�����Layout m_pfcPreHandleEventת�����Ŀ�����¼�����(ͨ��һ���¼���ת����һ���໥�����Ŀ��ԭ���¼����ϣ�Ȼ�󽻸������Ϣ����ӿ����δ���)
 */
typedef enum {
	GBIME_CT_KEY_DOWN_REFRESH						///<Layout��ť���£� ����ˢ��
	,GBIME_CT_KEY_UP_REFRESH						///<Layout��ť���� ����ˢ��
	,GBIME_CT_NORMAL_KEY_DOWN						///<Layout��ť������Ӧ�������水���¼�������GBMMIHandleKeyEvent����
	,GBIME_CT_NORMAL_KEY_UP							///<Layout��ť������Ӧ�������水���¼�������GBMMIHandleKeyEvent����
	,GBIME_CT_MOVE      							///<Layout ������滬��
	,GBIME_CT_SWITCH      							///<Layout �л�(�п�����Layout�����л�)
	,GBIME_CT_SHOW      							///<Layout ��ʾ�����
	,GBIME_CT_HIDE      							///<Layout ����

	// ��д���
	,GBIME_CT_HANDWRITE_DRAW_CANDIDATE				///<������дʶ������ĺ�ѡ
	,GBIME_CT_HANDWRITE_DRAW_ASSOCWORD				///<���������ַ���
	,GBIME_CT_HANDWRITE_SET_RECOGNIZE_RANGE			///<����ʶ��Χ

	// �������
	,GBIME_CT_CONFIG_MENU							///<�������ò˵�����

	// ���������
	,GBIME_CT_KEY_DOWN
	,GBIME_CT_KEY_UP
	,GBIME_CT_KEY_LONG_PRESS

	// ������̺���������໥�л�
	,GBIME_CT_VK_2_KB_SWITCH						///<������� -> �������
	,GBIME_CT_KB_2_VK_SWITCH						///<������� -> �������

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
 * \������¼� start [�����±����¼�ʱ�����ڱ�����棬��Ҫ���ұ����ڲ�˳��]
 */
enum PLATFORM_EVENT_ENUM{
	
	GBKEY_VK_A_UPPER = 0x4100       ///<��д��ĸA
	, GBKEY_VK_B_UPPER                ///<��д��ĸB
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

	// ���ż��̷���ѡ���
	, GBKEY_VK_1_NUMBERKEY              ///<���̰���1
	, GBKEY_VK_2_NUMBERKEY              ///<���̰���2
	, GBKEY_VK_3_NUMBERKEY              ///<���̰���3
	, GBKEY_VK_4_NUMBERKEY              ///<���̰���4
	, GBKEY_VK_5_NUMBERKEY              ///<���̰���5
	, GBKEY_VK_6_NUMBERKEY              ///<���̰���6
	, GBKEY_VK_7_NUMBERKEY              ///<���̰���7
	, GBKEY_VK_8_NUMBERKEY              ///<���̰���8
	, GBKEY_VK_9_NUMBERKEY              ///<���̰���9
	, GBKEY_VK_10_NUMBERKEY             ///<���̰���10
	, GBKEY_VK_11_NUMBERKEY             ///<���̰���11
	, GBKEY_VK_12_NUMBERKEY             ///<���̰���12

	//������¼� end

	//function key  start [�����±����¼�ʱ�����ڱ�����棬��Ҫ���ұ����ڲ�˳��]
	, GBKEY_VK_PUNCTUATION = 0x4200           ///<��ݱ��
	, GBKEY_VK_CAP                      ///<��Сд�л���
	, GBKEY_VK_SYM_PAGEUP               ///<�����Ϸ�ҳ��
	, GBKEY_VK_SYM_PAGEDOWN             ///<�����·�ҳ��
	, GBKEY_VK_SBC_DBC_SWICH            ///<ȫ����л���
	, GBKEY_VK_SYM_LOCK                 ///<����������
	, GBKEY_VK_SYM_comment              ///<��ͨ����
	, GBKEY_VK_SYM_EMOTION              ///<�������
	, GBKEY_VK_SYM_SPECIAL              ///<�������
    , GBKEY_VK_SYM_MATH                 ///<��ѧ����
    , GBKEY_VK_SYM_CANDIDATE_AREA        ///<��ѡ��
    , GBKEY_VK_SYM_INPUT_AREA            ///<������
	//handwrite key 
	, GBKEY_VK_V_HANDWRITE_CANDIDATE_AREA            ///<������д��ѡ��
	, GBKEY_VK_V_HANDWRITE_ASSOCIATE_AREA            ///<������д������
	, GBKEY_VK_V_HANDWRITE_ALL                      ///<������дȫ��
	, GBKEY_VK_v_HANDWRITE_ENG                      ///<������дӢ��
	, GBKEY_VK_v_HANDWRITE_NUMBER                   ///<������д����
	, GBKEY_VK_v_HANDWRITE_TONGYIN                   ///<������дͬ��
	, GBKEY_VK_BACKWORD								///<���ؼ�
	, GBKEY_VK_LATEST_KEYBOARD						///<�������һ������

	//function key  end

	//switch key start [�����±����¼�ʱ�����ڱ�����棬��Ҫ���ұ����ڲ�˳��]
	, GBKEY_VK_V9_PINYIN  = 0x4300      ///<����9��ƴ��
	, GBKEY_VK_V9_STROKE                ///<����9���ʻ�
	, GBKEY_VK_V9_ENG_UPPER             ///<����9����дӢ��
	, GBKEY_VK_V9_ENG_LOWER             ///<����9��СдӢ��
	, GBKEY_VK_V9_ABC_UPPER             ///<����9����д��ĸ
	, GBKEY_VK_V9_ABC_LOWER             ///<����9��Сд��ĸ
	, GBKEY_VK_V9_NUM                   ///<����9������
	, GBKEY_VK_V9_SYM                   ///<����9������
	, GBKEY_VK_V9_FS_HANDWRITE          ///<����ȫ����д
	, GBKEY_VK_V9_BOX_HANDWRITE         ///<����������д
	, GBKEY_VK_SETTING                  ///<����

	, GBKEY_VK_V26_PINYIN               ///<����26��ƴ��
	, GBKEY_VK_V26_STROKE               ///<����26���ʻ�
	, GBKEY_VK_V26_ENG_UPPER            ///<����26����дӢ��
	, GBKEY_VK_V26_ENG_LOWER            ///<����26��СдӢ��
	, GBKEY_VK_V26_ABC_UPPER            ///<����26����д��ĸ
	, GBKEY_VK_V26_ABC_LOWER            ///<����26��Сд��ĸ
	, GBKEY_VK_V26_NUM                  ///<����26������
	, GBKEY_VK_V26_SYM                  ///<����26������

	, GBKEY_VK_H9_PINYIN                ///<����9��ƴ��
	, GBKEY_VK_H9_STROKE                ///<����9���ʻ�
	, GBKEY_VK_H9_ENG_UPPER             ///<����9����дӢ��
	, GBKEY_VK_H9_ENG_LOWER             ///<����9��СдӢ��
	, GBKEY_VK_H9_ABC_UPPER             ///<����9����д��ĸ
	, GBKEY_VK_H9_ABC_LOWER             ///<����9��Сд��ĸ
	, GBKEY_VK_H9_NUM                   ///<����9������
	, GBKEY_VK_H9_SYM                   ///<����9������
	, GBKEY_VK_H9_FS_HANDWRITE          ///<����ȫ����д
	, GBKEY_VK_H9_BOX_HANDWRITE         ///<����������д

	, GBKEY_VK_H26_PINYIN               ///<����26��ƴ��
	, GBKEY_VK_H26_STROKE               ///<����26���ʻ�
	, GBKEY_VK_H26_ENG_UPPER            ///<����26����дӢ��
	, GBKEY_VK_H26_ENG_LOWER            ///<����26��СдӢ��
	, GBKEY_VK_H26_ABC_UPPER            ///<����26����д��ĸ
	, GBKEY_VK_H26_ABC_LOWER            ///<����26��Сд��ĸ
	, GBKEY_VK_H26_NUM                  ///<����26������
	, GBKEY_VK_H26_SYM                  ///<����26������

	//switch key end

	//����9��ģʽ�ļ������� start [�����±����¼�ʱ�����ڱ�����棬��Ҫ���ұ����ڲ�˳��]
	, GBKEY_VK_V9_SYMBOL_ADD = 0x4400	///<��������ģʽ"+"��
	, GBKEY_VK_V9_SYMBOL_AT				///<��������ģʽ"@"��
	, GBKEY_VK_V9_SYMBOL_DOLL			///<��������ģʽ"$"��
	, GBKEY_VK_V9_SYMBOL_PERCENT        ///<��������ģʽ"%"��s
	, GBKEY_VK_V9_SYMBOL_POINT			///<��������ģʽ"."��
	, GBKEY_VK_V9_SYMBOL_SUBTRACT		///<��������ģʽ"-"��
	, GBKEY_VK_V9_SYMBOL_COMMA			///<��������ģʽ","��
	, GBKEY_VK_V9_PINYIN_SEPARATOR		///<ƴ���ָ�����"'"
	, GBKEY_VK_V9_SYMBOL_FULL_STOP		///<���
	, GBKEY_VK_V9_SYMBOL_KEY			///<���ż�,�ü�ֵ�Ƚ����⣬��Ӧ������ţ�������ݰ��¸ð����Ĵ�������ȡ�����Ӧ�ļ�ֵ��
										///<��ֵ������BUTTONCONSTDATA(���ڸýṹ��ο�:CLayoutInfo.h)�ṹ��btKeyName�����Ա��
	, GBKEY_VK_V9_SYMBOL_FACE_MULTIP_KEY  ///<�������(���ֵ)

	//����9��ģʽ�ļ������� end

	// ����������Ӽ�ֵ
	, GBKEY_VK_V9_UPPERCASE_FRENCH = 0x4500///<����9����д����
	, GBKEY_VK_V9_LOWERCASE_FRENCH		///<����9��Сд����
	, GBKEY_VK_V9_UPPERCASE_FRENCH_ABC	///<����9����д������ĸ
	, GBKEY_VK_V9_LOWERCASE_FRENCH_ABC	///<����9��Сд������ĸ
	
	, GBKEY_VK_V9_UPPERCASE_GERMAN		///<����9����д����
	, GBKEY_VK_V9_LOWERCASE_GERMAN		///<����9��Сд����
	, GBKEY_VK_V9_UPPERCASE_GERMAN_ABC	///<����9����д������ĸ
	, GBKEY_VK_V9_LOWERCASE_GERMAN_ABC	///<����9��Сд������ĸ
	
	, GBKEY_VK_V9_UPPERCASE_SPANISH		///<����9����д��������
	, GBKEY_VK_V9_LOWERCASE_SPANISH		///<����9��Сд��������
	, GBKEY_VK_V9_UPPERCASE_SPANISH_ABC	///<����9����д����������ĸ
	, GBKEY_VK_V9_LOWERCASE_SPANISH_ABC	///<����9��Сд����������ĸ
	
	, GBKEY_VK_V26_UPPERCASE_FRENCH		///<����26����д����
	, GBKEY_VK_V26_LOWERCASE_FRENCH		///<����26��Сд����
	, GBKEY_VK_V26_UPPERCASE_FRENCH_ABC	///<����26����д������ĸ
	, GBKEY_VK_V26_LOWERCASE_FRENCH_ABC	///<����26��Сд������ĸ
	
	, GBKEY_VK_V26_UPPERCASE_GERMAN		///<����26����д����
	, GBKEY_VK_V26_LOWERCASE_GERMAN		///<����26��Сд����
	, GBKEY_VK_V26_UPPERCASE_GERMAN_ABC	///<����26����д������ĸ
	, GBKEY_VK_V26_LOWERCASE_GERMAN_ABC	///<����26��Сд������ĸ
	
	, GBKEY_VK_V26_UPPERCASE_SPANISH	///<����26����д��������
	, GBKEY_VK_V26_LOWERCASE_SPANISH	///<����26��Сд��������	
	, GBKEY_VK_V26_UPPERCASE_SPANISH_ABC///<����26����д����������ĸ
	, GBKEY_VK_V26_LOWERCASE_SPANISH_ABC///<����26��Сд����������ĸ
	
	, GBKEY_VK_H9_UPPERCASE_FRENCH		///<����9����д����
	, GBKEY_VK_H9_LOWERCASE_FRENCH		///<����9��Сд����
	, GBKEY_VK_H9_UPPERCASE_FRENCH_ABC	///<����9����д������ĸ
	, GBKEY_VK_H9_LOWERCASE_FRENCH_ABC	///<����9��Сд������ĸ
	
	, GBKEY_VK_H9_UPPERCASE_GERMAN		///<����9����д����
	, GBKEY_VK_H9_LOWERCASE_GERMAN		///<����9��Сд����
	, GBKEY_VK_H9_UPPERCASE_GERMAN_ABC	///<����9����д������ĸ
	, GBKEY_VK_H9_LOWERCASE_GERMAN_ABC	///<����9��Сд������ĸ
	
	, GBKEY_VK_H9_UPPERCASE_SPANISH		///<����9����д��������
	, GBKEY_VK_H9_LOWERCASE_SPANISH		///<����9��Сд��������
	, GBKEY_VK_H9_UPPERCASE_SPANISH_ABC	///<����9����д����������ĸ
	, GBKEY_VK_H9_LOWERCASE_SPANISH_ABC	///<����9��Сд����������ĸ
	
	, GBKEY_VK_H26_UPPERCASE_FRENCH		///<����26����д����
	, GBKEY_VK_H26_LOWERCASE_FRENCH		///<����26��Сд����
	, GBKEY_VK_H26_UPPERCASE_FRENCH_ABC	///<����26����д������ĸ
	, GBKEY_VK_H26_LOWERCASE_FRENCH_ABC	///<����26��Сд������ĸ
	
	, GBKEY_VK_H26_UPPERCASE_GERMAN	///<����26����д����
	, GBKEY_VK_H26_LOWERCASE_GERMAN		///<����26��Сд����
	, GBKEY_VK_H26_UPPERCASE_GERMAN_ABC	///<����26����д������ĸ
	, GBKEY_VK_H26_LOWERCASE_GERMAN_ABC	///<����26��Сд������ĸ
	
	, GBKEY_VK_H26_UPPERCASE_SPANISH	///<����26����д��������
	, GBKEY_VK_H26_LOWERCASE_SPANISH	///<����26��Сд��������
	, GBKEY_VK_H26_UPPERCASE_SPANISH_ABC///<����26����д����������ĸ
	, GBKEY_VK_H26_LOWERCASE_SPANISH_ABC///<����26��Сд����������ĸ
	
	, GBKEY_VK_V_HANDWRITING_FRENCH		///<������д����
	, GBKEY_VK_V_HANDWRITING_GERMAN		///<������д����
	, GBKEY_VK_V_HANDWRITING_SPANISH	///<������д��������
	
	, GBKEY_VK_H_HANDWRITING_FRENCH		///<������д����
	, GBKEY_VK_H_HANDWRITING_GERMAN		///<������д����
	, GBKEY_VK_H_HANDWRITING_SPANISH	///<������д��������
	
	, GBKEY_VK_SMART_ASSOCIATE			///<���������
	, GBKEY_VK_CYCLE_PAGE_TURNING		///<ѭ����ҳ��
	, GBKEY_VK_LANG_SWITCH				///<�����л���
	
}; //enum PLATFORM_EVENT_ENUM

#define LAYOUTVK_IS_NUM_KEY(key) ((key) >= GBKEY_0 && (key) <= GBKEY_9)
#define IS_ENGLISH_CHAR(ch)	 ((ch) >= 'a' && (ch) <= 'z' || (ch) >= 'A' && (ch) <= 'Z')

#endif //__GB_PLATFORM_DEFINE_