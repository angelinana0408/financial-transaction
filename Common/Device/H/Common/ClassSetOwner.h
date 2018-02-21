/////////////////////////////////////////////////////////////////////////////
//	Class Set Owner Header
/////////////////////////////////////////////////////////////////////////////
	m_pOwner		= pOwner;

	if (m_pOwner)
	{
		m_pDevCmn		= m_pOwner->m_pDevCmn;
		m_pTranCmn		= m_pOwner->m_pTranCmn;

		if (m_pOwner->m_pDevCmn)
		{
			m_pProfile		= m_pOwner->m_pDevCmn->m_pProfile;
			m_pNetWork		= m_pOwner->m_pDevCmn->m_pNetWork;
			m_pComPort		= m_pOwner->m_pDevCmn->m_pComPort;
			m_pLanPort		= m_pOwner->m_pDevCmn->m_pLanPort;
			m_pNmsWork		= m_pOwner->m_pDevCmn->m_pNmsWork;
			m_pEtcWork		= m_pOwner->m_pDevCmn->m_pEtcWork;
		}
	}
