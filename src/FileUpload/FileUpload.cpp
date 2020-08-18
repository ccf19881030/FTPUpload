
// FileUpload.cpp : 定义应用程序的类行为。
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


// CFileUploadApp 构造

CFileUploadApp::CFileUploadApp()
{
	// 支持重新启动管理器
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO:  在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CFileUploadApp 对象

CFileUploadApp theApp;


// CFileUploadApp 初始化

BOOL CFileUploadApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。  否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	// 创建 shell 管理器，以防对话框包含
	// 任何 shell 树视图控件或 shell 列表视图控件。
	CShellManager *pShellManager = new CShellManager;

	// 激活“Windows Native”视觉管理器，以便在 MFC 控件中启用主题
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO:  应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));

	// 设置当前目录
	TCHAR szPath[MAX_PATH] = { 0 };
	GetModuleFileName(NULL, szPath, MAX_PATH);
	CString strPath(szPath);
	int nPos = strPath.ReverseFind('\\');
	CString strCurPath = strPath.Left(nPos + 1);
	SetCurrentDirectory(strCurPath);

	CMiniDump dump("upload");

	// 唯一实例化
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
		// TODO:  在此放置处理何时用
		//  “确定”来关闭对话框的代码
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO:  在此放置处理何时用
		//  “取消”来关闭对话框的代码
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "警告: 对话框创建失败，应用程序将意外终止。\n");
		TRACE(traceAppMsg, 0, "警告: 如果您在对话框上使用 MFC 控件，则无法 #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS。\n");
	}

	// 删除上面创建的 shell 管理器。
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return FALSE;
}

