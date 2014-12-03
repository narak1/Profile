// MySock.cpp : implementation file
//

#include "stdafx.h"
#include "SIMBridge.h"
#include "SIMBridgeDlg.h"
#include "MySock.h"
#include "SocketManager.h"

#include "../Packet/Packet.h"
#include "../Packet/SimPacket.h"
#include "../Packet/InstPacket.h"
#include "../Packet/ErrorCode.h"

#include "../Logger/Logger.h"
#include "../Logger/Util.h"

// CMySock

int CMySock::m_nCount = 0;

CMySock::CMySock(CSIMBridgeDlg* pDlg)
	: m_pDlg(pDlg)
{
	++m_nCount;
	PRINTLOG((Logger::DebugLog, "CServerSock() : count=%d", this->m_nCount));
	this->m_Util = new Util();
	this->m_Packet = new InstPacket();
	this->m_SockMgr = this->m_pDlg->GetSocketManager();
}

CMySock::~CMySock()
{
	--m_nCount;
	PRINTLOG((Logger::DebugLog, "~CServerSock() : count=%d", this->m_nCount));
	delete this->m_Util;
	delete this->m_Packet;
}


// CMySock member functions


void CMySock::OnAccept(int nErrorCode)
{
	PRINTLOG((Logger::DebugLog, "OnAccept(%d)", nErrorCode));

	CMySock *pSock = new CMySock(this->m_pDlg);
	BOOL flag = this->Accept(*pSock);
	if( flag == 0 ) {
		DWORD dwError = ::GetLastError();
		PRINTOUT(("Socket Accept() Error! %08X:%s", dwError, this->m_Util->error_str(dwError)));
		delete pSock;
	}

	// Socket Register
	this->m_SockMgr->Add(pSock);

	CAsyncSocket::OnAccept(nErrorCode);
}


void CMySock::OnClose(int nErrorCode)
{
	PRINTLOG((Logger::DebugLog, "OnClose(%d)", nErrorCode));

	delete this;

	CAsyncSocket::OnClose(nErrorCode);
}


void CMySock::OnConnect(int nErrorCode)
{
	// Send Data if exist

	CAsyncSocket::OnConnect(nErrorCode);
}


void CMySock::OnReceive(int nErrorCode)
{
	PRINTLOG((Logger::DebugLog, "OnReceive(%d)", nErrorCode));

	char buff[1024];

	int nRecv = this->Receive(buff, 1024);
	PRINTLOG((Logger::DebugLog, "Recv Num=%d", nRecv));


	if( nRecv == SOCKET_ERROR ) {
		DWORD dwError = ::GetLastError();
		PRINTOUT(("Socket Receive(,1024) Error! %08X:%s", dwError, this->m_Util->error_str(dwError)));
	}
	else if( nRecv == 0 ) {		// closed by peer
		this->Close();
	}
	else {
		this->m_Packet->AddRecv(buff, nRecv);

		PRINTLOG((Logger::DebugLog, "m_Packet.AddRecv(,%d)", nRecv));
		ErrorCode ec = this->m_Packet->Parse();
		if( ec.is_error() ) {
			// clear recv buffer
			this->m_Packet->Clear();

			PRINTOUT(("Packet Parse Error! Code=%d", ec.get_code()));

			// Error Packet Generation
			this->m_Packet->SetArgNum(1);
			this->m_Packet->AddArg(ec.get_error());

			// encoding packet
			//
			this->m_Packet->Encode();

			// send error
			//
			int nSend = 0;
			const char *ptr = this->m_Packet->GetSend(nSend);
			int nRet = this->Send(ptr, nSend);
			if( nRet == SOCKET_ERROR ) {
				DWORD dwError = ::GetLastError();
				PRINTOUT(("Socket Send(,%d) Error! ErrorNo=%08X", nSend, dwError));
			}
			else {
				this->m_Packet->SubSend(nRet);
			}
		}
		else if( ec.is_clean() ) {
			// processing
			int nCmd = this->m_Packet->GetCommand();

PRINTLOG((Logger::DebugLog, "nCmd=%d", nCmd));

			int nArg = 0;
			Argument* pArg = this->m_Packet->GetArg(nArg);

PRINTLOG((Logger::DebugLog, "nArg=%d", nArg));

			ErrorCode ec;

PRINTLOG((Logger::DebugLog, "Result=%08X", ec.get_code()));

			// recv packet clear
			// clear argument
			this->m_Packet->Clear();

			// make reply
			// make argument
			if( ec.is_clean() ) {
				this->m_Packet->SetRequestType(Packet::ForResponse);
				this->m_Packet->SetCommand(nCmd);
				this->m_Packet->SetArgNum(1);
				this->m_Packet->AddArg();	// None
			}
			else if( ec.is_error() ) {
				this->m_Packet->SetRequestType(Packet::SendOnly);
				this->m_Packet->SetCommand(SimPacket::Error);
				this->m_Packet->SetArgNum(1);
				this->m_Packet->AddArg(ec.get_error());
			}
			else {
				// what is this?
			}

PRINTLOG((Logger::DebugLog, "Reply Arg Set=%08X", this->m_Packet->GetCommand() ));

			// encoding
			this->m_Packet->Encode();

PRINTLOG((Logger::DebugLog, "this->m_Packet.Encode() Finish"));

			// get send buffer
			// send(buf, num);
			int nSend = 0;
			const char *ptr = this->m_Packet->GetSend(nSend);
PRINTLOG((Logger::DebugLog, "Buffer=%p, nSend=%d", ptr, nSend));

			int nRet = this->Send(ptr, nSend);
			if( nRet == SOCKET_ERROR ) {
				DWORD dwError = ::GetLastError();
				PRINTOUT(("Socket Send(,%d) Error! ErrorNo=%08X", nSend, dwError));
			}
			else {
PRINTLOG((Logger::DebugLog, "nRet=%d", nRet));
				this->m_Packet->SubSend(nRet);
			}
		}
	}

	CAsyncSocket::OnReceive(nErrorCode);
}


void CMySock::OnSend(int nErrorCode)
{
	PRINTLOG((Logger::DebugLog, "OnSend(%d)", nErrorCode));

	int nSend = 0;
	const char *pBuf = this->m_Packet->GetSend(nSend);
	PRINTLOG((Logger::DebugLog, "Sendable Data = %d", nSend));

	if( nSend > 0 ) {
		int nRet = this->Send(pBuf, nSend);
		if( nRet == SOCKET_ERROR ) {
			DWORD dwError = ::GetLastError();
			PRINTOUT(("Socket Send(,%d) Error! ErrorNo=%08X", nSend, dwError));
		}
		else {
			this->m_Packet->SubSend(nRet);
		}
	}

	CAsyncSocket::OnSend(nErrorCode);
}
