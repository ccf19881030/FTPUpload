#pragma once
#include <iostream>
#define WIN32_LEAN_AND_MEAN 
#include <windows.h>
#include <DbgHelp.h>
#pragma comment(lib, "Dbghelp.lib")

class MiniDump
{
private:

public:
	MiniDump(LPCTSTR szAppName) {
		if (szAppName && lstrlen(szAppName) > 0)
		{
			lstrcpyn(s_szAppName, szAppName, sizeof(s_szAppName));
		}
		m_Filter = ::SetUnhandledExceptionFilter(_UnhandledFilter);
	}
	~MiniDump(void) {
		::SetUnhandledExceptionFilter(m_Filter);
	}

	static LONG WINAPI _UnhandledFilter(struct _EXCEPTION_POINTERS *pExceptionInfo)
	{
		TCHAR szFileName[512] = { 0 };
		SYSTEMTIME st;
		::GetLocalTime(&st);
		_snprintf_s(szFileName,
			sizeof(szFileName),
			TEXT("%s_%04d_%02d_%02d_%02d_%02d_%02d_%02d_%02d.dmp"),
			MiniDump::s_szAppName,
			st.wYear, st.wMonth, st.wDay,
			st.wHour, st.wMinute, st.wSecond, st.wMilliseconds,
			::rand() % 100);

		HANDLE hFile = ::CreateFile(szFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile != INVALID_HANDLE_VALUE)
		{
			MINIDUMP_EXCEPTION_INFORMATION ExInfo;

			ExInfo.ThreadId = ::GetCurrentThreadId();
			ExInfo.ExceptionPointers = pExceptionInfo;
			ExInfo.ClientPointers = false;

			// Write the information into the dump
			BOOL bOK = ::MiniDumpWriteDump(::GetCurrentProcess(),	// Handle of process
				::GetCurrentProcessId(),	// Handle of process
				hFile,						// Handle of dump file
				MiniDumpNormal,				// Dump Level: Mini
				&ExInfo,					// Exception information
				NULL,						// User stream parameter
				NULL);						// Callback Parameter
			if (bOK)
			{
				//printf("Create Dump File Success!\n");
			}
			else
			{
				//printf("MiniDumpWriteDump Failed: %d\n", ::GetLastError());
			}
			::CloseHandle(hFile);
		}
		return EXCEPTION_EXECUTE_HANDLER;
	}

public:
	static char	s_szAppName[128];

private:
	LPTOP_LEVEL_EXCEPTION_FILTER m_Filter;

};
char MiniDump::s_szAppName[128];