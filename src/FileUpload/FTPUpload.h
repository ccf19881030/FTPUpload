#pragma once
#include <string>
#include <vector>
typedef std::string String;
#include <io.h>
typedef std::vector<_finddata_t> VectorFile;

#include "../include/xml/pugixml.hpp"
#include "pub.h"
#include "my_log.h"

class FTPUpload
{
public:
	FTPUpload();
	~FTPUpload();

	// 设置本地目录和文件规则
	void set_local_path(const String& path, const String& dirRule, const String& file);

	// 设置远程路径和验证
	void set_remote_path(const String& url, const String& user = "", const String& pwd = "");

	//// 进行查找文件，对比更新，上传FTP
	void upload();

	// 清空记录
	bool clear_files();

protected:
	// 转换文件规则
	String convert_filepath(const String& srcpath);

	// 获取本地文件列表
	void get_local_files();

	// 获取XML文件记录的文件列表
	void get_xml_files();

	// 获取需要上传的文件列表
	void get_upload_files();

	// 更新XML文件列表
	void update_xml();

	// curl的读文件函数
	static size_t read_callback(void *ptr, size_t size, size_t nmemb, void *stream);

	// 开始上传
	void upload_files();

private:
	VectorFile m_fileLocal;		// 读取的本地文件列表
	VectorFile m_fileXml;		// XML文件记录的文件列表
	VectorFile m_fileUpload;	// 需要上传的文件列表
	VectorFile m_fileFailed;	// 上传失败的列表

	String	m_LocalDirRule;	// 本地文件夹规则
	String	m_FolderDir;	// 文件夹
	String	m_LocalPath;	// 本地文件目录
	String	m_LocalFile;	// 本地文件规则
	String	m_remoteUrl;	// 远程目录
	String	m_remoteUser;	// 远程用户名
	String	m_remotePwd;	// 远程密码

	String	m_pathXml;		// XML配置文件路径

	UploadRows m_upSuccessed;	// 上传成功的记录列表
	UploadRows m_upFailed;		// 上传失败的记录列表
};

