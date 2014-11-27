
// SIMBridgeDlg.h : header file
//

#pragma once

class Util;
class PeerInfo;


// CSIMBridgeDlg dialog
class CSIMBridgeDlg : public CDialog
{
// Construction
public:
	CSIMBridgeDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_SIMBRIDGE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	CString m_strHostname;
	int m_nServicePort;
	CString m_strSimTime;
	void ProfileLoad(void);
	void ProfileSave(void);
	void StartService(void);
	void StopService(void);
private:
	SOCKET m_ListenSock;
	HANDLE m_IOCP;
public:
	void Init(void);
	CString m_strOutput;
	CEdit m_Output;
	void PrintError(const char* fmt, ...);
private:
	static const int MaxThread = 64;
	int m_nThread;
	HANDLE m_hThread[MaxThread];
	HANDLE m_hListenThread;
	Util* m_Util;
	virtual void OnOK();
	bool m_bRun;
public:
	CButton m_BtnStart;
	void ListenFunction(void);
	void WorkFunction(void);
private:
	static const int MaxPeerInfo = 256;
	int m_nPeerInfo;
	PeerInfo* m_pPeerInfo[MaxPeerInfo];
	static const int SockBuffSize = 512;
public:
	afx_msg void OnBnClickedOk();
};
