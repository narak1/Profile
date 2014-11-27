#include <afx.h>
#include <WinSock2.h>
#include <WinError.h>
#include "Util.h"


Util::Util(void)
{
}


Util::~Util(void)
{
}


const char* Util::error_str(DWORD dwErrNo)
{
	int nLen = FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL,
				dwErrNo, MAKELANGID(LANG_ENGLISH, 0x1), this->m_buff, this->m_size, NULL);
	this->m_buff[nLen] = 0;
	return this->m_buff;
}


const char* Util::wsa_error_str(int nErrNo)
{
	const char* p = NULL;
	switch( nErrNo ) {
	case WSA_INVALID_HANDLE: p = "6: Specified event object handle is invalid."; break;
	case WSA_NOT_ENOUGH_MEMORY: p = "8: Insufficient memory available."; break;
	case WSA_INVALID_PARAMETER: p = "87: One or more parameters are invalid."; break;
	case WSA_OPERATION_ABORTED: p = "995: Overlapped operation aborted."; break;
	case WSA_IO_INCOMPLETE: p = "996: Overlapped I/O event object not in signaled state."; break;
	case WSA_IO_PENDING:	p = "997: Overlapped operations will complete later."; break;
	case WSAEINTR:			p = "10004: A blocking Windows Sockets 1.1 call was canceled through WSACancelBlockingCall."; break;
	case WSAEBADF:			p = "10009: File handle is not valid."; break;
	case WSAEACCES:			p = "10013: Permission denied."; break;
	case WSAEFAULT:			p = "10014: The addrlen parameter is too small or addr is not a valid part of the user address space."; break;
	case WSAEINVAL:			p = "10022: The listen function was not invoked prior to accept."; break;
	case WSAEMFILE:			p = "10024: The queue is nonempty upon entry to accept and there are no descriptors available."; break;
	case WSAEWOULDBLOCK:	p = "10035: The socket is marked as nonblocking and no connections are present to be accepted."; break;
	case WSAEINPROGRESS:	p = "10036: A blocking Windows Sockets 1.1 call is in progress, or the service provider is still processing a callback function."; break;
	case WSAEALREADY:		p = "10037: Operation already in progress."; break;
	case WSAENOTSOCK:		p = "10038: The descriptor is not a socket."; break;
	case WSAEDESTADDRREQ:	p = "10039: Destination address required."; break;

	case WSAEOPNOTSUPP:		p = "10045: The referenced socket is not a type that supports connection-oriented service."; break;
	case WSAENETDOWN:		p = "10050: The network subsystem has failed."; break;
	case WSAECONNRESET:		p = "10054: An incoming connection was indicated, but was subsequently terminated by the remote peer prior to accepting the call."; break;
	case WSAENOBUFS:		p = "10055: No buffer space is available."; break;
	case WSANOTINITIALISED: p = "10093: A successful WSAStartup call must occur before using this function."; break;
	}
	return p;
}
