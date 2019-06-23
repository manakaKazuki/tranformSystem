#include <SPI.h>
#include <MsTimer2.h>

// ピン定義。
#define PIN_SPI_MOSI 11
#define PIN_SPI_MISO 12
#define PIN_SPI_SCK 13
#define PIN_SPI_SS 10
#define PIN_BUSY 9

void setup()
{
  delay(1000);
  pinMode(PIN_SPI_MOSI, OUTPUT);
  pinMode(PIN_SPI_MISO, INPUT);
  pinMode(PIN_SPI_SCK, OUTPUT);
  pinMode(PIN_SPI_SS, OUTPUT);
  pinMode(PIN_BUSY, INPUT);
  SPI.begin();
  SPI.setDataMode(SPI_MODE3);
  SPI.setBitOrder(MSBFIRST);
  Serial.begin(9600);
  digitalWrite(PIN_SPI_SS, HIGH);
 
  L6480_resetdevice(); //L6480リセット
  L6480_setup();  //L6480を設定
  
  MsTimer2::set(50, fulash);//シリアルモニター用のタイマー割り込み
  MsTimer2::start();
  delay(4000);
  
  
  L6480_move(1,1600);//指定方向に指定数ステップする 
  L6480_busydelay(5000); //busyフラグがHIGHになってから、指定ミリ秒待つ。
  L6480_run(0,10000);//指定方向に連続回転
  delay(6000);
  L6480_softstop();//回転停止、保持トルクあり
  L6480_busydelay(5000);
  L6480_goto(0x6789);//指定座標に最短でいける回転方向で移動
  L6480_busydelay(5000);
  L6480_run(0,0x4567);
  delay(6000);
  L6480_hardhiz();//回転急停止、保持トルクなし
  
}

void loop(){

}

void L6480_setup(){
L6480_setparam_acc(0x001); //[R, WS] 加速度default 0x08A (12bit) (14.55*val+14.55[step/s^2])
L6480_setparam_dec(0x001); //[R, WS] 減速度default 0x08A (12bit) (14.55*val+14.55[step/s^2])
L6480_setparam_maxspeed(0x006); //[R, WR]最大速度default 0x041 (10bit) (15.25*val+15.25[step/s])
L6480_setparam_minspeed(0x001); //[R, WS]最小速度default 0x000 (1+12bit) (0.238*val[step/s])
L6480_setparam_fsspd(0x3ff); //[R, WR]μステップからフルステップへの切替点速度default 0x027 (10bit) (15.25*val+7.63[step/s])
L6480_setparam_kvalhold(0x60); //[R, WR]停止時励磁電圧default 0x29 (8bit) (Vs[V]*val/256)
L6480_setparam_kvalrun(0x60); //[R, WR]定速回転時励磁電圧default 0x29 (8bit) (Vs[V]*val/256)
L6480_setparam_kvalacc(0x60); //[R, WR]加速時励磁電圧default 0x29 (8bit) (Vs[V]*val/256)
L6480_setparam_kvaldec(0x60); //[R, WR]減速時励磁電圧default 0x29 (8bit) (Vs[V]*val/256)
L6480_setparam_config(0x2E08);

L6480_setparam_stepmood(0x04); //ステップモードdefault 0x07 (1+3+1+3bit)
}
 
void fulash(){
  Serial.print("0x");
  Serial.print( L6480_getparam_abspos(),HEX);
  Serial.print("  ");
  Serial.print("0x");
  Serial.println( L6480_getparam_speed(),HEX);
}
