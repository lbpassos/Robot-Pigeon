/**
 * \file
 *         Tmote Sky-specific My Contiki shell 
 *         Base code reference: sky-shell.c
 * \author
 *         
 */

#include "contiki.h"
#include "robotPigeon-shell.h"

/*---------------------------------------------------------------------------*/
PROCESS(robotpigeon_process, "My Robot-Pigeon");
AUTOSTART_PROCESSES(&robotpigeon_process); //autoiniciar o processo
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
PROCESS_THREAD(robotpigeon_process, ev, data)
{
  PROCESS_BEGIN();

  robotPigeon_shell_init();
  

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/

