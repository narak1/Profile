#include "ErrorCode.h"


ErrorCode::ErrorCode(void)
	: m_nCode(0), m_pStr(NULL)
{
}


ErrorCode::~ErrorCode(void)
{
}


ErrorCode ErrorCode::set(bool bError, int nModule, int nCode, const char *pStr)
{
	this->m_nCode = (nModule << 16) + nCode;
	if( bError ) {
		this->m_nCode = -this->m_nCode;
	}
	this->m_pStr = pStr;
	return *this;
}
