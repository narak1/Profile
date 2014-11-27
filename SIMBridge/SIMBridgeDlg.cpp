
// SIMBridgeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SIMBridge.h"
#include "SIMBridgeDlg.h"
#include "afxdialogex.h"

#include "PeerInfo.h"

#include "../Packet/InstPacket.h"
#include "../Packet/SimPacket.h"
#include "../Logger/Logger.h"
#include "../Logger/Util.h"

#include "../Packet/ErrorCode.h"

#pragma warning(disable:4996)

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSIMBridgeDlg dialog




CSIMBridgeDlg::CSIMBridgeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSIMBridgeDlg::IDD, pParent)
	, m_nThread(0)
	, m_Util(NULL)
	, m_bRun(false)
	, m_nPeerInfo(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_strHostname = _T("");
	m_nServicePort = 0;
	m_strSimTime = _T("");
	m_ListenSock = INVALID_SOCKET;
	m_IOCP = INVALID_HANDLE_VALUE;
	m_strOutput = _T("");
	for( int i=0 ; i<MaxThread ; i++ ) {
		this->m_hThread[i] = INVALID_HANDLE_VALUE;
	}
	for( int i=0 ; i<MaxPeerInfo ; i++ ) {
		this->m_pPeerInfo[i] = NULL;
	}
}

void CSIMBridgeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_HOSTNAME, m_strHostname);
	DDX_Text(pDX, IDC_SERVICE_PORT, m_nServicePort);
	DDX_Text(pDX, IDC_SIMULATION_TIME, m_strSimTime);
	DDX_Text(pDX, IDC_OUTPUT, m_strOutput);
	DDX_Control(pDX, IDC_OUTPUT, m_Output);
	DDX_Control(pDX, IDC_BUTTON1, m_BtnStart);
}

BEGIN_MESSAGE_MAP(CSIMBridgeDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CSIMBridgeDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDOK, &CSIMBridgeDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CSIMBridgeDlg message handlers

BOOL CSIMBridgeDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	this->m_Util = new Util();

	SYSTEMTIME lt;
	::GetLocalTime(&lt);
	CString strLogFile;
	strLogFile.Format("SIMBridge_%4d-%02d-%02d.log", lt.wYear, lt.wMonth, lt.wDay);

	g_Logger.Init(strLogFile);
	PRINTLOG(("Start... %02d:%02d:%02d.%d", lt.wHour, lt.wMinute, lt.wSecond, lt.wMilliseconds));

	DWORD nSize = MAX_COMPUTERNAME_LENGTH;
	BOOL flag = ::GetComputerNameA(this->m_strHostname.GetBuffer(MAX_COMPUTERNAME_LENGTH + 1), &nSize);
	this->m_strHostname.ReleaseBuffer();

	this->ProfileLoad();
	this->UpdateData(FALSE);

	this->Init();

	if( this->m_nServicePort > 0 ) {
		this->m_bRun = true;
		this->StartService();
		this->m_BtnStart.SetWindowTextA("Stop");
	}

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSIMBridgeDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSIMBridgeDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CSIMBridgeDlg::PrintError(const char* fmt, ...)
{
	this->UpdateData();

	va_list arg;
	va_start( arg, fmt );
	CString strTmp;
	strTmp.FormatV(fmt, arg);
	va_end( arg );

	PRINTLOG(("%s", strTmp));

	if( !this->m_strOutput.IsEmpty() ) 
		this->m_strOutput += "\r\n";
	this->m_strOutput += strTmp;

	this->UpdateData(FALSE);
	this->m_Output.LineScroll(this->m_Output.GetLineCount());
}


static CString strSection = "Config";
static CString strHost = "Host";
static CString strPort = "Port";

void CSIMBridgeDlg::ProfileLoad(void)
{
	this->m_strHostname = theApp.GetProfileStringA(strSection, strHost, this->m_strHostname);
	this->m_nServicePort = theApp.GetProfileIntA(strSection, strPort, this->m_nServicePort);
}


void CSIMBridgeDlg::ProfileSave(void)
{
	theApp.WriteProfileString(strSection, strPort, this->m_strHostname);
	theApp.WriteProfileInt(strSection, strPort, this->m_nServicePort);
}


void CSIMBridgeDlg::OnBnClickedButton1()
{
	if( this->m_bRun == false ) {
		this->m_bRun = true;
		this->UpdateData();
		this->ProfileSave();
		this->StartService();
		this->m_BtnStart.SetWindowTextA("Stop");
	}
	else {
		this->m_bRun = false;
		this->StopService();
		this->m_BtnStart.SetWindowTextA("Start");
	}
}


void CSIMBridgeDlg::StartService(void)
{
	UINT WINAPI ListenFunction( LPVOID pArg );
	PRINTLOG(("Starting.. Port=%d", this->m_nServicePort));

	// Listen Thread...
	DWORD dwThreadID;
	this->m_hListenThread = (HANDLE) _beginthreadex( NULL, 0, ListenFunction, this, 0, (unsigned *)&dwThreadID );
	if( this->m_hListenThread == NULL ) {
		this->PrintError("_beginthreadex() errno=%d:%s", errno, strerror(errno));
		return;
	}
	PRINTLOG(("Thread Created: ID=%d, Handle=%p", dwThreadID, this->m_hListenThread));
}


void CSIMBridgeDlg::StopService(void)
{
	Util util;

	PRINTLOG(("Stopping.. Port=%d", this->m_nServicePort));

	// listen socket is local connection need. because blocked at accept()

	// recieve socket is ??? how can do ?
	// Work Thread Finishing
	//
	DWORD dwSize = 0;
	LPOVERLAPPED lpOverlapped = NULL;
	for( int i=0 ; i<this->m_nThread ; i++ ) {
		BOOL flag = PostQueuedCompletionStatus(this->m_IOCP, dwSize, NULL, NULL);
		if( flag == FALSE ) {
			DWORD dwError = ::GetLastError();
			PRINTLOG(("PostQueuedCompletionStatus(,,,) Error! ErrorNo=%08X:%s", dwError, util.error_str(dwError)));
		}
	}
}


UINT WINAPI ListenFunction( LPVOID pArg )
{
	PRINTLOG(("Listen Thread Running: ID=%d", GetCurrentThreadId()));

	CSIMBridgeDlg *pDlg = (CSIMBridgeDlg *) pArg;
	pDlg->ListenFunction();

	PRINTLOG(("Listen Thread Finished: ID=%d", GetCurrentThreadId()));
	_endthreadex( 0 );
	return 0;
}


UINT WINAPI WorkFunction( LPVOID pArg )
{
	PRINTLOG(("Work Thread Running: ID=%d", GetCurrentThreadId()));

	CSIMBridgeDlg *pDlg = (CSIMBridgeDlg *) pArg;
	pDlg->WorkFunction();

	PRINTLOG(("Work Thread Finished: ID=%d", GetCurrentThreadId()));
	_endthreadex( 0 );
	return 0;
}


void CSIMBridgeDlg::Init(void)
{
	UINT WINAPI WorkFunction( LPVOID pCompletionPortIO );

	this->m_IOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	if( this->m_IOCP == NULL ) {
		DWORD dwError = ::GetLastError();
		this->PrintError("CreateIoCompletionPort() Error=%08X:%s", dwError, this->m_Util->error_str(dwError));
		return;
	}
	PRINTLOG(("CreateIoCompletionPort()=%p", this->m_IOCP));

	SYSTEM_INFO systemInfo;
	GetSystemInfo( &systemInfo );

	this->m_nThread = 3;// systemInfo.dwNumberOfProcessors + 5;		// process num + alpha
	if( this->m_nThread >= this->MaxThread )
		this->m_nThread = this->MaxThread;
	PRINTLOG(("NumberOfProcessors=%d, Thread Num=%d", systemInfo.dwNumberOfProcessors, this->m_nThread));

	for( int i=0 ; i<this->m_nThread ; i++ )					// index 0 use Listen Thread
	{
		DWORD dwThreadID;
		this->m_hThread[i] = (HANDLE) _beginthreadex( NULL, 0, WorkFunction, this, 0, (unsigned *)&dwThreadID );
		if( this->m_hThread[i] == NULL ) {
			this->PrintError("_beginthreadex() errno=%d:%s", errno, strerror(errno));
			return;
		}
		PRINTLOG(("Thread Created: ID=%d, Handle=%p", dwThreadID, this->m_hThread[i]));
	}
}


void CSIMBridgeDlg::ListenFunction(void)
{
	Util util;

	SOCKET sock = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (sock == INVALID_SOCKET) {
		int nError = ::WSAGetLastError();
		PRINTLOG(("Overlapped WSASocket() Error! WSAErrorNo=%d:%s", nError, util.wsa_error_str(nError)));
		return;
	}

	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(this->m_nServicePort);
	int nRet = bind(sock, (SOCKADDR *)&addr, sizeof(addr));
	if( nRet == SOCKET_ERROR ) {
		int nError = ::WSAGetLastError();
		PRINTLOG(("bind(PortNo=%d) Error! WSAErrorNo=%d:%s", this->m_nServicePort, nError, util.wsa_error_str(nError)));
		closesocket(sock);
		return;
	}

	nRet = listen(sock, SOMAXCONN);
	if( nRet == SOCKET_ERROR ) {
		int nError = ::WSAGetLastError();
		PRINTLOG(("listen(,backlog=%x) Error! WSAErrorNo=%d:%s", SOMAXCONN, nError, util.wsa_error_str(nError)));
		closesocket(sock);
		return;
	}

	this->PrintError("Listening.. Port=%d", this->m_nServicePort);

	// while loop --> thread need...
	SOCKET client_sock;
	int nSize = sizeof(addr);
	while( 1 )
	{
		client_sock = accept(sock, (SOCKADDR *)&addr, &nSize);		// Blocking Position

		PRINTLOG(("accept(%d,,)=%d ; Num of peer=%d", sock, client_sock, this->m_nPeerInfo));

		if( this->m_bRun == false ) {	// Service Stoping....
			break;
		}

		if( client_sock == INVALID_SOCKET ) {
			int nError = ::WSAGetLastError();
			PRINTLOG(("accept() Error! WSAErrorNo=%d:%s", sock, nError, util.wsa_error_str(nError)));
			continue;
		}

		if( this->m_nPeerInfo == this->MaxPeerInfo ) {
			PRINTLOG(("Too many peers. num=%d. skipped!", this->MaxPeerInfo));
			continue;
		}

		PeerInfo *pi = new PeerInfo(client_sock);
		pi->m_Packet = new Packet();					// Packet 이 어떤 클라이언트인지에 따라 처리되어야 함..
		pi->m_wsaBuf.len = SockBuffSize;
		pi->m_wsaBuf.buf = new CHAR [SockBuffSize];
		SecureZeroMemory((PVOID)&(pi->m_wsaOverlapped), sizeof(WSAOVERLAPPED));
PRINTLOG(("SecureZeroMemory(%p,%d) called", &(pi->m_wsaOverlapped), sizeof(WSAOVERLAPPED)));

		this->m_pPeerInfo[this->m_nPeerInfo] = pi;
		++this->m_nPeerInfo;

		HANDLE hRet = CreateIoCompletionPort((HANDLE)client_sock, this->m_IOCP, (ULONG_PTR) pi, INFINITE);
		if( hRet == NULL ) {
			DWORD dwError = ::GetLastError();
			PRINTLOG(("CreateIoCompletionPort(,,,) Error! ErrorNo=%08X:%s", dwError, util.error_str(dwError)));
			continue;
		}
PRINTLOG(("CreateIoCompletionPort(%d,%p,%p,INFINITE)=%p called", client_sock, this->m_IOCP, pi, hRet));

		// initial WSARecv call need
		char buf[BUFSIZ];
		WSABUF wsaBuf[1];
		wsaBuf[0].buf = buf;
		wsaBuf[0].len = BUFSIZ;
		DWORD dwRecv = 0;
		DWORD dwFlag = 0;
		WSAOVERLAPPED wsaOverlapped;
		SecureZeroMemory(&wsaOverlapped, sizeof(WSAOVERLAPPED));
PRINTLOG(("dwRecv=%u, dwFlag=%u", dwRecv, dwFlag));
PRINTLOG(("calling WSARecv(%d,%p,1,,,%p,NULL)", client_sock, &(pi->m_wsaBuf), &(pi->m_wsaOverlapped)));
		nRet = WSARecv(client_sock, &(pi->m_wsaBuf), 1, &dwRecv, &dwFlag, &(pi->m_wsaOverlapped), NULL);
//PRINTLOG(("calling WSARecv(%d,%p,1,,,%p,NULL)", client_sock, wsaBuf, &wsaOverlapped));
		//nRet = WSARecv(client_sock, wsaBuf, 1, &dwRecv, &dwFlag, &wsaOverlapped, NULL);
PRINTLOG(("WSARecv()=%d", nRet));
		if( nRet == SOCKET_ERROR ) {
			int nError = ::WSAGetLastError();
			PRINTLOG(("WSARecv(%d,,,,) Error! WSAErrorNo=%d:%s", client_sock, nError, util.wsa_error_str(nError)));
		}
		else {
			PRINTLOG(("WSARecv(client_sock=%d,buf,1,nRecv,flag,overlapped=%p", client_sock, &wsaOverlapped));
			PRINTLOG(("dwRecv=%u, dwFlag=%u", dwRecv, dwFlag));
		}
	}

	PRINTLOG(("Listen Thread Loop Finished"));

	nRet = closesocket(client_sock);
	if( nRet == SOCKET_ERROR ) {
		int nError = ::WSAGetLastError();
		PRINTLOG(("closesocket(%d) Error! WSAErrorNo=%d:%s", client_sock, nError, util.wsa_error_str(nError)));
	}

	nRet = closesocket(sock);
	if( nRet == SOCKET_ERROR ) {
		int nError = ::WSAGetLastError();
		PRINTLOG(("closesocket(%d) Error! WSAErrorNo=%d:%s", sock, nError, util.wsa_error_str(nError)));
	}
}


void CSIMBridgeDlg::WorkFunction(void)
{
	Util util;

	while( 1 ) {
		PRINTLOG(("Loop Start..."));

		DWORD dwSize = 0;
		ULONG_PTR lpPtr = NULL;
		LPOVERLAPPED lpOverlapped = NULL;
		BOOL flag = GetQueuedCompletionStatus(this->m_IOCP, &dwSize, &lpPtr, &lpOverlapped, INFINITE);	// Bolcking Position

		PeerInfo *pi = (PeerInfo *) lpPtr;

		PRINTLOG(("dwSize=%d, lpPtr=%p, lpOverlapped=%d, pi->overlapped=%p", dwSize, lpPtr, lpOverlapped, &(pi->m_wsaOverlapped)));
PRINTDUMP(pi->m_wsaBuf.buf, dwSize);
PRINTLOG(("DUmp DOne.."));

		if( this->m_bRun == false ) {	// Service Stoping....
			break;
		}

		if( flag == FALSE ) {
			DWORD dwError = ::GetLastError();
			this->PrintError("GetQueuedCompletionStatus() Error=%08X:%s", dwError, this->m_Util->error_str(dwError));
			continue;
		}

		DWORD dwFlag = 0;
		DWORD dwRecv = dwSize;
		//rc = WSARecv(ConnSocket, &DataBuf, 1, &RecvBytes, &Flags, &RecvOverlapped, NULL);
PRINTLOG(("calling WSARecv(%d,%p,1,,,%p,NULL)", pi->m_sock, &(pi->m_wsaBuf), &(pi->m_wsaOverlapped)));
		int nRet = WSARecv(pi->m_sock, &(pi->m_wsaBuf), 1, &dwRecv, &dwFlag, &(pi->m_wsaOverlapped), NULL);
PRINTLOG(("WSARecv()=%d", nRet));
PRINTLOG(("dwRecv=%u, dwFlag=%u", dwRecv, dwFlag));
		if( nRet == SOCKET_ERROR ) {
			int nError = ::WSAGetLastError();
			PRINTLOG(("WSARecv(%d,,,) Error! WSAErrorNo=%d:%s", pi->m_sock, nError, util.wsa_error_str(nError)));
		}
		else if( nRet == 0 ) {
			// WSARecv returns zero. In this case, the completion routine will have already been scheduled
			//  to be called once the calling thread is in the alertable state. 
			PRINTLOG(("WSARecv(%d,,,)=0", pi->m_sock));
			continue;
		}
PRINTDUMP(pi->m_wsaBuf.buf, dwRecv);
PRINTLOG(("Dump Done.."));
PRINTLOG(("pi->m_Packet=%p..pi->sock=%d", pi->m_Packet , pi->m_sock));

		// Packet Processing
		//
		pi->m_Packet->AddRecv(pi->m_wsaBuf.buf, dwRecv);
PRINTLOG(("pi->m_Packet->AddRecv(,%d)", dwRecv));

		ErrorCode ec = pi->m_Packet->Parse();
PRINTLOG(("pi->m_Packet->Parse()=%d", ec.get_code()));

		if( ec.is_clean() ) {
			pi->m_Packet->Clear();


			pi->m_Packet->SetRequestType(Packet::ForResponse);
			pi->m_Packet->SetCommand("good");
			pi->m_Packet->SetArgNum(1);
			pi->m_Packet->AddArg();	// None

			pi->m_Packet->Encode();

PRINTLOG(("this->m_Packet.Encode() Finish"));

			// get send buffer
			// send(buf, num);

			int nData = 0;
			const char *ptr = pi->m_Packet->GetSend(nData);
			pi->m_wsaBuf.buf = (CHAR *) ptr;
			pi->m_wsaBuf.len = nData;
			nRet = WSASend(pi->m_sock, &pi->m_wsaBuf, 1, (LPDWORD) &nData, dwFlag, &pi->m_wsaOverlapped, NULL);
			if( nRet == SOCKET_ERROR ) {
				int nError = ::WSAGetLastError();
				PRINTLOG(("WSASend(%,,,) Error! WSAErrorNo=%d:%s", pi->m_sock, nError, util.wsa_error_str(nError)));
			}

		}
	}

	PRINTLOG(("I/O Thread Loop Finished"));
}


void CSIMBridgeDlg::OnOK()
{
	PRINTLOG(("OnOK()"));
	this->StopService();

	PRINTLOG(("Listen Thread Waiting...."));
	::WaitForSingleObject( this->m_hListenThread, INFINITE );
	::CloseHandle( this->m_hListenThread );

	// close handle
	for( int i=0 ; i<this->m_nThread ; i++ ) {
		PRINTLOG(("%d Work Thread Waiting....", i));
		::WaitForSingleObject( this->m_hThread[i], INFINITE );
		::CloseHandle( this->m_hThread[i] );
	}
	::CloseHandle( this->m_IOCP );

	delete this->m_Util;
	PRINTLOG(("Finish OnOK()"));

	CDialog::OnOK();
}


void CSIMBridgeDlg::OnBnClickedOk()
{
	this->OnOK();
}
