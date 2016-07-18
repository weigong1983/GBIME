#ifndef __GBHW_H__
#define __GBHW_H__

//ʶ��Χ������
#define GBHW_OPT_RANGE_NUMBER				0x1
//ʶ��Χ��Сд��ĸ
#define GBHW_OPT_RANGE_LOWER_CHAR			0x2
//ʶ��Χ����д��ĸ
#define GBHW_OPT_RANGE_UPPER_CHAR			0x4
//ʶ��Χ����Ǳ�����
#define GBHW_OPT_RANGE_ASC_PUNCTUATION		0x8
//ʶ��Χ��ASCII�룬�������֡�Сд��ĸ����д��ĸ�Ͱ�Ǳ�����
#define GBHW_OPT_RANGE_ASCII				(GBHW_OPT_RANGE_NUMBER | GBHW_OPT_RANGE_LOWER_CHAR | GBHW_OPT_RANGE_UPPER_CHAR | GBHW_OPT_RANGE_ASC_PUNCTUATION)
//ʶ��Χ��GB2312����
#define GBHW_OPT_RANGE_GB2312				0x8000 
//ʶ��Χ��BIG5����
#define GBHW_OPT_RANGE_BIG5					0x200
//ʶ��Χ��GBK����
#define GBHW_OPT_RANGE_GBK					0x400
//ʶ��Χ�����ı�����
#define GBHW_OPT_RANGE_CHN_PUNCTUATION		0x800
//ʶ��Χ��GB18030-2003���涨�ĺ���
#define GBHW_OPT_RANGE_GB18030				0x1000
//ʶ��Χ���ո񡢻س��Լ�ɾ�������������ַ�
#define GBHW_OPT_RANGE_CONTROL_CHAR			0x2000

#define GBHW_OPT_RANGE_PUNCTUATION	(GBHW_OPT_RANGE_ASC_PUNCTUATION | GBHW_OPT_RANGE_CHN_PUNCTUATION)

//ʶ��Χ��Ĭ��ʶ��Χ��ΪGB2312���֡����֡���ĸ�ͱ����š����������Ʒ���
#define GBHW_OPT_RANGE_DEFAULT	(GBHW_OPT_RANGE_GB2312 | GBHW_OPT_RANGE_CONTROL_CHAR)
#define GBHW_OPT_RANGE_ALL		(GBHW_OPT_RANGE_GB2312 | GBHW_OPT_RANGE_CONTROL_CHAR | GBHW_OPT_RANGE_ASCII | GBHW_OPT_RANGE_PUNCTUATION)

#ifndef GBAPI
#ifdef GBIMPORT
#define GBAPI  __declspec(dllimport)
#else
#define GBAPI	extern
#endif
#endif

#ifdef __cplusplus
extern "C"{
#endif

enum GB_HW_Fail {
	GBHW_OK = 0
		, GBHW_Err_Begin = -1000
		, GBHW_Err_Not_Init
		, GBHW_Err_Init_Null_Data //������
		, GBHW_Err_Init_Invalid_Data //���ݸ�ʽ����
		, GBHW_Err_Recg_Invalid_Result_Len //Ҫ��ĺ�ѡ��Ŀ����
		, GBHW_Err_Init_Invalid_Dim //���������ݵ�ά��������Χ
		, GBHW_Err_Option_Invalid_Speed //GBHWSetOption��������GBHW_Index_Set_Speed��ʱ��param����������Чֵ
		, GBHW_Err_Option_Invalid_Index //GBHWSetOption����index������Ч

};

enum GB_HW_Option_Index {
	GBHW_Invalid_Index = 0
		/*
		valueΪ0��ʱ������ΪĬ�ϵ��ٶ�
		valueֵԽ�����ٶ�Խ����׼ȷ��Խ�ߣ�
		��֮�ٶ�Խ�죬׼ȷ��Խ�ͣ�
		��Ч����Ϊ[5, 30]
		���������Ҫ����������֮��(GBHWNew)���ã�������Ч��
		*/
		, GBHW_Index_Set_Speed 
};

// ʶ�������ʼ��
// ����ʶ������ǰ������øú������г�ʼ��
// ����0��ʾ�ɹ� 
#ifdef _C166
GBAPI int GBHWNew(const void _huge * data,void* param);
#else
GBAPI int GBHWNew(const void * data,void* param);
#endif

//ʶ��������Ҫ�ĺ���
//pbTrace (in) �ֹ켣��Ϊ�˽�ʡ�ռ䣬������޸���������ֵ
//result (out) Unicode �������
//candNum (in)��Ҫ�ĺ�ѡ����������ܳ���MAX_RESULT_COUNT
//option (in)ʶ�������GB_OPT�в�ֵͬ�����
//prob (out)ʶ��������, ȡֵ��Χ��(0-1000), ʵ��ֵ��1000����
//���ؽ���ĸ���
GBAPI int GBHWRecognize(short* pbTrace, unsigned short * result, int candNum, unsigned long option);
GBAPI int GBHWRecognizeEx(short* pbTrace, unsigned short * result, int candNum, unsigned long option,long *prob);

/*!
 * \brief ����ʶ������ʶ�����
 * \param index ��Χ��GB_HW_Option_Index�����ֵ
 * \param value ����index���岻ͬ����ϸ�����ÿһ��index��˵��
 * \ret ���indexΪ�Ƿ�ֵ���򷵻�GBHW_Err_Option_Invalid_Index 
 *      �ɹ�����GBHW_OK
 *      ��������ֵ��鿴ÿһ������ֵ��˵��
 */
GBAPI int GBHWSetOption(int index, unsigned long value);
#ifdef __cplusplus
}
#endif

#endif
