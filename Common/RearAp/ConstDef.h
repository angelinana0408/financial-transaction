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
// ----------------------------------------------------------------------------
// HISTORY OF [ConstDef.h] 
// ----------------------------------------------------------------------------
// - 2004.06.16 YOO		CHINA OP�� ���ۼ�
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// DEIVCE IDENTIFIER : ID
///////////////////////////////////////////////////////////////////////////////
#define		DEV_NONE					0x00000000			// ��ġ����
#define		DEV_SOP						0x00000040			// SCREEN : FLASH ACTIVEX
#define		DEV_TTU						0x80000000			// TTU


///////////////////////////////////////////////////////////////////////////////
// DEIVCE IDENTIFIER : NAME
///////////////////////////////////////////////////////////////////////////////
#define		DEVNM_SOP					("SOP")				// SCREEN FOR OP : 2004.06.16
#define		DEVNM_TTU					("TTU")				// TTU

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
// SCREEN ��ġ ��� ���� 
// ����̽� : SOP
///////////////////////////////////////////////////////////////////////////////
#define		SOP_SCREEN					("APCenterMovie")	// ȭ�����
#define		SOP_TOP						("APLocalMovie")	// ȭ���� ������
#define		SOP_TIME					("APMaxTime")		// �Է´��ð� 
#define		SOP_STRING					("APValue")			// ���ڿ� ���
#define		SOP_BTNONOFF				("APState")			// ��ư ON-OFF�� ���

#define		SOP_UPDATE					("APUpdate")		// ��ü���ڿ� �����
#define		SOP_UPDATE_SCREEN			("SCREEN")			// ��ü���ڿ� �����
#define		SOP_UPDATE_MESSAGE			("MESSAGE")			// �޼���     �����

#define		SOP_PROGRESS				("APProgress")		// PROCESS=����,FINISH=����
#define		SOP_PROGRESS_PROCESS		("PROCESS")			// ���� ������
#define		SOP_PROGRESS_FINISH			("FINISH")			// ���� ������

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
// MACRO DEFINITION
///////////////////////////////////////////////////////////////////////////////
//1. ���ڿ��� "ON"/"OFF"���� Ȯ��
#define		MAC_IS_SZONOFF(sFlag)		((strcmp(sFlag, "ON" ) == 0) ? TRUE	:		\
										 (strcmp(sFlag, "OFF") == 0) ? TRUE	: FALSE )
#endif // _CONST_DEF_H_
