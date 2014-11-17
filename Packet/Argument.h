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
	void* m_pValue;
public:
	int set(int nType, int nLen, const char* pBuf=NULL);

	int get_size(void)
	{
		return m_nLen * Argument::DataSize[m_nType];
	}
	char* encode(char* ptr);
};

