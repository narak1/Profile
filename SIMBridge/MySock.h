#pragma once

// CMySock command target

class Util;
class Packet;
class SimPacket;
class InstPacket;
class CSIMBridgeDlg;

class CMySock : public CAsyncSocket
{
	static int m_nCount;
public:
	CMySock(CSIMBridgeDlg* pDlg);
	virtual ~CMySock();
	virtual void OnAccept(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	virtual void OnConnect(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
	virtual void OnSend(int nErrorCode);
private:
	Util* m_Util;
	InstPacket* m_Packet;
	CSIMBridgeDlg* m_pDlg;
	SocketManager* m_SockMgr;
};


