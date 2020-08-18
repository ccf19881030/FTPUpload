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

	// ���ñ���Ŀ¼���ļ�����
	void set_local_path(const String& path, const String& dirRule, const String& file);

	// ����Զ��·������֤
	void set_remote_path(const String& url, const String& user = "", const String& pwd = "");

	//// ���в����ļ����Աȸ��£��ϴ�FTP
	void upload();

	// ��ռ�¼
	bool clear_files();

protected:
	// ת���ļ�����
	String convert_filepath(const String& srcpath);

	// ��ȡ�����ļ��б�
	void get_local_files();

	// ��ȡXML�ļ���¼���ļ��б�
	void get_xml_files();

	// ��ȡ��Ҫ�ϴ����ļ��б�
	void get_upload_files();

	// ����XML�ļ��б�
	void update_xml();

	// curl�Ķ��ļ�����
	static size_t read_callback(void *ptr, size_t size, size_t nmemb, void *stream);

	// ��ʼ�ϴ�
	void upload_files();

private:
	VectorFile m_fileLocal;		// ��ȡ�ı����ļ��б�
	VectorFile m_fileXml;		// XML�ļ���¼���ļ��б�
	VectorFile m_fileUpload;	// ��Ҫ�ϴ����ļ��б�
	VectorFile m_fileFailed;	// �ϴ�ʧ�ܵ��б�

	String	m_LocalDirRule;	// �����ļ��й���
	String	m_FolderDir;	// �ļ���
	String	m_LocalPath;	// �����ļ�Ŀ¼
	String	m_LocalFile;	// �����ļ�����
	String	m_remoteUrl;	// Զ��Ŀ¼
	String	m_remoteUser;	// Զ���û���
	String	m_remotePwd;	// Զ������

	String	m_pathXml;		// XML�����ļ�·��

	UploadRows m_upSuccessed;	// �ϴ��ɹ��ļ�¼�б�
	UploadRows m_upFailed;		// �ϴ�ʧ�ܵļ�¼�б�
};

