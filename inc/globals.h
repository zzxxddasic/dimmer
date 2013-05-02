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
EXT volatile unsigned int res[2];
EXT volatile unsigned int res_avg,res_pre,res_pre1;
EXT volatile unsigned int max_pre,max_cur;
EXT volatile char upflag,detect_period;
EXT volatile unsigned char SLV_Addr;
EXT volatile unsigned char i2c_st_fg;
EXT volatile unsigned char i2c_flag;
EXT volatile unsigned char I2C_State;
EXT volatile unsigned char Bytecount;
EXT volatile unsigned char transmit;
EXT volatile unsigned char reg_files[MAX_RCV_NUM];
EXT volatile unsigned reg_ptr;                        //Control Registers and access pointer
EXT volatile unsigned char ch1_light;
EXT volatile unsigned char ch2_light;
EXT volatile unsigned char ch3_light;
EXT volatile unsigned char ch4_light;      //channel 1-4 lighting control register range 0-100
EXT volatile unsigned char detect_zero;
EXT volatile unsigned char Group_Mask;
EXT volatile unsigned char pw_idx;
EXT volatile unsigned char pwoff_cnt;
EXT volatile unsigned int zero_thd;

