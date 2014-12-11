// MySock.cpp : implementation file
//

#include "stdafx.h"
#include "SIMBridge.h"
#include "SIMBridgeDlg.h"
#include "MySock.h"
#include "SocketManager.h"

#include "../Packet/Argument.h"
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
		CString strPeerName;
		UINT nPeerPort = 0;
		BOOL flag = this->GetPeerName(strPeerName, nPeerPort);
		if( flag == FALSE ) {
			DWORD dwError = ::GetLastError();
			PRINTOUT(("Socket GetPeerName(,) Error! %08X:%s", dwError, this->m_Util->error_str(dwError)));
			PRINTOUT(("Socket Closed."));
		}
		else {
			PRINTOUT(("%s:%d Socket Closed.", strPeerName, nPeerPort));
		}

		this->Close();
	}
	else {
		this->m_Packet->AddRecv(buff, nRecv);

		PRINTLOG((Logger::DebugLog, "m_Packet.AddRecv(,%d)", nRecv));

		while(1)	// multiple message recved
		{
			ErrorCode ec = this->m_Packet->Parse();
			if( ec.is_error() ) {
				PRINTOUT(("Packet Parse Error! Code=%d", ec.get_code()));

				SendError(ec);
			}
			else if( ec.is_clean() ) {
				// processing
				int nCmd = this->m_Packet->GetCommand();

PRINTLOG((Logger::DebugLog, "nCmd=%d", nCmd));

				int nArg = 0;
				Argument* pArg = this->m_Packet->GetArg(nArg);

PRINTLOG((Logger::DebugLog, "nArg=%d", nArg));

				ErrorCode ec;

			// Processing using Handler
//	enum Command { Error, Hello, Set, Info, SimInit, SimTime, TagInit, TagDown, Send, Run, MaxCmd };

				switch( nCmd ) {
				case InstPacket::Error:		ec = this->CmdError();	break;
				case InstPacket::Hello:		ec = this->CmdHello();	break;
				case InstPacket::Info:		ec = this->CmdInfo();	break;
				case InstPacket::SimInit:	ec = this->CmdSimInit();	break;
				case InstPacket::SimTime:	ec = this->CmdSimTime();	break;
				case InstPacket::TagInit:	ec = this->CmdTagInit();	break;
				case InstPacket::TagDown:	ec = this->CmdTagDown();	break;
				case InstPacket::Send:		ec = this->CmdSend();	break;
				case InstPacket::Run:		ec = this->CmdRun();	break;
				default: ec.set(true, 0, 0, "Unknown command");
				}
				PRINTLOG((Logger::DebugLog, "Result=%08X", ec.get_code()));

				this->m_Packet->Clear();	// recv packet clear
			}
			else {
				// not complete packet ==> end loop
				break;
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


ErrorCode CMySock::CmdError(void)
{
	return ErrorCode();
}


ErrorCode CMySock::CmdHello(void)
{
	PRINTLOG((Logger::DebugLog, "CMySock::CmdHello(void)"));

	// who?
	// SocketManager node name setting
	int nArg = 0;
	Argument *pArg = this->m_Packet->GetArg(nArg);
	const char* node_name = (const char*) pArg->get_value();

	PRINTLOG((Logger::DebugLog, "From Node Name=[%s]\n", node_name));

	// Send NodeName / ServerType / Priority
	//
	this->m_Packet->Clear();
	this->m_Packet->SetRequestType(Packet::ForResponse);
	this->m_Packet->SetCommand(InstPacket::Hello);
	this->m_Packet->SetArgNum(3);
	this->m_Packet->AddArg(this->m_pDlg->m_strHostname);	// Node Name
	this->m_Packet->AddArg(this->m_pDlg->m_strServerType);
	this->m_Packet->AddArg(this->m_pDlg->m_nPriority);

	this->Send_Start();

	return ErrorCode();
}


ErrorCode CMySock::CmdSet(void)
{
	return ErrorCode();
}


ErrorCode CMySock::CmdInfo(void)
{
	return ErrorCode();
}


ErrorCode CMySock::CmdSimInit(void)
{
	return ErrorCode();
}


ErrorCode CMySock::CmdSimTime(void)
{
	return ErrorCode();
}


ErrorCode CMySock::CmdTagInit(void)
{
	return ErrorCode();
}


ErrorCode CMySock::CmdTagDown(void)
{
	return ErrorCode();
}


ErrorCode CMySock::CmdSend(void)
{
	return ErrorCode();
}


ErrorCode CMySock::CmdRun(void)
{
	return ErrorCode();
}


void CMySock::Send_Start(void)
{
	// encoding packet
	//
	this->m_Packet->Encode();

	// send packet
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


void CMySock::SendError(ErrorCode& ec)
{
	// clear recv buffer
	this->m_Packet->Clear();

	// Error Packet Generation
	this->m_Packet->SetArgNum(1);
	this->m_Packet->AddArg(ec.get_error());

	this->Send_Start();
}
