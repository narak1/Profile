#pragma once

#include <cstdlib>

class ErrorCode
{
public:
	ErrorCode(void);
	~ErrorCode(void);
	enum ModuleType { NoModule, Packet, Socket };
private:
	int m_nCode;
public:
	ErrorCode set(bool bError, int nModule, int nCode);

	int is_clean(void)
	{
		return this->m_nCode == 0;
	}

	bool is_error(void)
	{
		return this->m_nCode < 0;
	}

	int get_module(void)
	{
		return (abs(this->m_nCode) >> 16);
	}

	int get_code(void)
	{
		return (abs(this->m_nCode) & 0xFFFF);
	}
};
