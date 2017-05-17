/*
 * serial.c
 *
 *  Testar
 *
 */


#include "contiki.h"
#include "dev/serial-line.h"
#include <stdio.h>

	PROCESS(test_serial, "Serial line test process");
	AUTOSTART_PROCESSES(&test_serial);

	PROCESS_THREAD(test_serial, ev, data)
	{
		PROCESS_BEGIN();
		static uint32_t seconds_green = 30;
		static struct etimer et_green; //define the timers
		etimer_set(&et_green, CLOCK_SECOND*seconds_green);  // Set the timers
		
		while(1){		
			PROCESS_WAIT_EVENT();
		 	if(etimer_expired(&et_green)) {  // If the event it's the timer expiration, then...
				printf("aqui");
				break;
    			}
		}
		for(;;) {
			printf("espera\n");
			//PROCESS_YIELD();
			PROCESS_WAIT_EVENT();
 			if(ev == serial_line_event_message) {
   				printf("received line: %s\n", (char *)data);
 			}
		}
		PROCESS_END();
	}
