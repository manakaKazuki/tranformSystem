/*******************************************************************************
*  skSPIlib.c - �r�o�h�ʐM���s���֐����C�u����                                 *
*                                                                              *
*    SPI_Init            - �r�o�h���[�h(�}�X�^�[)�̐ݒ�Ə��������s������      *
*    SPI_setDataMode     - �r�o�h�̓]�����[�h�ݒ���s������                    *
*    SPI_setClockDivider - �r�o�h�̒ʐM���x�ݒ���s������                      *
*    SPI_transfer        - �r�o�h�ʐM�ł̃f�[�^���M�ƃf�[�^��M���s������      *
*                                                                              *
*    �����FSDI�s���͕K���u�f�W�^�����̓s���v�ɐݒ���s���ĉ������B             *
*          �g�p����SS(CS)�s���͏���������HIGH�ɂ��܂��B                        *
*          SSP2�𗘗p����ꍇ�́AskI2Clib.h��"#define SPI_MSSP2_USE"��錾���� *
* ============================================================================ *

/*******************************************************************************
*  SPI_Init(mode,divider,sdo)                                                  *
*    �r�o�h���[�h�̐ݒ�Ə��������s������                                      *
*    ���荞�݂ł̏������~�߂Ē���"SSP1IF"������l�ɂ��Ă��܂�                  *
*                                                                              *
*    mode :   SPI�̓]�����[�h��ݒ肵�܂�(�N���b�N�ɐ��ƃN���b�N�ʑ��̑g����)  *
*             SPI_MODE1 = �ɐ�(0:LOW)  �ʑ�(0:�A�C�h��0V�ŁA0V->5V�ɕω��œ]��)*
*             SPI_MODE0 = �ɐ�(0:LOW)  �ʑ�(1:�A�C�h��0V�ŁA5V->0V�ɕω��œ]��)*
*             SPI_MODE3 = �ɐ�(1:HIGH) �ʑ�(0:�A�C�h��5V�ŁA5V->0V�ɕω��œ]��)*
*             SPI_MODE2 = �ɐ�(1:HIGH) �ʑ�(1:�A�C�h��5V�ŁA0V->5V�ɕω��œ]��)*
*    divider :SPI�̒ʐM���x��ݒ肵�܂�                                        *
*             SPI_CLOCK_DIV4   = Fosc/4                                        *
*             SPI_CLOCK_DIV16  = Fosc/16                                       *
*             SPI_CLOCK_DIV64  = Fosc/64                                       *
*             SPI_CLOCK_DIVT2  = TMR2�̏o�͂�1/2                               *
*             SPI_CLOCK_DIVADD = FOSC/((SSPxADD + 1)*4)                        *
*    sdo :    �g�p����SDO���M�̃s���ԍ����w�肷��                              *
*******************************************************************************/

#include <xc.h>
#include "SPIlib.h"

void SPI_Init(char mode,char divider,int sdo)
{
     char con , stat ;

#if defined(_12F1822)
     SDOSEL = 0 ;                  // 7�ԃs��(RA0)��SDO���M�s���Ƃ���
     if (sdo == 3) SDOSEL = 1 ;    // 3�ԃs��(RA4)��SDO���M�s���Ƃ���
#endif

#if defined(_16F1823) 
     SDOSEL = 0 ;                  // 8�ԃs��(RC2)��SDO���M�s���Ƃ���
     if (sdo == 3) SDOSEL = 1 ;    // 3�ԃs��(RA4)��SDO���M�s���Ƃ���
#endif

#if defined(_16F1825) 
     SDO1SEL = 0 ;                 // 8�ԃs��(RC2)��SDO1���M�s���Ƃ���
     if (sdo == 3) SDO1SEL = 1 ;   // 3�ԃs��(RA4)��SDO1���M�s���Ƃ���
#endif

#if defined(_16F1826) || defined(_16F1827)
     SDO1SEL = 0 ;                 //  8�ԃs��(RB2)��SDO1���M�s���Ƃ���
     if (sdo == 15) SDO1SEL = 1 ;  // 15�ԃs��(RA6)��SDO1���M�s���Ƃ���
#endif

#if defined(_16F1829) 
     SDO2SEL = 0 ;                 // 15�ԃs��(RC1)��SDO2���M�s���Ƃ���
     if (sdo == 2) SDO2SEL = 1 ;   //  2�ԃs��(RA5)��SDO2���M�s���Ƃ���
#endif

     con  = 0b00100000 ;       // �N���b�N�ɐ���LOW�@�}�X�^���[�h��FOSC/4�̃N���b�N�ɐݒ�
     stat = 0b00000000 ;       // �N���b�N�ʑ��͂O(�����G�b�W�Ńf�[�^�𑗐M)
     con  = con | divider ;    // �w��̃N���b�N���x��ݒ肷��
     if (mode == SPI_MODE0 || mode == SPI_MODE2) {
          stat = stat | 0x40 ; // �w��̃N���b�N�ʑ���ݒ肷��(�N���b�N�ʑ���1�Ƃ���)
     }
     if (mode == SPI_MODE2 || mode == SPI_MODE3) {
          con = con | 0x10 ;   // �w��̃N���b�N�ɐ���ݒ肷��(�ɐ���HIGH�Ƃ���)
     }
     SPI_SSPCON1 = con ;
     SPI_SSPSTAT = stat ;
     SPI_SSPIF   = 0 ;         // �r�o�h�̊����݃t���O������������
     INTCONbits.PEIE  = 1 ;               // ���ӑ��u�����ݗL��
     INTCONbits.GIE   = 1 ;               // �S�����ݏ�����������
}
/*******************************************************************************
*  SPI_setDataMode(mode)                                                       *
*    �r�o�h�̓]�����[�h�ݒ���s������                                          *
*    mode :   SPI�̓]�����[�h��ݒ肵�܂�(�N���b�N�ɐ��ƃN���b�N�ʑ��̑g����)  *
*******************************************************************************/
void SPI_setDataMode(char mode)
{
     if (mode == SPI_MODE0 || mode == SPI_MODE2) {
          SPI_SSPSTAT = SPI_SSPSTAT | 0x40 ; // �w��̃N���b�N�ʑ����P�ݒ肷��
     } else {
          SPI_SSPSTAT = SPI_SSPSTAT & 0xBF ; // �w��̃N���b�N�ʑ����O�ݒ肷��
     }
     if (mode == SPI_MODE2 || mode == SPI_MODE3) {
          SPI_SSPCON1 = SPI_SSPCON1 | 0x10 ; // �w��̃N���b�N�ɐ���HIGH�ݒ肷��
     } else {
          SPI_SSPCON1 = SPI_SSPCON1 & 0xEF ; // �w��̃N���b�N�ɐ���LOW�ݒ肷��
     }
}
/*******************************************************************************
*  SPI_setClockDivider(divider,rate)                                           *
*    �r�o�h�̒ʐM���x�ݒ���s������                                            *
*    divider :SPI�̒ʐM���x��ݒ肵�܂�                                        *
*             SPI_CLOCK_DIVADD = FOSC/((SSPxADD + 1)*4)                        *
*    rate    :SSPxADD�ɐݒ肷��l                                              *
*******************************************************************************/
void SPI_setClockDivider(char divider,char rate)
{
     if (divider == SPI_CLOCK_DIVADD) {
          SPI_SSPADD = rate ;
     }
     SPI_SSPCON1 = (SPI_SSPCON1 & 0xF0)  | divider ;    // �w��̃N���b�N���x��ݒ肷��
}
/*******************************************************************************
*  ans = SPI_transfer(dt)                                                      *
*    �r�o�h�ʐM�ł̃f�[�^���M�ƃf�[�^��M���s������                            *
*                                                                              *
*    dt  : �W�r�b�g�̑��M����f�[�^���w�肵�܂�                                *
*    ans : �W�r�b�g�̎�M�����f�[�^��Ԃ��܂�                                  *
*******************************************************************************/
unsigned char SPI_transfer(char dt)
{
     SPI_SSPBUF = dt ;            // �f�[�^�̑��M
     while(SPI_SSPIF == 0) ;      // ��M�҂�
     SPI_SSPIF = 0 ;              // �t���O�N���A
     return SPI_SSPBUF ;          // �f�[�^�̎�M
}

