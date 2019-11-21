#include <xc.h>
#include "can_command.h"
#include "mcc_generated_files/ecan.h"
#include "L6470.h"
#include "L6470_SPI.h"
#include "servo.h"

/*
union {
    unsigned char c[8] ;
    unsigned int  i;
} data ;
*/

unsigned char data[8];
unsigned long pos;

unsigned long L6470_change(unsigned char *dt, int num){
    int i;
    unsigned long val = 0;
    for(i=0; i<num; i++){
        val = (val << 8) + *dt;
        ++dt;
    }
    return val;
}

void L6470_command(void){
    switch(data[2]){
        case 0x00:
            L6470_Stop(data[7]);
            break;
        case 0x01:
            L6470_Run(data[3],L6470_change(&data[4],3));
            break;
        case 0x02:
            L6470_Move(data[3],L6470_change(&data[4],3));
            break;
        case 0x03:
            L6470_CMD(L6470_GO_HOME);
            break;
        case 0x04:
            L6470_CMD(L6470_RESET_DEVICE);
            break;
        case 0x05:
            while(STOP_SW){
                L6470_Run(Reverse,20000);
            }
            L6470_Stop(HARDSTOP);
            L6470_CMD(L6470_RESET_POS);
            break;
        case 0x06:
            pos = L6470_GetRegister(0x01,3);
            break;
        default:
            break;
            
    }                   
}

unsigned char can_recieve(uCAN_MSG rxMessage){
     //unsigned char buf[12] ;

     // •W€‚ÌŽ¯•ÊŽqID‚©H
     if (rxMessage.frame.idType == dSTANDARD_CAN_MSG_ID_2_0B) {
        // switch(rxMessage.frame.id){
            //case CAN_ID:
         if(rxMessage.frame.id == CAN_ID){
            data[0] = rxMessage.frame.data0 ;
            data[1] = rxMessage.frame.data1 ;
            data[2] = rxMessage.frame.data2 ;
            data[3] = rxMessage.frame.data3 ;
            data[4] = rxMessage.frame.data4 ;
            data[5] = rxMessage.frame.data5 ;
            data[6] = rxMessage.frame.data6 ;
            data[7] = rxMessage.frame.data7 ;
            ServoOut(data[1]);
            L6470_command();
         }
     }
     return data;
}

