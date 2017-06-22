/**
 * \file
 *         Tmote Sky-specific My Contiki shell 
 *         Base code reference: sky-shell.c
 * \author
 *         Abhilash Hegde <hegdea@usc.edu>
 */

#include "contiki.h"
#include <string.h>
#include <stdio.h>
//#include "collect-view.h"

//#include "net/rime.h"
#include "robotPigeon-radio.h"
#include "robotPigeon-shell.h"
//#include "vari.h"

#define SIZEBUFFER	8
#define SIZEMSG		5 //4 + 0x00
#define ENTER_ASCII	13

static char buff[SIZEBUFFER];
static unsigned char ptrToInsert = 0;
static unsigned char buffIsReady = 0;
static unsigned char buffSize = 0;
static unsigned char robotIsReady = 0;
static char msg;

/*---------------------------------------------------------------------------*/
PROCESS(myrobotpigeon_shell_process, "My Robot-Pigeon Contiki shell");
/*---------------------------------------------------------------------------*/

int serial_input_byte(unsigned char c){
	//printf("No interrupt\r\n");
	if(buffIsReady==0){
		if(c!=ENTER_ASCII){
			buff[ptrToInsert] = c;
			++buffSize;
			++ptrToInsert;
			ptrToInsert = ptrToInsert % SIZEBUFFER;
			if(ptrToInsert==0){
				buffSize = 0;
			}
		}
		else 
		if(c==ENTER_ASCII){
			//printf("ENTER\r\n");
			buff[ptrToInsert] = 0x00;		
			buffIsReady = 1;
			process_post(&myrobotpigeon_shell_process, PROCESS_EVENT_CONTINUE, NULL);
		}
	}
	return 1;
}

void setLink(){
	robotIsReady = 1;
}

void resetLink(){
	robotIsReady = 0;
}

void resetInputBuff(){
	buffSize = 0;
	buffIsReady = 0;
	ptrToInsert = 0;
}

char msgToSend(){
	return msg;
}
void shellMenu(){
	printf("\r\n");
	printf("\r\n");
	printf("\r\n");
	printf("Robot-Pigeon Available Commands \r\n");
	printf("    stat: gives the status (Ready or not Ready) of the link\r\n");
	printf("    send <msg>: send msg (Value between 0 and 7)\r\n");
	printf("    Press Enter to see the menu\r\n");
	printf("> ");
}

void processMenu(){
	//printf("No ProcessMenu\r\n");
	char tmp[SIZEMSG];
	if( buff[0]==0x00 ){ //carregou enter
		shellMenu();
		resetInputBuff();
		return;
	}
	else{ 
 		if( strcmp("stat", (const char *)buff) == 0 ){//check link status
			if( robotIsReady==0 )            		
				printf("stat: Link NOT Ready");
			else
				printf("stat: Link Ready");
        	}
		else{
			strncpy(tmp, (const char *)buff, SIZEMSG-1);
			tmp[SIZEMSG-1]=0x00;
			if( strcmp("send", (const char *)tmp) == 0 ){
				if( buff[SIZEMSG-1]==' ' && buff[SIZEMSG]!=' ' && robotIsReady==1 ){
					msg = buff[SIZEMSG];
					printf("msg sent: %c", msg);
					process_post(&radio_unicast_process, PROCESS_EVENT_MSG, msg);					
					//Enviar robot
					resetLink();//Robot a andar
				}
				else{
					printf("stat: Link NOT Ready");
				}
			}
			else{
				printf("Invalid Command");
			}
		}
	}
	printf("\r\n> ");
	resetInputBuff();
}



/*---------------------------------------------------------------------------*/
PROCESS_THREAD(myrobotpigeon_shell_process, ev, data)
{
  PROCESS_BEGIN();
  
  shellMenu();	
  while(1){
	//printf("Espera enevto\r\n");
	PROCESS_WAIT_EVENT(); //Esperar evento
	if(ev==PROCESS_EVENT_CONTINUE){	//Evento recebido
		processMenu();
	}
  }

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
void robotPigeon_shell_init(){
	process_start(&myrobotpigeon_shell_process, NULL);
}
