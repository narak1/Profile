#include <cstring>
#include "InstPacket.h"
#include "ErrorCode.h"

//	enum Command { Error, Hello, Set, Info, SimInit, SimTime, TagInit, TagDown, Send, Run, MaxCmd };
const char* InstPacket::m_strCmd[] = 
{ "Error", "Hello", "Set", "Info", "SimInit", "SimTime", "TagInit", "TagDown", "Send", "Run", "" };


InstPacket::InstPacket(void)
{
}


InstPacket::~InstPacket(void)
{
}


ErrorCode InstPacket::Parse(void)
{
	ErrorCode ec = Packet::Parse();
	if( ! ec.is_clean() ) {
		return ec;
	}

	// find command
	this->m_nCmd = this->MaxCmd;
	for( int i=0 ; i<this->MaxCmd ; i++ ) {
		if( strcmp(this->m_pCmd, this->m_strCmd[i]) == 0 ) {
			this->m_nCmd = i;
			break;
		}
	}

	if( this->m_nCmd == this->MaxCmd ) {
		return ec.set(true, ErrorCode::Packet, 5);
	}

	return ec;
}


void InstPacket::Encode(void)
{
	this->m_pCmd = this->m_strCmd[this->m_nCmd];
	Packet::Encode();
}
