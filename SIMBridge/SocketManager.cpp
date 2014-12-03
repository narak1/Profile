#include "StdAfx.h"
#include "SIMBridge.h"
#include "SIMBridgeDlg.h"
#include "SocketManager.h"
#include "MySock.h"

#include "../Logger/Logger.h"

#pragma warning(disable:4996)

SocketManager::SocketManager(void) : m_nNum(0)
{
	for( int i=0 ; i<this->MaxSock ; i++ ) {
		this->m_pSock[i] = NULL;
		this->m_strName[i] = NULL;
	}
}


SocketManager::~SocketManager(void)
{
}


bool SocketManager::Add(CMySock* pSock)
{
	if( this->m_nNum >= this->MaxSock ) {
		PRINTOUT(("SocketManager buffer full. Max=%d", this->MaxSock));
		return false;
	}

	this->m_pSock[this->m_nNum] = pSock;
	++this->m_nNum;
	return true;
}


bool SocketManager::SetName(CMySock* pSock, const char* strName)
{
	for( int i=0 ; i<this->m_nNum ; i++ ) {
		if( this->m_pSock[i] == pSock ) {
			if( this->m_strName[i] )
				free(this->m_strName[i]);
			this->m_strName[i] = strdup(strName);
			return true;
		}
	}
	return false;
}


bool SocketManager::SetName(int idx, const char* name)
{
	if( idx < this->m_nNum ) {
		if( this->m_strName[idx] )
			free(this->m_strName[idx]);
		this->m_strName[idx] = strdup(name);
		return true;
	}
	return false;
}


const char* SocketManager::GetName(CMySock* pSock)
{
	for( int i=0 ; i<this->m_nNum ; i++ ) {
		if( this->m_pSock[i] == pSock ) {
			return this->m_strName[i];
		}
	}
	return NULL;
}


const char* SocketManager::GetName(int idx)
{
	if( idx < this->m_nNum )
		return this->m_strName[idx];
	else
		return NULL;
}


CMySock* SocketManager::GetSock(const char* name)
{
	for( int i=0 ; i<this->m_nNum ; i++ ) {
		if( strcmp(this->m_strName[i], name) == 0 ) {
			return this->m_pSock[i];
		}
	}
	return NULL;
}


CMySock* SocketManager::GetSock(int nIdx)
{
	if( nIdx < this->m_nNum )
		return this->m_pSock[nIdx];
	else
		return NULL;
}


int SocketManager::GetIndex(const char* name)
{
	for( int i=0 ; i<this->m_nNum ; i++ ) {
		if( strcmp(this->m_strName[i], name) == 0 ) {
			return i;
		}
	}
	return -1;
}


int SocketManager::GetIndex(CMySock* pSock)
{
	for( int i=0 ; i<this->m_nNum ; i++ ) {
		if( this->m_pSock[i] == pSock ) {
			return i;
		}
	}
	return -1;
}
