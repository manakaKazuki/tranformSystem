#include <pic18f26k80.h>
#include <xc.h>
#include "L6470_SPI.h"

void L6470_setparam_abspos(long val){L6470_transfer(0x01,3,val);}
void L6470_setparam_elpos(long val){L6470_transfer(0x02,2,val);}
void L6470_setparam_mark(long val){L6470_transfer(0x03,3,val);}
void L6470_setparam_acc(long val){L6470_transfer(0x05,2,val);}
void L6470_setparam_dec(long val){L6470_transfer(0x06,2,val);}
void L6470_setparam_maxspeed(long val){L6470_transfer(0x07,2,val);}
void L6470_setparam_minspeed(long val){L6470_transfer(0x08,2,val);}
void L6470_setparam_fsspd(long val){L6470_transfer(0x15,2,val);}
void L6470_setparam_kvalhold(long val){L6470_transfer(0x09,1,val);}
void L6470_setparam_kvalrun(long val){L6470_transfer(0x0a,1,val);}
void L6470_setparam_kvalacc(long val){L6470_transfer(0x0b,1,val);}
void L6470_setparam_kvaldec(long val){L6470_transfer(0x0c,1,val);}
void L6470_setparam_intspd(long val){L6470_transfer(0x0d,2,val);}
void L6470_setparam_stslp(long val){L6470_transfer(0x0e,1,val);}
void L6470_setparam_fnslpacc(long val){L6470_transfer(0x0f,1,val);}
void L6470_setparam_fnslpdec(long val){L6470_transfer(0x10,1,val);}
void L6470_setparam_ktherm(long val){L6470_transfer(0x11,1,val);}
void L6470_setparam_ocdth(long val){L6470_transfer(0x13,1,val);}
void L6470_setparam_stallth(long val){L6470_transfer(0x14,1,val);}
void L6470_setparam_stepmood(long val){L6470_transfer(0x16,1,val);}
void L6470_setparam_alareen(long val){L6470_transfer(0x17,1,val);}
void L6470_setparam_config(long val){L6470_transfer(0x18,2,val);}

long L6470_getparam_abspos(void){return L6470_getparam(0x01,3);}
long L6470_getparam_elpos(void){return L6470_getparam(0x02,2);}
long L6470_getparam_mark(void){return L6470_getparam(0x03,3);}
long L6470_getparam_speed(void){return L6470_getparam(0x04,3);}
long L6470_getparam_acc(void){return L6470_getparam(0x05,2);}
long L6470_getparam_dec(void){return L6470_getparam(0x06,2);}
long L6470_getparam_maxspeed(void){return L6470_getparam(0x07,2);}
long L6470_getparam_minspeed(void){return L6470_getparam(0x08,2);}
long L6470_getparam_fsspd(void){return L6470_getparam(0x15,2);}
long L6470_getparam_kvalhold(void){return L6470_getparam(0x09,1);}
long L6470_getparam_kvalrun(void){return L6470_getparam(0x0a,1);}
long L6470_getparam_kvalacc(void){return L6470_getparam(0x0b,1);}
long L6470_getparam_kvaldec(void){return L6470_getparam(0x0c,1);}
long L6470_getparam_intspd(void){return L6470_getparam(0x0d,2);}
long L6470_getparam_stslp(void){return L6470_getparam(0x0e,1);}
long L6470_getparam_fnslpacc(void){return L6470_getparam(0x0f,1);}
long L6470_getparam_fnslpdec(void){return L6470_getparam(0x10,1);}
long L6470_getparam_ktherm(void){return L6470_getparam(0x11,1);}
long L6470_getparam_adcout(void){return L6470_getparam(0x12,1);}
long L6470_getparam_ocdth(void){return L6470_getparam(0x13,1);}
long L6470_getparam_stallth(void){return L6470_getparam(0x14,1);}
long L6470_getparam_stepmood(void){return L6470_getparam(0x16,1);}
long L6470_getparam_alareen(void){return L6470_getparam(0x17,1);}
long L6470_getparam_config(void){return L6470_getparam(0x18,2);}
long L6470_getparam_status(void){return L6470_getparam(0x19,2);}

void L6470_run(int dia,long spd){
  if(dia==1)
    L6470_transfer(0x51,3,spd);
  else
    L6470_transfer(0x50,3,spd);
}
void L6470_stepclock(int dia){
  if(dia==1)
    L6470_transfer(0x59,0,0);    
  else
    L6470_transfer(0x58,0,0);
}
void L6470_move(int dia,long n_step){
  if(dia==1)
    L6470_transfer(0x41,3,n_step);
  else
    L6470_transfer(0x40,3,n_step);
}
void L6470_goto(long pos){
  L6470_transfer(0x60,3,pos);
}
void L6470_gotodia(int dia,int pos){
  if(dia==1)    
    L6470_transfer(0x69,3,pos);
  else    
    L6470_transfer(0x68,3,pos);
}
void L6470_gountil(int act,int dia,long spd){
  if(act==1)
    if(dia==1)
      L6470_transfer(0x8b,3,spd);
    else
      L6470_transfer(0x8a,3,spd);
  else
    if(dia==1)
      L6470_transfer(0x83,3,spd);
    else
      L6470_transfer(0x82,3,spd);
}  
void L6470_relesesw(int act,int dia){
  if(act==1)
    if(dia==1)
      L6470_transfer(0x9b,0,0);
    else
      L6470_transfer(0x9a,0,0);
  else
    if(dia==1)
      L6470_transfer(0x93,0,0);
    else
      L6470_transfer(0x92,0,0);
}
void L6470_gohome(void){
  L6470_transfer(0x70,0,0);
}
void L6470_gomark(void){
  L6470_transfer(0x78,0,0);
}
void L6470_resetpos(void){
  L6470_transfer(0xd8,0,0);
}
void L6470_resetdevice(void){
  L6470_send_u(0x00);//nop����
  L6470_send_u(0x00);
  L6470_send_u(0x00);
  L6470_send_u(0x00);
  L6470_send_u(0xc0);
}
void L6470_softstop(void){
  L6470_transfer(0xb0,0,0);
}
void L6470_hardstop(void){
  L6470_transfer(0xb8,0,0);
}
void L6470_softhiz(void){
  L6470_transfer(0xa0,0,0);
}
void L6470_hardhiz(void){
  L6470_transfer(0xa8,0,0);
}
long L6470_getstatus(void){
  long val=0;
  L6470_send_u(0xd0);
  for(int i=0;i<=1;i++){
    val = val << 8;
    PIN_SPI_SS = 0;
    val = val | L6470_receive(0x00); // �A�h���X�������̓f�[�^���M�B
    PIN_SPI_SS = 1;
  }
  return val;
}

void L6470_transfer(int add, int bytes, long val){
    int data[3];
    L6470_send(add);
    for(int i=0;i<=bytes-1;i++){
        data[i] = val & 0xff;
        val = val >> 8;
    }
    if(bytes == 3){
        L6470_send(data[2]);
    }
    if(bytes >= 2){
        L6470_send(data[1]);
    }
    if(bytes >= 1){
        L6470_send(data[0]);
    }
}

void L6470_send(unsigned char add_or_val){
    while(!PIN_BUSY);
    PIN_SPI_SS = 0;
    SSPBUF = add_or_val;
    PIN_SPI_SS = 1;
}

void L6470_send_u(unsigned char add_or_val){
    PIN_SPI_SS = 0;
    SSPBUF = add_or_val;
    PIN_SPI_SS = 1;
}

void L6470_busydelay(long time){
    while(!PIN_BUSY);
    delay_ms(time);
}

unsigned char L6470_receive(unsigned char add_or_val){
    unsigned char data;
    SSPBUF = add_or_val;
    while(!SSPSTATbits.BF);
    data = SSPBUF;
    return data;
}

long L6470_getparam(int add, int bytes){
    long val = 0;
    int send_add = add | 0x20;
    L6470_send_u(send_add);
    for(int i=0;i<=bytes-1;i++){
        val = val << 8;
        PIN_SPI_SS = 0;
        val = val | L6470_receive(0x00);
        PIN_SPI_SS = 1;
    }
    return val;
}

void delay_ms(long time){
    for(int i=0; i<time;i++){
        __delay_ms(1);
    }
}