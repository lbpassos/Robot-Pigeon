#include "contiki.h"
#include "net/rime/rime.h"

#include "dev/button-sensor.h"

#include "dev/leds.h"

#include <stdio.h>
#include "sys/etimer.h"

#include "robotPigeon-radio.h"


/*---------------------------------------------------------------------------*/

PROCESS(radio_unicast_process, "Radio unicast");

/*---------------------------------------------------------------------------*/
#define WAIT_forACK	0x00
#define SEND_ACK	0x01
#define SEND_DATA	0x02
#define RECEIVE_DATA	0x03

static unsigned char status = WAIT_forACK;
static unsigned char status_recv = 0;

static char msgSink;

static void recv_uc(struct unicast_conn *c, const linkaddr_t *from);


static const struct unicast_callbacks unicast_callbacks = {recv_uc};
static struct unicast_conn uc;
static linkaddr_t addr;


static void
recv_uc(struct unicast_conn *c, const linkaddr_t *from)
{   
 	//printf("unicast message received from %d.%d: '%s'\n", from->u8[0], from->u8[1], (char *)packetbuf_dataptr());
	
	if(status_recv==0){	
		if( *(char *)packetbuf_dataptr()=='#' && *( (char *)packetbuf_dataptr() + 1) =='*' ){
			process_post(&radio_unicast_process, PROCESS_EVENT_CONTINUE, NULL);
		}
	}
	else
	if(status_recv==1){
		if( *(char *)packetbuf_dataptr()!='#' && *( (char *)packetbuf_dataptr() + 1) !='*' ){			
			//printf(" ----- DATA ------ %02x \r\n", (char *)packetbuf_dataptr());
			msgSink = *(char *)packetbuf_dataptr();
			process_post(&radio_unicast_process, PROCESS_EVENT_MSG, msgSink);
		}
	}
}





/*---------------------------------------------------------------------------*/

PROCESS_THREAD(radio_unicast_process, ev, data)
{
  //PROCESS_EXITHANDLER(unicast_close(&uc);)
  static struct etimer et;   
  PROCESS_BEGIN();

  unicast_open(&uc, 0x1A, &unicast_callbacks);
 
  //etimer_set(&et, CLOCK_SECOND*5);

  //leds_on(LEDS_GREEN);
  //leds_on(LEDS_RED);
  //leds_on(LEDS_YELLOW);
  
  //while(1);
  while(1) {
	if(status==WAIT_forACK){
		//printf(" ----- ESPERA ------\r\n");
		PROCESS_WAIT_EVENT(); //Esperar evento
		if(ev==PROCESS_EVENT_CONTINUE){	//Evento recebido
			status = SEND_ACK;
		}			
	}
	else
	if(status==SEND_ACK){
		//printf(" ----- SEND ACK ------\r\n");
		packetbuf_copyfrom("#*", 3); //ACK_2
		addr.u8[0] = 1;//Endereco do destino
    		addr.u8[1] = 0;
		unicast_send(&uc, &addr);
		//setLink(); //robot isready
		status_recv = 1;
		status = RECEIVE_DATA;

		//etimer_set(&et, CLOCK_SECOND);
		//PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
		
	}
	else
	if(status==RECEIVE_DATA){
		//printf(" ----- RECEIVE DATA ------\r\n");
		PROCESS_WAIT_EVENT(); //Espera de dados
		if(ev==PROCESS_EVENT_MSG){
			//printf(" ----- DATA ------ %02x \r\n", msgSink);
			//msgSink[0] = data;
			//printf(" ----- DATA ------ %c \r\n", m);
			if(msgSink&0x01)
				leds_on(LEDS_RED);
			else
				leds_off(LEDS_RED);

			if((msgSink>>1)&0x01)
				leds_on(LEDS_YELLOW);
			else
				leds_off(LEDS_YELLOW);

			if((msgSink>>2)&0x01)
				leds_on(LEDS_GREEN);
			else
				leds_off(LEDS_GREEN);

			packetbuf_copyfrom("#*", 3); //ACK_2
			addr.u8[0] = 1;//Endereco do destino
    			addr.u8[1] = 0;
			unicast_send(&uc, &addr);
			status_recv = 0;
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
