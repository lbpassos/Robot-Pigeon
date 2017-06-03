#include "mbed.h"

#define SIZE_PAYLOAD_CONTIKI    34

#define POSITION_ADDRESS_LOW_SINK        4            
#define POSITION_ADDRESS_LOW_SOURCE      6  

#define ADDRESS_LOW_SINK        0x01            
#define ADDRESS_LOW_SOURCE      0x02  

Serial xbee(p28, p27);
Serial log_serial(USBTX, USBRX);;
 
char messageIn[SIZE_PAYLOAD_CONTIKI];

void obtainSerialData(); //comandos
void obtainSeriaData_fromRadio(); //dados Payload
void changeRimeHeader();
void sendSerialData();

DigitalOut myled(LED1);

int main() {
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
    xbee.printf("ATDL100\r"); //DESTINO ADD (Low) 0x0300
    log_serial.printf("\r\nSet ADD (Low).....:");
    obtainSerialData();
    
    
    xbee.printf("ATMY200\r"); //ORIGEM ADD  0x200
    log_serial.printf("\r\nSet Endereco Origem.....:");
    obtainSerialData();
    
    xbee.printf("ATMM2\r"); //ORIGEM MAC Mode 2  na recepcao
    log_serial.printf("\r\nSet MAC Mode.....:");
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
    
    //xbee.printf("ATVL\r"); //canal 1A
    //wait(1);
    //log_serial.printf("\r\nFirmware.....:");
    //obtainSerialData();
    
    /*xbee.printf("ATCH\r");
    log_serial.printf("TRES.....\r\n");
    obtainSerialData();
    
    xbee.printf("ATAP\r");
    log_serial.printf("QUATRO.....\r\n");
    obtainSerialData();
    log_serial.printf("Espera.....\r\n");*/
    //wait(20);
    xbee.printf("ATCN\r");
    log_serial.printf("\r\nSair Modo comando.....:");
    obtainSerialData();
    
    
    //log_serial.printf("\r\nSai do modo comando.....\r\n");

    while(1){
        log_serial.printf("\r\n");
        obtainSeriaData_fromRadio();
        //if(sz<0){
        //    continue;
        //}
        //log_serial.printf("\r\nTamanho Recebido: %u", sz);
        //for(int i=0; i<34; ++i){
          //  xbee.putc(messageToSend[i]);
        //}
        log_serial.printf("PAYLOAD RECEBIDO: \r\n");
        for(int i=0; i<SIZE_PAYLOAD_CONTIKI; ++i)
            log_serial.printf(" %02x ", messageIn[i]);
            
        changeRimeHeader();
        
        log_serial.printf("\r\nPAYLOAD A ENVIAR: \r\n");
        for(int i=0; i<SIZE_PAYLOAD_CONTIKI; ++i)
            log_serial.printf(" %02x ", messageIn[i]);
        
        int count = 0;   
        while(1){
            sendSerialData();
            ++count;
            log_serial.printf("\r\nEnviei %d \r\n", count);
            wait(20);
        }
        
        //obtainSerialData();
        //log_serial.printf("\r\nRecebido.....\r\n");
        //xbee.printf("Hello Leni");
        //wait(10);
        //log_serial.printf("\r\nEnviei.....\r\n");
        //myled = !myled;
         
    }

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

void obtainSeriaData_fromRadio()
{
    int idx = 0;
    while(idx < SIZE_PAYLOAD_CONTIKI){
        while(!xbee.readable()); // espera dados
        messageIn[idx++] = xbee.getc();
    }
}

//Mudar o payload
void changeRimeHeader(){
    messageIn[POSITION_ADDRESS_LOW_SINK] = ADDRESS_LOW_SINK;
    messageIn[POSITION_ADDRESS_LOW_SOURCE] = ADDRESS_LOW_SOURCE;
}         


void sendSerialData()
{
    int idx = 0;
    while(idx < SIZE_PAYLOAD_CONTIKI){
        while(!xbee.writeable()); // espera dados
        xbee.putc(messageIn[idx++]);
    }
    
    
  
}
