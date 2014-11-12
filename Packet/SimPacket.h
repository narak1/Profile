#pragma once
#include "packet.h"
class SimPacket : public Packet
{
	static const char* m_strCmd[];
public:
	enum Command { Add, Del, Read, Write, Save, Load, Remove, NoCmd };
	SimPacket(void);
	~SimPacket(void);
	ErrorCode Parse(void);
private:
	int m_nCmd;
public:
	int GetCommand() { return this->m_nCmd; };
};

