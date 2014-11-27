
// DummySimDlg.h : header file
//

#pragma once
#include "afxcmn.h"

class Tag;
class CServerSock;

// CDummySimDlg dialog
class CDummySimDlg : public CDialog
{
// Construction
public:
	CDummySimDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_DUMMYSIM_DIALOG };

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
private:
	CStdioFile* m_pLogFile;
public:
	int m_nSvcPort;
	CString m_strOutput;
	CEdit m_Output;
	void PrintOut(const char* fmt, ...);
	void PrintLog(const char* fmt, ...);
	void PrintTime(const char* mesg);
	CListCtrl m_TagList;
	void ProfileLoad(void);
	void ProfileSave(void);
	Tag** m_ppTag;
	int m_nTag;
	afx_msg void OnBnClickedBtnStart();
private:
	CServerSock* m_pListenSock;
public:
	afx_msg void OnBnClickedOk();
	void StartService(void);
	afx_msg void OnBnClickedBtnStop();
};
