#pragma once

class CMySock;

class SocketManager
{
	static const int MaxSock = 1024;
	char *m_strName[MaxSock];
	CMySock *m_pSock[MaxSock];
	int m_nNum;
public:
	SocketManager(void);
	~SocketManager(void);
	bool Add(CMySock* pSock);
	bool SetName(CMySock* pSock, const char* strName);
	bool SetName(int idx, const char* name);
	const char* GetName(CMySock* pSock);
	const char* GetName(int idx);
	CMySock* GetSock(int nIdx);
	CMySock* GetSock(const char* name);
	int GetIndex(const char* name);
	int GetIndex(CMySock* pSock);
};

