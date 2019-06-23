/*******************************************************************************
*  skL6470.h - L6470�X�e�b�s���O���[�^�h���C�o�֐����C�u�����p�w�b�_�t�@�C��   *
*                                                                              *
* ============================================================================ *
*  VERSION DATE        BY                    CHANGE/COMMENT                    *
* ---------------------------------------------------------------------------- *
*  1.00    2016-07-07  ���ޒ��H�[(���ނ���)  Create                            *
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
 #define _XTAL_FREQ 16000000       // �g�p����PIC���ɂ�蓮����g���l��ݒ肷��
#endif

#define L6470_CS	PORTBbits.RB0	        // L6470��SPI-CS�̃s���ԍ�
#define L6470_BUSY	PORTBbits.RB1	// L6470��BUSY�̃s���ԍ�

// ���W�X�^�̃A�h���X
#define L6470_ACC	0x05	// �������̌W��(12bit)
#define L6470_DEC	0x06	// �������̌W��(12bit)
#define L6470_MAX_SPEED	0x07	// �ő��]���x(10bit)
#define L6470_MIN_SPEED	0x08	// �ŏ���]���x(13bit)
#define L6470_KVAL_HOLD	0x09	// ��~���̓d��(8bit)
#define L6470_KVAL_RUN	0x0A	// �葬��]���̓d��(8bit)
#define L6470_KVAL_ACC	0x0B	// �������̓d��(8bit)
#define L6470_KVAL_DEC	0x0C	// �������̓d��(8bit)
#define L6470_OCD_TH	0x13	// �ߓd����臒l(4bit)
#define L6470_STALL_TH	0x14	// �X�g�[���d����臒l(7bit)
#define L6470_STEP_MODE	0x16	// �X�e�b�v���[�h�̐ݒ�(8bit)
#define L6470_ALARM_EN	0x17	// �e��A���[���̗L���ݒ�(8bit)
#define L6470_CONFIG	0x18	// �R���t�B�O�̐ݒ�(16bit)
#define L6470_STATUS	0x19	// ���݂̃X�e�[�^�X(16bit)

// �萔�̒�`
#define Forward         1       // ���[�^�͐��]�ŉ�]
#define Reverse         0       // ���[�^�͋t�]�ŉ�]
#define SOFTSTOP        0       // ������~�A�ێ��g���N�L��
#define HARDSTOP        1       // ������~�A�ێ��g���N�L��
#define SOFTHIZ         2       // ������~�A�ێ��g���N����
#define HARDHIZ         3       // ������~�A�ێ��g���N����
#define FULL_STEP	0	// �t���X�e�b�v(���[�^�Ɠ����X�e�b�v��)
#define HALF_STEP	1	// �n�[�t�X�e�b�v(���[�^�Ɠ����X�e�b�v��x2)
#define MICROSTEP8	3	// 1/8 �}�C�N���X�e�b�v(���[�^�Ɠ����X�e�b�v��x8)
#define MICROSTEP64	6	// 1/64  �}�C�N���X�e�b�v(���[�^�Ɠ����X�e�b�v��x64)
#define MICROSTEP128	7	// 1/128 �}�C�N���X�e�b�v(���[�^�Ɠ����X�e�b�v��x128)
#define OSC_INT16MHZ    0       // �N���b�N�͓���16MHz


// ���W�X�^�̍\���̒�`
// STEP_MODE Register
union STEP_MODE_t {           // �X�e�b�s���O���[�h���W�X�^(�f�t�H���g=0x07)
     unsigned char DT ;
     struct {
          unsigned STEP_SEL   :3 ;      // �X�e�b�s���O���[�h�̐ݒ�
          unsigned SP         :1 ;      // �\��
          unsigned SYNC_SEL   :3 ;      // �������g���̑I��
          unsigned SYNC_EN    :1 ;      // BUSY/SYNC�̑I��
     } ;
} ;
// CONFIG Register
union CONFIG_t {              // �R���t�B�O���W�X�^(�f�t�H���g=0x2E88)
     unsigned int DT ;
     struct {
          unsigned OSC_SEL    :3 ;      // �N���b�N�̑I��
          unsigned EXT_CLK    :1 ;      // �N���b�N�o�̗͂L��
          unsigned SW_MODE    :1 ;      // SW�@�\�̑I��
          unsigned EN_VSCPMP  :1 ;      // ���[�^�̓d���d���⏞�̗L��
          unsigned SP         :1 ;      // �\��
          unsigned OC_SD      :1 ;      // �ߓd���������̃u���b�W��H��~�̗L��
          unsigned POW_SR     :2 ;      // ���[�^�o�͂̃X���[���[�g�l�̐ݒ�
          unsigned F_PWM_DEC  :3 ;      // PWM���g�������̐����������ݒ�
          unsigned F_PWM_INT  :3 ;      // PWM���g�������{����ݒ�
     } ;
} ;


// �֐��̃v���g�^�C�v�錾
unsigned long L6470_Init() ;
unsigned long L6470_GetRegister(char regadrs,int num) ;
int  L6470_ALARM() ;
void L6470_Run(char dir,unsigned long speed) ;
void L6470_Move(char dir,unsigned long step) ;
void L6470_Stop(int mode) ;

#endif
