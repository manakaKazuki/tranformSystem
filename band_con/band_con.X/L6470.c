/*******************************************************************************
*  skL6470.c - L6470�X�e�b�s���O���[�^�h���C�o�֐����C�u����                   *
*                                                                              *
*    L6470_Init  - L6470�̏��������s������                                     *
*    L6470_GetRegister - ���W�X�^�̃f�[�^��ǂݍ��ޏ���                        *
*    L6470_ALARM - �A���[���̏���ǂݍ��ޏ���                                *
*    L6470_Run   - ���[�^����葬�x�ŉ�]�����鏈��                            *
*    L6470_Move  - ���[�^���w��X�e�b�v���܂ŉ�]�����鏈��                    *
*    L6470_Stop  - ���[�^���~�����鏈��                                      *
*                                                                              *
* ============================================================================ *
*  VERSION DATE        BY                    CHANGE/COMMENT                    *
* ---------------------------------------------------------------------------- *
*  1.00    2016-07-07  ���ޒ��H�[(���ނ���)  Create                            *
* ============================================================================ *
*  PIC 18F2xK22 (���̃��C�u�������̂͑��̂o�h�b�ł��n�j�Ǝv���܂�)             *
*  MPLAB X(v2.15)                                                              *
*  MPLAB(R) XC8 C Compiler Version 1.32                                        *
*******************************************************************************/
#include <xc.h>
#include "SPIlib.h"
#include "L6470.h"

int Busy ;

// L6470�ɃR�}���h�𑗐M���鏈��
void CMD_Send(char cmd,unsigned long val,int num)
{
     unsigned char dt[4] ;
     int i ;

     for (i=0 ; i<num ; i++) {
          dt[i] = val & 0xff ;
          val = val >> 8 ;
     }

     L6470_CS = 0 ;
     SPI_transfer(cmd) ;                // �R�}���h�̑��M
     L6470_CS = 1 ;
     switch (num) {
        case 3:
               L6470_CS = 0 ;
               SPI_transfer(dt[2]) ;    // �p�����[�^�Q�̑��M
               L6470_CS = 1 ;
        case 2:
               L6470_CS = 0 ;
               SPI_transfer(dt[1]) ;    // �p�����[�^�P�̑��M
               L6470_CS = 1 ;
        case 1:
               L6470_CS = 0 ;
               SPI_transfer(dt[0]) ;    // �p�����[�^�O�̑��M
               L6470_CS = 1 ;
               break ;
     }
}
/*******************************************************************************
*  ans = L6470_Init()                                                          *
*    L6470�̏��������s������                                                   *
*                                                                              *
*    ans : ���݂�STATUS����Ԃ�                                               *
*******************************************************************************/
unsigned long L6470_Init()
{
     union STEP_MODE_t step_mode ;
     union CONFIG_t config ;
     long  val ;
     int i ;

     // L6470�Ƀ��Z�b�g���|����
     for (i=0 ; i<4 ; i++) {
          L6470_CS = 0 ;
          SPI_transfer(0x00) ;     // NOP�̑��M
          L6470_CS = 1 ;
     }
     L6470_CS = 0 ;
     SPI_transfer(0xC0) ;          // ResetDevice�R�}���h�̑��M
     L6470_CS = 1 ;
     // �e�탌�W�X�^�ɐݒ���s��(SetParam)
     CMD_Send(L6470_ACC,0x008A,2) ;          //   �������̌W��(2022.45 Step/s^2)
     CMD_Send(L6470_DEC,0x008A,2) ;          //   �������̌W��(2022.45 Step/s^2)
     CMD_Send(L6470_MAX_SPEED,0x0020,2) ;    // �ő��]���x(503.25 Step/s^2)
     CMD_Send(L6470_MIN_SPEED,0x0001,2) ;    //   �ŏ���]���x(0 Step/s^2 �ᑬ�̍œK���@�\����)
     CMD_Send(L6470_KVAL_HOLD,0xFF,1) ;      // ��~���̓d��(�ő� 0.996xVs)
     CMD_Send(L6470_KVAL_RUN,0xF0,1) ;       //   �葬���̓d��(�ő� 0.996xVs)
     CMD_Send(L6470_KVAL_ACC,0xF0,1) ;       //   �������̓d��(�ő� 0.996xVs)
     CMD_Send(L6470_KVAL_DEC,0xF0,1) ;       //   �������̓d��(�ő� 0.996xVs)
     CMD_Send(L6470_OCD_TH,0x0F,1) ;         // �ߓd����臒l(3A)
     CMD_Send(L6470_STALL_TH,0x7F,1) ;       // �X�g�[���d����臒l(2A)
      // �X�e�b�v���[�h�̐ݒ�
     step_mode.DT = 0x07 ;                   // �f�t�H���g�l
     step_mode.STEP_SEL = FULL_STEP ;        // �t���X�e�b�v�ɐݒ�
     step_mode.SYNC_EN  = Busy = 0 ;         // BUSY�o�̓��[�h
     val = step_mode.DT ;
     CMD_Send(L6470_STEP_MODE,val,1) ;
     // �R���t�B�O�̐ݒ�
     config.DT = 0x2F88 ;                    // �f�t�H���g�l(PWM���g��:15.6KHz)
     config.OSC_SEL = OSC_INT16MHZ ;         // �N���b�N�͓���16MHz
     config.EXT_CLK = 0 ;                    // �N���b�N�o��(OSCOUT)�͂Ȃ�
     config.SW_MODE = 1 ;                    // �n�[�h��~���[�h
     val = config.DT ;
     CMD_Send(L6470_CONFIG,val,2) ;          // �R���t�B�O�̐ݒ�
     return L6470_GetRegister(L6470_STATUS,2) ;// ���݂̃X�e�[�^�X��Ԃ��B
}
/*******************************************************************************
*  ans = L6470_GetRegister(regadrs,num)                                        *
*    ���W�X�^�̃f�[�^��ǂݍ��ޏ���                                            *
*                                                                              *
*    regadrs : �ǂݎ�肽�����W�X�^�̃A�h���X���w��(0x01-0x1B)                 *
*    num     : ���W�X�^�̓ǂݎ��f�[�^�̃o�C�g��(1-3byte)                     *
*    ans     : �ǂݎ�����f�[�^��long�l�ŕԂ�                                  *
*******************************************************************************/
unsigned long L6470_GetRegister(char regadrs,int num)
{
     unsigned long ans , dt ;
     int i ;

     L6470_CS = 0 ;
     if (regadrs == L6470_STATUS) SPI_transfer(0xD0) ; // GetStatus�R�}���h�̑��M
     else SPI_transfer(regadrs|0x20) ;                 // GetParam�R�}���h�̑��M
     L6470_CS = 1 ;
     ans = dt = 0 ;
     for (i=0 ; i<num ; i++) {
          ans = ans << 8 ;
          L6470_CS = 0 ;
          dt = SPI_transfer(0x00) ;     // ���X�|���f�[�^�𓾂�ׂɃ_�~�[�̑��M
          L6470_CS = 1 ;
          ans = ans | dt ;
     }
     return ans ;
}
/*******************************************************************************
*  ans = L6470_ALARM()                                                         *
*    �A���[���̏���ǂݍ��ޏ���                                              *
*    FLAG�s����LOW�ɂăA���[�������ł��A���荞�݂Ŏ󂯂܂��傤�B               *
*    ���̊֐��́A�hGetStatus�h�R�}���h�𔭍s���Ă���̂ŃA���[���͉�������܂� *
*                                                                              *
*    ans�F���������A���[������Ԃ�                                           *
*         0=�A���[���͔������Ă��Ȃ�                                           *
*         3=�X�C�b�`�C�x���g�����o                                             *
*         7=�R�}���h�����s�ł��Ȃ�                                             *
*         8=�R�}���h���S�����݂��Ȃ�                                           *
*         9=��d�������o                                                       *
*        10=�M�x�������o                                                       *
*        11=�T�[�}���V���b�g�_�E�������o                                       *
*        12=�ߓd�������o                                                       *
*        13=�`���X�g�[�������o                                                 *
*        14=�a���X�g�[�������o                                                 *
*******************************************************************************/
int L6470_ALARM()
{
     unsigned long dt ;
     int i ;

     dt = L6470_GetRegister(L6470_STATUS,2) ;
     for (i=14;i>=9;i--) {
          if ((dt && (1<<i)) == 0) return i ;
     }
     for (i=8;i>=7;i--) {
          if ((dt && (1<<i)) != 0) return i ;
     }
     if ((dt && (1<<3)) != 0) return 3 ;
     return 0 ;
}
/*******************************************************************************
*  L6470_Run(dir,speed)                                                        *
*    ���[�^����葬�x�ŉ�]�����鏈��                                          *
*    �I������Ă���X�e�b�s���O���[�h�ɂ���]���܂�                          *
*                                                                              *
*    dir   : ��]������������w�����܂�(0=�t�]  1=���])                        *
*    speed : ��]������ڕW���x���w�����܂�                                    *
*            0.015step/s�̕���\��0(0.015)-104166(15625)�̃X�e�b�v�ł��B       *
*            MAX_SPEED���Ⴍ�AMIN_SPEED���傫���Ȃ���΂Ȃ�܂���B        *
*******************************************************************************/
void L6470_Run(char dir,unsigned long speed)
{
     while( (Busy==0) && (L6470_BUSY==0) ) ; // �R�}���h���s���Ȃ�҂�
     CMD_Send(0x50|dir,speed,3) ;            // ��葬�x��]�R�}���h�̑��M
}
/*******************************************************************************
*  L6470_Move(dir,step)                                                        *
*    ���[�^���w��X�e�b�v���܂ŉ�]�����鏈��                                  *
*    �I������Ă���X�e�b�s���O���[�h�ɂ��X�e�b�v����̉�]�p�x�͕ς��܂��B*
*    �Ⴆ�΁Astep=200�Ńt���X�e�b�v���[�h�Ȃ�P��](360�x)�ł��B               *
*                                                                              *
*    dir   : ��]������������w�����܂�(0=�t�]  1=���])                        *
*    speed : ��]������ڕW�X�e�b�v�����w�����܂�(0-4194303:22bit)             *
*******************************************************************************/
void L6470_Move(char dir,unsigned long step)
{
    PORTCbits.RC2 = 1;
     while( (Busy==0) && (L6470_BUSY==0) ) ; // �R�}���h���s���Ȃ�҂�
     CMD_Send(0x40|dir,step,3) ;             // �ړ��R�}���h�̑��M
}
/*******************************************************************************
*  L6470_Stop(mode)                                                            *
*    ���[�^���~�����鏈��                                                    *
*    Hiz�̓��[�^���~��������A���[�^�̏o�͂�OFF(�n�C�C���s�[�_���X)�ɂ��܂��B*
*                                                                              *
*    mode : ��~���[�h���w�肵�܂�                                             *
*           0 = SoftStop �������Ȃ��烂�[�^���~�����܂��A�ێ��g���N�L��B    *
*           1 = HardStop �����Ƀ��[�^���~�����܂��A�ێ��g���N�L��B          *
*           2 = SoftHiz  �������Ȃ��烂�[�^���~�����܂��A�ێ��g���N�����B    *
*           3 = HardHiz  �����Ƀ��[�^���~�����܂��A�ێ��g���N�����B          *
*******************************************************************************/
void L6470_Stop(int mode)
{
     unsigned char cmd[4] = {0xB0,0xB8,0xA0,0xA8} ;

     while( (Busy==0) && (L6470_BUSY==0) ) ; // �R�}���h���s���Ȃ�҂�
     L6470_CS = 0 ;
     SPI_transfer(cmd[mode]) ;               // �X�g�b�v�R�}���h�̑��M
     L6470_CS = 1 ;
}

