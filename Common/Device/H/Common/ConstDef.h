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
// - 본파일은 MWI ActiveX로 부터 제공된 장치제어 관련 상수 Definition Header
//   file 입니다.
// - 파일내용중 선언된 상수에 대한 변경/수정/삭제/추가 등의 내용이 필요한 경우
//   본파일을 직접 수정하는 것 이외에 MWI담당자와의 협의후 변경하여 주십시오.
// 
// - 본파일을 사용함에 있어 문제는 없으나 다음과 같은 사양을 권장합니다.
//   기종 : T1ATM With KALIGNITE
//                                                       [노틸러스효성 SE본부]
//
// ----------------------------------------------------------------------------
// HISTORY OF [ConstDef.h] 
// ----------------------------------------------------------------------------
//
// - 2003.06.18	teo		MWI에서 사용하는 값(Return Value/Parameter)을 바탕으로
//						초기 파일 작성
//
// - 2003.09.25	teo		저널부 용지상태 값 수정(0x10->0x01)
//
// - 2003.09.30	teo		적외선부/지문부 추가로 인한 값 변경
//
// - 2003.10.30	HJH		NODEVICE 관련추가
//
// - 2003.11.13	teo		DES장치 추가로 인한 값 추가(장치ID/NAME, DES_TYPE등)
//
// - 2003.01.09	teo		UCM 출금부 센서정보 변경(SP사양파악 미숙으로 인한 오류)
//
// - 2004.01.15	teo		CDU장치 추가로 인한 값 추가(장치ID/NAME 등)
//
// - 2004.01.20	teo		CDU장치 추가로 인한 값 추가(CDU관련 값)
//
// - 2005.06.XX	teo		CIM장치추가
//
// ----------------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// DEIVCE IDENTIFIER : ID
///////////////////////////////////////////////////////////////////////////////
#define		DEV_NONE					0x00000000			// 장치없음
#define		DEV_SPR						0x00000001			// 명세표부
#define		DEV_JPR						0x00000002			// 페이퍼저널부
#define		DEV_MCU						0x00000004			// 카드부
#define		DEV_BRM						0x00000008			// 현금입출금부
#define		DEV_CSH						(DEV_CDU | DEV_CIM)	// 현금입출금부 V05.01.01

#define		DEV_CAM						0x00000010			// 수표입출금부
#define		DEV_PBM						0x00000020			// 통장부
#define		DEV_SCR						0x00000040			// SCREEN : FLASH ACTIVEX

#define		DEV_CDU						0x00000100			// CDU			// 2004.01.15_1.2_2003
#define		DEV_PIN						0x00000200			// PIN			// 2004.01.15_1.2_2003

#define		DEV_DOR						0x00001000			// 뒷문
#define		DEV_UPS						0x00002000			// 보조전원장치
#define		DEV_SIU						0x00004000			// GUIDELIGHT & INDICATOR			
#define		DEV_LGT						DEV_SIU
#define		DEV_SPL						0x00008000			// 7SEG/LED/INFORM/RPL

#define		DEV_CMR						0x00020000			// USB 2 CAMERA
#define		DEV_IRD						0x00040000			// 적외선		// 20030930_1.0_1081
#define		DEV_FNG						0x00080000			// 지문			// 20030930_1.0_1081

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
#define		DEVNM_NONE					("NONE")			// 장치없음
#define		DEVNM_MCU					("MCU")				// 카드부
#define		DEVNM_SPR					("SPR")				// 명세표부
#define		DEVNM_JPR					("JPR")				// 페이퍼저널부
#define		DEVNM_PBM					("PBM")				// 통장부
#define		DEVNM_BRM					("BRM")				// 현금입출금부

#define		DEVNM_CDU					("CDU")				// 현금/수표출금부 2004.01.15_1.2_2003
#define		DEVNM_PIN					("PINPAD")			// PINPAD 2004.01.15_1.2_2003

#define		DEVNM_DOR					("RDOOR")			// 뒷문
#define		DEVNM_UPS					("UPS")				// 보조전원장치
#define		DEVNM_SIU					("SIU")				// SENSOR & INDICATOR			
#define		DEVNM_SPL					("SPL")				// 7SEG/LED/INFORM/RPL
#define		DEVNM_NET					("NET")				// NETWORK
#define		DEVNM_CMR					("CAMERA")			// USB 2 CAMERA
#define		DEVNM_SCR					("SCR")				// SCREEN

#define		DEVNM_FNG					("FINGER")			// 지문부		20030930_1.0_1081
#define		DEVNM_IRD					("IRDA")			// 적외선부		20030930_1.0_1081
#define		DEVNM_BAR					("BARCODE")			// BarCode 		2003.11.20
#define		DEVNM_SNS					("SENSOR")			// Sensor		// 2004.04.14
#define		DEVNM_VFD					("VFD")				// VFD	 		2004.12.24
#define		DEVNM_CIM					("CIM")				// 현금입금부   V05.02.00
#define		DEVNM_RIC					("RIC")				// RIC	 		2008.06.24

#define		DEVNM_MAIN					("MAIN")			// 

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
// 저널 용지 잔량상태
// 디바이스 : JPR
///////////////////////////////////////////////////////////////////////////////
#define		JNL_NORMAL					0x00				// 저널 용지 정상
#define		JNL_EMPTY_PAPER				0x01				// 저널 용지 요보충		// 20030925_1.0_1078
#define		JNL_LOW_END					0x02				// 저널 용지 잔량소		// 20030925_1.0_1078

///////////////////////////////////////////////////////////////////////////////
// 매체 잔류상태
// 디바이스 : JPR / SPR / MCU / PBM
///////////////////////////////////////////////////////////////////////////////
#define		ST_NOT_DETECT				0x00				// 매체 미검지
#define		ST_SENSOR1					0x01				// 입구부(투입구) 매체검지
#define		ST_SENSOR2					0x02				// 경로상 매체검지

///////////////////////////////////////////////////////////////////////////////
// IC POWER 동작 종류
// 디바이스 : MCU
///////////////////////////////////////////////////////////////////////////////
#define		IC_ACT_COLD_RESET			1					// ACTION COLD RESET
#define		IC_ACT_WARM_RESET			2					// ACTION WARM RESET
#define		IC_ACT_POWER_OFF			3					// ACTION POWER OFF

///////////////////////////////////////////////////////////////////////////////
// IC POWER 상태
// 디바이스 : MCU
///////////////////////////////////////////////////////////////////////////////
#define		IC_POWER_ONLINE				0					// STATUS POWER ONLINE
#define		IC_POWER_POWEREDOFF			1					// STATUS POWER OFF
#define		IC_POWER_BUSY				2					// STATUS POWER BUSY
#define		IC_POWER_NODEVICE			3					// STATUS POWER NO DEVICE
#define		IC_POWER_HWERROR			4					// STATUS POWER HW ERROR
#define		IC_POWER_NOCARD				5					// STATUS POWER NO CARD
#define		IC_POWER_NOTSUPP			6					// STATUS POWER NOT SUPP

///////////////////////////////////////////////////////////////////////////////
// 명세표 용지 잔량상태
// 디바이스 : SPR
///////////////////////////////////////////////////////////////////////////////
#define		SLIP_NORMAL					0x00				// 명세표 용지 정상
#define		SLIP_EMPTY_PAPER			0x01				// 명세표 용지 요보충
#define		SLIP_LOW_END				0x02				// 명세표 용지 잔량소

///////////////////////////////////////////////////////////////////////////////
// 장치디바이스 장찰탁 정보
// 디바이스 : UCM / BRM
///////////////////////////////////////////////////////////////////////////////
#define		NORMAL_POS					0					// 정상위치
#define		ABNORMAL_POS				1					// 탈착

///////////////////////////////////////////////////////////////////////////////
// 장치디바이스 카세트 상태정보
// 디바이스 : UCM / BRM
///////////////////////////////////////////////////////////////////////////////
#define		CST_NORMAL					0					// 정상
#define		CST_NEAR					1					// CASSETTE NEAR END
#define		CST_EMPTY					2					// CASSETTE EMPTY
#define		CST_SET_NG					3					// SET NG
#define		CST_FULL					4					// CASSETTE FULL
#define		CST_ERROR					5					// CASSETTE ERROR
#define		CST_MANIP					6					// CASSETTE MANIP
#define		CST_HIGH					7					// CASSETTE HIGH

///////////////////////////////////////////////////////////////////////////////
// 장치디바이스 DISPENSER 장치구분
// 디바이스 : CDU
// 2004.01.20_1.2_2003 
// 20040315 JUNXI CHINA CDU 수정
///////////////////////////////////////////////////////////////////////////////
#define		DISPENSER_NOT_SET			0					// 현금출금부없음
#define		DISPENSER_1CASH				1					// 1 CST	2004.11.05 
#define		DISPENSER_2CASH				2					// 2 CST 
#define		DISPENSER_3CASH				3					// 3 CST
#define		DISPENSER_4CASH				4					// 4 CST
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// 장치디바이스 카세트 상태정보
// 디바이스 : CDU / BRM
// 2004.01.19_1.2_2003 장치정보추가
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
// 출금가능 여부 
// 디바이스 : CDU / BRM 
// 중국기종 대비 현지화 2004.03.31
///////////////////////////////////////////////////////////////////////////////
#define		WITH_NOT_AVAILABLE			0					// 출금 불가능
#define		WITH_TYPE1_ONLY				1					// 100/20원만 출금 가능
#define		WITH_TYPE2_ONLY				2					// 50/10원만 출금 가능
#define		WITH_BOTH_AVAILABLE			(WITH_TYPE1_ONLY | WITH_TYPE2_ONLY)
															// 100/20원 + 50/10원 출금 가능

///////////////////////////////////////////////////////////////////////////////
// 입금가능 여부 
// 디바이스 : CDU / BRM 
// 중국기종 대비 현지화 2004.03.31
///////////////////////////////////////////////////////////////////////////////
#define		DEP_NOT_AVAILABLE			0					// 입금 불가능
#define		DEP_TYPE100_ONLY			1					// 100원만 입금 가능
#define		DEP_TYPE50_ONLY				2					// 50원만 입금 가능
#define		DEP_TYPE20_ONLY				4					// 20원만 입금 가능
#define		DEP_TYPE10_ONLY				8					// 10원만 입금 가능
#define		DEP_ALL_AVAILABLE			(DEP_TYPE100_ONLY | DEP_TYPE50_ONLY | DEP_TYPE20_ONLY | DEP_TYPE10_ONLY)
															// 100/50/20/10원 입금 가능
///////////////////////////////////////////////////////////////////////////////
// 출금가능 여부 
// 디바이스 : UCM / BRM
///////////////////////////////////////////////////////////////////////////////
#define		WITHX_NOT_AVAILABLE			0					// 출금 불가능
#define		WITHX_CASH_ONLY				1					// 만원만 출금 가능
#define		WITHX_CHECK_ONLY			2					// 수표만 출금 가능
#define		WITHX_BOTH_AVAILABLE		3					// 만원 + 수표 출금 가능

///////////////////////////////////////////////////////////////////////////////
// 입금가능 여부 
// 디바이스 : UCM / BRM
///////////////////////////////////////////////////////////////////////////////
#define		DEPX_NOT_AVAILABLE			0					// 입금 불가능
#define		DEPX_CASH_ONLY				1					// 현금만 입금가능
#define		DEPX_CHECK_ONLY				2					// 수표만 입금가능
#define		DEPX_BOTH_AVAILABLE			3					// 현금+수표 입금가능
#define		DEPX_ONLY_TEN_TH			4					// 만원권만 입금가능

///////////////////////////////////////////////////////////////////////////////
// 장치 디바이스 이전탈착 정보
// 디바이스 : BRM
///////////////////////////////////////////////////////////////////////////////
#define		NO_CHANGE					0					// 이전탈착없음
#define		CHANGE_POS					1					// 이전탈착

///////////////////////////////////////////////////////////////////////////////
// 자동보충/회수 정보
// 디바이스 : BRM 
///////////////////////////////////////////////////////////////////////////////
#define		NEED_NONE					0					// 그대로
#define		NEED_REFILL					1					// 보충필요
#define		NEED_TAKEUP					2					// 회수필요

///////////////////////////////////////////////////////////////////////////////
// 현금 잔류 정보
// 디바이스 : BRM 
///////////////////////////////////////////////////////////////////////////////
#define		BRM_CLEAR					0x00				// 지폐 없음
#define		BRM_IN_CSM					0x01				// 현금 투입구에 지폐 있음
#define		BRM_IN_RJSD					0x02				// 반환 스태커에 지폐 있음
#define		BRM_IN_NSTK					0x04				// 일시 스태커에 지폐 있음
#define		BRM_ON_PATH					0x08				// 지폐 이동 경로상에 지폐 있음
#define		IN_CSM						BRM_IN_CSM
#define		IN_RJSD						BRM_IN_RJSD
#define		IN_NSTK						BRM_IN_NSTK
#define		IN_PATH						BRM_ON_PATH

///////////////////////////////////////////////////////////////////////////////
// 현금 입금상태 정보
// 디바이스 : BRM 
// 비고     : 0~2의 값은 [장치 디바이스 상태 정보]를 사용한다.
///////////////////////////////////////////////////////////////////////////////
#define		NO_MONEY					3					// 계수할 지폐 없음
#define		SKEW_NOTE					4					// 계수중 SKEW 발생
#define		MAX_OVER					5					// 입금 최대 매수 초과
#define		UNKNOWN_MONEY				6					// 감별되지 않은 지폐 있음

///////////////////////////////////////////////////////////////////////////////
// 현금 수취상태 정보
// 디바이스 : BRM 
///////////////////////////////////////////////////////////////////////////////
#define		GOT_MONEY					0					// 현금 수취
#define		WAIT_USER					1					// 현금 미수취

///////////////////////////////////////////////////////////////////////////////
// 현금 다권종 권종정보
// 디바이스 : CDU
// 중국기종 현지화	2004.03.31 
///////////////////////////////////////////////////////////////////////////////
#define		CASH_CHINA_100				100					// 100원권
#define		CASH_CHINA_50				50					// 50원권
#define		CASH_CHINA_20				20					// 20원권
#define		CASH_CHINA_10				10					// 10원권
#define		CASH_CHINA_5				5					//  5원권
#define		CASH_CHINA_1				1					//  1원권
#define		CASH_CHINA_UNKNOWN			0					// UNKNOWN

///////////////////////////////////////////////////////////////////////////////
// 출금카세트 현금 모드정도
// 디바이스 : CDU
// 중국기종 현지화	2004.03.31 
///////////////////////////////////////////////////////////////////////////////
#define		CASH_TYPE_NOTSET			0
#define		CASH_TYPE_BILL				1
#define		CASH_TYPE_RECYCLE			2

///////////////////////////////////////////////////////////////////////////////
// 입금카세트 현금 모드정도
// 디바이스 : CDU
// 중국기종 현지화	2004.03.31 
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
// 뒷문 열림상태 정보
// 디바이스 : DOR [RearDoor]
///////////////////////////////////////////////////////////////////////////////
#define		DOOR_OPENED					0					// 뒷문열림
#define		DOOR_CLOSED					1					// 뒷문닫힘

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
// PINPAD 입력모드 설정
// 디바이스 : PINPAD
// 중국기종 현지화	2004.04.02 
///////////////////////////////////////////////////////////////////////////////
#define		PINMODE_NOT_USE				0					// 미사용처리
#define		PINMODE_NORMAL				1					// Normal 입력모드
#define		PINMODE_EPP					2					// Epp 입력모드

///////////////////////////////////////////////////////////////////////////////
// FLICKER INDEX 정보
// 디바이스 : LGT [LIGNT-FLICKER]
///////////////////////////////////////////////////////////////////////////////
#define		BRM_FLICKER					0x01				// 지폐 입금/방출구
#define		PBM_FLICKER					0x02				// 통장 투입/방출구
#define		SPR_FLICKER					0x04				// 명세표 방출구
#define		MCU_FLICKER					0x08				// 카드 투입/방출구(명세표와동일)	중국나노요청(04->08) : 2004.04.09
#define		IC_FLICKER					0x08				// IC 카드 투입/방출구				중국나노요청(04->08) : 2004.04.09
#define		CDU_FLICKER					0x11				// CDU 지폐방출구					중국나노요청(09->11) : 2004.04.09
#define		PIN_FLICKER					0x20				// PIN 입력(NC3000)					중국나노요청(10->20) : 2004.04.09
#define		ALL_FLICKER					(BRM_FLICKER | PBM_FLICKER | SPR_FLICKER |	\
										 MCU_FLICKER | IC_FLICKER  |				\
										 CDU_FLICKER | PIN_FLICKER)
										// 모든 FLICKER

///////////////////////////////////////////////////////////////////////////////
// FLICKER 조작 정보
// 디바이스 : LGT [LIGNT-FLICKER]
///////////////////////////////////////////////////////////////////////////////
#define		FLICKER_OFF					0					// FLICKER OFF
#define		FLICKER_ON					1					// FLICKER ON

///////////////////////////////////////////////////////////////////////////////
// INDICATOR(거래표시등) 조작 정보
// 디바이스 : LGT [LIGNT-INDICATOR]
///////////////////////////////////////////////////////////////////////////////
#define		IND_TRANENABLE				0x01				// 거래가능	켬
#define		IND_TRANREADY				0x02				// 준비중(거래불가)	켬
#define		IND_CASH					0x04				// 현금	켬
#define		IND_CHECK		 			0x08				// 수표	켬
#define		IND_DEPOSIT		 			0x10				// 입금	켬
#define		IND_WITHDRAW	 			0x20				// 출금	켬
#define		IND_TRANSFER	 			0x40				// 이체	켬
#define		IND_PASSBOOK	 			0x80				// 통장	켬
#define		IND_ALL						0xff				// 취급표시등 모두다	켬
#define		IND_TRANALL	 		 		(IND_ALL & ~IND_TRANENABLE & ~IND_TRANREADY)
															// 거래가능/준비중을 제외한 취급표시등	켬
#define		IND_ALLOFF	 				0x00				// 취급표시등 모두다 끔
#define		IND_TRANALLOFF				(IND_TRANENABLE | IND_TRANREADY)
															// 거래가능/준비중을 제외한 취급표시등 끔
///////////////////////////////////////////////////////////////////////////////
// SPL 장치 정보 
// 디바이스 : SPL [7SEG/LED/RPL/INFORM]
// 비고     : 하단의 스펙은 T1ATM을 기준으로 작성되었음
///////////////////////////////////////////////////////////////////////////////
//       < SPL::LED >
//		┌────────┬────────────────────┐
//		│	  동작 상태   │               경고 / 이상부위          │
//		├────────┼────────────────────┤
//		│  취 급 중[0]   │    카    드[4]         명 세 표[8]     │
//		│  경    고[1]   │    통    장[5]         저    널[9]     │
//		│  이    상[2]   │    지    폐[6]         입금수표[10]    │
//		│  DUMMY   [3]   │    출금수표[7]         회 수 함[11]    │
//		└────────┴────────────────────┘
//		
//       < SPL::RPL >
//		┌────────┬────────────────────┐
//		│	  INDEX       │               CONTENTS                 │
//		├────────┼────────────────────┤
//		│       [0]      │    NORMAL  - 취급중                    │
//		│       [1]      │    CASH    - 현금                      │ 
//		│       [2]      │    LOSS    - 분실                      │
//		│       [3]      │    ALARM   - 알람                      │
//		│       [4]      │    RECEIPT - 명세표                    │
//		│       [5]      │    CALL    - 호출                      │
//		└────────┴────────────────────┘
//		  
//       < SPL::INFORM >
//		┌────────┬────────────────────┐
//		│	  INDEX       │               CONTENTS                 │
//		├────────┼────────────────────┤
//		│       [0]      │    LOSS    - 분실                      │
//		│       [1]      │    CASH    - 현금                      │ 
//		│       [2]      │    ALARM   - 알람                      │
//		│       [3]      │    RECEIPT - 명세표                    │
//		└────────┴────────────────────┘
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// LED 조작 정보 
// 디바이스 : LED
///////////////////////////////////////////////////////////////////////////////
#define		ON_USING_LED				1					// 취급중	(T1ATM용)
#define		ALERT_LED					2					// 경고		(T1ATM용)
#define		ABNORMAL_LED				3					// 이상		(T1ATM용)
#define		DUMMY_LED					4					// DUMMY	(T1ATM용)
#define		CARD_LED					5					// 카드		(T1ATM용)
#define		PASSBOOK_LED				6					// 통장		(T1ATM용)
#define		CASH_LED					7					// 지폐		(T1ATM용)
#define		OUT_CHECK_LED				8					// 출금수표	(T1ATM용)
#define		SLIP_LED					9					// 명세표	(T1ATM용)
#define		JNL_LED						10					// 저널		(T1ATM용)
#define		IN_CHECK_LED				11					// 입금수표	(T1ATM용)
#define		RJB_LED						12					// 회수함	(T1ATM용)

///////////////////////////////////////////////////////////////////////////////
// LED 조작 정보 
// 디바이스 : LED
///////////////////////////////////////////////////////////////////////////////
#define		LED_OFF						0					// LED OFF
#define		LED_ON						1					// LED ON

///////////////////////////////////////////////////////////////////////////////
// MCD4 LED 조작 정보 
// 디바이스 : LED
///////////////////////////////////////////////////////////////////////////////
#define		LED_STR_OFF					("OFF")				// LED STR OFF
#define		LED_STR_ON					("ON")				// LED STR ON

///////////////////////////////////////////////////////////////////////////////
// UPS 장치 상태 정보 
// 디바이스 : UPS
///////////////////////////////////////////////////////////////////////////////
#define		POWER_NORMAL				0x00				// POWER정상
#define		EXT_AC_OFF					0x01				// 정전또는AC오프
#define		INT_BATTERY_LOW				0x02				// 밧데리LOW
#define		CLERK_POWER_OFF				0x04				// 계원전원오프
#define		POWER_ABNORMAL				0x08				// UPS상태불량
#define		POWER_DOWN_GOOD				0x10				// POWER DOWN 정상처리

///////////////////////////////////////////////////////////////////////////////
// SCREEN 장치 출력 정보 
// 디바이스 : SCR
///////////////////////////////////////////////////////////////////////////////
#define		SCR_SCREEN					("APCenterMovie")	// 화면출력
#define		SCR_TOP						("APLocalMovie")	// 화면상단 배너출력
#define		SCR_TIME					("APMaxTime")		// 입력대기시간 
#define		SCR_STRING					("APValue")			// 문자열 출력
#define		SCR_BTNONOFF				("APState")			// 버튼 ON-OFF값 출력

#define		SCR_UPDATE					("APUpdate")		// 전체문자열 재출력
#define		SCR_UPDATE_SCREEN			("SCREEN")			// 전체문자열 재출력
#define		SCR_UPDATE_MESSAGE			("MESSAGE")			// 메세지     재출력

#define		SCR_PROGRESS				("APProgress")		// PROCESS=진행,FINISH=종료
#define		SCR_PROGRESS_PROCESS		("PROCESS")			// 진행 깜빡임
#define		SCR_PROGRESS_FINISH			("FINISH")			// 종료 깜빡임

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
#define		K_180_WAIT					180
#define		K_360_WAIT					360
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
// OPERATOR SWITCH설정 값관련
///////////////////////////////////////////////////////////////////////////////
#define		OPER_NONE					0					// NONE
#define		OPER_SUPERVISOR				1					// SUPERVISOR
#define		OPER_RUN					2					// RUN
#define		OPER_SUPERVISORCHANGED		11					// SUPERVISOR CHANGED
#define		OPER_RUNCHANGED				12					// RUN CHANGED

///////////////////////////////////////////////////////////////////////////////
// MACRO DEFINITION
///////////////////////////////////////////////////////////////////////////////
//1. 문자열이 "ON"/"OFF"인지 확인
#define		MAC_IS_SZONOFF(sFlag)		((strcmp(sFlag, "ON" ) == 0) ? TRUE	:		\
										 (strcmp(sFlag, "OFF") == 0) ? TRUE	: FALSE )
#endif // _CONST_DEF_H_
