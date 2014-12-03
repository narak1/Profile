
// SIMBridgeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SIMBridge.h"
#include "SIMBridgeDlg.h"
#include "MySock.h"
#include "SocketManager.h"

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

CSIMBridgeDlg *g_pDlg;


CSIMBridgeDlg::CSIMBridgeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSIMBridgeDlg::IDD, pParent)
	, m_Util(NULL)
	, m_bRun(false)
	, m_pListenSock(NULL)
	, m_SockMgr(NULL)
{
	::g_pDlg = this;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_strHostname = _T("");
	m_nServicePort = 0;
	m_strSimTime = _T("");
	m_strOutput = _T("");
	m_nPriority = 0;
	m_strServerType = _T("");
}

void CSIMBridgeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_HOSTNAME, m_strHostname);
	DDX_Text(pDX, IDC_SERVICE_PORT, m_nServicePort);
	DDV_MinMaxInt(pDX, m_nServicePort, 0, 65535);
	DDX_Text(pDX, IDC_SIMULATION_TIME, m_strSimTime);
	DDX_Text(pDX, IDC_OUTPUT, m_strOutput);
	DDX_Control(pDX, IDC_OUTPUT, m_Output);
	DDX_Control(pDX, IDC_BUTTON1, m_BtnStart);
	DDX_Text(pDX, IDC_PRIORITY, m_nPriority);
	DDV_MinMaxInt(pDX, m_nPriority, 0, 2147483647);
	DDX_CBString(pDX, IDC_SERVER_TYPE, m_strServerType);
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
	g_Logger.Init("SIMBridge_%Y-%m-%d.log");

	this->m_Util = new Util();

	SYSTEMTIME lt;
	::GetLocalTime(&lt);
	PRINTLOG((Logger::InfoLog, "Start... %02d:%02d:%02d.%d", lt.wHour, lt.wMinute, lt.wSecond, lt.wMilliseconds));

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


void CSIMBridgeDlg::PrintInfo(const char* fmt, ...)
{
	this->UpdateData();

	va_list arg;
	va_start( arg, fmt );
	CString strTmp;
	strTmp.FormatV(fmt, arg);
	va_end( arg );

	PRINTLOG((Logger::InfoLog, "%s", strTmp));
}


void CSIMBridgeDlg::PrintOut(const char* fmt, ...)
{
	this->UpdateData();

	va_list arg;
	va_start( arg, fmt );
	CString strTmp;
	strTmp.FormatV(fmt, arg);
	va_end( arg );

	PRINTLOG((Logger::ErrorLog, "%s", strTmp));

	if( !this->m_strOutput.IsEmpty() ) 
		this->m_strOutput += "\r\n";
	this->m_strOutput += strTmp;

	this->UpdateData(FALSE);
	this->m_Output.LineScroll(this->m_Output.GetLineCount());
}


static CString strSection = "Config";
static CString strHost = "Host";
static CString strPort = "Port";
static CString strType = "Type";
static CString strPrio = "Prio";

void CSIMBridgeDlg::ProfileLoad(void)
{
	this->m_strHostname = theApp.GetProfileStringA(strSection, strHost, this->m_strHostname);
	this->m_nServicePort = theApp.GetProfileIntA(strSection, strPort, this->m_nServicePort);
	this->m_strServerType = theApp.GetProfileStringA(strSection, strType, this->m_strServerType);
	this->m_nPriority = theApp.GetProfileIntA(strSection, strPrio, this->m_nPriority);
}


void CSIMBridgeDlg::ProfileSave(void)
{
	theApp.WriteProfileString(strSection, strHost, this->m_strHostname);
	theApp.WriteProfileInt(strSection, strPort, this->m_nServicePort);
	theApp.WriteProfileString(strSection, strType, this->m_strServerType);
	theApp.WriteProfileInt(strSection, strPrio, this->m_nPriority);
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
	PRINTLOG((Logger::InfoLog, "Starting.. Port=%d", this->m_nServicePort));

	CMySock *pSock = new CMySock(this);

	BOOL flag = pSock->Create(this->m_nServicePort);
	if( flag == 0 ) {
		DWORD dwError = ::GetLastError();
		this->PrintOut("Socket Create(%d) Error! ErrorNo=%08X", this->m_nServicePort, dwError);
		delete pSock;
		return;
	}

	flag = pSock->Listen();
	if( flag == 0 ) {
		DWORD dwError = ::GetLastError();
		this->PrintOut("Socket Listen() Error! ErrorNo=%08X", dwError);
		delete pSock;
		return;
	}

	this->m_pListenSock = pSock;
	this->PrintOut("Listening.. Port=%d", this->m_nServicePort);
}


void CSIMBridgeDlg::StopService(void)
{
	PRINTLOG((Logger::InfoLog, "Stopping.. Port=%d", this->m_nServicePort));

	if( this->m_pListenSock != NULL ) {
		this->m_pListenSock->Close();
		delete this->m_pListenSock;
		this->PrintOut("Stopped.. Port=%d", this->m_nServicePort);
	}
}


void CSIMBridgeDlg::Init(void)
{
	this->m_SockMgr = new SocketManager();
}


void CSIMBridgeDlg::OnOK()
{
	PRINTLOG((Logger::DebugLog, "OnOK()"));
	this->StopService();

	delete this->m_Util;
	PRINTLOG((Logger::DebugLog, "Finish OnOK()"));

	g_Logger.Finish();

	CDialog::OnOK();
}


void CSIMBridgeDlg::OnBnClickedOk()
{
	PRINTLOG((Logger::DebugLog, "OnBnClickedOk()"));
	this->OnOK();
}
