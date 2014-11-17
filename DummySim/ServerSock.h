#pragma once

#include "..\Packet\SimPacket.h"

// CServerSock command target

class CDummySimDlg;
class CServerSock;

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
public:
	ErrorCode TagAdd(int nArg, Argument* pArg);
	ErrorCode TagDel(int nArg, Argument* pArg);
	ErrorCode TagRead(int nArg, Argument* pArg);
	ErrorCode TagWrite(int nArg, Argument* pArg);
	ErrorCode SnapshotSave(int nArg, Argument* pArg);
	ErrorCode SnapshotLoad(int nArg, Argument* pArg);
	ErrorCode SnapshotRemove(int nArg, Argument* pArg);
};


