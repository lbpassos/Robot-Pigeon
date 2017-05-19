/**
 * \file   shell-robotpigeon.c
 *         Robot Contiki shell command
 * \author
 *          
 */

#include "contiki.h"
#include "shell.h"
#include <stdio.h>
#include <string.h>


/*---------------------------------------------------------------------------*/
PROCESS(shell_robot_process, "robot pigeon shell");
SHELL_COMMAND(robot_command,
	      "robot",
	      "robot <character>: character must be [0-7]",
	      &shell_robot_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(shell_robot_process, ev, data)
{

  PROCESS_BEGIN();
  
  /* Does echo */
  char *msg_1 = "Character Processed: ";
  char *msg_2 = "Character NOT Processed";
  
  if( *(char*)(data)>='0'  && *(char*)(data)<='7'){
	shell_output(&robot_command, msg_1, (int)strlen(msg_1), data, 1);
  }
  else{
	shell_output(&robot_command, msg_2, (int)strlen(msg_2), NULL, 0);
  }
  //shell_output(&robot_command, data, /*(int)strlen(data)*/1, NULL, NULL);
  
  PROCESS_END();
}


/*---------------------------------------------------------------------------*/
void
shell_robotpigeon_init(void)
{
  shell_register_command(&robot_command);
}
/*---------------------------------------------------------------------------*/
