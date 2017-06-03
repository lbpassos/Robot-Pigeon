# Robot-Pigeon
## Logbook

>**3-06-2017** - Communication module completed. mote to Xbee and xbee to mote.
Rime stack. All system integration started.

>**31-05-2017** - Communication between the two Contiki nodes Successful. Tests with unicast and broadcast both using the 
Rime stack. Work started on xbee.

>**27-05-2017** - Serial Interrupt working. All serial interface finished with success.

>**22-05-2017** - Robot (Pololu) movements finished with success.

>**19-05-2017** - Another aproach to the Serial line problem taken: The use of the Contiki shell. The solution runs OK in 
native Contiki environment but fails when uploaded to moteist5 with the same previous symptoms:  the event **serial_line_event_message** 
is not triggered. Work started in the robot.

>**17-05-2017** - Serial line test Unsuccessful. The data received from the serial line using Putty as the terminal is invalid.
>The characters received (when received) in the mote are over 127 (ascii). We also detect that the event **serial_line_event_message** is never propagated.

>**08-05-2017** - Test serial line in mote. Save in a flash and using a FET (Flash Emulator Terminal). Also draw line for robot to follow.

>**13-04-2017** - Testing in native of a multithread Contiki program. Two threads blocking and unblocking each other through messages.

>**12-04-2017** - Work started on Contiki OS \( Deployment on the moteist++s5 1011 based on a MSP430F5438A\).

>**10-04-2017** - System modelling ended.

>**27-03-2017** - Choosing which mote to use for the **WSN** nodes.