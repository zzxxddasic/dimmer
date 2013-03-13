/** @file
 * @brief I2C ISR
 * @author Zeng Xuedong
 * @date 2012-10-20
 */
#include <msp430.h>
#include "dimmer.h"
#include "globals.h"

/**
 * @ingroup ISR
 * @brief USI interrupt service routine
 *
 * Rx bytes from master: State 2->4->6->8<br>
 * Tx bytes to Master: State 2->4->10->12->14<br>
 */
#pragma vector = USI_VECTOR
__interrupt void USI_TXRX (void) {
    __bic_SR_register_on_exit(CPUOFF);        // Clear CPUOFF bit from 0(SR)
    if (USICTL1 & USISTTIFG) {                  // Start entry?
	    USICTL1 &= ~USISTTIFG;                  // Clear start flag
	    I2C_State = 2;                          // Enter 1st state on start
    }
    USICTL1 &= ~USIIFG;                         // Clear pending flags
    switch(I2C_State) {
	    case 0:                               // Idle, should not get here
	        break;

        case 2: // RX Address
            USICNT = (USICNT & 0xE0) + 0x08; // Bit counter = 8, RX address
            I2C_State = 4;                   // Go to next state: check address
            break;

        case 4: // Process Address and send (N)Ack
            if (USISRL & 0x01) {            // If master read...
		        SLV_Addr = 0x91;            // Save R/W bit
                transmit = 1;
            }
		    else {
		        transmit = 0;
		        SLV_Addr = 0x90;
		    }
            USICTL0 |= USIOE;               // SDA = output
            if (USISRL == SLV_Addr) {       // Address match?
                USISRL = 0x00;              // Send Ack
		        if (transmit == 0) {
			        Bytecount = 0xff;
                    I2C_State = 6;
		        }           // Go to next state: RX data
                if (transmit == 1) {
			        I2C_State = 10;
		        }          // Else go to next state: TX data
            }
		    else {
		        USISRL = 0xFF;              // Send NAck
                USICTL0 &= ~USIOE;          //SDA = input
		        I2C_State = 0;              // next state: prep for next Start
		    }
            USICNT |= 0x01;                 // Bit counter = 1, send (N)Ack bit
            break;

	    case 6: // Receive data byte
	        //Data_RX();
            USICTL0 &= ~USIOE;            // SDA = input
            USICNT |=  0x08;              // Bit counter = 8, RX data
            I2C_State = 8;                // next state: Test data and (N)Ack
            break;

	    case 8:// Check Data & TX (N)Ack
	        USICTL0 |= USIOE;               // SDA = output
	        if (Bytecount == 0xff) {
		        reg_ptr = USISRL;
	        }
	        else if (reg_ptr < MAX_RCV_NUM) {        // If not last byte
		        reg_files[reg_ptr] = USISRL;
		        reg_ptr++;
                i2c_flag = reg_ptr;
	        }
            else {
                i2c_flag = 0;
            }
	        USISRL = 0x00;              // Send Ack
	        I2C_State = 6;              // Rcv another byte
	        Bytecount++;
	        USICNT |= 0x01;
	        break;

	    case 10: // Send Data byte
            //TX_Data();
            USICTL0 |= USIOE;             // SDA = output
            USISRL = reg_files[reg_ptr];
            USICNT |=  0x08;              // Bit counter = 8, TX data
            I2C_State = 12;               // Go to next state: receive (N)Ack
            break;

        case 12:// Receive Data (N)Ack
            USICTL0 &= ~USIOE;            // SDA = input
            USICNT |= 0x01;               // Bit counter = 1, receive (N)Ack
            I2C_State = 14;               // Go to next state: check (N)Ack
            break;

        case 14:// Process Data Ack/NAck
            if (USISRL & 0x01) {              // If Nack received...
		        USICTL0 &= ~USIOE;            // SDA = input
                SLV_Addr = 0x90;              // Reset slave address
                I2C_State = 0;                // Reset state machine
                Bytecount = 0xff;
                // LPM0_EXIT;                 // Exit active for next transfer
            }
            else {                            // Ack received
		        reg_ptr++;
                //TX_Data();                    // TX next byte
                USICTL0 |= USIOE;             // SDA = output
                USISRL = reg_files[reg_ptr];
                USICNT |=  0x08;              // Bit counter = 8, TX data
                I2C_State = 12;               // Go to next state: receive (N)Ack
                USICTL0 |= USIOE;             // SDA = output
                USISRL = reg_files[reg_ptr];
                USICNT |=  0x08;              // Bit counter = 8, TX data
                I2C_State = 12;               // Go to next state: receive (N)Ack
            }
	        break;

    }
}

void Setup_USI_Slave(void) {
    P1OUT = 0xC0;                             // P1.6 & P1.7 Pullups
    P1REN |= 0xC0;                            // P1.6 & P1.7 Pullups
    P1DIR = 0xFF;                             // Unused pins as outputs
//    P2SEL = 0;
//    P2SEL2 = 0;
//    P2DIR = 0xFF;

    USICTL0 = USIPE6+USIPE7+USISWRST;         // Port & USI mode setup
    USICTL1 = USII2C+USIIE+USISTTIE;          // Enable I2C mode & USI interrupts
    USICKCTL = USISSEL_4 + USICKPL;           // Setup clock polarity
    USICNT |= USIIFGCC;                       // Disable automatic clear control
    USICTL0 &= ~USISWRST;                     // Enable USI
    USICTL1 &= ~USIIFG;                       // Clear pending flag

    transmit = 0;
    Bytecount = 0xff;
    _EINT();

}
