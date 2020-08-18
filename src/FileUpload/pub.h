#pragma once
#include <type.h>

// �ϴ��ɹ��Ƿ�ļ�¼��Ϣ
struct UploadRecord {
	std::string filename;	// �ļ���
	std::string beginTime;  // ���ݿ�ʼʱ��
	std::string endTime;	// ���ݽ���ʱ��
	bool isSuccess;		    // �Ƿ��ϴ��ɹ�
public:
	// ����¼���
	void clear() {
		filename = "";
		beginTime = "";
		endTime = "";
		isSuccess = false;
	}
};

typedef std::vector<UploadRecord> UploadRows;

// ���ݿ�������Ϣ
struct SQLConnectStr
{
	String datasoure;
	String database;
	String user;
	String passwd;
	uint16 port = 1433;
};

// FTP������Ϣ
struct FTPInfo 
{
	String uploadRate;
	String localPath;
	String folderDir;
	String localFile;
	String remoteUrl;
	String remoteUser;
	String remotePwd;
};