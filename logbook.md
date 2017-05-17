# Robot-Pigeon
## Logbook

27-03-2017 - Choosing which mote to use for the **WSN** nodes.

10-04-2017 - System modelling ended.

12-04-2017 - Work started on Contiki OS \( Deployment on the moteist++s5 1011 based on a MSP430F5438A\). 

13-04-2017 - Testing in native of a multithread Contiki program. Two threads blocking and unblocking each other through messages.

08-05-2017 - Test serial line in mote. Save in flash and using FET (Flash Emulator Terminal). Also draw line for robot to follow.

17-05-2017 - Serial line test Unsuccessful. The data received from the serial line using Putty as the terminal is invalid.

The characters received (when received) in the mote are over 127 (ascii). We also detect that the event **serial_line_event_message** is not propagated.
