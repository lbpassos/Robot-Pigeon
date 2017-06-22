/**
 * \file
 *         Robot Pigeon
 *         Base code reference: sky-shell.c
 * \author
 *         
 */

#include "contiki.h"
#include "robotPigeon-radio.h"

/*---------------------------------------------------------------------------*/
PROCESS(robotpigeon_process, "My Robot-Pigeon");
AUTOSTART_PROCESSES(&robotpigeon_process); //autoiniciar o processo
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
PROCESS_THREAD(robotpigeon_process, ev, data)
{
  PROCESS_BEGIN();

  robotPigeon_radio_init();
  

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/

