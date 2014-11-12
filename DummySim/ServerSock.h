#pragma once

#include "..\Packet\SimPacket.h"

// CServerSock command target

class CDummySimDlg;

class CServerSock : public CAsyncSocket
{
	static int m_nCount;
public:
	CServerSock(CDummySimDlg* pDlg);
	virtual ~CServerSock();
	virtual void OnAccept(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
	virtual void OnSend(int nErrorCode);
private:
	CDummySimDlg* m_pDlg;
public:
	virtual void OnClose(int nErrorCode);
private:
	SimPacket m_Packet;
};


