/////////////////////////////////////////////////////////////////////////////
#if !defined(_TranCmnMenu_H_)
#define _TranCmnMenu_H_

/////////////////////////////////////////////////////////////////////////////
//	CARD(2TR, 3TR, IC)
//	PASSBOOK
//	NO CARD(NO CARD, NO PASSBOOK)
/////////////////////////////////////////////////////////////////////////////
#define	MENU_ALL			(0xffffffff)						// ALL
#define	MENU_NULL			(0x00000000)						// NULL
#define	MENU_M2				(0x00000001)						// mOTHER CREDIT CARD(2TR)
#define	MENU_T2				(0x00000002)						// OtHER CREDIT CARD(2TR)
#define	MENU_M3				(0x00000004)						// MOTHER BANK CARD(3TR)
#define	MENU_T3				(0x00000008)						// OTHER BANK CARD(3TR)
#define	MENU_IC				(0x00010000)						// IC CARD
#define	MENU_PB				(0x00020000)						// PASSBOOK
#define	MENU_NC				(0x10000000)						// NO CARD

///////////////////////////////////////////////////////////////////////////////////
#define	MENU_M2T2			(MENU_M2 | MENU_T2)					// CREDIT CARD(2TR)
#define	MENU_M3T3			(MENU_M3 | MENU_T3)					// BANK CARD(3TR)

#define MENU_MCU2			(MENU_M2T2)							// CARD(2TR)
#define MENU_MCU3			(MENU_M3T3)							// CARD(3TR)

#define MENU_MCU23			(MENU_M2T2 | MENU_M3T3)				// CARD(2TR/3TR)
#define MENU_MCU			(MENU_MCU2 | MENU_MCU3 | MENU_IC)	// CARD
#define MENU_MCUMM			(MENU_M2 | MENU_M3 | MENU_IC)		// MOTHER CARD
#define MENU_MCUTT			(MENU_MCU & (~MENU_MCUMM))			// OTHER CARD

#define MENU_MM				(MENU_MCUMM | MENU_PB | MENU_NC) 	// MOTHER BANK
#define MENU_TT				(MENU_MCUTT)						// OTHER BANK

																// OTHER TYPE(MENU ATTRIBUTE)
#define MENU_MCUM2M3		(MENU_M2 | MENU_M3)					// 2005.03.16
#define MENU_MCUT2T3		(MENU_T2 | MENU_T3)					// 2005.03.16
																
/////////////////////////////////////////////////////////////////////////////
//	CUSTOMER TRANSACTION
/////////////////////////////////////////////////////////////////////////////
////MENUINFO(0)			CARD INSERT(1)
////						PASSBOOK INSERT(2)
////						AUTO CHOICE(2)(1 BUTTON)
////							INPUT(3)/BUTTON CHOICE(3)
////								TRANPROC(4)
////											TRANCODE(5)
////														TRANCODE2(6)
/////////////////////////////////////////////////////////////////////////////
//	MENU_MCU23,			0,	0,	0,	TRAN_WITH,	TC_WITHDRAWAL,0,// 1:WITHDRAWAL
//	MENU_MCU23,			0,	0,	0,	TRAN_INQ,	TC_INQUIRY,	0,	// 2:INQUIRY
//	MENU_MCU23,			0,	0,	0,	TRAN_TRANS,	TC_TRANSFER,0,	// 3:TRANSFER
//	MENU_MCU23,			0,	0,	0,	TRAN_INQ,	TC_DETAIL,	0,	// 4:DETAIL
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
//	TRANSACTION CODE DEFINITION
/////////////////////////////////////////////////////////////////////////////
#define		TC_INQUIRY			1								// 1:INQUIRY
#define		TC_TRANSFER			2								// 2:TRANSFER
#define		TC_WITHDRAWAL		3								// 3:WITHDRAWAL
#define		TC_CHANGEPIN		4								// 4:CHANGEPIN
#define		TC_DETAIL			5								// 5:DETAIL
#define		TC_PAY				6								// 6:PAY
#define		TC_PAY_INQ			7								// 7:PAY INQUIRY
#define		TC_REMIT			8								// 8:REMIT INQUIRY 2008.10.05
#define		TC_REMIT_ADD		9								// 9:REMIT ADDRESS
#define		TC_REMIT_IN			10								// 10:REMIT IN
#define		TC_REMIT_PWD		11								// 11:REMIT PASSWORD
#define		TC_REMIT_INF_INQ	12								// 12:汇票信息查询
#define		TC_OTHER			13								// 13:其他业务 added by liuxl 20111022
#define     TC_INQUIRY_CheckPin 14								// 14:验密inq

#define		TC_ACMANAGER		15								// 15:A/C MANAGER
#define		TC_AC_REMIT   		16								// 16:A/C MANAGER ACREMIT
#define		TC_AC_PAY    		17								// 17:A/C MANAGER ACPAY
#define		TC_AC_PAY_ASSU    	18  							// 18:A/C 约定缴费
#define		TC_AC_PAY_NOASSU   	19  							// 19:A/C 非约定缴费
#define		TC_AC_REMIT_FIX   	20								// 20:A/C 定期一本通汇款
#define		TC_AC_REMIT_PWD   	21								// 21:A/C 密码汇款
// 20080903 by yaokq
#define		TC_OTHER_INQ		22								//查询其它储种

#define		TC_HZT				23								//liyi add 绿卡通 活期转其它储种
#define		TC_TZH				24								//liyi add 绿卡通 其它储种转活期
#define		TC_CUS_INF_INQ		25								//yusy add 客户信息查询 2010.03.22

#define		TC_ICINQUIRY		26
#define		TC_ICDETAIL			27
#define		TC_LOAD				28								// 指定账户圈存
#define		TC_BOUNDMDOIFY		29								// 小额账户额度修改
#define		TC_DEPOSIT			31								// DEPOSIT
#define		TC_CASHTRANS		32								// 现金转账
#define		TC_ECHANNEL			34								// 开通电子渠道
#define		TC_ECHANNEL_INQ		35								// 电子银行查询
#define		TC_SMSPARITY		36								// 短信验证码
#define		TC_CUS_INF_INQ_OTHERBANK		37								// 跨行户名查询

#define		TC_AC_CUSMESS_INQ  	    5505						// 5505:A/C 查询客户基本信息
#define		TC_AC_CUSASSU_DETAIL 	5507				    	// 5507:A/C 查询客户约定细项信息  查询转账汇款和约定缴费 // atmunet
#define		TC_AC_OPENTRAN_INQ   	5518     					// 5518:A/C 开通业务查询  查询缴费业务是否开通   
#define	    TC_AC_PIN_VALI   	    5529					  	// 5529:A/C 凭密验证身份
#define		TC_AC_ACCDETAIL_INQ   	5402						// 5402:A/C 查询客户资金明细
#define		TC_AC_CHARGE     	    5403						// 5403:A/C 计算手续费
#define		TC_AC_BALANCE_INQ    	5407						// 5407:A/C 查询账户余额 //???
#define		TC_AC_ADD_INQ   	    5410						// 5410:A/C 通过邮编查地址
#define		TC_AC_POSTCODE_INQ     	5411						// 5411:A/C 通过地址查邮编
#define		TC_AC_STATUS_VALI      	5303						// 5303:A/C 客户身份认证
#define		TC_AC_TRAN_INQ   	    5416						// 5416:A/C 查询应用交易类型  查询非约定缴费记录
#define		TC_AC_ASSU_TRAN   	    5317						// 5317:A/C 约定交易 // atmunet
#define		TC_AC_CHANGE_PIN   	    5319						// 5319:A/C 修改验证密码
#define		TC_AC_NOASSU_PAY   	    5327						// 5327:A/C 非约定缴费 // atmunet
#define		TC_AC_PAY_INQ   	    5328						// 5328:A/C 缴费欠费查询 // atmunet

#define		TC_READY			(1000 + TRAN_READY)				// READY
#define		TC_OPEN				(1000 + TRAN_OPEN)				// OPEN
#define		TC_CANCEL			(1000 + TRAN_CANCEL)			// CANCEL
#define		TC_ERROR			(1000 + TRAN_ERROR)				// ERROR
#define		TC_AEXSTATUS		(1000 + TRAN_AEXSTATUS)			// AEXSTATUS
#define		TC_OEXSTATUS		(1000 + TRAN_OEXSTATUS)			// OEXSTATUS
#define		TC_MSRSTATUS		(1000 + TRAN_MSRSTATUS)			// MSRSTATUS
#define		TC_TRAN				(1000 + TRAN_TRAN)				// TRAN
#define		TC_TOTAL			(1000 + TRAN_TOTAL)				// TOTAL
#define		TC_SUBTOTAL			(1000 + TRAN_SUBTOTAL)			// SUBTOTAL
#define		TC_ADDCASH			(1000 + TRAN_ADDCASH)			// ADDCASH
#define		TC_SUBCASH			(1000 + TRAN_SUBCASH)			// SUBCASH
#define		TC_CLEARCASH		(1000 + TRAN_CLEARCASH)			// CLEARCASH
#define		TC_PRINTCASH		(1000 + TRAN_PRINTCASH)			// PRINTCASH
#define		TC_DOWNFIT			(1000 + TRAN_DOWNFIT)			// DOWNFIT
#define		TC_DOWNPARM			(1000 + TRAN_DOWNPARM)			// DOWNFIT
#define		TC_DOWNPAY			(1000 + TRAN_DOWNPAY)			// DOWNFIT
#define		TC_SENDDETAIL		(1000 + TRAN_SENDDETAIL)		// SENDDETAIL
#define		TC_DETAILEND		(1000 + TRAN_DETAILEND)			// DETAILEND
#define		TC_CONFIRMWITH		(1000 + TRAN_CONFIRMWITH)		// CONFIRMWITH
#define		TC_DCSENDEJR		(1000 + TRAN_DCSENDEJR)			// DCSENDEJR : 2006.07.03
#define		TC_DCEJREND			(1000 + TRAN_DCEJREND)			// DCEJREND  : 2006.07.03
#define		TC_DOWNICFIT		(1000 + TRAN_DOWNICFIT)			// DOWNICFIT add by yusy ic
#define		TC_DOWNAIDLIST		(1000 + TRAN_DOWNAIDLIST)		
#define		TC_DOWNPBOCPARA		(1000 + TRAN_DOWNPBOCPARA)
#define		TC_DOWNECHANNELPARA	(1000 + TRAN_DOWNECHANNELPARA)
#define		TC_DOWNECHANNELPROT	(1000 + TRAN_DOWNECHANNELPROT)
#define		TC_SCRIPT			(1000 + TRAN_SCRIPT)			// 脚本通知
#define		TC_CARDRETAIN		(1000 + TRAN_CARDRETAIN)		// CARDRETAIN
#define		TC_TOTALKIND		(1000 + TRAN_TOTALKIND)			// TOTALKIND
#define		TC_TOTALPRE			(1000 + TRAN_TOTALPRE)			// TOTALPRE

/////////////////////////////////////////////////////////////////////////////
//	NMS TRANSACTION : 2006.07.03
/////////////////////////////////////////////////////////////////////////////
#define		TC_ATMOPEN			(2001 + TRAN_BIDCOMMAND)		// ATMOPEN
#define		TC_ATMCLOSE			(2002 + TRAN_BIDCOMMAND)		// ATMCLOSE
#define		TC_ATMSTATUS		(2003 + TRAN_BIDCOMMAND)		// ATMSTATUS
#define		TC_ATMBIDSENDDETAIL	(2004 + TRAN_BIDCOMMAND)		// ATMBIDSENDDETAIL
#define		TC_ATMWORKPARAM		(2005 + TRAN_BIDCOMMAND)		// ATMWORKPARAM
#define		TC_ATMRESETALL		(2006 + TRAN_BIDCOMMAND)		// ATMRESETALL
#define		TC_ATMBIDADDOWNLOAD	(2007 + TRAN_BIDCOMMAND)		// ADDOWNLOAD
#define		TC_ATMBIDSOFTUPDATE	(2008 + TRAN_BIDCOMMAND)		// SOFTUPDATA

#define		TC_ATMOPENDC		(2101 + TRAN_BIDCOMMAND)		// ATMOPENDC
#define		TC_ATMCLOSEDC		(2102 + TRAN_BIDCOMMAND)		// ATMCLOSEDC
#define		TC_ATMSTATUSDC		(2103 + TRAN_BIDCOMMAND)		// ATMSTATUSDC
#define		TC_ATMOPENSYNC		(2104 + TRAN_BIDCOMMAND)		// ATMOPENSYNCDC
#define		TC_ATMCLOSESYNC		(2105 + TRAN_BIDCOMMAND)		// ATMCLOSESYNCDC
#define		TC_ATMSTATUSSYNC	(2106 + TRAN_BIDCOMMAND)		// ATMSTATUSSYNCDC
#define		TC_ATMBIDDCSENDEJR	(2107 + TRAN_BIDCOMMAND)		// ATMBIDDCSENDEJR

#define		TC_ATMREBOOT		(2200 + TRAN_BIDCOMMAND)		// ATMREBOOT
#define		TC_ATMFTPSEND		(2201 + TRAN_BIDCOMMAND)		// ATMFTPSEND
#define		TC_ATMRESETDVR		(2202 + TRAN_BIDCOMMAND)		// ATMRESETDVR


/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
static MENUHeader MenuTblConst[] = 
{
////MENUINFO(0)			CARD INSERT(1)
////						PASSBOOK INSERT(2)
////						AUTO CHOICE(2)(1 BUTTON)
////							INPUT(3)/BUTTON CHOICE(3)
////								TRANPROC(4)
////											TRANCODE(5)
////														TRANCODE2(6)
//// 0:MAIN SCREEN(NO EXIT)
	PIN_MENU_MODE,												// PIN INPUT MODE
	"",
	0,															// MAIN SCREEN(NO EXIT)
	"",
	"",
	"",
	"NOCARD",
	"ECHANNEL",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	MENU_IC,			5,	0,	0,	TRAN_NULL,	0,			0,	// 0:PROCESS INFORMATION
	MENU_MCU23,			2,	0,	0,	TRAN_NULL,	0,			0,	// 1:
	MENU_IC,			8,	0,	0,	TRAN_NULL,	0,			0,	// 2:
	MENU_NC,			0,	0,	0,	TRAN_DEP,	TC_CASHTRANS, 0,// 3:
	MENU_NULL,			0,	0,	10,	TRAN_NULL,	0,			0,	// 4:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 5:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 6:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 7:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 8:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 9:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 10:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 11:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 12:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 13:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 14:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 15:

//// 1:MAIN SCREEN(EXIT)
	PIN_MENU_MODE,												// PIN INPUT MODE
	"",
	0,															// MAIN SCREEN(EXIT)
	"",
	"",
	"",
	"NOCARD",
	"ECHANNEL",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	MENU_IC,			5,	0,	0,	TRAN_NULL,	0,			0,	// 0:PROCESS INFORMATION
	MENU_MCU23,			2,	0,	0,	TRAN_NULL,	0,			0,	// 1:
	MENU_IC,			8,	0,	0,	TRAN_NULL,	0,			0,	// 2:
	MENU_NC,			0,	0,	0,	TRAN_DEP,	TC_CASHTRANS,0,	// 3:
	MENU_NULL,			0,	0,	10,	TRAN_NULL,	0,			0,	// 4:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 5:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 6:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 7:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 8:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 9:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 10:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 11:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 12:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 13:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 14:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 15:

//// 2:INPUT PIN
	PIN_INPUT_MODE,											// PIN INPUT MODE
	"",
	101,													// INPUT PIN SCREEN
	"",
	"CLEAR",
	"ENTER",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	MENU_NULL,			0,	0,	3,	TRAN_NULL,	0,			0,	// 0:PROCESS INFORMATION
	MENU_NULL,			0,	0,	2,	TRAN_NULL,	0,			0,	// 1:CLEAR
	MENU_NULL,			0,	0,	2,	TRAN_NULL,	0,			0,	// 2:ENTER
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 3:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 4:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 5:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 6:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 7:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 8:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 9:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 10:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 11:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 12:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 13:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 14:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 15:

//// 3:SELECT SERVICE
	PIN_MENU_MODE,												// PIN INPUT MODE
	"",
	12,															// SELECT SERVICE SCREEN
	"",
	"TRANSFER",
	"PAYMENT",
	"ACMANAGER",
	//"OTHER",
	"",															// EXIT
	"INQUIRY",
	"WITHDRAWAL",
	"",
	"CHANGEPIN",												
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 0:PROCESS INFORMATION
	MENU_MCU23,			0,	0,	0,	TRAN_TRANS,	TC_TRANSFER,0,	// 1:TRANSFER
	MENU_MCU23,			0,	0,	0,	TRAN_TRANS,	TC_PAY,		0,	// 2:PAY
	MENU_MCU23,			0,	0,	0,	TRAN_TRANS,	TC_ACMANAGER, 0,// 3:A/C MANAGER
	//MENU_MCU23,			0,	0,	0,	TRAN_TRANS,	TC_OTHER, 0,// 3:OTHER
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 4:
	MENU_MCU23,			0,	0,	0,	TRAN_INQ,	TC_INQUIRY,	0,	// 5:INQUIRY,DETAIL
	MENU_MCU23,			0,	0,	0,	TRAN_WITH,	TC_WITHDRAWAL,0,// 6:WITHDRAWAL
	MENU_MCU23,			0,	0,	0,	TRAN_DEP,	TC_DEPOSIT,	0,	// 7:DEPOSIT
	MENU_MCU23,			0,	0,	0,	TRAN_INQ,	TC_CHANGEPIN,0,	// 8:CHANGEPIN
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 9:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 10:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 11:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 12:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 13:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 14:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 15:

//// 4:REMIT SERVICE
	PIN_MENU_MODE,												// PIN INPUT MODE
	"",
	14,															// SELECT SERVICE SCREEN  需要确定页面号  by zjd
	"",
	"REMIT_ADD",
	"REMIT_IN",
	"REMIT_PWD",
	"RETURN",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 0:PROCESS INFORMATION
	MENU_MCU23,			0,	0,	0,	TRAN_TRANS,	TC_REMIT_ADD,0,	// 1:ADDRESS
	MENU_MCU23,			0,	0,	0,	TRAN_TRANS,	TC_REMIT_IN,0,	// 2:IN
	MENU_MCU23,			0,	0,	0,	TRAN_TRANS,	TC_REMIT_PWD,0,	// 3:PASSWORD
	MENU_NULL,			0,	0,	3,	TRAN_NULL,	0,			0,	// 4:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 5:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 6:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 7:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 8:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 9:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 10:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 11:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 12:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 13:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 14:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 15:

//// 5:INPUT PIN FOR IC
	PIN_INPUT_MODE,											// PIN INPUT MODE
	"",
	101,													// INPUT PIN SCREEN
	"",
	"CLEAR",
	"ENTER",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	MENU_NULL,			0,	0,	6,	TRAN_NULL,	0,			0,	// 0:PROCESS INFORMATION
	MENU_NULL,			0,	0,	5,	TRAN_NULL,	0,			0,	// 1:CLEAR
	MENU_NULL,			0,	0,	5,	TRAN_NULL,	0,			0,	// 2:ENTER
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 3:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 4:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 5:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 6:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 7:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 8:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 9:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 10:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 11:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 12:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 13:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 14:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 15:

//// 6:IC TYPE SELECT
	PIN_MENU_MODE,											// PIN INPUT MODE
	"",
	59,													// INPUT PIN SCREEN
	"",
	"SavingAccount",
	"",
	"",
	"",
	"CreditAccout",
	"ICAccount",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 0:PROCESS INFORMATION
	MENU_NULL,			0,	0,	9,	TRAN_NULL,	0,			0,	// 1:CLEAR
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 2:ENTER
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 3:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 4:
	MENU_NULL,			0,	0,	9,	TRAN_NULL,	0,			0,	// 5:
	MENU_NULL,			0,	0,	7,	TRAN_NULL,	0,			0,	// 6:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 7:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 8:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 9:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 10:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 11:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 12:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 13:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 14:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 15:

//// 7:SELECT SERVICE
	PIN_MENU_MODE,												// PIN INPUT MODE
	"",
	60,															// SELECT SERVICE SCREEN
	"",
	"BANLANCE",
	"DETAIL",
	"",
	"RETURN",															// EXIT
	"SAVE",
	"MANAGE",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 0:PROCESS INFORMATION
	MENU_MCU23,			0,	0,	0,	TRAN_TRANS,	TC_ICINQUIRY,0,// 1:WITHDRAWAL
	MENU_MCU23,			0,	0,	0,	TRAN_TRANS,	TC_ICDETAIL,0,	// 2:TRANSFER
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 3:REMIT 2008.10.05
	MENU_NULL,			0,	0,	6,	TRAN_NULL,	0,			0,	// 4:
	MENU_MCU23,			0,	0,	0,	TRAN_TRANS,	TC_LOAD,	0,	// 5:INQUIRY,DETAIL
	MENU_MCU23,			0,	0,	0,	TRAN_TRANS,	TC_BOUNDMDOIFY,  0,	// 6:DETAIL
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 7:PAY
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,// 8:A/C MANAGER
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 9:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 10:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 11:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 12:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 13:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 14:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 15:

//// 8:SELECT SERVICE
	PIN_MENU_MODE,												// PIN INPUT MODE
	"",
	64,															// SELECT SERVICE SCREEN
	"",
	"BANLANCE",
	"DETAIL",
	"",
	"",															// EXIT
	"SAVE",
	"MANAGE",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 0:PROCESS INFORMATION
	MENU_IC,			0,	0,	0,	TRAN_TRANS,	TC_ICINQUIRY,0,// 1:BANLANCE
	MENU_IC,			0,	0,	0,	TRAN_TRANS,	TC_ICDETAIL,0,	// 2:DETAIL
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 3:
	MENU_NULL,			0,	0,	6,	TRAN_NULL,	0,			0,	// 4:
	MENU_IC,			0,	0,	0,	TRAN_TRANS,	TC_LOAD,	0,	// 5:SAVE
	MENU_IC,			0,	0,	0,	TRAN_TRANS,	TC_BOUNDMDOIFY,  0,	// 6:MANAGE
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 7:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 8:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 9:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 10:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 11:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 12:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 13:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 14:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 15:

//// 9:SELECT SERVICE
	PIN_MENU_MODE,												// PIN INPUT MODE
	"",
	66,															// SELECT SERVICE SCREEN
	"",
	"TRANSFER",
	"PAYMENT",
	"ACMANAGER",
	//"OTHER",
	"RETURN",													// RETURN
	"INQUIRY",
	"WITHDRAWAL",
	"",
	"CHANGEPIN",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 0:PROCESS INFORMATION
	MENU_MCU23,			0,	0,	0,	TRAN_TRANS,	TC_TRANSFER,0,	// 1:TRANSFER
	MENU_MCU23,			0,	0,	0,	TRAN_TRANS,	TC_PAY,		0,	// 2:PAY
	MENU_MCU23,			0,	0,	0,	TRAN_TRANS,	TC_ACMANAGER, 0,// 3:A/C MANAGER
	//MENU_MCU23,			0,	0,	0,	TRAN_TRANS,	TC_OTHER, 0,// 3:OTHER
	MENU_NULL,			0,	0,	6,	TRAN_NULL,	0,			0,	// 4:RETURN
	MENU_MCU23,			0,	0,	0,	TRAN_INQ,	TC_INQUIRY,	0,	// 5:INQUIRY,DETAIL
	MENU_MCU23,			0,	0,	0,	TRAN_WITH,	TC_WITHDRAWAL,0,// 6:WITHDRAWAL
	MENU_MCU23,			0,	0,	0,	TRAN_DEP,	TC_DEPOSIT,	0,	// 7:DEPOSIT
	MENU_MCU23,			0,	0,	0,	TRAN_INQ,	TC_CHANGEPIN,0,	// 8:CHANGEPIN
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 9:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 10:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 11:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 12:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 13:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 14:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 15:

	

//// 10:SELECT SERVICE
	PIN_MENU_MODE,												// PIN INPUT MODE
	"",
	610,														// MAIN SCREEN(NO EXIT)
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	MENU_IC,			5,	0,	0,	TRAN_NULL,	0,			0,	// 0:PROCESS INFORMATION
	MENU_MCU23,			2,	0,	0,	TRAN_NULL,	0,			0,	// 1:
	MENU_IC,			8,	0,	0,	TRAN_NULL,	0,			0,	// 2:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 3:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 4:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 5:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 6:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 7:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 8:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 9:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 10:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 11:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 12:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 13:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 14:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 15:

//// N: LAST
	PIN_DISABLE_MODE,											// PIN INPUT MODE
	"",
	0,															// SCREEN
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 0:PROCESS INFORMATION
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 1:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 2:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 3:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 4:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 5:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 6:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 7:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 8:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 9:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 10:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 11:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 12:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 13:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0,	// 14:
	MENU_NULL,			0,	0,	0,	TRAN_NULL,	0,			0	// 15:
};

/////////////////////////////////////////////////////////////////////////////
#endif

