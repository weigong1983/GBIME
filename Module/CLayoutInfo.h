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
	GBRECT   rcPos;		///< rect(x,y)ͼƬ��ʾ�����Ͻ�λ�����꣬rect(w,h)ͼƬ��С
	GBUINT16 imageID;	///< ͼƬID
}GBImage; ///< vc sizeof == 10

/*!
 *- Button Const Information
 */
typedef struct __tag_ButtonConstData
{
	GBRECT					btPos;								///<������������뷨windows������
	GBUINT16			    btKey;								///<��ֵ
	GBImage					btNormal;						    ///<��ǰ��ʾͼƬ
	GBImage					btDown;							    ///<����ͼƬ
	GBImage					btGrey;							    ///<��ɫͼƬ
	GBUINT16				btKeyName[MAX_BUTTON_KEY_NAME_LEN];	///<����
}BUTTONCONSTDATA, * PBUTTONCONSTDATA; ///< vc sizeof == 82
typedef const BUTTONCONSTDATA _gb_far *         GBLPCBUTTONCONSTDATA;

/*!
 *- Button Varia & Const Information
 */
typedef struct __tag_ButtonChangedData
{
//	GBUINT16			    btFlag;								///<���ű��,�Ƿ�����������   
// 	GBUACHAR			    btIsIcon;							///<�Ƿ���ʾ�������е�szKeyName��normalBmp��downBmp��greyBmp
 	GBUINT16			    btStatus;							///<����״̬��0Ϊ����̬��1Ϊ���£�2Ϊ��ɫ��3���ص�ǰ����
	GBPCVOID                btpfcResponse;                      ///<button��Ӧ����Ӧ����ָ��
	GBPWCHAR                btpChgKeyName;					    ///<��Щbutton�ı�����Ҫ�޸ĵģ���ʻ��Ĳ��ף���������ַ���
} BUTTONCHGDATA, *PBUTTONCHGDATA; //vc sizeof == 12

typedef const BUTTONCHGDATA _gb_far *         GBLPCBUTTONCHGDATA;

/*!
 *- Layout Information
 */
typedef struct __Tag_LayoutConstLayer{
	GBRECT					lyPos;								///<������������뷨windows������

	GBUINT16			    lyImageID;							///< Layer Image ID
	GBUINT16			    lyBKImageID;						///< Layer Background Image ID

	GBUINT16                __pack_1;
	GBUINT16			    lyBtCount; 
	//GBLPCBUTTONCONSTDATA    lypBtData;						    ///< ��������ϵ����а�ť������
}LAYOUTCONSTLAYER, *PLAYOUTCONSTLAYER;

typedef const LAYOUTCONSTLAYER _gb_far *         GBLPCLAYOUTCONSTLAYER;

/*!
 *- Layout Information
 */
typedef struct __Tag_LayoutChgtLayer
{
	GBUINT16			    lyBtCount; 
	GBLPCLAYOUTCONSTLAYER   plycData;                           ///<
	GBLPCBUTTONCONSTDATA    plycBtData;						    ///< ��������ϵ����а�ť������
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
	GBUINT16 ltIMLang;                                        ///< ���뷨�������͡�ֵ��Ӧgbim.h��GBLanguageEnum���塣
	GBUINT16 ltIMSubLang;                                     ///< ���뷨���������͡�ֵ��Ӧgbim.h��GBSubLanguageEnum���塣   
	GBUINT16 ltIMMode;                                        ///< ���뷨���ͼ���ֵ��Ӧgbim.h��GBInputModeEnum���塣

	GBUINT16 ltScrSwtID;                                      ///< ������ת��,ת����Ӧ�ĺ������� 
	GBUINT16 ltUPSwtID;                                       ///< ���ϼ�, ת����Ӧ��Layout ID 
	GBUINT16 ltDownSwtID;                                     ///< ���¼�, ת����Ӧ��Layout ID  
	GBUINT16 ltLeftSwtID;                                     ///< �����, ת����Ӧ��Layout ID 
	GBUINT16 ltRightSwtID;                                    ///< ���Ҽ�, ת����Ӧ��Layout ID 

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

/** \brief ͨ��Layout ID ֵ��ȡ��ӦLayout ����ָ��*/
typedef GBLPCLAYOUTCONSTINFO (* FPGetLayoutDataByID)(GBLPCVOID pskinData, GBUINT16 ltID);
/** \brief ��ʼ��Ƥ������ */
//typedef GBINT32 (* FPSkinInitData)(PSkinData pskInfo, GBPCVOID pskcData);

typedef struct __Tag_SkinData
{
	GBPCVOID              pskcData;
	GBUINT16              ltCount;
#ifdef Layout_Test //Layout������Ҫ��̬����
	PLayoutInfo           pskLtData;
#endif
	GBUINT16			  ltDefID;                      ///< input method type
	//GBUINT16			  ltCurID;                      ///< input method type
	///< im��¼��Ӧ���뷨,���ֵ����"PPC���뷨"��һ������������������,�������Ҫ����ʵ�����,���ܻ�����
	///< ��Ӧ���뷨, PINYIN, STROKE, SMART_ENGLISH
	//GBUINT16 ltIMLang;                                        ///< ���뷨�������͡�
	//GBUINT16 ltIMSub;                                         ///< ���뷨���ͼ������磬ƴ�����ʻ���

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

//Layout���
typedef enum __tag_LayerIndex
{
	FirstLayer,
	SecondLayer,
	ThirdLayer,
	FourthLayer
}LayerIndex;
//��ť״̬
typedef enum _ButtonStatusEnum
{
	ButtonStatusNomal,
	ButtonStatusDown,
	ButtonStatusUp,
	ButtonStatusMove,
	ButtonStatusGray,
	ButtonStatusHide
}ButtonStatusEnum;

//Layout����
typedef enum _LayoutIdEnum
{
	LayoutIdSP9JPinYin			= 0	,//����9��ƴ��
	LayoutIdSP9JStroke			= 1	,//����9���ʻ�
	LayoutIdSP9JLowerEnglish	= 2	,//����9��СдӢ��
	LayoutIdSP9Jabc				= 3	,//����9��Сд��ĸ
	LayoutIdSP9JABC				= 4	,//����9����д��ĸ
	LayoutIdSP9JNumber			= 5	,//����9������
	LayoutIdSP9JSymbol			= 6	,//����9������
	LayoutIdSP26JPinYin			= 7	,//����26��ƴ��
	LayoutIdSP26JStroke			= 8	,//����26���ʻ�
	LayoutIdSP26JLowerEnglish	= 9	,//����26��СдӢ��
	LayoutIdSP26Jabc			= 10,//����26��Сд��ĸ
	LayoutIdSP26JABC			= 11,//����26����д��ĸ
	LayoutIdSP26JNumber			= 12,//����26������
	LayoutIdSP26JSymbol			= 13,//����26������
	LayoutIdHP9JPinYin			= 14,//����9��ƴ��
	LayoutIdHP9JStroke			= 15,//����9���ʻ�
	LayoutIdHP9JEnglish			= 16,//����9��Ӣ��
	LayoutIdHP9Jabc				= 17,//����9��Сд��ĸ
	LayoutIdHP9JABC				= 18,//����9����д��ĸ
	LayoutIdHP9JNumber			= 19,//����9������
	LayoutIdHP9JSymbol			= 20,//����9������
	LayoutIdHP26JPinYin			= 21,//����26��ƴ��
	LayoutIdHP26JStroke			= 22,//����26���ʻ�
	LayoutIdHP26JEnglish		= 23,//����26��Ӣ��
	LayoutIdHP26Jabc			= 24,//����26��Сд��ĸ
	LayoutIdHP26JABC			= 25,//����26����ĸ��д
	LayoutIdHP26JNumber			= 26,//����26������
	LayoutIdHP26JSymbol			= 27,//����26������
	LayoutIdSPGeZiShouXiue		= 28,//����������д
	LayoutIdHPGeZiShouXiue		= 29,//����������д
	LayoutIdSPQuanPingShouXiue	= 30,//����ȫ����д
	LayoutIdHPQuanPingShouXiue	= 31,//����ȫ����д
	LayoutIdOneRowCandidate		= 32,//һ�к�ѡ��
	LayoutIdTowRowCandidate		= 33,//���к�ѡ��
	LayoutIdSP9JUpEnglish		= 34,//����9����дӢ��
	LayoutIdSP26JUpEnglish		= 35,//����26����дӢ��

	LayoutIdWL9JPinYin			= 36,//	����9��ƴ��
	LayoutIdWL9JStroke			= 37,//	����9���ʻ�
	LayoutIdWL9JEnglish			= 38,//	����9��Ӣ��
	LayoutIdWL26JPinYin			= 39,//	����26��ƴ��
	LayoutIdWL26JStroke			= 40,//	����26���ʻ�
	LayoutIdWL26JEnglish		= 41,//	����26��Ӣ��
	LayoutIdWL9JABC				= 42,//	����9����д��ĸ
	LayoutIdWL9Jabc				= 43,//	����9��Сд��ĸ
	LayoutIdWL9JNumber 			= 44,//	����9������
	LayoutIdWL26JABC			= 45,//	����26����д��ĸ
	LayoutIdWL26Jabc			= 46,//	����26��Сд��ĸ	
	LayoutIdWL26JNumber 		= 47,//	����26������
	
	// ����MTKƽ̨Ӧ��ר��Layout	
	LayoutIdPassword_V			= 48,//������������
	LayoutIdFloatNumber_V		= 49,//������������
	LayoutIdPhoneNumber_V		= 50,//�����绰����
// 	LayoutIdQuickSearchPinyin9_V= 51,//����ƴ��9�����ٲ���
// 	LayoutIdQuickSearchPinyin26_V= 52,//����ƴ��26�����ٲ���
// 	LayoutIdQuickSearchStroke9_V= 53,//�����ʻ�9�����ٲ���
// 	LayoutIdQuickSearch26Abc_V	= 54,//����9��abc���ٲ���
// 	LayoutIdQuickSearch9Abc_V	= 55,//����26��abc���ٲ���

	// ����������LayoutID
	LayoutId_SP9_UPPERCASE_FRENCH = 60 // ����9����д����
	,LayoutId_SP9_LOWERCASE_FRENCH = 61 // ����9��Сд����
	,LayoutId_SP9_UPPERCASE_FRENCH_ABC = 62 // ����9����д������ĸ
	,LayoutId_SP9_LOWERCASE_FRENCH_ABC = 63 // ����9��Сд������ĸ
	
	,LayoutId_SP9_UPPERCASE_GERMAN = 64 // ����9����д����
	,LayoutId_SP9_LOWERCASE_GERMAN = 65 // ����9��Сд����
	,LayoutId_SP9_UPPERCASE_GERMAN_ABC = 66 // ����9����д������ĸ
	,LayoutId_SP9_LOWERCASE_GERMAN_ABC = 67 // ����9��Сд������ĸ
	
	,LayoutId_SP9_UPPERCASE_SPANISH = 68 // ����9����д��������
	,LayoutId_SP9_LOWERCASE_SPANISH = 69 // ����9��Сд��������
	,LayoutId_SP9_UPPERCASE_SPANISH_ABC = 70 // ����9����д����������ĸ
	,LayoutId_SP9_LOWERCASE_SPANISH_ABC = 71 // ����9��Сд����������ĸ
	
	,LayoutId_SP26_UPPERCASE_FRENCH = 72 // ����26����д����
	,LayoutId_SP26_LOWERCASE_FRENCH = 73 // ����26��Сд����
	,LayoutId_SP26_UPPERCASE_FRENCH_ABC = 74 // ����26����д������ĸ
	,LayoutId_SP26_LOWERCASE_FRENCH_ABC = 75 // ����26��Сд������ĸ
	
	,LayoutId_SP26_UPPERCASE_GERMAN = 76 // ����26����д����
	,LayoutId_SP26_LOWERCASE_GERMAN = 77 // ����26��Сд����
	,LayoutId_SP26_UPPERCASE_GERMAN_ABC = 78 // ����26����д������ĸ
	,LayoutId_SP26_LOWERCASE_GERMAN_ABC = 79 // ����26��Сд������ĸ
	
	,LayoutId_SP26_UPPERCASE_SPANISH = 80 // ����26����д��������
	,LayoutId_SP26_LOWERCASE_SPANISH = 81 // ����26��Сд��������
	,LayoutId_SP26_UPPERCASE_SPANISH_ABC = 82 // ����26����д����������ĸ
	,LayoutId_SP26_LOWERCASE_SPANISH_ABC = 83 // ����26��Сд����������ĸ
	
	,LayoutId_HP9_UPPERCASE_FRENCH = 84 // ����9����д����
	,LayoutId_HP9_LOWERCASE_FRENCH = 85 // ����9��Сд����
	,LayoutId_HP9_UPPERCASE_FRENCH_ABC = 86 // ����9����д������ĸ
	,LayoutId_HP9_LOWERCASE_FRENCH_ABC = 87 // ����9��Сд������ĸ
	
	,LayoutId_HP9_UPPERCASE_GERMAN = 88 // ����9����д����
	,LayoutId_HP9_LOWERCASE_GERMAN = 89 // ����9��Сд����
	,LayoutId_HP9_UPPERCASE_GERMAN_ABC = 90 // ����9����д������ĸ
	,LayoutId_HP9_LOWERCASE_GERMAN_ABC = 91 // ����9��Сд������ĸ
	
	,LayoutId_HP9_UPPERCASE_SPANISH = 92 // ����9����д��������
	,LayoutId_HP9_LOWERCASE_SPANISH = 93 // ����9��Сд��������
	,LayoutId_HP9_UPPERCASE_SPANISH_ABC = 94 // ����9����д����������ĸ
	,LayoutId_HP9_LOWERCASE_SPANISH_ABC = 95 // ����9��Сд����������ĸ
	
	,LayoutId_HP26_UPPERCASE_FRENCH = 96 // ����26����д����
	,LayoutId_HP26_LOWERCASE_FRENCH = 97 // ����26��Сд����
	,LayoutId_HP26_UPPERCASE_FRENCH_ABC = 98 // ����26����д������ĸ
	,LayoutId_HP26_LOWERCASE_FRENCH_ABC = 99 // ����26��Сд������ĸ
	
	,LayoutId_HP26_UPPERCASE_GERMAN = 100 // ����26����д����
	,LayoutId_HP26_LOWERCASE_GERMAN = 101 // ����26��Сд����
	,LayoutId_HP26_UPPERCASE_GERMAN_ABC = 102 // ����26����д������ĸ
	,LayoutId_HP26_LOWERCASE_GERMAN_ABC = 103 // ����26��Сд������ĸ
	
	,LayoutId_HP26_UPPERCASE_SPANISH = 104 // ����26����д��������
	,LayoutId_HP26_LOWERCASE_SPANISH = 105 // ����26��Сд��������
	,LayoutId_HP26_UPPERCASE_SPANISH_ABC = 106 // ����26����д����������ĸ
	,LayoutId_HP26_LOWERCASE_SPANISH_ABC = 107 // ����26��Сд����������ĸ
	
	,LayoutId_SP_HANDWRITING_FRENCH = 108 // ������д����
	,LayoutId_SP_HANDWRITING_GERMAN = 109 // ������д����
	,LayoutId_SP_HANDWRITING_SPANISH = 110 // ������д��������
	
	,LayoutId_HP_HANDWRITING_FRENCH = 111 // ������д����
	,LayoutId_HP_HANDWRITING_GERMAN = 112 // ������д����
	,LayoutId_HP_HANDWRITING_SPANISH = 113, // ������д��������

	// ������LyoutID����ӵ�����֮ǰ
	LayoutIdEnd						= 0x0FFF,

	// Ƥ�����߶��Ƶ�Layout�л�ָ��
	LayoutIdSwitchPreObj			= 0x1000, ///<�����ؼ��л�����һ��Layout
	LayoutIdSwitchLatestObj			= 0x1001, ///<�л������һ��Layout
	LayoutIdSwitchLatestChnObj		= 0x1002, ///<�л������һ������ Layout
	LayoutIdSwitchLatestAlpObj		= 0x1003, ///<�л������һ��Alp Layout
	LayoutIdHide					= 0x1004  ///<����Layout
}LayoutIdEnum;
///<start [haj add 2010-8-27]

/*!
 *- ���LayoutID�Ƿ���Ч
 */
#define IS_VALID_LAYOUT_ID(layoutID) (layoutID >= LayoutIdSP9JPinYin && layoutID <= LayoutIdEnd) \
								     || (layoutID >= LayoutIdSwitchPreObj && layoutID <= LayoutIdHide)

enum __tag_LayoutType
{
	LayoutHandwriteFullScreen	///<ȫƴ��д
	,LayoutHandwriteMultiBlock  ///<������д����ʱ�������г�����ࣩ
	
	,LayoutVK9Pinyin			///<����9��--ƴ����
	,LayoutVK9Stroke			///<����9��--�ʻ���
	,LayoutVK9MultiLang			///<����9��--��������
	
	,LayoutVK26Pinyin			///<����ȫ����ƴ����
	,LayoutVK26Stroke			///<����ȫ���̱ʻ���
	,LayoutVK26MultiLang		///<����ȫ���̶�������
	
	,LayoutVKNumber				///<��ֱֵ��������--����
	,LayoutVKSymbol				///<��ֱֵ��������--����
	
	,LayoutVKCopyPaste			///<����ճ������ʱ�����������뷨��Ʒ���빦�ܣ��Ժ������ƻ�����
	,LayoutVKProductConfig		///<��Ʒ���ã���ʱ������Ŀǰ��ƽ̨�˵���ʽ���ò�Ʒ����
	
	,LayoutKB9SingleRow			///<����9������
	,LayoutKB26SingleRow		///<����Qwerty����
	,LayoutKB9TwinRow			///<����9��˫��
	,LayoutKB26TwinRow			///<����Qwerty˫��
	,LayoutFullScreenSymbol		///<ȫƴ������
};

// ��������
typedef enum _tagkeyboardType
{
	KEYBOARD_VK_NUMPAD		// ���ּ���
	,KEYBOARD_VK_QWERTY		// Qwerty����
	,KEYBOARD_KB_NUMPAD		//����Number����
	,KEYBOARD_KB_QWERTY		//����Qwerty����
	,KEYBOARD_NUM
}KEYBOARD_TYPE;

typedef struct __tag_LayoutKey2LayoutID_Mapping{
	GBUINT16 layoutKey;
	GBUINT16 layoutID;
}MAPPING_LAYOUTKEY2LAYOUTID, * PMAPPING_LAYOUTKEY2LAYOUTID;

typedef const MAPPING_LAYOUTKEY2LAYOUTID * PCMAPPING_LAYOUTKEY2LAYOUTID;

extern const MAPPING_LAYOUTKEY2LAYOUTID GBCMappingLayoutKey2ID[];
///<end [haj add 2010-8-27]

//�����ʰ���ʱ���ڵ�λ��
typedef enum _PenPosition
{
	PenPositionInNothing,//���ڴ���Χ��ʱ
	PenPositionInSyllable,//�����½�ʱ
	PenPositionInCandidate,//���к�ѡʱ
	PenPositionInKeyboardButton,//���м����ϵİ�ťʱ
	PenPositionInSyllablePageDown,//����ƴ�����·�ҳ
	PenPositionInSyllablePageUp,//����ƴ�����Ϸ�ҳ
	PenPositionInCandidatePageDown,//���к�ѡ���·�ҳ
	PenPositionInCandidatePageUp,//���к�ѡ���Ϸ�ҳ
	PenPositionLeave//�������뿪����λ��
}PenPosition;

/*!
 * \brief �����л�layout������
 */
typedef enum {
	SLIDING_DIRECTION_UP		///<��
	,SLIDING_DIRECTION_DOWN		///<��
	,SLIDING_DIRECTION_LEFT		///<��
	,SLIDING_DIRECTION_RIGHT	///<��
}GBSlidingDirection;

/*!
 * \ �жϵ�ǰlayoutID�����������
 */
#define IS_VK_LAYOUT(layoutID)	((layoutID) >=LayoutIdSP9JPinYin &&  (layoutID) <= LayoutIdSP26JUpEnglish)

/*!
 * \ �жϵ�ǰlayoutID�����������
 */
#define IS_KB_LAYOUT(layoutID)	((layoutID) >=LayoutIdWL9JPinYin &&  (layoutID) <= LayoutIdWL26JNumber)

/*!
 * \ �жϵ�ǰlayoutIDΪ��дlayout
 */
#define IS_HANDWRITE_LAYOUT(layoutID)	((layoutID) >=LayoutIdSPGeZiShouXiue &&  (layoutID) <= LayoutIdHPQuanPingShouXiue)

/*!
 * \ �ж��Ƿ��������ּ��̣���ʱ����Ҫ��ѡ��
 */
#define IS_PHYSIC_NUMBER_LAYOUT(layoutID) (layoutID == LayoutIdWL9JNumber || layoutID == LayoutIdWL26JNumber)

/*!
 * \ �ж��Ƿ��дӢ�ġ���ĸ�ͼ���
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
 * \brief ����LayoutID��ȡ�����л�����ģʽ�л���Ҫ����Ϣ
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
 * \brief ��Layout��Ϣ�л�ȡLayoutID
 * \param pltInfo 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-9 17:24:48
*/
GBUINT16 CLayoutInfo_GetLayoutID(PLayoutInfo pltInfo);

/*!
 * \brief ���ݻ���������Ŀ���л�LayoutID
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
 * \brief ��ȡLayoutID��Ӧ�ļ�������
 * \param layoutID 
 * \return 
 * \note 
 * \example
 * \author weizhiping
 * \date 2010-9-29 9:56:33
*/
KEYBOARD_TYPE CLayoutInfo_GetKeyboardTypeByLayoutID(GBUINT16 layoutID);

/*!
 * \brief ��ȡСдLayoutID��Ӧ�Ĵ�дLayoutID
 * \param lowerLayoutID 
 * \return 
 * \note Ѱ�Ҳ�����������
 * \example
 * \author weizhiping
 * \date 2010-10-21 17:23:09
*/
GBUINT16 CLayoutInfo_GetUpperLayoutID(GBUINT16 lowerLayoutID);

/*!
 * \brief ��ȡ��дLayoutID��Ӧ��СдLayoutID
 * \param upperLayoutID 
 * \return 
 * \note Ѱ�Ҳ�����������
 * \example
 * \author weizhiping
 * \date 2010-10-21 17:23:09
*/
GBUINT16 CLayoutInfo_GetLowerLayoutID(GBUINT16 upperLayoutID);

/*!
 * \brief ��ȡָ����ֵ�İ�ť����
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
 * \brief ����ָ��Layout��ť��״̬(Ӧ�ò�����ô˽ӿڽ���ָ����ť)
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