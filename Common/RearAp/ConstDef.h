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
// - 2004.06.16 YOO		CHINA OP용 재작성
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// DEIVCE IDENTIFIER : ID
///////////////////////////////////////////////////////////////////////////////
#define		DEV_NONE					0x00000000			// 장치없음
#define		DEV_SOP						0x00000040			// SCREEN : FLASH ACTIVEX
#define		DEV_TTU						0x80000000			// TTU


///////////////////////////////////////////////////////////////////////////////
// DEIVCE IDENTIFIER : NAME
///////////////////////////////////////////////////////////////////////////////
#define		DEVNM_SOP					("SOP")				// SCREEN FOR OP : 2004.06.16
#define		DEVNM_TTU					("TTU")				// TTU

///////////////////////////////////////////////////////////////////////////////
// 함수처리결과 RETURN VALUE
// 디바이스 : -
///////////////////////////////////////////////////////////////////////////////
#define		R_NORMAL					0
#define		R_DOING						1
#define		R_ERROR						2
#define		R_TIMEOVER					3
#define		R_CANCEL					-1
#define		R_TIMEOUT					-2

///////////////////////////////////////////////////////////////////////////////
// 장치 디바이스 상태 정보
// 디바이스 : JPR / SPR / MCU / PBM / BRM / UCM
///////////////////////////////////////////////////////////////////////////////
#define		NORMAL						0					// 정상종료 
#define		DOING						1					// 현재 동작중
#define		DOWN						2					// 기기 장애
#define		NODEVICE					3					// 기기 미연결

///////////////////////////////////////////////////////////////////////////////
// SCREEN 장치 출력 정보 
// 디바이스 : SOP
///////////////////////////////////////////////////////////////////////////////
#define		SOP_SCREEN					("APCenterMovie")	// 화면출력
#define		SOP_TOP						("APLocalMovie")	// 화면상단 배너출력
#define		SOP_TIME					("APMaxTime")		// 입력대기시간 
#define		SOP_STRING					("APValue")			// 문자열 출력
#define		SOP_BTNONOFF				("APState")			// 버튼 ON-OFF값 출력

#define		SOP_UPDATE					("APUpdate")		// 전체문자열 재출력
#define		SOP_UPDATE_SCREEN			("SCREEN")			// 전체문자열 재출력
#define		SOP_UPDATE_MESSAGE			("MESSAGE")			// 메세지     재출력

#define		SOP_PROGRESS				("APProgress")		// PROCESS=진행,FINISH=종료
#define		SOP_PROGRESS_PROCESS		("PROCESS")			// 진행 깜빡임
#define		SOP_PROGRESS_FINISH			("FINISH")			// 종료 깜빡임

///////////////////////////////////////////////////////////////////////////////
// 타임아웃 설정용 시간정보(초단위)
// 디바이스 : -
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
#define		CHK_NORMAL					("NORMAL")			// 정상처리
#define		CHK_TIMEOUT					("TIMEOUT")			// 매체 미수취
#define		CHK_TIMEOVER				("TIMEOVER")		// 장치 무응답
#define		CHK_ERROR					("ERROR")			// 처리중 장애발생

///////////////////////////////////////////////////////////////////////////////
// SCANDEVICEACTION설정 값관련
///////////////////////////////////////////////////////////////////////////////
#define		EVENT_ALL					0					// 전체이벤트 확인
#define		EVENT_IN					1					// 매체입력이벤트 확인
#define		EVENT_OUT					2					// 매체방출이벤트 확인

///////////////////////////////////////////////////////////////////////////////
// MACRO DEFINITION
///////////////////////////////////////////////////////////////////////////////
//1. 문자열이 "ON"/"OFF"인지 확인
#define		MAC_IS_SZONOFF(sFlag)		((strcmp(sFlag, "ON" ) == 0) ? TRUE	:		\
										 (strcmp(sFlag, "OFF") == 0) ? TRUE	: FALSE )
#endif // _CONST_DEF_H_
