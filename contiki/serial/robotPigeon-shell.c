/*
 * Copyright (c) 2008, Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * This file is part of the Contiki operating system.
 *
 */

/**
 * \file
 *         Tmote Sky-specific My Contiki shell 
 *         Base code reference: sky-shell.c
 * \author
 *         Abhilash Hegde <hegdea@usc.edu>
 */

#include "contiki.h"
#include "shell.h"
#include "serial-shell.h"

//#include "collect-view.h"

//#include "net/rime.h"

/*---------------------------------------------------------------------------*/
PROCESS(myrobotpigeon_shell_process, "My Robot-Pigeon Contiki shell");
AUTOSTART_PROCESSES(&myrobotpigeon_shell_process);
/*---------------------------------------------------------------------------*/
#define WITH_PERIODIC_DEBUG 0
#if WITH_PERIODIC_DEBUG
static struct ctimer debug_timer;
static void
periodic_debug(void *ptr)
{
  ctimer_set(&debug_timer, 20 * CLOCK_SECOND, periodic_debug, NULL);
  collect_print_stats();
}
#endif /* WITH_PERIODIC_DEBUG */
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(myrobotpigeon_shell_process, ev, data)
{
  PROCESS_BEGIN();

#if WITH_PERIODIC_DEBUG
  ctimer_set(&debug_timer, 20 * CLOCK_SECOND, periodic_debug, NULL);
#endif /* WITH_PERIODIC_DEBUG */

  serial_shell_init();
  /*shell_blink_init();
  shell_file_init();

  shell_ps_init();
  shell_reboot_init();

  shell_sky_init();
  shell_power_init();
  shell_powertrace_init();

  shell_text_init();
  shell_time_init();
*/
  //shell_collect_view_init();

  shell_robotpigeon_init();
  
  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
