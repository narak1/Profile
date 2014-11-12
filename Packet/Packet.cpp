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
	if( this->m_nRecvNum + nLen > this->m_nRBufSize )
	{
		char* pOld = this->m_pRecvBuf;
		this->m_nRBufSize += nLen;
		this->m_pRecvBuf = new char [this->m_nRBufSize];
		::memcpy(this->m_pRecvBuf, pOld, this->m_nRecvNum);
		delete [] pOld;
	}
	::memcpy(this->m_pRecvBuf + this->m_nRecvNum , pBuf, nLen);
	this->m_nRecvNum += nLen;
}


void Packet::SubSend(int nLen)
{
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
	PRINTLOG(("RecvData (%d):", this->m_nRecvNum));
	PRINTDUMP(this->m_pRecvBuf, this->m_nRecvNum);

	ErrorCode ec;

	if( this->m_nRecvNum < Packet::HDR_SIZE ) {
		PRINTLOG(("Not complete header"));
		return ec.set(false, ErrorCode::Packet, 1);
	}

	char *ptr = this->FindSTX();
	if( ptr == NULL ) {
		PRINTLOG(("Not found STX!"));
		return ec.set(true, ErrorCode::Packet, 2);
	}

	ptr += this->HDR_STX;
	this->m_pCmd = ptr;

	ptr += this->HDR_CMD;
	this->m_nLen = * (int *) ptr;

	ptr += this->HDR_SIZE;						// packet body pointer

	//=== End of header

	// packet size check
	if( this->m_nRecvNum <  this->m_nLen ) {
		PRINTLOG(("Not complete packet"));
		return ec.set(false, ErrorCode::Packet, 3);
	}

	int num = this->m_nLen - this->HDR_SIZE;	// packet body length

	// parse argement
	int nArg = * (int *) ptr;
	Argument* pArg = new Argument [this->m_nArg];
	ptr += 4;
	num -= 4;

	for( int i=0 ; i<nArg ; i++ )
	{
		short nType = * (short *) ptr;
		ptr += 2;
		num -= 2;
		short nLen = * (short *) ptr;
		ptr += 2;
		num -= 2;
		int nSize = pArg[i].set(nType, nLen, ptr);
		nSize = ((nSize + 3) / 4) * 4;				// make multiple of 4
		ptr += nSize;
		num -= nSize;
		if( num < 0 ) {
			ec.set(true, ErrorCode::Packet, 4);
			break;
		}
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
	else {
		this->m_nReqType = Undef;
	}

	return ptr;
}


Argument* Packet::GetArg(int& nArg)
{
	nArg = this->m_nArg;
	return this->m_pArg;
}
