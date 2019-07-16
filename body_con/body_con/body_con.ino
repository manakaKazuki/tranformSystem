#include <SPI.h>
#include <mcp_can.h>
#include <MsTimer2.h>

// ピン定義。
#define PIN_SPI_MOSI 11
#define PIN_SPI_MISO 12
#define PIN_SPI_SCK 13
#define PIN_SPI_SS 10
#define PIN_BUSY 9

#define CAN_CS_PIN  8 // CSを8ピンとする（変更可能）
#define CAN_INT_PIN 7  // INTを7ピンとする（変更可能）

#define SW_PIN 6

#define Forward 1
#define Reverse 0

#define CAN_ID 0x101

//////////////////////////////////////////////////////////

long unsigned int rxId;
unsigned char len = 0;
unsigned char rxBuf[8];

MCP_CAN CAN0(CAN_CS_PIN);

void setup()
{
  delay(1000);
  pinMode(SW_PIN, INPUT);

  pinMode(PIN_SPI_MOSI, OUTPUT);
  pinMode(PIN_SPI_MISO, INPUT);
  pinMode(PIN_SPI_SCK, OUTPUT);
  pinMode(PIN_SPI_SS, OUTPUT);
  pinMode(PIN_BUSY, INPUT);
  SPI.begin();
  SPI.setDataMode(SPI_MODE3);
  SPI.setBitOrder(MSBFIRST);
  Serial.begin(115200);
  digitalWrite(PIN_SPI_SS, HIGH);
 
  L6480_resetdevice(); //L6480リセット
  L6480_setup();  //L6480を設定 
  Serial.println("L6480 setup completed!");
  

  CAN0.begin(CAN_1000KBPS); // CANの通信速度を1Mbpsにする 
  pinMode(CAN_INT_PIN, INPUT); // 割り込みのためのピンを設定
  Serial.println("CAN setup 1Mbps completed!");

  MsTimer2::set(50, flush);//シリアルモニター用のタイマー割り込み
  MsTimer2::start();

  delay(4000);
  
}

void loop()
{
	if(!digitalRead(CAN_INT_PIN))   // 受信割り込みが発生したら、CANデータをReadする
    {            
      rxId = CAN0.getCanId();                    
      Serial.print("ID: ");
      Serial.print(rxId, HEX);

      if (rxId == CAN_ID){
      	CAN0.readMsgBuf(&len, rxBuf);
      	Serial.print("  Data: ");
      	for(int i = 0; i<len; i++)                
      	{
        	if(rxBuf[i] < 0x10)
        	{
          		Serial.print("0");
        	}
        	Serial.print(rxBuf[i], HEX);
        	Serial.print(" ");
      	}
      }else{
      	Serial.println("ID Miss Much..");
      }

      recieveMode(rxBuf);
      CAN0.sendMsgBuf(0x001, 0, 8, rxBuf);
      Serial.println();
    }
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
 
void flush(){
  Serial.print("0x");
  Serial.print( L6480_getparam_abspos(),HEX);
  Serial.print("  ");
  Serial.print("0x");
  Serial.println( L6480_getparam_speed(),HEX);
}

void recieveMode(unsigned char data[8]){
	switch (data[2]) {
	    case 0x00:
	      switch (data[7]) {
	          case 0:
	            L6480_softstop();
	            break;
	          case 1:
	            L6480_hardstop();
	            break;
	          case 2:
	            L6480_softhiz();
	            break;
	          case 3:
	            L6480_hardhiz();
	            break;
	      }
	      break;
	    case 0x01:
	      L6480_run(data[3],L6480_change(&data[4],3));
	      break;
	    case 0x02:
	      L6480_move(data[3],L6480_change(&data[4],3));
	      break;
	    case 0x03:
	      L6480_gohome();
	      break;
	    case 0x04:
	      L6480_resetdevice();
	      break;
	    case 0x05:
	      while(SW_PIN){
	          L6480_run(Reverse, 300000);
	      }
	      L6480_hardstop();
	      L6480_resetpos();
	      break;
	}
}


unsigned long L6480_change(unsigned char *dt, int num){
	int i;
    unsigned long val = 0;
    for(i=0; i<num; i++){
        val = (val << 8) + *dt;
        ++dt;
    }
    return val;
}