/**
 * @file
 * @brief define global var.
 * @author Zeng Xuedong
 * @date 2012-10-21
 */
#ifdef DEF_GLOBALS
#define EXT
#else
#define EXT extern
#endif
/** @var unsigned int res[2]
 * @brief Place to hold ADC raw result
 */
EXT unsigned int res[2];
EXT unsigned int res_avg,res_pre,res_pre1;
EXT unsigned int max_pre,max_cur;
EXT char upflag,adc_comp;
EXT unsigned char SLV_Addr;
EXT unsigned char i2c_st_fg;
EXT unsigned char i2c_flag;
EXT unsigned char I2C_State;
EXT unsigned char Bytecount;
EXT unsigned char transmit;
EXT unsigned char reg_files[MAX_RCV_NUM];
EXT unsigned reg_ptr;                        //Control Registers and access pointer
EXT unsigned char ch1_light;
EXT unsigned char ch2_light;
EXT unsigned char ch3_light;
EXT unsigned char ch4_light;      //channel 1-4 lighting control register range 0-100
EXT unsigned char detect_zero;
EXT unsigned char Group_Mask;
EXT unsigned char pw_idx;
EXT unsigned char pwoff_cnt;
EXT unsigned int zero_thd;

