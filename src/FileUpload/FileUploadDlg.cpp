
// FileUploadDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "FileUpload.h"
#include "FileUploadDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CFileUploadDlg �Ի���
#include "../include/xml/pugixml.hpp"
#define XML_SETTINGS	"Settings"
#define FTP_SRV		"ftp-srv"
#define LOCAL_PATH "local-path"
#define FOLDER_DIR "folder-dir"
#define LOCAL_FILE "local-file"
#define REMOTE_URL "remote-url"
#define REMOTE_USR "remote-user"
#define REMOTE_PWD "remote-pwd"
#define UPLOAD_RATE "upload-rate"


CFileUploadDlg::CFileUploadDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFileUploadDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFileUploadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_LOAD, m_btnLoadPath);
	DDX_Control(pDX, IDC_LOCAL_PATH, m_editLocalPath);
	DDX_Control(pDX, IDC_LOCAL_FILE, m_editLocalFile);
	DDX_Control(pDX, IDC_REMOTE_URL, m_editRemotePath);
	DDX_Control(pDX, IDC_USERNAME, m_editUsername);
	DDX_Control(pDX, IDC_PASSWORD, m_editPassword);
	DDX_Control(pDX, IDC_UPLOAD_RATE, m_editUploadRate);
	DDX_Control(pDX, IDC_BTN_SRV, m_btnSrv);
	DDX_Control(pDX, IDC_FOLDER_RULE, m_editFolderRule);
}

BEGIN_MESSAGE_MAP(CFileUploadDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_LOAD, &CFileUploadDlg::OnBnClickedBtnLoad)
	ON_BN_CLICKED(IDC_BTN_SRV, &CFileUploadDlg::OnBnClickedBtnSrv)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_CLEAR, &CFileUploadDlg::OnBnClickedBtnClear)
END_MESSAGE_MAP()


// CFileUploadDlg ��Ϣ�������

BOOL CFileUploadDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
	m_xmlSetttings = "./setting.xml";
	pugi::xml_document doc;
	pugi::xml_parse_result rc = doc.load_file(m_xmlSetttings);
	if (rc.status == pugi::status_file_not_found)
	{
		pugi::xml_node settings = doc.append_child(XML_SETTINGS);
		settings.append_attribute(UPLOAD_RATE).set_value("1");
		pugi::xml_node ftpSrv = settings.append_child(FTP_SRV);
		ftpSrv.append_attribute(LOCAL_PATH);
		ftpSrv.append_attribute(FOLDER_DIR);
		ftpSrv.append_attribute(LOCAL_FILE);
		ftpSrv.append_attribute(REMOTE_URL);
		ftpSrv.append_attribute(REMOTE_USR);
		ftpSrv.append_attribute(REMOTE_PWD);
		doc.save_file(m_xmlSetttings);
	}
	else if (rc.status == pugi::status_ok)
	{
		pugi::xml_node settings = doc.child(XML_SETTINGS);
		m_editUploadRate.SetWindowText(settings.attribute(UPLOAD_RATE).as_string("1"));
		pugi::xml_node ftpSrv = settings.child(FTP_SRV);
		m_editLocalPath.SetWindowText(ftpSrv.attribute(LOCAL_PATH).value());
		m_editFolderRule.SetWindowText(ftpSrv.attribute(FOLDER_DIR).value());
		m_editLocalFile.SetWindowText(ftpSrv.attribute(LOCAL_FILE).value());
		m_editRemotePath.SetWindowText(ftpSrv.attribute(REMOTE_URL).value());
		m_editUsername.SetWindowText(ftpSrv.attribute(REMOTE_USR).value());
		m_editPassword.SetWindowText(ftpSrv.attribute(REMOTE_PWD).value());

		OnBnClickedBtnSrv();
	}

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CFileUploadDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CFileUploadDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CFileUploadDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CFileUploadDlg::OnBnClickedBtnLoad()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	BROWSEINFO  bi = { 0 };
	ITEMIDLIST *pidl;
	TCHAR       szPath[MAX_PATH];

	bi.hwndOwner = m_hWnd;
	bi.pszDisplayName = szPath;
	bi.lpszTitle = TEXT("Select the Instrument Data Output File Path.");
	bi.ulFlags = BIF_RETURNONLYFSDIRS;

	if (pidl = SHBrowseForFolder(&bi))
	{
		if (SUCCEEDED(SHGetPathFromIDList(pidl, szPath)))
		{
			SetDlgItemText(IDC_LOCAL_PATH, szPath);
		}
	}
}


#define TIMER_UPLOAD	1000

void CFileUploadDlg::OnBnClickedBtnSrv()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString localPath, folderdir, localFile, remoteUrl, user, pwd;
	GetDlgItemText(IDC_LOCAL_PATH, localPath);
	GetDlgItemText(IDC_FOLDER_RULE, folderdir);
	GetDlgItemText(IDC_LOCAL_FILE, localFile);
	GetDlgItemText(IDC_REMOTE_URL, remoteUrl);
	GetDlgItemText(IDC_USERNAME, user);
	GetDlgItemText(IDC_PASSWORD, pwd);
	int rate = GetDlgItemInt(IDC_UPLOAD_RATE);
	rate = rate > 0 ? rate : 1;
	if (localPath == "" || localFile == "" || remoteUrl == "" || rate <= 0)
	{
		MessageBox("Please Complete the Information!", "Error", MB_OK | MB_ICONWARNING);
		return;
	}

	CString str;
	GetDlgItemText(IDC_BTN_SRV, str);
	bool bRet = false;
	bool bEnable = false;
	if (str == "START")
	{
		// ������Ϣ
		if (!set_ftp_setting(localPath, folderdir, localFile, remoteUrl, user, pwd, rate))
		{
			MessageBox("Save Configuration Information Failed, Check if the file is occupied!", "Error", MB_OK | MB_ICONWARNING);
			return;
		}

		// ��ʼ����
		start_task();

		// ���ÿؼ�
		bRet = true;
		bEnable = false;
		SetDlgItemText(IDC_BTN_SRV, "STOP");
	}
	else if (str == "STOP")
	{
		// ֹͣ����
		stop_task();

		// ���ÿؼ�
		bRet = true;
		bEnable = true;
		SetDlgItemText(IDC_BTN_SRV, "START");
	}
	if (bRet)
	{
		m_editLocalPath.EnableWindow(bEnable);
		m_editFolderRule.EnableWindow(bEnable);
		m_editLocalFile.EnableWindow(bEnable);
		m_btnLoadPath.EnableWindow(bEnable);
		m_editRemotePath.EnableWindow(bEnable);
		m_editUsername.EnableWindow(bEnable);
		m_editPassword.EnableWindow(bEnable);
		m_editUploadRate.EnableWindow(bEnable);
	}
	
}


void CFileUploadDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (nIDEvent == TIMER_UPLOAD)
	{
		// ��ʱ�ϴ�
		start_task();
	}

	CDialogEx::OnTimer(nIDEvent);
}



bool CFileUploadDlg::set_ftp_setting(const char* localPath, const char* folderdir, const char* localFile, const char* remoteUrl, const char* Usr, const char* Pwd, int rate)
{
	m_ftp.set_local_path(localPath, folderdir, localFile);
	m_ftp.set_remote_path(remoteUrl, Usr, Pwd);
	m_uploadRate = rate;

	pugi::xml_document doc;
	pugi::xml_parse_result rc = doc.load_file(m_xmlSetttings);
	if (rc.status == pugi::status_file_not_found)
	{
		pugi::xml_node settings = doc.append_child(XML_SETTINGS);
		settings.append_attribute(UPLOAD_RATE).set_value(rate);
		pugi::xml_node ftpSrv = settings.append_child(FTP_SRV);
		ftpSrv.append_attribute(LOCAL_PATH).set_value(localPath);
		ftpSrv.append_attribute(FOLDER_DIR).set_value(folderdir);
		ftpSrv.append_attribute(LOCAL_FILE).set_value(localFile);
		ftpSrv.append_attribute(REMOTE_URL).set_value(remoteUrl);
		ftpSrv.append_attribute(REMOTE_USR).set_value(Usr);
		ftpSrv.append_attribute(REMOTE_PWD).set_value(Pwd);
		return doc.save_file(m_xmlSetttings);
	}
	else if (rc.status == pugi::status_ok)
	{
		pugi::xml_node settings = doc.child(XML_SETTINGS);
		settings.attribute(UPLOAD_RATE).set_value(rate);
		pugi::xml_node ftpSrv = settings.child(FTP_SRV);
		ftpSrv.attribute(LOCAL_PATH).set_value(localPath);
		ftpSrv.attribute(FOLDER_DIR).set_value(folderdir);
		ftpSrv.attribute(LOCAL_FILE).set_value(localFile);
		ftpSrv.attribute(REMOTE_URL).set_value(remoteUrl);
		ftpSrv.attribute(REMOTE_USR).set_value(Usr);
		ftpSrv.attribute(REMOTE_PWD).set_value(Pwd);
		return doc.save_file(m_xmlSetttings);
	}
	return false;
}

void CFileUploadDlg::start_task()
{
	m_ftp.upload();
	// ��ʼ��ʱ����
	SetTimer(TIMER_UPLOAD, m_uploadRate * 60000, NULL);
}

void CFileUploadDlg::stop_task()
{
	// ֹͣ��ʱ����
	KillTimer(TIMER_UPLOAD);
}


void CFileUploadDlg::OnBnClickedBtnClear()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	bool rc = m_ftp.clear_files();
	MessageBox(rc ? "clear successfully!" : "clear failed!", "clear", MB_OK | (rc ? MB_ICONINFORMATION : MB_ICONWARNING));
}
