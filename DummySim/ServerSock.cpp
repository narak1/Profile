// ServerSock.cpp : implementation file
//

#include "stdafx.h"
#include "DummySim.h"
#include "ServerSock.h"

#include "DummySimDlg.h"

#include "../Packet/ErrorCode.h"

#define PRINTOUT(a) this->m_pDlg->PrintOut a
#define PRINTLOG(a) this->m_pDlg->PrintLog a

// CServerSock

int CServerSock::m_nCount = 0;

CServerSock::CServerSock(CDummySimDlg* pDlg)
	: m_pDlg(pDlg)
{
	++m_nCount;
	PRINTLOG(("CServerSock() : count=%d", this->m_nCount));
}

CServerSock::~CServerSock()
{
	--m_nCount;
	PRINTLOG(("~CServerSock() : count=%d", this->m_nCount));
}


// CServerSock member functions


void CServerSock::OnAccept(int nErrorCode)
{
	PRINTLOG(("OnAccept(%d)", nErrorCode));

	CServerSock *pSock = new CServerSock(this->m_pDlg);
	BOOL flag = this->Accept(*pSock);
	if( flag == 0 ) {
		DWORD dwError = ::GetLastError();
		PRINTOUT(("Socket Accept(&socket) Error! ErrorNo=%08X", dwError));
		delete pSock;
	}

	CAsyncSocket::OnAccept(nErrorCode);
}


void CServerSock::OnReceive(int nErrorCode)
{
	PRINTLOG(("OnReceive(%d)", nErrorCode));

	char buff[1024];

	int nRecv = this->Receive(buff, 1024);
	if( nRecv == SOCKET_ERROR ) {
		DWORD dwError = ::GetLastError();
		PRINTOUT(("Socket Receive(,1024) Error! ErrorNo=%08X", dwError));
	}
	else if( nRecv == 0 ) {		// closed by peer
		this->Close();
	}
	else {
		this->m_Packet.AddRecv(buff, nRecv);

		ErrorCode ec = this->m_Packet.Parse();
		if( ec.is_error() ) {
			PRINTOUT(("Packet Parse Error! Code=%d", ec.get_code()));

			// Error Packet Generation
			int nSend = 0;
			const char *ptr = this->m_Packet.GetSend(nSend);
			int nRet = this->Send(ptr, nSend);
			if( nRet == SOCKET_ERROR ) {
				DWORD dwError = ::GetLastError();
				PRINTOUT(("Socket Send(,%d) Error! ErrorNo=%08X", nSend, dwError));
			}
			else {
				this->m_Packet.SubSend(nRet);
			}
		}
		else if( ec.is_clean() ) {
			// processing
			//this->m_Packet..
		}
	}

	CAsyncSocket::OnReceive(nErrorCode);
}


void CServerSock::OnSend(int nErrorCode)
{
	PRINTLOG(("OnSend(%d)", nErrorCode));

	int nSend = 0;
	const char *pBuf = this->m_Packet.GetSend(nSend);

	if( nSend > 0 ) {
		int nRet = this->Send(pBuf, nSend);
		if( nRet == SOCKET_ERROR ) {
			DWORD dwError = ::GetLastError();
			PRINTOUT(("Socket Send(,%d) Error! ErrorNo=%08X", nSend, dwError));
		}
		else {
			this->m_Packet.SubSend(nRet);
		}
	}

	CAsyncSocket::OnSend(nErrorCode);
}


void CServerSock::OnClose(int nErrorCode)
{
	PRINTLOG(("OnClose(%d)", nErrorCode));

	delete this;

	CAsyncSocket::OnClose(nErrorCode);
}
