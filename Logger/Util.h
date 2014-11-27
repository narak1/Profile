#pragma once
class Util
{
public:
	Util(void);
	~Util(void);
private:
	static const int m_size = 256;
	char m_buff[m_size];
public:
	const char* error_str(DWORD dwErrNo);
	const char* wsa_error_str(int nErrNo);
};
