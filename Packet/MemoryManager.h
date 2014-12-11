#pragma once
class MemoryManager
{
public:
	MemoryManager(void);
	~MemoryManager(void);
private:
	static const int BlockSize = 1024;
	char* m_pBuf;
	char* m_pCur;
	int m_nBuf;
	int m_nFree;
public:
	void* alloc(int nVal);
	void extend(int nSize=BlockSize);
	void clear(void);
};

