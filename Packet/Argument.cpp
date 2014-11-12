#include <iostream>
#include "Argument.h"


//	enum DataType { None, Char, UChar, Bool, Short, Integer, Long, Single, Double, Binary };
const int Argument::DataSize[] = { 0, 1, 1, 1, 2, 4, 8, 4, 8, 1, };

Argument::Argument(void)
	: m_nType(0)
	, m_nLen(0)
	, m_pValue(NULL)
{
}


Argument::~Argument(void)
{
}


int Argument::set(int nType, int nLen, const char* pBuf)
{
	this->m_nType = nType;
	this->m_nLen = nLen;
	this->m_pValue = (void *)pBuf;
	return nLen * Argument::DataSize[nType];
}
