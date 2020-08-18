#ifndef _XM_DATA_TYPE_H_
#define _XM_DATA_TYPE_H_

// �Զ���
typedef unsigned char		uint8;
typedef unsigned short		uint16;
typedef unsigned int		uint32;
#ifdef WIN32
typedef unsigned __int64	uint64;
typedef __int64	 int64;
#else
typedef unsigned long long	uint64;
typedef long long	int64;
#endif
typedef char	int8;
typedef short	int16;
typedef int		int32;

#include <string.h>

// ����
#include <string>
#include <vector>
#include <map>
#include <list>
#include <functional>
typedef std::string	String;
typedef std::vector<uint8>		Uint8Array;
typedef std::vector<uint16>		Uint16Array;
typedef std::vector<uint32>		Uint32Array;
typedef std::vector<int8>		Int8Array;
typedef std::vector<int16>		Int16Array;
typedef std::vector<int32>		Int32Array;
typedef std::vector<int64>		Int64Array;
typedef std::vector<uint64>		Uint64Array;
typedef std::vector<float>		Float32Array;
typedef std::vector<double>		Float64Array;
typedef std::vector<std::string>	StringArray;
typedef std::map<std::string, std::string> StringMap;
typedef std::map<int32, std::string> Int32StringMap;
typedef std::list<std::string> StringList;
typedef std::vector<Uint8Array> Uint8sArray;


typedef std::function<bool(const char* topic, const char* payload, int payload_size, int qos)> publish_handler;
typedef std::function<void(const char* topic, const char* payload, int payload_size, int sid)> subscribe_handler;

typedef std::function<int(const Uint8Array& req, Uint8Array& res)> request_handler;

typedef std::function<void(int type, const String& data)> data_handler; 

typedef std::function<void(const char* topic, const void* payload, size_t payload_size)> handler_subscribe;

typedef std::function<void()> handler_empty;

typedef std::function<void(void* d)> handler_void;

#include "type_define.h"
typedef void(*log_cb)(const char* file, const char* func, long line, log_level level, bool need_send, const char* msg);

#include <thread>
typedef std::thread Thread;

#include <chrono>
// ���ߺ���
#define msleep(millsec) std::this_thread::sleep_for(std::chrono::milliseconds(millsec));
// ������
#define ssleep(sec) std::this_thread::sleep_for(std::chrono::seconds(sec));


#define macroStr(s) #s

// ��ȡ16λ���ݵĸ�8λ
#define GET16H(d)		((d)>>8&0xff)
// ��ȡ16λ���ݵĵ�8λ
#define GET16L(d)		((d)&0xff)
// ͨ���ߵ�8λ����16λ����
#define GET16T(h,l)		(((h)<<8&0xff00)|((l)&0xff))
// ����16λ���ݵĸߵ�8λ��ȡ����
#define GET16S(d)		GET16T(GET16L(d),GET16H(d))

#define GET32H(d)		((d)>>16&0xffff)
#define GET32L(d)		((d)&0xffff)

// ����Ӧλ��1, n��1��ʼ
#define SETBIT(x, n) (x |= 1u << (n-1))
// ����Ӧλ����, n��1��ʼ
#define CLRBIT(x, n) (x &= ~(1u << (n-1)))
// ȡ��Ӧλ��, n��1��ʼ
#define GETBIT(x, n) (x & 1u << (n-1))

#ifdef WIN32
#ifndef snprintf
#define snprintf sprintf_s
#endif
#endif


// ���
#define CLASS_DISCOPY(Class) \
Class(const Class&) = delete;\
Class& operator=(const Class&) = delete;


// ���Ա��set,get��������
#define	PROERTY_DEFINE(type, var) void set_##var(const type var); const type get_##var() const;

// ���Ա��set,get����ʵ��
#define	PROERTY_FUNC(type, var) void set_##var(const type var) { var##_ = var; } const type get_##var() const { return var##_; }

// ���Ա��set,get����ʵ��
#define	PROERTY_CLASS(Class, type, var) void Class::set_##var(const type var) { d_ptr->set_##var(var); } const type Class::get_##var() const { return d_ptr->get_##var(); }


// ɾ���ڴ�
#define ReleaseObj(ptr) if(ptr) delete ptr;
#define ReleaseObj2(ptr) if(ptr) { delete ptr; ptr = nullptr; }


#endif
