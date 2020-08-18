
// FileUpload.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "FileUpload.h"
#include "FileUploadDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include <DbgHelp.h>
#pragma comment(lib, "Dbghelp.lib")


class CMiniDump
{
private:

public:
	CMiniDump(LPCTSTR szAppName) {
		if (szAppName && lstrlen(szAppName) > 0)
		{
			lstrcpyn(s_szAppName, szAppName, sizeof(s_szAppName));
		}
		m_Filter = ::SetUnhandledExceptionFilter(_UnhandledFilter);
	}
	~CMiniDump(void) {
		::SetUnhandledExceptionFilter(m_Filter);
	}

	static LONG WINAPI _UnhandledFilter(struct _EXCEPTION_POINTERS *pExceptionInfo)
	{
		LONG  ret = EXCEPTION_CONTINUE_SEARCH;
		TCHAR szFileName[512] = { 0 };

		SYSTEMTIME st;
		::GetLocalTime(&st);
		_snprintf(szFileName,
			sizeof(szFileName),
			TEXT("%s_%04d_%02d_%02d_%02d_%02d_%02d_%02d_%02d.dmp"),
			CMiniDump::s_szAppName,
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

			//write the dump
			BOOL bOK = ::MiniDumpWriteDump(::GetCurrentProcess(),
				::GetCurrentProcessId(),
				hFile,
				MiniDumpNormal,
				&ExInfo,
				NULL,
				NULL);
			if (bOK)
			{
				//printf("Create Dump File Success!\n");
			}
			else
			{
				//printf("MiniDumpWriteDump Failed: %d\n", ::GetLastError());
			}

			ret = EXCEPTION_EXECUTE_HANDLER;
			::CloseHandle(hFile);
		}
		else
		{
			//printf("Create File %s Failed %d\n", szFileName, ::GetLastError());
		}

		return ret;
	}

public:
	static char	s_szAppName[128];

private:
	LPTOP_LEVEL_EXCEPTION_FILTER m_Filter;

};
char CMiniDump::s_szAppName[128];




// CFileUploadApp

BEGIN_MESSAGE_MAP(CFileUploadApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CFileUploadApp ����

CFileUploadApp::CFileUploadApp()
{
	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO:  �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CFileUploadApp ����

CFileUploadApp theApp;


// CFileUploadApp ��ʼ��

BOOL CFileUploadApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()��  ���򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	// ���� shell ���������Է��Ի������
	// �κ� shell ����ͼ�ؼ��� shell �б���ͼ�ؼ���
	CShellManager *pShellManager = new CShellManager;

	// ���Windows Native���Ӿ����������Ա��� MFC �ؼ�����������
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO:  Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

	// ���õ�ǰĿ¼
	TCHAR szPath[MAX_PATH] = { 0 };
	GetModuleFileName(NULL, szPath, MAX_PATH);
	CString strPath(szPath);
	int nPos = strPath.ReverseFind('\\');
	CString strCurPath = strPath.Left(nPos + 1);
	SetCurrentDirectory(strCurPath);

	CMiniDump dump("upload");

	// Ψһʵ����
// 	HANDLE hMutex = CreateMutexA(NULL, TRUE, "{35A88BFB-BA03-4270-87AC-8BE2828FA7F7}");
// 	if (hMutex != NULL && ERROR_ALREADY_EXISTS == GetLastError()) {
// 		MessageBox(NULL, "The program has been run", "Tip", MB_OK | MB_ICONWARNING);
// 		return 0;
// 	}

	CFileUploadDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO:  �ڴ˷��ô����ʱ��
		//  ��ȷ�������رնԻ���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO:  �ڴ˷��ô����ʱ��
		//  ��ȡ�������رնԻ���Ĵ���
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "����: �Ի��򴴽�ʧ�ܣ�Ӧ�ó���������ֹ��\n");
		TRACE(traceAppMsg, 0, "����: ������ڶԻ�����ʹ�� MFC �ؼ������޷� #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS��\n");
	}

	// ɾ�����洴���� shell ��������
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}

