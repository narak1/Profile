#pragma once

class Packet;

class PeerInfo
{
public:
	PeerInfo(void);
	PeerInfo(SOCKET sock) : m_sock(sock) { ; };
	~PeerInfo(void);
	SOCKET m_sock;
	WSABUF m_wsaBuf;
	WSAOVERLAPPED m_wsaOverlapped;
	Packet* m_Packet;
};

/*
typedef struct __WSABUF {
  u_long   len;
  char FAR *buf;
} WSABUF, *LPWSABUF;

typedef struct _WSAOVERLAPPED {
  ULONG_PTR Internal;
  ULONG_PTR InternalHigh;
  union {
    struct {
      DWORD Offset;
      DWORD OffsetHigh;
    };
    PVOID  Pointer;
  };
  HANDLE    hEvent;
} WSAOVERLAPPED, *LPWSAOVERLAPPED;
*/
