/*******************************************************************************
*  skSPIlib.h - �r�o�h�ʐM���s���֐��̃w�b�_�t�@�C��                           *
*               (PIC 12F1822 16F182x 16F193x 18F2xK22)                         *
*                                                                              *
* ============================================================================ *
*  VERSION DATE        BY                    CHANGE/COMMENT                    *
* ---------------------------------------------------------------------------- *
*  1.00    2012-04-30  ���ޒ��H�[            Create                            *
*  1.11    2014-06-21  ���ޒ��H�[(���ނ���)  SPI_MODE�̃R�����g��ύX          *
*  2.00    2014-09-03  ���ޒ��H�[(���ނ���)  18F25K22�̓���m�F,�����ݏ����~�߂�*
*  3.00    2015-04-05  ���ޒ��H�[(���ނ���)  ���x/�]��mode�֐���ǉ�           *
*  3.10    2015-04-20  ���ޒ��H�[(���ނ���)  SSP1/SSP2������16F193x�ɑΉ�      *
*  3.20    2016-06-14  ���ޒ��H�[(���ނ���)  SPI_MODE��CKE(�N���b�N�ʑ�)��ύX *
*******************************************************************************/
#ifndef _SPILIB_H_
#define _SPILIB_H_


// �萔�̒�`
#ifndef LOW
#define LOW  0
#endif
#ifndef HIGH
#define HIGH 1
#endif

#define SPI_MODE1 1           // �N���b�N�ɐ�(0:LOW)  �N���b�N�ʑ�(0:�A�C�h��0V�ŁA0V->5V�ɕω��œ]��)
#define SPI_MODE0 0           // �N���b�N�ɐ�(0:LOW)  �N���b�N�ʑ�(1:�A�C�h��0V�ŁA5V->0V�ɕω��œ]��)
#define SPI_MODE3 3           // �N���b�N�ɐ�(1:HIGH) �N���b�N�ʑ�(0:�A�C�h��5V�ŁA5V->0V�ɕω��œ]��)
#define SPI_MODE2 2           // �N���b�N�ɐ�(1:HIGH) �N���b�N�ʑ�(1:�A�C�h��5V�ŁA0V->5V�ɕω��œ]��)
#define SPI_CLOCK_DIV4   0x0  // �ʐM���x  Fosc/4
#define SPI_CLOCK_DIV16  0x1  // �ʐM���x  Fosc/16
#define SPI_CLOCK_DIV64  0x2  // �ʐM���x  Fosc/64
#define SPI_CLOCK_DIVT2  0x3  // �ʐM���x  TMR2�̏o�͂�1/2
#define SPI_CLOCK_DIVADD 0xA  // �ʐM���x  FOSC/((SSPxADD + 1)*4)

//#define SPI_MSSP2_USE         // MSSP2�𗘗p����ꍇ�̓R�����g���O���܂��B
// �l�r�r�o���W���[���̃��W�X�^�[��`
#if defined(_16F1933)||defined(_16F1934)||defined(_16F1936)||\
    defined(_16F1937)||defined(_16F1938)||defined(_16F1939)
// 16F193x�p��MSSP�𗘗p����ꍇ�̒�`
#define SPI_SSPCON1         SSPCON1
#define SPI_SSPSTAT         SSPSTAT
#define SPI_SSPADD          SSPADD
#define SPI_SSPBUF          SSPBUF
#define SPI_SSPIF           SSPIF
#else
#ifdef  SPI_MSSP2_USE
// MSSP2�𗘗p����ꍇ�̒�`
#define SPI_SSPCON1         SSP2CON1
#define SPI_SSPSTAT         SSP2STAT
#define SPI_SSPADD          SSP2ADD
#define SPI_SSPBUF          SSP2BUF
#define SPI_SSPIF           SSP2IF
#else
// MSSP1�𗘗p����ꍇ�̒�`
#define SPI_SSPCON1         SSPCON1
#define SPI_SSPSTAT         SSPSTAT
#define SPI_SSPADD          SSPADD
#define SPI_SSPBUF          SSPBUF
#define SPI_SSPIF           PIR1bits.SSPIF
#endif
#endif

// �֐��̃v���g�^�C�v�錾
void SPI_Init(char mode,char divider,int sdo) ;
void SPI_setDataMode(char mode) ;
void SPI_setClockDivider(char divider,char rate) ;
unsigned char SPI_transfer(char dt) ;


#endif
