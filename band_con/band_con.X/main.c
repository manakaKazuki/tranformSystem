/*
 * File:   main.c
 * Author: kazuki
 *
 * Created on 2019/04/21, 23:57
 */

//git check1

// PIC18F26K80 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1L
#pragma config RETEN = OFF      // VREG Sleep Enable bit (Ultra low-power regulator is Disabled (Controlled by REGSLP bit))
#pragma config INTOSCSEL = HIGH // LF-INTOSC Low-power Enable bit (LF-INTOSC in High-power mode during Sleep)
#pragma config SOSCSEL = DIG   // SOSC Power Selection and mode Configuration bits (High Power SOSC circuit selected)
#pragma config XINST = OFF       // Extended Instruction Set (Enabled)

// CONFIG1H
#pragma config FOSC = INTIO2    // Oscillator (Internal RC oscillator)
#pragma config PLLCFG = OFF     // PLL x4 Enable bit (Disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor (Disabled)
#pragma config IESO = OFF       // Internal External Oscillator Switch Over Mode (Disabled)

// CONFIG2L
#pragma config PWRTEN = OFF     // Power Up Timer (Disabled)
#pragma config BOREN = SBORDIS  // Brown Out Detect (Enabled in hardware, SBOREN disabled)
#pragma config BORV = 3         // Brown-out Reset Voltage bits (1.8V)
#pragma config BORPWR = ZPBORMV // BORMV Power level (ZPBORMV instead of BORMV is selected)

// CONFIG2H
#pragma config WDTEN = SWDTDIS  // Watchdog Timer (WDT enabled in hardware; SWDTEN bit disabled)
#pragma config WDTPS = 1048576  // Watchdog Postscaler (1:1048576)

// CONFIG3H
#pragma config CANMX = PORTB    // ECAN Mux bit (ECAN TX and RX pins are located on RB2 and RB3, respectively)
#pragma config MSSPMSK = MSK7   // MSSP address masking (7 Bit address masking mode)
#pragma config MCLRE = ON       // Master Clear Enable (MCLR Enabled, RE3 Disabled)

// CONFIG4L
#pragma config STVREN = ON      // Stack Overflow Reset (Enabled)
#pragma config BBSIZ = BB2K     // Boot Block Size (2K word Boot Block size)

// CONFIG5L
#pragma config CP0 = OFF        // Code Protect 00800-03FFF (Disabled)
#pragma config CP1 = OFF        // Code Protect 04000-07FFF (Disabled)
#pragma config CP2 = OFF        // Code Protect 08000-0BFFF (Disabled)
#pragma config CP3 = OFF        // Code Protect 0C000-0FFFF (Disabled)

// CONFIG5H
#pragma config CPB = OFF        // Code Protect Boot (Disabled)
#pragma config CPD = OFF        // Data EE Read Protect (Disabled)

// CONFIG6L
#pragma config WRT0 = OFF       // Table Write Protect 00800-03FFF (Disabled)
#pragma config WRT1 = OFF       // Table Write Protect 04000-07FFF (Disabled)
#pragma config WRT2 = OFF       // Table Write Protect 08000-0BFFF (Disabled)
#pragma config WRT3 = OFF       // Table Write Protect 0C000-0FFFF (Disabled)

// CONFIG6H
#pragma config WRTC = OFF       // Config. Write Protect (Disabled)
#pragma config WRTB = OFF       // Table Write Protect Boot (Disabled)
#pragma config WRTD = OFF       // Data EE Write Protect (Disabled)

// CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protect 00800-03FFF (Disabled)
#pragma config EBTR1 = OFF      // Table Read Protect 04000-07FFF (Disabled)
#pragma config EBTR2 = OFF      // Table Read Protect 08000-0BFFF (Disabled)
#pragma config EBTR3 = OFF      // Table Read Protect 0C000-0FFFF (Disabled)

// CONFIG7H
#pragma config EBTRB = OFF      // Table Read Protect Boot (Disabled)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mcc_generated_files/ecan.h"
#include <math.h>
#include "SPIlib.h"
#include "L6470.h"
#include "can_command.h"
#include "servo.h"

#define _XTAL_FREQ 16000000

#define LED_PIN PORTCbits.RC2

#define ARM_SW_UP PORTAbits.RA7
#define ARM_SW_MODE PORTAbits.RA6
#define ARM_SW_DOWN PORTCbits.RC0


//  �v���g�^�C�v�̐錾
void Wait(unsigned int num) ;

// ���ʕϐ��̐錾
uCAN_MSG txMessage ;               // ���M���b�Z�[�W�o�b�t�@
uCAN_MSG rxMessage ;               // ��M���b�Z�[�W�o�b�t�@
int *data;
unsigned char servo_Flag = 0;
int servo_Angle[2] ={60,120};
int arm_mode_flag = 0;

void main(void) {
    
    unsigned long ans ;
    
    OSCCON  = 0b01110010; // �����N���b�N�Ƃ���(16MHz)
    INTCON2bits.RBPU = 0;
    WPUB    = 0b00100010; // RB1�v���A�b�v��RON
    ANCON0  = 0b00000000; // AN0-4�A�i���O�͎g�p���Ȃ��A�f�W�^��I/O�Ɋ���
    ANCON1  = 0b00000000; // AN8-10�A�i���O�͎g�p���Ȃ��A�f�W�^��I/O�Ɋ���
    TRISA   = 0b11000000; // RA0-5�S�ďo��, 6,7���͐ݒ�
    TRISB   = 0b00101010; // RB3(CANRX),RB1(BUSY�p)�͓��́A���̑��͏o�͂ɐݒ�
    TRISC   = 0b00010001; // RC4(SDI)�͓��́ARC0�͓��͂��̑��͏o�͂ɐݒ�
    PORTA   = 0b00000000; // �o��A�s���̏�����(�S��LOW�ɂ���)
    PORTB   = 0b00000001; // �o��B�s���̏�����(�S��LOW�ɂ���)
    PORTC   = 0b00000000; // �o��C�s���̏�����(�S��LOW�ɂ���)
    
    T1CON   = 0b00110000;   //FOSC, �v���X�P�[��1:8
    T3CON   = 0b00110000;   //FOSC, �v���X�P�[��1:8
    PIR1bits.TMR1IF = 0;
    PIE1bits.TMR1IE = 1;
    PIR2bits.TMR3IF = 0;
    PIE2bits.TMR3IE = 1;
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1;
    
    //�T�[�{���[�^������
    ServoOut(servo_Angle[servo_Flag]);
    T1CONbits.TMR1ON = 1;
    T3CONbits.TMR3ON = 1; 
    
    // ECAN�̏���������
    // �ʐM���x:500Kbps  �W�����ʎqID:0x123����t����
    ECAN_Initialize() ;
    
    // SPI(MSSP1)�̏��������s���A�ʐM���x500KHz
    SPI_Init(SPI_MODE3,SPI_CLOCK_DIV16,0) ;
    // �X�e�b�s���O���[�^�h���C�o�̏��������s��
    ans = L6470_Init() ;   
    
    Wait(300) ;              // 3�b��ɊJ�n
    
    while(STOP_SW){
        L6470_Run(Reverse,30000);
    }
    L6470_Stop(HARDSTOP);
    L6470_CMD(L6470_RESET_POS);
    
    
    LED_PIN = 1;
    Wait(100);
    LED_PIN = 0;
    
    while(1) {
        if(ARM_SW_MODE == 0){
            if(arm_mode_flag == 1){
                LED_PIN = 1;
                if(servo_Flag == 1){
                    servo_Flag = 0;
                }else{
                    servo_Flag = 1;
                }
                ServoOut(servo_Angle[servo_Flag]);
                arm_mode_flag = 0;
            }
            Wait(1);
        }else{
            LED_PIN = 0;
            arm_mode_flag = 1;
            Wait(1);
        }
        
        if(ARM_SW_UP == 0){           
            L6470_Run(Forward,30000);
        }else if(ARM_SW_DOWN == 0){
            if(STOP_SW){
                L6470_Run(Reverse,30000);                
            }else{
                L6470_Stop(HARDSTOP);
            }
        }else{
            L6470_Stop(HARDSTOP);
        }
        
        if (CAN_receive(&rxMessage)) {
            // ��M�����珈�����s��
            LED_PIN = 1;
            data = can_recieve(rxMessage);
            txMessage.frame.idType = dSTANDARD_CAN_MSG_ID_2_0B ;
            txMessage.frame.id     = 0x001 ;
            txMessage.frame.dlc    = 8 ;  // �����[�g�t���[�����M�v��
            txMessage.frame.data0  = data[0];
            txMessage.frame.data1  = data[1];
            txMessage.frame.data2  = data[2];
            txMessage.frame.data3  = data[3];
            txMessage.frame.data4  = data[4];
            txMessage.frame.data5  = data[5];
            txMessage.frame.data6  = data[6];
            txMessage.frame.data7  = data[7];
            //CAN_transmit(&txMessage) ;
            Wait(50);
        }
    }
    
}

// �w�肵������(num x 10ms)�����E�G�C�g���s�������֐�
void Wait(unsigned int num)
{
     int i ;

     // num�Ŏw�肵���񐔂����J��Ԃ�
     for (i=0 ; i<num ; i++) {
          __delay_ms(10) ;         // 10ms�v���O�����̈ꎞ��~
     }
}