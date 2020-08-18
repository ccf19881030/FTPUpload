#include "Config.h"

#include "../include/xml/pugixml.hpp"
#define XML_SETTINGS	"Settings"
#define DATADB "DataDB"
#define DATABASE "database"
#define DBUSER "user"
#define DBPWD "passwd"
#define DBPORT "port"
#define FTP_SRV		"ftp-srv"
#define LOCAL_PATH "local-path"
#define FOLDER_DIR "folder-dir"
#define LOCAL_FILE "local-file"
#define REMOTE_URL "remote-url"
#define REMOTE_USR "remote-user"
#define REMOTE_PWD "remote-pwd"
#define UPLOAD_RATE "upload-rate"

std::mutex Config::m_mutex;

Config::Config(sig_log& lg)
	: my_log(lg)
{
	m_config_xml_path = "./setting.xml";
}


Config::~Config()
{
}

bool Config::get_sql_connect_info(SQLConnectStr& connstr)
{
	std::lock_guard<std::mutex> locker(m_mutex);
	pugi::xml_document doc;
	// 读取xml配置
	pugi::xml_parse_result rc = doc.load_file(m_config_xml_path.c_str());
	if (rc.status == pugi::status_ok)
	{
		auto conf = doc.child("DataDB");
		connstr.datasoure = conf.attribute(DATADB).value();
		connstr.database = conf.attribute(DATABASE).value();
		connstr.user = conf.attribute(DBUSER).value();
		connstr.passwd = conf.attribute(DBPWD).value();
		connstr.port = conf.attribute(DBPORT).as_uint(1433);
		return true;
	}
	return false;
}

// 获取FTP配置信息
bool Config::get_ftp_info(FTPInfo& ftpInfo)
{
	std::lock_guard<std::mutex> locker(m_mutex);
	pugi::xml_document doc;
	pugi::xml_parse_result rc = doc.load_file(m_config_xml_path.c_str());
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
		doc.save_file(m_config_xml_path.c_str());
	}
	else if (rc.status == pugi::status_ok)
	{
		pugi::xml_node settings = doc.child(XML_SETTINGS);
		ftpInfo.uploadRate =  settings.attribute(UPLOAD_RATE).as_string("1");
		pugi::xml_node ftpSrv = settings.child(FTP_SRV);
		ftpInfo.localPath =  ftpSrv.attribute(LOCAL_PATH).value();
		ftpInfo.folderDir =  ftpSrv.attribute(FOLDER_DIR).value();
		ftpInfo.localFile =  ftpSrv.attribute(LOCAL_FILE).value();
		ftpInfo.remoteUrl =  ftpSrv.attribute(REMOTE_URL).value();
		ftpInfo.remoteUser =  ftpSrv.attribute(REMOTE_USR).value();
		ftpInfo.remotePwd = ftpSrv.attribute(REMOTE_PWD).value();
	}

	return true;
}

// 设置FTP配置信息
bool Config::set_ftp_info(const FTPInfo& ftpInfo)
{
	std::lock_guard<std::mutex> locker(m_mutex);
	pugi::xml_document doc;
	pugi::xml_parse_result rc = doc.load_file(m_config_xml_path.c_str());
	if (rc.status == pugi::status_file_not_found)
	{
		pugi::xml_node settings = doc.append_child(XML_SETTINGS);
		settings.append_attribute(UPLOAD_RATE).set_value(ftpInfo.uploadRate.c_str());
		pugi::xml_node ftpSrv = settings.append_child(FTP_SRV);
		ftpSrv.append_attribute(LOCAL_PATH).set_value(ftpInfo.localPath.c_str());
		ftpSrv.append_attribute(FOLDER_DIR).set_value(ftpInfo.folderDir.c_str());
		ftpSrv.append_attribute(LOCAL_FILE).set_value(ftpInfo.localFile.c_str());
		ftpSrv.append_attribute(REMOTE_URL).set_value(ftpInfo.remoteUrl.c_str());
		ftpSrv.append_attribute(REMOTE_USR).set_value(ftpInfo.remoteUser.c_str());
		ftpSrv.append_attribute(REMOTE_PWD).set_value(ftpInfo.remotePwd.c_str());
		return doc.save_file(m_config_xml_path.c_str());
	}
	else if (rc.status == pugi::status_ok)
	{
		pugi::xml_node settings = doc.child(XML_SETTINGS);
		settings.attribute(UPLOAD_RATE).set_value(ftpInfo.uploadRate.c_str());
		pugi::xml_node ftpSrv = settings.child(FTP_SRV);
		ftpSrv.attribute(LOCAL_PATH).set_value(ftpInfo.localPath.c_str());
		ftpSrv.attribute(FOLDER_DIR).set_value(ftpInfo.folderDir.c_str());
		ftpSrv.attribute(LOCAL_FILE).set_value(ftpInfo.localFile.c_str());
		ftpSrv.attribute(REMOTE_URL).set_value(ftpInfo.remoteUrl.c_str());
		ftpSrv.attribute(REMOTE_USR).set_value(ftpInfo.remoteUser.c_str());
		ftpSrv.attribute(REMOTE_PWD).set_value(ftpInfo.remotePwd.c_str());
		return doc.save_file(m_config_xml_path.c_str());
	}
	return false;
}
