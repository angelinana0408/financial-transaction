/////////////////////////////////////////////////////////////////////////////
#if !defined(_RearApScrChn_H_)
#define _RearApScrChn_H_

/////////////////////////////////////////////////////////////////////////////
//	Guide Scr Table
/////////////////////////////////////////////////////////////////////////////
// Status Information
	// OP_STATUS
		// 0:ATM STATUS(INIT, ERROR, CLERK, READY, CUSTOM, TRAN, DOWN)
		// 1:OPEN STATUS(ON,OFF)
		// 2:SEND RECV STATUS(ON,OFF)
		// 3:TRANSACTION STATUS(ON,OFF)
		// 4:POWER STATUS(ON,OFF)
		// 5:AS PASSWORD
		// 6:OP PASSWORD
		// 7:SCR BANK NAME
		// 8:MACHINE KIND(NANOCASH, MCD3, TTW, MCD4, TTW2, EATMGBA1, EATMULL1, EATMTTW, TTW2E, MCD4E, MX8000T, MX8000L, MX5600T, MX5600L)
		// 9:OPERATOR SWITCH STATUS(NONE,SUPERVISOR,RUN,SUPERVISORCHANGED,RUNCHANGED)
		// 10:FTP IP(NNNNNNNNNNNN)
		// 11:FTP PORT(NNNNN)
		// 12:ATM SERIAL NUMBER(NNNNNNNN)
		// 13:BRANCH NUMBER(NNNNNNNN)
		// 14:AP VERSION(V00-00-00)
		// 15:TRANSACTION DATE(YYYYMMDD)
		// 16:INTERFACE KIND(N)
		// 17:OP DEVICE(NOT_SET, OP_TOUCH, OP_TSPL, OP_HSPL)

// Header Information			
	// OP_HEADER
		// 0:MACHINE KIND(NANOCASH, MCD3, TTW, MCD4, TTW2, EATMGBA1, EATMULL1, EATMTTW, TTW2E, MCD4E, MX8000T, MX8000L, MX5600T, MX5600L)
		// 1:ATM STATUS(INIT, ERROR, CLERK, READY, CUSTOM, TRAN, DOWN)
		// 2:DOOR STATUS(DOOROPEN, DOORCLOSE)
		// 3:LINE STATUS(HOSTONLINE, HOSTOFFLINE)
		// 4:MESSAGE
		// 5:ERROR CODE([P] - [EEEEE(SS)])
		// 6:CST1(CCCC(GGG))
		// 7:CST2(CCCC(GGG))
		// 8:CST3(CCCC(GGG))
		// 9:CST4(CCCC(GGG))
		// 10:CDU(NORMAL, ERROR)
		// 11:MCU(NORMAL, ERROR)
		// 12:SPR(NORMAL, ERROR)
		// 13:JPR(NORMAL, ERROR)
		// 14:CST1(MISSING, NORMAL, LOW, EMPTY, FULL)
		// 15:CST2(MISSING, NORMAL, LOW, EMPTY, FULL)
		// 16:CST3(MISSING, NORMAL, LOW, EMPTY, FULL)
		// 17:CST4(MISSING, NORMAL, LOW, EMPTY, FULL)
		// 18:REJECT(MISSING, NORMAL, LOW, EMPTY, FULL)
		// 19:MCU(C)
		// 20:SPR(MISSING, NORMAL, LOW, EMPTY)
		// 21:JPR(MISSING, NORMAL, LOW, EMPTY)
		// 22:ATM SERIAL NUMBER(NNNNNNNN)
		// 23:BRANCH NUMBER(NNNNNNNN)
		// 24:AP VERSION(V00-00-00)
		// 25:TRANSACTION DATE(YYYYMMDD)

// ApUpdate Information 
	// OP_APUPDATE
		// 0:AP VERSION(V00-00-00)
		// 1:TRANSACTION DATE(YYYYMMDD)

// TraceCopy Information 
	// OP_TRACECOPY
		// 0:AP VERSION(V00-00-00)
		// 1:TRANSACTION DATE(YYYYMMDD)
		// 2:TRACE DATE(YYYYMMDD)

// Version Information
	// OP_VERSION
		// 0:AP VERSION(V00-00-00)
		// 1:TRANSACTION DATE(YYYYMMDD)
		// 2:CDU SP VERSION
		// 3:JPR SP VERSION
		// 4:SPR SP VERSION
		// 5:MCU SP VERSION
		// 6:PIN SP VERSION
		// 7:CAM SP VERSION
		// 8:SIU SP VERSION
		// 9:UPS SP VERSION
		// 10:TTU SP VERSION
		// 11:MWI SP VERSION
		// 12:COMMONITOR SP VERSION
		// 13:CDU EP VERSION
		// 14:JPR EP VERSION
		// 15:SPR EP VERSION
		// 16:MCU EP VERSION
		// 17:PIN EP VERSION
		// 18:CAM EP VERSION
		// 19:SIU EP VERSION
		// 20:UPS EP VERSION
		// 21:TTU EP VERSION

// WorkParam Information
	// OP_WORKPARAM
		// 0:ATM SERIAL NUMBER(NNNNNNNN)
		// 1:BRANCH NUMBER(NNNNNNNN)
		// 2:RESERVED NUMBER1(NNNNNNNN)
		// 3:RESERVED NUMBER2(NNNNNNNN)
		// 4:HOST IP(NNNnnnNNNnnn)
		// 5:HOST PORT(NNNNN)
		// 6:HOST IP2(NNNnnnNNNnnn)
		// 7:HOST PORT2(NNNNN)
		// 8:ATM IP(NNNnnnNNNnnn)
		// 9:ATM SUBNET MASK(NNNnnnNNNnnn)
		// 10:ATM GATE WAY(NNNnnnNNNnnn)
		// 11:ATM PORT(NNNNN)
		// 12:HOST IP3(NNNnnnNNNnnn) : BID
		// 13:HOST PORT3(NNNNN) : BID
		// 14:HOST IP4(NNNnnnNNNnnn) : FTP
		// 15:HOST PORT4(NNNNN) : FTP

// AddCash Information
	// OP_ADDCASH
		// 0:CST1(CCCC(GGG))
		// 1:CST2(CCCC(GGG))
		// 2:CST3(CCCC(GGG))
		// 3:CST4(CCCC(GGG))
		// 4:CDU(NORMAL, ERROR)
		// 5:MCU(NORMAL, ERROR)
		// 6:SPR(NORMAL, ERROR)
		// 7:JPR(NORMAL, ERROR)
		// 8:CST1(VVV)
		// 9:CST1(CCCC)
		// 10:CST1(AAAAAA)
		// 11:CST2(VVV)
		// 12:CST2(CCCC)
		// 13:CST2(AAAAAA)
		// 14:CST3(VVV)
		// 15:CST3(CCCC)
		// 16:CST3(AAAAAA)
		// 17:CST4(VVV)
		// 18:CST4(CCCC)
		// 19:CST4(AAAAAA)

// EjmView Information
	// OP_EJMVIEW
		// 0:TRANSACTION DATE(YYYYMMDD)
		// 1:SERIAL NO(NNNNNN)
		// 2:CARD NUMBER(X19)

// EjmResult Information
	// OP_EJMRESULT
		// 0-11:EJR DATA(X40)
		// 12-15:PIC NAME

// EjmCopy Information 
	// OP_EJMCOPY
		// 0:AP VERSION(V00-00-00)
		// 1:TRANSACTION DATE(YYYYMMDD)
		// 2:EJM START DATE(YYYYMMDD)
		// 3:EJM END DATE(YYYYMMDD)

// EjmSend Information 
	// OP_EJMSEND
		// 0:AP VERSION(V00-00-00)
		// 1:TRANSACTION DATE(YYYYMMDD)
		// 2:EJM START DATE(YYYYMMDD)
		// 3:EJM END DATE(YYYYMMDD)

// EjmPrint Information 
	// OP_EJMPRINT
		// 0:AP VERSION(V00-00-00)
		// 1:TRANSACTION DATE(YYYYMMDD)
		// 2:EJM START DATE(YYYYMMDD)
		// 3:EJM END DATE(YYYYMMDD)

// CashValue Information 
	// OP_CASHVALUE
		// 0:CST1(VVV)
		// 1:CST2(VVV)
		// 2:CST3(VVV)
		// 3:CST4(VVV)

// DbInitail Information 
	// OP_DBINITIAL
		// 0:AP VERSION(V00-00-00)
		// 1:TRANSACTION DATE(YYYYMMDD)

// Total Information
	// OP_TOTAL
		// 0:TRAN BATCH NO(NNNNNNNNNnNNNN)
		// 1:INPUT BATCH NO(NNNNNNNNNnNNNN)

// SetParam Information
	// OP_SETPARAM
		// 0:RESERVE1(NNNNNNNNNnNNNNNN)
		// 1:RESERVE2(NNNNNNNNNnNNNNNN)
		// 2:RESERVE3(NNNNNNNNNnNNNNNN)
		// 3:RESERVE4(NNNNNNNNNnNNNNNN)
		// 4:RECEIVE TIMEOUT(NNNN)
		// 5:IDLE RETRY TIME(NNNN)
		// 6:WITH MAX AMOUNT(NNNN)
		// 7:TRANS MAX AMOUNT(NNNNNNNNNnNN)
		// 8:HOST IP PAY(NNNnnnNNNnnn)
		// 9:HOST PORT PAY(NNNNN)

static GuideScrHeader GuideScrTblChn[] = {
		"",														// 0
		0,
		// SCREEN DISPLAY(NO,XX,YY,ATTRIBUTE,LENGTH,JUSTIFIER,FORMAT)
//		 0123456789012345678901234567890123456789
/*0*/	"                                        ",
/*1*/	"                                        ",
/*2*/	"                                        ",
/*3*/	"                                        ",
/*4*/	"                                        ",
/*5*/	"                                        ",
/*6*/	"                                        ",
/*7*/	"                                        ",
/*8*/	"                                        ",
/*9*/	"                                        ",
/*0*/	"                                        ",
/*1*/	"                                        ",
/*2*/	"                                        ",
/*3*/	"                                        ",
/*4*/	"                                        ",
/*5*/	"                                        ",
		// SCREEN DISPLAY(NO,XX,YY,ATTRIBUTE,LENGTH,JUSTIFIER,FORMAT)
		":"														/* 00: */	\
		"",
		// SCREEN ACCEPT(INDEX,NO,XX,YY,LENGTH,ECHOMODE,ATTRIBUTE,INPUTTYPE,CURSORON,FLUSH,AUTOEND,TIMEOUT)
		":"														/* 00: */	\
		"",

/////////////////////////////////////////////////////////////////////////////
		"OP_STATUS",											// 1
		900,
		// SCREEN DATA
//		 0123456789012345678901234567890123456789
/*0*/	" �����ͺ�..........  ȡ���״̬........ ",
/*1*/	" ������״̬........  ͨѸ״̬.......... ",
/*2*/	" ��Ϣ..............  ������............ ",
/*3*/	"----------------------------------------",
/*4*/	" ����1:9999(100)     ����2:9999(100)    ",
/*5*/	" ����3:9999(100)     ����4:9999(100)    ",
/*6*/	" ��  ��  ��:����     ��  ��  ��:����    ",
/*7*/	" ƾ����ӡ��:����     ��ˮ��ӡ��:����    ",
/*8*/	"----------------------------------------",
/*9*/	" ����1:����          ����2:����         ",
/*0*/	" ����3:����          ����4:����         ",
/*1*/	" ��  ��  ��:����     ��  ��  ��:999     ",
/*2*/	" ƾ����ӡ��:����     ��ˮ��ӡ��:����    ",
/*3*/	"----------------------------------------",
/*4*/	" �豸���:12345678   ���б��:12345678  ",
/*5*/	" AP�汾��:V01-01-01  ��    ��:2000.01.01",

		// SCREEN DISPLAY(NO,XX,YY,ATTRIBUTE,LENGTH,JUSTIFIER,FORMAT)
		"01,01,00,NORMAL,18,LEFT,:"								/* 01:MACHINE KIND(NANOCASH, MCD3, TTW, MCD4, TTW2, EATMGBA1, EATMULL1, EATMTTW, TTW2E, MCD4E, MX8000T, MX8000L, MX5600T, MX5600L) */	\
		"02,21,00,NORMAL,18,LEFT,:"								/* 02:ATM STATUS(INIT, ERROR, CLERK, READY, CUSTOM, TRAN, DOWN) */	\
		"03,01,01,NORMAL,18,LEFT,:"								/* 03:DOOR STATUS(DOOROPEN, DOORCLOSE) */	\
		"04,21,01,NORMAL,18,LEFT,:"								/* 04:LINE STATUS(HOSTONLINE, HOSTOFFLINE) */	\
		"05,01,02,NORMAL,18,LEFT,:"								/* 05:MESSAGE */	\
		"06,21,02,NORMAL,18,LEFT,:"								/* 06:ERROR CODE([P] - [EEEEE(SS)]) */	\

		"07,07,04,NORMAL,09,LEFT,:"								/* 07:CST1(CCCC(GGG)) */	\
		"08,27,04,NORMAL,09,LEFT,:"								/* 08:CST2(CCCC(GGG)) */	\
		"09,07,05,NORMAL,09,LEFT,:"								/* 09:CST3(CCCC(GGG)) */	\
		"10,27,05,NORMAL,09,LEFT,:"								/* 10:CST4(CCCC(GGG)) */	\
		"11,12,06,NORMAL,04,LEFT,:"								/* 11:CDU(NORMAL, AUTO-OFF, MISSING, ABNORMAL, ERROR, OFF) */	\
		"12,32,06,NORMAL,04,LEFT,:"								/* 12:MCU(NORMAL, AUTO-OFF, MISSING, ABNORMAL, ERROR, OFF) */	\
		"13,12,07,NORMAL,04,LEFT,:"								/* 13:SPR(NORMAL, AUTO-OFF, MISSING, ABNORMAL, ERROR, OFF) */	\
		"14,32,07,NORMAL,04,LEFT,:"								/* 14:JPR(NORMAL, AUTO-OFF, MISSING, ABNORMAL, ERROR, OFF) */	\

		"15,07,09,NORMAL,04,LEFT,:"								/* 15:CST1(MISSING, NORMAL, LOW, EMPTY, FULL) */	\
		"16,27,09,NORMAL,04,LEFT,:"								/* 16:CST2(MISSING, NORMAL, LOW, EMPTY, FULL) */	\
		"17,07,10,NORMAL,04,LEFT,:"								/* 17:CST3(MISSING, NORMAL, LOW, EMPTY, FULL) */	\
		"18,27,10,NORMAL,04,LEFT,:"								/* 18:CST4(MISSING, NORMAL, LOW, EMPTY, FULL) */	\
		"19,12,11,NORMAL,04,LEFT,:"								/* 19:REJECT(MISSING, NORMAL, LOW, EMPTY, FULL) */	\
		"20,32,11,NORMAL,03,LEFT,:"								/* 20:MCU(C) */	\
		"21,12,12,NORMAL,04,LEFT,:"								/* 21:SPR(MISSING, NORMAL, LOW, EMPTY) */	\
		"22,32,12,NORMAL,04,LEFT,:"								/* 22:JPR(MISSING, NORMAL, LOW, EMPTY) */	\

		"23,10,14,NORMAL,10,LEFT,:"								/* 23:ATM SERIAL NUMBER(NNNNNNNN) */	\
		"24,30,14,NORMAL,10,LEFT,:"								/* 24:BRANCH NUMBER(NNNNNNNN) */	\
		"25,10,15,NORMAL,10,LEFT,:"								/* 25:AP VERSION(V00-00-00) */	\
		"26,30,15,NORMAL,10,LEFT,####.##.##:"					/* 26:DATE(YYYYMMDD) */ \
		"",
		// SCREEN ACCEPT(INDEX,NO,XX,YY,LENGTH,ECHOMODE,ATTRIBUTE,INPUTTYPE,CURSORON,FLUSH,AUTOEND,TIMEOUT)
		"",

/////////////////////////////////////////////////////////////////////////////
		"OP_MODE",												// 2
		901,
		// SCREEN DATA
//		 0123456789012345678901234567890123456789
/*0*/	" ��Ϣ..............  ������............ ",
/*1*/	" ����1:9999(100)     ����2:9999(100)    ",
/*2*/	" ����3:9999(100)     ����4:9999(100)    ",
/*3*/	" ������:���� ������:���� ƾ����ӡ��:����",
/*4*/	" �ϳ���:���� �̿���:999  ��ˮ��ӡ��:����",
/*5*/	" �豸���:12345678   AP�汾��:V01-01-01 ",
/*6*/	"01)����״̬��Ϣ                         ",
/*7*/	"02)ҵ�����Ա�˵�   11)�򿪳�����       ",
/*8*/	"03)��������Ա�˵�   12)�رճ�����       ",
/*9*/	"                    --------------------",
/*0*/	"                    33)�豸����         ",
/*1*/	"                    44)�����豸����     ",
/*2*/	"                    55)����ά��ģʽ     ",
/*3*/	"08)��ӡ������Ϣ                         ",
/*4*/	"09)���ó������     99)�ػ�             ",
/*5*/	"                      ������������:NN ",
		// SCREEN DISPLAY(NO,XX,YY,ATTRIBUTE,LENGTH,JUSTIFIER,FORMAT)
		"01,00,00,NORMAL,00,LEFT,:"								/* 01:MACHINE KIND(NANOCASH, MCD3, TTW, MCD4, TTW2, EATMGBA1, EATMULL1, EATMTTW, TTW2E, MCD4E, MX8000T, MX8000L, MX5600T, MX5600L) */	\
		"02,00,00,NORMAL,00,LEFT,:"								/* 02:ATM STATUS(INIT, ERROR, CLERK, READY, CUSTOM, TRAN, DOWN) */	\
		"03,00,00,NORMAL,00,LEFT,:"								/* 03:DOOR STATUS(DOOROPEN, DOORCLOSE) */	\
		"04,00,00,NORMAL,00,LEFT,:"								/* 04:LINE STATUS(HOSTONLINE, HOSTOFFLINE) */	\
		"05,01,00,NORMAL,18,LEFT,:"								/* 05:MESSAGE */	\
		"06,21,00,NORMAL,18,LEFT,:"								/* 06:ERROR CODE([P] - [EEEEE(SS)]) */	\

		"07,07,01,NORMAL,09,LEFT,:"								/* 07:CST1(CCCC(GGG)) */	\
		"08,27,01,NORMAL,09,LEFT,:"								/* 08:CST2(CCCC(GGG)) */	\
		"09,07,02,NORMAL,09,LEFT,:"								/* 09:CST3(CCCC(GGG)) */	\
		"10,27,02,NORMAL,09,LEFT,:"								/* 10:CST4(CCCC(GGG)) */	\
		"11,08,03,NORMAL,04,LEFT,:"								/* 11:CDU(NORMAL, AUTO-OFF, MISSING, ABNORMAL, ERROR, OFF) */	\
		"12,20,03,NORMAL,04,LEFT,:"								/* 12:MCU(NORMAL, AUTO-OFF, MISSING, ABNORMAL, ERROR, OFF) */	\
		"13,36,03,NORMAL,04,LEFT,:"								/* 13:SPR(NORMAL, AUTO-OFF, MISSING, ABNORMAL, ERROR, OFF) */	\
		"14,36,04,NORMAL,04,LEFT,:"								/* 14:JPR(NORMAL, AUTO-OFF, MISSING, ABNORMAL, ERROR, OFF) */	\

		"15,00,00,NORMAL,00,LEFT,:"								/* 15:CST1(MISSING, NORMAL, LOW, EMPTY, FULL) */	\
		"16,00,00,NORMAL,00,LEFT,:"								/* 16:CST2(MISSING, NORMAL, LOW, EMPTY, FULL) */	\
		"17,00,00,NORMAL,00,LEFT,:"								/* 17:CST3(MISSING, NORMAL, LOW, EMPTY, FULL) */	\
		"18,00,00,NORMAL,00,LEFT,:"								/* 18:CST4(MISSING, NORMAL, LOW, EMPTY, FULL) */	\
		"19,08,04,NORMAL,04,LEFT,:"								/* 19:REJECT(MISSING, NORMAL, LOW, EMPTY, FULL) */	\
		"20,20,04,NORMAL,03,LEFT,:"								/* 20:MCU(C) */	\
		"21,00,00,NORMAL,00,LEFT,:"								/* 21:SPR(MISSING, NORMAL, LOW, EMPTY) */	\
		"22,00,00,NORMAL,00,LEFT,:"								/* 22:JPR(MISSING, NORMAL, LOW, EMPTY) */	\

		"23,10,05,NORMAL,10,LEFT,:"								/* 23:ATM SERIAL NUMBER(NNNNNNNN) */	\
		"24,00,00,NORMAL,00,LEFT,:"								/* 24:BRANCH NUMBER(NNNNNNNN) */	\
		"25,30,05,NORMAL,10,LEFT,:"								/* 25:AP VERSION(V00-00-00) */	\
		"26,00,00,NORMAL,00,LEFT,####.##.##:"					/* 26:TRANSACTION DATE(YYYYMMDD) */ \

		"27,37,15,NORMAL,02,LEFT,##:"							/* 27:MENU CHOICE */	\

		"48,00,15,NORMAL,20,LEFT,:"								/* 48:MESSAGE */	\
		"",
		// SCREEN ACCEPT(INDEX,NO,XX,YY,LENGTH,ECHOMODE,ATTRIBUTE,INPUTTYPE,CURSORON,FLUSH,AUTOEND,TIMEOUT)
		"0,27,37,15,02,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 27:MENU CHOICE */	\
		"",

/////////////////////////////////////////////////////////////////////////////
		"INPUT_PASSWORD",										// 3
		902,
		// SCREEN DATA
//		 0123456789012345678901234567890123456789
/*0*/	"                                        ",
/*1*/	"             ��������������             ",
/*2*/	"                                        ",
/*3*/	"             ��ENTER ������             ",
/*4*/	"                                        ",
/*5*/	"                [      ]                ",
/*6*/	"                                        ",
/*7*/	"                                        ",
/*8*/	"                                        ",
/*9*/	"                                        ",
/*0*/	"                                        ",
/*1*/	"                                        ",
/*2*/	"                                        ",
/*3*/	"                                        ",
/*4*/	"                                        ",
/*5*/	"                                        ",
		// SCREEN DISPLAY(NO,XX,YY,ATTRIBUTE,LENGTH,JUSTIFIER,FORMAT)
		"00,15,03,INVERTED,05,LEFT,ENTER:"						\

		"01,17,05,NORMAL,06,LEFT,******:"						/* 01:PASSWORD */	\

		"48,10,13,NORMAL,20,LEFT,:"								/* 48:MESSAGE */	\
		"",
		// SCREEN ACCEPT(INDEX,NO,XX,YY,LENGTH,ECHOMODE,ATTRIBUTE,INPUTTYPE,CURSORON,FLUSH,AUTOEND,TIMEOUT)
		"0,01,17,05,06,PASSWORD,NORMAL,REALNUMERIC,1,1,1,-1:"	/* 01:PASSWORD */	\
		"",

/////////////////////////////////////////////////////////////////////////////
		"INPUT_PASSWORD_AGAIN",									// 4
		903,
		// SCREEN DATA
//		 0123456789012345678901234567890123456789
/*0*/	"                                        ",
/*1*/	"             ���ٴ���������             ",
/*2*/	"                                        ",
/*3*/	"             ��ENTER ������             ",
/*4*/	"                                        ",
/*5*/	"                [      ]                ",
/*6*/	"                                        ",
/*7*/	"                                        ",
/*8*/	"                                        ",
/*9*/	"                                        ",
/*0*/	"                                        ",
/*1*/	"                                        ",
/*2*/	"                                        ",
/*3*/	"                                        ",
/*4*/	"                                        ",
/*5*/	"                                        ",
		// SCREEN DISPLAY(NO,XX,YY,ATTRIBUTE,LENGTH,JUSTIFIER,FORMAT)
		"00,15,03,INVERTED,05,LEFT,ENTER:"						\

		"01,17,05,NORMAL,06,LEFT,******:"						/* 01:PASSWORD */	\

		"48,10,13,NORMAL,20,LEFT,:"								/* 48:MESSAGE */	\
		"",
		// SCREEN ACCEPT(INDEX,NO,XX,YY,LENGTH,ECHOMODE,ATTRIBUTE,INPUTTYPE,CURSORON,FLUSH,AUTOEND,TIMEOUT)
		"0,01,17,05,06,PASSWORD,NORMAL,REALNUMERIC,1,1,1,-1:"	/* 01:PASSWORD */	\
		"",

/////////////////////////////////////////////////////////////////////////////
		"AS_MODE",												// 5
		904,
		// SCREEN DATA
//		 0123456789012345678901234567890123456789
/*0*/	" ����1:9999(100)     ����2:9999(100)    ",
/*1*/	" ����3:9999(100)     ����4:9999(100)    ",
/*2*/	" ������:���� ������:���� ƾ����ӡ��:����",
/*3*/	" �ϳ���:���� �̿���:999  ��ˮ��ӡ��:����",
/*4*/	" �豸���:12345678   AP�汾��:V01-01-01 ",
/*5*/	"                                        ",
/*6*/	"01)�޸�����         06)�汾��Ϣ         ",
/*7*/	"                                        ",
/*8*/	"02)����AP           07)��������         ",
/*9*/	"                                        ",
/*0*/	"03)������Ϣ         08)DB��ʼ��         ",
/*1*/	"                                        ",
/*2*/	"04)��־����         09)DB��ӡ           ",
/*3*/	"                                        ",
/*4*/	"05)�豸���         10)�˳�             ",
/*5*/	"                      ������������:NN ",
		// SCREEN DISPLAY(NO,XX,YY,ATTRIBUTE,LENGTH,JUSTIFIER,FORMAT)
		"01,07,00,NORMAL,09,LEFT,:"								/* 01:CST1(CCCC(GGG)) */	\
		"02,27,00,NORMAL,09,LEFT,:"								/* 02:CST2(CCCC(GGG)) */	\
		"03,07,01,NORMAL,09,LEFT,:"								/* 03:CST3(CCCC(GGG)) */	\
		"04,27,01,NORMAL,09,LEFT,:"								/* 04:CST4(CCCC(GGG)) */	\
		"05,08,02,NORMAL,04,LEFT,:"								/* 05:CDU(NORMAL, AUTO-OFF, MISSING, ABNORMAL, ERROR, OFF) */	\
		"06,20,02,NORMAL,04,LEFT,:"								/* 06:MCU(NORMAL, AUTO-OFF, MISSING, ABNORMAL, ERROR, OFF) */	\
		"07,36,02,NORMAL,04,LEFT,:"								/* 07:SPR(NORMAL, AUTO-OFF, MISSING, ABNORMAL, ERROR, OFF) */	\
		"08,36,03,NORMAL,04,LEFT,:"								/* 08:JPR(NORMAL, AUTO-OFF, MISSING, ABNORMAL, ERROR, OFF) */	\

		"09,00,00,NORMAL,00,LEFT,:"								/* 09:CST1(MISSING, NORMAL, LOW, EMPTY, FULL) */	\
		"10,00,00,NORMAL,00,LEFT,:"								/* 10:CST2(MISSING, NORMAL, LOW, EMPTY, FULL) */	\
		"11,00,00,NORMAL,00,LEFT,:"								/* 11:CST3(MISSING, NORMAL, LOW, EMPTY, FULL) */	\
		"12,00,00,NORMAL,00,LEFT,:"								/* 12:CST4(MISSING, NORMAL, LOW, EMPTY, FULL) */	\
		"13,08,03,NORMAL,04,LEFT,:"								/* 13:REJECT(MISSING, NORMAL, LOW, EMPTY, FULL) */	\
		"14,20,03,NORMAL,03,LEFT,:"								/* 14:MCU(C) */	\
		"15,00,00,NORMAL,00,LEFT,:"								/* 15:SPR(MISSING, NORMAL, LOW, EMPTY) */	\
		"16,00,00,NORMAL,00,LEFT,:"								/* 16:JPR(MISSING, NORMAL, LOW, EMPTY) */	\

		"17,10,04,NORMAL,10,LEFT,:"								/* 17:ATM SERIAL NUMBER(NNNNNNNN) */	\
		"18,00,00,NORMAL,00,LEFT,:"								/* 18:BRANCH NUMBER(NNNNNNNN) */	\
		"19,30,04,NORMAL,10,LEFT,:"								/* 19:AP VERSION(V00-00-00) */	\
		"20,00,00,NORMAL,00,LEFT,####.##.##:"					/* 20:TRANSACTION DATE(YYYYMMDD) */ \

		"21,37,15,NORMAL,02,LEFT,##:"							/* 21:MENU CHOICE */	\

		"48,00,15,NORMAL,20,LEFT,:"								/* 48:MESSAGE */	\
		"",
		// SCREEN ACCEPT(INDEX,NO,XX,YY,LENGTH,ECHOMODE,ATTRIBUTE,INPUTTYPE,CURSORON,FLUSH,AUTOEND,TIMEOUT)
		"0,21,37,15,02,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 21:MENU CHOICE */	\
		"",

/////////////////////////////////////////////////////////////////////////////
		"EJM",													// 6
		905,
		// SCREEN DATA
//		 0123456789012345678901234567890123456789
/*0*/	" ����1:9999(100)     ����2:9999(100)    ",
/*1*/	" ����3:9999(100)     ����4:9999(100)    ",
/*2*/	" ������:���� ������:���� ƾ����ӡ��:����",
/*3*/	" �ϳ���:���� �̿���:999  ��ˮ��ӡ��:����",
/*4*/	" �豸���:12345678   AP�汾��:V01-01-01 ",
/*5*/	"                                        ",
/*6*/	"01)��־���         05)��ˮ����         ",
/*7*/	"                                        ",
/*8*/	"02)��־����                             ",
/*9*/	"                                        ",
/*0*/	"03)��־����                             ",
/*1*/	"                                        ",
/*2*/	"04)��־��ӡ                             ",
/*3*/	"                                        ",
/*4*/	"                    10)�˳�             ",
/*5*/	"                      ������������:NN ",
		// SCREEN DISPLAY(NO,XX,YY,ATTRIBUTE,LENGTH,JUSTIFIER,FORMAT)
		"01,07,00,NORMAL,09,LEFT,:"								/* 01:CST1(CCCC(GGG)) */	\
		"02,27,00,NORMAL,09,LEFT,:"								/* 02:CST2(CCCC(GGG)) */	\
		"03,07,01,NORMAL,09,LEFT,:"								/* 03:CST3(CCCC(GGG)) */	\
		"04,27,01,NORMAL,09,LEFT,:"								/* 04:CST4(CCCC(GGG)) */	\
		"05,08,02,NORMAL,04,LEFT,:"								/* 05:CDU(NORMAL, AUTO-OFF, MISSING, ABNORMAL, ERROR, OFF) */	\
		"06,20,02,NORMAL,04,LEFT,:"								/* 06:MCU(NORMAL, AUTO-OFF, MISSING, ABNORMAL, ERROR, OFF) */	\
		"07,36,02,NORMAL,04,LEFT,:"								/* 07:SPR(NORMAL, AUTO-OFF, MISSING, ABNORMAL, ERROR, OFF) */	\
		"08,36,03,NORMAL,04,LEFT,:"								/* 08:JPR(NORMAL, AUTO-OFF, MISSING, ABNORMAL, ERROR, OFF) */	\

		"09,00,00,NORMAL,00,LEFT,:"								/* 09:CST1(MISSING, NORMAL, LOW, EMPTY, FULL) */	\
		"10,00,00,NORMAL,00,LEFT,:"								/* 10:CST2(MISSING, NORMAL, LOW, EMPTY, FULL) */	\
		"11,00,00,NORMAL,00,LEFT,:"								/* 11:CST3(MISSING, NORMAL, LOW, EMPTY, FULL) */	\
		"12,00,00,NORMAL,00,LEFT,:"								/* 12:CST4(MISSING, NORMAL, LOW, EMPTY, FULL) */	\
		"13,08,03,NORMAL,04,LEFT,:"								/* 13:REJECT(MISSING, NORMAL, LOW, EMPTY, FULL) */	\
		"14,20,03,NORMAL,03,LEFT,:"								/* 14:MCU(C) */	\
		"15,00,00,NORMAL,00,LEFT,:"								/* 15:SPR(MISSING, NORMAL, LOW, EMPTY) */	\
		"16,00,00,NORMAL,00,LEFT,:"								/* 16:JPR(MISSING, NORMAL, LOW, EMPTY) */	\

		"17,10,04,NORMAL,10,LEFT,:"								/* 17:ATM SERIAL NUMBER(NNNNNNNN) */	\
		"18,00,00,NORMAL,00,LEFT,:"								/* 18:BRANCH NUMBER(NNNNNNNN) */	\
		"19,30,04,NORMAL,10,LEFT,:"								/* 19:AP VERSION(V00-00-00) */	\
		"20,00,00,NORMAL,00,LEFT,####.##.##:"					/* 20:TRANSACTION DATE(YYYYMMDD) */ \

		"21,37,15,NORMAL,02,LEFT,##:"							/* 21:MENU CHOICE */	\

		"48,00,15,NORMAL,20,LEFT,:"								/* 48:MESSAGE */	\
		"",
		// SCREEN ACCEPT(INDEX,NO,XX,YY,LENGTH,ECHOMODE,ATTRIBUTE,INPUTTYPE,CURSORON,FLUSH,AUTOEND,TIMEOUT)
		"0,21,37,15,02,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 21:MENU CHOICE */	\
		"",

/////////////////////////////////////////////////////////////////////////////
		"APUDATE",												// 7
		906,
		// SCREEN DATA
//		 0123456789012345678901234567890123456789
/*0*/	"                                        ",
/*1*/	"                                        ",
/*2*/	"           AP�汾:V00-00-00             ",
/*3*/	"                                        ",
/*4*/	"           ��  ��:0000.00.00            ",
/*5*/	"                                        ",
/*6*/	"                                        ",
/*7*/	"                                        ",
/*8*/	"                                        ",
/*9*/	"                                        ",
/*0*/	"                                        ",
/*1*/	"                                        ",
/*2*/	"                                        ",
/*3*/	"                                        ",
/*4*/	"11:����,12:����,13:USB��,88:�˳�        ",
/*5*/	"                      ������������:NN ",
		// SCREEN DISPLAY(NO,XX,YY,ATTRIBUTE,LENGTH,JUSTIFIER,FORMAT)
		"00,00,14,INVERTED,02,LEFT,11:"							\
		"00,08,14,INVERTED,02,LEFT,12:"							\
		"00,16,14,INVERTED,02,LEFT,13:"							\
		"00,25,14,INVERTED,02,LEFT,88:"							\

		"01,18,02,NORMAL,10,LEFT,:"								/* 01:AP VERSION(V00-00-00) */	\
		"02,18,04,NORMAL,10,LEFT,####.##.##:"					/* 02:TRANSACTION DATE(YYYYMMDD) */	\

		"03,37,15,NORMAL,02,LEFT,##:"							/* 03:MENU CHOICE */	\

		"48,00,15,NORMAL,20,LEFT,:"								/* 48:MESSAGE */	\
		"",
		// SCREEN ACCEPT(INDEX,NO,XX,YY,LENGTH,ECHOMODE,ATTRIBUTE,INPUTTYPE,CURSORON,FLUSH,AUTOEND,TIMEOUT)
		"0,03,37,15,02,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 03:MENU CHOICE */	\
		"",

/////////////////////////////////////////////////////////////////////////////
		"TRACECOPY",											// 8
		907,
		// SCREEN DATA
//		 0123456789012345678901234567890123456789
/*0*/	"                                        ",
/*1*/	"                                        ",
/*2*/	"           AP�汾:V00-00-00             ",
/*3*/	"                                        ",
/*4*/	"           ��  ��:0000.00.00            ",
/*5*/	"                                        ",
/*6*/	"1>     TRACE ����:0000.00.00            ",
/*7*/	"                                        ",
/*8*/	"                                        ",
/*9*/	"                                        ",
/*0*/	"                                        ",
/*1*/	"                                        ",
/*2*/	"                                        ",
/*3*/	"1~9:�����к�,ENTER:��һ��,CANCEL:ȡ��   ",
/*4*/	"11:����,12:USB��,88:�˳�                ",
/*5*/	"                      ������������:NN ",
		// SCREEN DISPLAY(NO,XX,YY,ATTRIBUTE,LENGTH,JUSTIFIER,FORMAT)
		"00,00,13,INVERTED,03,LEFT,1~9:"						\
		"00,13,13,INVERTED,05,LEFT,ENTER:"						\
		"00,26,13,INVERTED,06,LEFT,CANCEL:"						\
		"00,00,14,INVERTED,02,LEFT,11:"							\
		"00,08,14,INVERTED,02,LEFT,12:"							\
		"00,17,14,INVERTED,02,LEFT,88:"							\

		"01,18,02,NORMAL,10,LEFT,:"								/* 01:AP VERSION(V00-00-00) */	\
		"02,18,04,NORMAL,10,LEFT,####.##.##:"					/* 02:TRANSACTION DATE(YYYYMMDD) */	\

		"03,18,06,NORMAL,10,LEFT,####.##.##:"					/* 03:TRACE DATE(YYYYMMDD) */	\

		"04,37,15,NORMAL,02,LEFT,##:"							/* 04:MENU CHOICE */	\
		
		"48,00,15,NORMAL,20,LEFT,:"								/* 48:MESSAGE */	\
		"",
		// SCREEN ACCEPT(INDEX,NO,XX,YY,LENGTH,ECHOMODE,ATTRIBUTE,INPUTTYPE,CURSORON,FLUSH,AUTOEND,TIMEOUT)
		"1,03,18,06,08,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 03:TRACE DATE(YYYYMMDD) */	\

		"0,04,37,15,02,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 04:MENU CHOICE */	\
		"",

/////////////////////////////////////////////////////////////////////////////
		"VERSION",												// 9
		908,
		// SCREEN DATA
//		 0123456789012345678901234567890123456789
/*0*/	" AP�汾:V00-00-00.  ����:0000.00.00     ",
/*1*/	"     SP �汾             EP �汾        ",
/*2*/	"CDU:XXXXXXXXXXXXXXX CDU:XXXXXXXXXXXXXXX ",
/*3*/	"JPR:XXXXXXXXXXXXXXX JPR:XXXXXXXXXXXXXXX ",
/*4*/	"SPR:XXXXXXXXXXXXXXX SPR:XXXXXXXXXXXXXXX ",
/*5*/	"MCU:XXXXXXXXXXXXXXX MCU:XXXXXXXXXXXXXXX ",
/*6*/	"PIN:XXXXXXXXXXXXXXX PIN:XXXXXXXXXXXXXXX ",
/*7*/	"CAM:XXXXXXXXXXXXXXX CAM:XXXXXXXXXXXXXXX ",
/*8*/	"SIU:XXXXXXXXXXXXXXX SIU:XXXXXXXXXXXXXXX ",
/*9*/	"UPS:XXXXXXXXXXXXXXX UPS:XXXXXXXXXXXXXXX ",
/*0*/	"TTU:XXXXXXXXXXXXXXX TTU:XXXXXXXXXXXXXXX ",
/*1*/	"MWI:XXXXXXXXXXXXXXX                     ",
/*2*/	"COMMONITOR:XXXXXXXXXXXXXXX              ",
/*3*/	"                                        ",
/*4*/	"11:��ӡ,88:�˳�                         ",
/*5*/	"                      ������������:NN ",
		// SCREEN DISPLAY(NO,XX,YY,ATTRIBUTE,LENGTH,JUSTIFIER,FORMAT)
		"00,00,14,INVERTED,02,LEFT,11:"							\
		"00,08,14,INVERTED,02,LEFT,88:"							\

		"01,08,00,NORMAL,10,LEFT,:"								/* 01:AP VERSION(V00-00-00) */	\
		"02,25,00,NORMAL,10,LEFT,####.##.##:"					/* 02:TRANSACTION DATE(YYYYMMDD) */ \

		"03,04,02,NORMAL,15,LEFT,:"								/* 03:CDU SP VERSION */	\
		"04,04,03,NORMAL,15,LEFT,:"								/* 04:JPR SP VERSION */	\
		"05,04,04,NORMAL,15,LEFT,:"								/* 05:SPR SP VERSION */	\
		"06,04,05,NORMAL,15,LEFT,:"								/* 06:MCU SP VERSION */	\
		"07,04,06,NORMAL,15,LEFT,:"								/* 07:PIN SP VERSION */	\
		"08,04,07,NORMAL,15,LEFT,:"								/* 08:CAM SP VERSION */	\
		"09,04,08,NORMAL,15,LEFT,:"								/* 09:SIU SP VERSION */	\
		"10,04,09,NORMAL,15,LEFT,:"								/* 10:UPS SP VERSION */	\
		"11,04,10,NORMAL,15,LEFT,:"								/* 11:TTU SP VERSION */	\
		"12,04,11,NORMAL,15,LEFT,:"								/* 12:MWI SP VERSION */	\
		"13,11,12,NORMAL,15,LEFT,:"								/* 13:COMMONITOR SP VERSION  */	\
		"14,24,02,NORMAL,15,LEFT,:"								/* 14:CDU EP VERSION */	\
		"15,24,03,NORMAL,15,LEFT,:"								/* 15:JPR EP VERSION */	\
		"16,24,04,NORMAL,15,LEFT,:"								/* 16:SPR EP VERSION */	\
		"17,24,05,NORMAL,15,LEFT,:"								/* 17:MCU EP VERSION */	\
		"18,24,06,NORMAL,15,LEFT,:"								/* 18:PIN EP VERSION */	\
		"19,24,07,NORMAL,15,LEFT,:"								/* 19:CAM EP VERSION */	\
		"20,24,08,NORMAL,15,LEFT,:"								/* 20:SIU EP VERSION */	\
		"21,24,09,NORMAL,15,LEFT,:"								/* 21:UPS EP VERSION */	\
		"22,24,10,NORMAL,15,LEFT,:"								/* 22:TTU EP VERSION */	\

		"23,37,15,NORMAL,02,LEFT,##:"							/* 23:MENU CHOICE */	\
		
		"48,00,15,NORMAL,20,LEFT,:"								/* 48:MESSAGE */	\
		"",
		// SCREEN ACCEPT(INDEX,NO,XX,YY,LENGTH,ECHOMODE,ATTRIBUTE,INPUTTYPE,CURSORON,FLUSH,AUTOEND,TIMEOUT)
		"0,23,37,15,02,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 23:MENU CHOICE */	\
		"",

/////////////////////////////////////////////////////////////////////////////
		"WORKPARAM",											// 10
		909,
		// SCREEN DATA
//		 0123456789012345678901234567890123456789
/*0*/	"1>ATM���  :00000000 ���к�   :000000000",
/*1*/	"2>Ԥ�����1:00000000 Ԥ�����2:00000000 ",
/*2*/	"                                        ",
/*3*/	"3>����IP��ַ :XXX.XXX.XXX.XXX �˿�:XXXXX",
/*4*/	"4>��־����IP :XXX.XXX.XXX.XXX �˿�:XXXXX",
/*5*/	"5>ATM IP��ַ :XXX.XXX.XXX.XXX           ",
/*6*/	"6>ATM��������:XXX.XXX.XXX.XXX           ",
/*7*/	"7>ATM����    :XXX.XXX.XXX.XXX �˿�:XXXXX",
/*8*/	"8>��������IP :XXX.XXX.XXX.XXX �˿�:XXXXX",
/*9*/	"9>�ӳ���ʽ   :X   0:���ؼӳ�  1:����ӳ�",
/*0*/	"                                        ",
/*1*/	"                                        ",
/*2*/	"                                        ",
/*3*/	"1~9:�����к�,ENTER:��һ��,CANCEL:ȡ��   ",
/*4*/	"11:����,12:��ӡ,88:�˳�                 ",
/*5*/	"                      ������������:NN ",
		// SCREEN DISPLAY(NO,XX,YY,ATTRIBUTE,LENGTH,JUSTIFIER,FORMAT)
		"00,00,13,INVERTED,03,LEFT,1~9:"						\
		"00,13,13,INVERTED,05,LEFT,ENTER:"						\
		"00,26,13,INVERTED,06,LEFT,CANCEL:"						\
		"00,00,14,INVERTED,02,LEFT,11:"							\
		"00,08,14,INVERTED,02,LEFT,12:"							\
		"00,16,14,INVERTED,02,LEFT,88:"							\

		"01,12,00,NORMAL,08,LEFT,:"								/* 01:ATM SERIAL NUMBER(NNNNNNNN) */	\
		"02,31,00,NORMAL,09,LEFT,:"								/* 02:BRANCH NUMBER(NNNNNNNN) */	\
		"03,12,01,NORMAL,08,LEFT,:"								/* 03:RESERVED NUMBER1(NNNNNNNN) */	\
		"04,31,01,NORMAL,08,LEFT,:"								/* 04:RESERVED NUMBER2(NNNNNNNN) */	\

		"05,14,03,NORMAL,15,LEFT,###.###.###.###:"				/* 05:HOST IP(NNNnnnNNNnnn) */	\
		"06,35,03,NORMAL,05,LEFT,:"								/* 06:HOST PORT(NNNNN) */	\
		"07,14,04,NORMAL,15,LEFT,###.###.###.###:"				/* 07:HOST IP2(NNNnnnNNNnnn) */	\
		"08,35,04,NORMAL,05,LEFT,:"								/* 08:HOST PORT2(NNNNN) */	\
		"09,14,05,NORMAL,15,LEFT,###.###.###.###:"				/* 09:ATM IP(NNNnnnNNNnnn) */	\
		"10,14,06,NORMAL,15,LEFT,###.###.###.###:"				/* 10:ATM SUBNET MASK(NNNnnnNNNnnn) */	\
		"11,14,07,NORMAL,15,LEFT,###.###.###.###:"				/* 11:ATM GATE WAY(NNNnnnNNNnnn) */	\
		"12,35,07,NORMAL,05,LEFT,:"								/* 12:ATM PORT(NNNNN) */	\
		"13,14,08,NORMAL,15,LEFT,###.###.###.###:"				/* 13:HOST IP3(NNNnnnNNNnnn) : BID */	\
		"14,35,08,NORMAL,05,LEFT,:"								/* 14:HOST PORT3(NNNNN) : BID */	\
		"15,14,09,NORMAL,01,LEFT,:"								/* 15:ADDCASH MODE */	\

		"16,37,15,NORMAL,02,LEFT,##:"							/* 17:MENU CHOICE */	\
		
		"48,00,15,NORMAL,20,LEFT,:"								/* 48:MESSAGE */	\
		"",
		// SCREEN ACCEPT(INDEX,NO,XX,YY,LENGTH,ECHOMODE,ATTRIBUTE,INPUTTYPE,CURSORON,FLUSH,AUTOEND,TIMEOUT)
		"1,01,12,00,08,NORMAL,NORMAL,ALPHANUMERIC,1,1,1,-1:"	/* 01:ATM SERIAL NUMBER(NNNNNNNN) */	\
		"*,02,31,00,09,NORMAL,NORMAL,ALPHANUMERIC,1,1,1,-1:"	/* 02:BRANCH NUMBER(NNNNNNNN) */	\
		"2,03,12,01,08,NORMAL,NORMAL,ALPHANUMERIC,1,1,1,-1:"	/* 03:RESERVED NUMBER1(NNNNNNNN) */	\
		"*,04,31,01,08,NORMAL,NORMAL,ALPHANUMERIC,1,1,1,-1:"	/* 04:RESERVED NUMBER2(NNNNNNNN) */	\

		"3,05,14,03,12,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 05:HOST IP(NNNnnnNNNnnn) */	\
		"*,06,35,03,05,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 06:HOST PORT(NNNNN) */	\
		"4,07,14,04,12,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 07:HOST IP2(NNNnnnNNNnnn) */	\
		"*,08,35,04,05,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 08:HOST PORT2(NNNNN) */	\
		"5,09,14,05,12,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 09:ATM IP(NNNnnnNNNnnn) */	\
		"6,10,14,06,12,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 10:ATM SUBNET MASK(NNNnnnNNNnnn) */	\
		"7,11,14,07,12,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 11:ATM GATE WAY(NNNnnnNNNnnn) */	\
		"*,12,35,07,05,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 12:ATM PORT(NNNNN) */	\
		"8,13,14,08,12,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 13:HOST IP3(NNNnnnNNNnnn) : BID */	\
		"*,14,35,08,05,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 14:HOST PORT3(NNNNN) : BID */	\
		"9,15,14,09,01,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 15:ADDCASH MODE */	\

		"0,16,37,15,02,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 17:MENU CHOICE */	\
		"",

/////////////////////////////////////////////////////////////////////////////
		"ADDCASH",												// 11
		910,
		// SCREEN DATA
//		 0123456789012345678901234567890123456789
/*0*/	"                ������Ϣ                ",
/*1*/	" ����1:9999(100)     ����2:9999(100)    ",
/*2*/	" ����3:9999(100)     ����4:9999(100)    ",
/*3*/	"                �豸״̬                ",
/*4*/	" ��  ��  ��:����     ��  ��  ��:����    ",
/*5*/	" ƾ����ӡ��:����     ��ˮ��ӡ��:����    ",
/*6*/	"                                        ",
/*7*/	"  ������� �ӳ����  �ӳ�����  �ӳ���� ",
/*8*/	"1> ����1     100       9999     999999  ",
/*9*/	"2> ����2     100       9999     999999  ",
/*0*/	"3> ����3     050       9999     999999  ",
/*1*/	"4> ����4     050       9999     999999  ",
/*2*/	"                                        ",
/*3*/	"1~9:�����к�,ENTER:��һ��,CANCEL:ȡ��   ",
/*4*/	"11:�ӳ�                 88:�˳�         ",    //"11:�ӳ�,12:�峮,13:����,88:�˳�         ",
/*5*/	"                      ������������:NN ",
		// SCREEN DISPLAY(NO,XX,YY,ATTRIBUTE,LENGTH,JUSTIFIER,FORMAT)
		"00,00,13,INVERTED,03,LEFT,1~9:"						\
		"00,13,13,INVERTED,05,LEFT,ENTER:"						\
		"00,26,13,INVERTED,06,LEFT,CANCEL:"						\
		"00,00,14,INVERTED,02,LEFT,11:"							\
/*
 				"00,08,14,INVERTED,02,LEFT,12:"							\
 		 		"00,16,14,INVERTED,02,LEFT,13:"							\*/
 		
		"00,24,14,INVERTED,02,LEFT,88:"							\

		"01,07,01,NORMAL,10,LEFT,:"								/* 01:CST1(CCCC(GGG)) */	\
		"02,27,01,NORMAL,10,LEFT,:"								/* 02:CST2(CCCC(GGG)) */	\
		"03,07,02,NORMAL,10,LEFT,:"								/* 03:CST3(CCCC(GGG)) */	\
		"04,27,02,NORMAL,10,LEFT,:"								/* 04:CST4(CCCC(GGG)) */	\

		"05,12,04,NORMAL,04,LEFT,:"								/* 05:CDU(NORMAL, ERROR) */	\
		"06,32,04,NORMAL,04,LEFT,:"								/* 06:MCU(NORMAL, ERROR) */	\
		"07,12,05,NORMAL,04,LEFT,:"								/* 07:SPR(NORMAL, ERROR) */	\
		"08,32,05,NORMAL,04,LEFT,:"								/* 08:JPR(NORMAL, ERROR) */	\
		"09,13,08,NORMAL,03,LEFT,:"								/* 09:CST1(VVV) */	\
		"10,23,08,NORMAL,04,LEFT,:"								/* 10:CST1(CCCC) */	\
		"11,32,08,NORMAL,06,LEFT,:"								/* 11:CST1(AAAAAA) */	\
		"12,13,09,NORMAL,03,LEFT,:"								/* 12:CST2(VVV) */	\
		"13,23,09,NORMAL,04,LEFT,:"								/* 13:CST2(CCCC) */	\
		"14,32,09,NORMAL,06,LEFT,:"								/* 14:CST2(AAAAAA) */	\
		"15,13,10,NORMAL,03,LEFT,:"								/* 15:CST3(VVV) */	\
		"16,23,10,NORMAL,04,LEFT,:"								/* 16:CST3(CCCC) */	\
		"17,32,10,NORMAL,06,LEFT,:"								/* 17:CST3(AAAAAA) */	\
		"18,13,11,NORMAL,03,LEFT,:"								/* 18:CST4(VVV) */	\
		"19,23,11,NORMAL,04,LEFT,:"								/* 19:CST4(CCCC) */	\
		"20,32,11,NORMAL,06,LEFT,:"								/* 20:CST4(AAAAAA) */	\

		"21,37,15,NORMAL,02,LEFT,##:"							/* 21:MENU CHOICE */	\
		
		"48,00,15,NORMAL,20,LEFT,:"								/* 48:MESSAGE */	\
		"",
		// SCREEN ACCEPT(INDEX,NO,XX,YY,LENGTH,ECHOMODE,ATTRIBUTE,INPUTTYPE,CURSORON,FLUSH,AUTOEND,TIMEOUT)
		"1,10,23,08,04,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 10:CST1(CCCC) */	\
		"*,11,32,08,06,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 11:CST1(AAAAAA) */	\
		"2,13,23,09,04,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 13:CST2(CCCC) */	\
		"*,14,32,09,06,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 14:CST2(AAAAAA) */	\
		"3,16,23,10,04,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 16:CST3(CCCC) */	\
		"*,17,32,10,06,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 17:CST3(AAAAAA) */	\
		"4,19,23,11,04,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 19:CST4(CCCC) */	\
		"*,20,32,11,06,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 20:CST4(AAAAAA) */	\

		"0,21,37,15,02,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 21:MENU CHOICE */	\
		"",

/////////////////////////////////////////////////////////////////////////////
		"EJMVIEW",												// 12
		911,
		// SCREEN DATA
//		 0123456789012345678901234567890123456789
/*0*/	"                                        ",
/*1*/	"            �������ѯ����              ",
/*2*/	"                                        ",
/*3*/	"1>  ��������:0000.00.00                 ",
/*4*/	"                                        ",
/*5*/	"2>������ˮ��:000000                     ",
/*6*/	"                                        ",
/*7*/	"3>  ���׿���:123456789012345678901234   ",
/*8*/	"                                        ",
/*9*/	"                                        ",
/*0*/	"                                        ",
/*1*/	"                                        ",
/*2*/	"                                        ",
/*3*/	"1~9:�����к�,ENTER:��һ��,CANCEL:ȡ��   ",
/*4*/	"11:�鿴,88:�˳�                         ",
/*5*/	"                      ������������:NN ",
		// SCREEN DISPLAY(NO,XX,YY,ATTRIBUTE,LENGTH,JUSTIFIER,FORMAT)
		"00,00,13,INVERTED,03,LEFT,1~9:"						\
		"00,13,13,INVERTED,05,LEFT,ENTER:"						\
		"00,26,13,INVERTED,06,LEFT,CANCEL:"						\
		"00,00,14,INVERTED,02,LEFT,11:"							\
		"00,08,14,INVERTED,02,LEFT,88:"							\

		"01,13,03,NORMAL,10,LEFT,####.##.##:"					/* 01:TRANSACTION DATE(YYYYMMDD) */ \
		"02,13,05,NORMAL,06,LEFT,:"								/* 02:TRANSACTION SERIAL NO */ \
		"03,13,07,NORMAL,24,LEFT,:"								/* 03:TRANSACTION CARD NO */ \

		"04,37,15,NORMAL,02,LEFT,##:"							/* 04:MENU CHOICE */	\
		
		"48,00,15,NORMAL,20,LEFT,:"								/* 48:MESSAGE */	\
		"",
		// SCREEN ACCEPT(INDEX,NO,XX,YY,LENGTH,ECHOMODE,ATTRIBUTE,INPUTTYPE,CURSORON,FLUSH,AUTOEND,TIMEOUT)
		"1,01,13,03,08,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 01:TRANSACTION DATE(YYYYMMDD) */ \
		"2,02,13,05,06,NORMAL,NORMAL,REALNUMERIC,1,1,1,-1:"		/* 02:TRANSACTION SERIAL NO */ \
		"3,03,13,07,24,NORMAL,NORMAL,REALNUMERIC,1,1,1,-1:"		/* 03:TRANSACTION CARD NO */ \

		"0,04,37,15,02,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 04:MENU CHOICE */	\
		"",

/////////////////////////////////////////////////////////////////////////////
		"EJMRESULT",											// 13
		912,
		// SCREEN DATA
//		 0123456789012345678901234567890123456789
/*0*/	"                ��ѯ���                ",
/*1*/	"                                        ",
/*2*/	"                                        ",
/*3*/	"                                        ",
/*4*/	"                                        ",
/*5*/	"                                        ",
/*6*/	"                                        ",
/*7*/	"                                        ",
/*8*/	"                                        ",
/*9*/	"                                        ",
/*0*/	"                                        ",
/*1*/	"                                        ",
/*2*/	"                                        ",
/*3*/	"                                        ",
/*4*/	"11ǰʮ��12��ӡ13����14ǰ1��15��1��88�˳�",
/*5*/	"                      ������������:NN ",
		// SCREEN DISPLAY(NO,XX,YY,ATTRIBUTE,LENGTH,JUSTIFIER,FORMAT)
		"00,00,14,INVERTED,02,LEFT,11:"							\
		"00,08,14,INVERTED,02,LEFT,12:"							\
		"00,14,14,INVERTED,02,LEFT,13:"							\
		"00,20,14,INVERTED,02,LEFT,14:"							\
		"00,27,14,INVERTED,02,LEFT,15:"							\
		"00,34,14,INVERTED,02,LEFT,88:"							\

		"01,00,01,NORMAL,40,LEFT,:"								/* 01:EJR DATA(X40)00 */	\
		"02,00,02,NORMAL,40,LEFT,:"								/* 02:EJR DATA(X40)01 */	\
		"03,00,03,NORMAL,40,LEFT,:"								/* 03:EJR DATA(X40)02 */	\
		"04,00,04,NORMAL,40,LEFT,:"								/* 04:EJR DATA(X40)03 */	\
		"05,00,05,NORMAL,40,LEFT,:"								/* 05:EJR DATA(X40)04 */	\
		"06,00,06,NORMAL,40,LEFT,:"								/* 06:EJR DATA(X40)05 */	\
		"07,00,07,NORMAL,40,LEFT,:"								/* 07:EJR DATA(X40)06 */	\
		"08,00,08,NORMAL,40,LEFT,:"								/* 08:EJR DATA(X40)07 */	\
		"09,00,09,NORMAL,40,LEFT,:"								/* 09:EJR DATA(X40)08 */	\
		"10,00,10,NORMAL,40,LEFT,:"								/* 10:EJR DATA(X40)09 */	\
		"11,00,11,NORMAL,40,LEFT,:"								/* 11:EJR DATA(X40)10 */	\
		"12,00,12,NORMAL,40,LEFT,:"								/* 12:EJR DATA(X40)11 */	\
		"13,00,13,NORMAL,40,LEFT,:"								/* 13:EJR DATA(X40)12 */	\

		"14,37,15,NORMAL,02,LEFT,##:"							/* 14:MENU CHOICE */	\
		
		"48,00,15,NORMAL,20,LEFT,:"								/* 48:MESSAGE */	\
		"",
		// SCREEN ACCEPT(INDEX,NO,XX,YY,LENGTH,ECHOMODE,ATTRIBUTE,INPUTTYPE,CURSORON,FLUSH,AUTOEND,TIMEOUT)
		"0,14,37,15,02,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 14:MENU CHOICE */	\
		"",

/////////////////////////////////////////////////////////////////////////////
		"INPUT_NEW_PIN",										// 14
		913,
		// SCREEN DATA
//		 0123456789012345678901234567890123456789
/*0*/	"                                        ",
/*1*/	"            ����������������            ",
/*2*/	"                                        ",
/*3*/	"             ��ENTER ������             ",
/*4*/	"                                        ",
/*5*/	"                [      ]                ",
/*6*/	"                                        ",
/*7*/	"                                        ",
/*8*/	"                                        ",
/*9*/	"                                        ",
/*0*/	"                                        ",
/*1*/	"                                        ",
/*2*/	"                                        ",
/*3*/	"                                        ",
/*4*/	"                                        ",
/*5*/	"                                        ",
		// SCREEN DISPLAY(NO,XX,YY,ATTRIBUTE,LENGTH,JUSTIFIER,FORMAT)
		"00,15,03,INVERTED,05,LEFT,ENTER:"						\

		"01,17,05,NORMAL,06,LEFT,******:"						/* 01:PASSWORD */	\

		"48,10,13,NORMAL,20,LEFT,:"								/* 48:MESSAGE */	\
		"",
		// SCREEN ACCEPT(INDEX,NO,XX,YY,LENGTH,ECHOMODE,ATTRIBUTE,INPUTTYPE,CURSORON,FLUSH,AUTOEND,TIMEOUT)
		"0,01,17,05,06,PASSWORD,NORMAL,REALNUMERIC,1,1,1,-1:"	/* 01:PASSWORD */	\
		"",

/////////////////////////////////////////////////////////////////////////////
		"INPUT_NEW_PIN_AGAIN",									// 15
		914,
		// SCREEN DATA
//		 0123456789012345678901234567890123456789
/*0*/	"                                        ",
/*1*/	"            ���ٴ�����������            ",
/*2*/	"                                        ",
/*3*/	"             ��ENTER ������             ",
/*4*/	"                                        ",
/*5*/	"                [      ]                ",
/*6*/	"                                        ",
/*7*/	"                                        ",
/*8*/	"                                        ",
/*9*/	"                                        ",
/*0*/	"                                        ",
/*1*/	"                                        ",
/*2*/	"                                        ",
/*3*/	"                                        ",
/*4*/	"                                        ",
/*5*/	"                                        ",
		// SCREEN DISPLAY(NO,XX,YY,ATTRIBUTE,LENGTH,JUSTIFIER,FORMAT)
		"00,15,03,INVERTED,05,LEFT,ENTER:"						\

		"01,17,05,NORMAL,06,LEFT,******:"						/* 01:PASSWORD */	\

		"48,10,13,NORMAL,20,LEFT,:"								/* 48:MESSAGE */	\
		"",
		// SCREEN ACCEPT(INDEX,NO,XX,YY,LENGTH,ECHOMODE,ATTRIBUTE,INPUTTYPE,CURSORON,FLUSH,AUTOEND,TIMEOUT)
		"0,01,17,05,06,PASSWORD,NORMAL,REALNUMERIC,1,1,1,-1:"	/* 01:PASSWORD */	\
		"",

/////////////////////////////////////////////////////////////////////////////
		"EJMCOPY",												// 16
		915,
		// SCREEN DATA
//		 0123456789012345678901234567890123456789
/*0*/	"                                        ",
/*1*/	"                                        ",
/*2*/	"           AP�汾:V00-00-00.            ",
/*3*/	"                                        ",
/*4*/	"           ��  ��:0000.00.00            ",
/*5*/	"                                        ",
/*6*/	"              ������־ʱ��              ",
/*7*/	"                                        ",
/*8*/	"1>      [XXXX.XX.XX]~[XXXX.XX.XX]       ",
/*9*/	"                                        ",
/*0*/	"                                        ",
/*1*/	"                                        ",
/*2*/	"                                        ",
/*3*/	"1~9:�����к�,ENTER:��һ��,CANCEL:ȡ��   ",
/*4*/	"11:����,12:USB��,88:�˳�                ",
/*5*/	"                      ������������:NN ",
		// SCREEN DISPLAY(NO,XX,YY,ATTRIBUTE,LENGTH,JUSTIFIER,FORMAT)
		"00,00,13,INVERTED,03,LEFT,1~9:"						\
		"00,13,13,INVERTED,05,LEFT,ENTER:"						\
		"00,26,13,INVERTED,06,LEFT,CANCEL:"						\
		"00,00,14,INVERTED,02,LEFT,11:"							\
		"00,08,14,INVERTED,02,LEFT,12:"							\
		"00,17,14,INVERTED,02,LEFT,88:"							\

		"01,18,02,NORMAL,10,LEFT,:"								/* 01:AP VERSION(V00-00-00) */	\
		"02,18,04,NORMAL,10,LEFT,####.##.##:"					/* 02:TRANSACTION DATE(YYYYMMDD) */ \

		"03,09,08,NORMAL,10,LEFT,####.##.##:"					/* 03:EJM START DATE(YYYYMMDD) */	\
		"04,22,08,NORMAL,10,LEFT,####.##.##:"					/* 04:EJM END DATE(YYYYMMDD) */	\

		"05,37,15,NORMAL,02,LEFT,##:"							/* 05:MENU CHOICE */	\
		
		"48,00,15,NORMAL,20,LEFT,:"								/* 48:MESSAGE */	\
		"",
		// SCREEN ACCEPT(INDEX,NO,XX,YY,LENGTH,ECHOMODE,ATTRIBUTE,INPUTTYPE,CURSORON,FLUSH,AUTOEND,TIMEOUT)
		"1,03,09,08,08,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 03:EJM START DATE(YYYYMMDD) */	\
		"*,04,22,08,08,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 04:EJM END DATE(YYYYMMDD) */	\

		"0,05,37,15,02,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 05:MENU CHOICE */	\
		"",

/////////////////////////////////////////////////////////////////////////////
		"CASHVALUE",											// 17
		916,
		// SCREEN DATA
//		 0123456789012345678901234567890123456789
/*0*/	"                ���ؾ���                ",
/*1*/	"                                        ",
/*2*/	"         �����������볮���豸         ",
/*3*/	"         ����ʵ��װ�����һ��         ",
/*4*/	"   ��ȷ���Ƿ�����,���ⷢ�����̿��¹�!   ",
/*5*/	"                                        ",
/*6*/	"  �������  ԭ�����  �������  �������",
/*7*/	"1> ����1      100       XXX       XXX   ",
/*8*/	"2> ����2      100       XXX       XXX   ",
/*9*/	"3> ����3      050       XXX       XXX   ",
/*0*/	"4> ����4      050       XXX       XXX   ",
/*1*/	"                                        ",
/*2*/	"                                        ",
/*3*/	"1~9:�����к�,ENTER:��һ��,CANCEL:ȡ��   ",
/*4*/	"11:�������,88:�˳�                     ",
/*5*/	"                      ������������:NN ",
		// SCREEN DISPLAY(NO,XX,YY,ATTRIBUTE,LENGTH,JUSTIFIER,FORMAT)
		"00,00,13,INVERTED,03,LEFT,1~9:"						\
		"00,13,13,INVERTED,05,LEFT,ENTER:"						\
		"00,26,13,INVERTED,06,LEFT,CANCEL:"						\
		"00,00,14,INVERTED,02,LEFT,11:"							\
		"00,12,14,INVERTED,02,LEFT,88:"							\

		"01,14,07,NORMAL,03,LEFT,:"								/* 01:CST1 ORG CURRENCY(VVV) */	\
		"02,14,08,NORMAL,03,LEFT,:"								/* 02:CST2 ORG CURRENCY(VVV) */	\
		"03,14,09,NORMAL,03,LEFT,:"								/* 03:CST3 ORG CURRENCY(VVV) */	\
		"04,14,10,NORMAL,03,LEFT,:"								/* 04:CST4 ORG CURRENCY(VVV) */	\
		"05,24,07,NORMAL,03,LEFT,:"								/* 05:CST1 NEW CURRENCY(VVV) */	\
		"06,24,08,NORMAL,03,LEFT,:"								/* 06:CST2 NEW CURRENCY(VVV) */	\
		"07,24,09,NORMAL,03,LEFT,:"								/* 07:CST3 NEW CURRENCY(VVV) */	\
		"08,24,10,NORMAL,03,LEFT,:"								/* 08:CST4 NEW CURRENCY(VVV) */	\
		"09,34,07,NORMAL,03,LEFT,:"								/* 09:CST1 CONFIRM CURRENCY(VVV) */	\
		"10,34,08,NORMAL,03,LEFT,:"								/* 10:CST2 CONFIRM CURRENCY(VVV) */	\
		"11,34,09,NORMAL,03,LEFT,:"								/* 11:CST3 CONFIRM CURRENCY(VVV) */	\
		"12,34,10,NORMAL,03,LEFT,:"								/* 12:CST4 CONFIRM CURRENCY(VVV) */	\

		"13,37,15,NORMAL,02,LEFT,##:"							/* 13:MENU CHOICE */	\
		
		"48,00,15,NORMAL,20,LEFT,:"								/* 48:MESSAGE */	\
		"",
		// SCREEN ACCEPT(INDEX,NO,XX,YY,LENGTH,ECHOMODE,ATTRIBUTE,INPUTTYPE,CURSORON,FLUSH,AUTOEND,TIMEOUT)
		"1,05,24,07,03,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 05:CST1 NEW CURRENCY(VVV) */	\
		"*,09,34,07,03,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 09:CST1 CONFIRM CURRENCY(VVV) */	\
		"2,06,24,08,03,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 06:CST2 NEW CURRENCY(VVV) */	\
		"*,10,34,08,03,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 10:CST2 CONFIRM CURRENCY(VVV) */	\
		"3,07,24,09,03,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 07:CST3 NEW CURRENCY(VVV) */	\
		"*,11,34,09,03,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 11:CST3 CONFIRM CURRENCY(VVV) */	\
		"4,08,24,10,03,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 08:CST4 NEW CURRENCY(VVV) */	\
		"*,12,34,10,03,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 12:CST4 CONFIRM CURRENCY(VVV) */	\

		"0,13,37,15,02,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 13:MENU CHOICE */	\
		"",

/////////////////////////////////////////////////////////////////////////////
		"EJMSEND",												// 18
		917,
		// SCREEN DATA
//		 0123456789012345678901234567890123456789
/*0*/	"                                        ",
/*1*/	"                                        ",
/*2*/	"           AP�汾:V00-00-00.            ",
/*3*/	"                                        ",
/*4*/	"           ��  ��:0000.00.00            ",
/*5*/	"                                        ",
/*6*/	"              ������־ʱ��              ",
/*7*/	"                                        ",
/*8*/	"1>      [XXXX.XX.XX]~[XXXX.XX.XX]       ",
/*9*/	"                                        ",
/*0*/	"                                        ",
/*1*/	"                                        ",
/*2*/	"                                        ",
/*3*/	"1~9:�����к�,ENTER:��һ��,CANCEL:ȡ��   ",
/*4*/	"11:ȡ��,12:ȫ��,13:����,88:�˳�         ",
/*5*/	"                      ������������:NN ",
		// SCREEN DISPLAY(NO,XX,YY,ATTRIBUTE,LENGTH,JUSTIFIER,FORMAT)
		"00,00,13,INVERTED,03,LEFT,1~9:"						\
		"00,13,13,INVERTED,05,LEFT,ENTER:"						\
		"00,26,13,INVERTED,06,LEFT,CANCEL:"						\
		"00,00,14,INVERTED,02,LEFT,11:"							\
		"00,08,14,INVERTED,02,LEFT,12:"							\
		"00,16,14,INVERTED,02,LEFT,13:"							\
		"00,24,14,INVERTED,02,LEFT,88:"							\

		"01,18,02,NORMAL,10,LEFT,:"								/* 01:AP VERSION(V00-00-00) */	\
		"02,18,04,NORMAL,10,LEFT,####.##.##:"					/* 02:TRANSACTION DATE(YYYYMMDD) */ \

		"03,09,08,NORMAL,10,LEFT,####.##.##:"					/* 03:EJM START DATE(YYYYMMDD) */	\
		"04,22,08,NORMAL,10,LEFT,####.##.##:"					/* 04:EJM END DATE(YYYYMMDD) */	\

		"05,37,15,NORMAL,02,LEFT,##:"							/* 05:MENU CHOICE */	\
		
		"48,00,15,NORMAL,20,LEFT,:"								/* 48:MESSAGE */	\
		"",
		// SCREEN ACCEPT(INDEX,NO,XX,YY,LENGTH,ECHOMODE,ATTRIBUTE,INPUTTYPE,CURSORON,FLUSH,AUTOEND,TIMEOUT)
		"1,03,09,08,08,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 03:EJM START DATE(YYYYMMDD) */	\
		"*,04,22,08,08,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 04:EJM END DATE(YYYYMMDD) */	\

		"0,05,37,15,02,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 05:MENU CHOICE */	\
		"",

/////////////////////////////////////////////////////////////////////////////
		"EJMPRINT",												// 19
		918,
		// SCREEN DATA
//		 0123456789012345678901234567890123456789
/*0*/	"                                        ",
/*1*/	"                                        ",
/*2*/	"           AP�汾:V00-00-00.            ",
/*3*/	"                                        ",
/*4*/	"           ��  ��:0000.00.00            ",
/*5*/	"                                        ",
/*6*/	"              ������־ʱ��              ",
/*7*/	"                                        ",
/*8*/	"1>      [XXXX.XX.XX]~[XXXX.XX.XX]       ",
/*9*/	"                                        ",
/*0*/	"                                        ",
/*1*/	"                                        ",
/*2*/	"                                        ",
/*3*/	"1~9:�����к�,ENTER:��һ��,CANCEL:ȡ��   ",
/*4*/	"11:ȡ��,12:ȫ��,13:����,88:�˳�         ",
/*5*/	"                      ������������:NN ",
		// SCREEN DISPLAY(NO,XX,YY,ATTRIBUTE,LENGTH,JUSTIFIER,FORMAT)
		"00,00,13,INVERTED,03,LEFT,1~9:"						\
		"00,13,13,INVERTED,05,LEFT,ENTER:"						\
		"00,26,13,INVERTED,06,LEFT,CANCEL:"						\
		"00,00,14,INVERTED,02,LEFT,11:"							\
		"00,08,14,INVERTED,02,LEFT,12:"							\
		"00,16,14,INVERTED,02,LEFT,13:"							\
		"00,24,14,INVERTED,02,LEFT,88:"							\

		"01,18,02,NORMAL,10,LEFT,:"								/* 01:AP VERSION(V00-00-00) */	\
		"02,18,04,NORMAL,10,LEFT,####.##.##:"					/* 02:TRANSACTION DATE(YYYYMMDD) */ \

		"03,09,08,NORMAL,10,LEFT,####.##.##:"					/* 03:EJM START DATE(YYYYMMDD) */	\
		"04,22,08,NORMAL,10,LEFT,####.##.##:"					/* 04:EJM END DATE(YYYYMMDD) */	\

		"05,37,15,NORMAL,02,LEFT,##:"							/* 05:MENU CHOICE */	\
		
		"48,00,15,NORMAL,20,LEFT,:"								/* 48:MESSAGE */	\
		"",
		// SCREEN ACCEPT(INDEX,NO,XX,YY,LENGTH,ECHOMODE,ATTRIBUTE,INPUTTYPE,CURSORON,FLUSH,AUTOEND,TIMEOUT)
		"1,03,09,08,08,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 03:EJM START DATE(YYYYMMDD) */	\
		"*,04,22,08,08,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 04:EJM END DATE(YYYYMMDD) */	\

		"0,05,37,15,02,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 05:MENU CHOICE */	\
		"",

/////////////////////////////////////////////////////////////////////////////
		"DB_INITIAL",											// 20
		919,
		// SCREEN DATA
//		 0123456789012345678901234567890123456789
/*0*/	"                                        ",
/*1*/	"                                        ",
/*2*/	"           AP�汾:V00-00-00.            ",
/*3*/	"                                        ",
/*4*/	"           ��  ��:0000.00.00            ",
/*5*/	"                                        ",
/*6*/	"       ATM�ϵĸ������ݽ�����ʼ��!       ",
/*7*/	"                                        ",
/*8*/	"             �Ƿ���г�ʼ��?            ",
/*9*/	"                                        ",
/*0*/	"                                        ",
/*1*/	"                                        ",
/*2*/	"                                        ",
/*3*/	"                                        ",
/*4*/	"11:��ʼ��,88:�˳�                       ",
/*5*/	"                      ������������:NN ",
		// SCREEN DISPLAY(NO,XX,YY,ATTRIBUTE,LENGTH,JUSTIFIER,FORMAT)
		"00,00,14,INVERTED,02,LEFT,11:"							\
		"00,10,14,INVERTED,02,LEFT,88:"							\

		"01,18,02,NORMAL,10,LEFT,:"								/* 01:AP VERSION(V00-00-00) */	\
		"02,18,04,NORMAL,10,LEFT,####.##.##:"					/* 02:TRANSACTION DATE(YYYYMMDD) */ \

		"03,37,15,NORMAL,02,LEFT,##:"							/* 03:MENU CHOICE */	\
		
		"48,00,15,NORMAL,20,LEFT,:"								/* 48:MESSAGE */	\
		"",
		// SCREEN ACCEPT(INDEX,NO,XX,YY,LENGTH,ECHOMODE,ATTRIBUTE,INPUTTYPE,CURSORON,FLUSH,AUTOEND,TIMEOUT)
		"0,03,37,15,02,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 03:MENU CHOICE */	\
		"",

/////////////////////////////////////////////////////////////////////////////
		"TOTALSTIP",											// 20
		920,
		// SCREEN DATA
		//		 0123456789012345678901234567890123456789
		/*0*/	"                                        ",
		/*1*/	"                                        ",
		/*2*/	"                ��ܰ��ʾ                ",
		/*3*/	"                                        ",
		/*4*/	"    1. ���˺������\"����ӳ�\"����,    ",
		/*5*/	"       �����޷������ֽ��ཻ��.          ",
		/*6*/	"    2. ֻ������ӳ�ʱ, ���˺������     ",
		/*7*/	"       \"0�ӳ�\"����.                   ",
		/*8*/	"                                        ",
		/*9*/	"                                        ",
		/*0*/	"                                        ",
		/*1*/	"                                        ",
		/*2*/	"                                        ",
		/*3*/	"                                        ",
		/*4*/	"11:ȷ��                                 ",
		/*5*/	"                      ������������:NN ",
		// SCREEN DISPLAY(NO,XX,YY,ATTRIBUTE,LENGTH,JUSTIFIER,FORMAT)
		"00,00,14,INVERTED,02,LEFT,11:"							\
//		"00,10,14,INVERTED,02,LEFT,88:"							\
		
//		"01,18,02,NORMAL,10,LEFT,:"								/* 01:AP VERSION(V00-00-00) */	\
//		"02,18,04,NORMAL,10,LEFT,####.##.##:"					/* 02:TRANSACTION DATE(YYYYMMDD) */ \
		
		"03,37,15,NORMAL,02,LEFT,##:"							/* 03:MENU CHOICE */	\
		
		"48,00,15,NORMAL,20,LEFT,:"								/* 48:MESSAGE */	\
		"",
		// SCREEN ACCEPT(INDEX,NO,XX,YY,LENGTH,ECHOMODE,ATTRIBUTE,INPUTTYPE,CURSORON,FLUSH,AUTOEND,TIMEOUT)
		"0,03,37,15,02,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 03:MENU CHOICE */	\
		"",

/////////////////////////////////////////////////////////////////////////////
		"LOGIN",												// 21
		921,
		// SCREEN DATA
//		 0123456789012345678901234567890123456789
/*0*/	"                                        ",
/*1*/	"          �������Ա��ź�����          ",
/*2*/	"                                        ",
/*3*/	"             ѡ��'��¼'����             ",
/*4*/	"                                        ",
/*5*/	"1>            ���:XX                   ",
/*6*/	"                                        ",
/*7*/	"2>            ����:                     ",
/*8*/	"                                        ",
/*9*/	"                                        ",
/*0*/	"                                        ",
/*1*/	"                                        ",
/*2*/	"                                        ",
/*3*/	"1~9:�����к�,ENTER:��һ��,CANCEL:ȡ��   ",
/*4*/	"11:��¼,88:�˳�                         ",
/*5*/	"                      ������������:NN ",
		// SCREEN DISPLAY(NO,XX,YY,ATTRIBUTE,LENGTH,JUSTIFIER,FORMAT)
		"00,00,13,INVERTED,03,LEFT,1~9:"						\
		"00,13,13,INVERTED,05,LEFT,ENTER:"						\
		"00,26,13,INVERTED,06,LEFT,CANCEL:"						\
		"00,00,14,INVERTED,02,LEFT,11:"							\
		"00,08,14,INVERTED,02,LEFT,88:"							\

		"01,19,05,NORMAL,02,LEFT,:"								/* 01:OP NUMBER */	\
		"02,19,07,NORMAL,06,LEFT,******:"						/* 02:PASSWORD */	\
		"03,37,15,NORMAL,02,LEFT,##:"							/* 03:MENU CHOICE */	\
		
		"48,00,15,NORMAL,20,LEFT,:"								/* 48:MESSAGE */	\
		"",
		// SCREEN ACCEPT(INDEX,NO,XX,YY,LENGTH,ECHOMODE,ATTRIBUTE,INPUTTYPE,CURSORON,FLUSH,AUTOEND,TIMEOUT)
		"1,01,19,05,02,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 01:OP NUMBER */	\
		"2,02,19,07,06,PASSWORD,NORMAL,REALNUMERIC,1,1,1,-1:"	/* 02:PASSWORD */	\

		"0,03,37,15,02,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 03:MENU CHOICE */	\
		"",

/////////////////////////////////////////////////////////////////////////////
		"ADMIN",												// 22
		922,
		// SCREEN DATA
//		 0123456789012345678901234567890123456789
/*0*/	" ����1:9999(100)     ����2:9999(100)    ",
/*1*/	" ����3:9999(100)     ����4:9999(100)    ",
/*2*/	" ������:���� ������:���� ƾ����ӡ��:����",
/*3*/	" �ϳ���:���� �̿���:999  ��ˮ��ӡ��:����",
/*4*/	" �豸���:12345678   AP�汾��:V01-01-01 ",
/*5*/	"                                        ",
/*6*/	"01)�޸�����                             ",
/*7*/	"                                        ",
/*8*/	"02)��ӹ�Ա                             ",
/*9*/	"                                        ",
/*0*/	"03)ȡ����Ա                             ",
/*1*/	"                                        ",
/*2*/	"                                        ",
/*3*/	"                                        ",
/*4*/	"                    10)�˳�             ",
/*5*/	"                      ������������:NN ",
		// SCREEN DISPLAY(NO,XX,YY,ATTRIBUTE,LENGTH,JUSTIFIER,FORMAT)
		"01,07,00,NORMAL,09,LEFT,:"								/* 01:CST1(CCCC(GGG)) */	\
		"02,27,00,NORMAL,09,LEFT,:"								/* 02:CST2(CCCC(GGG)) */	\
		"03,07,01,NORMAL,09,LEFT,:"								/* 03:CST3(CCCC(GGG)) */	\
		"04,27,01,NORMAL,09,LEFT,:"								/* 04:CST4(CCCC(GGG)) */	\
		"05,08,02,NORMAL,04,LEFT,:"								/* 05:CDU(NORMAL, AUTO-OFF, MISSING, ABNORMAL, ERROR, OFF) */	\
		"06,20,02,NORMAL,04,LEFT,:"								/* 06:MCU(NORMAL, AUTO-OFF, MISSING, ABNORMAL, ERROR, OFF) */	\
		"07,36,02,NORMAL,04,LEFT,:"								/* 07:SPR(NORMAL, AUTO-OFF, MISSING, ABNORMAL, ERROR, OFF) */	\
		"08,36,03,NORMAL,04,LEFT,:"								/* 08:JPR(NORMAL, AUTO-OFF, MISSING, ABNORMAL, ERROR, OFF) */	\
		
		"09,00,00,NORMAL,00,LEFT,:"								/* 09:CST1(MISSING, NORMAL, LOW, EMPTY, FULL) */	\
		"10,00,00,NORMAL,00,LEFT,:"								/* 10:CST2(MISSING, NORMAL, LOW, EMPTY, FULL) */	\
		"11,00,00,NORMAL,00,LEFT,:"								/* 11:CST3(MISSING, NORMAL, LOW, EMPTY, FULL) */	\
		"12,00,00,NORMAL,00,LEFT,:"								/* 12:CST4(MISSING, NORMAL, LOW, EMPTY, FULL) */	\
		"13,08,03,NORMAL,04,LEFT,:"								/* 13:REJECT(MISSING, NORMAL, LOW, EMPTY, FULL) */	\
		"14,20,03,NORMAL,03,LEFT,:"								/* 14:MCU(C) */	\
		"15,00,00,NORMAL,00,LEFT,:"								/* 15:SPR(MISSING, NORMAL, LOW, EMPTY) */	\
		"16,00,00,NORMAL,00,LEFT,:"								/* 16:JPR(MISSING, NORMAL, LOW, EMPTY) */	\
		
		"17,10,04,NORMAL,10,LEFT,:"								/* 17:ATM SERIAL NUMBER(NNNNNNNN) */	\
		"18,00,00,NORMAL,00,LEFT,:"								/* 18:BRANCH NUMBER(NNNNNNNN) */	\
		"19,30,04,NORMAL,10,LEFT,:"								/* 19:AP VERSION(V00-00-00) */	\
		"20,00,00,NORMAL,00,LEFT,####.##.##:"					/* 20:TRANSACTION DATE(YYYYMMDD) */ \

		"21,37,15,NORMAL,02,LEFT,##:"							/* 21:MENU CHOICE */	\

		"48,00,15,NORMAL,20,LEFT,:"								/* 48:MESSAGE */	\
		"",
		// SCREEN ACCEPT(INDEX,NO,XX,YY,LENGTH,ECHOMODE,ATTRIBUTE,INPUTTYPE,CURSORON,FLUSH,AUTOEND,TIMEOUT)
		"0,21,37,15,02,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 21:MENU CHOICE */	\
		"",

/////////////////////////////////////////////////////////////////////////////
		"ADDADMIN",												// 23
		923,
		// SCREEN DATA
//		 0123456789012345678901234567890123456789
/*0*/	"                                        ",
/*1*/	"     ��������Ҫ��ӵĹ�Ա��ź�����     ",
/*2*/	"                                        ",
/*3*/	"             ѡ��'���'����             ",
/*4*/	"                                        ",
/*5*/	"1>            ���:XX                   ",
/*6*/	"                                        ",
/*7*/	"2>            ����:                     ",
/*8*/	"                                        ",
/*9*/	"                                        ",
/*0*/	"                                        ",
/*1*/	"                                        ",
/*2*/	"                                        ",
/*3*/	"1~9:�����к�,ENTER:��һ��,CANCEL:ȡ��   ",
/*4*/	"11:���,88:�˳�                         ",
/*5*/	"                      ������������:NN ",
		// SCREEN DISPLAY(NO,XX,YY,ATTRIBUTE,LENGTH,JUSTIFIER,FORMAT)
		"00,00,13,INVERTED,03,LEFT,1~9:"						\
		"00,13,13,INVERTED,05,LEFT,ENTER:"						\
		"00,26,13,INVERTED,06,LEFT,CANCEL:"						\
		"00,00,14,INVERTED,02,LEFT,11:"							\
		"00,08,14,INVERTED,02,LEFT,88:"							\

		"01,19,05,NORMAL,02,LEFT,:"								/* 01:OP NUMBER */	\
		"02,19,07,NORMAL,06,LEFT,******:"						/* 02:PASSWORD */	\
		"03,37,15,NORMAL,02,LEFT,##:"							/* 03:MENU CHOICE */	\
		
		"48,00,15,NORMAL,20,LEFT,:"								/* 48:MESSAGE */	\
		"",
		// SCREEN ACCEPT(INDEX,NO,XX,YY,LENGTH,ECHOMODE,ATTRIBUTE,INPUTTYPE,CURSORON,FLUSH,AUTOEND,TIMEOUT)
		"1,01,19,05,02,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 01:OP NUMBER */	\
		"2,02,19,07,06,PASSWORD,NORMAL,REALNUMERIC,1,1,1,-1:"	/* 02:PASSWORD */	\

		"0,03,37,15,02,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 03:MENU CHOICE */	\
		"",

/////////////////////////////////////////////////////////////////////////////
		"DELADMIN",												// 24
		924,
		// SCREEN DATA
//		 0123456789012345678901234567890123456789
/*0*/	"                                        ",
/*1*/	"        ��������Ҫȡ���Ĺ�Ա���        ",
/*2*/	"                                        ",
/*3*/	"             ѡ��'ɾ��'����             ",
/*4*/	"                                        ",
/*5*/	"1>            ���:XX                   ",
/*6*/	"                                        ",
/*7*/	"                                        ",
/*8*/	"                                        ",
/*9*/	"                                        ",
/*0*/	"                                        ",
/*1*/	"                                        ",
/*2*/	"                                        ",
/*3*/	"1~9:�����к�,ENTER:��һ��,CANCEL:ȡ��   ",
/*4*/	"11:ɾ��,88:�˳�                         ",
/*5*/	"                      ������������:NN ",
		// SCREEN DISPLAY(NO,XX,YY,ATTRIBUTE,LENGTH,JUSTIFIER,FORMAT)
		"00,00,13,INVERTED,03,LEFT,1~9:"						\
		"00,13,13,INVERTED,05,LEFT,ENTER:"						\
		"00,26,13,INVERTED,06,LEFT,CANCEL:"						\
		"00,00,14,INVERTED,02,LEFT,11:"							\
		"00,08,14,INVERTED,02,LEFT,88:"							\

		"01,19,05,NORMAL,02,LEFT,:"								/* 01:OP NUMBER */	\

		"02,37,15,NORMAL,02,LEFT,##:"							/* 02:MENU CHOICE */	\
		
		"48,00,15,NORMAL,20,LEFT,:"								/* 48:MESSAGE */	\
		"",
		// SCREEN ACCEPT(INDEX,NO,XX,YY,LENGTH,ECHOMODE,ATTRIBUTE,INPUTTYPE,CURSORON,FLUSH,AUTOEND,TIMEOUT)
		"1,01,19,05,02,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 01:OP NUMBER */	\

		"0,02,37,15,02,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 02:MENU CHOICE */	\
		"",

/////////////////////////////////////////////////////////////////////////////
		"CHANGEADMINPIN",										// 25
		925,
		// SCREEN DATA
//		 0123456789012345678901234567890123456789
/*0*/	"                                        ",
/*1*/	"     �������Ա���,�������������      ",
/*2*/	"                                        ",
/*3*/	"           ѡ��'�޸�����'����           ",
/*4*/	"                                        ",
/*5*/	"1>      ��      ��:XX                   ",
/*6*/	"                                        ",
/*7*/	"2>      ��  ��  ��:                     ",
/*8*/	"                                        ",
/*9*/	"3>      ��  ��  ��:                     ",
/*0*/	"                                        ",
/*1*/	"4>      ������ȷ��:                     ",
/*2*/	"                                        ",
/*3*/	"1~9:�����к�,ENTER:��һ��,CANCEL:ȡ��   ",
/*4*/	"11:�޸�����,88:�˳�                     ",
/*5*/	"                      ������������:NN ",
		// SCREEN DISPLAY(NO,XX,YY,ATTRIBUTE,LENGTH,JUSTIFIER,FORMAT)
		"00,00,13,INVERTED,03,LEFT,1~9:"						\
		"00,13,13,INVERTED,05,LEFT,ENTER:"						\
		"00,26,13,INVERTED,06,LEFT,CANCEL:"						\
		"00,00,14,INVERTED,02,LEFT,11:"							\
		"00,12,14,INVERTED,02,LEFT,88:"							\

		"01,19,05,NORMAL,02,LEFT,:"								/* 01:OP NUMBER */	\
		"02,19,07,NORMAL,06,LEFT,******:"						/* 02:PASSWORD */	\
		"03,19,09,NORMAL,06,LEFT,******:"						/* 03:PASSWORD */	\
		"04,19,11,NORMAL,06,LEFT,******:"						/* 04:PASSWORD */	\
		"05,37,15,NORMAL,02,LEFT,##:"							/* 05:MENU CHOICE */	\
		
		"48,00,15,NORMAL,20,LEFT,:"								/* 48:MESSAGE */	\
		"",
		// SCREEN ACCEPT(INDEX,NO,XX,YY,LENGTH,ECHOMODE,ATTRIBUTE,INPUTTYPE,CURSORON,FLUSH,AUTOEND,TIMEOUT)
		"1,01,19,05,02,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 01:OP NUMBER */	\
		"2,02,19,07,06,PASSWORD,NORMAL,REALNUMERIC,1,1,1,-1:"	/* 02:OP OLD PASSWORD */	\
		"3,03,19,09,06,PASSWORD,NORMAL,REALNUMERIC,1,1,1,-1:"	/* 03:OP NEW PASSWORD */	\
		"4,04,19,11,06,PASSWORD,NORMAL,REALNUMERIC,1,1,1,-1:"	/* 04:OP NEW PASSWORD AGAIN */	\

		"0,05,37,15,02,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 05:MENU CHOICE */	\
		"",

/////////////////////////////////////////////////////////////////////////////
		"BATCHTOTAL",											// 26
		926,
		// SCREEN DATA
//		 0123456789012345678901234567890123456789
/*0*/	"                                        ",
/*1*/	"                                        ",
/*2*/	"                ATMP�ս�                ",
/*3*/	"                                        ",
/*4*/	"  ��ǰ���κ�:XXXXXXXXXXXXXX             ",
/*5*/	"                                        ",
/*6*/	"1>ָ�����κ�:XXXXXXXXXXXXXX             ",
/*7*/	"                                        ",
/*8*/	"                                        ",
/*9*/	"                                        ",
/*0*/	"                                        ",
/*1*/	"                                        ",
/*2*/	"                                        ",
/*3*/	"1~9:�����к�,ENTER:��һ��,CANCEL:ȡ��   ",
/*4*/	"11:��ǰ���κ�,12:ָ�����κ�,88:�˳�     ",
/*5*/	"                      ������������:NN ",
		// SCREEN DISPLAY(NO,XX,YY,ATTRIBUTE,LENGTH,JUSTIFIER,FORMAT)
		"00,00,13,INVERTED,03,LEFT,1~9:"						\
		"00,13,13,INVERTED,05,LEFT,ENTER:"						\
		"00,26,13,INVERTED,06,LEFT,CANCEL:"						\
		"00,00,14,INVERTED,02,LEFT,11:"							\
		"00,14,14,INVERTED,02,LEFT,12:"							\
		"00,28,14,INVERTED,02,LEFT,88:"							\

		"01,13,04,NORMAL,14,LEFT,:"								/* 01:CURRENT BATCH NO */	\
		"02,13,06,NORMAL,14,LEFT,:"	                  			/* 02:SETTING BATCH NO */	\

		"03,37,15,NORMAL,02,LEFT,##:"							/* 03:MENU CHOICE */	\
		
		"48,00,15,NORMAL,20,LEFT,:"								/* 48:MESSAGE */	\
		"",
		// SCREEN ACCEPT(INDEX,NO,XX,YY,LENGTH,ECHOMODE,ATTRIBUTE,INPUTTYPE,CURSORON,FLUSH,AUTOEND,TIMEOUT)
		"1,02,13,06,14,NORMAL,NORMAL,REALNUMERIC,1,1,1,-1:"		/* 02:SETTING BATCH NO */	\

		"0,03,37,15,02,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 03:MENU CHOICE */	\
		"",

/////////////////////////////////////////////////////////////////////////////
		"USERDEFINE",											// 27
		927,
		// SCREEN DATA
//		 0123456789012345678901234567890123456789
/*0*/	" ����1:9999(100)     ����2:9999(100)    ",
/*1*/	" ����3:9999(100)     ����4:9999(100)    ",
/*2*/	" ������:���� ������:���� ƾ����ӡ��:����",
/*3*/	" �ϳ���:���� �̿���:999  ��ˮ��ӡ��:����",
/*4*/	" �豸���:12345678   AP�汾��:V01-01-01 ",
/*5*/	"                                        ",
/*6*/	"01)���ؿ���                             ",
/*7*/	"                                        ",
/*8*/	"02)Ӧ�ò�������                         ",
/*9*/	"                                        ",
/*0*/	"03)��������Կ                           ",
/*1*/	"                                        ",
/*2*/	"                                        ",
/*3*/	"                                        ",
/*4*/	"                    10)�˳�             ",
/*5*/	"                      ������������:NN ",
		// SCREEN DISPLAY(NO,XX,YY,ATTRIBUTE,LENGTH,JUSTIFIER,FORMAT)
		"01,07,00,NORMAL,09,LEFT,:"								/* 01:CST1(CCCC(GGG)) */	\
		"02,27,00,NORMAL,09,LEFT,:"								/* 02:CST2(CCCC(GGG)) */	\
		"03,07,01,NORMAL,09,LEFT,:"								/* 03:CST3(CCCC(GGG)) */	\
		"04,27,01,NORMAL,09,LEFT,:"								/* 04:CST4(CCCC(GGG)) */	\
		"05,08,02,NORMAL,04,LEFT,:"								/* 05:CDU(NORMAL, AUTO-OFF, MISSING, ABNORMAL, ERROR, OFF) */	\
		"06,20,02,NORMAL,04,LEFT,:"								/* 06:MCU(NORMAL, AUTO-OFF, MISSING, ABNORMAL, ERROR, OFF) */	\
		"07,36,02,NORMAL,04,LEFT,:"								/* 07:SPR(NORMAL, AUTO-OFF, MISSING, ABNORMAL, ERROR, OFF) */	\
		"08,36,03,NORMAL,04,LEFT,:"								/* 08:JPR(NORMAL, AUTO-OFF, MISSING, ABNORMAL, ERROR, OFF) */	\
		
		"09,00,00,NORMAL,00,LEFT,:"								/* 09:CST1(MISSING, NORMAL, LOW, EMPTY, FULL) */	\
		"10,00,00,NORMAL,00,LEFT,:"								/* 10:CST2(MISSING, NORMAL, LOW, EMPTY, FULL) */	\
		"11,00,00,NORMAL,00,LEFT,:"								/* 11:CST3(MISSING, NORMAL, LOW, EMPTY, FULL) */	\
		"12,00,00,NORMAL,00,LEFT,:"								/* 12:CST4(MISSING, NORMAL, LOW, EMPTY, FULL) */	\
		"13,08,03,NORMAL,04,LEFT,:"								/* 13:REJECT(MISSING, NORMAL, LOW, EMPTY, FULL) */	\
		"14,20,03,NORMAL,03,LEFT,:"								/* 14:MCU(C) */	\
		"15,00,00,NORMAL,00,LEFT,:"								/* 15:SPR(MISSING, NORMAL, LOW, EMPTY) */	\
		"16,00,00,NORMAL,00,LEFT,:"								/* 16:JPR(MISSING, NORMAL, LOW, EMPTY) */	\
		
		"17,10,04,NORMAL,10,LEFT,:"								/* 17:ATM SERIAL NUMBER(NNNNNNNN) */	\
		"18,00,00,NORMAL,00,LEFT,:"								/* 18:BRANCH NUMBER(NNNNNNNN) */	\
		"19,30,04,NORMAL,10,LEFT,:"								/* 19:AP VERSION(V00-00-00) */	\
		"20,00,00,NORMAL,00,LEFT,####.##.##:"					/* 20:TRANSACTION DATE(YYYYMMDD) */ \

		"21,37,15,NORMAL,02,LEFT,##:"							/* 21:MENU CHOICE */	\

		"48,00,15,NORMAL,20,LEFT,:"								/* 48:MESSAGE */	\
		"",
		// SCREEN ACCEPT(INDEX,NO,XX,YY,LENGTH,ECHOMODE,ATTRIBUTE,INPUTTYPE,CURSORON,FLUSH,AUTOEND,TIMEOUT)
		"0,21,37,15,02,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 21:MENU CHOICE */	\
		"",

/////////////////////////////////////////////////////////////////////////////
//liyi add 2008.09.23
		"SETPARAM",												// 28
		928,
		// SCREEN DATA
//		 0123456789012345678901234567890123456789
/*0*/	"1>�������:XX  ���ʱ��:XX  ���׳�ʱ:XXX",
/*1*/	"2>�޲����ȴ�:XXX�̿�ʱ��:XXX�̳�ʱ��:XXX",
/*2*/	"3>��ʱ״̬���:XXX      ��ѯ��ϸ����:XXX",
/*3*/	"4>�쳣֪ͨ����:XXX      �Զ��ط�����:XXX",
/*4*/	"5>�Զ���������:XXX     ����ȡ�����:XXXX",
/*5*/	"6>����ȡ����С:XXXX  ���ʻ�����:XXXXXX",
/*6*/	"7>���ʻ����С:XXXXXX����ת�����:XXXXXX",
/*7*/	"8>����ת����С:XXXX�����������:XXXXXXXX",
/*8*/	"9>����ת�������:XXXXXXXX  ��С:XXXXXXXX",
/*9*/	"  ����ת�������:XXXXXXXX  ��С:XXXXXXXX",
/*0*/	"  ֪ͨת�������:XXXXXXXX  ��С:XXXXXXXX",
/*1*/	"  ����ת�������:XXXXXXXX  ��С:XXXXXXXX",
/*2*/	"  ����ת֪ͨ���:XXXXXXXX  ��С:XXXXXXXX",
/*3*/	"  ����ת�������:XXXXXXXX  ��С:XXXXXXXX",
/*4*/	"1~9:�����к�,ENTER:��һ��,CANCEL:ȡ��   ",
/*5*/	"           11����,12��ӡ,88:�˳� ����:NN",
		// SCREEN DISPLAY(NO,XX,YY,ATTRIBUTE,LENGTH,JUSTIFIER,FORMAT)
		"00,00,14,INVERTED,03,LEFT,1~9:"						\
		"00,13,14,INVERTED,05,LEFT,ENTER:"						\
		"00,26,14,INVERTED,06,LEFT,CANCEL:"						\
		"00,11,15,INVERTED,02,LEFT,11:"							\
		"00,18,15,INVERTED,02,LEFT,12:"							\
		"00,25,15,INVERTED,02,LEFT,88:"							\

		"01,11,00,NORMAL,02,LEFT,:"								/* 01:ADV NUMBER(NN) */	\
		"02,24,00,NORMAL,02,LEFT,:"								/* 02:ADV INTERVAL(NN) */	\
		"03,37,00,NORMAL,03,LEFT,:"								/* 03:TRANS TIMEOUT(NNN) */	\
		"04,13,01,NORMAL,03,LEFT,:"								/* 04:OP TIMEOUT(NNN) */	\
		"05,25,01,NORMAL,03,LEFT,:"								/* 05:RETRACT CARD(NNN) */	\
		"06,37,01,NORMAL,03,LEFT,:"								/* 06:RETRACT CASH(NNN) */	\
		"07,15,02,NORMAL,03,LEFT,:"								/* 07:MSR TIME(NNN) */	\
		"08,37,02,NORMAL,03,LEFT,:"								/* 08:DETAIL NUMBER(NNN) */	\
		"09,15,03,NORMAL,03,LEFT,:"								/* 09:EX TIMES(NNN) */	\
		"10,37,03,NORMAL,03,LEFT,:"								/* 10:RESEND TIMES(NNN) */	\
		"11,15,04,NORMAL,03,LEFT,:"								/* 11:CWC TIMES(NNN) */	\
		"12,36,04,NORMAL,04,LEFT,:"								/* 12:CWD MAX(NNNN) */	\
		"13,15,05,NORMAL,04,LEFT,:"								/* 13:CWD MIN(NNNN) */	\
		"14,34,05,NORMAL,06,LEFT,:"								/* 14:REMIT MAX(NNNNNN) */	\
		"15,15,06,NORMAL,06,LEFT,:"								/* 15:REMIT MIN(NNNNNN) */	\
		"16,34,06,NORMAL,06,LEFT,:"								/* 16:TFR MAX(NNNNNN) */	\
		"17,15,07,NORMAL,04,LEFT,:"								/* 17:TFR MIN(NNNN) */	\
		"18,32,07,NORMAL,08,LEFT,:"								/* 18:�����������(NNNNNNNN) */	\
		"19,17,08,NORMAL,08,LEFT,:"								/* 19:����ת�������(NNNNNNNN) */	\
		"20,32,08,NORMAL,08,LEFT,:"								/* 20:��С(NNNNNNNN) */	\
		"21,17,09,NORMAL,08,LEFT,:"								/* 21:����ת�������(NNNNNNNN) */	\
		"22,32,09,NORMAL,08,LEFT,:"								/* 22:��С(NNNNNNNN) */	\
		"23,17,10,NORMAL,08,LEFT,:"								/* 23:֪ͨת�������(NNNNNNNN) */	\
		"24,32,10,NORMAL,08,LEFT,:"								/* 24:��С(NNNNNNNN) */	\
		"25,17,11,NORMAL,08,LEFT,:"								/* 25:����ת�������(NNNNNNNN) */	\
		"26,32,11,NORMAL,08,LEFT,:"								/* 26:��С(NNNNNNNN) */	\
		"27,17,12,NORMAL,08,LEFT,:"								/* 27:����ת֪ͨ���(NNNNNNNN) */	\
		"28,32,12,NORMAL,08,LEFT,:"								/* 28:��С(NNNNNNNN) */	\
		"29,17,13,NORMAL,08,LEFT,:"								/* 29:����ת�������(NNNNNNNN) */	\
		"30,32,13,NORMAL,08,LEFT,:"								/* 30:��С(NNNNNNNN) */	\
		
		"31,38,15,NORMAL,02,LEFT,##:"							/* 31:MENU CHOICE */	\
		
		"48,00,15,NORMAL,10,LEFT,:"								/* 48:MESSAGE */	\
		"",
		// SCREEN ACCEPT(INDEX,NO,XX,YY,LENGTH,ECHOMODE,ATTRIBUTE,INPUTTYPE,CURSORON,FLUSH,AUTOEND,TIMEOUT)
		"1,01,11,00,02,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 01:ADV NUMBER(NN) */	\
		"*,02,24,00,02,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 02:ADV INTERVAL(NN) */	\
		"*,03,37,00,03,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 03:TRANS TIMEOUT(NNN) */	\
		"2,04,13,01,03,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 04:OP TIMEOUT(NNN) */	\
		"*,05,25,01,03,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 05:RETRACT CARD(NNN) */	\
		"*,06,37,01,03,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 06:RETRACT CASH(NNN) */	\
		"3,07,15,02,03,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 07:MSR TIME(NNN) */	\
		"*,08,37,02,03,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 08:DETAIL NUMBER(NNN) */	\
		"4,09,15,03,03,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 09:EX TIMES(NNN) */	\
		"*,10,37,03,03,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 10:RESEND TIMES(NNN) */	\
		"5,11,15,04,03,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 11:CWC TIMES(NNN) */	\
		"*,12,36,04,04,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 12:CWD MAX(NNNN) */	\
		"6,13,15,05,04,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 13:CWD MIN(NNNN) */	\
		"*,14,34,05,06,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 14:REMIT MAX(NNNNNN) */	\
		"7,15,15,06,06,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 15:REMIT MIN(NNNNNN) */	\
		"*,16,34,06,06,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 16:TFR MAX(NNNNNN) */	\
		"8,17,15,07,04,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 17:TFR MIN(NNNNNN) */	\
		"*,18,32,07,08,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 17:TFR MIN(NNNNNN) */	\
		"9,19,18,08,08,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 17:TFR MIN(NNNNNN) */	\
		"*,20,32,08,08,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 17:TFR MIN(NNNNNN) */	\
		"10,21,18,09,08,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"		/* 17:TFR MIN(NNNNNN) */	\
		"*,22,32,09,08,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 17:TFR MIN(NNNNNN) */	\
		"11,23,18,10,08,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"		/* 17:TFR MIN(NNNNNN) */	\
		"*,24,32,10,08,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 17:TFR MIN(NNNNNN) */	\
		"12,25,18,11,08,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"		/* 17:TFR MIN(NNNNNN) */	\
		"*,26,32,11,08,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 17:TFR MIN(NNNNNN) */	\
		"13,27,18,12,08,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"		/* 17:TFR MIN(NNNNNN) */	\
		"*,28,32,12,08,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 17:TFR MIN(NNNNNN) */	\
		"14,29,18,13,08,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"		/* 17:TFR MIN(NNNNNN) */	\
		"*,30,32,13,08,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 17:TFR MIN(NNNNNN) */	\

		"0,31,38,15,02,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 18:MENU CHOICE */	\
		"",

/////////////////////////////////////////////////////////////////////////////

		"SETMASTERKEY",											// 29
		929,
		// SCREEN DATA
//		 0123456789012345678901234567890123456789
/*0*/	"                                        ",
/*1*/	"              ����������Կ              ",
/*2*/	"                                        ",
/*3*/	"1>         [XXXXXXXXXXXXXXXX]           ",
/*4*/	"                                        ",
/*5*/	"                                        ",
/*6*/	"                                        ",
/*7*/	"                                        ",
/*8*/	"                                        ",
/*9*/	"                                        ",
/*0*/	"                                        ",
/*1*/	"                                        ",
/*2*/	"                                        ",
/*3*/	"1~9:�����к�,ENTER:��һ��,CANCEL:ȡ��   ",
/*4*/	"11:����,88:�˳�                         ",
/*5*/	"                      ������������:NN ",
		// SCREEN DISPLAY(NO,XX,YY,ATTRIBUTE,LENGTH,JUSTIFIER,FORMAT)
		"00,00,13,INVERTED,03,LEFT,1~9:"						\
		"00,13,13,INVERTED,05,LEFT,ENTER:"						\
		"00,26,13,INVERTED,06,LEFT,CANCEL:"						\
		"00,00,14,INVERTED,02,LEFT,11:"							\
		"00,08,14,INVERTED,02,LEFT,88:"							\

		"01,12,03,NORMAL,16,LEFT,:"								/* 01:MASTER KEY */	\

		"02,37,15,NORMAL,02,LEFT,##:"							/* 02:MENU CHOICE */	\
		
		"48,00,15,NORMAL,20,LEFT,:"								/* 48:MESSAGE */	\
		"",
		// SCREEN ACCEPT(INDEX,NO,XX,YY,LENGTH,ECHOMODE,ATTRIBUTE,INPUTTYPE,CURSORON,FLUSH,AUTOEND,TIMEOUT)
		"1,01,12,03,16,NORMAL,NORMAL,ALPHANUMERIC,1,1,1,-1:"	/* 01:MASTER KEY */	\

		"0,02,37,15,02,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 02:MENU CHOICE */	\
		"",


/////////////////////////////////////////////////////////////////////////////
		"INPUTAUTHCODE",										// 32
		932,
		// SCREEN DATA
//		 0123456789012345678901234567890123456789
/*0*/	"                                        ",
/*1*/	"               ��Ȩ��Ϣ��               ",
/*2*/	"                                        ",
/*3*/	"         [XXXXXXXXXXXXXXXXXXXX]         ",
/*4*/	"                                        ",
/*5*/	"                                        ",
/*6*/	"              ��������Ȩ��              ",
/*7*/	"                                        ",
/*8*/	"1>         [XXXXXXXXXXXXXXXX]           ",
/*9*/	"                                        ",
/*0*/	"                                        ",
/*1*/	"                                        ",
/*2*/	"                                        ",
/*3*/	"1~9:�����к�,ENTER:��һ��,CANCEL:ȡ��   ",
/*4*/	"11:����,88:�˳�                         ",
/*5*/	"                      ������������:NN ",
		// SCREEN DISPLAY(NO,XX,YY,ATTRIBUTE,LENGTH,JUSTIFIER,FORMAT)
		"00,00,14,INVERTED,02,LEFT,11:"							\
		"00,08,14,INVERTED,02,LEFT,88:"							\

		"01,10,03,NORMAL,20,LEFT,:"								/* 01:AUTH INFORMATION */	\

		"02,12,08,NORMAL,16,LEFT,:"								/* 02:AUTH CODE */	\

		"03,37,15,NORMAL,02,LEFT,##:"							/* 03:MENU CHOICE */	\
		
		"48,00,15,NORMAL,20,LEFT,:"								/* 48:MESSAGE */	\
		"",
		// SCREEN ACCEPT(INDEX,NO,XX,YY,LENGTH,ECHOMODE,ATTRIBUTE,INPUTTYPE,CURSORON,FLUSH,AUTOEND,TIMEOUT)
		"1,02,12,08,16,NORMAL,NORMAL,ALPHANUMERIC,1,1,1,-1:"	/* 02:Auth Code */	

		"0,03,37,15,02,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 03:MENU CHOICE */	\
		"",


/////////////////////////////////////////////////////////////////////////////
		"INPUTMASTERKEY_A",										// 41
		941,
		// SCREEN DATA
//		 0123456789012345678901234567890123456789
/*0*/	"                                        ",
/*1*/	"           ����������ԿA����            ",
/*2*/	"                                        ",
/*3*/	"1> [XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX]   ",
/*4*/	"                                        ",
/*5*/	"                                        ",
/*6*/	"                                        ",
/*7*/	"                                        ",
/*8*/	"                                        ",
/*9*/	"                                        ",
/*0*/	"                                        ",
/*1*/	"                                        ",
/*2*/	"                                        ",
/*3*/	"1~9:�����к�,ENTER:��һ��,CANCEL:ȡ��   ",
/*4*/	"11:ȷ��,88:�˳�                         ",
/*5*/	"                      ������������:NN ",
		// SCREEN DISPLAY(NO,XX,YY,ATTRIBUTE,LENGTH,JUSTIFIER,FORMAT)
		"00,00,13,INVERTED,03,LEFT,1~9:"						\
		"00,13,13,INVERTED,05,LEFT,ENTER:"						\
		"00,26,13,INVERTED,06,LEFT,CANCEL:"						\
		"00,00,14,INVERTED,02,LEFT,11:"							\
		"00,08,14,INVERTED,02,LEFT,88:"							\

		"01,04,03,NORMAL,32,LEFT,:"								/* 01:MASTER KEY */	\

		"02,37,15,NORMAL,02,LEFT,##:"							/* 02:MENU CHOICE */	\
		
		"48,00,15,NORMAL,20,LEFT,:"								/* 48:MESSAGE */	\
		"",
		// SCREEN ACCEPT(INDEX,NO,XX,YY,LENGTH,ECHOMODE,ATTRIBUTE,INPUTTYPE,CURSORON,FLUSH,AUTOEND,TIMEOUT)
		"1,01,04,03,32,NORMAL,NORMAL,ALPHANUMERIC,1,1,1,-1:"	/* 01:MASTER KEY */	\

		"0,02,37,15,02,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 02:MENU CHOICE */	\
		"",

/////////////////////////////////////////////////////////////////////////////
		"INPUTMASTERKEY_B",										// 42
		942,
		// SCREEN DATA
//		 0123456789012345678901234567890123456789
/*0*/	"                                        ",
/*1*/	"           ����������ԿB����            ",
/*2*/	"                                        ",
/*3*/	"1> [XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX]   ",
/*4*/	"                                        ",
/*5*/	"                                        ",
/*6*/	"                                        ",
/*7*/	"                                        ",
/*8*/	"                                        ",
/*9*/	"                                        ",
/*0*/	"                                        ",
/*1*/	"                                        ",
/*2*/	"                                        ",
/*3*/	"1~9:�����к�,ENTER:��һ��,CANCEL:ȡ��   ",
/*4*/	"11:ȷ��,88:�˳�                         ",
/*5*/	"                      ������������:NN ",
		// SCREEN DISPLAY(NO,XX,YY,ATTRIBUTE,LENGTH,JUSTIFIER,FORMAT)
		"00,00,13,INVERTED,03,LEFT,1~9:"						\
		"00,13,13,INVERTED,05,LEFT,ENTER:"						\
		"00,26,13,INVERTED,06,LEFT,CANCEL:"						\
		"00,00,14,INVERTED,02,LEFT,11:"							\
		"00,08,14,INVERTED,02,LEFT,88:"							\

		"01,04,03,NORMAL,32,LEFT,:"								/* 01:MASTER KEY */	\

		"02,37,15,NORMAL,02,LEFT,##:"							/* 02:MENU CHOICE */	\

		"24,04,08,NORMAL,30,LEFT,:"								/* 24:CHECKVALUE */	\
		
		"48,00,15,NORMAL,20,LEFT,:"								/* 48:MESSAGE */	\
		"",
		// SCREEN ACCEPT(INDEX,NO,XX,YY,LENGTH,ECHOMODE,ATTRIBUTE,INPUTTYPE,CURSORON,FLUSH,AUTOEND,TIMEOUT)
		"1,01,04,03,32,NORMAL,NORMAL,ALPHANUMERIC,1,1,1,-1:"	/* 01:MASTER KEY */	\

		"0,02,37,15,02,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 02:MENU CHOICE */	\
		"",

/////////////////////////////////////////////////////////////////////////////
		"ADDCASHINPUTCOUNTER",										// 43
		943,
		// SCREEN DATA
//		 0123456789012345678901234567890123456789
/*0*/	"                                        ",
/*1*/	"              �������Ա��              ",
/*2*/	"                                        ",
/*3*/	"1>              [XXXXXXX]               ",
/*4*/	"                                        ",
/*5*/	"                                        ",
/*6*/	"                                        ",
/*7*/	"                                        ",
/*8*/	"                                        ",
/*9*/	"                                        ",
/*0*/	"                                        ",
/*1*/	"                                        ",
/*2*/	"                                        ",
/*3*/	"1~9:�����к�,ENTER:��һ��,CANCEL:ȡ��   ",
/*4*/	"11:ȷ��,88:�˳�                         ",
/*5*/	"                      ������������:NN ",
		// SCREEN DISPLAY(NO,XX,YY,ATTRIBUTE,LENGTH,JUSTIFIER,FORMAT)
		"00,00,13,INVERTED,03,LEFT,1~9:"						\
		"00,13,13,INVERTED,05,LEFT,ENTER:"						\
		"00,26,13,INVERTED,06,LEFT,CANCEL:"						\
		"00,00,14,INVERTED,02,LEFT,11:"							\
		"00,08,14,INVERTED,02,LEFT,88:"							\
		
		"01,17,03,NORMAL,7,LEFT,:"								/* 01:COUNTER NUMBER */	\
		
		"02,37,15,NORMAL,02,LEFT,##:"							/* 02:MENU CHOICE */	\
		
		"48,00,15,NORMAL,20,LEFT,:"								/* 48:MESSAGE */	\
		"",
		// SCREEN ACCEPT(INDEX,NO,XX,YY,LENGTH,ECHOMODE,ATTRIBUTE,INPUTTYPE,CURSORON,FLUSH,AUTOEND,TIMEOUT)
		"1,01,17,03,7,NORMAL,NORMAL,ALPHANUMERIC,1,1,1,-1:"		/* 01:MASTER KEY */	\
		
		"0,02,37,15,02,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 02:MENU CHOICE */	\
		"",
		
/////////////////////////////////////////////////////////////////////////////
		"TOTALINPUTAMOUNT",										// 43
		944,
		// SCREEN DATA
//		 0123456789012345678901234567890123456789
/*0*/	"                                        ",
/*1*/	"           ����������ʣ����           ",
/*2*/	"                                        ",
/*3*/	"1>             [XXXXXXX]Ԫ              ",
/*4*/	"                                        ",
/*5*/	"  ��ʾ:���˽��Ϊȫ������ʣ���ֽ���ܺ� ",
/*6*/	"        (�����ϳ���,��ȡ�������)       ",
/*7*/	"                                        ",
/*8*/	"                                        ",
/*9*/	"                                        ",
/*0*/	"                                        ",
/*1*/	"                                        ",
/*2*/	"                                        ",
/*3*/	"1~9:�����к�,ENTER:��һ��,CANCEL:ȡ��   ",
/*4*/	"11:ȷ��,88:�˳�                         ",
/*5*/	"                      ������������:NN ",
		// SCREEN DISPLAY(NO,XX,YY,ATTRIBUTE,LENGTH,JUSTIFIER,FORMAT)
		"00,00,13,INVERTED,03,LEFT,1~9:"						\
		"00,13,13,INVERTED,05,LEFT,ENTER:"						\
		"00,26,13,INVERTED,06,LEFT,CANCEL:"						\
		"00,00,14,INVERTED,02,LEFT,11:"							\
		"00,08,14,INVERTED,02,LEFT,88:"							\
		
		"01,16,03,NORMAL,7,LEFT,:"								/* 01:REMAIN AMOUNT */	\
		
		"02,37,15,NORMAL,02,LEFT,##:"							/* 02:MENU CHOICE */	\
		
		"48,00,15,NORMAL,20,LEFT,:"								/* 48:MESSAGE */	\
		"",
		// SCREEN ACCEPT(INDEX,NO,XX,YY,LENGTH,ECHOMODE,ATTRIBUTE,INPUTTYPE,CURSORON,FLUSH,AUTOEND,TIMEOUT)
		"1,01,16,03,7,NORMAL,NORMAL,ALPHANUMERIC,1,1,1,-1:"		/* 01:REMAIN AMOUNT */	\
		
		"0,02,37,15,02,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 02:MENU CHOICE */	\
		"",
		
/////////////////////////////////////////////////////////////////////////////
		"OP_COUNTER_TOTAL",											// 59
		959,
		// SCREEN DATA
//		 0123456789012345678901234567890123456789
/*0*/	" ��Ϣ..............  ������............ ",
/*1*/	" ����1:9999(100)     ����2:9999(100)    ",
/*2*/	" ����3:9999(100)     ����4:9999(100)    ",
/*3*/	" ������:���� ������:���� ƾ����ӡ��:����",
/*4*/	" �ϳ���:���� �̿���:999  ��ˮ��ӡ��:����",
/*5*/	" �豸���:12345678   AP�汾��:V01-01-01 ",
/*6*/	"                                        ",
/*7*/	"                    05)�ϴ�������ϸ     ",
/*8*/	"                                        ",
/*9*/	"                    06)���Ƶ�����ˮ     ",
/*0*/	"                                        ",
/*1*/	"03)Ԥ����           07)���˷�ʽ         ",
/*2*/	"                                        ",
/*3*/	"04)����ӳ�         10)����             ",
/*4*/	"                                        ",
/*5*/	"                      ������������:NN ",
		// SCREEN DISPLAY(NO,XX,YY,ATTRIBUTE,LENGTH,JUSTIFIER,FORMAT)
		"01,00,00,NORMAL,00,LEFT,:"								/* 01:MACHINE KIND(NANOCASH, MCD3, TTW, MCD4, TTW2, EATMGBA1, EATMULL1, EATMTTW, TTW2E, MCD4E, MX8000T, MX8000L, MX5600T, MX5600L) */	\
		"02,00,00,NORMAL,00,LEFT,:"								/* 02:ATM STATUS(INIT, ERROR, CLERK, READY, CUSTOM, TRAN, DOWN) */	\
		"03,00,00,NORMAL,00,LEFT,:"								/* 03:DOOR STATUS(DOOROPEN, DOORCLOSE) */	\
		"04,00,00,NORMAL,00,LEFT,:"								/* 04:LINE STATUS(HOSTONLINE, HOSTOFFLINE) */	\
		"05,01,00,NORMAL,18,LEFT,:"								/* 05:MESSAGE */	\
		"06,21,00,NORMAL,18,LEFT,:"								/* 06:ERROR CODE([P] - [EEEEE(SS)]) */	\
		
		"07,07,01,NORMAL,09,LEFT,:"								/* 07:CST1(CCCC(GGG)) */	\
		"08,27,01,NORMAL,09,LEFT,:"								/* 08:CST2(CCCC(GGG)) */	\
		"09,07,02,NORMAL,09,LEFT,:"								/* 09:CST3(CCCC(GGG)) */	\
		"10,27,02,NORMAL,09,LEFT,:"								/* 10:CST4(CCCC(GGG)) */	\
		"11,08,03,NORMAL,04,LEFT,:"								/* 11:CDU(NORMAL, AUTO-OFF, MISSING, ABNORMAL, ERROR, OFF) */	\
		"12,20,03,NORMAL,04,LEFT,:"								/* 12:MCU(NORMAL, AUTO-OFF, MISSING, ABNORMAL, ERROR, OFF) */	\
		"13,36,03,NORMAL,04,LEFT,:"								/* 13:SPR(NORMAL, AUTO-OFF, MISSING, ABNORMAL, ERROR, OFF) */	\
		"14,36,04,NORMAL,04,LEFT,:"								/* 14:JPR(NORMAL, AUTO-OFF, MISSING, ABNORMAL, ERROR, OFF) */	\
		
		"15,00,00,NORMAL,00,LEFT,:"								/* 15:CST1(MISSING, NORMAL, LOW, EMPTY, FULL) */	\
		"16,00,00,NORMAL,00,LEFT,:"								/* 16:CST2(MISSING, NORMAL, LOW, EMPTY, FULL) */	\
		"17,00,00,NORMAL,00,LEFT,:"								/* 17:CST3(MISSING, NORMAL, LOW, EMPTY, FULL) */	\
		"18,00,00,NORMAL,00,LEFT,:"								/* 18:CST4(MISSING, NORMAL, LOW, EMPTY, FULL) */	\
		"19,08,04,NORMAL,04,LEFT,:"								/* 19:REJECT(MISSING, NORMAL, LOW, EMPTY, FULL) */	\
		"20,20,04,NORMAL,03,LEFT,:"								/* 20:MCU(C) */	\
		"21,00,00,NORMAL,00,LEFT,:"								/* 21:SPR(MISSING, NORMAL, LOW, EMPTY) */	\
		"22,00,00,NORMAL,00,LEFT,:"								/* 22:JPR(MISSING, NORMAL, LOW, EMPTY) */	\
		
		"23,10,05,NORMAL,10,LEFT,:"								/* 23:ATM SERIAL NUMBER(NNNNNNNN) */	\
		"24,00,00,NORMAL,00,LEFT,:"								/* 24:BRANCH NUMBER(NNNNNNNN) */	\
		"25,30,05,NORMAL,10,LEFT,:"								/* 25:AP VERSION(V00-00-00) */	\
		"26,00,00,NORMAL,00,LEFT,####.##.##:"					/* 26:TRANSACTION DATE(YYYYMMDD) */ \
		
		"27,37,15,NORMAL,02,LEFT,##:"							/* 27:MENU CHOICE */	\
		
		"48,00,15,NORMAL,20,LEFT,:"								/* 48:MESSAGE */	\
		"",
		// SCREEN ACCEPT(INDEX,NO,XX,YY,LENGTH,ECHOMODE,ATTRIBUTE,INPUTTYPE,CURSORON,FLUSH,AUTOEND,TIMEOUT)
		"0,27,37,15,02,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 27:MENU CHOICE */	\
		"",
				
/////////////////////////////////////////////////////////////////////////////
		"OP_COUNTER_MENU",											// 60
		960,
		// SCREEN DATA
//		 0123456789012345678901234567890123456789
/*0*/	" ��Ϣ..............  ������............ ",
/*1*/	" ����1:9999(100)     ����2:9999(100)    ",
/*2*/	" ����3:9999(100)     ����4:9999(100)    ",
/*3*/	" ������:���� ������:���� ƾ����ӡ��:����",
/*4*/	" �ϳ���:���� �̿���:999  ��ˮ��ӡ��:����",
/*5*/	" �豸���:12345678   AP�汾��:V01-01-01 ",
/*6*/	"                                        ",
/*7*/	"01)���˺�����ӳ�   05)����������     ",
/*8*/	"                                        ",
/*9*/	"                                        ",
/*0*/	"                                        ",
/*1*/	"03)�忨                                 ",
/*2*/	"                                        ",
/*3*/	"04)�鿴״̬         10)�˳�����˵�     ",
/*4*/	"                                        ",
/*5*/	"                          ���������:NN ",
		// SCREEN DISPLAY(NO,XX,YY,ATTRIBUTE,LENGTH,JUSTIFIER,FORMAT)
		"01,00,00,NORMAL,00,LEFT,:"								/* 01:MACHINE KIND(NANOCASH, MCD3, TTW, MCD4, TTW2, EATMGBA1, EATMULL1, EATMTTW, TTW2E, MCD4E, MX8000T, MX8000L, MX5600T, MX5600L) */	\
		"02,00,00,NORMAL,00,LEFT,:"								/* 02:ATM STATUS(INIT, ERROR, CLERK, READY, CUSTOM, TRAN, DOWN) */	\
		"03,00,00,NORMAL,00,LEFT,:"								/* 03:DOOR STATUS(DOOROPEN, DOORCLOSE) */	\
		"04,00,00,NORMAL,00,LEFT,:"								/* 04:LINE STATUS(HOSTONLINE, HOSTOFFLINE) */	\
		"05,01,00,NORMAL,18,LEFT,:"								/* 05:MESSAGE */	\
		"06,21,00,NORMAL,18,LEFT,:"								/* 06:ERROR CODE([P] - [EEEEE(SS)]) */	\

		"07,07,01,NORMAL,09,LEFT,:"								/* 07:CST1(CCCC(GGG)) */	\
		"08,27,01,NORMAL,09,LEFT,:"								/* 08:CST2(CCCC(GGG)) */	\
		"09,07,02,NORMAL,09,LEFT,:"								/* 09:CST3(CCCC(GGG)) */	\
		"10,27,02,NORMAL,09,LEFT,:"								/* 10:CST4(CCCC(GGG)) */	\
		"11,08,03,NORMAL,04,LEFT,:"								/* 11:CDU(NORMAL, AUTO-OFF, MISSING, ABNORMAL, ERROR, OFF) */	\
		"12,20,03,NORMAL,04,LEFT,:"								/* 12:MCU(NORMAL, AUTO-OFF, MISSING, ABNORMAL, ERROR, OFF) */	\
		"13,36,03,NORMAL,04,LEFT,:"								/* 13:SPR(NORMAL, AUTO-OFF, MISSING, ABNORMAL, ERROR, OFF) */	\
		"14,36,04,NORMAL,04,LEFT,:"								/* 14:JPR(NORMAL, AUTO-OFF, MISSING, ABNORMAL, ERROR, OFF) */	\

		"15,00,00,NORMAL,00,LEFT,:"								/* 15:CST1(MISSING, NORMAL, LOW, EMPTY, FULL) */	\
		"16,00,00,NORMAL,00,LEFT,:"								/* 16:CST2(MISSING, NORMAL, LOW, EMPTY, FULL) */	\
		"17,00,00,NORMAL,00,LEFT,:"								/* 17:CST3(MISSING, NORMAL, LOW, EMPTY, FULL) */	\
		"18,00,00,NORMAL,00,LEFT,:"								/* 18:CST4(MISSING, NORMAL, LOW, EMPTY, FULL) */	\
		"19,08,04,NORMAL,04,LEFT,:"								/* 19:REJECT(MISSING, NORMAL, LOW, EMPTY, FULL) */	\
		"20,20,04,NORMAL,03,LEFT,:"								/* 20:MCU(C) */	\
		"21,00,00,NORMAL,00,LEFT,:"								/* 21:SPR(MISSING, NORMAL, LOW, EMPTY) */	\
		"22,00,00,NORMAL,00,LEFT,:"								/* 22:JPR(MISSING, NORMAL, LOW, EMPTY) */	\

		"23,10,05,NORMAL,10,LEFT,:"								/* 23:ATM SERIAL NUMBER(NNNNNNNN) */	\
		"24,00,00,NORMAL,00,LEFT,:"								/* 24:BRANCH NUMBER(NNNNNNNN) */	\
		"25,30,05,NORMAL,10,LEFT,:"								/* 25:AP VERSION(V00-00-00) */	\
		"26,00,00,NORMAL,00,LEFT,####.##.##:"					/* 26:TRANSACTION DATE(YYYYMMDD) */ \

		"27,37,15,NORMAL,02,LEFT,##:"							/* 27:MENU CHOICE */	\

		"48,00,15,NORMAL,26,LEFT,:"								/* 48:MESSAGE */	\
		"",
		// SCREEN ACCEPT(INDEX,NO,XX,YY,LENGTH,ECHOMODE,ATTRIBUTE,INPUTTYPE,CURSORON,FLUSH,AUTOEND,TIMEOUT)
		"0,27,37,15,02,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 27:MENU CHOICE */	\
		"",

/////////////////////////////////////////////////////////////////////////////
		"OP_COUNTER_TOTAL",											// 61
		961,
		// SCREEN DATA
//		 0123456789012345678901234567890123456789
/*0*/	" ��Ϣ..............  ������............ ",
/*1*/	" ����1:9999(100)     ����2:9999(100)    ",
/*2*/	" ����3:9999(100)     ����4:9999(100)    ",
/*3*/	" ������:���� ������:���� ƾ����ӡ��:����",
/*4*/	" �ϳ���:���� �̿���:999  ��ˮ��ӡ��:����",
/*5*/	" �豸���:12345678   AP�汾��:V01-01-01 ",
/*6*/	"                                        ",
/*7*/	"                    05)�ϴ�������ϸ     ",
/*8*/	"                                        ",
/*9*/	"                    06)���Ƶ�����ˮ     ",
/*0*/	"                                        ",
/*1*/	"03)�ն�����         07)���˷�ʽ         ",
/*2*/	"                                        ",
/*3*/	"04)����ӳ�         10)����             ",
/*4*/	"                                        ",
/*5*/	"                      ������������:NN ",
		// SCREEN DISPLAY(NO,XX,YY,ATTRIBUTE,LENGTH,JUSTIFIER,FORMAT)
		"01,00,00,NORMAL,00,LEFT,:"								/* 01:MACHINE KIND(NANOCASH, MCD3, TTW, MCD4, TTW2, EATMGBA1, EATMULL1, EATMTTW, TTW2E, MCD4E, MX8000T, MX8000L, MX5600T, MX5600L) */	\
		"02,00,00,NORMAL,00,LEFT,:"								/* 02:ATM STATUS(INIT, ERROR, CLERK, READY, CUSTOM, TRAN, DOWN) */	\
		"03,00,00,NORMAL,00,LEFT,:"								/* 03:DOOR STATUS(DOOROPEN, DOORCLOSE) */	\
		"04,00,00,NORMAL,00,LEFT,:"								/* 04:LINE STATUS(HOSTONLINE, HOSTOFFLINE) */	\
		"05,01,00,NORMAL,18,LEFT,:"								/* 05:MESSAGE */	\
		"06,21,00,NORMAL,18,LEFT,:"								/* 06:ERROR CODE([P] - [EEEEE(SS)]) */	\

		"07,07,01,NORMAL,09,LEFT,:"								/* 07:CST1(CCCC(GGG)) */	\
		"08,27,01,NORMAL,09,LEFT,:"								/* 08:CST2(CCCC(GGG)) */	\
		"09,07,02,NORMAL,09,LEFT,:"								/* 09:CST3(CCCC(GGG)) */	\
		"10,27,02,NORMAL,09,LEFT,:"								/* 10:CST4(CCCC(GGG)) */	\
		"11,08,03,NORMAL,04,LEFT,:"								/* 11:CDU(NORMAL, AUTO-OFF, MISSING, ABNORMAL, ERROR, OFF) */	\
		"12,20,03,NORMAL,04,LEFT,:"								/* 12:MCU(NORMAL, AUTO-OFF, MISSING, ABNORMAL, ERROR, OFF) */	\
		"13,36,03,NORMAL,04,LEFT,:"								/* 13:SPR(NORMAL, AUTO-OFF, MISSING, ABNORMAL, ERROR, OFF) */	\
		"14,36,04,NORMAL,04,LEFT,:"								/* 14:JPR(NORMAL, AUTO-OFF, MISSING, ABNORMAL, ERROR, OFF) */	\

		"15,00,00,NORMAL,00,LEFT,:"								/* 15:CST1(MISSING, NORMAL, LOW, EMPTY, FULL) */	\
		"16,00,00,NORMAL,00,LEFT,:"								/* 16:CST2(MISSING, NORMAL, LOW, EMPTY, FULL) */	\
		"17,00,00,NORMAL,00,LEFT,:"								/* 17:CST3(MISSING, NORMAL, LOW, EMPTY, FULL) */	\
		"18,00,00,NORMAL,00,LEFT,:"								/* 18:CST4(MISSING, NORMAL, LOW, EMPTY, FULL) */	\
		"19,08,04,NORMAL,04,LEFT,:"								/* 19:REJECT(MISSING, NORMAL, LOW, EMPTY, FULL) */	\
		"20,20,04,NORMAL,03,LEFT,:"								/* 20:MCU(C) */	\
		"21,00,00,NORMAL,00,LEFT,:"								/* 21:SPR(MISSING, NORMAL, LOW, EMPTY) */	\
		"22,00,00,NORMAL,00,LEFT,:"								/* 22:JPR(MISSING, NORMAL, LOW, EMPTY) */	\

		"23,10,05,NORMAL,10,LEFT,:"								/* 23:ATM SERIAL NUMBER(NNNNNNNN) */	\
		"24,00,00,NORMAL,00,LEFT,:"								/* 24:BRANCH NUMBER(NNNNNNNN) */	\
		"25,30,05,NORMAL,10,LEFT,:"								/* 25:AP VERSION(V00-00-00) */	\
		"26,00,00,NORMAL,00,LEFT,####.##.##:"					/* 26:TRANSACTION DATE(YYYYMMDD) */ \

		"27,37,15,NORMAL,02,LEFT,##:"							/* 27:MENU CHOICE */	\

		"48,00,15,NORMAL,20,LEFT,:"								/* 48:MESSAGE */	\
		"",
		// SCREEN ACCEPT(INDEX,NO,XX,YY,LENGTH,ECHOMODE,ATTRIBUTE,INPUTTYPE,CURSORON,FLUSH,AUTOEND,TIMEOUT)
		"0,27,37,15,02,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 27:MENU CHOICE */	\
		"",

/////////////////////////////////////////////////////////////////////////////
		"OP_COUNTER_OTHER",											// 62
		962,
		// SCREEN DATA
//		 0123456789012345678901234567890123456789
/*0*/	" ��Ϣ..............  ������............ ",
/*1*/	" ����1:9999(100)     ����2:9999(100)    ",
/*2*/	" ����3:9999(100)     ����4:9999(100)    ",
/*3*/	" ������:���� ������:���� ƾ����ӡ��:����",
/*4*/	" �ϳ���:���� �̿���:999  ��ˮ��ӡ��:����",
/*5*/	" �豸���:12345678   AP�汾��:V01-01-01 ",
/*6*/	"                                        ",
/*7*/	"01)�޸Ĺ���Ա����   05)��ʼ��ƾ����ӡ�� ",
/*8*/	"                                        ",
/*9*/	"02)�ر�ϵͳ         06)��ʼ��������     ",
/*0*/	"                                        ",
/*1*/	"03)��������         07)����״̬  ����   ",
/*2*/	"                                        ",
/*3*/	"04)��ʼ����ˮ��ӡ�� 10)����             ",
/*4*/	"                                        ",
/*5*/	"                      ������������:NN ",
		// SCREEN DISPLAY(NO,XX,YY,ATTRIBUTE,LENGTH,JUSTIFIER,FORMAT)
		"01,00,00,NORMAL,00,LEFT,:"								/* 01:MACHINE KIND(NANOCASH, MCD3, TTW, MCD4, TTW2, EATMGBA1, EATMULL1, EATMTTW, TTW2E, MCD4E, MX8000T, MX8000L, MX5600T, MX5600L) */	\
		"02,00,00,NORMAL,00,LEFT,:"								/* 02:ATM STATUS(INIT, ERROR, CLERK, READY, CUSTOM, TRAN, DOWN) */	\
		"03,00,00,NORMAL,00,LEFT,:"								/* 03:DOOR STATUS(DOOROPEN, DOORCLOSE) */	\
		"04,00,00,NORMAL,00,LEFT,:"								/* 04:LINE STATUS(HOSTONLINE, HOSTOFFLINE) */	\
		"05,01,00,NORMAL,18,LEFT,:"								/* 05:MESSAGE */	\
		"06,21,00,NORMAL,18,LEFT,:"								/* 06:ERROR CODE([P] - [EEEEE(SS)]) */	\

		"07,07,01,NORMAL,09,LEFT,:"								/* 07:CST1(CCCC(GGG)) */	\
		"08,27,01,NORMAL,09,LEFT,:"								/* 08:CST2(CCCC(GGG)) */	\
		"09,07,02,NORMAL,09,LEFT,:"								/* 09:CST3(CCCC(GGG)) */	\
		"10,27,02,NORMAL,09,LEFT,:"								/* 10:CST4(CCCC(GGG)) */	\
		"11,08,03,NORMAL,04,LEFT,:"								/* 11:CDU(NORMAL, AUTO-OFF, MISSING, ABNORMAL, ERROR, OFF) */	\
		"12,20,03,NORMAL,04,LEFT,:"								/* 12:MCU(NORMAL, AUTO-OFF, MISSING, ABNORMAL, ERROR, OFF) */	\
		"13,36,03,NORMAL,04,LEFT,:"								/* 13:SPR(NORMAL, AUTO-OFF, MISSING, ABNORMAL, ERROR, OFF) */	\
		"14,36,04,NORMAL,04,LEFT,:"								/* 14:JPR(NORMAL, AUTO-OFF, MISSING, ABNORMAL, ERROR, OFF) */	\

		"15,00,00,NORMAL,00,LEFT,:"								/* 15:CST1(MISSING, NORMAL, LOW, EMPTY, FULL) */	\
		"16,00,00,NORMAL,00,LEFT,:"								/* 16:CST2(MISSING, NORMAL, LOW, EMPTY, FULL) */	\
		"17,00,00,NORMAL,00,LEFT,:"								/* 17:CST3(MISSING, NORMAL, LOW, EMPTY, FULL) */	\
		"18,00,00,NORMAL,00,LEFT,:"								/* 18:CST4(MISSING, NORMAL, LOW, EMPTY, FULL) */	\
		"19,08,04,NORMAL,04,LEFT,:"								/* 19:REJECT(MISSING, NORMAL, LOW, EMPTY, FULL) */	\
		"20,20,04,NORMAL,03,LEFT,:"								/* 20:MCU(C) */	\
		"21,00,00,NORMAL,00,LEFT,:"								/* 21:SPR(MISSING, NORMAL, LOW, EMPTY) */	\
		"22,00,00,NORMAL,00,LEFT,:"								/* 22:JPR(MISSING, NORMAL, LOW, EMPTY) */	\

		"23,10,05,NORMAL,10,LEFT,:"								/* 23:ATM SERIAL NUMBER(NNNNNNNN) */	\
		"24,00,00,NORMAL,00,LEFT,:"								/* 24:BRANCH NUMBER(NNNNNNNN) */	\
		"25,30,05,NORMAL,10,LEFT,:"								/* 25:AP VERSION(V00-00-00) */	\
		"26,00,00,NORMAL,00,LEFT,####.##.##:"					/* 26:TRANSACTION DATE(YYYYMMDD) */ \

		"27,33,11,NORMAL,04,LEFT,:"								/* 27:RULE SETTING */	\
		"28,37,15,NORMAL,02,LEFT,##:"							/* 28:MENU CHOICE */	\

		"48,00,15,NORMAL,20,LEFT,:"								/* 48:MESSAGE */	\
		"",
		// SCREEN ACCEPT(INDEX,NO,XX,YY,LENGTH,ECHOMODE,ATTRIBUTE,INPUTTYPE,CURSORON,FLUSH,AUTOEND,TIMEOUT)
		"0,28,37,15,02,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 27:MENU CHOICE */	\
		"",

/////////////////////////////////////////////////////////////////////////////
// yaokq add 20081029                                                    // 65
		"OP_VIEW_ATMCTOTALS",												
		963,
// SCREEN DATA
//		 0123456789012345678901234567890123456789
/*0*/	"             ATMC TOTAL                 ",
/*1*/	"      ATM Cassettes Total Number:X      ",
/*2*/	"        Cas.1    Cas.2    Cas.3   Cas.4 ",
/*3*/	"Value  :XXXX,    XXXX,    XXXX,    XXXX ",
/*4*/	"Last   :XXXX,    XXXX,    XXXX,    XXXX ",
/*5*/	"Left   :XXXX,    XXXX,    XXXX,    XXXX ",
/*6*/	"Disp   :XXXX,    XXXX,    XXXX,    XXXX ",
/*7*/	"PURG   :XXXX,    XXXX,    XXXX,    XXXX ",
/*8*/	"Cash Init   Amount       :XXXXXXXXXX    ",
/*9*/	"Cash Remain Amount       :XXXXXXXXXX    ",
/*0*/	"CWD Times     :XXXX  Amount :XXXXXXXXXX ",
/*1*/	"TFR Times     :XXXX  Amount :XXXXXXXXXX ",
/*2*/	"CWC Times     :XXXX  Amount :XXXXXXXXXX ",
/*3*/	"Retact Times  :XXXX  Amount :XXXXXXXXXX ",
/*4*/	"Card Retained :XXXX                     ",	
/*5*/	"               12:��ӡ, 88:�˳�  ����:NN",	
		
// SCREEN DISPLAY(NO,XX,YY,ATTRIBUTE,LENGTH,JUSTIFIER,FORMAT)
		"00,15,15,INVERTED,02,LEFT,12:"						\
		"00,24,15,INVERTED,02,LEFT,88:"						\
		"01,33,01,NORMAL,01,LEFT,:"								/* 01:ATM Cassettes Total Number(N) */	\
		"02,08,03,NORMAL,04,LEFT,:"								/* 02:Value.1(NNNN) */	\
		"03,17,03,NORMAL,04,LEFT,:"								/* 03:Value.2(NNNN) */	\
		"04,26,03,NORMAL,04,LEFT,:"								/* 04:Value.3(NNNN) */	\
		"05,35,03,NORMAL,04,LEFT,:"								/* 05:Value.4(NNNN) */	\
		"06,08,04,NORMAL,04,LEFT,:"								/* 06:Last.1(NNNN) */	\
		"07,17,04,NORMAL,04,LEFT,:"								/* 07:Last.2(NNNN) */	\
		"08,26,04,NORMAL,04,LEFT,:"								/* 08:Last.3(NNNN) */	\
		"09,35,04,NORMAL,04,LEFT,:"								/* 09:Last.4(NNNN) */	\
		"10,08,05,NORMAL,04,LEFT,:"								/* 10:Left.1(NNNN) */	\
		"11,17,05,NORMAL,04,LEFT,:"								/* 11:Left.2(NNNN) */	\
		"12,26,05,NORMAL,04,LEFT,:"								/* 12:Left.3(NNNN) */	\
		"13,35,05,NORMAL,04,LEFT,:"								/* 13:Left.4(NNNN) */	\
		"14,08,06,NORMAL,04,LEFT,:"								/* 14:Disp.1(NNNN) */	\
		"15,17,06,NORMAL,04,LEFT,:"								/* 15:Disp.2(NNNN) */	\
		"16,26,06,NORMAL,04,LEFT,:"								/* 16:Disp.3(NNNN) */	\
		"17,35,06,NORMAL,04,LEFT,:"								/* 17:Disp.4(NNNN) */	\
		"18,08,07,NORMAL,04,LEFT,:"								/* 18:PURG.1(NNNN) */	\
		"19,17,07,NORMAL,04,LEFT,:"								/* 19:PURG.2(NNNN) */	\
		"20,26,07,NORMAL,04,LEFT,:"								/* 20:PURG.3(NNNN) */	\
		"21,35,07,NORMAL,04,LEFT,:"								/* 21:PURG.4(NNNN) */	\
		"22,26,08,NORMAL,10,LEFT,:"								/* 22:RMB Cash Init Amount(NNNNNNNNNN.NN) */\
		"23,26,09,NORMAL,10,LEFT,:"								/* 23:RMB Cash Remain Amount(NNNNNNNNNN.NN) */\
		"24,15,10,NORMAL,04,LEFT,:"								/* 24:RMB CWD Times(NNNN) */	\
		"25,29,10,NORMAL,10,LEFT,:"								/* 25:RMB CWD Amount(NNNNNNNNNN.NN) */\
		"26,15,11,NORMAL,04,LEFT,:"								/* 26:RMB TFR Times(NNNN) */	\
		"27,29,11,NORMAL,10,LEFT,:"								/* 27:RMB TFR Amount(NNNNNNNNNN.NN) */\
		"28,15,12,NORMAL,04,LEFT,:"								/* 28:RRMB CWC Times(NNNN) */	\
		"29,29,12,NORMAL,10,LEFT,:"								/* 29:RMB CWC Amount(NNNNNNNNNN.NN) */\
		"30,15,13,NORMAL,04,LEFT,:"								/* 30:Momeny Retact Times(NNNN) */	\
		"31,29,13,NORMAL,10,LEFT,:"								/* 31:Momeny Retact Amount(NNNNNN.NN) */\
		"32,15,14,NORMAL,04,LEFT,:"								/* 32:Card Retained(NNNN) */	\
		
		"33,38,15,NORMAL,02,LEFT,##:"							/* 33:MENU CHOICE */	\

		"48,00,15,NORMAL,15,LEFT,:"								/* 48:MESSAGE */	\
		"",

		// SCREEN ACCEPT(INDEX,NO,XX,YY,LENGTH,ECHOMODE,ATTRIBUTE,INPUTTYPE,CURSORON,FLUSH,AUTOEND,TIMEOUT)
//		"1,01,33,01,01,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 01:ATM Cassettes Total Number(N) */\
//		"2,02,08,03,04,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 02:Value.1(NNNN) */	\
//		"*,03,17,03,04,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 03:Value.2(NNNN) */	\
//		"*,04,26,03,04,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 04:Value.3(NNNN) */	\
//		"*,05,35,03,04,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 05:Value.4(NNNN) */	\
//		"3,06,08,04,04,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 06:Last.1(NNNN) */	\
//		"*,07,17,04,04,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 07:Last.2(NNNN) */	\
//		"*,08,26,04,04,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 08:Last.3(NNNN) */	\
//		"*,09,35,04,04,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 09:Last.4(NNNN) */	\
//		"4,10,08,05,04,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 10:Left.1(NNNN) */	\
//		"*,11,17,05,04,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 11:Left.2(NNNN) */	\
//		"*,12,26,05,04,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 12:Left.3(NNNN) */	\
//		"*,13,35,05,04,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 13:Left.4(NNNN) */	\
//		"5,14,08,06,04,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 14:Disp.1(NNNN) */	\
//		"*,15,17,06,04,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 15:Disp.2(NNNN) */	\
//		"*,16,26,06,04,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 16:Disp.3(NNNN) */	\
//		"*,17,35,06,04,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 17:Disp.4(NNNN) */	\
//		"6,18,08,07,04,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 18:PURG.1(NNNN) */	\
//		"*,19,17,07,04,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 19:PURG.2(NNNN) */	\
//		"*,20,26,07,04,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 20:PURG.3(NNNN) */	\
//		"*,21,35,07,04,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 21:PURG.4(NNNN) */	\
//		"7,22,26,08,10,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 22:RMB Cash Init Amount(NNNNNNNNNN.NN) */	\
//		"8,23,26,09,10,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 23:RMB Cash Remain Amount(NNNNNNNNNN.NN) */	\
//		"9,24,15,10,04,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 24:RMB CWD Times(NNNN) */	\
//		"*,25,29,10,10,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 25:RMB CWD Amount(NNNNNNNNNN.NN) */	\
//		"10,26,15,11,04,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 26:RMB TFR Times(NNNN) */	\
//		"*,27,29,11,10,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 27:RMB TFR Amount(NNNNNNNNNN.NN) */	\
//		"11,28,15,12,04,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 28:RMB CWC Times(NNNN) */	\
//		"*,29,29,12,10,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 29:RMB CWC Amount(NNNNNNNNNN.NN) */	\
//		"12,30,15,13,04,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 30:Momeny Retact Times(NNNN) */	\
//		"*,31,29,13,10,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 31:Momeny Retact Amount(NNNNNN.NN) */\
//		"13,32,15,14,04,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 32:Card Retained(NNNN) */	\
//		
		"0,33,38,15,02,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"				/* 33:MENU CHOICE */	\
		"",

/////////////////////////////////////////////////////////////////////////////
		"OP_COUNTER_OTHER",											// 62
		964,
		// SCREEN DATA
		//		 0123456789012345678901234567890123456789
		/*0*/	" ��Ϣ..............  ������............ ",
		/*1*/	" ����1:9999(100)     ����2:9999(100)    ",
		/*2*/	" ����3:9999(100)     ����4:9999(100)    ",
		/*3*/	" ������:���� ������:���� ƾ����ӡ��:����",
		/*4*/	" �ϳ���:���� �̿���:999  ��ˮ��ӡ��:����",
		/*5*/	" �豸���:12345678   AP�汾��:V01-01-01 ",
		/*6*/	"                                        ",
		/*7*/	"01)�޸Ĺ���Ա����   05)��ʼ��ƾ����ӡ�� ",
		/*8*/	"                                        ",
		/*9*/	"02)�ر�ϵͳ         06)��ʼ��������     ",
		/*0*/	"                                        ",
		/*1*/	"03)��������         07)ƾ���Ż�  ����   ",
		/*2*/	"                                        ",
		/*3*/	"04)��ʼ����ˮ��ӡ�� 10)����             ",
		/*4*/	"                                        ",
		/*5*/	"                      ������������:NN ",
		// SCREEN DISPLAY(NO,XX,YY,ATTRIBUTE,LENGTH,JUSTIFIER,FORMAT)
		"01,00,00,NORMAL,00,LEFT,:"								/* 01:MACHINE KIND(NANOCASH, MCD3, TTW, MCD4, TTW2, EATMGBA1, EATMULL1, EATMTTW, TTW2E, MCD4E, MX8000T, MX8000L, MX5600T, MX5600L) */	\
		"02,00,00,NORMAL,00,LEFT,:"								/* 02:ATM STATUS(INIT, ERROR, CLERK, READY, CUSTOM, TRAN, DOWN) */	\
		"03,00,00,NORMAL,00,LEFT,:"								/* 03:DOOR STATUS(DOOROPEN, DOORCLOSE) */	\
		"04,00,00,NORMAL,00,LEFT,:"								/* 04:LINE STATUS(HOSTONLINE, HOSTOFFLINE) */	\
		"05,01,00,NORMAL,18,LEFT,:"								/* 05:MESSAGE */	\
		"06,21,00,NORMAL,18,LEFT,:"								/* 06:ERROR CODE([P] - [EEEEE(SS)]) */	\
		
		"07,07,01,NORMAL,09,LEFT,:"								/* 07:CST1(CCCC(GGG)) */	\
		"08,27,01,NORMAL,09,LEFT,:"								/* 08:CST2(CCCC(GGG)) */	\
		"09,07,02,NORMAL,09,LEFT,:"								/* 09:CST3(CCCC(GGG)) */	\
		"10,27,02,NORMAL,09,LEFT,:"								/* 10:CST4(CCCC(GGG)) */	\
		"11,08,03,NORMAL,04,LEFT,:"								/* 11:CDU(NORMAL, AUTO-OFF, MISSING, ABNORMAL, ERROR, OFF) */	\
		"12,20,03,NORMAL,04,LEFT,:"								/* 12:MCU(NORMAL, AUTO-OFF, MISSING, ABNORMAL, ERROR, OFF) */	\
		"13,36,03,NORMAL,04,LEFT,:"								/* 13:SPR(NORMAL, AUTO-OFF, MISSING, ABNORMAL, ERROR, OFF) */	\
		"14,36,04,NORMAL,04,LEFT,:"								/* 14:JPR(NORMAL, AUTO-OFF, MISSING, ABNORMAL, ERROR, OFF) */	\
		
		"15,00,00,NORMAL,00,LEFT,:"								/* 15:CST1(MISSING, NORMAL, LOW, EMPTY, FULL) */	\
		"16,00,00,NORMAL,00,LEFT,:"								/* 16:CST2(MISSING, NORMAL, LOW, EMPTY, FULL) */	\
		"17,00,00,NORMAL,00,LEFT,:"								/* 17:CST3(MISSING, NORMAL, LOW, EMPTY, FULL) */	\
		"18,00,00,NORMAL,00,LEFT,:"								/* 18:CST4(MISSING, NORMAL, LOW, EMPTY, FULL) */	\
		"19,08,04,NORMAL,04,LEFT,:"								/* 19:REJECT(MISSING, NORMAL, LOW, EMPTY, FULL) */	\
		"20,20,04,NORMAL,03,LEFT,:"								/* 20:MCU(C) */	\
		"21,00,00,NORMAL,00,LEFT,:"								/* 21:SPR(MISSING, NORMAL, LOW, EMPTY) */	\
		"22,00,00,NORMAL,00,LEFT,:"								/* 22:JPR(MISSING, NORMAL, LOW, EMPTY) */	\
		
		"23,10,05,NORMAL,10,LEFT,:"								/* 23:ATM SERIAL NUMBER(NNNNNNNN) */	\
		"24,00,00,NORMAL,00,LEFT,:"								/* 24:BRANCH NUMBER(NNNNNNNN) */	\
		"25,30,05,NORMAL,10,LEFT,:"								/* 25:AP VERSION(V00-00-00) */	\
		"26,00,00,NORMAL,00,LEFT,####.##.##:"					/* 26:TRANSACTION DATE(YYYYMMDD) */ \
		
		"27,33,11,NORMAL,04,LEFT,:"								/* 27:OPTIMIZATION SETTING */	\
		"28,37,15,NORMAL,02,LEFT,##:"							/* 28:MENU CHOICE */	\
		
		"48,00,15,NORMAL,20,LEFT,:"								/* 48:MESSAGE */	\
		"",
		// SCREEN ACCEPT(INDEX,NO,XX,YY,LENGTH,ECHOMODE,ATTRIBUTE,INPUTTYPE,CURSORON,FLUSH,AUTOEND,TIMEOUT)
		"0,28,37,15,02,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 28:MENU CHOICE */	\
		"",
		
/////////////////////////////////////////////////////////////////////////////
		

/////////////////////////////////////////////////////////////////////////////
		"OP_TECHER_MENU",											// 70
		970,
		// SCREEN DATA
//		 0123456789012345678901234567890123456789
/*0*/	" ��Ϣ..............  ������............ ",
/*1*/	" ����1:9999(100)     ����2:9999(100)    ",
/*2*/	" ����3:9999(100)     ����4:9999(100)    ",
/*3*/	" ������:���� ������:���� ƾ����ӡ��:����",
/*4*/	" �ϳ���:���� �̿���:999  ��ˮ��ӡ��:����",
/*5*/	" �豸���:12345678   AP�汾��:V01-01-01 ",
/*6*/	"                                        ",
/*7*/	"01)�鿴״̬         05)��������         ",
/*8*/	"                                        ",
/*9*/	"02)��������         06)�����Ĳ�         ",
/*0*/	"                                        ",
/*1*/	"03)��Կ����         07)���ȡ������     ",
/*2*/	"                                        ",
/*3*/	"04)����������     10)�˳�����˵�     ",
/*4*/	"                                        ",
/*5*/	"                      ������������:NN ",
		// SCREEN DISPLAY(NO,XX,YY,ATTRIBUTE,LENGTH,JUSTIFIER,FORMAT)
		"01,00,00,NORMAL,00,LEFT,:"								/* 01:MACHINE KIND(NANOCASH, MCD3, TTW, MCD4, TTW2, EATMGBA1, EATMULL1, EATMTTW, TTW2E, MCD4E, MX8000T, MX8000L, MX5600T, MX5600L) */	\
		"02,00,00,NORMAL,00,LEFT,:"								/* 02:ATM STATUS(INIT, ERROR, CLERK, READY, CUSTOM, TRAN, DOWN) */	\
		"03,00,00,NORMAL,00,LEFT,:"								/* 03:DOOR STATUS(DOOROPEN, DOORCLOSE) */	\
		"04,00,00,NORMAL,00,LEFT,:"								/* 04:LINE STATUS(HOSTONLINE, HOSTOFFLINE) */	\
		"05,01,00,NORMAL,18,LEFT,:"								/* 05:MESSAGE */	\
		"06,21,00,NORMAL,18,LEFT,:"								/* 06:ERROR CODE([P] - [EEEEE(SS)]) */	\

		"07,07,01,NORMAL,09,LEFT,:"								/* 07:CST1(CCCC(GGG)) */	\
		"08,27,01,NORMAL,09,LEFT,:"								/* 08:CST2(CCCC(GGG)) */	\
		"09,07,02,NORMAL,09,LEFT,:"								/* 09:CST3(CCCC(GGG)) */	\
		"10,27,02,NORMAL,09,LEFT,:"								/* 10:CST4(CCCC(GGG)) */	\
		"11,08,03,NORMAL,04,LEFT,:"								/* 11:CDU(NORMAL, AUTO-OFF, MISSING, ABNORMAL, ERROR, OFF) */	\
		"12,20,03,NORMAL,04,LEFT,:"								/* 12:MCU(NORMAL, AUTO-OFF, MISSING, ABNORMAL, ERROR, OFF) */	\
		"13,36,03,NORMAL,04,LEFT,:"								/* 13:SPR(NORMAL, AUTO-OFF, MISSING, ABNORMAL, ERROR, OFF) */	\
		"14,36,04,NORMAL,04,LEFT,:"								/* 14:JPR(NORMAL, AUTO-OFF, MISSING, ABNORMAL, ERROR, OFF) */	\

		"15,00,00,NORMAL,00,LEFT,:"								/* 15:CST1(MISSING, NORMAL, LOW, EMPTY, FULL) */	\
		"16,00,00,NORMAL,00,LEFT,:"								/* 16:CST2(MISSING, NORMAL, LOW, EMPTY, FULL) */	\
		"17,00,00,NORMAL,00,LEFT,:"								/* 17:CST3(MISSING, NORMAL, LOW, EMPTY, FULL) */	\
		"18,00,00,NORMAL,00,LEFT,:"								/* 18:CST4(MISSING, NORMAL, LOW, EMPTY, FULL) */	\
		"19,08,04,NORMAL,04,LEFT,:"								/* 19:REJECT(MISSING, NORMAL, LOW, EMPTY, FULL) */	\
		"20,20,04,NORMAL,03,LEFT,:"								/* 20:MCU(C) */	\
		"21,00,00,NORMAL,00,LEFT,:"								/* 21:SPR(MISSING, NORMAL, LOW, EMPTY) */	\
		"22,00,00,NORMAL,00,LEFT,:"								/* 22:JPR(MISSING, NORMAL, LOW, EMPTY) */	\

		"23,10,05,NORMAL,10,LEFT,:"								/* 23:ATM SERIAL NUMBER(NNNNNNNN) */	\
		"24,00,00,NORMAL,00,LEFT,:"								/* 24:BRANCH NUMBER(NNNNNNNN) */	\
		"25,30,05,NORMAL,10,LEFT,:"								/* 25:AP VERSION(V00-00-00) */	\
		"26,00,00,NORMAL,00,LEFT,####.##.##:"					/* 26:TRANSACTION DATE(YYYYMMDD) */ \

		"27,37,15,NORMAL,02,LEFT,##:"							/* 27:MENU CHOICE */	\

		"48,00,15,NORMAL,20,LEFT,:"								/* 48:MESSAGE */	\
		"",
		// SCREEN ACCEPT(INDEX,NO,XX,YY,LENGTH,ECHOMODE,ATTRIBUTE,INPUTTYPE,CURSORON,FLUSH,AUTOEND,TIMEOUT)
		"0,27,37,15,02,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 27:MENU CHOICE */	\
		"",

/////////////////////////////////////////////////////////////////////////////
		"OP_CHANGEMETA_MENU",											// 76
		975,
		// SCREEN DATA
//		 0123456789012345678901234567890123456789
/*0*/	" ��Ϣ..............  ������............ ",
/*1*/	" ����1:9999(100)     ����2:9999(100)    ",
/*2*/	" ����3:9999(100)     ����4:9999(100)    ",
/*3*/	" ������:���� ������:���� ƾ����ӡ��:����",
/*4*/	" �ϳ���:���� �̿���:999  ��ˮ��ӡ��:����",
/*5*/	" �豸���:12345678   AP�汾��:V01-01-01 ",
/*6*/	"                                        ",
/*7*/	"01)����ƾ��                             ",
/*8*/	"                                        ",
/*9*/	"02)������ˮ                             ",
/*0*/	"                                        ",
/*1*/	"03)����ɫ��                             ",
/*2*/	"                                        ",
/*3*/	"                    10)����             ",
/*4*/	"                                        ",
/*5*/	"                      ������������:NN ",
		// SCREEN DISPLAY(NO,XX,YY,ATTRIBUTE,LENGTH,JUSTIFIER,FORMAT)
		"01,00,00,NORMAL,00,LEFT,:"								/* 01:MACHINE KIND(NANOCASH, MCD3, TTW, MCD4, TTW2, EATMGBA1, EATMULL1, EATMTTW, TTW2E, MCD4E, MX8000T, MX8000L, MX5600T, MX5600L) */	\
		"02,00,00,NORMAL,00,LEFT,:"								/* 02:ATM STATUS(INIT, ERROR, CLERK, READY, CUSTOM, TRAN, DOWN) */	\
		"03,00,00,NORMAL,00,LEFT,:"								/* 03:DOOR STATUS(DOOROPEN, DOORCLOSE) */	\
		"04,00,00,NORMAL,00,LEFT,:"								/* 04:LINE STATUS(HOSTONLINE, HOSTOFFLINE) */	\
		"05,01,00,NORMAL,18,LEFT,:"								/* 05:MESSAGE */	\
		"06,21,00,NORMAL,18,LEFT,:"								/* 06:ERROR CODE([P] - [EEEEE(SS)]) */	\

		"07,07,01,NORMAL,09,LEFT,:"								/* 07:CST1(CCCC(GGG)) */	\
		"08,27,01,NORMAL,09,LEFT,:"								/* 08:CST2(CCCC(GGG)) */	\
		"09,07,02,NORMAL,09,LEFT,:"								/* 09:CST3(CCCC(GGG)) */	\
		"10,27,02,NORMAL,09,LEFT,:"								/* 10:CST4(CCCC(GGG)) */	\
		"11,08,03,NORMAL,04,LEFT,:"								/* 11:CDU(NORMAL, AUTO-OFF, MISSING, ABNORMAL, ERROR, OFF) */	\
		"12,20,03,NORMAL,04,LEFT,:"								/* 12:MCU(NORMAL, AUTO-OFF, MISSING, ABNORMAL, ERROR, OFF) */	\
		"13,36,03,NORMAL,04,LEFT,:"								/* 13:SPR(NORMAL, AUTO-OFF, MISSING, ABNORMAL, ERROR, OFF) */	\
		"14,36,04,NORMAL,04,LEFT,:"								/* 14:JPR(NORMAL, AUTO-OFF, MISSING, ABNORMAL, ERROR, OFF) */	\

		"15,00,00,NORMAL,00,LEFT,:"								/* 15:CST1(MISSING, NORMAL, LOW, EMPTY, FULL) */	\
		"16,00,00,NORMAL,00,LEFT,:"								/* 16:CST2(MISSING, NORMAL, LOW, EMPTY, FULL) */	\
		"17,00,00,NORMAL,00,LEFT,:"								/* 17:CST3(MISSING, NORMAL, LOW, EMPTY, FULL) */	\
		"18,00,00,NORMAL,00,LEFT,:"								/* 18:CST4(MISSING, NORMAL, LOW, EMPTY, FULL) */	\
		"19,08,04,NORMAL,04,LEFT,:"								/* 19:REJECT(MISSING, NORMAL, LOW, EMPTY, FULL) */	\
		"20,20,04,NORMAL,03,LEFT,:"								/* 20:MCU(C) */	\
		"21,00,00,NORMAL,00,LEFT,:"								/* 21:SPR(MISSING, NORMAL, LOW, EMPTY) */	\
		"22,00,00,NORMAL,00,LEFT,:"								/* 22:JPR(MISSING, NORMAL, LOW, EMPTY) */	\

		"23,10,05,NORMAL,10,LEFT,:"								/* 23:ATM SERIAL NUMBER(NNNNNNNN) */	\
		"24,00,00,NORMAL,00,LEFT,:"								/* 24:BRANCH NUMBER(NNNNNNNN) */	\
		"25,30,05,NORMAL,10,LEFT,:"								/* 25:AP VERSION(V00-00-00) */	\
		"26,00,00,NORMAL,00,LEFT,####.##.##:"					/* 26:TRANSACTION DATE(YYYYMMDD) */ \

		"27,37,15,NORMAL,02,LEFT,##:"							/* 27:MENU CHOICE */	\

		"48,00,15,NORMAL,20,LEFT,:"								/* 48:MESSAGE */	\
		"",
		// SCREEN ACCEPT(INDEX,NO,XX,YY,LENGTH,ECHOMODE,ATTRIBUTE,INPUTTYPE,CURSORON,FLUSH,AUTOEND,TIMEOUT)
		"0,27,37,15,02,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 27:MENU CHOICE */	\
		"",

/////////////////////////////////////////////////////////////////////////////
		"OP_TECHER_KEY",											// 71
		971,
		// SCREEN DATA
//		 0123456789012345678901234567890123456789
/*0*/	" ��Ϣ..............  ������............ ",
/*1*/	" ����1:9999(100)     ����2:9999(100)    ",
/*2*/	" ����3:9999(100)     ����4:9999(100)    ",
/*3*/	" ������:���� ������:���� ƾ����ӡ��:����",
/*4*/	" �ϳ���:���� �̿���:999  ��ˮ��ӡ��:����",
/*5*/	" �豸���:12345678   AP�汾��:V01-01-01 ",
/*6*/	"                                        ",
/*7*/	"01)��ʼ��CDK        05)��ʼ��EPP����    ",
/*8*/	"                                        ",
/*9*/	"02)����CDK                              ",
/*0*/	"                                        ",
/*1*/	"03)����MACKEY                           ",
/*2*/	"                                        ",
/*3*/	"04)����PINKEY       10)����             ",
/*4*/	"                                        ",
/*5*/	"                      ������������:NN ",
		// SCREEN DISPLAY(NO,XX,YY,ATTRIBUTE,LENGTH,JUSTIFIER,FORMAT)
		"01,00,00,NORMAL,00,LEFT,:"								/* 01:MACHINE KIND(NANOCASH, MCD3, TTW, MCD4, TTW2, EATMGBA1, EATMULL1, EATMTTW, TTW2E, MCD4E, MX8000T, MX8000L, MX5600T, MX5600L) */	\
		"02,00,00,NORMAL,00,LEFT,:"								/* 02:ATM STATUS(INIT, ERROR, CLERK, READY, CUSTOM, TRAN, DOWN) */	\
		"03,00,00,NORMAL,00,LEFT,:"								/* 03:DOOR STATUS(DOOROPEN, DOORCLOSE) */	\
		"04,00,00,NORMAL,00,LEFT,:"								/* 04:LINE STATUS(HOSTONLINE, HOSTOFFLINE) */	\
		"05,01,00,NORMAL,18,LEFT,:"								/* 05:MESSAGE */	\
		"06,21,00,NORMAL,18,LEFT,:"								/* 06:ERROR CODE([P] - [EEEEE(SS)]) */	\

		"07,07,01,NORMAL,09,LEFT,:"								/* 07:CST1(CCCC(GGG)) */	\
		"08,27,01,NORMAL,09,LEFT,:"								/* 08:CST2(CCCC(GGG)) */	\
		"09,07,02,NORMAL,09,LEFT,:"								/* 09:CST3(CCCC(GGG)) */	\
		"10,27,02,NORMAL,09,LEFT,:"								/* 10:CST4(CCCC(GGG)) */	\
		"11,08,03,NORMAL,04,LEFT,:"								/* 11:CDU(NORMAL, AUTO-OFF, MISSING, ABNORMAL, ERROR, OFF) */	\
		"12,20,03,NORMAL,04,LEFT,:"								/* 12:MCU(NORMAL, AUTO-OFF, MISSING, ABNORMAL, ERROR, OFF) */	\
		"13,36,03,NORMAL,04,LEFT,:"								/* 13:SPR(NORMAL, AUTO-OFF, MISSING, ABNORMAL, ERROR, OFF) */	\
		"14,36,04,NORMAL,04,LEFT,:"								/* 14:JPR(NORMAL, AUTO-OFF, MISSING, ABNORMAL, ERROR, OFF) */	\

		"15,00,00,NORMAL,00,LEFT,:"								/* 15:CST1(MISSING, NORMAL, LOW, EMPTY, FULL) */	\
		"16,00,00,NORMAL,00,LEFT,:"								/* 16:CST2(MISSING, NORMAL, LOW, EMPTY, FULL) */	\
		"17,00,00,NORMAL,00,LEFT,:"								/* 17:CST3(MISSING, NORMAL, LOW, EMPTY, FULL) */	\
		"18,00,00,NORMAL,00,LEFT,:"								/* 18:CST4(MISSING, NORMAL, LOW, EMPTY, FULL) */	\
		"19,08,04,NORMAL,04,LEFT,:"								/* 19:REJECT(MISSING, NORMAL, LOW, EMPTY, FULL) */	\
		"20,20,04,NORMAL,03,LEFT,:"								/* 20:MCU(C) */	\
		"21,00,00,NORMAL,00,LEFT,:"								/* 21:SPR(MISSING, NORMAL, LOW, EMPTY) */	\
		"22,00,00,NORMAL,00,LEFT,:"								/* 22:JPR(MISSING, NORMAL, LOW, EMPTY) */	\

		"23,10,05,NORMAL,10,LEFT,:"								/* 23:ATM SERIAL NUMBER(NNNNNNNN) */	\
		"24,00,00,NORMAL,00,LEFT,:"								/* 24:BRANCH NUMBER(NNNNNNNN) */	\
		"25,30,05,NORMAL,10,LEFT,:"								/* 25:AP VERSION(V00-00-00) */	\
		"26,00,00,NORMAL,00,LEFT,####.##.##:"					/* 26:TRANSACTION DATE(YYYYMMDD) */ \

		"27,37,15,NORMAL,02,LEFT,##:"							/* 27:MENU CHOICE */	\

		"48,00,15,NORMAL,20,LEFT,:"								/* 48:MESSAGE */	\
		"",
		// SCREEN ACCEPT(INDEX,NO,XX,YY,LENGTH,ECHOMODE,ATTRIBUTE,INPUTTYPE,CURSORON,FLUSH,AUTOEND,TIMEOUT)
		"0,27,37,15,02,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 27:MENU CHOICE */	\
		"",

/////////////////////////////////////////////////////////////////////////////
		"OP_TECHER_DEVICE",										// 72
		972,
		// SCREEN DATA
//		 0123456789012345678901234567890123456789
/*0*/	" ��Ϣ..............  ������............ ",
/*1*/	" ����1:9999(100)     ����2:9999(100)    ",
/*2*/	" ����3:9999(100)     ����4:9999(100)    ",
/*3*/	" ������:���� ������:���� ƾ����ӡ��:����",
/*4*/	" �ϳ���:���� �̿���:999  ��ˮ��ӡ��:����",
/*5*/	" �豸���:12345678   AP�汾��:V01-01-01 ",
/*6*/	"                                        ",
/*7*/	"01)��ʼ����ˮ��ӡ�� 06)�޸Ĺ���Ա����   ",
/*8*/	"02)��ʼ��ƾ����ӡ�� 07)ϵͳ��������     ",
/*9*/	"03)��ʼ������ģ��   08)�ر�ϵͳ         ",
/*0*/	"04)��ʼ��������                         ",
/*1*/	"05)Ӳ����⼰����                       ",
/*2*/	"                                        ",
/*3*/	"                    10)����             ",
/*4*/	"                                        ",
/*5*/	"                      ������������:NN ",
		// SCREEN DISPLAY(NO,XX,YY,ATTRIBUTE,LENGTH,JUSTIFIER,FORMAT)
		"01,00,00,NORMAL,00,LEFT,:"								/* 01:MACHINE KIND(NANOCASH, MCD3, TTW, MCD4, TTW2, EATMGBA1, EATMULL1, EATMTTW, TTW2E, MCD4E, MX8000T, MX8000L, MX5600T, MX5600L) */	\
		"02,00,00,NORMAL,00,LEFT,:"								/* 02:ATM STATUS(INIT, ERROR, CLERK, READY, CUSTOM, TRAN, DOWN) */	\
		"03,00,00,NORMAL,00,LEFT,:"								/* 03:DOOR STATUS(DOOROPEN, DOORCLOSE) */	\
		"04,00,00,NORMAL,00,LEFT,:"								/* 04:LINE STATUS(HOSTONLINE, HOSTOFFLINE) */	\
		"05,01,00,NORMAL,18,LEFT,:"								/* 05:MESSAGE */	\
		"06,21,00,NORMAL,18,LEFT,:"								/* 06:ERROR CODE([P] - [EEEEE(SS)]) */	\

		"07,07,01,NORMAL,09,LEFT,:"								/* 07:CST1(CCCC(GGG)) */	\
		"08,27,01,NORMAL,09,LEFT,:"								/* 08:CST2(CCCC(GGG)) */	\
		"09,07,02,NORMAL,09,LEFT,:"								/* 09:CST3(CCCC(GGG)) */	\
		"10,27,02,NORMAL,09,LEFT,:"								/* 10:CST4(CCCC(GGG)) */	\
		"11,08,03,NORMAL,04,LEFT,:"								/* 11:CDU(NORMAL, AUTO-OFF, MISSING, ABNORMAL, ERROR, OFF) */	\
		"12,20,03,NORMAL,04,LEFT,:"								/* 12:MCU(NORMAL, AUTO-OFF, MISSING, ABNORMAL, ERROR, OFF) */	\
		"13,36,03,NORMAL,04,LEFT,:"								/* 13:SPR(NORMAL, AUTO-OFF, MISSING, ABNORMAL, ERROR, OFF) */	\
		"14,36,04,NORMAL,04,LEFT,:"								/* 14:JPR(NORMAL, AUTO-OFF, MISSING, ABNORMAL, ERROR, OFF) */	\

		"15,00,00,NORMAL,00,LEFT,:"								/* 15:CST1(MISSING, NORMAL, LOW, EMPTY, FULL) */	\
		"16,00,00,NORMAL,00,LEFT,:"								/* 16:CST2(MISSING, NORMAL, LOW, EMPTY, FULL) */	\
		"17,00,00,NORMAL,00,LEFT,:"								/* 17:CST3(MISSING, NORMAL, LOW, EMPTY, FULL) */	\
		"18,00,00,NORMAL,00,LEFT,:"								/* 18:CST4(MISSING, NORMAL, LOW, EMPTY, FULL) */	\
		"19,08,04,NORMAL,04,LEFT,:"								/* 19:REJECT(MISSING, NORMAL, LOW, EMPTY, FULL) */	\
		"20,20,04,NORMAL,03,LEFT,:"								/* 20:MCU(C) */	\
		"21,00,00,NORMAL,00,LEFT,:"								/* 21:SPR(MISSING, NORMAL, LOW, EMPTY) */	\
		"22,00,00,NORMAL,00,LEFT,:"								/* 22:JPR(MISSING, NORMAL, LOW, EMPTY) */	\

		"23,10,05,NORMAL,10,LEFT,:"								/* 23:ATM SERIAL NUMBER(NNNNNNNN) */	\
		"24,00,00,NORMAL,00,LEFT,:"								/* 24:BRANCH NUMBER(NNNNNNNN) */	\
		"25,30,05,NORMAL,10,LEFT,:"								/* 25:AP VERSION(V00-00-00) */	\
		"26,00,00,NORMAL,00,LEFT,####.##.##:"					/* 26:TRANSACTION DATE(YYYYMMDD) */ \

		"27,37,15,NORMAL,02,LEFT,##:"							/* 27:MENU CHOICE */	\

		"48,00,15,NORMAL,20,LEFT,:"								/* 48:MESSAGE */	\
		"",
		// SCREEN ACCEPT(INDEX,NO,XX,YY,LENGTH,ECHOMODE,ATTRIBUTE,INPUTTYPE,CURSORON,FLUSH,AUTOEND,TIMEOUT)
		"0,27,37,15,02,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 27:MENU CHOICE */	\
		"",

/////////////////////////////////////////////////////////////////////////////
		"OP_TECHER_DOWNLOAD",										// 73
		973,
		// SCREEN DATA
//		 0123456789012345678901234567890123456789
/*0*/	" ��Ϣ..............  ������............ ",
/*1*/	" ����1:9999(100)     ����2:9999(100)    ",
/*2*/	" ����3:9999(100)     ����4:9999(100)    ",
/*3*/	" ������:���� ������:���� ƾ����ӡ��:����",
/*4*/	" �ϳ���:���� �̿���:999  ��ˮ��ӡ��:����",
/*5*/	" �豸���:12345678   AP�汾��:V01-01-01 ",
/*6*/	"                                        ",
/*7*/	"01)���Ʋ�������      05)IC��������      ",
/*8*/	"                                        ",
/*9*/	"02)���ؿ����ļ�      06)PBOC��������    ",
/*0*/	"                                        ",
/*1*/	"03)����ҵ���������  07)AID�б�����     ",
/*2*/	"                                        ",
/*3*/	"04)���Ʋ����޸�      08)����������������",
/*4*/	"                     10)����            ",
/*5*/	"                      ������������:NN ",
		// SCREEN DISPLAY(NO,XX,YY,ATTRIBUTE,LENGTH,JUSTIFIER,FORMAT)
		"01,00,00,NORMAL,00,LEFT,:"								/* 01:MACHINE KIND(NANOCASH, MCD3, TTW, MCD4, TTW2, EATMGBA1, EATMULL1, EATMTTW, TTW2E, MCD4E, MX8000T, MX8000L, MX5600T, MX5600L) */	\
		"02,00,00,NORMAL,00,LEFT,:"								/* 02:ATM STATUS(INIT, ERROR, CLERK, READY, CUSTOM, TRAN, DOWN) */	\
		"03,00,00,NORMAL,00,LEFT,:"								/* 03:DOOR STATUS(DOOROPEN, DOORCLOSE) */	\
		"04,00,00,NORMAL,00,LEFT,:"								/* 04:LINE STATUS(HOSTONLINE, HOSTOFFLINE) */	\
		"05,01,00,NORMAL,18,LEFT,:"								/* 05:MESSAGE */	\
		"06,21,00,NORMAL,18,LEFT,:"								/* 06:ERROR CODE([P] - [EEEEE(SS)]) */	\

		"07,07,01,NORMAL,09,LEFT,:"								/* 07:CST1(CCCC(GGG)) */	\
		"08,27,01,NORMAL,09,LEFT,:"								/* 08:CST2(CCCC(GGG)) */	\
		"09,07,02,NORMAL,09,LEFT,:"								/* 09:CST3(CCCC(GGG)) */	\
		"10,27,02,NORMAL,09,LEFT,:"								/* 10:CST4(CCCC(GGG)) */	\
		"11,08,03,NORMAL,04,LEFT,:"								/* 11:CDU(NORMAL, AUTO-OFF, MISSING, ABNORMAL, ERROR, OFF) */	\
		"12,20,03,NORMAL,04,LEFT,:"								/* 12:MCU(NORMAL, AUTO-OFF, MISSING, ABNORMAL, ERROR, OFF) */	\
		"13,36,03,NORMAL,04,LEFT,:"								/* 13:SPR(NORMAL, AUTO-OFF, MISSING, ABNORMAL, ERROR, OFF) */	\
		"14,36,04,NORMAL,04,LEFT,:"								/* 14:JPR(NORMAL, AUTO-OFF, MISSING, ABNORMAL, ERROR, OFF) */	\

		"15,00,00,NORMAL,00,LEFT,:"								/* 15:CST1(MISSING, NORMAL, LOW, EMPTY, FULL) */	\
		"16,00,00,NORMAL,00,LEFT,:"								/* 16:CST2(MISSING, NORMAL, LOW, EMPTY, FULL) */	\
		"17,00,00,NORMAL,00,LEFT,:"								/* 17:CST3(MISSING, NORMAL, LOW, EMPTY, FULL) */	\
		"18,00,00,NORMAL,00,LEFT,:"								/* 18:CST4(MISSING, NORMAL, LOW, EMPTY, FULL) */	\
		"19,08,04,NORMAL,04,LEFT,:"								/* 19:REJECT(MISSING, NORMAL, LOW, EMPTY, FULL) */	\
		"20,20,04,NORMAL,03,LEFT,:"								/* 20:MCU(C) */	\
		"21,00,00,NORMAL,00,LEFT,:"								/* 21:SPR(MISSING, NORMAL, LOW, EMPTY) */	\
		"22,00,00,NORMAL,00,LEFT,:"								/* 22:JPR(MISSING, NORMAL, LOW, EMPTY) */	\

		"23,10,05,NORMAL,10,LEFT,:"								/* 23:ATM SERIAL NUMBER(NNNNNNNN) */	\
		"24,00,00,NORMAL,00,LEFT,:"								/* 24:BRANCH NUMBER(NNNNNNNN) */	\
		"25,30,05,NORMAL,10,LEFT,:"								/* 25:AP VERSION(V00-00-00) */	\
		"26,00,00,NORMAL,00,LEFT,####.##.##:"					/* 26:TRANSACTION DATE(YYYYMMDD) */ \

		"27,37,15,NORMAL,02,LEFT,##:"							/* 27:MENU CHOICE */	\

		"48,00,15,NORMAL,20,LEFT,:"								/* 48:MESSAGE */	\
		"",
		// SCREEN ACCEPT(INDEX,NO,XX,YY,LENGTH,ECHOMODE,ATTRIBUTE,INPUTTYPE,CURSORON,FLUSH,AUTOEND,TIMEOUT)
		"0,27,37,15,02,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 27:MENU CHOICE */	\
		"",

/////////////////////////////////////////////////////////////////////////////
		"ADDCASHINPUTCOUNTER",										// 43
		974,
		// SCREEN DATA
//		 0123456789012345678901234567890123456789
/*0*/	"                                        ",
/*1*/	"             ������ӳ���ʽ             ",
/*2*/	"                                        ",
/*3*/	"1>                 [X]                  ",
/*4*/	"                                        ",
/*5*/	"                                        ",
/*6*/	"                0:���ؼӳ�              ",
/*7*/	"                1:����ӳ�              ",
/*8*/	"                                        ",
/*9*/	"                                        ",
/*0*/	"                                        ",
/*1*/	"                                        ",
/*2*/	"                                        ",
/*3*/	"1~9:�����к�,ENTER:��һ��,CANCEL:ȡ��   ",
/*4*/	"11:ȷ��,88:�˳�                         ",
/*5*/	"                      ������������:NN ",
		// SCREEN DISPLAY(NO,XX,YY,ATTRIBUTE,LENGTH,JUSTIFIER,FORMAT)
		"00,00,13,INVERTED,03,LEFT,1~9:"						\
		"00,13,13,INVERTED,05,LEFT,ENTER:"						\
		"00,26,13,INVERTED,06,LEFT,CANCEL:"						\
		"00,00,14,INVERTED,02,LEFT,11:"							\
		"00,08,14,INVERTED,02,LEFT,88:"							\
		
		"01,20,03,NORMAL,01,LEFT,:"								/* 01:ADDCASH MODE */	\
		
		"02,37,15,NORMAL,02,LEFT,##:"							/* 02:MENU CHOICE */	\
		
		"48,00,15,NORMAL,20,LEFT,:"								/* 48:MESSAGE */	\
		"",
		// SCREEN ACCEPT(INDEX,NO,XX,YY,LENGTH,ECHOMODE,ATTRIBUTE,INPUTTYPE,CURSORON,FLUSH,AUTOEND,TIMEOUT)
		"1,01,20,03,1,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 01:ADDCASH MODE */	\
		
		"0,02,37,15,02,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 02:MENU CHOICE */	\
		"",
		
		
/////////////////////////////////////////////////////////////////////////////
"OP_RESET_KEY_CONFIRM",											// 71
976,
// SCREEN DATA
//		 0123456789012345678901234567890123456789
/*0*/	" ��Ϣ..............  ������............ ",
/*1*/	" ����1:9999(100)     ����2:9999(100)    ",
/*2*/	" ����3:9999(100)     ����4:9999(100)    ",
/*3*/	" ������:���� ������:���� ƾ����ӡ��:����",
/*4*/	" �ϳ���:���� �̿���:999  ��ˮ��ӡ��:����",
/*5*/	" �豸���:12345678   AP�汾��:V01-01-01 ",
/*6*/	"                                        ",
/*7*/	" ��ʼ��EPP���̻ᵼ����Կ��ʧ����Ҫ����  ",
/*8*/	"                                        ",
/*9*/	" ������Կ���Ƿ�ȷ�ϴ˲�����             ",
/*0*/	"                                        ",
/*1*/	"                                        ",
/*2*/	"                                        ",
/*3*/	"11)ȷ��             10)����             ",
/*4*/	"                                        ",
/*5*/	"                      ������������:NN ",
		// SCREEN DISPLAY(NO,XX,YY,ATTRIBUTE,LENGTH,JUSTIFIER,FORMAT)
		"01,00,00,NORMAL,00,LEFT,:"								/* 01:MACHINE KIND(NANOCASH, MCD3, TTW, MCD4, TTW2, EATMGBA1, EATMULL1, EATMTTW, TTW2E, MCD4E, MX8000T, MX8000L, MX5600T, MX5600L) */	\
		"02,00,00,NORMAL,00,LEFT,:"								/* 02:ATM STATUS(INIT, ERROR, CLERK, READY, CUSTOM, TRAN, DOWN) */	\
		"03,00,00,NORMAL,00,LEFT,:"								/* 03:DOOR STATUS(DOOROPEN, DOORCLOSE) */	\
		"04,00,00,NORMAL,00,LEFT,:"								/* 04:LINE STATUS(HOSTONLINE, HOSTOFFLINE) */	\
		"05,01,00,NORMAL,18,LEFT,:"								/* 05:MESSAGE */	\
		"06,21,00,NORMAL,18,LEFT,:"								/* 06:ERROR CODE([P] - [EEEEE(SS)]) */	\
		
		"07,07,01,NORMAL,09,LEFT,:"								/* 07:CST1(CCCC(GGG)) */	\
		"08,27,01,NORMAL,09,LEFT,:"								/* 08:CST2(CCCC(GGG)) */	\
		"09,07,02,NORMAL,09,LEFT,:"								/* 09:CST3(CCCC(GGG)) */	\
		"10,27,02,NORMAL,09,LEFT,:"								/* 10:CST4(CCCC(GGG)) */	\
		"11,08,03,NORMAL,04,LEFT,:"								/* 11:CDU(NORMAL, AUTO-OFF, MISSING, ABNORMAL, ERROR, OFF) */	\
		"12,20,03,NORMAL,04,LEFT,:"								/* 12:MCU(NORMAL, AUTO-OFF, MISSING, ABNORMAL, ERROR, OFF) */	\
		"13,36,03,NORMAL,04,LEFT,:"								/* 13:SPR(NORMAL, AUTO-OFF, MISSING, ABNORMAL, ERROR, OFF) */	\
		"14,36,04,NORMAL,04,LEFT,:"								/* 14:JPR(NORMAL, AUTO-OFF, MISSING, ABNORMAL, ERROR, OFF) */	\
		
		"15,00,00,NORMAL,00,LEFT,:"								/* 15:CST1(MISSING, NORMAL, LOW, EMPTY, FULL) */	\
		"16,00,00,NORMAL,00,LEFT,:"								/* 16:CST2(MISSING, NORMAL, LOW, EMPTY, FULL) */	\
		"17,00,00,NORMAL,00,LEFT,:"								/* 17:CST3(MISSING, NORMAL, LOW, EMPTY, FULL) */	\
		"18,00,00,NORMAL,00,LEFT,:"								/* 18:CST4(MISSING, NORMAL, LOW, EMPTY, FULL) */	\
		"19,08,04,NORMAL,04,LEFT,:"								/* 19:REJECT(MISSING, NORMAL, LOW, EMPTY, FULL) */	\
		"20,20,04,NORMAL,03,LEFT,:"								/* 20:MCU(C) */	\
		"21,00,00,NORMAL,00,LEFT,:"								/* 21:SPR(MISSING, NORMAL, LOW, EMPTY) */	\
		"22,00,00,NORMAL,00,LEFT,:"								/* 22:JPR(MISSING, NORMAL, LOW, EMPTY) */	\
		
		"23,10,05,NORMAL,10,LEFT,:"								/* 23:ATM SERIAL NUMBER(NNNNNNNN) */	\
		"24,00,00,NORMAL,00,LEFT,:"								/* 24:BRANCH NUMBER(NNNNNNNN) */	\
		"25,30,05,NORMAL,10,LEFT,:"								/* 25:AP VERSION(V00-00-00) */	\
		"26,00,00,NORMAL,00,LEFT,####.##.##:"					/* 26:TRANSACTION DATE(YYYYMMDD) */ \
		
		"27,37,15,NORMAL,02,LEFT,##:"							/* 27:MENU CHOICE */	\
		
		"48,00,15,NORMAL,20,LEFT,:"								/* 48:MESSAGE */	\
		"",
		// SCREEN ACCEPT(INDEX,NO,XX,YY,LENGTH,ECHOMODE,ATTRIBUTE,INPUTTYPE,CURSORON,FLUSH,AUTOEND,TIMEOUT)
		"0,27,37,15,02,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 27:MENU CHOICE */	\
		"",


/////////////////////////////////////////////////////////////////////////////
		"OP_TOTALKIND",													// 77
		977,
		// SCREEN DATA
//		 0123456789012345678901234567890123456789
/*0*/	"                                        ",
/*1*/	"             ���������˷�ʽ             ",
/*2*/	"                                        ",
/*3*/	"1>                 [X]                  ",
/*4*/	"                                        ",
/*5*/	"                                        ",
/*6*/	"                0:Ԥ����                ",
/*7*/	"                1:�ն�����              ",
/*8*/	"                                        ",
/*9*/	"                                        ",
/*0*/	"                                        ",
/*1*/	"                                        ",
/*2*/	"                                        ",
/*3*/	"1~9:�����к�,ENTER:��һ��,CANCEL:ȡ��   ",
/*4*/	"11:ȷ��,88:�˳�                         ",
/*5*/	"                      ������������:NN ",
		// SCREEN DISPLAY(NO,XX,YY,ATTRIBUTE,LENGTH,JUSTIFIER,FORMAT)
		"00,00,13,INVERTED,03,LEFT,1~9:"						\
		"00,13,13,INVERTED,05,LEFT,ENTER:"						\
		"00,26,13,INVERTED,06,LEFT,CANCEL:"						\
		"00,00,14,INVERTED,02,LEFT,11:"							\
		"00,08,14,INVERTED,02,LEFT,88:"							\
		
		"01,20,03,NORMAL,01,LEFT,:"								/* 01:ADDCASH MODE */	\
		
		"02,37,15,NORMAL,02,LEFT,##:"							/* 02:MENU CHOICE */	\
		
		"48,00,15,NORMAL,20,LEFT,:"								/* 48:MESSAGE */	\
		"",
		// SCREEN ACCEPT(INDEX,NO,XX,YY,LENGTH,ECHOMODE,ATTRIBUTE,INPUTTYPE,CURSORON,FLUSH,AUTOEND,TIMEOUT)
		"1,01,20,03,1,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 01:ADDCASH MODE */	\
		
		"0,02,37,15,02,NORMAL,NORMAL,NUMERIC,1,1,1,-1:"			/* 02:MENU CHOICE */	\
		"",
		









/////////////////////////////////////////////////////////////////////////////
		"",														// NNN
		0,
		// SCREEN DISPLAY(NO,XX,YY,ATTRIBUTE,LENGTH,JUSTIFIER,FORMAT)
//		 0123456789012345678901234567890123456789
/*0*/	"                                        ",
/*1*/	"                                        ",
/*2*/	"                                        ",
/*3*/	"                                        ",
/*4*/	"                                        ",
/*5*/	"                                        ",
/*6*/	"                                        ",
/*7*/	"                                        ",
/*8*/	"                                        ",
/*9*/	"                                        ",
/*0*/	"                                        ",
/*1*/	"                                        ",
/*2*/	"                                        ",
/*3*/	"                                        ",
/*4*/	"                                        ",
/*5*/	"                                        ",
		// SCREEN DISPLAY(NO,XX,YY,ATTRIBUTE,LENGTH,JUSTIFIER,FORMAT)
		":"														/* NN: */	\
		"",
		// SCREEN ACCEPT(INDEX,NO,XX,YY,LENGTH,ECHOMODE,ATTRIBUTE,INPUTTYPE,CURSORON,FLUSH,AUTOEND,TIMEOUT)
		":"														/* NN: */	\
		""
};

/////////////////////////////////////////////////////////////////////////////
#endif
