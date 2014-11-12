#include <cstring>
#include "SimPacket.h"
#include "ErrorCode.h"

//	enum Command { Add, Del, Read, Write, Save, Load, Remove, NoCmd };
const char* SimPacket::m_strCmd[] = { "Add", "Del", "Read", "Write", "Save", "Load", "Remove", "" };


SimPacket::SimPacket(void)
	: m_nCmd(0)
{
}


SimPacket::~SimPacket(void)
{
}


ErrorCode SimPacket::Parse(void)
{
	ErrorCode ec = Packet::Parse();
	if( ! ec.is_clean() ) {
		return ec;
	}

	// find command
	for( int i=0 ; i<this->NoCmd ; i++ ) {
		if( strcmp(this->m_pCmd, this->m_strCmd[i]) == 0 ) {
			this->m_nCmd = i;
			break;
		}
	}

	if( this->m_nCmd == this->NoCmd ) {
		return ec.set(true, ErrorCode::Packet, 5);
	}

	return ec;
}
