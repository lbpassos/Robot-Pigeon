/**
 * \file
 *         Tmote Sky-specific My Contiki shell 
 *         Base code reference: sky-shell.c
 * \author
 *         Abhilash Hegde <hegdea@usc.edu>
 */

#include "contiki.h"
#include <string.h>
//#include "collect-view.h"

//#include "net/rime.h"


int serial_input_byte(unsigned char c);
void setLink();
void resetLink();
char msgToSend();


/*---------------------------------------------------------------------------*/
void robotPigeon_shell_init();
