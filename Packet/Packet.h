#pragma once
#include "memorymanager.h"

class Argument;
class ErrorCode;

class Packet
{
public:
	Packet(void);
	~Packet(void);
	static const int HDR_STX = 4;
	static const int HDR_CMD = 8;
	static const int HDR_LEN = 4;
	static const int HDR_SIZE = (HDR_STX + HDR_CMD + HDR_LEN);
	enum RequestType { ForSend='S', ForResponse='R', SendOnly='O', };
protected:
	int m_nRecvNum;
	int m_nSendNum;
	char* m_pRecvBuf;
	char* m_pSendBuf;
public:
	void AddRecv(const char* pBuf, int nLen);
	void SubSend(int nLen);
	const char* GetSend(int& nData);
protected:
	int m_nRBufSize;
	int m_nSBufSize;
public:
	ErrorCode Parse(void);
private:
	char* FindSTX(void);
protected:
	int m_nReqType;
	const char* m_pCmd;
	int m_nCmd;
	int m_nLen;
	int m_nArg;
	Argument* m_pArg;
public:
	Argument* GetArg(int& nArg);
	void Clear(bool bSend=false);
	void AddArg(const char* pStr);
	void SetArgNum(int nNum);
	void Encode(void);
	void AddArg(void);
	int GetRequestType() { return this->m_nReqType; };
	void SetRequestType(int nType) { this->m_nReqType = nType; };
	void SetCommand(const char* pCmd) { this->m_pCmd = pCmd; };
	void SetCommand(int nCmd) { this->m_nCmd = nCmd; };
	int GetCommand(void) { return this->m_nCmd; };
	void AddArg(int nVal);
	void AddArg(int* pVal);
	void AddArg(int arrVal[], int num);
private:
	MemoryManager m_mm;
};
