// Logger.cpp: implementation of the Logger class.
//
//////////////////////////////////////////////////////////////////////
#include <afx.h>
#include <afxmt.h>
#include "Logger.h"

#pragma warning(disable : 4996)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

static CString m_NewLine = "\n";

Logger g_Logger;

Logger::Logger()
	: m_File(NULL)
	, m_LogLevel(DebugLog)
	, m_Cycle(0)
	, m_Date(0)
	, m_FileNameFmt("Log_%Y-%m-%d_%H%M%S.txt")
{
	this->m_CS = new CCriticalSection;
}

Logger::~Logger()
{
	if( m_File != NULL )
	{
		m_File->Close();
		delete m_File;
	}
	delete this->m_CS;
}

bool Logger::Init(const char *file_name_fmt, int cycle)
{
	if( this->m_LogLevel == this->NoLog )
	{
		return false;
	}

	// filename set
	//
	CString strFileName;
	if( (file_name_fmt != NULL) && (file_name_fmt[0] != 0) )
	{
		this->m_FileNameFmt = file_name_fmt;
	}
	CTime curTime = CTime::GetCurrentTime();
	strFileName = curTime.Format(this->m_FileNameFmt);

	// file name cycle variable set
	//
	this->m_Cycle = cycle;		// hr unit

	struct tm cur_tm;
	curTime.GetLocalTm(&cur_tm);
	if( cycle > 0 ) {
		this->m_Date = this->MakeDate(cur_tm.tm_mday, cur_tm.tm_hour) / cycle;
	}

	// log file pull path set
	//
	CString strPathName;
	if( this->m_Path.IsEmpty() ) {
		strPathName = strFileName;
	}
	else {
		strPathName = this->m_Path + "\\" + strFileName;
	}

	// file open
	//
	if( m_File == NULL ) {
		this->m_File = new CStdioFile();
	}
	else {
		this->m_File->Close();
	}
	UINT openFlag = CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::shareDenyNone | CFile::typeText;
	BOOL flag = m_File->Open(strPathName, openFlag);
	if( flag ) {
		this->m_File->SeekToEnd();
	}

	return flag == TRUE;
}

void Logger::Finish(void)
{
	if( this->m_File != NULL )
	{
		this->m_File->Close();
		delete this->m_File;
		this->m_File = NULL;
	}
}

void Logger::PrintLogV(LogLevel log_level, const char* fmt, va_list arg)
{
	if( this->m_File == NULL )
		return;
	if( this->m_LogLevel < log_level )
		return;

	char buf[BUFSIZ];
	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);

	if( this->m_Cycle > 0 ) {		// file name cycling check
		this->m_CS->Lock();
		int ymd = this->MakeDate(sysTime.wDay, sysTime.wHour) / this->m_Cycle;
		if( ymd != this->m_Date ) {
			this->Init(NULL, this->m_Cycle);
		}
		this->m_CS->Unlock();
	}

	sprintf(buf, "[%04d-%02d-%02d %02d:%02d:%02d] ",
		sysTime.wYear, sysTime.wMonth, sysTime.wDay,
		sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
	vsprintf(buf+strlen(buf), fmt, arg);
	strcat(buf, m_NewLine);
	m_File->WriteString(buf);

	m_File->Flush();
}

void Logger::PrintLog(LogLevel log_level, const char *fmt, ...)
{
	if( this->m_File == NULL )
		return;
	if( this->m_LogLevel < log_level )
		return;

	va_list arg;
	va_start( arg, fmt );
	this->PrintLogV(log_level, fmt, arg);
	va_end( arg );
}

void Logger::PrintBin(const void *data, int size)
{
	if( this->m_File == NULL )
		return;
	if( this->m_LogLevel < this->DebugLog )
		return;

	m_File->Write(data, size);
	m_File->Flush();
}

void Logger::PrintDump(const void *data, int size, int width, int base_pos)
{
	if( this->m_File == NULL )
		return;
	if( this->m_LogLevel < this->DebugLog )
		return;

	int i;
	char *ptr = (char *) data;
	CString buf, line;

	for( i=0 ; i<size ; i++ )
	{
		if( (width != 0) && (i % width == 0) )
		{
			buf.Format("%04X:", base_pos + i);
			m_File->WriteString(buf);

			line = "   ";
		}

		buf.Format(" %02X", ptr[i] & 0xFF);
		m_File->WriteString(buf);

		if( isprint(ptr[i]) )
			line += ptr[i];
		else line += ".";

		if( (width != 0) && (i % width == (width-1)) )
		{
			m_File->WriteString(line + m_NewLine);
		}
	}

	if( (i % width) != 0 )
	{
		for( ; i%width ; i++ )
			m_File->WriteString("   ");

		m_File->WriteString(line + m_NewLine);
	}

	m_File->Flush();
}


void Logger::SetDir(const char* strPath)
{
	this->m_Path = strPath;
}


void Logger::Flush(void)
{
	if( this->m_File != NULL )
		this->m_File->Flush();
}
