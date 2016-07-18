#ifndef __GBHW_H__
#define __GBHW_H__

//识别范围：数字
#define GBHW_OPT_RANGE_NUMBER				0x1
//识别范围：小写字母
#define GBHW_OPT_RANGE_LOWER_CHAR			0x2
//识别范围：大写字母
#define GBHW_OPT_RANGE_UPPER_CHAR			0x4
//识别范围：半角标点符号
#define GBHW_OPT_RANGE_ASC_PUNCTUATION		0x8
//识别范围：ASCII码，包含数字、小写字母、大写字母和半角标点符号
#define GBHW_OPT_RANGE_ASCII				(GBHW_OPT_RANGE_NUMBER | GBHW_OPT_RANGE_LOWER_CHAR | GBHW_OPT_RANGE_UPPER_CHAR | GBHW_OPT_RANGE_ASC_PUNCTUATION)
//识别范围：GB2312汉字
#define GBHW_OPT_RANGE_GB2312				0x8000 
//识别范围：BIG5汉字
#define GBHW_OPT_RANGE_BIG5					0x200
//识别范围：GBK汉字
#define GBHW_OPT_RANGE_GBK					0x400
//识别范围：中文标点符号
#define GBHW_OPT_RANGE_CHN_PUNCTUATION		0x800
//识别范围：GB18030-2003所规定的汉字
#define GBHW_OPT_RANGE_GB18030				0x1000
//识别范围：空格、回车以及删除等三个控制字符
#define GBHW_OPT_RANGE_CONTROL_CHAR			0x2000

#define GBHW_OPT_RANGE_PUNCTUATION	(GBHW_OPT_RANGE_ASC_PUNCTUATION | GBHW_OPT_RANGE_CHN_PUNCTUATION)

//识别范围：默认识别范围，为GB2312汉字、数字、字母和标点符号。不包括控制符号
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
		, GBHW_Err_Init_Null_Data //空数据
		, GBHW_Err_Init_Invalid_Data //数据格式错误
		, GBHW_Err_Recg_Invalid_Result_Len //要求的候选数目有误
		, GBHW_Err_Init_Invalid_Dim //数据中数据的维数超出范围
		, GBHW_Err_Option_Invalid_Speed //GBHWSetOption函数设置GBHW_Index_Set_Speed的时候，param参数超出有效值
		, GBHW_Err_Option_Invalid_Index //GBHWSetOption函数index参数无效

};

enum GB_HW_Option_Index {
	GBHW_Invalid_Index = 0
		/*
		value为0的时候，设置为默认的速度
		value值越大，则速度越慢，准确率越高，
		反之速度越快，准确率越低；
		有效区间为[5, 30]
		这个设置需要在引擎启动之后(GBHWNew)设置，才能生效。
		*/
		, GBHW_Index_Set_Speed 
};

// 识别引擎初始化
// 调用识别引擎前必须调用该函数进行初始化
// 返回0表示成功 
#ifdef _C166
GBAPI int GBHWNew(const void _huge * data,void* param);
#else
GBAPI int GBHWNew(const void * data,void* param);
#endif

//识别引擎主要的函数
//pbTrace (in) 字轨迹，为了节省空间，引擎会修改这个数组的值
//result (out) Unicode 内码输出
//candNum (in)需要的候选个数，最大不能超过MAX_RESULT_COUNT
//option (in)识别参数，GB_OPT中不同值的组合
//prob (out)识别概率输出, 取值范围是(0-1000), 实际值的1000倍。
//返回结果的个数
GBAPI int GBHWRecognize(short* pbTrace, unsigned short * result, int candNum, unsigned long option);
GBAPI int GBHWRecognizeEx(short* pbTrace, unsigned short * result, int candNum, unsigned long option,long *prob);

/*!
 * \brief 设置识别引擎识别参数
 * \param index 范围是GB_HW_Option_Index里面的值
 * \param value 根据index定义不同，详细请参照每一个index的说明
 * \ret 如果index为非法值，则返回GBHW_Err_Option_Invalid_Index 
 *      成功返回GBHW_OK
 *      其他返回值请查看每一个错误值的说明
 */
GBAPI int GBHWSetOption(int index, unsigned long value);
#ifdef __cplusplus
}
#endif

#endif
