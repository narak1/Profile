
// SIMBridgeDlg.h : header file
//

#pragma once

class Util;
class CMySock;
class SocketManager;


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
	void Init(void);
	CString m_strOutput;
	CEdit m_Output;
	void PrintInfo(const char* fmt, ...);
	void PrintOut(const char* fmt, ...);
private:
	Util* m_Util;
	virtual void OnOK();
	bool m_bRun;
public:
	CButton m_BtnStart;
	afx_msg void OnBnClickedOk();
private:
	CMySock* m_pListenSock;
	SocketManager* m_SockMgr;
public:
	SocketManager* GetSocketManager() { return m_SockMgr; };
	int m_nPriority;
	CString m_strServerType;
};

extern CSIMBridgeDlg *g_pDlg;

#define PRINTINFO(a) g_pDlg->PrintInfo a
#define PRINTOUT(a) g_pDlg->PrintOut a
