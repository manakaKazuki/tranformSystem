#include <xc.h>
#include "can_command.h"
#include "mcc_generated_files/ecan.h"
#include "L6470.h"
#include "L6470_SPI.h"
#include "servo.h"

union {
    unsigned char c[8] ;
    unsigned int  i;
} data ;

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
    switch(data.c[1]){
        case 0x00:
            L6470_Stop(data.c[2]);
        case 0x50:
            L6470_Run(data.c[2],L6470_change(&data.c[3],3));
            break;
        case 0x40:
            L6470_Move(data.c[2],L6470_change(&data.c[3],3));
            break;
        default:
            break;
            
    }                   
}

void can_recieve(uCAN_MSG rxMessage){
     unsigned char buf[12] ;

     // •W€‚ÌŽ¯•ÊŽqID‚©H
     if (rxMessage.frame.idType == dSTANDARD_CAN_MSG_ID_2_0B) {
          switch (rxMessage.frame.id) {
            case CAN_ID :
                data.c[0] = rxMessage.frame.data0 ;
                data.c[1] = rxMessage.frame.data1 ;
                data.c[2] = rxMessage.frame.data2 ;
                data.c[3] = rxMessage.frame.data3 ;
                data.c[4] = rxMessage.frame.data4 ;
                data.c[5] = rxMessage.frame.data5 ;
                data.c[6] = rxMessage.frame.data6 ;
                data.c[7] = rxMessage.frame.data7 ;
                switch(data.c[0]){
                    case 0x01:
                        ServoOut(data.c[1]);
                        break;
                    case 0x10:                        
                        L6470_command();                        
                    default:
                        break;
                }
                break ;
            default :
                break ;
          }
     }
}

