// Logger.cpp: implementation of the Logger class.
//
//////////////////////////////////////////////////////////////////////
#include <afx.h>
#include "Logger.h"

#pragma warning(disable : 4996)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

static CString m_NewLine = "\n";

Logger g_Logger(true);

Logger::Logger(bool flag)
	: m_Path(NULL)
{
	if( flag )
	{
		SYSTEMTIME sysTime;
		::GetLocalTime(&sysTime);
		CString strFileName;
		strFileName.Format("Log_%4d-%02d-%02d_%02d%02d%02d.txt",
			sysTime.wYear, sysTime.wMonth, sysTime.wDay, sysTime.wHour, sysTime.wMinute, sysTime.wSecond);

		this->m_File = new CStdioFile();
		UINT openFlag = CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::shareDenyNone | CFile::typeText;
		BOOL flag = m_File->Open(strFileName, openFlag);
		if( flag == FALSE ) {
			delete this->m_File;
			this->m_File = NULL;
		}
		else {
			this->m_File->SeekToEnd();
		}
	}
	else {
		this->m_File = NULL;
	}
}

Logger::~Logger()
{
	if( m_File != NULL )
	{
		m_File->Close();
		delete m_File;
	}
}

bool Logger::Init(const char *file_name)
{
	if( m_File != NULL )
	{
		m_File->Close();
		delete m_File;
	}

	if( (file_name == NULL) || (file_name[0] == 0) ) {
		this->m_File = NULL;
		return false;
	}

	bool bFlag;
	char path_name[1024];
	strcpy(path_name, this->m_Path);
	strcat(path_name, "\\");
	strcat(path_name, file_name);

	this->m_File = new CStdioFile();
	UINT openFlag = CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::shareDenyNone | CFile::typeText;
	BOOL flag = m_File->Open(path_name, openFlag);
	if( flag == FALSE ) {
		delete this->m_File;
		this->m_File = NULL;
		bFlag = false;
	}
	else {
		this->m_File->SeekToEnd();
		bFlag = true;
	}

	return bFlag;
}

void Logger::Init(FILE* fp)
{
	if( m_File != NULL )
	{
		m_File->Close();
		delete m_File;
	}

	if( fp != NULL ) {
		this->m_File = new CStdioFile(fp);
	}
	else {
		this->m_File = NULL;
	}
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

void Logger::PrintLogV(const char* fmt, va_list arg)
{
	if( this->m_File == NULL )
		return;

	char buf[BUFSIZ];

	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);
	sprintf(buf, "[%04d-%02d-%02d %02d:%02d:%02d] ",
		sysTime.wYear, sysTime.wMonth, sysTime.wDay,
		sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
	m_File->WriteString(buf);

	vsprintf(buf, fmt, arg );
	m_File->WriteString(buf);

	int len = strlen(fmt);
	if( (len == 0) || (fmt[len-1] != m_NewLine.GetAt(0)) )
		m_File->WriteString(m_NewLine);

	m_File->Flush();
}

void Logger::PrintLog(const char *fmt, ...)
{
	va_list arg;
	va_start( arg, fmt );
	this->PrintLogV(fmt, arg);
	va_end( arg );
}

void Logger::PrintBin(const void *data, int size)
{
	if( this->m_File == NULL )
		return;

	m_File->Write(data, size);
}

void Logger::PrintDump(const void *data, int size, int width, int base_pos)
{
	if( this->m_File == NULL )
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
	this->m_File->Flush();
}
