/*******************************************************************************
*  skL6470.h - L6470ステッピングモータドライバ関数ライブラリ用ヘッダファイル   *
*                                                                              *
* ============================================================================ *
*  VERSION DATE        BY                    CHANGE/COMMENT                    *
* ---------------------------------------------------------------------------- *
*  1.00    2016-07-07  きむ茶工房(きむしげ)  Create                            *
* ============================================================================ *
*  PIC 18F2xK22                                                                *
*  MPLAB X(v2.15)                                                              *
*  MPLAB(R) XC8 C Compiler Version 1.32                                        *
*******************************************************************************/
#ifndef _L6470_H_
#define _L6470_H_

#ifndef _XTAL_FREQ
 // Unless already defined assume 64MHz system frequency
 // This definition is required to calibrate __delay_us() and __delay_ms()
 #define _XTAL_FREQ 16000000       // 使用するPIC等により動作周波数値を設定する
#endif

#define L6470_CS	PORTBbits.RB0	        // L6470のSPI-CSのピン番号
#define L6470_BUSY	PORTBbits.RB1	// L6470のBUSYのピン番号

// レジスタのアドレス
#define L6470_ACC	0x05	// 加速時の係数(12bit)
#define L6470_DEC	0x06	// 減速時の係数(12bit)
#define L6470_MAX_SPEED	0x07	// 最大回転速度(10bit)
#define L6470_MIN_SPEED	0x08	// 最少回転速度(13bit)
#define L6470_KVAL_HOLD	0x09	// 停止時の電圧(8bit)
#define L6470_KVAL_RUN	0x0A	// 定速回転時の電圧(8bit)
#define L6470_KVAL_ACC	0x0B	// 加速時の電圧(8bit)
#define L6470_KVAL_DEC	0x0C	// 減速時の電圧(8bit)
#define L6470_OCD_TH	0x13	// 過電流の閾値(4bit)
#define L6470_STALL_TH	0x14	// ストール電流の閾値(7bit)
#define L6470_STEP_MODE	0x16	// ステップモードの設定(8bit)
#define L6470_ALARM_EN	0x17	// 各種アラームの有無設定(8bit)
#define L6470_CONFIG	0x18	// コンフィグの設定(16bit)
#define L6470_STATUS	0x19	// 現在のステータス(16bit)

// 定数の定義
#define Forward         1       // モータは正転で回転
#define Reverse         0       // モータは逆転で回転
#define SOFTSTOP        0       // 減速停止、保持トルク有り
#define HARDSTOP        1       // 即時停止、保持トルク有り
#define SOFTHIZ         2       // 減速停止、保持トルク無し
#define HARDHIZ         3       // 即時停止、保持トルク無し
#define FULL_STEP	0	// フルステップ(モータと同じステップ数)
#define HALF_STEP	1	// ハーフステップ(モータと同じステップ数x2)
#define MICROSTEP8	3	// 1/8 マイクロステップ(モータと同じステップ数x8)
#define MICROSTEP64	6	// 1/64  マイクロステップ(モータと同じステップ数x64)
#define MICROSTEP128	7	// 1/128 マイクロステップ(モータと同じステップ数x128)
#define OSC_INT16MHZ    0       // クロックは内蔵16MHz


// レジスタの構造体定義
// STEP_MODE Register
union STEP_MODE_t {           // ステッピングモードレジスタ(デフォルト=0x07)
     unsigned char DT ;
     struct {
          unsigned STEP_SEL   :3 ;      // ステッピングモードの設定
          unsigned SP         :1 ;      // 予備
          unsigned SYNC_SEL   :3 ;      // 同期周波数の選択
          unsigned SYNC_EN    :1 ;      // BUSY/SYNCの選択
     } ;
} ;
// CONFIG Register
union CONFIG_t {              // コンフィグレジスタ(デフォルト=0x2E88)
     unsigned int DT ;
     struct {
          unsigned OSC_SEL    :3 ;      // クロックの選択
          unsigned EXT_CLK    :1 ;      // クロック出力の有無
          unsigned SW_MODE    :1 ;      // SW機能の選択
          unsigned EN_VSCPMP  :1 ;      // モータの電源電圧補償の有無
          unsigned SP         :1 ;      // 予備
          unsigned OC_SD      :1 ;      // 過電流発生時のブリッジ回路停止の有無
          unsigned POW_SR     :2 ;      // モータ出力のスルーレート値の設定
          unsigned F_PWM_DEC  :3 ;      // PWM周波数生成の整数分周比を設定
          unsigned F_PWM_INT  :3 ;      // PWM周波数生成倍率を設定
     } ;
} ;


// 関数のプロトタイプ宣言
unsigned long L6470_Init() ;
unsigned long L6470_GetRegister(char regadrs,int num) ;
int  L6470_ALARM() ;
void L6470_Run(char dir,unsigned long speed) ;
void L6470_Move(char dir,unsigned long step) ;
void L6470_Stop(int mode) ;

#endif
