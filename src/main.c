//******************************************************************************
//  MSP430G2452 DIMMER
//
//  Description: Detect Cross point of AC power from P1.0
//  Control 4 channel MOS swith,each channel control by setting ch1_light-ch4_light Register separately .
//  Interface: I2C.
//
//  Zeng Xuedong
//  SunSunlighting
//  Noveber 2012
//
//******************************************************************************
/** @file
 * @author Zeng Xuedong
 * @date 2012-10-20
 *
 * @detail
 * @brief MSP430G2452 DIMMER main program</br>
 *
 * Detect Cross point of AC power from P1.0</br>
 * Control 4 channel MOS swith,each channel control by setting ch1_light-ch4_light Register separately .</br>
 * Interface: I2C.
 */
#include <msp430.h> 
#include "dimmer.h"
#define DEF_GLOBALS
#include "globals.h"

/*
 * main.c
 */


void main(void) {
    char tmp;
    char inc_all;
    unsigned char abs_all;
    unsigned int timer_cnt;
    WDTCTL = WDTPW + WDTHOLD;	/**Set Watch time to timer mode */

    detect_zero=0;				/**initial global var */
    SLV_Addr = 0x90;
    i2c_flag=0;

    P2SEL = 0;					/**Set P2 GPIO mode, and set to 0, turn off MOS*/
    P2SEL2 = 0;
    P2OUT = 0xD0;
    P2DIR = 0xFF;

    CCR0 = 0xffff;				/**delay about 100mS for cross zero detect circuit become stable*/
    for(tmp=0;tmp<2;tmp++) {
    	TACTL = TASSEL_2 + ID_3 + MC_1;                 //SMCLK/8,up mode
    	while(TAR < 32768);								//delay 32768*8uS(1us*8)
    	TAR = 0;
    	TACTL = TASSEL_2 + ID_3 + MC_0;					//Stop timer
    }


    BCSCTL1 = CALBC1_16MHZ;		/**Set MCLK to 16MHz*/
    DCOCTL = CALDCO_16MHZ;

    FCTL2 = FWKEY + FSSEL_1 + 53;			/** MCLK/54 for Flash Timing Generator*/

    BCSCTL2 +=DIVS_0;						/**smclk = mclk / 1*/
    CCTL0 = CCIE;
    CCR0 = 0xffff;

    /**TASSEL TimerA clock source select 0=TACLK 1=ACLK 2=SMCLK 3=INCLK<br>
     * ID     TimerA input divider 0=/1 1=/2 2=/4 3=/8<br>
     * MC	  TimerA mode control  0=stop 1=up 2=continuous 3=up/down
     *
     */
    TACTL = TASSEL_2 + ID_3 + MC_0;			/**SMCLK,continuous mode*/
    init_flash();							/**initial flash*/

    /** Set ADC Repeat single channel on A1*/
    ADC10CTL1 = CONSEQ_2 + INCH_0 + ADC10DIV_1;
    ADC10CTL0 = REFON + ADC10SHT_0 + SREF_2 + MSC + ADC10ON + ADC10IE; /** ADC10ON, interrupt enable*/
    ADC10DTC0 = ADC10CT;                    /**Continuous Transfer*/
    ADC10DTC1 = 2;                         	/** conversions 2 times every convert period*/

    ADC10SA = (unsigned int)res;			/**Set ADC10SA to address of res buffer*/
    ADC10AE0 |= 0x01;                       /** P1.0 is ADC input pin*/


    Setup_USI_Slave();
    //-----------------------------------
    //          detect zero point threshold
    //-----------------------------------
    timer_cnt = 0;
    ch2_light = 0;
    while (1) {
        //ADC10CTL0 &= ~ENC;
        while (ADC10CTL1 & BUSY);
        if (res_avg >= res_pre && res_avg > 600) {
        	if (res_avg > timer_cnt) {
        		//if (res_avg < 1020) {
        			timer_cnt = res_avg;
        			//ch2_light = 0;
        		//}
        	}
        	ch2_light++;
        	if (ch2_light == 16) {
        		break;
        	}
        }
        ADC10CTL0 |= ENC + ADC10SC;             // Sampling and conversion start
        __bis_SR_register(CPUOFF);        // LPM0, ISR will force exit
        _NOP();
    }
    zero_thd = timer_cnt - 20;

    //P2OUT &= 0xDF;
    WDTCTL = WDT_MDLY_32;                     /** Set Watchdog Timer interval to ~30ms*/
    IE1 |= WDTIE;                             /** Enable WDT interrupt*/
    //-----------------------------------
    //          detect Power freq.
    //-----------------------------------
    ch2_light = 2;
    TAR = 0;
    timer_cnt = 0;
    while (timer_cnt < 15000 || timer_cnt > 20000) {
        //ADC10CTL0 &= ~ENC;
        while (ADC10CTL1 & BUSY);
        if (res_avg > zero_thd) {
            TACTL = TASSEL_2 + ID_3 + MC_0; //input clk / 8
            if (TAR > 1024 ) {
                timer_cnt = TAR;
                ch2_light--;                  //look up cross zero point 10 times
                //if (ch2_light == 0) break;
            }

            TAR = 0;
            TACTL = TASSEL_2 + ID_3 + MC_1;
        }
        ADC10CTL0 |= ENC + ADC10SC;             // Sampling and conversion start
        __bis_SR_register(CPUOFF);        // LPM0, ISR will force exit
        _NOP();
    }

    TACTL = TASSEL_2 + MC_0;                 //SMCLK,countinuous mode
    CCTL0 = CCIE;

    /**timer A input clock frequency is 16MHz/8, when input power frequency is 50Hz,
     * the timer counter 20000 in one period, the counter is 16666 for 60Hz alternate power.
     */

    if (timer_cnt > 18000) {
        CCR0 = PW_50HZ;
        P2OUT |= 0x20;
    }
    else {
        CCR0 = PW_60HZ;
        P2OUT &= 0xDF;
    }

    res_pre = 0;
    pwoff_cnt = 0;
    reg_files[GROUP_SEL] = 0;
    while(1) {
/**When the cross zero detector consecutive 8 times detect valid cross zero point,
 * it indicate the power is loss,then save current state to eeprom.<br>
 * And toggle indicate LED lighting state to show that situation occurred.
 */
    	if (res_pre1 == res_pre && res_pre > zero_thd) {
    		pwoff_cnt++;
    		if (pwoff_cnt > 8) {
    			P2OUT &= 0xf0;
    			save_sta();
    			if (CCR0 == PW_50HZ) {
    				P2OUT &= 0xD0;
    			}
    			else {
    				P2OUT |= 0x20;   /*delighting LED when power frequency is 60Hz*/
    			}
    			while(1);
    		}
    	}
    	else {
    		pwoff_cnt = 0;//detect_zero ? 0 : 6;
    	}

        if (detect_zero == 0) {
            if (res_avg > zero_thd) {
	            //ADC10CTL0 &= ~ENC;
	            while (ADC10CTL1 & BUSY);               // Wait if ADC10 core is active
	            P2OUT |=(reg_files[GROUP_SEL] >> 4) | 0xf0;  // switch on channel 1-4 MOS
            	//P2OUT &=~0x0f;
                detect_zero = 1;
                TAR = 0;
                TACTL = TASSEL_2 + MC_1;
            }
            else {
            	while (ADC10CTL1 & BUSY);
                ADC10CTL0 |= ENC + ADC10SC;             // Sampling and conversion start
            }
        }

        if (i2c_flag != 0) {
            char inc_tmp;
            Group_Mask = reg_files[GROUP_SEL];
            inc_all = (char)reg_files[INC_ALL];
            abs_all = reg_files[ABS_ALL] & 0x7f;
            reg_files[ABS_G0] &= 0x7f;
            reg_files[ABS_G1] &= 0x7f;
            reg_files[ABS_G2] &= 0x7f;
            reg_files[ABS_G3] &= 0x7f;
            switch(i2c_flag) {
            	case ABSALL:
                    if (Group_Mask & 0x01) {
                        reg_files[ABS_G0] = abs_all;
                    }
                    if (Group_Mask & 0x02) {
                        reg_files[ABS_G1] = abs_all;
                    }
                    if (Group_Mask & 0x04) {
                        reg_files[ABS_G2] = abs_all;
                    }
                    if (Group_Mask & 0x08) {
                        reg_files[ABS_G3] = abs_all;
                    }

                    break;

            	case INCALL:
            		tmp = (char)reg_files[ABS_G0];
            		inc_tmp = (Group_Mask & 0x01) ? inc_all : 0;
            		tmp += inc_tmp;
            		inc_tmp = (inc_tmp & 0x80) ? 0 : 0x7f;
            		reg_files[ABS_G0]= (0x80 & tmp) ? inc_tmp : tmp;

            		tmp = (char)reg_files[ABS_G1];
            		inc_tmp = (Group_Mask & 0x02) ? inc_all : 0;
            		tmp += inc_tmp;
            		inc_tmp = (inc_tmp & 0x80) ? 0 : 0x7f;
            		reg_files[ABS_G1]= (0x80 & tmp) ? inc_tmp : tmp;

            		tmp = (char)reg_files[ABS_G2];
            		inc_tmp = (Group_Mask & 0x04) ? inc_all : 0;
            		tmp += inc_tmp;
            		inc_tmp = (inc_tmp & 0x80) ? 0 : 0x7f;
            		reg_files[ABS_G2]= (0x80 & tmp) ? inc_tmp : tmp;

            		tmp = (char)reg_files[ABS_G3];
            		inc_tmp = (Group_Mask & 0x08) ? inc_all : 0;
            		tmp += inc_tmp;
            		inc_tmp = (inc_tmp & 0x80) ? 0 : 0x7f;
            		reg_files[ABS_G3]= (0x80 & tmp) ? inc_tmp : tmp;
            		break;
            	case INCG0:
            		tmp = (char)reg_files[ABS_G0];
            		inc_tmp = (char)reg_files[INC_G0];
            		tmp += inc_tmp;
            		inc_tmp = (inc_tmp & 0x80) ? 0 : 0x7f;
            		reg_files[ABS_G0]= (0x80 & tmp) ? inc_tmp : tmp;
            		break;
            	case INCG1:
            		tmp = (char)reg_files[ABS_G1];
            		inc_tmp = (char)reg_files[INC_G1];
            		tmp += inc_tmp;
            		inc_tmp = (inc_tmp & 0x80) ? 0 : 0x7f;
            		reg_files[ABS_G1]= (0x80 & tmp) ? inc_tmp : tmp;
            		break;
            	case INCG2:
            		tmp = (char)reg_files[ABS_G2];
            		inc_tmp = (char)reg_files[INC_G2];
            		tmp += inc_tmp;
            		inc_tmp = (inc_tmp & 0x80) ? 0 : 0x7f;
            		reg_files[ABS_G2]= (0x80 & tmp) ? inc_tmp : tmp;
            		break;
            	case INCG3:
            		tmp = (char)reg_files[ABS_G3];
            		inc_tmp = (char)reg_files[INC_G3];
            		tmp += inc_tmp;
            		inc_tmp = (inc_tmp & 0x80) ? 0 : 0x7f;
            		reg_files[ABS_G3]= (0x80 & tmp) ? inc_tmp : tmp;
            		break;
            }

            i2c_flag = 0;

        }
        __bis_SR_register(CPUOFF);        // LPM0, ISR will force exit
        _NOP();
    }
}


void init_flash() {
    unsigned char * flash_ptr;
    unsigned int i;
    flash_ptr = (unsigned char *)0x1000;
    pw_idx = 0xff;
    for (i=0;i<16;i++) {
        if (*flash_ptr != 0xff) {
            reg_files[ABS_G0] = *flash_ptr & 0x7f;
            reg_files[ABS_G1] = *(flash_ptr+1) & 0x7f;
            reg_files[ABS_G2] = *(flash_ptr+2) & 0x7f;
            reg_files[ABS_G3] = *(flash_ptr+3) & 0x7f;
            pw_idx = (i+1) << 2;
        }
        flash_ptr+=4;
    }
    if (pw_idx == 0xff) {  //erase flash seg.D when no space.
    	reg_files[ABS_G0] = 0;
        reg_files[ABS_G1] = 0;
        reg_files[ABS_G2] = 0;
        reg_files[ABS_G3] = 0;
        pw_idx = 0;
    }
    if (pw_idx == 64) {
        erase_flash();
        pw_idx = 0;
    }

}
void save_sta() {
    unsigned char * flash_ptr;
    flash_ptr = (unsigned char *)0x1000;
    FCTL3 = FWKEY;                            // Clear Lock bit
    FCTL1 = FWKEY + WRT;                      // Set WRT bit for write operation

    flash_ptr += pw_idx;
    *flash_ptr = reg_files[ABS_G0];
    flash_ptr++;
    *flash_ptr = reg_files[ABS_G1];
    flash_ptr++;
    *flash_ptr = reg_files[ABS_G2];
    flash_ptr++;
    *flash_ptr = reg_files[ABS_G3];
    flash_ptr++;

    FCTL1 = FWKEY;                            // Clear WRT bit
    FCTL3 = FWKEY + LOCK;                     // Set LOCK bit


}
void erase_flash() {
    unsigned char * flash_ptr;
    flash_ptr = (unsigned char *)0x1000;
    FCTL3 = FWKEY;                            // Clear Lock bit
    FCTL1 = FWKEY + ERASE;                    // Set Erase bit
    *flash_ptr = 0;                           // Dummy write to erase Flash segment
    FCTL1 = FWKEY;                            // Clear ERASE bit
    FCTL3 = FWKEY + LOCK;                     // Clear Lock bit

}
