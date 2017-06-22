/**
 * \file
 *	Robot Pigeon         
 *	Single-hop unicast RIME
 * \author
 *         
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
