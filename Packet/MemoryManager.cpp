#include <cstdio>
#include <cstdlib>
#include "MemoryManager.h"


MemoryManager::MemoryManager(void)
{
	this->m_nBuf = BlockSize;
	this->m_pBuf = (char *) malloc(BlockSize);
	this->m_nFree = this->m_nBuf;
	this->m_pCur = this->m_pBuf;
}


MemoryManager::~MemoryManager(void)
{
	free(this->m_pBuf);
}


void MemoryManager::extend(int nSize)
{
	if( nSize % BlockSize ) {
		nSize = (nSize / BlockSize + 1) * BlockSize;
	}
	this->m_nBuf += nSize;
	this->m_nFree += nSize;
	this->m_pBuf = (char *) realloc(this->m_pBuf, this->m_nBuf);
}


void* MemoryManager::alloc(int nVal)
{
	if( this->m_nFree < 4 ) {
		this->extend();
	}

	void *ret = this->m_pCur;

	* (int *) this->m_pCur = nVal;
	this->m_pBuf += 4;
	this->m_nFree -= 4;
	
	return ret;
}


void MemoryManager::clear(void)
{
	this->m_pCur = this->m_pBuf;
	this->m_nFree = this->m_nBuf;
}
