/* 
 * File:   can_command.h
 * Author: kazuki
 *
 * Created on 2019/05/04, 13:41
 */

#ifndef CAN_COMMAND_H
#define	CAN_COMMAND_H

#include "mcc_generated_files/ecan.h"

#ifndef _XTAL_FREQ
 // Unless already defined assume 64MHz system frequency
 // This definition is required to calibrate __delay_us() and __delay_ms()
 #define _XTAL_FREQ 16000000       // �g�p����PIC���ɂ�蓮����g���l��ݒ肷��
#endif

#define CAN_ID 0x123


void can_recieve(uCAN_MSG rxMessage);

#endif	/* CAN_COMMAND_H */

