#include "mbed.h"
 #include "BufferedSerial.h"
 #include "robot.h"
 #include "m3pi.h"
//#include "rtos.h"

#define SIZE_PAYLOAD_CONTIKI    34

#define PAYLOAD_CONTIKI_MSG_POS 8    

#define COUNT_SILENCE           5 //5 SEGUNDOS

#define POSITION_ADDRESS_LOW_SINK        4            
#define POSITION_ADDRESS_LOW_SOURCE      6  

#define ADDRESS_LOW_SINK        0x01            
#define ADDRESS_LOW_SOURCE      0x02  

#define START                               0
#define SENDING_ACK_TO_MOTE_TRX             1
#define WAIT_FOR_DATA                       2
#define MOVE_ROBOT                          3
#define TRANSFER_TO_OTHER_ROBOT             4
#define FINISH_TRANSFER_DATA_TO_OTHER_ROBOT 5
#define RETURN_TO_BASE                      6





BufferedSerial xbee(p28, p27);

Serial log_serial(USBTX, USBRX);
 
char messageIn[SIZE_PAYLOAD_CONTIKI];
char messageOut[SIZE_PAYLOAD_CONTIKI];
char messageStore[SIZE_PAYLOAD_CONTIKI];

char msgACK[SIZE_PAYLOAD_CONTIKI]={0x00, 0x08, 0x1A, 0x00, 0x01, 0x00, 0x02, 0x00, '#'};
char msgACK_1[SIZE_PAYLOAD_CONTIKI]={0x00, 0x08, 0x1A, 0x00, 0x01, 0x00, 0x02, 0x00, '*'};

//char msgTeste[SIZE_PAYLOAD_CONTIKI]={0x00, 0x08, 0x1A, 0x00, 0x01, 0x00, 0x02, 0x00, 'H', 'E', 'L', 'L', 'O'};

void obtainSerialData(); //comandos
//int obtainSeriaData_fromRadio(); //dados Payload
int readContiki_frame();
//void changeRimeHeader();
void sendSerialData();

DigitalOut myled(LED1);


static char status = START;
//static char status = MOVE_ROBOT; //Para teste robot a robot



    
int main() {
    int i;// int j=13;
    for(i=9; i<SIZE_PAYLOAD_CONTIKI; ++i){
        msgACK[i]=0x00;
        msgACK_1[i]=0x00;
        
        //if(j<SIZE_PAYLOAD_CONTIKI)
         //   msgTeste[j++]=0x00;
    }
    
    /* Para teste robot a robot */
    /*memcpy( (char *)messageStore, (char *)msgTeste, SIZE_PAYLOAD_CONTIKI);
    log_serial.printf("\r\nMENSAGEM: \r\n");
    for(int i=0; i<SIZE_PAYLOAD_CONTIKI; ++i)
        log_serial.printf(" %02x ", messageStore[i]);*/
    /* ----------------------------------------------------------------- */
    
    xbee.baud(9600);

    log_serial.printf("A Comecar.....\r\n");
    
    wait(1); //segundos
    xbee.printf("+++"); //Entrar em modo de comando
    wait(1); //segundos
    log_serial.printf("\r\nModo Comando.....:");
    obtainSerialData();
    
    xbee.printf("ATCH1A\r"); //canal 1A
    log_serial.printf("\r\nSet Canal.....:");
    obtainSerialData();
    
    xbee.printf("ATIDABCD\r"); //Pan ID ABCD
    log_serial.printf("\r\nSet PANID.....:");
    obtainSerialData();
    
    //Enviar
    xbee.printf("ATDH0\r"); //DESTINO ADD (High) 0
    log_serial.printf("\r\nSet ADD (High).....:");
    obtainSerialData();
    xbee.printf("ATDL100\r"); //DESTINO ADD (Low) 0x0100
    log_serial.printf("\r\nSet ADD (Low).....:");
    obtainSerialData();
    
    
    xbee.printf("ATMY200\r"); //ORIGEM ADD  0x200
    log_serial.printf("\r\nSet Endereco Origem.....:");
    obtainSerialData();
    
    xbee.printf("ATMM2\r"); //ORIGEM MAC Mode 2  na recepcao
    log_serial.printf("\r\nSet MAC Mode.....:");
    obtainSerialData();
    
    xbee.printf("ATEA0\r"); //reset ack
    log_serial.printf("\r\nATEA.....:");
    obtainSerialData();
    
    xbee.printf("ATGT\r"); //reset ack
    log_serial.printf("\r\nATGT.....:");
    obtainSerialData();
    
    
    xbee.printf("ATCH\r"); //canal 1A
    //wait(1);
    log_serial.printf("\r\nCanal.....:");
    obtainSerialData();
    
    xbee.printf("ATID\r"); //canal 1A
    //wait(1);
    log_serial.printf("\r\nPanID.....:");
    obtainSerialData();
    
    xbee.printf("ATMY\r"); //canal 1A
    //wait(1);
    log_serial.printf("\r\nMeu ADD.....:");
    obtainSerialData();
    
    xbee.printf("ATMM\r"); //MAC Mode 2
    log_serial.printf("\r\nMAC Mode.....:");
    obtainSerialData();
    
    //Enviar
    xbee.printf("ATDH\r"); //DESTINO ADD (High) 0
    log_serial.printf("\r\nADD (High).....:");
    obtainSerialData();
    xbee.printf("ATDL\r"); //DESTINO ADD (Low) 0x0200
    log_serial.printf("\r\nADD (Low).....:");
    obtainSerialData();
    
    
    xbee.printf("ATEA\r"); //saber retransmissoes
    log_serial.printf("\r\nATEA.....:");
    obtainSerialData();
    
    xbee.printf("ATCN\r");
    log_serial.printf("\r\nSair Modo comando.....:");
    obtainSerialData();
    
    //iniciar robot
    /*              */
    follow_line_sensor_init();
    wait(30);
    
    while(1){
        switch(status){
            case START:
                log_serial.printf("\r\nNo START 10s: \r\n");
                memcpy( (char *)messageOut, (char *)msgACK, SIZE_PAYLOAD_CONTIKI);
                status = SENDING_ACK_TO_MOTE_TRX;    
                break;
            case SENDING_ACK_TO_MOTE_TRX: //Bombaredear o moteTRX
                sendSerialData();
                wait_ms(50);
                if( readContiki_frame()==1 ){
                    if( messageIn[PAYLOAD_CONTIKI_MSG_POS] == '#' && messageIn[PAYLOAD_CONTIKI_MSG_POS+1] == 0x00){
                        log_serial.printf("\r\nACK recebido: \r\n");
                        status = WAIT_FOR_DATA;
                    }
                }    
                break;
            case WAIT_FOR_DATA:
                wait_ms(500);
                if( readContiki_frame()==1 ){
                    //log_serial.printf("\r\nPAYLOAD RECEBIDO do outro: \r\n");
                    //for(int i=0; i<SIZE_PAYLOAD_CONTIKI; ++i)
                    //    log_serial.printf(" %02x ", messageIn[i]);
                    //while(1);//Mover robot
                    memcpy( (char *)messageStore, (char *)messageIn, SIZE_PAYLOAD_CONTIKI); //guardar
                    status = MOVE_ROBOT;
                    log_serial.printf("\r\nJA TENHO DATA: \r\n");
                }
                break;
            case MOVE_ROBOT:
                if( follow_line()==1 ){//chegou
                    memcpy( (char *)messageOut, (char *)msgACK_1, SIZE_PAYLOAD_CONTIKI); //guardar
                    wait(5);//garantir limpeza buffer do outro robot
                    status = TRANSFER_TO_OTHER_ROBOT;
                }
                else{
                    wait_ms(10);
                }
                
                //log_serial.printf(" A ANDAR \r\n");
                //
                //wait(10);
                
                //log_serial.printf("\r\nANDAR: \r\n");
                //memcpy( (char *)messageOut, (char *)msgACK_1, SIZE_PAYLOAD_CONTIKI); //guardar
                //wait(5);//garantir limpeza buffer do outro robot
                //status = TRANSFER_TO_OTHER_ROBOT;
                break;
            case TRANSFER_TO_OTHER_ROBOT:
                sendSerialData();
                //wait_ms(500);
                wait(1);
                if( readContiki_frame()==1 ){
                    if( messageIn[PAYLOAD_CONTIKI_MSG_POS] == '*' && messageIn[PAYLOAD_CONTIKI_MSG_POS+1] == 0x00){
                        log_serial.printf("\r\nACK_1 recebido: \r\n");
                        memcpy( (char *)messageOut, (char *)messageStore, SIZE_PAYLOAD_CONTIKI); //guardar
                        status = FINISH_TRANSFER_DATA_TO_OTHER_ROBOT;
                    }
                }    
                break;
            case FINISH_TRANSFER_DATA_TO_OTHER_ROBOT:
                sendSerialData();
                //wait_ms(500);
                wait(1);
                if( readContiki_frame()==1 ){
                    if( messageIn[PAYLOAD_CONTIKI_MSG_POS] == '*' && messageIn[PAYLOAD_CONTIKI_MSG_POS+1] == 0x00 ){
                        log_serial.printf("\r\nACK_1 recebido DATA ENVIADO: \r\n");
                        status = RETURN_TO_BASE;
                    }
                }    
                break;
            case RETURN_TO_BASE:
                if( follow_line()==1 ){//chegou
                     status = START;
                }
                else{
                    wait_ms(10);
                    break;
                }
                
                log_serial.printf(" Cheguei \r\n");
                
                /* Periodo de silencio para garantir (simular) duas redes diferentes. 
                   XBEE em modo AT nao permite a mudanca de endereco dinamico.
                   Elimina o momento critico que e: o burst do Robot 2 par o moteRX (garantir entrega)
                                                    o burst do Robot 1 par o moteTX (informar TX que link ready)
                */
                wait(60);//espera 60 s
                log_serial.printf(" ESPERA DE LIBERTAR \r\n");
                //while( readContiki_frame()!=1 );
                
                int i=0;
                while( 1 ){
                    wait(1);
                    if( xbee.readable()==0 ){
                        ++i;
                        if( i==COUNT_SILENCE ) // 5 segundos de silencio consecutivo. Canal livre
                            break;
                    }
                    else{
                        i = 0;
                        while( xbee.readable() ){ // Limpar buffer
                            xbee.getc();
                        }
                    }
                }

                //while(1);
                status = START;
                break;
                                        
        }//end_switch
    }//end_while  



    

} // end of the main function

void obtainSerialData()
{
    char nxtChr;
    while(1){
        while(xbee.readable()) 
        {
            nxtChr =  xbee.getc();
            log_serial.printf("%c", nxtChr);
            
            if (nxtChr == '\0')
            {
                return;
            }
            if (nxtChr == '\n' || nxtChr == '\r')
            {
                return;
            }
        }
    }
}

/*int obtainSeriaData_fromRadio()
{
    int idx = 0;
    
    if( !xbee.readable() )
        return 0;
        
    while(idx < SIZE_PAYLOAD_CONTIKI){ //SIZE_PAYLOAD_CONTIKI_ACK
         log_serial.printf("\r\n idx %d \r\n", idx);
        while( !xbee.readable() ); // espera dados
        messageIn[idx++] = xbee.getc();
        //wait_us(500);
    }
    return 1;
}*/

int readContiki_frame(){
    int idx = 0;
    
    if( !xbee.readable() )
        return 0;
    
    while(idx < SIZE_PAYLOAD_CONTIKI){ //SIZE_PAYLOAD_CONTIKI_ACK
         //log_serial.printf("\r\n idx %d \r\n", idx);
        while( xbee.readable() ){ // espera dados
            messageIn[idx++] = xbee.getc();
        }
    }
    return 1;
}



//Mudar o payload
void changeRimeHeader(){
    messageIn[POSITION_ADDRESS_LOW_SINK] = ADDRESS_LOW_SINK;
    messageIn[POSITION_ADDRESS_LOW_SOURCE] = ADDRESS_LOW_SOURCE;
}         


void sendSerialData()
{
    int idx = 0;
    
    //Timer serial_update_timer;
    
    //serial_update_timer.start();
    while(idx < SIZE_PAYLOAD_CONTIKI){
        //idx_Uart = 0;
        //while( idx_Uart<SIZE_BUFFER_UART_LPC1768 ){
            while( !xbee.writeable() ); // espera dados
            xbee.putc(messageOut[idx++]);
            //wait_us(105);
            //++idx_Uart;
                //serial_update_timer.reset();
            //}
        //}//end_uart
        //wait_ms(2);
    }
    //ait(1);
}
