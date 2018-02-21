/////////////////////////////////////////////////////////////////////////////
//	DevCsh.cpp : implementation file
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"

#include "..\..\H\Common\CmnLibIn.h"
#include "..\..\H\Common\ConstDef.h"

#include "..\..\H\Dll\DevDefine.h"
#include "..\..\H\Dll\DevCmn.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
//	CASH FUNCTION(CASH) : CASH SET MODE
/////////////////////////////////////////////////////////////////////////////
// Clear Error Code
int	CDevCmn::fnCSH_ClearErrorCode()
{
	int		nReturn = FALSE;

	switch (AtmDefine.CashHandler)
	{
		case BRM_SET:
			nReturn = fnBRM_ClearErrorCode();
			break;
		case CDU_SHUTTER1510_SET:
		case CDU_SHUTTER20_SET:
		case CDU_SHUTTER30_SET:
		case CDU_RETRACT20_SET:
		case CDU_RETRACT30_SET:
		case CDU_PRESENT20_SET:
		case CDU_PRESENT30_SET:
		case CDU_GPRESENT20_SET:
		case CDU_GPRESENT30_SET:
		case CDU_TRAY20_SET:
		case CDU_TRAY30_SET:
			nReturn = fnCDU_ClearErrorCode();
			break;
	}

	return nReturn;
}

// Clear Dispense Information
int	CDevCmn::fnCSH_ClearDispenseInfo()
{
	int		nReturn = FALSE;

	switch (AtmDefine.CashHandler)
	{
		case BRM_SET:
			nReturn = fnBRM_ClearDispenseInfo();
			break;
		case CDU_SHUTTER1510_SET:
		case CDU_SHUTTER20_SET:
		case CDU_SHUTTER30_SET:
		case CDU_RETRACT20_SET:
		case CDU_RETRACT30_SET:
		case CDU_PRESENT20_SET:
		case CDU_PRESENT30_SET:
		case CDU_GPRESENT20_SET:
		case CDU_GPRESENT30_SET:
		case CDU_TRAY20_SET:
		case CDU_TRAY30_SET:
			nReturn = fnCDU_ClearDispenseInfo();
			break;
	}

	return nReturn;
}

// Set Value Of Cash(Cash Value Data)
int	CDevCmn::fnCSH_SetValueOfCash(LPCTSTR szCashValueData)
{
	int		nReturn = FALSE;

	switch (AtmDefine.CashHandler)
	{
		case BRM_SET:
			nReturn = fnBRM_SetValueOfCash(szCashValueData);
			break;
		case CDU_SHUTTER1510_SET:
		case CDU_SHUTTER20_SET:
		case CDU_SHUTTER30_SET:
		case CDU_RETRACT20_SET:
		case CDU_RETRACT30_SET:
		case CDU_PRESENT20_SET:
		case CDU_PRESENT30_SET:
		case CDU_GPRESENT20_SET:
		case CDU_GPRESENT30_SET:
		case CDU_TRAY20_SET:
		case CDU_TRAY30_SET:
			nReturn = fnCDU_SetValueOfCash(szCashValueData);
			break;
	}

	return nReturn;
}

// Set MinMax Of CST(Min Count Data, Max Count Data)
int	CDevCmn::fnCSH_SetMinMaxCST(LPCTSTR szMinCntData, LPCTSTR szMaxCntData)
{
	int		nReturn = FALSE;

	switch (AtmDefine.CashHandler)
	{
		case BRM_SET:
			nReturn = fnBRM_SetMinMaxCST(szMinCntData, szMaxCntData);
			break;
		case CDU_SHUTTER1510_SET:
		case CDU_SHUTTER20_SET:
		case CDU_SHUTTER30_SET:
		case CDU_RETRACT20_SET:
		case CDU_RETRACT30_SET:
		case CDU_PRESENT20_SET:
		case CDU_PRESENT30_SET:
		case CDU_GPRESENT20_SET:
		case CDU_GPRESENT30_SET:
		case CDU_TRAY20_SET:
		case CDU_TRAY30_SET:
			nReturn = fnCDU_SetMinMaxCST(szMinCntData, szMaxCntData);
			break;
	}

	return nReturn;
}

// Set Number Of Cash(Cash Count Data, Total Cash Count Data)
int	CDevCmn::fnCSH_SetNumberOfCash(LPCTSTR szCashCntData, LPCTSTR szTotalCashCntData)
{
	int		nReturn = FALSE;
	CString	strTotalCashCntData(szTotalCashCntData);			// 2004.11.05
	CStringArray strTotalCashCntDataArray;

	SplitString(strTotalCashCntData, ",", strTotalCashCntDataArray);

	switch (AtmDefine.CashHandler)
	{
		case BRM_SET:
			nReturn = fnBRM_SetNumberOfCash(szCashCntData);		// 2004.11.05

			if ((CSTCnt >= BRM_CST_1) &&						// 2004.11.05
				(strTotalCashCntDataArray.GetSize() > BRM_CST_1))
			{
				RegSetInt(_REGKEY_DEVICE, _REGKEY_DEVICE_TOTALSETCASHCST1, 
										  Asc2Int(strTotalCashCntDataArray[BRM_CST_1])); 
			}
			if ((CSTCnt >= BRM_CST_2) &&
				(strTotalCashCntDataArray.GetSize() > BRM_CST_2))
			{
				RegSetInt(_REGKEY_DEVICE, _REGKEY_DEVICE_TOTALSETCASHCST2, 
										  Asc2Int(strTotalCashCntDataArray[BRM_CST_2])); 
			}
			if ((CSTCnt >= BRM_CST_3) &&
				(strTotalCashCntDataArray.GetSize() > BRM_CST_3))
			{
				RegSetInt(_REGKEY_DEVICE, _REGKEY_DEVICE_TOTALSETCASHCST3, 
										  Asc2Int(strTotalCashCntDataArray[BRM_CST_3])); 
			}
			if ((CSTCnt >= BRM_CST_4) &&
				(strTotalCashCntDataArray.GetSize() > BRM_CST_4))
			{
				RegSetInt(_REGKEY_DEVICE, _REGKEY_DEVICE_TOTALSETCASHCST4, 
										  Asc2Int(strTotalCashCntDataArray[BRM_CST_4])); 
			}
			break;
		case CDU_SHUTTER1510_SET:
		case CDU_SHUTTER20_SET:
		case CDU_SHUTTER30_SET:
		case CDU_RETRACT20_SET:
		case CDU_RETRACT30_SET:
		case CDU_PRESENT20_SET:
		case CDU_PRESENT30_SET:
		case CDU_GPRESENT20_SET:
		case CDU_GPRESENT30_SET:
		case CDU_TRAY20_SET:
		case CDU_TRAY30_SET:
			nReturn = fnCDU_SetNumberOfCash(szCashCntData);		// 2004.11.05

			if ((CSTCnt >= CDU_CST_1) &&						// 2004.11.05
				(strTotalCashCntDataArray.GetSize() > CDU_CST_1))
			{
				RegSetInt(_REGKEY_DEVICE, _REGKEY_DEVICE_TOTALSETCASHCST1, 
										  Asc2Int(strTotalCashCntDataArray[CDU_CST_1])); 
			}
			if ((CSTCnt >= CDU_CST_2) &&
				(strTotalCashCntDataArray.GetSize() > CDU_CST_2))
			{
				RegSetInt(_REGKEY_DEVICE, _REGKEY_DEVICE_TOTALSETCASHCST2, 
										  Asc2Int(strTotalCashCntDataArray[CDU_CST_2])); 
			}
			if ((CSTCnt >= CDU_CST_3) &&
				(strTotalCashCntDataArray.GetSize() > CDU_CST_3))
			{
				RegSetInt(_REGKEY_DEVICE, _REGKEY_DEVICE_TOTALSETCASHCST3, 
										  Asc2Int(strTotalCashCntDataArray[CDU_CST_3])); 
			}
			if ((CSTCnt >= CDU_CST_4) &&
				(strTotalCashCntDataArray.GetSize() > CDU_CST_4))
			{
				RegSetInt(_REGKEY_DEVICE, _REGKEY_DEVICE_TOTALSETCASHCST4, 
										  Asc2Int(strTotalCashCntDataArray[CDU_CST_4])); 
			}
			break;
	}

	return nReturn;
}

// Set Number Of Dispense Cash(Dispense Count Data)
int	CDevCmn::fnCSH_SetNumberOfDispenseCash(LPCTSTR szDispCntData)
{
	int		nReturn = FALSE;
	CString	strDispCntData(szDispCntData);						// 2004.11.05
	CStringArray strDispCntDataArray;

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCSH_SetNumberOfDispenseCash(strDispCntData[%s])", strDispCntData);
	SplitString(strDispCntData, ",", strDispCntDataArray);

	switch (AtmDefine.CashHandler)
	{
		case BRM_SET:
			if ((CSTCnt >= BRM_CST_1) &&						// 2004.11.05
				(strDispCntDataArray.GetSize() > BRM_CST_1))
			{
				RegSetInt(_REGKEY_DEVICE, _REGKEY_DEVICE_DISPCASHCST1, 
										  Asc2Int(strDispCntDataArray[BRM_CST_1])); 
			}
			if ((CSTCnt >= BRM_CST_2) &&
				(strDispCntDataArray.GetSize() > BRM_CST_2))
			{
				RegSetInt(_REGKEY_DEVICE, _REGKEY_DEVICE_DISPCASHCST2, 
										  Asc2Int(strDispCntDataArray[BRM_CST_2])); 
			}
			if ((CSTCnt >= BRM_CST_3) &&
				(strDispCntDataArray.GetSize() > BRM_CST_3))
			{
				RegSetInt(_REGKEY_DEVICE, _REGKEY_DEVICE_DISPCASHCST3, 
										  Asc2Int(strDispCntDataArray[BRM_CST_3])); 
			}
			if ((CSTCnt >= BRM_CST_4) &&
				(strDispCntDataArray.GetSize() > BRM_CST_4))
			{
				RegSetInt(_REGKEY_DEVICE, _REGKEY_DEVICE_DISPCASHCST4, 
										  Asc2Int(strDispCntDataArray[BRM_CST_4])); 
			}
			break;
		case CDU_SHUTTER1510_SET:
		case CDU_SHUTTER20_SET:
		case CDU_SHUTTER30_SET:
		case CDU_RETRACT20_SET:
		case CDU_RETRACT30_SET:
		case CDU_PRESENT20_SET:
		case CDU_PRESENT30_SET:
		case CDU_GPRESENT20_SET:
		case CDU_GPRESENT30_SET:
		case CDU_TRAY20_SET:
		case CDU_TRAY30_SET:
			if ((CSTCnt >= CDU_CST_1) &&						// 2004.11.05
				(strDispCntDataArray.GetSize() > CDU_CST_1))
			{
				RegSetInt(_REGKEY_DEVICE, _REGKEY_DEVICE_DISPCASHCST1, 
										  Asc2Int(strDispCntDataArray[CDU_CST_1])); 
			}
			if ((CSTCnt >= CDU_CST_2) &&
				(strDispCntDataArray.GetSize() > CDU_CST_2))
			{
				RegSetInt(_REGKEY_DEVICE, _REGKEY_DEVICE_DISPCASHCST2, 
										  Asc2Int(strDispCntDataArray[CDU_CST_2])); 
			}
			if ((CSTCnt >= CDU_CST_3) &&
				(strDispCntDataArray.GetSize() > CDU_CST_3))
			{
				RegSetInt(_REGKEY_DEVICE, _REGKEY_DEVICE_DISPCASHCST3, 
										  Asc2Int(strDispCntDataArray[CDU_CST_3])); 
			}
			if ((CSTCnt >= CDU_CST_4) &&
				(strDispCntDataArray.GetSize() > CDU_CST_4))
			{
				RegSetInt(_REGKEY_DEVICE, _REGKEY_DEVICE_DISPCASHCST4, 
										  Asc2Int(strDispCntDataArray[CDU_CST_4])); 
			}
			break;
	}

	return nReturn;
}

// Set Avail Deposit(Deposit Type)
int	CDevCmn::fnCSH_SetAvailDeposit(int nDepType)
{
	int		nReturn = FALSE;

	switch (AtmDefine.CashHandler)
	{
		case BRM_SET:
			nReturn = fnBRM_SetAvailDeposit(nDepType);
			break;
		case CDU_SHUTTER1510_SET:
		case CDU_SHUTTER20_SET:
		case CDU_SHUTTER30_SET:
		case CDU_RETRACT20_SET:
		case CDU_RETRACT30_SET:
		case CDU_PRESENT20_SET:
		case CDU_PRESENT30_SET:
		case CDU_GPRESENT20_SET:
		case CDU_GPRESENT30_SET:
		case CDU_TRAY20_SET:
		case CDU_TRAY30_SET:
			break;
	}

	return nReturn;
}

/////////////////////////////////////////////////////////////////////////////
//	CASH FUNCTION(CASH) : CASH GET MODE
/////////////////////////////////////////////////////////////////////////////
// Get Error Code
CString CDevCmn::fstrCSH_GetErrorCode()
{
	CString strReturn("");

	switch (AtmDefine.CashHandler)
	{
		case BRM_SET:
			strReturn = fstrBRM_GetErrorCode();
			break;
		case CDU_SHUTTER1510_SET:
		case CDU_SHUTTER20_SET:
		case CDU_SHUTTER30_SET:
		case CDU_RETRACT20_SET:
		case CDU_RETRACT30_SET:
		case CDU_PRESENT20_SET:
		case CDU_PRESENT30_SET:
		case CDU_GPRESENT20_SET:
		case CDU_GPRESENT30_SET:
		case CDU_TRAY20_SET:
		case CDU_TRAY30_SET:
			strReturn = fstrCDU_GetErrorCode();
			break;
	}

	return strReturn;
}

// Get Sensor Information
CString CDevCmn::fstrCSH_GetSensorInfo()
{
	CString	strReturn("");

	switch (AtmDefine.CashHandler)
	{
		case BRM_SET:
			strReturn = fstrBRM_GetSensorInfo();
			break;
		case CDU_SHUTTER1510_SET:
		case CDU_SHUTTER20_SET:
		case CDU_SHUTTER30_SET:
		case CDU_RETRACT20_SET:
		case CDU_RETRACT30_SET:
		case CDU_PRESENT20_SET:
		case CDU_PRESENT30_SET:
		case CDU_GPRESENT20_SET:
		case CDU_GPRESENT30_SET:
		case CDU_TRAY20_SET:
		case CDU_TRAY30_SET:
			strReturn = fstrCDU_GetSensorInfo();
			break;
	}

	return strReturn;
}

// Get Device Status
int	CDevCmn::fnCSH_GetDeviceStatus()
{
	int		nReturn = FALSE;

	switch (AtmDefine.CashHandler)
	{
		case BRM_SET:
			nReturn = fnBRM_GetDeviceStatus();
			break;
		case CDU_SHUTTER1510_SET:
		case CDU_SHUTTER20_SET:
		case CDU_SHUTTER30_SET:
		case CDU_RETRACT20_SET:
		case CDU_RETRACT30_SET:
		case CDU_PRESENT20_SET:
		case CDU_PRESENT30_SET:
		case CDU_GPRESENT20_SET:
		case CDU_GPRESENT30_SET:
		case CDU_TRAY20_SET:
		case CDU_TRAY30_SET:
			nReturn = fnCDU_GetDeviceStatus();
			break;
	}

	return nReturn;
}

// Brm Cim Get Device Status
int	CDevCmn::fnCSH_BrmCimGetDeviceStatus()
{
	int		nReturn = FALSE;

	switch (AtmDefine.CashHandler)
	{
		case BRM_SET:
			nReturn = fnBRM_CimGetDeviceStatus();
			break;
		case CDU_SHUTTER1510_SET:
		case CDU_SHUTTER20_SET:
		case CDU_SHUTTER30_SET:
		case CDU_RETRACT20_SET:
		case CDU_RETRACT30_SET:
		case CDU_PRESENT20_SET:
		case CDU_PRESENT30_SET:
		case CDU_GPRESENT20_SET:
		case CDU_GPRESENT30_SET:
		case CDU_TRAY20_SET:
		case CDU_TRAY30_SET:
			nReturn = fnCDU_GetDeviceStatus();
			break;
	}

	return nReturn;
}

// Brm Cdu Get Device Status
int	CDevCmn::fnCSH_BrmCduGetDeviceStatus()
{
	int		nReturn = FALSE;

	switch (AtmDefine.CashHandler)
	{
		case BRM_SET:
			nReturn = fnBRM_CduGetDeviceStatus();
			break;
		case CDU_SHUTTER1510_SET:
		case CDU_SHUTTER20_SET:
		case CDU_SHUTTER30_SET:
		case CDU_RETRACT20_SET:
		case CDU_RETRACT30_SET:
		case CDU_PRESENT20_SET:
		case CDU_PRESENT30_SET:
		case CDU_GPRESENT20_SET:
		case CDU_GPRESENT30_SET:
		case CDU_TRAY20_SET:
		case CDU_TRAY30_SET:
			nReturn = fnCDU_GetDeviceStatus();
			break;
	}

	return nReturn;
}

// Get Shutter Status
int	CDevCmn::fnCSH_GetShutterStatus()
{
	int		nReturn = FALSE;

	switch (AtmDefine.CashHandler)
	{
		case BRM_SET:
			nReturn = fnBRM_GetShutterStatus();
			break;
		case CDU_SHUTTER1510_SET:
		case CDU_SHUTTER20_SET:
		case CDU_SHUTTER30_SET:
		case CDU_RETRACT20_SET:
		case CDU_RETRACT30_SET:
		case CDU_PRESENT20_SET:
		case CDU_PRESENT30_SET:
		case CDU_GPRESENT20_SET:
		case CDU_GPRESENT30_SET:
		case CDU_TRAY20_SET:
		case CDU_TRAY30_SET:
			nReturn = fnCDU_GetShutterStatus();
			break;
	}

	return nReturn;
}

// Brm Cim Get Shutter Status
int	CDevCmn::fnCSH_BrmCimGetShutterStatus()
{
	int		nReturn = FALSE;

	switch (AtmDefine.CashHandler)
	{
		case BRM_SET:
			nReturn = fnBRM_CimGetShutterStatus();
			break;
		case CDU_SHUTTER1510_SET:
		case CDU_SHUTTER20_SET:
		case CDU_SHUTTER30_SET:
		case CDU_RETRACT20_SET:
		case CDU_RETRACT30_SET:
		case CDU_PRESENT20_SET:
		case CDU_PRESENT30_SET:
		case CDU_GPRESENT20_SET:
		case CDU_GPRESENT30_SET:
		case CDU_TRAY20_SET:
		case CDU_TRAY30_SET:
			nReturn = fnCDU_GetShutterStatus();
			break;
	}

	return nReturn;
}

// Brm Cdu Get Shutter Status
int	CDevCmn::fnCSH_BrmCduGetShutterStatus()
{
	int		nReturn = FALSE;

	switch (AtmDefine.CashHandler)
	{
		case BRM_SET:
			nReturn = fnBRM_CduGetShutterStatus();
			break;
		case CDU_SHUTTER1510_SET:
		case CDU_SHUTTER20_SET:
		case CDU_SHUTTER30_SET:
		case CDU_RETRACT20_SET:
		case CDU_RETRACT30_SET:
		case CDU_PRESENT20_SET:
		case CDU_PRESENT30_SET:
		case CDU_GPRESENT20_SET:
		case CDU_GPRESENT30_SET:
		case CDU_TRAY20_SET:
		case CDU_TRAY30_SET:
			nReturn = fnCDU_GetShutterStatus();
			break;
	}

	return nReturn;
}

// Get Retract Action
int	CDevCmn::fnCSH_GetRetractAction()
{
	int		nReturn = FALSE;

	switch (AtmDefine.CashHandler)
	{
		case BRM_SET:
			nReturn = fnBRM_GetRetractAction();
			break;
		case CDU_SHUTTER1510_SET:
		case CDU_SHUTTER20_SET:
		case CDU_SHUTTER30_SET:
		case CDU_RETRACT20_SET:
		case CDU_RETRACT30_SET:
		case CDU_PRESENT20_SET:
		case CDU_PRESENT30_SET:
		case CDU_GPRESENT20_SET:
		case CDU_GPRESENT30_SET:
		case CDU_TRAY20_SET:
		case CDU_TRAY30_SET:
			nReturn = fnCDU_GetRetractAction();
			break;
	}

	return nReturn;
}

// Brm Cim Get Retract Action
int	CDevCmn::fnCSH_BrmCimGetRetractAction()
{
	int		nReturn = FALSE;

	switch (AtmDefine.CashHandler)
	{
		case BRM_SET:
			nReturn = fnBRM_CimGetRetractAction();
			break;
		case CDU_SHUTTER1510_SET:
		case CDU_SHUTTER20_SET:
		case CDU_SHUTTER30_SET:
		case CDU_RETRACT20_SET:
		case CDU_RETRACT30_SET:
		case CDU_PRESENT20_SET:
		case CDU_PRESENT30_SET:
		case CDU_GPRESENT20_SET:
		case CDU_GPRESENT30_SET:
		case CDU_TRAY20_SET:
		case CDU_TRAY30_SET:
			nReturn = fnCDU_GetRetractAction();
			break;
	}

	return nReturn;
}

// Brm Cdu Get Retract Action
int	CDevCmn::fnCSH_BrmCduGetRetractAction()
{
	int		nReturn = FALSE;

	switch (AtmDefine.CashHandler)
	{
		case BRM_SET:
			nReturn = fnBRM_CduGetRetractAction();
			break;
		case CDU_SHUTTER1510_SET:
		case CDU_SHUTTER20_SET:
		case CDU_SHUTTER30_SET:
		case CDU_RETRACT20_SET:
		case CDU_RETRACT30_SET:
		case CDU_PRESENT20_SET:
		case CDU_PRESENT30_SET:
		case CDU_GPRESENT20_SET:
		case CDU_GPRESENT30_SET:
		case CDU_TRAY20_SET:
		case CDU_TRAY30_SET:
			nReturn = fnCDU_GetRetractAction();
			break;
	}

	return nReturn;
}

// Get Position
int	CDevCmn::fnCSH_GetPosition()
{
	int		nReturn = FALSE;

	switch (AtmDefine.CashHandler)
	{
		case BRM_SET:
			nReturn = fnBRM_GetPosition();
			break;
		case CDU_SHUTTER1510_SET:
		case CDU_SHUTTER20_SET:
		case CDU_SHUTTER30_SET:
		case CDU_RETRACT20_SET:
		case CDU_RETRACT30_SET:
		case CDU_PRESENT20_SET:
		case CDU_PRESENT30_SET:
		case CDU_GPRESENT20_SET:
		case CDU_GPRESENT30_SET:
		case CDU_TRAY20_SET:
		case CDU_TRAY30_SET:
			switch (AtmDefine.MachineKind)
			{
				case NANOCASH:
					nReturn = NORMAL_POS;
					break;
				case MCD3:
					nReturn = NORMAL_POS;
					break;
				case TTW:
					nReturn = fnCDU_GetPosition();
					break;
				case MCD4:
					nReturn = NORMAL_POS;
					break;
				case TTW2:
					nReturn = fnCDU_GetPosition();
					break;
				case EATMGBA1:									// 2005.11.11
					nReturn = NORMAL_POS;
					break;
				case EATMULL1:									// 2005.11.11
					nReturn = NORMAL_POS;
					break;
				case EATMTTW:									// 2005.11.11
					nReturn = NORMAL_POS;
					break;
				case TTW2E:										// 2005.11.11
					nReturn = NORMAL_POS;
					break;
				case MCD4E:										// 2005.11.11
					nReturn = NORMAL_POS;
					break;
				case MX8000T:									// 2005.11.11
					nReturn = NORMAL_POS;
					break;
				case MX8000L:									// 2005.11.11
					nReturn = NORMAL_POS;
					break;
				case MX5600T:
					nReturn = NORMAL_POS;
					break;
				case MX5600L:
					nReturn = NORMAL_POS;
					break;
			}
			break;
	}

	return nReturn;
}

// Get Change Unit Lock
int	CDevCmn::fnCSH_GetChangeUnitLock()
{
	int		nReturn = FALSE;

	switch (AtmDefine.CashHandler)
	{
		case BRM_SET:
			nReturn = fnBRM_GetChangeUnitLock();
			break;
		case CDU_SHUTTER1510_SET:
		case CDU_SHUTTER20_SET:
		case CDU_SHUTTER30_SET:
		case CDU_RETRACT20_SET:
		case CDU_RETRACT30_SET:
		case CDU_PRESENT20_SET:
		case CDU_PRESENT30_SET:
		case CDU_GPRESENT20_SET:
		case CDU_GPRESENT30_SET:
		case CDU_TRAY20_SET:
		case CDU_TRAY30_SET:
			nReturn = NO_CHANGE;
			break;
	}

	return nReturn;
}

// Get Clerk CST Status
int	CDevCmn::fnCSH_GetClerkCSTStatus()
{
	int		nReturn = FALSE;

	switch (AtmDefine.CashHandler)
	{
		case BRM_SET:
			nReturn = fnBRM_GetClerkCSTStatus();
			break;
		case CDU_SHUTTER1510_SET:
		case CDU_SHUTTER20_SET:
		case CDU_SHUTTER30_SET:
		case CDU_RETRACT20_SET:
		case CDU_RETRACT30_SET:
		case CDU_PRESENT20_SET:
		case CDU_PRESENT30_SET:
		case CDU_GPRESENT20_SET:
		case CDU_GPRESENT30_SET:
		case CDU_TRAY20_SET:
		case CDU_TRAY30_SET:
			nReturn = CST_NORMAL;
			break;
	}

	return nReturn;
}

// Get Reject CST Status
int	CDevCmn::fnCSH_GetRejectCSTStatus()
{
	int		nReturn = FALSE;

	switch (AtmDefine.CashHandler)
	{
		case BRM_SET:
			nReturn = fnBRM_GetRejectCSTStatus();
			break;
		case CDU_SHUTTER1510_SET:
		case CDU_SHUTTER20_SET:
		case CDU_SHUTTER30_SET:
		case CDU_RETRACT20_SET:
		case CDU_RETRACT30_SET:
		case CDU_PRESENT20_SET:
		case CDU_PRESENT30_SET:
		case CDU_GPRESENT20_SET:
		case CDU_GPRESENT30_SET:
		case CDU_TRAY20_SET:
		case CDU_TRAY30_SET:
			nReturn = fnCDU_GetRejectCSTStatus();
			break;
	}

	return nReturn;
}

// Get Retract CST Status
int	CDevCmn::fnCSH_GetRetractCSTStatus()
{
	int		nReturn = FALSE;

	switch (AtmDefine.CashHandler)
	{
		case BRM_SET:
			nReturn = fnBRM_GetRetractCSTStatus();
			break;
		case CDU_SHUTTER1510_SET:
		case CDU_SHUTTER20_SET:
		case CDU_SHUTTER30_SET:
		case CDU_RETRACT20_SET:
		case CDU_RETRACT30_SET:
		case CDU_PRESENT20_SET:
		case CDU_PRESENT30_SET:
		case CDU_GPRESENT20_SET:
		case CDU_GPRESENT30_SET:
		case CDU_TRAY20_SET:
		case CDU_TRAY30_SET:
			nReturn = CST_NORMAL;
			break;
	}

	return nReturn;
}

// Get Deposit CST Status(Cash Type)
int	CDevCmn::fnCSH_GetDepositCSTStatus(int nCashType)
{
	int		nReturn = FALSE;

	switch (AtmDefine.CashHandler)
	{
		case BRM_SET:
			nReturn = fnBRM_GetDepositCSTStatus(nCashType);
			break;
		case CDU_SHUTTER1510_SET:
		case CDU_SHUTTER20_SET:
		case CDU_SHUTTER30_SET:
		case CDU_RETRACT20_SET:
		case CDU_RETRACT30_SET:
		case CDU_PRESENT20_SET:
		case CDU_PRESENT30_SET:
		case CDU_GPRESENT20_SET:
		case CDU_GPRESENT30_SET:
		case CDU_TRAY20_SET:
		case CDU_TRAY30_SET:
			nReturn = CST_NORMAL;
			break;
	}

	return nReturn;
}

// Get Recycle Box Status
int	CDevCmn::fnCSH_GetRecycleBoxStatus()
{
	int		nReturn = FALSE;

	switch (AtmDefine.CashHandler)
	{
		case BRM_SET:
			nReturn = fnBRM_GetRecycleBoxStatus();
			break;
		case CDU_SHUTTER1510_SET:
		case CDU_SHUTTER20_SET:
		case CDU_SHUTTER30_SET:
		case CDU_RETRACT20_SET:
		case CDU_RETRACT30_SET:
		case CDU_PRESENT20_SET:
		case CDU_PRESENT30_SET:
		case CDU_GPRESENT20_SET:
		case CDU_GPRESENT30_SET:
		case CDU_TRAY20_SET:
		case CDU_TRAY30_SET:
			nReturn = fnCDU_GetRecycleBoxStatus();
			break;
	}

	return nReturn;
}

// Get Autoload Condition
int	CDevCmn::fnCSH_GetAutoloadCondition()
{
	int		nReturn = FALSE;

	switch (AtmDefine.CashHandler)
	{
		case BRM_SET:
			nReturn = fnBRM_GetAutoloadCondition();
			break;
		case CDU_SHUTTER1510_SET:
		case CDU_SHUTTER20_SET:
		case CDU_SHUTTER30_SET:
		case CDU_RETRACT20_SET:
		case CDU_RETRACT30_SET:
		case CDU_PRESENT20_SET:
		case CDU_PRESENT30_SET:
		case CDU_GPRESENT20_SET:
		case CDU_GPRESENT30_SET:
		case CDU_TRAY20_SET:
		case CDU_TRAY30_SET:
			nReturn = NEED_NONE;
			break;
	}

	return nReturn;
}

// Get Material Information
int	CDevCmn::fnCSH_GetMaterialInfo()
{
	int		nReturn = FALSE;

	switch (AtmDefine.CashHandler)
	{
		case BRM_SET:
			nReturn = fnBRM_GetMaterialInfo();
			break;
		case CDU_SHUTTER1510_SET:
		case CDU_SHUTTER20_SET:
		case CDU_SHUTTER30_SET:
		case CDU_RETRACT20_SET:
		case CDU_RETRACT30_SET:
		case CDU_PRESENT20_SET:
		case CDU_PRESENT30_SET:
		case CDU_GPRESENT20_SET:
		case CDU_GPRESENT30_SET:
		case CDU_TRAY20_SET:
		case CDU_TRAY30_SET:
			nReturn = fnCDU_GetMaterialInfo();
			break;
	}

	return nReturn;
}

// Get Avail Deposit
int	CDevCmn::fnCSH_GetAvailDeposit()
{
	int		nReturn = FALSE;

	switch (AtmDefine.CashHandler)
	{
		case BRM_SET:
			nReturn = fnBRM_GetAvailDeposit();
			break;
		case CDU_SHUTTER1510_SET:
		case CDU_SHUTTER20_SET:
		case CDU_SHUTTER30_SET:
		case CDU_RETRACT20_SET:
		case CDU_RETRACT30_SET:
		case CDU_PRESENT20_SET:
		case CDU_PRESENT30_SET:
		case CDU_GPRESENT20_SET:
		case CDU_GPRESENT30_SET:
		case CDU_TRAY20_SET:
		case CDU_TRAY30_SET:
			nReturn = DEP_NOT_AVAILABLE;
			break;
	}

	return nReturn;
}

// Get Avail Withdraw
int	CDevCmn::fnCSH_GetAvailWithdraw()
{
	int		nReturn = FALSE;

	switch (AtmDefine.CashHandler)
	{
		case BRM_SET:
			nReturn = fnBRM_GetAvailWithdraw();
			break;
		case CDU_SHUTTER1510_SET:
		case CDU_SHUTTER20_SET:
		case CDU_SHUTTER30_SET:
		case CDU_RETRACT20_SET:
		case CDU_RETRACT30_SET:
		case CDU_PRESENT20_SET:
		case CDU_PRESENT30_SET:
		case CDU_GPRESENT20_SET:
		case CDU_GPRESENT30_SET:
		case CDU_TRAY20_SET:
		case CDU_TRAY30_SET:
			nReturn = fnCDU_GetAvailWithdraw();
			break;
	}

	return nReturn;
}

// Get Accept Count Status
int	CDevCmn::fnCSH_GetAcceptCountStatus()
{
	int		nReturn = FALSE;

	switch (AtmDefine.CashHandler)
	{
		case BRM_SET:
			nReturn = fnBRM_GetAcceptCountStatus();
			break;
		case CDU_SHUTTER1510_SET:
		case CDU_SHUTTER20_SET:
		case CDU_SHUTTER30_SET:
		case CDU_RETRACT20_SET:
		case CDU_RETRACT30_SET:
		case CDU_PRESENT20_SET:
		case CDU_PRESENT30_SET:
		case CDU_GPRESENT20_SET:
		case CDU_GPRESENT30_SET:
		case CDU_TRAY20_SET:
		case CDU_TRAY30_SET:
			nReturn = NORMAL;
			break;
	}

	return nReturn;
}

// Get Count Result
CString CDevCmn::fstrCSH_GetCountResult()
{
	CString strReturn("");

	switch (AtmDefine.CashHandler)
	{
		case BRM_SET:
			strReturn = fstrBRM_GetCountResult();
			break;
		case CDU_SHUTTER1510_SET:
		case CDU_SHUTTER20_SET:
		case CDU_SHUTTER30_SET:
		case CDU_RETRACT20_SET:
		case CDU_RETRACT30_SET:
		case CDU_PRESENT20_SET:
		case CDU_PRESENT30_SET:
		case CDU_GPRESENT20_SET:
		case CDU_GPRESENT30_SET:
		case CDU_TRAY20_SET:
		case CDU_TRAY30_SET:
			strReturn = "0,0,0,0,0,0";
			break;
	}

	return strReturn;
}

// Get Retract Result
CString CDevCmn::fstrCSH_GetRetractResult()
{
	CString strReturn("");

	switch (AtmDefine.CashHandler)
	{
		case BRM_SET:
			strReturn = fstrBRM_GetRetractResult();
			break;
		case CDU_SHUTTER1510_SET:
		case CDU_SHUTTER20_SET:
		case CDU_SHUTTER30_SET:
		case CDU_RETRACT20_SET:
		case CDU_RETRACT30_SET:
		case CDU_PRESENT20_SET:
		case CDU_PRESENT30_SET:
		case CDU_GPRESENT20_SET:
		case CDU_GPRESENT30_SET:
		case CDU_TRAY20_SET:
		case CDU_TRAY30_SET:
			strReturn = "0,0,0,0,0,0";
			break;
	}

	return strReturn;
}

// Get Refill Result
CString CDevCmn::fstrCSH_GetRefillResult()
{
	CString strReturn("");

	switch (AtmDefine.CashHandler)
	{
		case BRM_SET:
			strReturn = fstrBRM_GetRefillResult();
			break;
		case CDU_SHUTTER1510_SET:
		case CDU_SHUTTER20_SET:
		case CDU_SHUTTER30_SET:
		case CDU_RETRACT20_SET:
		case CDU_RETRACT30_SET:
		case CDU_PRESENT20_SET:
		case CDU_PRESENT30_SET:
		case CDU_GPRESENT20_SET:
		case CDU_GPRESENT30_SET:
		case CDU_TRAY20_SET:
		case CDU_TRAY30_SET:
			strReturn = "0,0,0,0,0,0,0,0";
			break;
	}

	return strReturn;
}

// Get Takeup Result
CString CDevCmn::fstrCSH_GetTakeupResult()
{
	CString strReturn("");

	switch (AtmDefine.CashHandler)
	{
		case BRM_SET:
			strReturn = fstrBRM_GetTakeupResult();
			break;
		case CDU_SHUTTER1510_SET:
		case CDU_SHUTTER20_SET:
		case CDU_SHUTTER30_SET:
		case CDU_RETRACT20_SET:
		case CDU_RETRACT30_SET:
		case CDU_PRESENT20_SET:
		case CDU_PRESENT30_SET:
		case CDU_GPRESENT20_SET:
		case CDU_GPRESENT30_SET:
		case CDU_TRAY20_SET:
		case CDU_TRAY30_SET:
			strReturn = "0,0,0,0,0,0,0,0";
			break;
	}

	return strReturn;
}

// Get Number Of CST
int	CDevCmn::fnCSH_GetNumberOfCST()
{
	int		nReturn = FALSE;

	switch (AtmDefine.CashHandler)
	{
		case BRM_SET:
			nReturn = fnBRM_GetNumberOfCST();
			break;
		case CDU_SHUTTER1510_SET:
		case CDU_SHUTTER20_SET:
		case CDU_SHUTTER30_SET:
		case CDU_RETRACT20_SET:
		case CDU_RETRACT30_SET:
		case CDU_PRESENT20_SET:
		case CDU_PRESENT30_SET:
		case CDU_GPRESENT20_SET:
		case CDU_GPRESENT30_SET:
		case CDU_TRAY20_SET:
		case CDU_TRAY30_SET:
			nReturn = fnCDU_GetNumberOfCST();
			break;
	}

	return nReturn;
}

// Get CST Type(CST No)
int	CDevCmn::fnCSH_GetCSTType(int nCSTNo)
{
	int		nReturn = FALSE;

	switch (AtmDefine.CashHandler)
	{
		case BRM_SET:
			nReturn = fnBRM_GetCSTType(nCSTNo);
			break;
		case CDU_SHUTTER1510_SET:
		case CDU_SHUTTER20_SET:
		case CDU_SHUTTER30_SET:
		case CDU_RETRACT20_SET:
		case CDU_RETRACT30_SET:
		case CDU_PRESENT20_SET:
		case CDU_PRESENT30_SET:
		case CDU_GPRESENT20_SET:
		case CDU_GPRESENT30_SET:
		case CDU_TRAY20_SET:
		case CDU_TRAY30_SET:
			nReturn = CASH_TYPE_BILL;
			break;
	}

	return nReturn;
}

// Get Amount Of CST
int	CDevCmn::fnCSH_GetAmountOfCST()
{
	int		nAmount = 0;

	switch (AtmDefine.CashHandler)
	{
		case BRM_SET:											// 2008.04.10
			if ((CSTCnt >= 1) &&								// 2004.08.23
				(FstCSTLoc == NORMAL_POS) &&					// 2004.11.05
				((FstCSTStatus == CST_NORMAL) || (FstCSTStatus == CST_NEAR) || (FstCSTStatus == CST_FULL) || (FstCSTStatus == CST_HIGH)))
			{
				if (FstCSTValue == CASH_CHINA_100)
					nAmount += fnCSH_GetNumberOfCash(BRM_CST_1) * CASH_CHINA_100;
				else
				if (FstCSTValue == CASH_CHINA_50)
					nAmount += fnCSH_GetNumberOfCash(BRM_CST_1) * CASH_CHINA_50;
				else
				if (FstCSTValue == CASH_CHINA_20)
					nAmount += fnCSH_GetNumberOfCash(BRM_CST_1) * CASH_CHINA_20;
				else
				if (FstCSTValue == CASH_CHINA_10)
					nAmount += fnCSH_GetNumberOfCash(BRM_CST_1) * CASH_CHINA_10;
			}
			if ((CSTCnt >= 2) &&
				(SndCSTLoc == NORMAL_POS) &&					// 2004.11.05
				((SndCSTStatus == CST_NORMAL) || (SndCSTStatus == CST_NEAR) || (SndCSTStatus == CST_FULL) || (SndCSTStatus == CST_HIGH)))
			{
				if (SndCSTValue == CASH_CHINA_100)
					nAmount += fnCSH_GetNumberOfCash(BRM_CST_2) * CASH_CHINA_100;
				else
				if (SndCSTValue == CASH_CHINA_50)
					nAmount += fnCSH_GetNumberOfCash(BRM_CST_2) * CASH_CHINA_50;
				else
				if (SndCSTValue == CASH_CHINA_20)
					nAmount += fnCSH_GetNumberOfCash(BRM_CST_2) * CASH_CHINA_20;
				else
				if (SndCSTValue == CASH_CHINA_10)
					nAmount += fnCSH_GetNumberOfCash(BRM_CST_2) * CASH_CHINA_10;
			}
			if ((CSTCnt >= 3) &&
				(TrdCSTLoc == NORMAL_POS) &&					// 2004.11.05
				((TrdCSTStatus == CST_NORMAL) || (TrdCSTStatus == CST_NEAR) || (TrdCSTStatus == CST_FULL) || (TrdCSTStatus == CST_HIGH)))
			{
				if (TrdCSTValue == CASH_CHINA_100)
					nAmount += fnCSH_GetNumberOfCash(BRM_CST_3) * CASH_CHINA_100;
				else
				if (TrdCSTValue == CASH_CHINA_50)
					nAmount += fnCSH_GetNumberOfCash(BRM_CST_3) * CASH_CHINA_50;
				else
				if (TrdCSTValue == CASH_CHINA_20)
					nAmount += fnCSH_GetNumberOfCash(BRM_CST_3) * CASH_CHINA_20;
				else
				if (TrdCSTValue == CASH_CHINA_10)
					nAmount += fnCSH_GetNumberOfCash(BRM_CST_3) * CASH_CHINA_10;
			}
			if ((CSTCnt >= 4) &&
				(FthCSTLoc == NORMAL_POS) &&					// 2004.11.05
				((FthCSTStatus == CST_NORMAL) || (FthCSTStatus == CST_NEAR) || (FthCSTStatus == CST_FULL) || (FthCSTStatus == CST_HIGH)))
			{
				if (FthCSTValue == CASH_CHINA_100)
					nAmount += fnCSH_GetNumberOfCash(BRM_CST_4) * CASH_CHINA_100;
				else
				if (FthCSTValue == CASH_CHINA_50)
					nAmount += fnCSH_GetNumberOfCash(BRM_CST_4) * CASH_CHINA_50;
				else
				if (FthCSTValue == CASH_CHINA_20)
					nAmount += fnCSH_GetNumberOfCash(BRM_CST_4) * CASH_CHINA_20;
				else
				if (FthCSTValue == CASH_CHINA_10)
					nAmount += fnCSH_GetNumberOfCash(BRM_CST_4) * CASH_CHINA_10;
			}
			break;
		case CDU_SHUTTER1510_SET:
		case CDU_SHUTTER20_SET:
		case CDU_SHUTTER30_SET:
		case CDU_RETRACT20_SET:
		case CDU_RETRACT30_SET:
		case CDU_PRESENT20_SET:
		case CDU_PRESENT30_SET:
		case CDU_GPRESENT20_SET:
		case CDU_GPRESENT30_SET:
		case CDU_TRAY20_SET:
		case CDU_TRAY30_SET:
			if ((CSTCnt >= 1) &&								// 2004.08.23
				(FstCSTLoc == NORMAL_POS) &&					// 2004.11.05
				((FstCSTStatus == CST_NORMAL) || (FstCSTStatus == CST_NEAR) || (FstCSTStatus == CST_FULL) || (FstCSTStatus == CST_HIGH)) &&
				(fnCSH_GetNumberOfCash(CDU_CST_1) > CashEmptyCashCnt))
			{
				if (FstCSTValue == CASH_CHINA_100)
					nAmount += fnCSH_GetNumberOfCash(CDU_CST_1) * CASH_CHINA_100;
				else
				if (FstCSTValue == CASH_CHINA_50)
					nAmount += fnCSH_GetNumberOfCash(CDU_CST_1) * CASH_CHINA_50;
				else
				if (FstCSTValue == CASH_CHINA_20)
					nAmount += fnCSH_GetNumberOfCash(CDU_CST_1) * CASH_CHINA_20;
				else
				if (FstCSTValue == CASH_CHINA_10)
					nAmount += fnCSH_GetNumberOfCash(CDU_CST_1) * CASH_CHINA_10;
			}
			if ((CSTCnt >= 2) &&
				(SndCSTLoc == NORMAL_POS) &&					// 2004.11.05
				((SndCSTStatus == CST_NORMAL) || (SndCSTStatus == CST_NEAR) || (SndCSTStatus == CST_FULL) || (SndCSTStatus == CST_HIGH)) &&
				(fnCSH_GetNumberOfCash(CDU_CST_2) > CashEmptyCashCnt))
			{
				if (SndCSTValue == CASH_CHINA_100)
					nAmount += fnCSH_GetNumberOfCash(CDU_CST_2) * CASH_CHINA_100;
				else
				if (SndCSTValue == CASH_CHINA_50)
					nAmount += fnCSH_GetNumberOfCash(CDU_CST_2) * CASH_CHINA_50;
				else
				if (SndCSTValue == CASH_CHINA_20)
					nAmount += fnCSH_GetNumberOfCash(CDU_CST_2) * CASH_CHINA_20;
				else
				if (SndCSTValue == CASH_CHINA_10)
					nAmount += fnCSH_GetNumberOfCash(CDU_CST_2) * CASH_CHINA_10;
			}
			if ((CSTCnt >= 3) &&
				(TrdCSTLoc == NORMAL_POS) &&					// 2004.11.05
				((TrdCSTStatus == CST_NORMAL) || (TrdCSTStatus == CST_NEAR) || (TrdCSTStatus == CST_FULL) || (TrdCSTStatus == CST_HIGH)) &&
				(fnCSH_GetNumberOfCash(CDU_CST_3) > CashEmptyCashCnt))
			{
				if (TrdCSTValue == CASH_CHINA_100)
					nAmount += fnCSH_GetNumberOfCash(CDU_CST_3) * CASH_CHINA_100;
				else
				if (TrdCSTValue == CASH_CHINA_50)
					nAmount += fnCSH_GetNumberOfCash(CDU_CST_3) * CASH_CHINA_50;
				else
				if (TrdCSTValue == CASH_CHINA_20)
					nAmount += fnCSH_GetNumberOfCash(CDU_CST_3) * CASH_CHINA_20;
				else
				if (TrdCSTValue == CASH_CHINA_10)
					nAmount += fnCSH_GetNumberOfCash(CDU_CST_3) * CASH_CHINA_10;
			}
			if ((CSTCnt >= 4) &&
				(FthCSTLoc == NORMAL_POS) &&					// 2004.11.05
				((FthCSTStatus == CST_NORMAL) || (FthCSTStatus == CST_NEAR) || (FthCSTStatus == CST_FULL) || (FthCSTStatus == CST_HIGH)) &&
				(fnCSH_GetNumberOfCash(CDU_CST_4) > CashEmptyCashCnt))
			{
				if (FthCSTValue == CASH_CHINA_100)
					nAmount += fnCSH_GetNumberOfCash(CDU_CST_4) * CASH_CHINA_100;
				else
				if (FthCSTValue == CASH_CHINA_50)
					nAmount += fnCSH_GetNumberOfCash(CDU_CST_4) * CASH_CHINA_50;
				else
				if (FthCSTValue == CASH_CHINA_20)
					nAmount += fnCSH_GetNumberOfCash(CDU_CST_4) * CASH_CHINA_20;
				else
				if (FthCSTValue == CASH_CHINA_10)
					nAmount += fnCSH_GetNumberOfCash(CDU_CST_4) * CASH_CHINA_10;
			}
			break;
	}

	return nAmount;
}

// Get CST Status(CST No)
int	CDevCmn::fnCSH_GetCSTStatus(int nCSTNo)
{
	int		nReturn = FALSE;

	switch (AtmDefine.CashHandler)
	{
		case BRM_SET:
			nReturn = fnBRM_GetCSTStatus(nCSTNo);
			break;
		case CDU_SHUTTER1510_SET:
		case CDU_SHUTTER20_SET:
		case CDU_SHUTTER30_SET:
		case CDU_RETRACT20_SET:
		case CDU_RETRACT30_SET:
		case CDU_PRESENT20_SET:
		case CDU_PRESENT30_SET:
		case CDU_GPRESENT20_SET:
		case CDU_GPRESENT30_SET:
		case CDU_TRAY20_SET:
		case CDU_TRAY30_SET:
			nReturn = fnCDU_GetCSTStatus(nCSTNo);
			break;
	}

	return nReturn;
}

// Get Value Of Cash(CST No)
int	CDevCmn::fnCSH_GetValueOfCash(int nCSTNo)
{
	int		nReturn = FALSE;

	switch (AtmDefine.CashHandler)
	{
		case BRM_SET:
			nReturn = fnBRM_GetValueOfCash(nCSTNo);
			break;
		case CDU_SHUTTER1510_SET:
		case CDU_SHUTTER20_SET:
		case CDU_SHUTTER30_SET:
		case CDU_RETRACT20_SET:
		case CDU_RETRACT30_SET:
		case CDU_PRESENT20_SET:
		case CDU_PRESENT30_SET:
		case CDU_GPRESENT20_SET:
		case CDU_GPRESENT30_SET:
		case CDU_TRAY20_SET:
		case CDU_TRAY30_SET:
			nReturn = fnCDU_GetValueOfCash(nCSTNo);
			break;
	}

	return nReturn;
}

// Get Number Of Cash(CST No)
int	CDevCmn::fnCSH_GetNumberOfCash(int nCSTNo)
{
	int		nReturn = FALSE;

	switch (AtmDefine.CashHandler)
	{
		case BRM_SET:
			if (nCSTNo == BRM_CST_REJECT)
				nReturn = fnBRM_GetNumberOfCash(nCSTNo);
			else
			if (CSTCnt >= nCSTNo)
			{
				nReturn = fnBRM_GetNumberOfCash(nCSTNo);		// 2004.11.05
			}
			break;
		case CDU_SHUTTER1510_SET:
		case CDU_SHUTTER20_SET:
		case CDU_SHUTTER30_SET:
		case CDU_RETRACT20_SET:
		case CDU_RETRACT30_SET:
		case CDU_PRESENT20_SET:
		case CDU_PRESENT30_SET:
		case CDU_GPRESENT20_SET:
		case CDU_GPRESENT30_SET:
		case CDU_TRAY20_SET:
		case CDU_TRAY30_SET:
			if (nCSTNo == CDU_CST_REJECT)
				nReturn = fnCDU_GetNumberOfCash(nCSTNo);
			else
			if (CSTCnt >= nCSTNo)
			{
				nReturn = fnCDU_GetNumberOfCash(nCSTNo);		// 2004.11.05
			}
			break;
	}

	return nReturn;
}

// Get Amount Of Cash(CST No)
int	CDevCmn::fnCSH_GetAmountOfCash(int nCSTNo)
{
	int		nAmount = 0;

	switch (AtmDefine.CashHandler)
	{
		case BRM_SET:											// 2008.04.10
			if (CSTCnt >= nCSTNo)
			{
				if ((nCSTNo == BRM_CST_1) &&					// 2004.08.23
					(FstCSTLoc == NORMAL_POS) &&				// 2004.11.05
					((FstCSTStatus == CST_NORMAL) || (FstCSTStatus == CST_NEAR) || (FstCSTStatus == CST_FULL) || (FstCSTStatus == CST_HIGH)))
				{
					if (FstCSTValue == CASH_CHINA_100)
						nAmount += fnCSH_GetNumberOfCash(nCSTNo) * CASH_CHINA_100;
					else
					if (FstCSTValue == CASH_CHINA_50)
						nAmount += fnCSH_GetNumberOfCash(nCSTNo) * CASH_CHINA_50;
					else
					if (FstCSTValue == CASH_CHINA_20)
						nAmount += fnCSH_GetNumberOfCash(nCSTNo) * CASH_CHINA_20;
					else
					if (FstCSTValue == CASH_CHINA_10)
						nAmount += fnCSH_GetNumberOfCash(nCSTNo) * CASH_CHINA_10;
				}
				if ((nCSTNo == BRM_CST_2) &&
					(SndCSTLoc == NORMAL_POS) &&				// 2004.11.05
					((SndCSTStatus == CST_NORMAL) || (SndCSTStatus == CST_NEAR) || (SndCSTStatus == CST_FULL) || (SndCSTStatus == CST_HIGH)))
				{
					if (SndCSTValue == CASH_CHINA_100)
						nAmount += fnCSH_GetNumberOfCash(nCSTNo) * CASH_CHINA_100;
					else
					if (SndCSTValue == CASH_CHINA_50)
						nAmount += fnCSH_GetNumberOfCash(nCSTNo) * CASH_CHINA_50;
					else
					if (SndCSTValue == CASH_CHINA_20)
						nAmount += fnCSH_GetNumberOfCash(nCSTNo) * CASH_CHINA_20;
					else
					if (SndCSTValue == CASH_CHINA_10)
						nAmount += fnCSH_GetNumberOfCash(nCSTNo) * CASH_CHINA_10;
				}
				if ((nCSTNo == BRM_CST_3) &&
					(TrdCSTLoc == NORMAL_POS) &&				// 2004.11.05
					((TrdCSTStatus == CST_NORMAL) || (TrdCSTStatus == CST_NEAR) || (TrdCSTStatus == CST_FULL) || (TrdCSTStatus == CST_HIGH)))
				{
					if (TrdCSTValue == CASH_CHINA_100)
						nAmount += fnCSH_GetNumberOfCash(nCSTNo) * CASH_CHINA_100;
					else
					if (TrdCSTValue == CASH_CHINA_50)
						nAmount += fnCSH_GetNumberOfCash(nCSTNo) * CASH_CHINA_50;
					else
					if (TrdCSTValue == CASH_CHINA_20)
						nAmount += fnCSH_GetNumberOfCash(nCSTNo) * CASH_CHINA_20;
					else
					if (TrdCSTValue == CASH_CHINA_10)
						nAmount += fnCSH_GetNumberOfCash(nCSTNo) * CASH_CHINA_10;
				}
				if ((nCSTNo == BRM_CST_4) &&
					(FthCSTLoc == NORMAL_POS) &&				// 2004.11.05
					((FthCSTStatus == CST_NORMAL) || (FthCSTStatus == CST_NEAR) || (FthCSTStatus == CST_FULL) || (FthCSTStatus == CST_HIGH)))
				{
					if (FthCSTValue == CASH_CHINA_100)
						nAmount += fnCSH_GetNumberOfCash(nCSTNo) * CASH_CHINA_100;
					else
					if (FthCSTValue == CASH_CHINA_50)
						nAmount += fnCSH_GetNumberOfCash(nCSTNo) * CASH_CHINA_50;
					else
					if (FthCSTValue == CASH_CHINA_20)
						nAmount += fnCSH_GetNumberOfCash(nCSTNo) * CASH_CHINA_20;
					else
					if (FthCSTValue == CASH_CHINA_10)
						nAmount += fnCSH_GetNumberOfCash(nCSTNo) * CASH_CHINA_10;
				}
			}
			break;
		case CDU_SHUTTER1510_SET:
		case CDU_SHUTTER20_SET:
		case CDU_SHUTTER30_SET:
		case CDU_RETRACT20_SET:
		case CDU_RETRACT30_SET:
		case CDU_PRESENT20_SET:
		case CDU_PRESENT30_SET:
		case CDU_GPRESENT20_SET:
		case CDU_GPRESENT30_SET:
		case CDU_TRAY20_SET:
		case CDU_TRAY30_SET:
			if (CSTCnt >= nCSTNo)
			{
				if ((nCSTNo == CDU_CST_1) &&					// 2004.08.23
					(FstCSTLoc == NORMAL_POS) &&				// 2004.11.05
					((FstCSTStatus == CST_NORMAL) || (FstCSTStatus == CST_NEAR) || (FstCSTStatus == CST_FULL) || (FstCSTStatus == CST_HIGH)) &&
					(fnCSH_GetNumberOfCash(nCSTNo) > CashEmptyCashCnt))
				{
					if (FstCSTValue == CASH_CHINA_100)
						nAmount += fnCSH_GetNumberOfCash(nCSTNo) * CASH_CHINA_100;
					else
					if (FstCSTValue == CASH_CHINA_50)
						nAmount += fnCSH_GetNumberOfCash(nCSTNo) * CASH_CHINA_50;
					else
					if (FstCSTValue == CASH_CHINA_20)
						nAmount += fnCSH_GetNumberOfCash(nCSTNo) * CASH_CHINA_20;
					else
					if (FstCSTValue == CASH_CHINA_10)
						nAmount += fnCSH_GetNumberOfCash(nCSTNo) * CASH_CHINA_10;
				}
				if ((nCSTNo == CDU_CST_2) &&
					(SndCSTLoc == NORMAL_POS) &&				// 2004.11.05
					((SndCSTStatus == CST_NORMAL) || (SndCSTStatus == CST_NEAR) || (SndCSTStatus == CST_FULL) || (SndCSTStatus == CST_HIGH)) &&
					(fnCSH_GetNumberOfCash(nCSTNo) > CashEmptyCashCnt))
				{
					if (SndCSTValue == CASH_CHINA_100)
						nAmount += fnCSH_GetNumberOfCash(nCSTNo) * CASH_CHINA_100;
					else
					if (SndCSTValue == CASH_CHINA_50)
						nAmount += fnCSH_GetNumberOfCash(nCSTNo) * CASH_CHINA_50;
					else
					if (SndCSTValue == CASH_CHINA_20)
						nAmount += fnCSH_GetNumberOfCash(nCSTNo) * CASH_CHINA_20;
					else
					if (SndCSTValue == CASH_CHINA_10)
						nAmount += fnCSH_GetNumberOfCash(nCSTNo) * CASH_CHINA_10;
				}
				if ((nCSTNo == CDU_CST_3) &&
					(TrdCSTLoc == NORMAL_POS) &&				// 2004.11.05
					((TrdCSTStatus == CST_NORMAL) || (TrdCSTStatus == CST_NEAR) || (TrdCSTStatus == CST_FULL) || (TrdCSTStatus == CST_HIGH)) &&
					(fnCSH_GetNumberOfCash(nCSTNo) > CashEmptyCashCnt))
				{
					if (TrdCSTValue == CASH_CHINA_100)
						nAmount += fnCSH_GetNumberOfCash(nCSTNo) * CASH_CHINA_100;
					else
					if (TrdCSTValue == CASH_CHINA_50)
						nAmount += fnCSH_GetNumberOfCash(nCSTNo) * CASH_CHINA_50;
					else
					if (TrdCSTValue == CASH_CHINA_20)
						nAmount += fnCSH_GetNumberOfCash(nCSTNo) * CASH_CHINA_20;
					else
					if (TrdCSTValue == CASH_CHINA_10)
						nAmount += fnCSH_GetNumberOfCash(nCSTNo) * CASH_CHINA_10;
				}
				if ((nCSTNo == CDU_CST_4) &&
					(FthCSTLoc == NORMAL_POS) &&				// 2004.11.05
					((FthCSTStatus == CST_NORMAL) || (FthCSTStatus == CST_NEAR) || (FthCSTStatus == CST_FULL) || (FthCSTStatus == CST_HIGH)) &&
					(fnCSH_GetNumberOfCash(nCSTNo) > CashEmptyCashCnt))
				{
					if (FthCSTValue == CASH_CHINA_100)
						nAmount += fnCSH_GetNumberOfCash(nCSTNo) * CASH_CHINA_100;
					else
					if (FthCSTValue == CASH_CHINA_50)
						nAmount += fnCSH_GetNumberOfCash(nCSTNo) * CASH_CHINA_50;
					else
					if (FthCSTValue == CASH_CHINA_20)
						nAmount += fnCSH_GetNumberOfCash(nCSTNo) * CASH_CHINA_20;
					else
					if (FthCSTValue == CASH_CHINA_10)
						nAmount += fnCSH_GetNumberOfCash(nCSTNo) * CASH_CHINA_10;
				}
			}
			break;
	}

	return nAmount;
}

// Get Number Of Set Cash(CST No)
int	CDevCmn::fnCSH_GetNumberOfSetCash(int nCSTNo)
{
	int		nReturn = FALSE;

	switch (AtmDefine.CashHandler)
	{
		case BRM_SET:
			if (nCSTNo == BRM_CST_REJECT)
				nReturn = fnBRM_GetNumberOfSetCash(nCSTNo);
			else
			if (CSTCnt >= nCSTNo)
			{
				nReturn = fnBRM_GetNumberOfSetCash(nCSTNo);
			}
			break;
		case CDU_SHUTTER1510_SET:
		case CDU_SHUTTER20_SET:
		case CDU_SHUTTER30_SET:
		case CDU_RETRACT20_SET:
		case CDU_RETRACT30_SET:
		case CDU_PRESENT20_SET:
		case CDU_PRESENT30_SET:
		case CDU_GPRESENT20_SET:
		case CDU_GPRESENT30_SET:
		case CDU_TRAY20_SET:
		case CDU_TRAY30_SET:
			if (nCSTNo == CDU_CST_REJECT)
				nReturn = fnCDU_GetNumberOfSetCash(nCSTNo);
			else
			if (CSTCnt >= nCSTNo)
			{
				nReturn = fnCDU_GetNumberOfSetCash(nCSTNo);		// 2004.11.05
			}
			break;
	}

	return nReturn;
}

// Get Dispense Of CST(Cash Amount)
CString CDevCmn::fstrCSH_GetDispenseOfCST(int nCashAmt)
{
	CString strReturn("");

	switch (AtmDefine.CashHandler)
	{
		case BRM_SET:
			strReturn = fstrBRM_GetDispenseOfCST(nCashAmt);
			break;
		case CDU_SHUTTER1510_SET:
		case CDU_SHUTTER20_SET:
		case CDU_SHUTTER30_SET:
		case CDU_RETRACT20_SET:
		case CDU_RETRACT30_SET:
		case CDU_PRESENT20_SET:
		case CDU_PRESENT30_SET:
		case CDU_GPRESENT20_SET:
		case CDU_GPRESENT30_SET:
		case CDU_TRAY20_SET:
		case CDU_TRAY30_SET:
			strReturn = fstrCDU_GetDispenseOfCST(nCashAmt);
			break;
	}

	return strReturn;
}

// Get Number Of Dispense(CST No)
int	CDevCmn::fnCSH_GetNumberOfDispense(int nCSTNo)
{
	int		nReturn = FALSE;

	switch (AtmDefine.CashHandler)
	{
		case BRM_SET:
			nReturn = fnCSH_GetNumberOfSetCash(nCSTNo) - fnCSH_GetNumberOfCash(nCSTNo);
			break;
		case CDU_SHUTTER1510_SET:
		case CDU_SHUTTER20_SET:
		case CDU_SHUTTER30_SET:
		case CDU_RETRACT20_SET:
		case CDU_RETRACT30_SET:
		case CDU_PRESENT20_SET:
		case CDU_PRESENT30_SET:
		case CDU_GPRESENT20_SET:
		case CDU_GPRESENT30_SET:
		case CDU_TRAY20_SET:
		case CDU_TRAY30_SET:
			nReturn = fnCDU_GetNumberOfDispense(nCSTNo);
			break;
	}

	return nReturn;
}

// Get Number Of Reject(CST No)
int	CDevCmn::fnCSH_GetNumberOfReject(int nCSTNo)
{
	int		nReturn = FALSE;

	switch (AtmDefine.CashHandler)
	{
		case BRM_SET:
			nReturn = fnBRM_GetNumberOfReject(nCSTNo);
			break;
		case CDU_SHUTTER1510_SET:
		case CDU_SHUTTER20_SET:
		case CDU_SHUTTER30_SET:
		case CDU_RETRACT20_SET:
		case CDU_RETRACT30_SET:
		case CDU_PRESENT20_SET:
		case CDU_PRESENT30_SET:
		case CDU_GPRESENT20_SET:
		case CDU_GPRESENT30_SET:
		case CDU_TRAY20_SET:
		case CDU_TRAY30_SET:
			nReturn = fnCDU_GetNumberOfReject(nCSTNo);
			break;
	}

	return nReturn;
}

// Get Number Of Retract()
CString	CDevCmn::fstrCSH_GetNumberOfRetract()
{
	CString strReturn("");

	switch (AtmDefine.CashHandler)
	{
		case BRM_SET:
			strReturn = fstrBRM_GetNumberOfRetract();
			break;
		case CDU_SHUTTER1510_SET:
		case CDU_SHUTTER20_SET:
		case CDU_SHUTTER30_SET:
		case CDU_RETRACT20_SET:
		case CDU_RETRACT30_SET:
		case CDU_PRESENT20_SET:
		case CDU_PRESENT30_SET:
		case CDU_GPRESENT20_SET:
		case CDU_GPRESENT30_SET:
		case CDU_TRAY20_SET:
		case CDU_TRAY30_SET:
			strReturn = "0,0,0,0,0,0";
			break;
	}

	return strReturn;
}

// Get Number Of Deposit(Cash Type)
CString	CDevCmn::fstrCSH_GetNumberOfDeposit(int nCashType)
{
	CString strReturn("");

	switch (AtmDefine.CashHandler)
	{
		case BRM_SET:
			strReturn = fstrBRM_GetNumberOfDeposit(nCashType);
			break;
		case CDU_SHUTTER1510_SET:
		case CDU_SHUTTER20_SET:
		case CDU_SHUTTER30_SET:
		case CDU_RETRACT20_SET:
		case CDU_RETRACT30_SET:
		case CDU_PRESENT20_SET:
		case CDU_PRESENT30_SET:
		case CDU_GPRESENT20_SET:
		case CDU_GPRESENT30_SET:
		case CDU_TRAY20_SET:
		case CDU_TRAY30_SET:
			strReturn = "0,0,0,0,0,0";
			break;
	}

	return strReturn;
}

// Get Number Of Total Set Cash(CST No)
int	CDevCmn::fnCSH_GetNumberOfTotalSetCash(int nCSTNo)
{
	int		nReturn = FALSE;

	switch (AtmDefine.CashHandler)
	{
		case BRM_SET:
			if (nCSTNo == BRM_CST_REJECT)
				nReturn = 0;
			else
			if (CSTCnt >= nCSTNo)								// 2004.08.23
			{
				if (nCSTNo == BRM_CST_1)
					nReturn = RegGetInt(_REGKEY_DEVICE, _REGKEY_DEVICE_TOTALSETCASHCST1);
				if (nCSTNo == BRM_CST_2)
					nReturn = RegGetInt(_REGKEY_DEVICE, _REGKEY_DEVICE_TOTALSETCASHCST2);
				if (nCSTNo == BRM_CST_3)
					nReturn = RegGetInt(_REGKEY_DEVICE, _REGKEY_DEVICE_TOTALSETCASHCST3);
				if (nCSTNo == BRM_CST_4)
					nReturn = RegGetInt(_REGKEY_DEVICE, _REGKEY_DEVICE_TOTALSETCASHCST4);
			}
			break;
		case CDU_SHUTTER1510_SET:
		case CDU_SHUTTER20_SET:
		case CDU_SHUTTER30_SET:
		case CDU_RETRACT20_SET:
		case CDU_RETRACT30_SET:
		case CDU_PRESENT20_SET:
		case CDU_PRESENT30_SET:
		case CDU_GPRESENT20_SET:
		case CDU_GPRESENT30_SET:
		case CDU_TRAY20_SET:
		case CDU_TRAY30_SET:
			if (nCSTNo == CDU_CST_REJECT)
				nReturn = 0;
			else
			if (CSTCnt >= nCSTNo)								// 2004.08.23
			{
				if (nCSTNo == CDU_CST_1)
					nReturn = RegGetInt(_REGKEY_DEVICE, _REGKEY_DEVICE_TOTALSETCASHCST1);
				if (nCSTNo == CDU_CST_2)
					nReturn = RegGetInt(_REGKEY_DEVICE, _REGKEY_DEVICE_TOTALSETCASHCST2);
				if (nCSTNo == CDU_CST_3)
					nReturn = RegGetInt(_REGKEY_DEVICE, _REGKEY_DEVICE_TOTALSETCASHCST3);
				if (nCSTNo == CDU_CST_4)
					nReturn = RegGetInt(_REGKEY_DEVICE, _REGKEY_DEVICE_TOTALSETCASHCST4);
			}
			break;
	}

	static	int		nSaveReturn0 = FALSE;
	static	int		nSaveReturn1 = FALSE;
	static	int		nSaveReturn2 = FALSE;
	static	int		nSaveReturn3 = FALSE;
	static	int		nSaveReturn4 = FALSE;
	
	switch (nCSTNo)
	{
		case CDU_CST_REJECT:
if (nReturn != nSaveReturn0)
{
	nSaveReturn0 = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCSH_GetNumberOfTotalSetCash(nCSTNo[%d]):return(%d)", nCSTNo, nReturn);
}
			break;
		case CDU_CST_1:
if (nReturn != nSaveReturn1)
{
	nSaveReturn1 = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCSH_GetNumberOfTotalSetCash(nCSTNo[%d]):return(%d)", nCSTNo, nReturn);
}
			break;
		case CDU_CST_2:
if (nReturn != nSaveReturn2)
{
	nSaveReturn2 = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCSH_GetNumberOfTotalSetCash(nCSTNo[%d]):return(%d)", nCSTNo, nReturn);
}
			break;
		case CDU_CST_3:
if (nReturn != nSaveReturn3)
{
	nSaveReturn3 = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCSH_GetNumberOfTotalSetCash(nCSTNo[%d]):return(%d)", nCSTNo, nReturn);
}
			break;
		case CDU_CST_4:
if (nReturn != nSaveReturn4)
{
	nSaveReturn4 = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCSH_GetNumberOfTotalSetCash(nCSTNo[%d]):return(%d)", nCSTNo, nReturn);
}
			break;
	}

	return nReturn;
}

// Get Number Of Dispense Cash(CST No)
int	CDevCmn::fnCSH_GetNumberOfDispenseCash(int nCSTNo)
{
	int		nReturn = FALSE;

	switch (AtmDefine.CashHandler)
	{
		case BRM_SET:
			if (nCSTNo == BRM_CST_REJECT)
				nReturn = 0;
			else
			if (CSTCnt >= nCSTNo)								// 2004.08.23
			{
				if (nCSTNo == BRM_CST_1)
					nReturn = RegGetInt(_REGKEY_DEVICE, _REGKEY_DEVICE_DISPCASHCST1);
				if (nCSTNo == BRM_CST_2)
					nReturn = RegGetInt(_REGKEY_DEVICE, _REGKEY_DEVICE_DISPCASHCST2);
				if (nCSTNo == BRM_CST_3)
					nReturn = RegGetInt(_REGKEY_DEVICE, _REGKEY_DEVICE_DISPCASHCST3);
				if (nCSTNo == BRM_CST_4)
					nReturn = RegGetInt(_REGKEY_DEVICE, _REGKEY_DEVICE_DISPCASHCST4);
			}
			break;
		case CDU_SHUTTER1510_SET:
		case CDU_SHUTTER20_SET:
		case CDU_SHUTTER30_SET:
		case CDU_RETRACT20_SET:
		case CDU_RETRACT30_SET:
		case CDU_PRESENT20_SET:
		case CDU_PRESENT30_SET:
		case CDU_GPRESENT20_SET:
		case CDU_GPRESENT30_SET:
		case CDU_TRAY20_SET:
		case CDU_TRAY30_SET:
			if (nCSTNo == CDU_CST_REJECT)
				nReturn = 0;
			else
			if (CSTCnt >= nCSTNo)								// 2004.08.23
			{
				if (nCSTNo == CDU_CST_1)
					nReturn = RegGetInt(_REGKEY_DEVICE, _REGKEY_DEVICE_DISPCASHCST1);
				if (nCSTNo == CDU_CST_2)
					nReturn = RegGetInt(_REGKEY_DEVICE, _REGKEY_DEVICE_DISPCASHCST2);
				if (nCSTNo == CDU_CST_3)
					nReturn = RegGetInt(_REGKEY_DEVICE, _REGKEY_DEVICE_DISPCASHCST3);
				if (nCSTNo == CDU_CST_4)
					nReturn = RegGetInt(_REGKEY_DEVICE, _REGKEY_DEVICE_DISPCASHCST4);
			}
			break;
	}

	static	int		nSaveReturn0 = FALSE;
	static	int		nSaveReturn1 = FALSE;
	static	int		nSaveReturn2 = FALSE;
	static	int		nSaveReturn3 = FALSE;
	static	int		nSaveReturn4 = FALSE;

	switch (nCSTNo)
	{
		case CDU_CST_REJECT:
if (nReturn != nSaveReturn0)
{
	nSaveReturn0 = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCSH_GetNumberOfDispenseCash(nCSTNo[%d]):return(%d)", nCSTNo, nReturn);
}
			break;
		case CDU_CST_1:
if (nReturn != nSaveReturn1)
{
	nSaveReturn1 = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCSH_GetNumberOfDispenseCash(nCSTNo[%d]):return(%d)", nCSTNo, nReturn);
}
			break;
		case CDU_CST_2:
if (nReturn != nSaveReturn2)
{
	nSaveReturn2 = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCSH_GetNumberOfDispenseCash(nCSTNo[%d]):return(%d)", nCSTNo, nReturn);
}
			break;
		case CDU_CST_3:
if (nReturn != nSaveReturn3)
{
	nSaveReturn3 = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCSH_GetNumberOfDispenseCash(nCSTNo[%d]):return(%d)", nCSTNo, nReturn);
}
			break;
		case CDU_CST_4:
if (nReturn != nSaveReturn4)
{
	nSaveReturn4 = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCSH_GetNumberOfDispenseCash(nCSTNo[%d]):return(%d)", nCSTNo, nReturn);
}
			break;
	}

	return nReturn;
}

/////////////////////////////////////////////////////////////////////////////
//	CASH FUNCTION(CASH) : CASH OPERATION MODE
/////////////////////////////////////////////////////////////////////////////
// Initialize
int	CDevCmn::fnCSH_Initialize()
{
	int		nReturn = FALSE;

	switch (AtmDefine.CashHandler)
	{
		case BRM_SET:
			nReturn = fnBRM_Initialize();
			break;
		case CDU_SHUTTER1510_SET:
		case CDU_SHUTTER20_SET:
		case CDU_SHUTTER30_SET:
		case CDU_RETRACT20_SET:
		case CDU_RETRACT30_SET:
		case CDU_PRESENT20_SET:
		case CDU_PRESENT30_SET:
		case CDU_GPRESENT20_SET:
		case CDU_GPRESENT30_SET:
		case CDU_TRAY20_SET:
		case CDU_TRAY30_SET:
			nReturn = fnCDU_Initialize();
			break;
	}

	return nReturn;
}

// Deinitialize
int	CDevCmn::fnCSH_Deinitialize()
{
	int		nReturn = FALSE;

	switch (AtmDefine.CashHandler)
	{
		case BRM_SET:
			nReturn = fnBRM_Deinitialize();
			break;
		case CDU_SHUTTER1510_SET:
		case CDU_SHUTTER20_SET:
		case CDU_SHUTTER30_SET:
		case CDU_RETRACT20_SET:
		case CDU_RETRACT30_SET:
		case CDU_PRESENT20_SET:
		case CDU_PRESENT30_SET:
		case CDU_GPRESENT20_SET:
		case CDU_GPRESENT30_SET:
		case CDU_TRAY20_SET:
		case CDU_TRAY30_SET:
			nReturn = fnCDU_Deinitialize();
			break;
	}

	return nReturn;
}

// Open Shutter
int	CDevCmn::fnCSH_OpenShutter()
{
	int		nReturn = FALSE;

	switch (AtmDefine.CashHandler)
	{
		case BRM_SET:
			nReturn = fnBRM_OpenShutter();
			break;
		case CDU_SHUTTER1510_SET:
		case CDU_SHUTTER20_SET:
		case CDU_SHUTTER30_SET:
		case CDU_RETRACT20_SET:
		case CDU_RETRACT30_SET:
		case CDU_PRESENT20_SET:
		case CDU_PRESENT30_SET:
		case CDU_GPRESENT20_SET:
		case CDU_GPRESENT30_SET:
		case CDU_TRAY20_SET:
		case CDU_TRAY30_SET:
			nReturn = fnCDU_OpenShutter();
			break;
	}

	return nReturn;
}

// Close Shutter
int	CDevCmn::fnCSH_CloseShutter()
{
	int		nReturn = FALSE;

	switch (AtmDefine.CashHandler)
	{
		case BRM_SET:
			nReturn = fnBRM_CloseShutter();
			break;
		case CDU_SHUTTER1510_SET:
		case CDU_SHUTTER20_SET:
		case CDU_SHUTTER30_SET:
		case CDU_RETRACT20_SET:
		case CDU_RETRACT30_SET:
		case CDU_PRESENT20_SET:
		case CDU_PRESENT30_SET:
		case CDU_GPRESENT20_SET:
		case CDU_GPRESENT30_SET:
		case CDU_TRAY20_SET:
		case CDU_TRAY30_SET:
			nReturn = fnCDU_CloseShutter();
			break;
	}

	return nReturn;
}

// Ready
int	CDevCmn::fnCSH_Ready()
{
	int		nReturn = FALSE;

	switch (AtmDefine.CashHandler)
	{
		case BRM_SET:
			nReturn = fnBRM_Ready();
			break;
		case CDU_SHUTTER1510_SET:
		case CDU_SHUTTER20_SET:
		case CDU_SHUTTER30_SET:
		case CDU_RETRACT20_SET:
		case CDU_RETRACT30_SET:
		case CDU_PRESENT20_SET:
		case CDU_PRESENT30_SET:
		case CDU_GPRESENT20_SET:
		case CDU_GPRESENT30_SET:
		case CDU_TRAY20_SET:
		case CDU_TRAY30_SET:
			break;
	}

	return nReturn;
}

// Accept(Wait Time)
int	CDevCmn::fnCSH_Accept(int nWaitSec)
{
	int		nReturn = FALSE;

	switch (AtmDefine.CashHandler)
	{
		case BRM_SET:
			nReturn = fnBRM_Accept(nWaitSec);
			break;
		case CDU_SHUTTER1510_SET:
		case CDU_SHUTTER20_SET:
		case CDU_SHUTTER30_SET:
		case CDU_RETRACT20_SET:
		case CDU_RETRACT30_SET:
		case CDU_PRESENT20_SET:
		case CDU_PRESENT30_SET:
		case CDU_GPRESENT20_SET:
		case CDU_GPRESENT30_SET:
		case CDU_TRAY20_SET:
		case CDU_TRAY30_SET:
			break;
	}

	return nReturn;
}

// Cancel Accept 
int	CDevCmn::fnCSH_CancelAcceptCash()
{
	int		nReturn = FALSE;

	switch (AtmDefine.CashHandler)
	{
		case BRM_SET:
			nReturn = fnBRM_CancelAcceptCash();
			break;
		case CDU_SHUTTER1510_SET:
		case CDU_SHUTTER20_SET:
		case CDU_SHUTTER30_SET:
		case CDU_RETRACT20_SET:
		case CDU_RETRACT30_SET:
		case CDU_PRESENT20_SET:
		case CDU_PRESENT30_SET:
		case CDU_GPRESENT20_SET:
		case CDU_GPRESENT30_SET:
		case CDU_TRAY20_SET:
		case CDU_TRAY30_SET:
			break;
	}

	return nReturn;
}

// Ready Count
int	CDevCmn::fnCSH_ReadyCount()
{
	int		nReturn = FALSE;

	switch (AtmDefine.CashHandler)
	{
		case BRM_SET:
			nReturn = fnBRM_ReadyCount();
			break;
		case CDU_SHUTTER1510_SET:
		case CDU_SHUTTER20_SET:
		case CDU_SHUTTER30_SET:
		case CDU_RETRACT20_SET:
		case CDU_RETRACT30_SET:
		case CDU_PRESENT20_SET:
		case CDU_PRESENT30_SET:
		case CDU_GPRESENT20_SET:
		case CDU_GPRESENT30_SET:
		case CDU_TRAY20_SET:
		case CDU_TRAY30_SET:
			break;
	}

	return nReturn;
}

// Count
int	CDevCmn::fnCSH_Count()
{
	int		nReturn = FALSE;

	switch (AtmDefine.CashHandler)
	{
		case BRM_SET:
			nReturn = fnBRM_Count();
			break;
		case CDU_SHUTTER1510_SET:
		case CDU_SHUTTER20_SET:
		case CDU_SHUTTER30_SET:
		case CDU_RETRACT20_SET:
		case CDU_RETRACT30_SET:
		case CDU_PRESENT20_SET:
		case CDU_PRESENT30_SET:
		case CDU_GPRESENT20_SET:
		case CDU_GPRESENT30_SET:
		case CDU_TRAY20_SET:
		case CDU_TRAY30_SET:
			break;
	}

	return nReturn;
}

// Stack
int	CDevCmn::fnCSH_Stack()
{
	int		nReturn = FALSE;

	switch (AtmDefine.CashHandler)
	{
		case BRM_SET:
			nReturn = fnBRM_Stack();
			break;
		case CDU_SHUTTER1510_SET:
		case CDU_SHUTTER20_SET:
		case CDU_SHUTTER30_SET:
		case CDU_RETRACT20_SET:
		case CDU_RETRACT30_SET:
		case CDU_PRESENT20_SET:
		case CDU_PRESENT30_SET:
		case CDU_GPRESENT20_SET:
		case CDU_GPRESENT30_SET:
		case CDU_TRAY20_SET:
		case CDU_TRAY30_SET:
			break;
	}

	return nReturn;
}

// Reject(Wait Time):Reject All
int	CDevCmn::fnCSH_Reject(int nWaitSec)
{
	int		nReturn = FALSE;

	switch (AtmDefine.CashHandler)
	{
		case BRM_SET:
			nReturn = fnBRM_Reject(nWaitSec);
			break;
		case CDU_SHUTTER1510_SET:
		case CDU_SHUTTER20_SET:
		case CDU_SHUTTER30_SET:
		case CDU_RETRACT20_SET:
		case CDU_RETRACT30_SET:
		case CDU_PRESENT20_SET:
		case CDU_PRESENT30_SET:
		case CDU_GPRESENT20_SET:
		case CDU_GPRESENT30_SET:
		case CDU_TRAY20_SET:
		case CDU_TRAY30_SET:
			break;
	}

	return nReturn;
}

// Wait Lift
int	CDevCmn::fnCSH_WaitLift()
{
	int		nReturn = FALSE;

	switch (AtmDefine.CashHandler)
	{
		case BRM_SET:
			nReturn = fnBRM_WaitLift();
			break;
		case CDU_SHUTTER1510_SET:
		case CDU_SHUTTER20_SET:
		case CDU_SHUTTER30_SET:
		case CDU_RETRACT20_SET:
		case CDU_RETRACT30_SET:
		case CDU_PRESENT20_SET:
		case CDU_PRESENT30_SET:
		case CDU_GPRESENT20_SET:
		case CDU_GPRESENT30_SET:
		case CDU_TRAY20_SET:
		case CDU_TRAY30_SET:
			break;
	}

	return nReturn;
}

// Wait Taken
int	CDevCmn::fnCSH_WaitTaken()
{
	int		nReturn = FALSE;

	switch (AtmDefine.CashHandler)
	{
		case BRM_SET:
			nReturn = fnBRM_WaitTaken();
			break;
		case CDU_SHUTTER1510_SET:
		case CDU_SHUTTER20_SET:
		case CDU_SHUTTER30_SET:
		case CDU_RETRACT20_SET:
		case CDU_RETRACT30_SET:
		case CDU_PRESENT20_SET:
		case CDU_PRESENT30_SET:
		case CDU_GPRESENT20_SET:
		case CDU_GPRESENT30_SET:
		case CDU_TRAY20_SET:
		case CDU_TRAY30_SET:
			nReturn = fnCDU_WaitTaken();
			break;
	}

	return nReturn;
}

// Cancel Wait Taken
int	CDevCmn::fnCSH_CancelWaitTaken()
{
	int		nReturn = FALSE;

	switch (AtmDefine.CashHandler)
	{
		case BRM_SET:
			nReturn = fnBRM_CancelWaitTaken();
			break;
		case CDU_SHUTTER1510_SET:
		case CDU_SHUTTER20_SET:
		case CDU_SHUTTER30_SET:
		case CDU_RETRACT20_SET:
		case CDU_RETRACT30_SET:
		case CDU_PRESENT20_SET:
		case CDU_PRESENT30_SET:
		case CDU_GPRESENT20_SET:
		case CDU_GPRESENT30_SET:
		case CDU_TRAY20_SET:
		case CDU_TRAY30_SET:
			nReturn = fnCDU_CancelWaitTaken();
			break;
	}

	return nReturn;
}

// Retract
int	CDevCmn::fnCSH_Retract()
{
	int		nReturn = FALSE;

	switch (AtmDefine.CashHandler)
	{
		case BRM_SET:
			nReturn = fnBRM_Retract();
			break;
		case CDU_SHUTTER1510_SET:
		case CDU_SHUTTER20_SET:
		case CDU_SHUTTER30_SET:
		case CDU_RETRACT20_SET:
		case CDU_RETRACT30_SET:
		case CDU_PRESENT20_SET:
		case CDU_PRESENT30_SET:
		case CDU_GPRESENT20_SET:
		case CDU_GPRESENT30_SET:
		case CDU_TRAY20_SET:
		case CDU_TRAY30_SET:
			nReturn = fnCDU_Retract();
			break;
	}

	return nReturn;
}

// Present2(Wait Time):Reject Part
int	CDevCmn::fnCSH_Present2(int nWaitSec)
{
	int		nReturn = FALSE;

	switch (AtmDefine.CashHandler)
	{
		case BRM_SET:
			nReturn = fnBRM_Present2(nWaitSec);
			break;
		case CDU_SHUTTER1510_SET:
		case CDU_SHUTTER20_SET:
		case CDU_SHUTTER30_SET:
		case CDU_RETRACT20_SET:
		case CDU_RETRACT30_SET:
		case CDU_PRESENT20_SET:
		case CDU_PRESENT30_SET:
		case CDU_GPRESENT20_SET:
		case CDU_GPRESENT30_SET:
		case CDU_TRAY20_SET:
		case CDU_TRAY30_SET:
			break;
	}

	return nReturn;
}

// Ready Dispense
int	CDevCmn::fnCSH_ReadyDispense()
{
	int		nReturn = FALSE;

	switch (AtmDefine.CashHandler)
	{
		case BRM_SET:
			nReturn = fnBRM_ReadyDispense();
			break;
		case CDU_SHUTTER1510_SET:
		case CDU_SHUTTER20_SET:
		case CDU_SHUTTER30_SET:
		case CDU_RETRACT20_SET:
		case CDU_RETRACT30_SET:
		case CDU_PRESENT20_SET:
		case CDU_PRESENT30_SET:
			nReturn = fnCDU_CloseShutter();						// 2007.11.06
			break;
		case CDU_GPRESENT20_SET:
		case CDU_GPRESENT30_SET:
			nReturn = fnCDU_Retract();							// 2007.06.06
			break;
		case CDU_TRAY20_SET:
		case CDU_TRAY30_SET:
			break;
	}

	return nReturn;
}

// Dispense(Cash Amount)
int	CDevCmn::fnCSH_Dispense(int nCashAmt)
{
	int		nReturn = FALSE;

	switch (AtmDefine.CashHandler)
	{
		case BRM_SET:
			nReturn = fnBRM_Dispense(nCashAmt);
			break;
		case CDU_SHUTTER1510_SET:
		case CDU_SHUTTER20_SET:
		case CDU_SHUTTER30_SET:
		case CDU_RETRACT20_SET:
		case CDU_RETRACT30_SET:
		case CDU_PRESENT20_SET:
		case CDU_PRESENT30_SET:
		case CDU_GPRESENT20_SET:
		case CDU_GPRESENT30_SET:
		case CDU_TRAY20_SET:
		case CDU_TRAY30_SET:
			nReturn = fnCDU_Dispense(nCashAmt);
			break;
	}

	return nReturn;
}

// Present(Wait Time)
int	CDevCmn::fnCSH_Present(int nWaitSec)
{
	int		nReturn = FALSE;

	switch (AtmDefine.CashHandler)
	{
		case BRM_SET:
			nReturn = fnBRM_Present(nWaitSec);
			break;
		case CDU_SHUTTER1510_SET:
		case CDU_SHUTTER20_SET:
		case CDU_SHUTTER30_SET:
		case CDU_RETRACT20_SET:
		case CDU_RETRACT30_SET:
		case CDU_PRESENT20_SET:
		case CDU_PRESENT30_SET:
		case CDU_GPRESENT20_SET:
		case CDU_GPRESENT30_SET:
		case CDU_TRAY20_SET:
		case CDU_TRAY30_SET:
			nReturn = fnCDU_Present(nWaitSec);
			break;
	}

	return nReturn;
}

// Refill Money(Cash Count)
int	CDevCmn::fnCSH_RefillMoney(int nCashCnt)
{
	int		nReturn = FALSE;

	switch (AtmDefine.CashHandler)
	{
		case BRM_SET:
			nReturn = fnBRM_RefillMoney(nCashCnt);
			break;
		case CDU_SHUTTER1510_SET:
		case CDU_SHUTTER20_SET:
		case CDU_SHUTTER30_SET:
		case CDU_RETRACT20_SET:
		case CDU_RETRACT30_SET:
		case CDU_PRESENT20_SET:
		case CDU_PRESENT30_SET:
		case CDU_GPRESENT20_SET:
		case CDU_GPRESENT30_SET:
		case CDU_TRAY20_SET:
		case CDU_TRAY30_SET:
			break;
	}

	return nReturn;
}

// Takeup Money(Cash Count)
int	CDevCmn::fnCSH_TakeupMoney(int nCashCnt)
{
	int		nReturn = FALSE;

	switch (AtmDefine.CashHandler)
	{
		case BRM_SET:
			nReturn = fnBRM_TakeupMoney(nCashCnt);
			break;
		case CDU_SHUTTER1510_SET:
		case CDU_SHUTTER20_SET:
		case CDU_SHUTTER30_SET:
		case CDU_RETRACT20_SET:
		case CDU_RETRACT30_SET:
		case CDU_PRESENT20_SET:
		case CDU_PRESENT30_SET:
		case CDU_GPRESENT20_SET:
		case CDU_GPRESENT30_SET:
		case CDU_TRAY20_SET:
		case CDU_TRAY30_SET:
			break;
	}

	return nReturn;
}
