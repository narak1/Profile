
// DummySimDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DummySim.h"
#include "DummySimDlg.h"
#include "afxdialogex.h"

#include "ServerSock.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDummySimDlg dialog




CDummySimDlg::CDummySimDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDummySimDlg::IDD, pParent)
	, m_pLogFile(NULL)
	, m_ppTag(NULL)
	, m_nTag(0)
	, m_pListenSock(NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_nSvcPort = 0;
	m_strOutput = _T("");
}

void CDummySimDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_SVC_PORT, m_nSvcPort);
	DDV_MinMaxInt(pDX, m_nSvcPort, 0, 65535);
	DDX_Text(pDX, IDC_OUTPUT, m_strOutput);
	DDX_Control(pDX, IDC_OUTPUT, m_Output);
	DDX_Control(pDX, IDC_TAG_LIST, m_TagList);
}

BEGIN_MESSAGE_MAP(CDummySimDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_START, &CDummySimDlg::OnBnClickedBtnStart)
	ON_BN_CLICKED(IDOK, &CDummySimDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CDummySimDlg message handlers

BOOL CDummySimDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	SYSTEMTIME lt;
	::GetLocalTime(&lt);
	CString strDateTime;
	strDateTime.Format("%4d-%02d-%02d_%02d%02d%02d.txt", lt.wYear, lt.wMonth, lt.wDay, lt.wHour, lt.wMinute, lt.wSecond);

	this->m_pLogFile = new CStdioFile();
	UINT nFlag = CFile::modeWrite | CFile::modeCreate | CFile::modeNoTruncate | CFile::typeText | CFile::shareDenyNone;
	BOOL flag = this->m_pLogFile->Open(strDateTime, nFlag);
	if( flag == FALSE ) {
		delete this->m_pLogFile;
		this->m_pLogFile = NULL;
		this->PrintOut("log file open failed!");
	}

	this->PrintTime("Current Time");

	this->ProfileLoad();
	this->UpdateData(FALSE);

	this->m_TagList.InsertColumn(0, "Tag Name", LVCFMT_LEFT, 200);
	this->m_TagList.InsertColumn(1, "Var Type", LVCFMT_LEFT, 50);
	this->m_TagList.InsertColumn(2, "B.C. Value", LVCFMT_LEFT, 50);
	this->m_TagList.InsertColumn(3, "Calc Value", LVCFMT_LEFT, 50);

	this->m_TagList.SetExtendedStyle( this->m_TagList.GetExtendedStyle()
								| LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CDummySimDlg::OnPaint()
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
HCURSOR CDummySimDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CDummySimDlg::PrintOut(const char* fmt, ...)
{
	this->UpdateData();

	va_list arg;
	va_start( arg, fmt );
	CString strTmp;
	strTmp.FormatV(fmt, arg);
	va_end( arg );

	if( this->m_pLogFile ) {
		this->m_pLogFile->WriteString(strTmp + "\n");
		this->m_pLogFile->Flush();
	}

	if( !this->m_strOutput.IsEmpty() ) 
		this->m_strOutput += "\r\n";
	this->m_strOutput += strTmp;

	this->UpdateData(FALSE);
	this->m_Output.LineScroll(this->m_Output.GetLineCount());
}


void CDummySimDlg::PrintLog(const char* fmt, ...)
{
	if( this->m_pLogFile == NULL )
		return;

	va_list arg;
	va_start( arg, fmt );
	CString strTmp;
	strTmp.FormatV(fmt, arg);
	va_end( arg );

	this->m_pLogFile->WriteString(strTmp + "\n");
	this->m_pLogFile->Flush();
}


void CDummySimDlg::PrintTime(const char* mesg)
{
	SYSTEMTIME lt;
	::GetLocalTime(&lt);
	this->PrintOut("%s: %4d-%02d-%02d %02d:%02d:%02d.%03d",
			mesg, lt.wYear, lt.wMonth, lt.wDay, lt.wHour, lt.wMinute, lt.wSecond, lt.wMilliseconds);
}


static CString strSection = "Config";
static CString strPort = "Port";

void CDummySimDlg::ProfileLoad(void)
{
	this->m_nSvcPort = theApp.GetProfileIntA(strSection, strPort, this->m_nSvcPort);
}


void CDummySimDlg::ProfileSave(void)
{
	theApp.WriteProfileInt(strSection, strPort, this->m_nSvcPort);
}


void CDummySimDlg::OnBnClickedBtnStart()
{
	if( this->m_pListenSock != NULL ) {
		return;
	}

	this->UpdateData();
	this->ProfileSave();
	this->PrintOut("Starting.. Port=%d", this->m_nSvcPort);

	CServerSock *pSock = new CServerSock(this);

	BOOL flag = pSock->Create(this->m_nSvcPort);
	if( flag == 0 ) {
		DWORD dwError = ::GetLastError();
		this->PrintOut("Socket Create(%d) Error! ErrorNo=%08X", this->m_nSvcPort, dwError);
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
	this->PrintOut("Listening.. Port=%d", this->m_nSvcPort);
}


void CDummySimDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	if( this->m_pListenSock != NULL ) {
		delete this->m_pListenSock;
	}
	this->m_pLogFile->Close();
	delete this->m_pLogFile;

	CDialog::OnOK();
}
