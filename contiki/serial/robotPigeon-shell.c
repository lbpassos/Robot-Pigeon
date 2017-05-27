/**
 * \file
 *         Tmote Sky-specific My Contiki shell 
 *         Base code reference: sky-shell.c
 * \author
 *         Abhilash Hegde <hegdea@usc.edu>
 */

#include "contiki.h"
#include "robotPigeon-shell.h"
#include <string.h>
#include <stdio.h>
//#include "collect-view.h"

//#include "net/rime.h"
#define SIZEBUFFER	8
#define SIZEMSG		5 //4 + 0x00
#define ENTER_ASCII	13

static char buff[SIZEBUFFER];
static unsigned char ptrToInsert = 0;
static unsigned char buffIsReady = 0;
static unsigned char buffSize = 0;
static char msg;

/*---------------------------------------------------------------------------*/
PROCESS(myrobotpigeon_shell_process, "My Robot-Pigeon Contiki shell");
/*---------------------------------------------------------------------------*/

int serial_input_byte(unsigned char c){
	//printf("%c", c); //Fazer eco
	//printf("--------- No interrupt -------: %c %u\r\n", c, c);
	if(buffIsReady==0){
		//printf("--------- Antes -------: %c %u\r\n", c, c);
		if(c!=ENTER_ASCII){
			//printf("--------- aqui -------: %c %u\r\n", c, c);
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
			//printf("--------- NO else -------: %c %u\r\n", c, c);
			buff[ptrToInsert] = 0x00;		
			buffIsReady = 1;
			process_post(&myrobotpigeon_shell_process, PROCESS_EVENT_CONTINUE, NULL);
		}
	}
	return 1;
}

void resetInputBuff(){
	buffSize = 0;
	buffIsReady = 0;
	ptrToInsert = 0;
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
	char tmp[SIZEMSG];
	//printf("--------- Antes -------: %u\r\n", buff[0]);
	if( buff[0]==0x00){ //carregou enter
		shellMenu();
		resetInputBuff();
		return;
	}
	else{ 
		if( strcmp("stat", (const char *)buff) == 0 ){
			printf("stat: Link NOT Ready");
			//robot
		}
		else{
					
			strncpy(tmp, (const char *)buff, SIZEMSG-1);
			tmp[SIZEMSG-1]=0x00;
			if( strcmp("send", (const char *)tmp) == 0 ){
				if( buff[SIZEMSG-1]==' ' && buff[SIZEMSG]!=' '){
					msg = buff[SIZEMSG];
					printf("msg sent: %c", msg);
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
	//printf("--------- While -------:\r\n");
	PROCESS_WAIT_EVENT(); //Esperar evento
	if(ev==PROCESS_EVENT_CONTINUE){	//Evento recebido
		//printf("--------- Processar -------:\r\n");
		processMenu();
	}
  }

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
void robotPigeon_shell_init(){
	process_start(&myrobotpigeon_shell_process, NULL);
}
