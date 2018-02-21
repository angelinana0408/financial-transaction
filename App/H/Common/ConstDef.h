#ifndef _CONST_DEF_H_
#define _CONST_DEF_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

///////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------------------
//
//                                 CONSTDEF.H
//
// - �������� MWI ActiveX�� ���� ������ ��ġ���� ���� ��� Definition Header
//   file �Դϴ�.
// - ���ϳ����� ����� ����� ���� ����/����/����/�߰� ���� ������ �ʿ��� ���
//   �������� ���� �����ϴ� �� �̿ܿ� MWI����ڿ��� ������ �����Ͽ� �ֽʽÿ�.
// 
// - �������� ����Կ� �־� ������ ������ ������ ���� ����� �����մϴ�.
//   ���� : T1ATM With KALIGNITE
//                                                       [��ƿ����ȿ�� SE����]
//
// ----------------------------------------------------------------------------
// HISTORY OF [ConstDef.h] 
// ----------------------------------------------------------------------------
//
// - 2003.06.18	teo		MWI���� ����ϴ� ��(Return Value/Parameter)�� ��������
//						�ʱ� ���� �ۼ�
//
// - 2003.09.25	teo		���κ� �������� �� ����(0x10->0x01)
//
// - 2003.09.30	teo		���ܼ���/������ �߰��� ���� �� ����
//
// - 2003.10.30	HJH		NODEVICE �����߰�
//
// - 2003.11.13	teo		DES��ġ �߰��� ���� �� �߰�(��ġID/NAME, DES_TYPE��)
//
// - 2003.01.09	teo		UCM ��ݺ� �������� ����(SP����ľ� �̼����� ���� ����)
//
// - 2004.01.15	teo		CDU��ġ �߰��� ���� �� �߰�(��ġID/NAME ��)
//
// - 2004.01.20	teo		CDU��ġ �߰��� ���� �� �߰�(CDU���� ��)
//
// - 2005.06.XX	teo		CIM��ġ�߰�
//
// ----------------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// DEIVCE IDENTIFIER : ID
///////////////////////////////////////////////////////////////////////////////
#define		DEV_NONE					0x00000000			// ��ġ����
#define		DEV_SPR						0x00000001			// ��ǥ��
#define		DEV_JPR						0x00000002			// ���������κ�
#define		DEV_MCU						0x00000004			// ī���
#define		DEV_BRM						0x00000008			// ��������ݺ�
#define		DEV_CSH						(DEV_CDU | DEV_CIM)	// ��������ݺ� V05.01.01

#define		DEV_CAM						0x00000010			// ��ǥ����ݺ�
#define		DEV_PBM						0x00000020			// �����
#define		DEV_SCR						0x00000040			// SCREEN : FLASH ACTIVEX

#define		DEV_CDU						0x00000100			// CDU			// 2004.01.15_1.2_2003
#define		DEV_PIN						0x00000200			// PIN			// 2004.01.15_1.2_2003

#define		DEV_DOR						0x00001000			// �޹�
#define		DEV_UPS						0x00002000			// ����������ġ
#define		DEV_SIU						0x00004000			// GUIDELIGHT & INDICATOR			
#define		DEV_LGT						DEV_SIU
#define		DEV_SPL						0x00008000			// 7SEG/LED/INFORM/RPL

#define		DEV_CMR						0x00020000			// USB 2 CAMERA
#define		DEV_IRD						0x00040000			// ���ܼ�		// 20030930_1.0_1081
#define		DEV_FNG						0x00080000			// ����			// 20030930_1.0_1081

#define		DEV_BAR						0x00100000			// BARCode Reader // 2003.11.20
#define		DEV_SNS						0x00200000			// Sensor		// 2004.04.14
#define		DEV_VFD						0x00400000			// VFD			// 2004.12.24
#define		DEV_CIM						0x00800000			// CashAcceptor	V05.01.01
#define		DEV_RIC						0x01000000			// RIC			// 2008.06.24

#define		DEV_MAIN					(DEV_MCU	| DEV_SPR	| DEV_JPR	|	\
										 DEV_PBM	| DEV_CSH				)
																			// 2007.06.06
#define		DEV_CLERK					(DEV_SPR	| DEV_JPR	| DEV_CSH	)

///////////////////////////////////////////////////////////////////////////////
// DEIVCE IDENTIFIER : NAME
///////////////////////////////////////////////////////////////////////////////
#define		DEVNM_NONE					("NONE")			// ��ġ����
#define		DEVNM_MCU					("MCU")				// ī���
#define		DEVNM_SPR					("SPR")				// ��ǥ��
#define		DEVNM_JPR					("JPR")				// ���������κ�
#define		DEVNM_PBM					("PBM")				// �����
#define		DEVNM_BRM					("BRM")				// ��������ݺ�

#define		DEVNM_CDU					("CDU")				// ����/��ǥ��ݺ� 2004.01.15_1.2_2003
#define		DEVNM_PIN					("PINPAD")			// PINPAD 2004.01.15_1.2_2003

#define		DEVNM_DOR					("RDOOR")			// �޹�
#define		DEVNM_UPS					("UPS")				// ����������ġ
#define		DEVNM_SIU					("SIU")				// SENSOR & INDICATOR			
#define		DEVNM_SPL					("SPL")				// 7SEG/LED/INFORM/RPL
#define		DEVNM_NET					("NET")				// NETWORK
#define		DEVNM_CMR					("CAMERA")			// USB 2 CAMERA
#define		DEVNM_SCR					("SCR")				// SCREEN

#define		DEVNM_FNG					("FINGER")			// ������		20030930_1.0_1081
#define		DEVNM_IRD					("IRDA")			// ���ܼ���		20030930_1.0_1081
#define		DEVNM_BAR					("BARCODE")			// BarCode 		2003.11.20
#define		DEVNM_SNS					("SENSOR")			// Sensor		// 2004.04.14
#define		DEVNM_VFD					("VFD")				// VFD	 		2004.12.24
#define		DEVNM_CIM					("CIM")				// �����Աݺ�   V05.02.00
#define		DEVNM_RIC					("RIC")				// RIC	 		2008.06.24

#define		DEVNM_MAIN					("MAIN")			// 

///////////////////////////////////////////////////////////////////////////////
// �Լ�ó����� RETURN VALUE
// ����̽� : -
///////////////////////////////////////////////////////////////////////////////
#define		R_NORMAL					0
#define		R_DOING						1
#define		R_ERROR						2
#define		R_TIMEOVER					3
#define		R_CANCEL					-1
#define		R_TIMEOUT					-2

///////////////////////////////////////////////////////////////////////////////
// ��ġ ����̽� ���� ����
// ����̽� : JPR / SPR / MCU / PBM / BRM / UCM
///////////////////////////////////////////////////////////////////////////////
#define		NORMAL						0					// �������� 
#define		DOING						1					// ���� ������
#define		DOWN						2					// ��� ���
#define		NODEVICE					3					// ��� �̿���

///////////////////////////////////////////////////////////////////////////////
// ���� ���� �ܷ�����
// ����̽� : JPR
///////////////////////////////////////////////////////////////////////////////
#define		JNL_NORMAL					0x00				// ���� ���� ����
#define		JNL_EMPTY_PAPER				0x01				// ���� ���� �亸��		// 20030925_1.0_1078
#define		JNL_LOW_END					0x02				// ���� ���� �ܷ���		// 20030925_1.0_1078

///////////////////////////////////////////////////////////////////////////////
// ��ü �ܷ�����
// ����̽� : JPR / SPR / MCU / PBM
///////////////////////////////////////////////////////////////////////////////
#define		ST_NOT_DETECT				0x00				// ��ü �̰���
#define		ST_SENSOR1					0x01				// �Ա���(���Ա�) ��ü����
#define		ST_SENSOR2					0x02				// ��λ� ��ü����

///////////////////////////////////////////////////////////////////////////////
// IC POWER ���� ����
// ����̽� : MCU
///////////////////////////////////////////////////////////////////////////////
#define		IC_ACT_COLD_RESET			1					// ACTION COLD RESET
#define		IC_ACT_WARM_RESET			2					// ACTION WARM RESET
#define		IC_ACT_POWER_OFF			3					// ACTION POWER OFF

///////////////////////////////////////////////////////////////////////////////
// IC POWER ����
// ����̽� : MCU
///////////////////////////////////////////////////////////////////////////////
#define		IC_POWER_ONLINE				0					// STATUS POWER ONLINE
#define		IC_POWER_POWEREDOFF			1					// STATUS POWER OFF
#define		IC_POWER_BUSY				2					// STATUS POWER BUSY
#define		IC_POWER_NODEVICE			3					// STATUS POWER NO DEVICE
#define		IC_POWER_HWERROR			4					// STATUS POWER HW ERROR
#define		IC_POWER_NOCARD				5					// STATUS POWER NO CARD
#define		IC_POWER_NOTSUPP			6					// STATUS POWER NOT SUPP

///////////////////////////////////////////////////////////////////////////////
// ��ǥ ���� �ܷ�����
// ����̽� : SPR
///////////////////////////////////////////////////////////////////////////////
#define		SLIP_NORMAL					0x00				// ��ǥ ���� ����
#define		SLIP_EMPTY_PAPER			0x01				// ��ǥ ���� �亸��
#define		SLIP_LOW_END				0x02				// ��ǥ ���� �ܷ���

///////////////////////////////////////////////////////////////////////////////
// ��ġ����̽� ����Ź ����
// ����̽� : UCM / BRM
///////////////////////////////////////////////////////////////////////////////
#define		NORMAL_POS					0					// ������ġ
#define		ABNORMAL_POS				1					// Ż��

///////////////////////////////////////////////////////////////////////////////
// ��ġ����̽� ī��Ʈ ��������
// ����̽� : UCM / BRM
///////////////////////////////////////////////////////////////////////////////
#define		CST_NORMAL					0					// ����
#define		CST_NEAR					1					// CASSETTE NEAR END
#define		CST_EMPTY					2					// CASSETTE EMPTY
#define		CST_SET_NG					3					// SET NG
#define		CST_FULL					4					// CASSETTE FULL
#define		CST_ERROR					5					// CASSETTE ERROR
#define		CST_MANIP					6					// CASSETTE MANIP
#define		CST_HIGH					7					// CASSETTE HIGH

///////////////////////////////////////////////////////////////////////////////
// ��ġ����̽� DISPENSER ��ġ����
// ����̽� : CDU
// 2004.01.20_1.2_2003 
// 20040315 JUNXI CHINA CDU ����
///////////////////////////////////////////////////////////////////////////////
#define		DISPENSER_NOT_SET			0					// ������ݺξ���
#define		DISPENSER_1CASH				1					// 1 CST	2004.11.05 
#define		DISPENSER_2CASH				2					// 2 CST 
#define		DISPENSER_3CASH				3					// 3 CST
#define		DISPENSER_4CASH				4					// 4 CST
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// ��ġ����̽� ī��Ʈ ��������
// ����̽� : CDU / BRM
// 2004.01.19_1.2_2003 ��ġ�����߰�
///////////////////////////////////////////////////////////////////////////////
#define		CDU_CST_REJECT				0					// Reject CST
#define		CDU_CST_1					1					// 1st    CST
#define		CDU_CST_2					2					// 2nd    CST
#define		CDU_CST_3					3					// 3rd    CST
#define		CDU_CST_4					4					// 4th    CST
#define		CDU_CST_5					5					// 5th    CST
#define		CDU_CST_RETRACT				6					// Retract CST

#define		BRM_CST_REJECT				0					// Reject CST
#define		BRM_CST_1					1					// 1st    CST
#define		BRM_CST_2					2					// 2nd    CST
#define		BRM_CST_3					3					// 3rd    CST
#define		BRM_CST_4					4					// 4th    CST
#define		BRM_CST_5					5					// 5th    CST
#define		BRM_CST_RETRACT				6					// Retract CST

#define		CSH_CST_REJECT				0					// Reject CST
#define		CSH_CST_1					1					// 1st    CST
#define		CSH_CST_2					2					// 2nd    CST
#define		CSH_CST_3					3					// 3rd    CST
#define		CSH_CST_4					4					// 4th    CST
#define		CSH_CST_5					5					// 5th    CST
#define		CSH_CST_RETRACT				6					// Retract CST

///////////////////////////////////////////////////////////////////////////////
// ��ݰ��� ���� 
// ����̽� : CDU / BRM 
// �߱����� ��� ����ȭ 2004.03.31
///////////////////////////////////////////////////////////////////////////////
#define		WITH_NOT_AVAILABLE			0					// ��� �Ұ���
#define		WITH_TYPE1_ONLY				1					// 100/20���� ��� ����
#define		WITH_TYPE2_ONLY				2					// 50/10���� ��� ����
#define		WITH_BOTH_AVAILABLE			(WITH_TYPE1_ONLY | WITH_TYPE2_ONLY)
															// 100/20�� + 50/10�� ��� ����

///////////////////////////////////////////////////////////////////////////////
// �Աݰ��� ���� 
// ����̽� : CDU / BRM 
// �߱����� ��� ����ȭ 2004.03.31
///////////////////////////////////////////////////////////////////////////////
#define		DEP_NOT_AVAILABLE			0					// �Ա� �Ұ���
#define		DEP_TYPE100_ONLY			1					// 100���� �Ա� ����
#define		DEP_TYPE50_ONLY				2					// 50���� �Ա� ����
#define		DEP_TYPE20_ONLY				4					// 20���� �Ա� ����
#define		DEP_TYPE10_ONLY				8					// 10���� �Ա� ����
#define		DEP_ALL_AVAILABLE			(DEP_TYPE100_ONLY | DEP_TYPE50_ONLY | DEP_TYPE20_ONLY | DEP_TYPE10_ONLY)
															// 100/50/20/10�� �Ա� ����
///////////////////////////////////////////////////////////////////////////////
// ��ݰ��� ���� 
// ����̽� : UCM / BRM
///////////////////////////////////////////////////////////////////////////////
#define		WITHX_NOT_AVAILABLE			0					// ��� �Ұ���
#define		WITHX_CASH_ONLY				1					// ������ ��� ����
#define		WITHX_CHECK_ONLY			2					// ��ǥ�� ��� ����
#define		WITHX_BOTH_AVAILABLE		3					// ���� + ��ǥ ��� ����

///////////////////////////////////////////////////////////////////////////////
// �Աݰ��� ���� 
// ����̽� : UCM / BRM
///////////////////////////////////////////////////////////////////////////////
#define		DEPX_NOT_AVAILABLE			0					// �Ա� �Ұ���
#define		DEPX_CASH_ONLY				1					// ���ݸ� �Աݰ���
#define		DEPX_CHECK_ONLY				2					// ��ǥ�� �Աݰ���
#define		DEPX_BOTH_AVAILABLE			3					// ����+��ǥ �Աݰ���
#define		DEPX_ONLY_TEN_TH			4					// �����Ǹ� �Աݰ���

///////////////////////////////////////////////////////////////////////////////
// ��ġ ����̽� ����Ż�� ����
// ����̽� : BRM
///////////////////////////////////////////////////////////////////////////////
#define		NO_CHANGE					0					// ����Ż������
#define		CHANGE_POS					1					// ����Ż��

///////////////////////////////////////////////////////////////////////////////
// �ڵ�����/ȸ�� ����
// ����̽� : BRM 
///////////////////////////////////////////////////////////////////////////////
#define		NEED_NONE					0					// �״��
#define		NEED_REFILL					1					// �����ʿ�
#define		NEED_TAKEUP					2					// ȸ���ʿ�

///////////////////////////////////////////////////////////////////////////////
// ���� �ܷ� ����
// ����̽� : BRM 
///////////////////////////////////////////////////////////////////////////////
#define		BRM_CLEAR					0x00				// ���� ����
#define		BRM_IN_CSM					0x01				// ���� ���Ա��� ���� ����
#define		BRM_IN_RJSD					0x02				// ��ȯ ����Ŀ�� ���� ����
#define		BRM_IN_NSTK					0x04				// �Ͻ� ����Ŀ�� ���� ����
#define		BRM_ON_PATH					0x08				// ���� �̵� ��λ� ���� ����
#define		IN_CSM						BRM_IN_CSM
#define		IN_RJSD						BRM_IN_RJSD
#define		IN_NSTK						BRM_IN_NSTK
#define		IN_PATH						BRM_ON_PATH

///////////////////////////////////////////////////////////////////////////////
// ���� �Աݻ��� ����
// ����̽� : BRM 
// ���     : 0~2�� ���� [��ġ ����̽� ���� ����]�� ����Ѵ�.
///////////////////////////////////////////////////////////////////////////////
#define		NO_MONEY					3					// ����� ���� ����
#define		SKEW_NOTE					4					// ����� SKEW �߻�
#define		MAX_OVER					5					// �Ա� �ִ� �ż� �ʰ�
#define		UNKNOWN_MONEY				6					// �������� ���� ���� ����

///////////////////////////////////////////////////////////////////////////////
// ���� ������� ����
// ����̽� : BRM 
///////////////////////////////////////////////////////////////////////////////
#define		GOT_MONEY					0					// ���� ����
#define		WAIT_USER					1					// ���� �̼���

///////////////////////////////////////////////////////////////////////////////
// ���� �ٱ��� ��������
// ����̽� : CDU
// �߱����� ����ȭ	2004.03.31 
///////////////////////////////////////////////////////////////////////////////
#define		CASH_CHINA_100				100					// 100����
#define		CASH_CHINA_50				50					// 50����
#define		CASH_CHINA_20				20					// 20����
#define		CASH_CHINA_10				10					// 10����
#define		CASH_CHINA_5				5					//  5����
#define		CASH_CHINA_1				1					//  1����
#define		CASH_CHINA_UNKNOWN			0					// UNKNOWN

///////////////////////////////////////////////////////////////////////////////
// ���ī��Ʈ ���� �������
// ����̽� : CDU
// �߱����� ����ȭ	2004.03.31 
///////////////////////////////////////////////////////////////////////////////
#define		CASH_TYPE_NOTSET			0
#define		CASH_TYPE_BILL				1
#define		CASH_TYPE_RECYCLE			2

///////////////////////////////////////////////////////////////////////////////
// �Ա�ī��Ʈ ���� �������
// ����̽� : CDU
// �߱����� ����ȭ	2004.03.31 
///////////////////////////////////////////////////////////////////////////////
#define		CASH_TYPE_DEPOSIT			1
#define		CASH_TYPE_DEPOSIT100		2
#define		CASH_TYPE_DEPOSIT50			3
#define		CASH_TYPE_RECYCLE100		4
#define		CASH_TYPE_RECYCLE50			5
#define		CASH_TYPE_RETRACT			6

// Deposit Type
#define		DEPOSIT						1
#define		DEPOSIT100					2
#define		DEPOSIT100B					3
#define		DEPOSIT100C					4
#define		DEPOSIT100D					5
#define		DEPOSIT100E					6
#define		DEPOSIT50					7
#define		DEPOSIT50B					8
#define		DEPOSIT50C					9
#define		DEPOSIT50D					10
#define		DEPOSIT50E					11
#define		RECYCLE100					12
#define		RECYCLE100B					13
#define		RECYCLE100C					14
#define		RECYCLE100D					15
#define		RECYCLE100E					16
#define		RECYCLE50					17
#define		RECYCLE50B					18
#define		RECYCLE50C					19
#define		RECYCLE50D					20
#define		RECYCLE50E					21
#define		MULTICST					22
#define		MULTICSTB					23
#define		MULTICSTC					24
#define		MULTICSTD					25
#define		MULTICSTE					26
#define		RETRACT						27
#define		MULTICST0					28
#define		MULTICST10					29
#define		MULTICST20					30
#define		MULTICST50					31
#define		MULTICST100					32
#define		MULTICSTB0					33
#define		MULTICSTB10					34
#define		MULTICSTB20					35
#define		MULTICSTB50					36
#define		MULTICSTB100				37
#define		MULTICSTC0					38
#define		MULTICSTC10					39
#define		MULTICSTC20					40
#define		MULTICSTC50					41
#define		MULTICSTC100				42
#define		MULTICSTD0					43
#define		MULTICSTD10					44
#define		MULTICSTD20					45
#define		MULTICSTD50					46
#define		MULTICSTD100				47
#define		MULTICSTE0					48
#define		MULTICSTE10					49
#define		MULTICSTE20					50
#define		MULTICSTE50					51
#define		MULTICSTE100				52

///////////////////////////////////////////////////////////////////////////////
// �޹� �������� ����
// ����̽� : DOR [RearDoor]
///////////////////////////////////////////////////////////////////////////////
#define		DOOR_OPENED					0					// �޹�����
#define		DOOR_CLOSED					1					// �޹�����

///////////////////////////////////////////////////////////////////////////////
// Shutter Status
///////////////////////////////////////////////////////////////////////////////
#define		SHUTTER_CLOSED				0					// CLOSED
#define		SHUTTER_OPEN				1					// OPEN
#define		SHUTTER_JAMMED				2					// JAMMED
#define		SHUTTER_UNKNOWN				3					// UNKNOWN
#define		SHUTTER_NOTSUPPORTED		4					// NOTSUPPORTED

///////////////////////////////////////////////////////////////////////////////
// Retract Action Result
///////////////////////////////////////////////////////////////////////////////
#define		RETRACT_RETRACTED			0					// RETRACTED
#define		RETRACT_CASHTAKEN			1					// CASHTAKEN
#define		RETRACT_NOTACTIVE			2					// NOTACTIVE

///////////////////////////////////////////////////////////////////////////////
// PINPAD �Է¸�� ����
// ����̽� : PINPAD
// �߱����� ����ȭ	2004.04.02 
///////////////////////////////////////////////////////////////////////////////
#define		PINMODE_NOT_USE				0					// �̻��ó��
#define		PINMODE_NORMAL				1					// Normal �Է¸��
#define		PINMODE_EPP					2					// Epp �Է¸��

///////////////////////////////////////////////////////////////////////////////
// FLICKER INDEX ����
// ����̽� : LGT [LIGNT-FLICKER]
///////////////////////////////////////////////////////////////////////////////
#define		BRM_FLICKER					0x01				// ���� �Ա�/���ⱸ
#define		PBM_FLICKER					0x02				// ���� ����/���ⱸ
#define		SPR_FLICKER					0x04				// ��ǥ ���ⱸ
#define		MCU_FLICKER					0x08				// ī�� ����/���ⱸ(��ǥ�͵���)	�߱������û(04->08) : 2004.04.09
#define		IC_FLICKER					0x08				// IC ī�� ����/���ⱸ				�߱������û(04->08) : 2004.04.09
#define		CDU_FLICKER					0x11				// CDU ������ⱸ					�߱������û(09->11) : 2004.04.09
#define		PIN_FLICKER					0x20				// PIN �Է�(NC3000)					�߱������û(10->20) : 2004.04.09
#define		ALL_FLICKER					(BRM_FLICKER | PBM_FLICKER | SPR_FLICKER |	\
										 MCU_FLICKER | IC_FLICKER  |				\
										 CDU_FLICKER | PIN_FLICKER)
										// ��� FLICKER

///////////////////////////////////////////////////////////////////////////////
// FLICKER ���� ����
// ����̽� : LGT [LIGNT-FLICKER]
///////////////////////////////////////////////////////////////////////////////
#define		FLICKER_OFF					0					// FLICKER OFF
#define		FLICKER_ON					1					// FLICKER ON

///////////////////////////////////////////////////////////////////////////////
// INDICATOR(�ŷ�ǥ�õ�) ���� ����
// ����̽� : LGT [LIGNT-INDICATOR]
///////////////////////////////////////////////////////////////////////////////
#define		IND_TRANENABLE				0x01				// �ŷ�����	��
#define		IND_TRANREADY				0x02				// �غ���(�ŷ��Ұ�)	��
#define		IND_CASH					0x04				// ����	��
#define		IND_CHECK		 			0x08				// ��ǥ	��
#define		IND_DEPOSIT		 			0x10				// �Ա�	��
#define		IND_WITHDRAW	 			0x20				// ���	��
#define		IND_TRANSFER	 			0x40				// ��ü	��
#define		IND_PASSBOOK	 			0x80				// ����	��
#define		IND_ALL						0xff				// ���ǥ�õ� ��δ�	��
#define		IND_TRANALL	 		 		(IND_ALL & ~IND_TRANENABLE & ~IND_TRANREADY)
															// �ŷ�����/�غ����� ������ ���ǥ�õ�	��
#define		IND_ALLOFF	 				0x00				// ���ǥ�õ� ��δ� ��
#define		IND_TRANALLOFF				(IND_TRANENABLE | IND_TRANREADY)
															// �ŷ�����/�غ����� ������ ���ǥ�õ� ��
///////////////////////////////////////////////////////////////////////////////
// SPL ��ġ ���� 
// ����̽� : SPL [7SEG/LED/RPL/INFORM]
// ���     : �ϴ��� ������ T1ATM�� �������� �ۼ��Ǿ���
///////////////////////////////////////////////////////////////////////////////
//       < SPL::LED >
//		��������������������������������������������������������������
//		��	  ���� ����   ��               ��� / �̻����          ��
//		��������������������������������������������������������������
//		��  �� �� ��[0]   ��    ī    ��[4]         �� �� ǥ[8]     ��
//		��  ��    ��[1]   ��    ��    ��[5]         ��    ��[9]     ��
//		��  ��    ��[2]   ��    ��    ��[6]         �Աݼ�ǥ[10]    ��
//		��  DUMMY   [3]   ��    ��ݼ�ǥ[7]         ȸ �� ��[11]    ��
//		��������������������������������������������������������������
//		
//       < SPL::RPL >
//		��������������������������������������������������������������
//		��	  INDEX       ��               CONTENTS                 ��
//		��������������������������������������������������������������
//		��       [0]      ��    NORMAL  - �����                    ��
//		��       [1]      ��    CASH    - ����                      �� 
//		��       [2]      ��    LOSS    - �н�                      ��
//		��       [3]      ��    ALARM   - �˶�                      ��
//		��       [4]      ��    RECEIPT - ��ǥ                    ��
//		��       [5]      ��    CALL    - ȣ��                      ��
//		��������������������������������������������������������������
//		  
//       < SPL::INFORM >
//		��������������������������������������������������������������
//		��	  INDEX       ��               CONTENTS                 ��
//		��������������������������������������������������������������
//		��       [0]      ��    LOSS    - �н�                      ��
//		��       [1]      ��    CASH    - ����                      �� 
//		��       [2]      ��    ALARM   - �˶�                      ��
//		��       [3]      ��    RECEIPT - ��ǥ                    ��
//		��������������������������������������������������������������
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// LED ���� ���� 
// ����̽� : LED
///////////////////////////////////////////////////////////////////////////////
#define		ON_USING_LED				1					// �����	(T1ATM��)
#define		ALERT_LED					2					// ���		(T1ATM��)
#define		ABNORMAL_LED				3					// �̻�		(T1ATM��)
#define		DUMMY_LED					4					// DUMMY	(T1ATM��)
#define		CARD_LED					5					// ī��		(T1ATM��)
#define		PASSBOOK_LED				6					// ����		(T1ATM��)
#define		CASH_LED					7					// ����		(T1ATM��)
#define		OUT_CHECK_LED				8					// ��ݼ�ǥ	(T1ATM��)
#define		SLIP_LED					9					// ��ǥ	(T1ATM��)
#define		JNL_LED						10					// ����		(T1ATM��)
#define		IN_CHECK_LED				11					// �Աݼ�ǥ	(T1ATM��)
#define		RJB_LED						12					// ȸ����	(T1ATM��)

///////////////////////////////////////////////////////////////////////////////
// LED ���� ���� 
// ����̽� : LED
///////////////////////////////////////////////////////////////////////////////
#define		LED_OFF						0					// LED OFF
#define		LED_ON						1					// LED ON

///////////////////////////////////////////////////////////////////////////////
// MCD4 LED ���� ���� 
// ����̽� : LED
///////////////////////////////////////////////////////////////////////////////
#define		LED_STR_OFF					("OFF")				// LED STR OFF
#define		LED_STR_ON					("ON")				// LED STR ON

///////////////////////////////////////////////////////////////////////////////
// UPS ��ġ ���� ���� 
// ����̽� : UPS
///////////////////////////////////////////////////////////////////////////////
#define		POWER_NORMAL				0x00				// POWER����
#define		EXT_AC_OFF					0x01				// �����Ǵ�AC����
#define		INT_BATTERY_LOW				0x02				// �嵥��LOW
#define		CLERK_POWER_OFF				0x04				// �����������
#define		POWER_ABNORMAL				0x08				// UPS���ºҷ�
#define		POWER_DOWN_GOOD				0x10				// POWER DOWN ����ó��

///////////////////////////////////////////////////////////////////////////////
// SCREEN ��ġ ��� ���� 
// ����̽� : SCR
///////////////////////////////////////////////////////////////////////////////
#define		SCR_SCREEN					("APCenterMovie")	// ȭ�����
#define		SCR_TOP						("APLocalMovie")	// ȭ���� ������
#define		SCR_TIME					("APMaxTime")		// �Է´��ð� 
#define		SCR_STRING					("APValue")			// ���ڿ� ���
#define		SCR_BTNONOFF				("APState")			// ��ư ON-OFF�� ���

#define		SCR_UPDATE					("APUpdate")		// ��ü���ڿ� �����
#define		SCR_UPDATE_SCREEN			("SCREEN")			// ��ü���ڿ� �����
#define		SCR_UPDATE_MESSAGE			("MESSAGE")			// �޼���     �����

#define		SCR_PROGRESS				("APProgress")		// PROCESS=����,FINISH=����
#define		SCR_PROGRESS_PROCESS		("PROCESS")			// ���� ������
#define		SCR_PROGRESS_FINISH			("FINISH")			// ���� ������

///////////////////////////////////////////////////////////////////////////////
// Ÿ�Ӿƿ� ������ �ð�����(�ʴ���)
// ����̽� : -
///////////////////////////////////////////////////////////////////////////////
#define		K_NO_WAIT					0
#define		K_1_WAIT					1
#define		K_5_WAIT					5
#define		K_10_WAIT					10
#define		K_15_WAIT					15
#define		K_20_WAIT					20
#define		K_25_WAIT					25
#define		K_30_WAIT					30
#define		K_35_WAIT					35
#define		K_40_WAIT					40
#define		K_45_WAIT					45
#define		K_60_WAIT					60
#define		K_65_WAIT					65
#define		K_120_WAIT					120
#define		K_125_WAIT					125
#define		K_180_WAIT					180
#define		K_360_WAIT					360
#define		K_WAIT						-1

///////////////////////////////////////////////////////////////////////////////
// CHECKDEVICEACTION FUNCTION RETURN VALUE
///////////////////////////////////////////////////////////////////////////////
#define		CHK_NORMAL					("NORMAL")			// ����ó��
#define		CHK_TIMEOUT					("TIMEOUT")			// ��ü �̼���
#define		CHK_TIMEOVER				("TIMEOVER")		// ��ġ ������
#define		CHK_ERROR					("ERROR")			// ó���� ��ֹ߻�

///////////////////////////////////////////////////////////////////////////////
// SCANDEVICEACTION���� ������
///////////////////////////////////////////////////////////////////////////////
#define		EVENT_ALL					0					// ��ü�̺�Ʈ Ȯ��
#define		EVENT_IN					1					// ��ü�Է��̺�Ʈ Ȯ��
#define		EVENT_OUT					2					// ��ü�����̺�Ʈ Ȯ��

///////////////////////////////////////////////////////////////////////////////
// OPERATOR SWITCH���� ������
///////////////////////////////////////////////////////////////////////////////
#define		OPER_NONE					0					// NONE
#define		OPER_SUPERVISOR				1					// SUPERVISOR
#define		OPER_RUN					2					// RUN
#define		OPER_SUPERVISORCHANGED		11					// SUPERVISOR CHANGED
#define		OPER_RUNCHANGED				12					// RUN CHANGED

///////////////////////////////////////////////////////////////////////////////
// MACRO DEFINITION
///////////////////////////////////////////////////////////////////////////////
//1. ���ڿ��� "ON"/"OFF"���� Ȯ��
#define		MAC_IS_SZONOFF(sFlag)		((strcmp(sFlag, "ON" ) == 0) ? TRUE	:		\
										 (strcmp(sFlag, "OFF") == 0) ? TRUE	: FALSE )
#endif // _CONST_DEF_H_
