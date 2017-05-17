/*
 * Copyright (c) 2010, Swedish Institute of Computer Science.
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

/**
 * \file
 *         A brief description of what this file is
 * \author
 *         Joakim Eriksson <joakime@sics.se>
 */

#ifndef PLATFORM_CONF_H_
#define PLATFORM_CONF_H_

#define MOTEIST5 1

#define PLATFORM_HAS_LEDS    1
#define PLATFORM_HAS_BUTTON  0
#define PLATFORM_HAS_LIGHT   0
#define PLATFORM_HAS_BATTERY 0
#define PLATFORM_HAS_SHT11   0
#define PLATFORM_HAS_RADIO   1

/*
 * Definitions below are dictated by the hardware and not really
 * changeable
 */

#ifndef WITH_LCD
#define WITH_LCD 0
#endif /* 0 */

/* CPU target speed in Hz */
#define F_CPU 16000000uL // 8MHz by default

/* Our clock resolution, this is the same as Unix HZ. */
#define CLOCK_CONF_SECOND 128UL

//#define BAUD2UBR(baud) (baud)
#define BAUD2UBR(baud) ( baud)

#define CCIF
#define CLIF

#define HAVE_STDINT_H
#define MSP430_MEMCPY_WORKAROUND 1
#include "msp430def.h"

/* XXX Temporary place for defines that are lacking in mspgcc4's gpio.h */
#ifdef __IAR_SYSTEMS_ICC__
#ifndef P1SEL2_
#define P1SEL2_              (0x0041u)  /* Port 1 Selection 2*/
DEFC(P1SEL2, P1SEL2_)
#endif
#ifndef P5SEL2_
#define P5SEL2_              (0x0045u)  /* Port 5 Selection 2*/
DEFC(P5SEL2, P5SEL2_)
#endif
#ifndef P9SEL2_
#define P9SEL2_              (0x0049u)  /* Port 9 Selection 2*/
DEFC(P9SEL2, P9SEL2_)
#endif
#else /* __IAR_SYSTEMS_ICC__ */
#ifdef __GNUC__
#ifndef P1SEL2_
#define P1SEL2_             0x0041    /* Port 1 Selection 2*/
sfrb(P1SEL2, P1SEL2_);
#endif
#ifndef P5SEL2_
#define P5SEL2_             0x0045    /* Port 5 Selection 2*/
sfrb(P5SEL2, P5SEL2_);
#endif
#ifndef P9SEL2_
#define P9SEL2_             0x0049    /* Port 9 Selection 2*/
sfrb(P9SEL2, P9SEL2_);
#endif
#endif /* __GNUC__ */
#endif /* __IAR_SYSTEMS_ICC__ */


/* Types for clocks and uip_stats */
typedef unsigned short uip_stats_t;
typedef unsigned long clock_time_t;
typedef unsigned long off_t;

/* the low-level radio driver */
#define NETSTACK_CONF_RADIO   cc2420_driver

/*
 * Definitions below are dictated by the hardware and not really
 * changeable!
 */


/* DCO speed resynchronization for more robust UART, etc. */
/* Not needed from MSP430x5xx since it make use of the FLL */
#define DCOSYNCH_CONF_ENABLED 0
#define DCOSYNCH_CONF_PERIOD 30

#define ROM_ERASE_UNIT_SIZE  512
#define XMEM_ERASE_UNIT_SIZE (64*1024L)

#define CFS_CONF_OFFSET_TYPE    long

/* Use the first 64k of external flash for node configuration */
#define NODE_ID_XMEM_OFFSET     (0 * XMEM_ERASE_UNIT_SIZE)

/* Use the second 64k of external flash for codeprop. */
#define EEPROMFS_ADDR_CODEPROP  (1 * XMEM_ERASE_UNIT_SIZE)

#define CFS_XMEM_CONF_OFFSET    (2 * XMEM_ERASE_UNIT_SIZE)
#define CFS_XMEM_CONF_SIZE      (1 * XMEM_ERASE_UNIT_SIZE)

#define CFS_RAM_CONF_SIZE 4096

/*
 * SPI bus configuration for the TMote Sky.
 */

/* SPI input/output registers. */
#define SPI_TXBUF UCB3TXBUF
#define SPI_RXBUF UCB3RXBUF

                                /* USART0 Tx ready? */
#define SPI_WAITFOREOTx() while ((UCB3STAT & UCBUSY) != 0)
                                /* USART0 Rx ready? */
#define SPI_WAITFOREORx() while ((UCB3IFG & UCRXIFG) == 0)
                                /* USART0 Tx buffer ready? */
#define SPI_WAITFORTxREADY() while ((UCB3IFG & UCTXIFG) == 0)

#define MOSI           1  /* P3.1 - Output: SPI Master out - slave in (MOSI) */
#define MISO           2  /* P3.2 - Input:  SPI Master in - slave out (MISO) */
#define SCK            3  /* P3.3 - Output: SPI Serial Clock (SCLK) */

/*
 * SPI bus - M25P80 external flash configuration.
 */
//#define FLASH_PWR     3       /* P4.3 Output */ ALWAYS POWERED ON Z1
#define FLASH_CS        4       /* P4.4 Output */
#define FLASH_HOLD      7       /* P5.7 Output */

/* Enable/disable flash access to the SPI bus (active low). */

#define SPI_FLASH_ENABLE()  ( P4OUT &= ~BV(FLASH_CS) )
#define SPI_FLASH_DISABLE() ( P4OUT |=  BV(FLASH_CS) )

#define SPI_FLASH_HOLD()                ( P5OUT &= ~BV(FLASH_HOLD) )
#define SPI_FLASH_UNHOLD()              ( P5OUT |=  BV(FLASH_HOLD) )


/*
 * SPI bus - CC2420 pin configuration.
 */

#define CC2420_CONF_SYMBOL_LOOP_COUNT 1302      /* 326us msp430X @ 8MHz */

/* P1.6 - Input: FIFOP from CC2420 */
#define CC2420_FIFOP_PORT(type)    P1##type
#define CC2420_FIFOP_PIN           6
/* P1.2 - Input: FIFO from CC2420 */
#define CC2420_FIFO_PORT(type)     P1##type
#define CC2420_FIFO_PIN            2
/* P1.7 - Input: CCA from CC2420 */
#define CC2420_CCA_PORT(type)      P1##type
#define CC2420_CCA_PIN             7
/* P1.1 - Input:  SFD from CC2420 */
#define CC2420_SFD_PORT(type)      P1##type
#define CC2420_SFD_PIN             1
 /* P10.0 - Output: SPI Chip Select (CS_N) */
#define CC2420_CSN_PORT(type)      P10##type
#define CC2420_CSN_PIN             0
/* P9.7 - Output: VREG_EN to CC2420 */
#define CC2420_VREG_PORT(type)     P9##type
#define CC2420_VREG_PIN            7
/* P9.6 - Output: RESET_N to CC2420 */
#define CC2420_RESET_PORT(type)    P9##type
#define CC2420_RESET_PIN           6


#define CC2420_IRQ_VECTOR PORT1_VECTOR

/* Pin status. */
#define CC2420_FIFOP_IS_1 (!!(CC2420_FIFOP_PORT(IN) & BV(CC2420_FIFOP_PIN)))
#define CC2420_FIFO_IS_1  (!!(CC2420_FIFO_PORT(IN) & BV(CC2420_FIFO_PIN)))
#define CC2420_CCA_IS_1   (!!(CC2420_CCA_PORT(IN) & BV(CC2420_CCA_PIN)))
#define CC2420_SFD_IS_1   (!!(CC2420_SFD_PORT(IN) & BV(CC2420_SFD_PIN)))

/* The CC2420 reset pin. */
#define SET_RESET_INACTIVE()   (CC2420_RESET_PORT(OUT) |=  BV(CC2420_RESET_PIN))
#define SET_RESET_ACTIVE()     (CC2420_RESET_PORT(OUT) &= ~BV(CC2420_RESET_PIN))

/* CC2420 voltage regulator enable pin. */
#define SET_VREG_ACTIVE()       (CC2420_VREG_PORT(OUT) |=  BV(CC2420_VREG_PIN))
#define SET_VREG_INACTIVE()     (CC2420_VREG_PORT(OUT) &= ~BV(CC2420_VREG_PIN))

/* CC2420 rising edge trigger for external interrupt (FIFOP). */
#define CC2420_FIFOP_INT_INIT() do {                  \
    CC2420_FIFOP_PORT(IES) &= ~BV(CC2420_FIFOP_PIN);  \
    CC2420_CLEAR_FIFOP_INT();                         \
  } while(0)

/* FIFOP on external interrupt 0. */
#define CC2420_ENABLE_FIFOP_INT()  do {CC2420_FIFOP_PORT(IE) |= BV(CC2420_FIFOP_PIN);} while(0)
#define CC2420_DISABLE_FIFOP_INT() do {CC2420_FIFOP_PORT(IE) &= ~BV(CC2420_FIFOP_PIN);} while(0)
#define CC2420_CLEAR_FIFOP_INT()   do {CC2420_FIFOP_PORT(IFG) &= ~BV(CC2420_FIFOP_PIN);} while(0)


/*
 * Enables/disables CC2420 access to the SPI bus (not the bus).
 * (Chip Select)
 */

 /* ENABLE CSn (active low) */
#define CC2420_SPI_ENABLE()     (CC2420_CSN_PORT(OUT) &= ~BV(CC2420_CSN_PIN))
 /* DISABLE CSn (active low) */
#define CC2420_SPI_DISABLE()    (CC2420_CSN_PORT(OUT) |=  BV(CC2420_CSN_PIN))
#define CC2420_SPI_IS_ENABLED() ((CC2420_CSN_PORT(OUT) & BV(CC2420_CSN_PIN)) != BV(CC2420_CSN_PIN))

#endif /* PLATFORM_CONF_H_ */