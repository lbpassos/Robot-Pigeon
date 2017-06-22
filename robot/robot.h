#ifndef __ROBOT_H_
#define __ROBOT_H_

#include "mbed.h"
#include "m3pi.h"

// Follow Line
#define SENSOR_DARK 300 // Sensibilidade dos sensores: a acima de SENSOR_DARK -> escuro  a baixo = claro
// Quanto mais pequeno  mais sensivel a cor

#define WHITE 1
#define BLACK 0

#define Reverse     0
#define H_Left      1
#define M_Left      2
#define S_Left      3
#define Forward     4
#define S_Right     5
#define M_Right     6
#define H_Right     7
#define Stop        8
#define Slow        9
#define Invert_Dir  10
#define Repeat      255

#define STOP_V  0
#define SPEED   1

#define Soft_Turn_value     SPEED - 0.15*SPEED  // 0.08
#define Medium_Turn_value   SPEED - 0.25*SPEED  // 0.15
#define Hard_Turn_value     SPEED - 0.4*SPEED   // 0.25

#define Slow_value          0.5*SPEED   // em %

// -----------------------------------------------------------------------

// Tasks
#define STACK_SIZE_MODE              128*4   
#define STACK_SIZE_FOLLOWLINE        128*4    
#define STACK_SIZE_COMMUNICATION     128*4   
#define STACK_SIZE_CONFIG            128*4   
#define STACK_SIZE_OBJDIST           128*4    

// Task Priorities
#define PRIORITY_MAX        4
#define PRIORITY_HIGH       3
#define PRIORITY_NORMAL     2
#define PRIORITY_LOW        1
#define PRIORITY_IDLE       0


// ------ Controls CMD ----------------------------------------------------------
// 
// |  0 |X/Y |   [0 - 200]:(0 - 63)        |  <- Apenas Analógicos   (ignorado em modo pista e modo config)
// |  1 |P/L |Conf| -- | -- | -- | -- | -- |  <- Apenas Botões   (Mode) < comandos prioritarios
// |  1 |  1 |  1 | OK | A  | B  | -- | -- |  <- Apenas Botões   (Config) < botoes em modo config
// |  1 |  1 |  1 | XX | XX | YY | YY | -- |  <- Apenas Botões   (Livre) < botoes em modo livre
// |  1 |  1 |  1 |  0 |  0 |  0 |  0 |  1 |  <- reservado
// |  0 |  0 |  0 |  0 |  1 |  0 |  1 |  0 |  <- valor ignorado (\n)
// |  0 |  0 |  0 |  0 |  1 |  1 |  0 |  1 |  <- valor ignorado (\r)

#define CMD_TYPE                7   // Tipo : analógico ou botões (0 / 1)
#define CMD_XY                  6   // X & Y (0 / 1)
#define CMD_MODO                5   // Pista / Livre / config 
#define CMD_OK                  4   // Botão OK (menu config)
#define CMD_A                   3   // Botão A  (menu config)
#define CMD_B                   2   // Botão B  (menu config)
#define CMD_XX                  3   // Botões XX (modo livre)
#define CMD_YY                  1   // Botões YY (modo livre)
#define CMD_ANG_VAL             0   // Valor dos analógicos (shift da mascara)
//--------------------------------------------------------------------------------
// Modo Funcionamento |P/L |Conf|:
// | 0 | 0 | -> Pista
// | 1 | 0 | -> Livre 
// | 0 | 1 | -> Config
// | 1 | 1 | -> Manter anterior
#define CMD_MODO_PISTA          0b00
#define CMD_MODO_LIVRE          0b10
#define CMD_MODO_CONFIG         0b01
#define CMD_MODO_KEEP           0b11
//--------------------------------------------------------------------------------
// Modo Livre Botões
#define CMD_LIVRE_NOP        0b00
#define CMD_LIVRE_FRENTE     0b01   
#define CMD_LIVRE_TRAZ       0b10
#define CMD_LIVRE_SPIN       0b11
//--------------------------------------------------------------------------------
#define CMD_TYPE_MASK           1<<CMD_TYPE             // mascara do tipo de cmd recebido (valores dos analógicos ou dos botões)
#define CMD_AND_MODE            0                       // analógico
#define CMD_BTN_MODE            1                       // botões
#define CMD_XY_MASK             1<<CMD_XY               // mascara do modo do valor analógico (speed ou direction)
#define CMD_X                   0                       // speed
#define CMD_Y                   1                       // direction
#define CMD_MODO_MASK           (0b11)<<CMD_MODO             // Modo de funcionamento (pista ou livre)
#define CMD_PISTA               0
#define CMD_LIVRE               1
#define CMD_OK_MASK             1<<CMD_OK
#define CMD_A_MASK              1<<CMD_A
#define CMD_B_MASK              1<<CMD_B
#define CMD_XX_MASK             (0b11)<<CMD_XX
#define CMD_YY_MASK             (0b11)<<CMD_YY
#define CMD_ANG_VAL_MASK        (0b111111)<<CMD_ANG_VAL
// -------------------------------------------------------------------------------
// CMD AUX
#define FL_STOP     0
#define FL_INVERT   1

#define SINGLE_ANALOG   0
#define DOUBLE_ANALOG   1

#define BTN_RELEASE     0b10
#define BTN_PRESSING    0b11
#define BTN_PRESSED     0b01  
#define BTN_UNPRESSED   0b00
#define BTN_HIGH        1
#define BTN_LOW         0    

// Sensor Frontal
#define RANGING_TIME        100 / portTICK_RATE_MS

typedef enum workMode {PISTA = 0,CONFIG = 1,LIVRE = 2} workMode; // exemplo

typedef enum menu {MENU_EXIT = 0,MENU_1,MENU_1_IN,MENU_2,MENU_2_IN,MENU_3,MENU_3_IN,MENU_4,MENU_4_IN,MENU_5,MENU_5_IN,MENU_6,MENU_6_IN} menu;

//--------------------------------------------------------------------------------
void Robot_Tasks_init();

void Mode_Task(void * pvParameters);

int follow_line();

void follow_line_sensor_init();

void checksensors(char lineColor);

void updatesensors();

char directionCalc(char lineColor, char prevDirection);

void invertDirection();

void reset_robot();

void livre_update_motores();

void CMD_Process();

unsigned char button_read(unsigned char * button_curr, unsigned char * button_prev);

void Config_Task(void * pvParameters);

void OBJDist_Task(void * pvParameters);

#endif