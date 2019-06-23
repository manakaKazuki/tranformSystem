/* 
 * File:   servo.h
 * Author: kazuki
 *
 * Created on 2019/05/04, 13:55
 */

#ifndef SERVO_H
#define	SERVO_H

#ifndef _XTAL_FREQ
 // Unless already defined assume 64MHz system frequency
 // This definition is required to calibrate __delay_us() and __delay_ms()
 #define _XTAL_FREQ 16000000       // g—p‚·‚éPIC“™‚É‚æ‚è“®ìü”g”’l‚ğİ’è‚·‚é
#endif

#define SERVO_PIN PORTCbits.RC1

void ServoOut(int angle);

#endif	/* SERVO_H */

