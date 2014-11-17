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
	PRINTLOG(("Recv Num=%d", nRecv));


	if( nRecv == SOCKET_ERROR ) {
		DWORD dwError = ::GetLastError();
		PRINTOUT(("Socket Receive(,1024) Error! ErrorNo=%08X", dwError));
	}
	else if( nRecv == 0 ) {		// closed by peer
		this->Close();
	}
	else {
		this->m_Packet.AddRecv(buff, nRecv);

PRINTLOG(("m_Packet.AddRecv(,%d)", nRecv));

		ErrorCode ec = this->m_Packet.Parse();
		if( ec.is_error() ) {
			// clear recv buffer
			this->m_Packet.Clear();

			PRINTOUT(("Packet Parse Error! Code=%d", ec.get_code()));

			// Error Packet Generation
			this->m_Packet.SetArgNum(1);
			this->m_Packet.AddArg(ec.get_error());

			// encoding packet
			//
			this->m_Packet.Encode();

			// send error
			//
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
			int nCmd = this->m_Packet.GetCommand();

PRINTLOG(("nCmd=%d", nCmd));

			int nArg = 0;
			Argument* pArg = this->m_Packet.GetArg(nArg);

PRINTLOG(("nArg=%d", nArg));

			ErrorCode ec;
			switch( nCmd ) {
			case SimPacket::Add: ec = this->TagAdd(nArg, pArg); break;
			case SimPacket::Del: ec = this->TagDel(nArg, pArg); break;
			case SimPacket::Read: ec = this->TagRead(nArg, pArg); break;
			case SimPacket::Write: ec = this->TagWrite(nArg, pArg); break;
			case SimPacket::Save: ec = this->SnapshotSave(nArg, pArg); break;
			case SimPacket::Load: ec = this->SnapshotLoad(nArg, pArg); break;
			case SimPacket::Remove: ec = this->SnapshotRemove(nArg, pArg); break;
			}

PRINTLOG(("Result=%08X", ec.get_code()));

			// recv packet clear
			// clear argument
			this->m_Packet.Clear();

			// make reply
			// make argument
			if( ec.is_clean() ) {
				this->m_Packet.SetRequestType(Packet::ForResponse);
				this->m_Packet.SetCommand(nCmd);
				this->m_Packet.SetArgNum(1);
				this->m_Packet.AddArg();	// None
			}
			else if( ec.is_error() ) {
				this->m_Packet.SetRequestType(Packet::SendOnly);
				this->m_Packet.SetCommand(SimPacket::Error);
				this->m_Packet.SetArgNum(1);
				this->m_Packet.AddArg(ec.get_error());
			}
			else {
				// what is this?
			}

PRINTLOG(("Reply Arg Set=%08X", this->m_Packet.GetCommand() ));

			// encoding
			this->m_Packet.Encode();

PRINTLOG(("this->m_Packet.Encode() Finish"));

			// get send buffer
			// send(buf, num);
			int nSend = 0;
			const char *ptr = this->m_Packet.GetSend(nSend);
PRINTLOG(("Buffer=%p, nSend=%d", ptr, nSend));

			int nRet = this->Send(ptr, nSend);
			if( nRet == SOCKET_ERROR ) {
				DWORD dwError = ::GetLastError();
				PRINTOUT(("Socket Send(,%d) Error! ErrorNo=%08X", nSend, dwError));
			}
			else {
PRINTLOG(("nRet=%d", nRet));
				this->m_Packet.SubSend(nRet);
			}
		}
	}

	CAsyncSocket::OnReceive(nErrorCode);
}


void CServerSock::OnSend(int nErrorCode)
{
	PRINTLOG(("OnSend(%d)", nErrorCode));

	int nSend = 0;
	const char *pBuf = this->m_Packet.GetSend(nSend);
	PRINTLOG(("Sendable Data = %d", nSend));

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


ErrorCode test_handler(int nArg, Argument* pArg)
{
	return ErrorCode();
}


ErrorCode CServerSock::TagAdd(int nArg, Argument* pArg)
{
	return ErrorCode();
}


ErrorCode CServerSock::TagDel(int nArg, Argument* pArg)
{
	return ErrorCode();
}


ErrorCode CServerSock::TagRead(int nArg, Argument* pArg)
{
	return ErrorCode();
}


ErrorCode CServerSock::TagWrite(int nArg, Argument* pArg)
{
	return ErrorCode();
}


ErrorCode CServerSock::SnapshotSave(int nArg, Argument* pArg)
{
	return ErrorCode();
}


ErrorCode CServerSock::SnapshotLoad(int nArg, Argument* pArg)
{
	return ErrorCode();
}


ErrorCode CServerSock::SnapshotRemove(int nArg, Argument* pArg)
{
	return ErrorCode();
}
