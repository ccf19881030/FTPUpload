#pragma once
#include <boost/signals2.hpp>
#include <stdarg.h>

#include "type_define.h"

// 日志信号
typedef boost::signals2::signal<void(const char* file, const char* func, long line, log_level level, bool need_send, const char* msg)> sig_log;
typedef sig_log::slot_type function_log;
// 空信号
typedef boost::signals2::signal<void()> sig_void;
typedef sig_void::slot_type function_void;


#define EmitLog(lvl, ...) write_log(__FILE__, __FUNCTION__, __LINE__, log_level::lvl, true, __VA_ARGS__)
#define EmitLog2(lvl, need, ...) write_log(__FILE__, __FUNCTION__, __LINE__, log_level::lvl, need, __VA_ARGS__)

#define SIGNAL_LOG_FUNC(lg) \
	inline void set_log_cb(const function_log& handler){\
		lg.connect(handler);}\
	inline void write_log(const char* file, const char* func, long line, log_level level, bool need_send, const char* fmt, ...) const {\
		char str[4096];va_list ap;\
		va_start(ap, fmt);vsnprintf(str, sizeof(str), fmt, ap);va_end(ap);\
		lg(file, func, line, level, need_send, str);}

#define LOG_CONSTRUCT(Class) \
	explicit Class(sig_log& lg)
#define LOG_CONSTRUCT_IMPL(Class) \
	Class::Class(sig_log& lg) : my_log(lg)

class my_log
{
public:
	//explicit my_log(my_log* lg) :signal_log_(lg->signal_log_) {}
	explicit my_log(sig_log& lg):signal_log_(lg){}

	SIGNAL_LOG_FUNC(signal_log_)

protected:
	sig_log&		signal_log_;
};
