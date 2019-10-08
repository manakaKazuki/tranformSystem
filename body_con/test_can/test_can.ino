// CAN受信のサンプル
// 
// 受信したCAN信号をシリアルで送信する
 
// ライブラリヘッダのインクルード
#include <mcp_can.h>
#include <SPI.h>
 
//設定値
#define CS_PIN  10 // CSを10ピンとする（変更可能）
#define INT_PIN 2  // INTを9ピンとする（変更可能）
 
long unsigned int rxId;
unsigned char len = 0;
unsigned char rxBuf[8];


MCP_CAN CAN0(CS_PIN);      
 
void setup()
{
  Serial.begin(115200);
  CAN0.begin(CAN_1000KBPS); // CANの通信速度を1Mbpsにする 
  pinMode(INT_PIN, INPUT); // 割り込みのためのピンを設定
  Serial.println("MCP2515 Library Receive Example...");
}
 
void loop()
{
    if(!digitalRead(INT_PIN))   // 受信割り込みが発生したら、CANデータをReadする
    {
      CAN0.readMsgBuf(&len, rxBuf);              
      rxId = CAN0.getCanId();                    
      Serial.print("ID: ");
      Serial.print(rxId, HEX);
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
      Serial.println();
    }
}