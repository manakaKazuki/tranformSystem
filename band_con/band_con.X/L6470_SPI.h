/* 
 * File:   L6470_SPI.h
 * Author: kazuki
 *
 * Created on 2019/04/07, 15:26
 */

#ifndef L6470_SPI_H
#define	L6470_SPI_H

#ifndef _XTAL_FREQ
#define _XTAL_FREQ 16000000
#endif

#define PIN_BUSY      PORTBbits.RB1
#define PIN_SPI_SS    PORTBbits.RB0

void L6470_setparam_abspos(long val);
void L6470_setparam_elpos(long val);
void L6470_setparam_mark(long val);
void L6470_setparam_acc(long val);
void L6470_setparam_dec(long val);
void L6470_setparam_maxspeed(long val);
void L6470_setparam_minspeed(long val);
void L6470_setparam_fsspd(long val);
void L6470_setparam_kvalhold(long val);
void L6470_setparam_kvalrun(long val);
void L6470_setparam_kvalacc(long val);
void L6470_setparam_kvaldec(long val);
void L6470_setparam_intspd(long val);
void L6470_setparam_stslp(long val);
void L6470_setparam_fnslpacc(long val);
void L6470_setparam_fnslpdec(long val);
void L6470_setparam_ktherm(long val);
void L6470_setparam_ocdth(long val);
void L6470_setparam_stallth(long val);
void L6470_setparam_stepmood(long val);
void L6470_setparam_alareen(long val);
void L6470_setparam_config(long val);

long L6470_getparam_abspos(void);
long L6470_getparam_elpos(void);
long L6470_getparam_mark(void);
long L6470_getparam_speed(void);
long L6470_getparam_acc(void);
long L6470_getparam_dec(void);
long L6470_getparam_maxspeed(void);
long L6470_getparam_minspeed(void);
long L6470_getparam_fsspd(void);
long L6470_getparam_kvalhold(void);
long L6470_getparam_kvalrun(void);
long L6470_getparam_kvalacc(void);
long L6470_getparam_kvaldec(void);
long L6470_getparam_intspd(void);
long L6470_getparam_stslp(void);
long L6470_getparam_fnslpacc(void);
long L6470_getparam_fnslpdec(void);
long L6470_getparam_ktherm(void);
long L6470_getparam_adcout(void);
long L6470_getparam_ocdth(void);
long L6470_getparam_stallth(void);
long L6470_getparam_stepmood(void);
long L6470_getparam_alareen(void);
long L6470_getparam_config(void);
long L6470_getparam_status(void);

void L6470_run(int dia,long spd);
void L6470_stepclock(int dia);
void L6470_move(int dia,long n_step);
void L6470_goto(long pos);
void L6470_gotodia(int dia,int pos);
void L6470_gountil(int act,int dia,long spd);
void L6470_relesesw(int act,int dia);
void L6470_gohome(void);
void L6470_gomark(void);
void L6470_resetpos(void);
void L6470_resetdevice(void);
void L6470_softstop(void);
void L6470_hardstop(void);
void L6470_softhiz(void);
void L6470_hardhiz(void);
long L6470_getstatus(void);

void L6470_transfer(int add, int bytes, long val);
void L6470_send(unsigned char add_or_val);
void L6470_send_u(unsigned char add_or_val);
void L6470_busydelay(long time);
unsigned char L6470_receive(unsigned char add_or_val);
long L6470_getparam(int add, int bytes);

void delay_ms(long time);


#endif	/* L6470_SPI_H */

