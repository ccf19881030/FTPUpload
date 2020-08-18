#pragma once
#include "type.h"
#ifdef _WIN32
#include <io.h>
#include <direct.h>
#else
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#endif
// base64
#include <iostream>
#include <sstream>
#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/binary_from_base64.hpp>
#include <boost/archive/iterators/transform_width.hpp>
#include <boost/locale/encoding.hpp>


namespace Math
{
	class Byte
	{
	public:
		// bytes转换成BCD码
		static Uint8Array bytes2bcd(const Uint8Array& data)
		{
			Uint8Array out;
			// 比如 20 18 转换成 0x20 0x18
			for (size_t i = 0; i < data.size(); i++)
			{
				const uint8 d = data[i];
				out.push_back(d / 10 * 16 + d % 10);
			}
			return out;
		}

		// BCD码转换成bytes
		static Uint8Array bcd2bytes(const Uint8Array& data)
		{
			Uint8Array out;
			// 比如 0x20 0x18 转换成 20 18
			for (size_t i = 0; i < data.size(); i++)
			{
				const uint8 d = data[i];
				out.push_back(d / 16 * 10 + d % 16);
			}
			return out;
		}

		// BCD码转换成bytes
		static Uint8Array bcd2bytes(const uint8* buff, uint32 buffsize)
		{
			Uint8Array out(buff, buff + buffsize);
			return bcd2bytes(out);
		}

		// 计算16位校验和
		static uint16 getsum(const uint8* buff, uint32 buffsize)
		{
			uint32 sum = 0;
			for (uint32 i = 0; i < buffsize; i++)
			{
				sum += (uint8)buff[i];
			}
			return sum;
		}

		// 计算32位校验和
		static uint32 getsum32(const uint8* buff, uint32 buffsize)
		{
			uint32 sum = 0;
			for (uint32 i = 0; i < buffsize; i++)
			{
				sum += (uint8)buff[i];
			}
			return sum;
		}

		// 计算CRC16循环冗余校验
		static uint16 crc16_checksum(uint8 *puchMsg, uint32 usDataLen)
		{
			uint32 crc_reg, check;
			crc_reg = 0xFFFF;
			for (uint32 i = 0; i < usDataLen; i++) {
				crc_reg = (crc_reg >> 8) ^ puchMsg[i];
				for (uint32 j = 0; j < 8; j++) {
					check = crc_reg & 0x0001;
					crc_reg >>= 1;
					if (check == 0x0001) {    
						crc_reg ^= 0xA001;   
					}
				}
			} 
			return crc_reg;
		}
	};
	
	// 时间
	class Date
	{
	public:
		// 获取unix时间戳
		static time_t now_unix() {
			return time(0);
		}
		// 获取本地时间tm
		static tm gettm(const time_t unix_time = 0)
		{
			time_t t = unix_time == 0 ? time(0) : unix_time;
			tm tt;
#ifdef WIN32
			localtime_s(&tt, &t);
#else
			localtime_r(&t, &tt);
#endif
			return tt;
		}
		// 获取当前时间的BCD码20190308150102
		static Uint8Array getbcd(const time_t unix_time = 0)
		{
			time_t t = unix_time <= 0 ? time(0) : unix_time;
			Uint8Array _return;
			tm tt = gettm(t);
			int year = tt.tm_year + 1900;
			_return.push_back(year / 100);
			_return.push_back(year % 100);
			_return.push_back(tt.tm_mon + 1);
			_return.push_back(tt.tm_mday);
			_return.push_back(tt.tm_hour);
			_return.push_back(tt.tm_min);
			_return.push_back(tt.tm_sec);
			return Byte::bytes2bcd(_return);
		}
		// 将unix时间戳转换成本地时间, fmt默认格式 %04d-%02d-%02d %02d:%02d:%02d
		static std::string unix2str(const time_t unix_time, const char* fmt = "%04d-%02d-%02d %02d:%02d:%02d")
		{
			tm tt = gettm(unix_time);
			char date[64];
			snprintf(date, sizeof(date), fmt, tt.tm_year + 1900, tt.tm_mon + 1, tt.tm_mday, tt.tm_hour, tt.tm_min, tt.tm_sec);
			return date;
		}
		// 获取当前时间字符串，默认格式 2018-10-01 01:10:20
		static std::string getnow(const char* fmt = "%04d-%02d-%02d %02d:%02d:%02d")
		{
			return unix2str(::time(0), fmt);
		}

		// 转换字符串时间的格式, 输入顺序需和输出顺序一致, %04d-%02d-%02d %02d:%02d:%02d
		static std::string convertfmt(const char* indatetime, const char* infmt, const char* outfmt)
		{
			int year, month, day, hour, minute, second;
			sscanf(indatetime, infmt, &year, &month, &day, &hour, &minute, &second);
			char out[64];
			sprintf(out, outfmt, year, month, day, hour, minute, second);
			return out;
		}

		// 获取输入字符串时间时间戳，默认格式 2018-10-01 01:10:20, %04d-%02d-%02d
		static time_t getunix(const char* indatetime, const char* infmt = "%04d-%02d-%02d %02d:%02d:%02d")
		{
			int year, month, day, hour, minute, second;
			sscanf(indatetime, infmt, &year, &month, &day, &hour, &minute, &second);
			return gettime(year, month, day, hour, minute, second);
		}

		// 构造时间
		static time_t gettime(int year, int month, int day, int hour = 0, int minute = 0, int second = 0)
		{
			tm tt = { 0 };
			tt.tm_year = year - 1900;
			tt.tm_mon = month - 1;
			tt.tm_mday = day;
			tt.tm_hour = hour;
			tt.tm_min = minute;
			tt.tm_sec = second;
			return mktime(&tt);
		}

	};

	//  定时器
	class Timer
	{
		// 毫秒时钟
		typedef std::chrono::time_point<std::chrono::system_clock, std::chrono::milliseconds> millisecClock_type;
	public:
		// 秒
		static time_t now_s() {
			return std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		}
		// 毫秒
		static time_t now_ms() {
			return std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now()).time_since_epoch().count();
			// return system_clock::to_time_t(time_point_cast<milliseconds>(system_clock::now()));
		}
		// 微秒
		static time_t now_mms() {
			return std::chrono::time_point_cast<std::chrono::microseconds>(std::chrono::system_clock::now()).time_since_epoch().count();
		}
		// 条件休眠,单位秒
		static inline void sleep_if(volatile bool& condition, int32 sec) {
			auto t0 = now_s();
			while (condition && (now_s() - t0) < sec)
			{
				msleep(10);
			}
		}
		// 条件休眠,单位毫秒
		static inline void usleep_if(volatile bool& condition, int32 millsec) {
			auto t0 = now_ms();
			while (condition && (now_ms() - t0) < millsec)
			{
				msleep(10);
			}
		}
	public:
		typedef std::function<void()> timer_handler;
		void start(timer_handler func, int timeout_ms)
		{
			cancel();
			continue_ = true;
			auto t1 = Timer::now_ms();
			thread_ = std::thread([this, func, t1, timeout_ms]() {
				while (continue_ && (Timer::now_ms() - t1 < timeout_ms))
				{
					msleep(10);
				}
				func();
			});
		}
		void cancel()
		{
			continue_ = false;
			if (thread_.joinable()) {
				thread_.join();
			}
		}

	private:
		std::thread		thread_;
		volatile bool	continue_ = false;
	};

	// 文件相关
	class File
	{
	public:
		// 多级创建文件夹，path不包含文件名
		static bool mk_dirs(const char* path)
		{
			if (!path) {
				return false;
			}
			int32 len = strlen(path);
			if (len > 256) {
				return false;
			}
			const char* p1 = path;
			const char* p2;
			while (p1 - path < len)
			{
				p2 = strchr(p1, '/');
				if (!p2) {
					p2 = strchr(p1, '\\');
				}
				if (!p2) {
					p2 = path + len;
				}
				char dir[64] = { 0 };
				memcpy(dir, path, p2 - path + 1);
#ifdef _WIN32
				if (_access(dir, 0) == 0 || _mkdir(dir) == 0)
#else
				if (access(dir, R_OK | W_OK) == 0 || mkdir(dir, 0666) == 0)
#endif
				{
					p1 = p2 + 1;
					continue;
				}
				return false;
			}
			return true;
		}
		// 文件是否存在
		static bool exist(const char* path) {
#ifdef _WIN32
			return _access(path, 0) == 0;
#else
			return access(path, R_OK | W_OK) == 0;
#endif
		}
		// 删除文件
		static bool remove(const char* filename) {
			return ::remove(filename) == 0;
		}
		// 重命名文件
		static bool rename(const char* oldfile, const char* newfile) {
			return ::rename(oldfile, newfile) == 0;
		}
	};

	class Tools
	{
	public:
		// 将数字类型转成字符类型. fmt同printf中的fmt
		static inline String to_string(const char* fmt, const uint32 val)
		{
			char tmp[256];
			sprintf(tmp, fmt, val);
			return tmp;
		}
		// 替换字符
		static inline String replace(const String& str, const String& search, const String& dest)
		{
			String result;
			size_t start = 0, end = 0;	
			while (start < str.size())
			{
				end = str.find_first_of(search, start);
				result += str.substr(start, end - start);
				start = end + search.size();
				if (end == String::npos) {
					break;
				}
				result += dest;
			}
			return result;
		}

		// 字符串分割函数
		static inline StringArray split(const std::string& s, const std::string& sep, const bool compress = false)
		{
			StringArray out;
			int pos = 0;
			int index = -1;
			while ((index = s.find(sep, pos)) != s.npos)
			{
				if (index - pos == 0)
				{

				}
				// s.substr(pos, index - pos == 0 ? 1 : index - pos);
				std::string it = index - pos == 0 ? "" : s.substr(pos, index - pos);
				if (compress && it == "") // 压缩 index - pos == sep.size() && 
				{

				}
				else // 不用压缩
				{
					out.push_back(it);
				}

				pos = index + sep.size();
			}
			if (pos < (int32)s.size())
			{
				out.push_back(s.substr(pos));
			}
			else if (pos == (int32)s.size() && !compress)
			{
				out.push_back("");
			}
			return out;
		}

		// 数据组合字符串
		static inline String join(const StringArray& arr, const std::string& sep) {
			String result;
			for (auto& i : arr) {
				if (result.empty()) {
					result = i;
					continue;
				}
				result += sep + i;
			}
			return result;
		}
		
		// 转换hex到字符串显示
		static inline String hex2str(const char* buff, const size_t buffsize, const char* sep = "", bool is_case = false) {
			String out;
			char ch[4];
			const char* fmt = is_case ? "%02x" : "%02X";
			for (size_t i = 0; i < buffsize; i++) {
				sprintf(ch, fmt, buff[i] & 0xFF);
				if (out.empty()) {
					out = ch;
				}
				else {
					out += sep;
					out += ch;
				}
			}
			return out;
		}

		// 转换字符串显示到hex数组
		static inline String str2hex(const String& buff, const String& sep = "") {
			String out;
			size_t buffsize = buff.size();
			StringArray items;
			if (sep.empty() && buffsize % 2 == 0) {
				for (size_t i = 0; i < buffsize / 2; i++) {
					items.emplace_back(buff.substr(i * 2, 2));
				}
			}
			else if (sep.size()) {
				items = split(buff, sep, true);
			}
			for (auto& i : items) {
				int ch;
				if (!to_int(ch, i, 16)) {
					return out;
				}
				out.push_back(ch & 0xff);
			}
			return out;
		}

		// 获取字符串中数字
		static inline bool to_int(int& _return, const std::string& buff, int base = 10, size_t offset = 0, size_t count = String::npos)
		{
			_return = 0;
			if (buff.empty())
				return false;
			try {
				_return = std::stoi(buff.substr(offset, count), 0, base);
			}
			catch (...) {
				return false;
			}
			return true;
		}

		// 截取字符串buff中从offset开始的count个字符转换成float
		static inline bool to_float(double& _return, const std::string& buff, size_t offset = 0, size_t count = String::npos)
		{
			_return = 0;
			if (buff.empty())
				return false;
			try {
				_return = std::stod(buff.substr(offset, count));
			}
			catch (...) {
				return false;
			}
			return true;
		}

		// 去掉尾部的特定字符
		static inline String trim(const String& buffer, const String& sep = " ") {
			String result;
			size_t pos = 0;
			do
			{
				size_t n = buffer.find(sep, pos);
				result += buffer.substr(pos, n - pos);
				pos = (n != String::npos) ? (n + sep.size()) : n;
			} while (pos < buffer.size());
			return result;
		}

		// 转换成base64码
		static inline bool base64_encode(String* _return, const Uint8Array& buffer) {
			typedef boost::archive::iterators::base64_from_binary<boost::archive::iterators::transform_width<Uint8Array::const_iterator, 6, 8>> base64encodeIter;
			std::stringstream result;
			try {
				std::copy(base64encodeIter(buffer.begin()), base64encodeIter(buffer.end()), std::ostream_iterator<uint8>(result));
			}
			catch (...) {
				return false;
			}
			size_t eqNum = (3 - buffer.size() % 3) % 3;
			for (size_t i = 0; i < eqNum; i++) {
				result.put('=');
			}
			*_return = result.str();
			return _return->size();
		}

		// base64转换成hex
		static inline bool base64_decode(Uint8Array* _return, const String& buffer) {
			typedef boost::archive::iterators::transform_width<boost::archive::iterators::binary_from_base64<String::const_iterator>, 8, 6> base64decodeIter;
			String input = trim(buffer, "=");
			std::stringstream result;
			try {
				std::copy(base64decodeIter(input.begin()), base64decodeIter(input.end()), std::ostream_iterator<char>(result));
			}
			catch (...) {
				return false;
			}
			auto str = result.str();
			std::copy(str.begin(), str.end(), std::back_inserter(*_return));
			return _return->size();
		}
		
		// asii转成utf8
		static inline String to_utf8(const String& str, const String& charset = "gbk") {
			return boost::locale::conv::to_utf<char>(str, charset);
		}

		// utf8转成asii
		static inline String from_utf8(const String& str, const String& charset = "gbk") {
			return boost::locale::conv::from_utf<char>(str, charset);
		}
	};
}
