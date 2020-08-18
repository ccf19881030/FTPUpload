#pragma once

#include "my_log.h"
#include "pub.h"

#include <mutex>

class Config : public my_log
{
public:
	Config(sig_log& lg);
	~Config();

	// 获取数据库连接信息
	bool get_sql_connect_info(SQLConnectStr& connstr);
	// 获取FTP配置信息
	bool get_ftp_info(FTPInfo& ftpInfo);
	// 设置FTP配置信息
	bool set_ftp_info(const FTPInfo& ftpInfo);

private:
	String m_config_xml_path;		// xml配置文件所在路径

	static std::mutex	m_mutex;
};

