#pragma once

#include "my_log.h"
#include "pub.h"

#include <mutex>

class Config : public my_log
{
public:
	Config(sig_log& lg);
	~Config();

	// ��ȡ���ݿ�������Ϣ
	bool get_sql_connect_info(SQLConnectStr& connstr);
	// ��ȡFTP������Ϣ
	bool get_ftp_info(FTPInfo& ftpInfo);
	// ����FTP������Ϣ
	bool set_ftp_info(const FTPInfo& ftpInfo);

private:
	String m_config_xml_path;		// xml�����ļ�����·��

	static std::mutex	m_mutex;
};

