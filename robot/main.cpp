#include "mbed.h"
#include "m3pi.h"
#include "robot.h"
#include "rtos.h"

//m3pi m3pi;
m3pi teste;
Thread thread;

void robotMove_thread() {
    teste.locate(0,1);
    teste.printf("Robot Pigeon");
    follow_line_sensor_init();
    wait(30);
    
    while (true) {
       follow_line();
    }
}

int main() {

    /*m3pi.locate(0,1);
    m3pi.printf("LO World");

    wait (2.0);

    m3pi.forward(0.5); // Forward half speed
    wait (0.5);        // wait half a second
    m3pi.left(0.5);    // Turn left at half speed
    wait (0.5);        // wait half a second
    m3pi.backward(0.5);// Backward at half speed 
    wait (0.5);        // wait half a second
    m3pi.right(0.5);   // Turn right at half speed
    wait (0.5);        // wait half a second

    m3pi.stop();*/ 
    /*teste.locate(0,1);
    teste.printf("Robot Pigeon");
    
    follow_line_sensor_init();
    wait(30);
    while(1){
        
        follow_line();
    }*/
    
    thread.start(robotMove_thread);
    while(true){ }
 
  
}

