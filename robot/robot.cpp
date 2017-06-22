#include "robot.h"
#include "m3pi.h"

#define DELAY_TESTE        5000 / portTICK_RATE_MS

DigitalOut myleds[] = {(LED1), (LED2), (LED3), (LED4)};

m3pi m3pi;


//SRF08 srf08(p28, p27, 0xE0); // E0
//I2C srf08(p28, p27); // E0

int sensors[5];

// Follow Line Mode 
char direction_Next = Stop;
char direction_Prev = Stop;

// config vars
char follow_line_mode = FL_STOP; 
float follow_line_speed = 0.5;
float livre_max_speed   = 0.7; 
char lineColor = WHITE;
char LIVRE_ADC_INPUT_TYPE = DOUBLE_ANALOG;
unsigned int minDist = 50;  // cm


//-----------

// hash table to follow white line
static const char direction_hashTable[] =        {Repeat,     // 00000
                                            H_Right,    // 00001
                                            S_Right,    // 00010
                                            M_Right,    // 00011    
                                            Forward,    // 00100
                                            Repeat,     // 00101
                                            S_Right,    // 00110
                                            S_Right,    // 00111
                                            S_Left,     // 01000
                                            Repeat,     // 01001
                                            Repeat,     // 01010
                                            Repeat,     // 01011
                                            S_Left,     // 01100   
                                            Repeat,     // 01101   
                                            Forward,    // 01110
                                            S_Right,    // 01111
                                            H_Left,     // 10000
                                            Forward,    // 10001
                                            Repeat,     // 10010
                                            S_Left,     // 10011
                                            Repeat,     // 10100
                                            Repeat,     // 10101
                                            Repeat,     // 10110
                                            S_Left,     // 10111
                                            M_Left,     // 11000
                                            Repeat,     // 11001
                                            Repeat,     // 11010
                                            Forward,    // 11011
                                            S_Left,     // 11100 
                                            S_Right,    // 11101
                                            S_Left,     // 11110
                                            Forward};   // 11111


static const float right_motor_hashTable[] =      {-SPEED,                // Reverse
                                            (Hard_Turn_value),      // Hard Left
                                            (Medium_Turn_value),    // Medium Left
                                            (Soft_Turn_value),      // Soft Left
                                            SPEED,                  // Forward
                                            SPEED,                  // Soft Right
                                            SPEED,                  // Medium Right
                                            SPEED,                  // Hard Right
                                            STOP_V,                 // Stop
                                            Slow_value};           // Slow
                                            
static const float left_motor_hashTable[] =       {-SPEED,                // Reverse
                                            SPEED,                  // Hard Left
                                            SPEED,                  // Medium Left
                                            SPEED,                  // Soft Left
                                            SPEED,                  // Forward
                                            (Soft_Turn_value),      // Soft Right
                                            (Medium_Turn_value),    // Medium Right
                                            (Hard_Turn_value),      // Hard Right
                                            STOP_V,                 // Stop
                                            Slow_value};           // Slow




/* */
void follow_line_sensor_init(){
    direction_Prev = Stop;
    m3pi.sensor_auto_calibrate();
}

/* */
void updatesensors(){
    int sensorsVal[5];
    m3pi.readsensor(sensorsVal);
    int s;
    for(s = 0; s < 5; s++){
        if (sensorsVal[s]>SENSOR_DARK){ // BLACK
            sensors[s] = BLACK;
        }
        else{                           // WHITE
            sensors[s] = WHITE;
        }
    }
}

/* */
char directionCalc(char lineColor, char prevDirection){
    char position = 0;
    char sensorVal = 0;
    int s;
    
    if(lineColor == WHITE)
        for(s = 0; s < 5; s++){
            sensorVal |= sensors[s]<<(4-s);
        }
    else
        for(s = 0; s < 5; s++){
            sensorVal |= (!(sensors[s]))<<(4-s);
        }
    
    position = direction_hashTable[sensorVal];
   
    /*if(position == Repeat)
        return prevDirection;
    */
    return position;
}

/* */
void invertDirection(){
    m3pi.right_motor(0.2);
    m3pi.left_motor(-0.2);
    wait_ms(675);
    m3pi.right_motor(0);
    m3pi.left_motor(0);
    //wait(5);    
}





int follow_line(){

    updatesensors();
    direction_Next = directionCalc(lineColor, direction_Prev);
    
    if( direction_Next==Repeat ){
        m3pi.stop();
        wait(5);
        invertDirection();
        //wait(10);
        return 1;
    }
    
    m3pi.right_motor(follow_line_speed*right_motor_hashTable[direction_Next]);
    m3pi.left_motor(follow_line_speed*left_motor_hashTable[direction_Next]);
    direction_Prev = direction_Next;
    return 0;
}     
    
 

            /*m3pi.locate(0,0);
            m3pi.printf("M:CONFIG");
            m3pi.locate(0,1);
            m3pi.printf("3>LNCOLR");
            */
            
