/////////////////////////////////////////////////////////////////////////////
//	DevMonitor2.cpp : implementation file
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"

#include "..\..\H\Common\CmnLibIn.h"
#include "..\..\H\Common\ConstDef.h"

#include "..\..\H\Dll\DevDefine.h"

#include "..\..\H\Dll\DevCmn.h"
#include "..\..\H\Dll\MonitorClient.h"

/////////////////////////////////////////////////////////////////////////////
//	AVI FUNCTION(MONITOR2) : AVI SET MODE
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
//	AVI FUNCTION(MONITOR2) : AVI GET MODE
/////////////////////////////////////////////////////////////////////////////
// Get Device Count
int	CDevCmn::fnAVI_GetDeviceCount()
{
	int		nReturn = FALSE;

	switch (AtmDefine.CameraDevice)
	{
		case USB_AVICAMERA1_SET:
		case USB_AVICAMERA2_SET:
			nReturn = HCMonitor2_GetDeviceCount();
			break;
	}

	return nReturn;
}


/////////////////////////////////////////////////////////////////////////////
//	AVI FUNCTION(MONITOR2) : AVI OPERATION MODE
/////////////////////////////////////////////////////////////////////////////
// Register
int	CDevCmn::fnAVI_Register()
{
	int		nReturn = FALSE;

	switch (AtmDefine.CameraDevice)
	{
		case USB_AVICAMERA1_SET:
		case USB_AVICAMERA2_SET:
			HCMonitor2_Register("A5000B");
			break;
	}

	return nReturn;
}

// Customer Service Begin
int CDevCmn::fnAVI_CustomerServiceBegin()
{
	int	nReturn = FALSE;

	switch (AtmDefine.CameraDevice)
	{
		case USB_AVICAMERA1_SET:
		case USB_AVICAMERA2_SET:
			nReturn = HCMonitor2_CustomerServiceBegin();
			break;
	}

	return nReturn;
}

// Customer Service End
int	CDevCmn::fnAVI_CustomerServiceEnd(DWORD dwCustomer)
{
	int		nReturn = FALSE;

	switch (AtmDefine.CameraDevice)
	{
		case USB_AVICAMERA1_SET:
		case USB_AVICAMERA2_SET:
			HCMonitor2_CustomerServiceEnd(dwCustomer);
			break;
	}

	return nReturn;
}

// Add Info
int	CDevCmn::fnAVI_AddInfo(DWORD dwCustomer, const char* pCardNo, const char* pTrans, const char* pResult)
{
	int		nReturn = FALSE;

	switch (AtmDefine.CameraDevice)
	{
		case USB_AVICAMERA1_SET:
		case USB_AVICAMERA2_SET:
			HCMonitor2_AddInfo(dwCustomer, pCardNo, pTrans, pResult);
			break;
	}

	return nReturn;
}

// Start Cap
int	CDevCmn::fnAVI_StartCap(DWORD dwSlotNum, DWORD dwCustomer, const char* pCardNo, const char* pTrans, const char* pResult)
{
	int		nReturn = FALSE;

	switch (AtmDefine.CameraDevice)
	{
		case USB_AVICAMERA1_SET:
		case USB_AVICAMERA2_SET:
			HCMonitor2_StartCap(dwSlotNum, dwCustomer, pCardNo, pTrans, pResult);
			break;
	}

	return nReturn;
}

// Start Cap With Cap Seconds
int	CDevCmn::fnAVI_StartCapWithCapSeconds(DWORD dwSlotNum, DWORD dwSeconds, DWORD dwCustomer, const char* pCardNo, const char* pTrans, const char* pResult)
{
	int		nReturn = FALSE;

	switch (AtmDefine.CameraDevice)
	{
		case USB_AVICAMERA1_SET:
		case USB_AVICAMERA2_SET:
			HCMonitor2_StartCapWithCapSeconds(dwSlotNum, dwSeconds, dwCustomer, pCardNo, pTrans, pResult);
			break;
	}

	return nReturn;
}

// Stop Cap
int	CDevCmn::fnAVI_StopCap(DWORD dwSlotNum)
{
	int		nReturn = FALSE;

	switch (AtmDefine.CameraDevice)
	{
		case USB_AVICAMERA1_SET:
		case USB_AVICAMERA2_SET:
			HCMonitor2_StopCap(dwSlotNum);
			break;
	}

	return nReturn;
}

// Stop Cap After Seconds
int	CDevCmn::fnAVI_StopCapAfterSeconds(DWORD dwSlotNum, DWORD dwSeconds)
{
	int		nReturn = FALSE;

	switch (AtmDefine.CameraDevice)
	{
		case USB_AVICAMERA1_SET:
		case USB_AVICAMERA2_SET:
			HCMonitor2_StopCapAfterSeconds(dwSlotNum, dwSeconds);
			break;
	}

	return nReturn;
}

// Set Video Size
int	CDevCmn::fnAVI_SetVideoSize(DWORD dwSlotNum, DWORD dwWidth, DWORD dwHeight)
{
	int		nReturn = FALSE;

	switch (AtmDefine.CameraDevice)
	{
		case USB_AVICAMERA1_SET:
		case USB_AVICAMERA2_SET:
			HCMonitor2_SetVideoSize(dwSlotNum, dwWidth, dwHeight);
			break;
	}

	return nReturn;
}

// Start Pre Recording
int	CDevCmn::fnAVI_StartPreRecording(DWORD dwSlotNum, DWORD dwSeconds)
{
	int		nReturn = FALSE;

	switch (AtmDefine.CameraDevice)
	{
		case USB_AVICAMERA1_SET:
		case USB_AVICAMERA2_SET:
			HCMonitor2_StartPreRecording(dwSlotNum, dwSeconds);
			break;
	}

	return nReturn;
}

// Restart Allowed
int	CDevCmn::fnAVI_RestartAllowed()
{
	int		nReturn = FALSE;

	switch (AtmDefine.CameraDevice)
	{
		case USB_AVICAMERA1_SET:
		case USB_AVICAMERA2_SET:
			HCMonitor2_RestartAllowed();
			break;
	}

	return nReturn;
}

