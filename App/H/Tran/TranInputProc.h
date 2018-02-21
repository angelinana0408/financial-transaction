/////////////////////////////////////////////////////////////////////////////
#if !defined(_TranInputProc_H_INCLUDED_)
#define _TranInputProc_H_INCLUDED_

/////////////////////////////////////////////////////////////////////////////
// Attributes
public:
/////////////////////////////////////////////////////////////////////////////
//	Limit Amount Variable
/////////////////////////////////////////////////////////////////////////////
	int		MoneyMaxOut;				// Money Max Out
	double	TMoneyMaxOut;				// Transfer Money Max Out
	int		TransPassWordFlag;
	double	FirstPbcECInq;
	double	LastPbcECInq;

/////////////////////////////////////////////////////////////////////////////
//	Input Variable
/////////////////////////////////////////////////////////////////////////////
struct	tagAccept
{
//	Input(Menu)
	char	PassWord[16];				// PassWord

//	Input(Before Send)
//	Input Money(Before Send)
	char	Money[12];					// Money
	
//	Input Other(Before Send)
	char	NewPassWord[16];			// New PassWord
	char	LogInPassWord[16];
	char	TransPassWord[16];
	char	CellPhoneNo[11];
	char	SMSParityCode[6];
	char	RecvData61[1024];
	char	RecvData63[1024];
	
	int     InAccountNumSize;			// In Account Number Size
	char	InAccountNum[28];			// In Account Number
	CString	InAccountName;				// In Account Name			added by yusy 2010.03.22

//	Input(After Recv)
	int		SlipSelectFlag;				// Slip Select Flag
} Accept;

/////////////////////////////////////////////////////////////////////////////
// Operations
public:
/////////////////////////////////////////////////////////////////////////////
//	Calculate Money Function
/////////////////////////////////////////////////////////////////////////////
	int		fnAPP_CalcMoneyMaxOut();							// Calculate Money Max Out
	int		fnAPP_CalcMoneyMaxOut10050();						// Calculate Money Max Out 100 50
	double	fdAPP_CalcTMoneyMaxOut();							// Calculate Transfer Money Max Out

/////////////////////////////////////////////////////////////////////////////
//	Input function
/////////////////////////////////////////////////////////////////////////////
	int		fnAPP_AcceptWith();									// Accept Withdraw
	int		fnAPP_AcceptInq();									// Accept Inquiry
	int		fnAPP_AcceptTrans();								// Accept Transfer
	int		fnAPP_AcceptDep();									// Accept Deposit

	int		fnAPP_AcceptMoney();								// Accept Money
	int		fnAPP_AcceptMoney10050();							// Accept Money 100 50
	int		fnAPP_AcceptTMoney();								// Accept Transfer Money

	int		fnAPP_AcceptNewPassWord(int firstFlag);				// Accept New PassWord 0: 首次输入新密码 1: 密码错后重新输入新密码
	int		fnAPP_AcceptInAccountNum();							// Accept In Account Number

	int		fnAPP_Trans_LiveToOther();							//liyi add 绿卡通 活期转其它品种
	int		fnAPP_Trans_OtherToLive();							//liyi add 绿卡通 其它品种转活期
	int		fnAPP_Trans_ToOtherMoney();							//liyi add 绿卡通 活期转其它品种金额
/////////////////////////////////////////////////////////////////////////////
	int		fnAPP_AcceptIcLoadAccountMoney();					// Accept Ic Load Account Money
	int		fnAPP_AcceptIcLimitMoney();							// Accept Ic Limit Money
	int		fnAPP_AcceptIcLoadAccountMoneyConfirm();			// Accept Ic Load Account Money Confirm
	int		fnAPP_AcceptIcLimitConfirm();						// Accept Ic Limit Confirm
#endif
