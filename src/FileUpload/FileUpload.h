
// FileUpload.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CFileUploadApp: 
// �йش����ʵ�֣������ FileUpload.cpp
//

class CFileUploadApp : public CWinApp
{
public:
	CFileUploadApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CFileUploadApp theApp;