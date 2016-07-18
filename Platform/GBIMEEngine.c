/*****************************************************************************
 *
 * Filename:
 * ---------
 *    GBIMEEngine.c
 *
 * Project:
 * --------
 *   Maui_Software
 *
 * Description:
 * ------------
 *   common header file for ZiEngine.c, T9Engine.c and CstarEngine.c
 *
 * Author:
 * -------
 * -------
 *
 *============================================================================
 *             HISTORY
 * Below this line, this part is controlled by PVCS VM. DO NOT MODIFY!!
 *------------------------------------------------------------------------------
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 *
 *------------------------------------------------------------------------------
 * Upper this line, this part is controlled by PVCS VM. DO NOT MODIFY!!
 *============================================================================
 ****************************************************************************/
#include "MMI_features.h"

#if defined(__GBIME__)
#include "Word_engine.h"
#include "GBIME.h"
#include "GB_Global.h"

/*****************************************************************************
 * FUNCTION
 *  mmi_ime_word_get_spellings
 * DESCRIPTION
 * PARAMETERS
 * RETURNS
 *****************************************************************************/
void mmi_ime_word_get_spellings( mmi_ime_query_param_struct_p param_ptr, mmi_ime_query_result_struct_p result_ptr )
{
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/

	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/
	//pidongming add for stoke quick search 080827
	if(param_ptr->elembuf_ptr == NULL)
		return;

	switch(*param_ptr->elembuf_ptr)
	{
	case MMI_IMC_SYMB_KEY_1:
		*result_ptr->result_buffer  =  0x4E00  ;
		result_ptr->result_cnt  =  1;
		break;
	case MMI_IMC_SYMB_KEY_2:
		*result_ptr->result_buffer  =  0x4E28 ;	
		result_ptr->result_cnt  =  1;
		break;
	case MMI_IMC_SYMB_KEY_3:
		*result_ptr->result_buffer  =  0x4E3F ;
		result_ptr->result_cnt  =  1;
		break;
	case MMI_IMC_SYMB_KEY_4:
		*result_ptr->result_buffer  =  0x4E36;
		result_ptr->result_cnt  =  1;
		break;
	case MMI_IMC_SYMB_KEY_5:     
#if 1//def __GB2312__
		*result_ptr->result_buffer  =  0xf005;
#else
		*result_ptr->result_buffer  =  0x4E5B;
#endif
		result_ptr->result_cnt  =  1;
		break;

#if 1//defined(__USE_GB_INPUT_PANEL__)		
	case MMI_IMC_SYMB_KEY_6:    //  [8/13/2009 pool]for"笔画快速查找下,实现按键6可以通配符使用""0x003f(?)" 
		*result_ptr->result_buffer  =  0x003F;
		result_ptr->result_cnt  =  1;
		break;
#endif

	default:
		result_ptr->result_cnt  =  0;
		break;

	}
	//end pidongming add
}


/*****************************************************************************
 * FUNCTION
 *  mmi_ime_word_get_candidates
 * DESCRIPTION
 * PARAMETERS
 * RETURNS
 *****************************************************************************/
void mmi_ime_word_get_candidates( mmi_ime_query_param_struct_p param_ptr, mmi_ime_query_result_struct_p result_ptr )
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
}


/*****************************************************************************
 * FUNCTION
 *  mmi_ime_word_get_associates
 * DESCRIPTION
 * PARAMETERS
 * RETURNS
 *****************************************************************************/
void mmi_ime_word_get_associates( mmi_ime_query_param_struct_p param_ptr, mmi_ime_query_result_struct_p result_ptr )
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
}


/*****************************************************************************
 * FUNCTION
 *  mmi_ime_word_set_context
 * DESCRIPTION
 * PARAMETERS
 * RETURNS
 *****************************************************************************/
void  mmi_ime_word_set_context( U8 context )
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
}


/*****************************************************************************
 * FUNCTION
 *  mmi_ime_word_is_valid_key
 * DESCRIPTION
 * PARAMETERS
 * RETURNS
 *****************************************************************************/
MMI_BOOL  mmi_ime_word_is_valid_key( mmi_imm_input_mode_enum input_mode, U16 key_code)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/

    return MMI_FALSE;
}


/*****************************************************************************
 * FUNCTION
 *  mmi_ime_word_symbol_to_code
 * DESCRIPTION
 * PARAMETERS
 * RETURNS
 *****************************************************************************/
UI_character_type mmi_ime_word_symbol_to_code(U16 symbol)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/

    return 0;
}


/*****************************************************************************
 * FUNCTION
 *  mmi_ime_word_code_to_symbol
 * DESCRIPTION
 * PARAMETERS
 * RETURNS
 *****************************************************************************/
U16  mmi_ime_word_code_to_symbol(UI_character_type code)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/

    return 0xFF;
}


/*****************************************************************************
 * FUNCTION
 *  mmi_ime_word_is_symbol_key
 * DESCRIPTION
 * PARAMETERS
 * RETURNS
 *****************************************************************************/
MMI_BOOL  mmi_ime_word_is_symbol_key( mmi_imm_input_mode_enum input_mode, mmi_imc_key_custom_value key_code )
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/

    return MMI_FALSE;
}


/*****************************************************************************
 * FUNCTION
 *  mmi_ime_word_is_redicals
 * DESCRIPTION
 * PARAMETERS
 * RETURNS
 *****************************************************************************/
MMI_BOOL  mmi_ime_word_is_redicals(UI_character_type key_code )
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/

    return MMI_FALSE;
}


/*****************************************************************************
 * FUNCTION
 *  mmi_ime_word_is_wildcard
 * DESCRIPTION
 * PARAMETERS
 * RETURNS
 *****************************************************************************/
MMI_BOOL mmi_ime_word_is_wildcard(UI_character_type code)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return MMI_FALSE;
}


/*****************************************************************************
 * FUNCTION
 *  mmi_ime_word_add_separator_to_candidate_buffer
 * DESCRIPTION
 * PARAMETERS
 * RETURNS
 *****************************************************************************/
void  mmi_ime_word_add_separator_to_candidate_buffer(UI_character_type * destination_buffer, UI_character_type * source_buffer, S32 num)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 i = 0, j = 0;
    
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    while (i < num && source_buffer[i] != 0)
    {
        destination_buffer[j] = source_buffer[i];
        destination_buffer[j + 1] = 0; /* Default separator is '\0' */
        i++;
        j += 2;
    }
	
    destination_buffer[j] = 0;
}

/*!
 * \brief 将一个字按照某个特定的输入模式转换为编码 (用于快速查找和拼音排序下的字符转换)
 * \param wc 要反查的字的Unicode
 * \param pCharInfoBuffer 输出Buffer, 用户需要保证此Buffer有不少于32个字节的空间
 * \param MaxInfoBuffer pCharInfoBuffer 空间大小
 * \param inputMode 输入模式, 参见mmi_imm_input_mode_enum
 * \return 反查是否成功
 * \note
 * \author weizhiping
 * \date 2009年12月11日
 */
static GBBOOL GBGetCharInfo(unsigned short wc, unsigned short * pCharInfoBuffer,
       						unsigned char MaxInfoBuffer, unsigned short inputMode) 
{
	GBPWCHAR temp_buffer = pCharInfoBuffer;
	int nGBInputMode = GBIM_Pinyin;
	GBRETURN ret;
	GBLANG backupLang = GBEngine_GetCurLang(Global_GetEnineInstance());
	GBU8 backupGBInputMode = GBEngine_GetInputMode(Global_GetEnineInstance());

	GBEngine_Reset(Global_GetEnineInstance());
	
	// 转换MTK输入模式为对应的GB输入模式
    switch (inputMode)
    {
#ifdef __MMI_GBIME_LANG_SM_CHINESE__
    case IMM_INPUT_MODE_QUICK_SEARCH_PINYIN:             
		nGBInputMode = GBIM_Qwerty_Pinyin;
        break;
    case IMM_INPUT_MODE_QUICK_SEARCH_SM_STROKE:
		nGBInputMode = GBIM_Stroke;
		break;
#endif
#ifdef __MMI_GBIME_LANG_TR_CHINESE__
	case IMM_INPUT_MODE_QUICK_SEARCH_BOPOMO:
		nGBInputMode = GBIM_Qwerty_Jyutping;
		break;		
    case IMM_INPUT_MODE_QUICK_SEARCH_TR_STROKE:
	    nGBInputMode = GBIM_Stroke;
        break;
#endif
	default:
    	#if defined(__MMI_GBIME_LANG_SM_CHINESE__) || defined(__MMI_GBIME_LANG_TR_CHINESE__) //只有繁体中文
			nGBInputMode = GBIM_Qwerty_Pinyin;
			break;
        #else
            return GBFalse;
		#endif
    }
	
	memset(pCharInfoBuffer, 0, MaxInfoBuffer);

	// 切换到中文输入法模式
	if ((ret = GBEngine_SwitchInputMode(Global_GetEnineInstance(), nGBInputMode, GBL_Chinese, GBL_Sub_Not_Spec)) != GBOK)
	{
		return GBFalse;
	}

	// [反查码表仍有问题]
	if ((ret = GBEngine_Word2CodesByInputmod(Global_GetEnineInstance(), nGBInputMode, wc, 0, pCharInfoBuffer)) != GBOK)
	{
		return GBFalse;
	}

	// 恢复原来的输入法模式
	if ((ret = GBEngine_SwitchInputMode(Global_GetEnineInstance(), backupGBInputMode, backupLang, GBL_Sub_Not_Spec)) != GBOK)
	{
		return GBFalse;
	}

	// 额外特殊处理
	if(nGBInputMode == GBIM_Stroke)
	{
		int i = 0;
		while(pCharInfoBuffer[i] !=0)
		{
			switch(pCharInfoBuffer[i]) 
			{
			case '1':
				*temp_buffer++ = 0x4E00;
				break;
			case '2':
				*temp_buffer++ = 0x4E28;
				break;
			case '3':
				*temp_buffer++ = 0x4E3F;     
				break;
			case '4':
				*temp_buffer++ = 0x4E36;
				break;
			case '5':
#if 1 //def __GB2312__
				*temp_buffer++ = 0xf005;
#else
				*temp_buffer++ = 0x4E5B;
#endif
				break;
			default:
				break;
			}
			i++;
		}
	}

	return GBTrue;
}

/*****************************************************************************
 * FUNCTION
 *  mmi_ime_word_engine_get_char_info
 * DESCRIPTION
 * PARAMETERS
 * RETURNS
 *****************************************************************************/
MMI_BOOL mmi_ime_word_engine_get_char_info(UI_character_type code, UI_string_type info_buffer, S32 buffer_size, mmi_imm_input_mode_enum mode)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/

    //return MMI_FALSE;
	return GBGetCharInfo(code, info_buffer, buffer_size, mode); 
}

/*****************************************************************************
 * FUNCTION
 *  mmi_ime_word_map_char_to_symbol
 * DESCRIPTION
 * PARAMETERS
 *  void
 * RETURNS
 *  void 
 *****************************************************************************/
mmi_imc_symbol_key_enum mmi_ime_word_map_char_to_symbol(mmi_imm_input_mode_enum input_mode, UI_character_type char_code)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return 0;
}


/*****************************************************************************
 * FUNCTION
 *  mmi_ime_word_init
 * DESCRIPTION
 * PARAMETERS
 * RETURNS
 *****************************************************************************/
MMI_BOOL mmi_ime_word_init( void )
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GBIME_Create();
    return MMI_TRUE;
}


/*****************************************************************************
 * FUNCTION
 *  mmi_ime_word_deinit
 * DESCRIPTION
 * PARAMETERS
 * RETURNS
 *****************************************************************************/
void mmi_ime_word_deinit( void )
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
   GBIME_Destroy();
}


/*****************************************************************************
 * FUNCTION
 *  mmi_ime_word_change_mode
 * DESCRIPTION
 * PARAMETERS
 * RETURNS
 *****************************************************************************/
MMI_BOOL mmi_ime_word_change_mode(const sIMEModeDetails * mode_details_ptr, U32 reserved )
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
	// 无法切换物理、9VK、26VK
	GBIME_SwitchInputMode(mode_details_ptr->imm_mode_id); // 平台通知输入框架切换输入模式

    return MMI_TRUE;
}


/*****************************************************************************
 * FUNCTION
 *  mmi_ime_word_leave_current_mode
 * DESCRIPTION
 * PARAMETERS
 * RETURNS
 *****************************************************************************/
void  mmi_ime_word_leave_current_mode(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
}

/*****************************************************************************
 * FUNCTION
 *  mmi_ime_word_change_word_case
 * DESCRIPTION
 * PARAMETERS
 * RETURNS
 *****************************************************************************/
void   mmi_ime_word_change_word_case(mmi_ime_word_capital_state_enum capital_state)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
}

#if defined(__MMI_IME_MAGIC_ONE__)
/*****************************************************************************
 * FUNCTION
 *  mmi_ime_word_is_magic_key
 * DESCRIPTION
 * PARAMETERS
 * RETURNS
 *****************************************************************************/
MMI_BOOL mmi_ime_word_is_magic_key( mmi_imm_input_mode_enum input_mode, mmi_imc_key_custom_value key_code )
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return MMI_FALSE;
}
#endif

#endif /* #if defined(__MMI_NO_IME_VENDOR__) */
