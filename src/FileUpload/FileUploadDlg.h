
// FileUploadDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "FTPUpload.h"


// CFileUploadDlg 对话框
class CFileUploadDlg : public CDialogEx
{
// 构造
public:
	CFileUploadDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MAIN_DLG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnLoad();
	afx_msg void OnBnClickedBtnSrv();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CButton m_btnLoadPath;
	CEdit m_editLocalPath;
	CEdit m_editLocalFile;
	CEdit m_editRemotePath;
	CEdit m_editUsername;
	CEdit m_editPassword;
	CEdit m_editUploadRate;
	CButton m_btnSrv;

	int			m_uploadRate = 1;
	CString		m_xmlSetttings;
	FTPUpload	m_ftp;

	bool set_ftp_setting(const char* localPath, const char* folderdir, const char* localFile, const char* remoteUrl, const char* Usr, const char* Pwd, int rate);

	void start_task();

	void stop_task();

	afx_msg void OnBnClickedBtnClear();
	CEdit m_editFolderRule;
};
