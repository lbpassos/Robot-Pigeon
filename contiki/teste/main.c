
/**
 * \file
 *         A very simple Contiki application showing how Contiki programs look
 * \author
 *         Adam Dunkels <adam@sics.se>
 */

#include "contiki.h"

#include <stdio.h> /* For printf() */
#include "sys/etimer.h" //para o timer
#include <string.h>


/*---------------------------------------------------------------------------*/
PROCESS(first_process, "first_process");
PROCESS(second_process, "second_process");

AUTOSTART_PROCESSES(&first_process, &second_process); //autoiniciar o processo

static char msg[] = " ---- Dados ----";
static const char *fim = "Fim";

static char msg_dois[] = "Fim";
static char cnt[] = "e";
static int count = 0;

PROCESS_THREAD(first_process, ev, data)
{
  static struct etimer et;
  
  PROCESS_BEGIN();
  
  etimer_set(&et, CLOCK_SECOND*10);//activa 10 s
  while(1){
	  printf("Espera de entrar no processo 1\n");
	  PROCESS_WAIT_EVENT_UNTIL( etimer_expired(&et) );
          printf("Estou no processo 1\n");
	  process_post(&second_process, PROCESS_EVENT_CONTINUE, &msg);	
	  while(1){
		printf("processo 1 - A espera que o 2 acabe\n");
		PROCESS_WAIT_EVENT();
		if(ev==PROCESS_EVENT_CONTINUE){
			printf("processo 1 - 2 acabou\n");
			etimer_restart(&et); // Restart the timer from current time.			
			break;
		}
	  }
	  if( strcmp(fim, (const char *)data) == 0 ){
		break;
	  }
          
  }
  printf(" ----- Acabou processo 1 ------\n");
  PROCESS_END();
}
/*---------------------------------------------------------------------------*/


PROCESS_THREAD(second_process, ev, data)
{
  
  PROCESS_BEGIN();
  
    
  while(1){

	  printf("Espera de entrar no processo 2\n");
	  PROCESS_WAIT_EVENT();
	  
	  if(ev==PROCESS_EVENT_CONTINUE){
 		printf("Estou no processo 2\n");			
		if(count == 1){
			process_post(&first_process, PROCESS_EVENT_CONTINUE, &msg_dois);
			break;
		}
		else{
			process_post(&first_process, PROCESS_EVENT_CONTINUE, &cnt);
		}
		++count;
		printf("Sai do processo 2\n");
	  }	  
 }
  printf(" ----- Acabou processo 2 ------\n");
  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
