#pragma warning(disable:4996)

#include <cstring>
#include "Packet.h"
#include "Argument.h"
#include "ErrorCode.h"

#include "../Logger/Logger.h"

Packet::Packet(void)
	: m_nRecvNum(0)
	, m_nSendNum(0)
	, m_pRecvBuf(NULL)
	, m_pSendBuf(NULL)
	, m_nRBufSize(0)
	, m_nSBufSize(0)
	, m_nReqType(0)
	, m_pCmd(NULL)
	, m_nCmd(0)
	, m_nLen(0)
	, m_nArg(0)
	, m_pArg(NULL)
{
}


Packet::~Packet(void)
{
}


void Packet::AddRecv(const char* pBuf, int nLen)
{
	PRINTLOG((Logger::DebugLog, "this=%p, AddRecv(pBuf=%p, nLen=%d)", this, pBuf, nLen));
	PRINTDUMP(pBuf, nLen);
	PRINTLOG((Logger::DebugLog, "this->m_nRecvNum=%d, this->m_nRBufSize=%d", this->m_nRecvNum, this->m_nRBufSize));

	if( this->m_nRecvNum + nLen > this->m_nRBufSize )
	{
		char* pOld = this->m_pRecvBuf;
		this->m_nRBufSize += nLen;
		this->m_pRecvBuf = new char [this->m_nRBufSize];
		::memcpy(this->m_pRecvBuf, pOld, this->m_nRecvNum);
		if( pOld != NULL ) delete [] pOld;
	}
	::memcpy(this->m_pRecvBuf + this->m_nRecvNum , pBuf, nLen);
	this->m_nRecvNum += nLen;
}


void Packet::SubSend(int nLen)
{
	PRINTLOG((Logger::DebugLog, "Packet::SubSend(nLen=%d)  vs this->m_nSendNum=%d", nLen, this->m_nSendNum));
	this->m_nSendNum -= nLen;
	::memcpy(this->m_pSendBuf, this->m_pSendBuf + nLen, this->m_nSendNum);
}


const char* Packet::GetSend(int& nData)
{
	nData = this->m_nSendNum;
	return this->m_pSendBuf;
}


ErrorCode Packet::Parse(void)
{
	PRINTLOG((Logger::DebugLog, "Packet::Parse()"));
	PRINTLOG((Logger::DebugLog, "RecvData (%d):", this->m_nRecvNum));
	PRINTLOG((Logger::DebugLog, "RecvBuff (%p):", this->m_pRecvBuf));
	PRINTDUMP(this->m_pRecvBuf, this->m_nRecvNum);
	PRINTLOG((Logger::DebugLog, "RecvData (%d):", this->m_nRecvNum));

	ErrorCode ec;

	if( this->m_nRecvNum < Packet::HDR_SIZE ) {
		PRINTLOG((Logger::DebugLog, "Not complete header"));
		return ec.set(false, ErrorCode::Packet, 1);
	}

	char *ptr = this->FindSTX();
	if( ptr == NULL ) {
		PRINTLOG((Logger::DebugLog, "Not found STX!"));
		return ec.set(true, ErrorCode::Packet, 2);
	}
	ptr += this->HDR_STX;

	this->m_pCmd = ptr;
	ptr += this->HDR_CMD;

	this->m_nLen = * (int *) ptr;
	ptr += this->HDR_LEN;						// packet body pointer

	//=== End of header

	// packet size check
	if( this->m_nRecvNum <  this->m_nLen ) {
		PRINTLOG((Logger::DebugLog, "Not complete packet"));
		return ec.set(false, ErrorCode::Packet, 3);
	}

	int num = this->m_nLen - this->HDR_SIZE;	// packet body length

	PRINTLOG((Logger::DebugLog, "Packet Size=%d, Packet Body Length=%d", this->m_nLen, num));

	// parse argement
	int nArg = * (int *) ptr;
	Argument* pArg = new Argument [nArg];
	ptr += 4;
	num -= 4;

	PRINTLOG((Logger::DebugLog, "nArg=%d, num=%d", nArg, num));

	for( int i=0 ; i<nArg ; i++ )
	{
		short nType = * (short *) ptr;
		ptr += 2;
		num -= 2;
		short nLen = * (short *) ptr;
		ptr += 2;
		num -= 2;
		PRINTLOG((Logger::DebugLog, "i=%d: nType=%d, nLen=%d, num=%d", i, nType, nLen, num));

		int nSize = pArg[i].set(nType, nLen, ptr);
		nSize = ((nSize + 3) / 4) * 4;				// make multiple of 4
		ptr += nSize;
		num -= nSize;

		PRINTLOG((Logger::DebugLog, "nSize=%d, num=%d", nSize, num));

		if( num < 0 ) {
			ec.set(true, ErrorCode::Packet, 4);
			break;
		}
	}

	PRINTLOG((Logger::DebugLog, "Parse() return code=%d", ec.get_code()));
	if( ec.is_clean() ) {
		this->m_nArg = nArg;
		this->m_pArg = pArg;
	}
	else {
		delete [] pArg;
	}

	return ec;
}


char* Packet::FindSTX(void)
{
	char* ptr = this->m_pRecvBuf;
	int num = this->m_nRecvNum;
	if( ptr == NULL || num == 0 ) {
		return NULL;
	}

	while( num ) {
		if( ptr[0] == '#' && ptr[1] == '!' && ptr[2] == 'R' ) {
			break;
		}
		--num;
		++ptr;
	}

	if( num == 0 ) {	// not found STX
		delete [] this->m_pRecvBuf;
		this->m_pRecvBuf = NULL;
		this->m_nRecvNum = 0;
		ptr = NULL;
	}
	else if( num == this->m_nRecvNum ) {	// OK
		;
	}
	else {
		memcpy(this->m_pRecvBuf, ptr, num);
		this->m_nRecvNum = num;
		ptr = this->m_pRecvBuf;
	}

	if( ptr != NULL ) {
		this->m_nReqType = ptr[3];
	}

	return ptr;
}


Argument* Packet::GetArg(int& nArg)
{
	nArg = this->m_nArg;
	return this->m_pArg;
}


void Packet::Clear(bool bSend)
{
	if( bSend ) {	// send packet
		this->m_nSendNum = 0;
	}
	else {
		this->m_nRecvNum -= this->m_nLen;
		memcpy(this->m_pRecvBuf, this->m_pRecvBuf + this->m_nLen, this->m_nRecvNum);
		this->m_nLen = 0;
	}
	this->m_nArg = 0;
	delete []  this->m_pArg;
	this->m_pArg = NULL;
}


void Packet::AddArg(const char* pStr)
{
	Argument *arg = this->m_pArg + this->m_nArg;
	arg->set(arg->Char, strlen(pStr)+1, pStr);
	this->m_nArg++;
}


void Packet::SetArgNum(int nNum)
{
	if( this->m_pArg != NULL ) delete []  this->m_pArg;
	this->m_pArg = new Argument[nNum];
}


void Packet::Encode(void)
{
	PRINTLOG((Logger::DebugLog, "Packet::Encode()"));

	// calc send packet size
	//
	int nLen = this->HDR_SIZE;
	nLen += 4;		//arg num
	for( int i=0 ; i<this->m_nArg ; i++ ) {
		nLen += 2 + 2 + this->m_pArg[i].get_size();
	}

	PRINTLOG((Logger::DebugLog, "this->m_nSBufSize=%d, nLen=%d", this->m_nSBufSize, nLen));

	// mem alloc or not
	if( this->m_nSBufSize < nLen ) {
		if( this->m_pSendBuf != NULL ) delete [] this->m_pSendBuf;

		this->m_pSendBuf = new char [nLen];
		this->m_nSBufSize = nLen;
	}

	PRINTLOG((Logger::DebugLog, "Encoding Start..."));

	// encoding
	//
	// STX
	char *ptr = this->m_pSendBuf;
	ptr[0] = '#';
	ptr[1] = '!';
	ptr[2] = 'R';
	ptr[3] = this->m_nReqType;
	ptr += this->HDR_STX;

	// Command
	strcpy(ptr, this->m_pCmd);
	ptr += this->HDR_CMD;

	// length
	* (int *) ptr = nLen;
	ptr += 4;

	PRINTLOG((Logger::DebugLog, "Encoding Body Start..."));

	//
	// Body
	//
	* (int *) ptr = this->m_nArg;
	ptr += 4;
	for( int i=0 ; i<this->m_nArg ; i++ ) {
		ptr = this->m_pArg[i].encode(ptr);
	}

	this->m_nSendNum = ptr - this->m_pSendBuf;

	PRINTLOG((Logger::DebugLog, "Encoding Send Buffer (%d):", this->m_nSendNum));
	PRINTDUMP(this->m_pSendBuf, this->m_nSendNum);
}


void Packet::AddArg(void)
{
	Argument *arg = this->m_pArg + this->m_nArg;
	arg->set(arg->None, 0);
	this->m_nArg++;
}
