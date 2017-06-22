/*
 * Copyright (c) 2007, Swedish Institute of Computer Science.
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
 * This file is part of the Contiki operating system.
 *
 */

/**
 * \file
 *         Best-effort single-hop unicast example
 * \author
 *         Adam Dunkels <adam@sics.se>
 */

#include "contiki.h"
#include "net/rime/rime.h"

#include "dev/button-sensor.h"

#include "dev/leds.h"

#include <stdio.h>
#include "sys/etimer.h"

#include "robotPigeon-shell.h"
#include "robotPigeon-radio.h"
//#include "vari.h"

/*---------------------------------------------------------------------------*/

PROCESS(radio_unicast_process, "Radio unicast");

/*---------------------------------------------------------------------------*/
#define WAIT_forACK	0x00
#define SEND_ACK	0x01
#define SEND_DATA	0x02


static unsigned char status = WAIT_forACK;
static char msgSink[2] = " ";

static void recv_uc(struct unicast_conn *c, const linkaddr_t *from);


static const struct unicast_callbacks unicast_callbacks = {recv_uc};
static struct unicast_conn uc;
static linkaddr_t addr;


static void
recv_uc(struct unicast_conn *c, const linkaddr_t *from)
{   
 	//printf("unicast message received from %d.%d: '%s'\n", from->u8[0], from->u8[1], (char *)packetbuf_dataptr());
	
	if( *(char *)packetbuf_dataptr()=='#' && *( (char *)packetbuf_dataptr()+1 )==0x00){
		process_post(&radio_unicast_process, PROCESS_EVENT_CONTINUE, NULL);
	}
}





/*---------------------------------------------------------------------------*/

PROCESS_THREAD(radio_unicast_process, ev, data)
{
  //PROCESS_EXITHANDLER(unicast_close(&uc);)
  static struct etimer et;   
  PROCESS_BEGIN();

  unicast_open(&uc, 0x1A, &unicast_callbacks);
 
  etimer_set(&et, CLOCK_SECOND*5);
  
  while(1) {
	if(status==WAIT_forACK){
		PROCESS_WAIT_EVENT(); //Esperar evento
		if(ev==PROCESS_EVENT_CONTINUE){	//Evento recebido
			status = SEND_ACK;
		}			
	}
	else
	if(status==SEND_ACK){
		//printf(" ----- SEND ACK ------\r\n");
		packetbuf_copyfrom("#", 2); //ACK
		addr.u8[0] = 2;//Endereco do destino
    		addr.u8[1] = 0;
		unicast_send(&uc, &addr);
		setLink(); //robot isready
		status = SEND_DATA;
	}
	else
	if(status==SEND_DATA){
		//printf(" ----- SEND DATA ------\r\n");
		PROCESS_WAIT_EVENT(); //Espera de dados
		if(ev==PROCESS_EVENT_MSG){
			//printf(" ----- DATA ------ %02x \r\n", data);
			msgSink[0] = data;
			//printf(" ----- DATA ------ %c \r\n", m);
			packetbuf_copyfrom(msgSink, 1); //ACK
			addr.u8[0] = 2;//Endereco do destino
    			addr.u8[1] = 0;
			unicast_send(&uc, &addr);
			status = WAIT_forACK;
		}
	}
  }//end_while

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
void robotPigeon_radio_init(){
	process_start(&radio_unicast_process, NULL);
}
