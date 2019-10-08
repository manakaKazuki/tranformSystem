#include <xc.h>
#include "servo.h"

unsigned int T1time;

//サーボモータ用タイマー割り込み
void __interrupt() InterTimer(){
    if(PIR1bits.TMR1IF == 1){
        SERVO_PIN = 0;        
        PIR1bits.TMR1IF = 0;
    }
    if(PIR2bits.TMR3IF == 1){
        SERVO_PIN = 1;                
        TMR3H = (55536 >> 8);   //Timer3を20ms周期に設定
        TMR3L = (55536 & 0x00ff);
        TMR1H = (T1time >> 8);
        TMR1L = (T1time & 0x00ff); 
        PIR1bits.TMR1IF = 0;
        PIR2bits.TMR3IF = 0;
    }
}

//サーボ出力
void ServoOut(int angle){
    long value;
    
    PIR1bits.TMR1IF = 0;
    PIR2bits.TMR3IF = 0;
    
    value = (long)(angle) * 1000 / 180 + 250;
    T1time = 65536 - value;
    
    TMR1H = (T1time >> 8);
    TMR1L = (T1time & 0x00ff);    
    TMR3H = (55536>>8);   //Timer3を20ms周期に設定
    TMR3L = (55536 & 0x00ff);
    
    SERVO_PIN = 1;          
}
