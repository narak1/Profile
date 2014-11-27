// Logger.h: interface for the Logger class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOG_H__6FAE5A01_A5AC_4EE3_8CB1_665654A57471__INCLUDED_)
#define AFX_LOG_H__6FAE5A01_A5AC_4EE3_8CB1_665654A57471__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <stdio.h>

class CStdioFile;
class CCriticalSection;

class Logger  
{
	const char*m_Path;
	CStdioFile *m_File;
	CCriticalSection* m_CS;
public:
	void PrintDump(const void *data, int size, int width=16, int base_pos=0);
	void PrintBin(const void *data, int size);
	void PrintLog(const char *fmt, ...);
	void PrintLogV(const char* fmt, va_list arg);
	void Finish(void);
	bool Init(const char *file_name=NULL);
	void Init(FILE* fp);
	Logger();
	virtual ~Logger();
	void SetDir(const char* strPath);
	void Flush(void);
};

extern Logger g_Logger;
#define PRINTLOG(a) g_Logger.PrintLog a
#define PRINTLOGV(a,b) g_Logger.PrintLogV(a,b)
#define PRINTDUMP(a,b) g_Logger.PrintDump(a,b)

#endif // !defined(AFX_LOG_H__6FAE5A01_A5AC_4EE3_8CB1_665654A57471__INCLUDED_)
