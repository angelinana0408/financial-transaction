/////////////////////////////////////////////////////////////////////////////
// AtmHostCtrl.cpp : Implementation of the CWinAtmCtrl ActiveX Control class.
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"

#include "..\H\Common\CmnLibIn.h"
#include "..\H\Common\ConstDef.h"
#include "..\H\Common\ClassInclude.h"

#include "..\H\Dll\DevDefine.h"
#include "..\H\Dll\DevCmn.h"
#include "..\H\Tran\TranCmnDefine.h"
#include "..\H\Tran\TranCmn.h"

#include "..\H\Dll\DevEtc.h"

#include "WinAtm.h"
#include "WinAtmCtl.h"
#include "WinAtmPpg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
//	Host Status
/////////////////////////////////////////////////////////////////////////////
//	Mode :	HOST_OFFLINE		: OffLine		Mode
//			HOST_WAITLINK		: Wait Link		Mode
//			HOST_LINK			: Link			Mode
//			HOST_WAITREADY		: Wait Ready	Mode
//			HOST_READY			: Ready			Mode
//			HOST_WAITONLINE		: Wait Online	Mode
//			HOST_ONLINE			: Online		Mode
/////////////////////////////////////////////////////////////////////////////
// Atm Host Control
int CWinAtmCtrl::AtmHostCtrl()
{
/////////////////////////////////////////////////////////////////////////////
	if (!m_pDevCmn->fnAPM_AtmHostCtrl())						// Atm Host Control
		return TRUE;
/////////////////////////////////////////////////////////////////////////////

if (m_pDevCmn->HostStatus != m_pDevCmn->HostStatusSave)
{
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "HostStatus[%s], HostStatusSave[%s]", HostStatusName[m_pDevCmn->HostStatus], HostStatusName[m_pDevCmn->HostStatusSave]);
}
	switch (m_pDevCmn->HostStatus) 
	{
		case HOST_OFFLINE:										// OffLine Mode
			m_pDevCmn->HostStatusSave = HOST_OFFLINE;

/////////////////////////////////////////////////////////////////////////////
//	Skip Procedure : 2004.07.12
/////////////////////////////////////////////////////////////////////////////
			if ((m_pDevCmn->AtmStatus == ATM_CUSTOM) ||			
				(m_pDevCmn->AtmStatus == ATM_TRAN))
			{
				break;
			}
/////////////////////////////////////////////////////////////////////////////
			m_pDevCmn->fnNET_Initialize(LINE_OPCL_TIME, TRUE);	// 2004.07.12

			if (m_pDevCmn->fnNET_GetDeviceStatus() != LINE_OPEN)
			{
				m_pDevCmn->Timer.HostWaitLinkTime = m_pTranCmn->InitInfo.LineRetryTime;
																// 2005.07.26 : DO NOT MODIFY
				m_pDevCmn->HostStatus = HOST_WAITLINK;
			}
			else m_pDevCmn->HostStatus = HOST_LINK;
			break;

		case HOST_WAITLINK:										// Wait Link Mode
			m_pDevCmn->HostStatusSave = HOST_WAITLINK;

			if (m_pDevCmn->fnNET_GetDeviceStatus() != LINE_OPEN)
			{
				if (!m_pDevCmn->Timer.HostWaitLinkTime)
				{
					m_pDevCmn->HostStatus = HOST_OFFLINE;
					m_pDevCmn->HostOpenFlag = FALSE;
				}
				else m_pDevCmn->HostStatus = HOST_WAITLINK;
			}
			else m_pDevCmn->HostStatus = HOST_LINK;
			break;

		case HOST_LINK:											// Link Mode
			m_pDevCmn->HostStatusSave = HOST_LINK;

			if (m_pDevCmn->fnNET_GetDeviceStatus() != LINE_OPEN)
			{
				m_pDevCmn->HostStatus = HOST_OFFLINE;
				m_pDevCmn->HostOpenFlag = FALSE;
			}
			else m_pDevCmn->HostStatus = HOST_WAITREADY;
			break;

		case HOST_WAITREADY:									// Wait Ready Mode
			m_pDevCmn->HostStatusSave = HOST_WAITREADY;

			if (m_pDevCmn->fnNET_GetDeviceStatus() != LINE_OPEN)
			{
				m_pDevCmn->HostStatus = HOST_OFFLINE;
				m_pDevCmn->HostOpenFlag = FALSE;
			}
			else m_pDevCmn->HostStatus = HOST_WAITREADY;
			break;

		case HOST_READY:										// Ready Mode
			m_pDevCmn->HostStatusSave = HOST_READY;

			if (m_pDevCmn->fnNET_GetDeviceStatus() != LINE_OPEN)
			{
				m_pDevCmn->HostStatus = HOST_OFFLINE;
				m_pDevCmn->HostOpenFlag = FALSE;
			}
			else m_pDevCmn->HostStatus = HOST_WAITONLINE;
			break;

		case HOST_WAITONLINE:									// Wait Online Mode
			m_pDevCmn->HostStatusSave = HOST_WAITONLINE;

			if (m_pDevCmn->fnNET_GetDeviceStatus() != LINE_OPEN)
			{
				m_pDevCmn->HostStatus = HOST_OFFLINE;
				m_pDevCmn->HostOpenFlag = FALSE;
			}
			else m_pDevCmn->HostStatus = HOST_WAITONLINE;
			break;

		case HOST_ONLINE:										// Online Mode
			m_pDevCmn->HostStatusSave = HOST_ONLINE;

			if (m_pDevCmn->fnNET_GetDeviceStatus() != LINE_OPEN)
			{
				m_pDevCmn->HostStatus = HOST_OFFLINE;
				m_pDevCmn->HostOpenFlag = FALSE;
			}
			else m_pDevCmn->HostStatus = HOST_ONLINE;
			break;

		default:
			break;
	}

	return TRUE;
}