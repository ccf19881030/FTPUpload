#ifndef _XM_DATA_TYPE_H_
#define _XM_DATA_TYPE_H_

// 自定义
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

// 数组
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
// 休眠毫秒
#define msleep(millsec) std::this_thread::sleep_for(std::chrono::milliseconds(millsec));
// 休眠秒
#define ssleep(sec) std::this_thread::sleep_for(std::chrono::seconds(sec));


#define macroStr(s) #s

// 获取16位数据的高8位
#define GET16H(d)		((d)>>8&0xff)
// 获取16位数据的低8位
#define GET16L(d)		((d)&0xff)
// 通过高低8位构造16位数据
#define GET16T(h,l)		(((h)<<8&0xff00)|((l)&0xff))
// 交换16位数据的高低8位获取数据
#define GET16S(d)		GET16T(GET16L(d),GET16H(d))

#define GET32H(d)		((d)>>16&0xffff)
#define GET32L(d)		((d)&0xffff)

// 将对应位置1, n从1开始
#define SETBIT(x, n) (x |= 1u << (n-1))
// 将对应位置零, n从1开始
#define CLRBIT(x, n) (x &= ~(1u << (n-1)))
// 取对应位数, n从1开始
#define GETBIT(x, n) (x & 1u << (n-1))

#ifdef WIN32
#ifndef snprintf
#define snprintf sprintf_s
#endif
#endif


// 类宏
#define CLASS_DISCOPY(Class) \
Class(const Class&) = delete;\
Class& operator=(const Class&) = delete;


// 类成员的set,get函数定义
#define	PROERTY_DEFINE(type, var) void set_##var(const type var); const type get_##var() const;

// 类成员的set,get函数实现
#define	PROERTY_FUNC(type, var) void set_##var(const type var) { var##_ = var; } const type get_##var() const { return var##_; }

// 类成员的set,get函数实现
#define	PROERTY_CLASS(Class, type, var) void Class::set_##var(const type var) { d_ptr->set_##var(var); } const type Class::get_##var() const { return d_ptr->get_##var(); }


// 删除内存
#define ReleaseObj(ptr) if(ptr) delete ptr;
#define ReleaseObj2(ptr) if(ptr) { delete ptr; ptr = nullptr; }


#endif
