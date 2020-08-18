#pragma once
#include <type.h>

// 上传成功是否的记录信息
struct UploadRecord {
	std::string filename;	// 文件名
	std::string beginTime;  // 数据开始时间
	std::string endTime;	// 数据结束时间
	bool isSuccess;		    // 是否上传成功
public:
	// 将记录清空
	void clear() {
		filename = "";
		beginTime = "";
		endTime = "";
		isSuccess = false;
	}
};

typedef std::vector<UploadRecord> UploadRows;

// 数据库连接信息
struct SQLConnectStr
{
	String datasoure;
	String database;
	String user;
	String passwd;
	uint16 port = 1433;
};

// FTP配置信息
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