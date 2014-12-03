#pragma once
#include "packet.h"
class SimPacket : public Packet
{
	static const char* m_strCmd[];
public:
	enum Command { Add, Del, Read, Write, Save, Load, Remove, Error, NoCmd, };
	SimPacket(void);
	~SimPacket(void);
	ErrorCode Parse(void);
	void Encode(void);
};
