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
private:
	int m_nCmd;
public:
	int GetCommand() { return this->m_nCmd; };
	void SetCommand(int nCmd) { this->m_nCmd = nCmd; };
void Encode(void);
};

