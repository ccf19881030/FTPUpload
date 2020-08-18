#pragma once
#include <type.h>
#include <boost/signals2.hpp>

// 空参数信号
typedef boost::signals2::signal<void()> sig_void0;

// 返回数据信号
typedef boost::signals2::signal<void(const String& payload)> sig_data;

// 数据缓存信号
typedef boost::signals2::signal<void(uint8* buffer, size_t buffer_size)> sig_buffer;
typedef sig_buffer::slot_type function_buffer;

typedef std::function<String(const uint8* buff, const size_t buff_size, const int32 timeout_ms)> function_rw;

// MQ数据推送
typedef boost::signals2::signal<void(const String& topic, int qos, const String& payload)> sig_pub;

// Redis数据
typedef boost::signals2::signal<void(const String& key, const String& val)> sig_set;
typedef boost::signals2::signal<void(const String& key, int32 seconds, const String& val)> sig_setex;
typedef boost::signals2::signal<void(const String& key, const String& field, const String& val)> sig_hset;

typedef boost::signals2::signal<void(const String& key, int32 seconds)> sig_expire;

typedef boost::signals2::signal<String(const String& key)> sig_get;

typedef boost::signals2::signal<StringArray(const String& pattern)> sig_keys;

typedef boost::signals2::signal<StringMap(const String& key)> sig_hgetall;

typedef boost::signals2::signal<String(const String& key, const String& field)> sig_hget;

typedef boost::signals2::signal<void(const String& key)> sig_del;

typedef boost::signals2::signal<void(const String& key, const String& field)> sig_hdel;
