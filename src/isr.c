/** @file
 * @brief watchdog,ADC and Timer_A0 ISR
 * @author Zeng Xuedong
 * @date 2012-10-20
 */
#include <msp430.h>
#include "dimmer.h"
#include "globals.h"
/**
 * @defgroup ISR
 */

/**
 * @ingroup ISR
 * @brief Watchdog Timer interrupt service routine
 *
 */
#pragma vector=WDT_VECTOR
__interrupt void watchdog_timer(void) {
	__bis_SR_register(GIE);                   //Enable nesting INT
    __bic_SR_register_on_exit(CPUOFF);        // Clear CPUOFF bit from 0(SR)

}

/**
 * @ingroup ISR
 * @brief ADC10 interrupt service routine
 *
 * First enable GIE to enable nesting interrupt.<br>
 * Make average value from raw result, and store it as previous value.<br>
 * then stop ADC and bring CPU to normal mode.<br>
 */
#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR(void) {
	__bis_SR_register(GIE);                   // Enable nesting INT
	ADC10CTL0 &= ~ENC;
    res_avg = (res[0] + res[1]) / 2 ;
    res_pre1 = res_pre;
    res_pre = res_avg;
    __bic_SR_register_on_exit(CPUOFF);        // Clear CPUOFF bit from 0(SR)
}

/**
 * @ingroup ISR
 * @brief Timer A interrupt service routine
 * @details Timer_A using CCR0 interrupt
 */
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A (void) {
	__bis_SR_register(GIE);                   //Enable nesting INT
    __bic_SR_register_on_exit(CPUOFF);        // Clear CPUOFF bit from 0(SR)
    if (ch1_light==0) {
        P2OUT &= ~0x01;                            // set P2.0 ,switch off channel 1 MOS
    }
    else {
        ch1_light--;
    }
    if (ch2_light==0) {
        P2OUT &= ~0x02;                            // set P2.1 ,switch off channel 2 MOS
    }
    else {
        ch2_light--;
    }
    if (ch3_light==0) {
        P2OUT &= ~0x04;                            // set P2.2 ,switch off channel 3 MOS
    }
    else {
        ch3_light--;
    }
    if (ch4_light==0) {
        P2OUT &= ~0x08;                            // set P2.3 ,switch off channel 4 MOS
    }
    else {
        ch4_light--;
    }
    if ((P2OUT & 0x0f) == 0x00) {
        detect_zero = 0;
        TACTL = TASSEL_2 + MC_0;
        ch1_light = reg_files[ABS_G0];
        ch2_light = reg_files[ABS_G1];
        ch3_light = reg_files[ABS_G2];
        ch4_light = reg_files[ABS_G3];
        res_avg = 0;
    }
    else {
    	ADC10CTL0 |= ENC + ADC10SC;
    }

}

