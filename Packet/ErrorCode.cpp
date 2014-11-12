#include "ErrorCode.h"


ErrorCode::ErrorCode(void)
	: m_nCode(0)
{
}


ErrorCode::~ErrorCode(void)
{
}


ErrorCode ErrorCode::set(bool bError, int nModule, int nCode)
{
	this->m_nCode = (nModule << 16) + nCode;
	if( bError ) {
		this->m_nCode = -this->m_nCode;
	}
	return *this;
}
