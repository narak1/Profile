#pragma once
class Argument
{
public:
	Argument(void);
	~Argument(void);
	enum DataType { None, Char, UChar, Bool, Short, Integer, Long, Single, Double, Binary };
private:
	static const int DataSize[];
	int m_nType;
	int m_nLen;
	const void* m_pValue;
public:
	int set(int nType, int nLen, const void* pBuf=NULL);

	int get_size(void)
	{
		return m_nLen * Argument::DataSize[m_nType];
	}
	int get_type() { return this->m_nType; };
	int get_len() { return this->m_nLen; };
	const void* get_value() { return this->m_pValue; };
	char* encode(char* ptr);
};
