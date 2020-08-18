#include "stdafx.h"
#include "FTPUpload.h"
#include <algorithm>

#define BUILDING_LIBCURL
#include "../include/curl/curl.h"
#pragma comment(lib, "wldap32.lib")
#ifdef _DEBUG
#pragma comment(lib, "../lib/debug/libcurld.lib")
#else
#pragma comment(lib, "../lib/release/libcurl.lib")
#endif

FTPUpload::FTPUpload()
{
	m_pathXml = "./file.xml";

	/* In windows, this will init the winsock stuff */
	curl_global_init(CURL_GLOBAL_ALL);
}


FTPUpload::~FTPUpload()
{
	curl_global_cleanup();
}

void FTPUpload::set_local_path(const String& path, const String& dirRule, const String& file)
{
	char ch =  path.at(path.size() - 1);
	if (ch == '\\' || ch == '/')
		m_LocalPath = path.substr(0, path.size() - 1);
	else
		m_LocalPath = path;
	m_LocalDirRule = dirRule;
	m_LocalFile = file;
}

void FTPUpload::set_remote_path(const String& url, const String& user /*= ""*/, const String& pwd /*= ""*/)
{
	char ch = url.at(url.size() - 1);
	if (ch == '\\' || ch == '/')
		m_remoteUrl = url.substr(0, url.size() - 1);
	else
		m_remoteUrl = url;
	
	m_remoteUser = user;
	m_remotePwd = pwd;
}

void FTPUpload::upload()
{
	// 读取本地文件列表
	get_local_files();
	// 读取上次文件列表
	get_xml_files();
	// 对比变化的文件
	get_upload_files();
	// 上传文件列表
	upload_files();
	// 更新XML列表
	update_xml();
}

// 清空记录
bool FTPUpload::clear_files()
{
	pugi::xml_document doc;
	return doc.save_file(m_pathXml.c_str());
}

#include <boost/algorithm/string.hpp>
String FTPUpload::convert_filepath(const String& srcpath)
{
	time_t tt = time(0);
	tm* now = localtime(&tt);
	auto it = [](int val, int count, const char* cc) {
		std::string fmt1 = "%$b$cd";
		std::string fmt = boost::replace_all_copy(boost::replace_all_copy(fmt1, "$b", cc), "$c", std::to_string(count));
		char str[256];
		std::sprintf(str, fmt.c_str(), val);
		return std::string(str);
	};
	return boost::replace_all_copy(
		boost::replace_all_copy(
			boost::replace_all_copy(
				boost::replace_all_copy(srcpath, "$yyyy", it(now->tm_year + 1900, 4, "0")),
				"$mm", it(now->tm_mon + 1, 2, "0")),
			"$dd", it(now->tm_mday, 2, "0")),
		"$HH", it(now->tm_hour, 2, "0"));
}

// 获取本地文件列表
void FTPUpload::get_local_files()
{
	// 路径
	String local_path = m_LocalPath;
	// 规则文件夹
	m_FolderDir = convert_filepath(m_LocalDirRule);
	if (m_FolderDir.size())
	{
		local_path += "\\" + m_FolderDir;
	}

#ifdef WIN32
	local_path += "\\";
#else
	local_path += "/";
#endif
	local_path += m_LocalFile;

	// 清空列表
	m_fileLocal.clear();
	// 读取文件目录
	_finddata_t fdd;
	intptr_t fd = _findfirst(local_path.c_str(), &fdd);
	int rc = fd;
	while (rc != -1)
	{
		m_fileLocal.push_back(fdd);
		rc = _findnext(fd, &fdd);
	}
	_findclose(fd);
}

// 获取XML文件记录的文件列表
void FTPUpload::get_xml_files()
{
	// 清空
	m_fileXml.clear();
	// 读取xml配置
	pugi::xml_document doc;
	pugi::xml_parse_result rc = doc.load_file(m_pathXml.c_str());
	if (rc.status == pugi::status_ok)
	{
		auto fl = doc.child("file-list");
		for (auto it = fl.child("file"); it; it = it.next_sibling("file"))
		{
			_finddata_t dd;
			strcpy_s(dd.name, it.attribute("name").value());
			dd.size = it.attribute("size").as_uint();
			dd.time_write = it.attribute("last_time").as_uint();
			m_fileXml.push_back(dd);
		}
	}
}

// 获取需要上传的文件列表
void FTPUpload::get_upload_files()
{
	// 清空上传列表
	m_fileUpload.clear();
	// 对比文件信息
	for (auto i : m_fileLocal)
	{
		// xml中没有的就加入
		auto it1 = std::find_if(m_fileXml.begin(), m_fileXml.end(), [i](const _finddata_t& oth){
			return strcmp(oth.name, i.name) == 0;
		});
		if (it1 == m_fileXml.end())
		{
			m_fileUpload.push_back(i);
		}
		// xml中有的，对比日期，日期变化的就加入
		auto it2 = std::find_if(m_fileXml.begin(), m_fileXml.end(), [i](const _finddata_t& oth){
			return strcmp(oth.name, i.name) == 0 && i.time_write != oth.time_write;
		});
		if (it2 != m_fileXml.end())
		{
			m_fileUpload.push_back(i);
		}
	}
}

// 更新XML文件列表
void FTPUpload::update_xml()
{
	if (m_fileUpload.empty())
	{
		return;
	}
	// 将所有文件保存
	pugi::xml_document doc;
	auto all = doc.append_child("file-list");
	for (auto i : m_fileLocal)
	{
		// 检查传输失败的文件列表
		auto it = std::find_if(m_fileFailed.begin(), m_fileFailed.end(), [i](const _finddata_t& oth){
			return strcmp(oth.name, i.name) == 0;
		});
		// 记录传输成功的文件
		if (it != m_fileFailed.end())
		{
			continue;
		}
		auto item = all.append_child("file");
		item.append_attribute("name").set_value(i.name);
		item.append_attribute("size").set_value(i.size);
		item.append_attribute("last_time").set_value(i.time_write);
	}
	doc.save_file(m_pathXml.c_str());
}

size_t FTPUpload::read_callback(void *ptr, size_t size, size_t nmemb, void *stream)
{
	curl_off_t nread;
	/* in real-world cases, this would probably get this data differently
	as this fread() stuff is exactly what the library already would do
	by default internally */
	size_t retcode = fread(ptr, size, nmemb, (FILE*)stream);

	nread = (curl_off_t)retcode;

	// 	fprintf(stderr, "*** We read %" CURL_FORMAT_CURL_OFF_T
	// 		" bytes from file\n", nread);
	return retcode;
}

// 进行查找文件，对比更新，上传FTP
void FTPUpload::upload_files()
{
	m_fileFailed.clear();
	m_upFailed.clear();
	m_upSuccessed.clear();

	/* get a curl handle */
	CURL* curl = curl_easy_init();
	if (!curl)
	{
		m_fileFailed = m_fileUpload;
		return;
	}

	/* enable uploading */
	curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);

	/* we want to use our own read function */
	curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_callback);

	// 设置远程访问的用户名和密码
	if (m_remoteUser.size())
	{
		String verify = m_remoteUser + ":" + m_remotePwd;
		curl_easy_setopt(curl, CURLOPT_USERPWD, verify.c_str());
	}

	for (auto it : m_fileUpload)
	{
		// 本地文件路径
		String local_file = m_LocalPath;

		// 组合文件路径
		local_file += "\\";
		if (m_FolderDir.size())
		{
			local_file += m_FolderDir + "\\";
		}
		local_file += it.name;

		// 打开上传的文件流
		FILE* fd = fopen(local_file.c_str(), "rb");

		// 设置上传的文件流
		curl_easy_setopt(curl, CURLOPT_READDATA, fd);

		// 设置上传的文件大小
		curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE, (curl_off_t)it.size);

		// 设置文件夹
		curl_easy_setopt(curl, CURLOPT_FTP_CREATE_MISSING_DIRS, 1);

		// 设置远程路径
		String remote_file = m_remoteUrl;
		remote_file += "/";
		if (m_FolderDir.size())
		{
			remote_file += m_FolderDir + "/";
		}
		remote_file += it.name;
		curl_easy_setopt(curl, CURLOPT_URL, remote_file.c_str());

		// 开始上传文件
		CURLcode res = curl_easy_perform(curl);
		if (res != CURLE_OK)
		{
			fprintf(stderr, "curl_easy_perform() failed: %s\n",
				curl_easy_strerror(res));
		}
		
		// 关闭本地文件流
		if (fd > 0) fclose(fd);
	}

	// 释放网络
	curl_easy_cleanup(curl);
}
