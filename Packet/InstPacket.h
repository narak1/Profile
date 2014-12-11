#pragma once
#include "packet.h"
class InstPacket : public Packet
{
	static const char* m_strCmd[];
public:
	enum Command { Error, Hello, Set, Info, SimInit, SimTime, TagInit, TagDown, Send, Run, MaxCmd };
	InstPacket(void);
	~InstPacket(void);
	ErrorCode Parse(void);
	void Encode(void);
};

