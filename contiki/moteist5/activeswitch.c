/*
 * Copyright (c) 2006, Swedish Institute of Computer Science
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 */

#include "contiki-conf.h"

/*
 * This is initialization code for the MOTEIST5
 *
 */
/*---------------------------------------------------------------------------*/
/*
 * Initialize activeswitch of MOTEIST5
 */
void
activeswitch_init(void)
{
  	P3SEL |= 0x80;                            // P3.7 - SDA - Assign I2C pins
	P5SEL |= 0x10;							  // P5.4 - SCL

	UCB1CTL1 |= UCSWRST;                      // Enable SW reset

	UCB1CTL0 = UCMST + UCMODE_3 + UCSYNC;     // I2C Master, synchronous mode
	UCB1CTL1 = UCSSEL_2 + UCSWRST;            // Use SMCLK
	UCB1BR0 = 144;                            // fSCL = SMCLK/12 = ~100kHz
  	UCB1BR1 = 0;
  	UCB1I2CSA = 0x48;                         // Slave Address is 090h 1001 0000 -> address (7bit, A0=A1=0) + R/!W

  	UCB1CTL1 &= ~UCSWRST;                     // Clear SW reset*/

	while (UCB1CTL1 & UCTXSTP);               // Ensure stop condition got sent
    	UCB1CTL1 |= UCTR + UCTXSTT;               // I2C start condition
  	UCB1TXBUF = 0xFF;  	                      // Send init data
  	while(UCB1CTL1 & UCTXSTT);                // Start condition sent?
  	UCB1CTL1 |= UCTXSTP;				      // I2C1 Stop Condition*/
}
/*---------------------------------------------------------------------------*/
