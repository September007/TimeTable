#pragma once
#pragma warning(disable:4996)
#pragma warning(disable:4244)
#pragma warning(disable:4305)
#pragma warning(disable:4091)
#pragma warning(disable:26495)
#pragma warning(disable:26812)
#pragma warning(disable:26451)
#pragma warning(disable:4309)
#include<iostream>
#include<fstream>
#include<sstream>
#include<vector>
#include<map>
#include<unordered_map>
#include<set>
#include<functional>
#include<mutex>
#include<format>
#include<type_traits>
//#include<timezoneapi.h>
using namespace std;
using std::stringstream;
using std::stoi;
using std::ws;
using std::string;

using std::vector;
using std::map;
#define _assert(flag) if(!(flag))throw 1;
#define trace_line cout<<"at:"<<__LINE__<<endl
#define trace_arg(arg) cout<<#arg<<arg<<endl
#define inline_reference_var_helper(type,name) inline type& name##_sub(){static type sta_##name;return sta_##name;}inline type &name=name##_sub();

//stringstream 流的转换
inline std::stringstream& ssOS2SS(std::ostream & ssOS) {
	auto* p = (std::stringstream*)&ssOS;
	return *p;
}
inline string operator""_s(const char* p, size_t)
{
	return p;
}
inline string operator""_S(const char* p, size_t st)
{
	return operator""_s(p, st);
}
//时间转换
inline time_t operator""_M(const char* p, size_t)
{
	return stoi(p) * 60;
}
inline time_t operator""_H(const char* p, size_t)
{
	return stoi(p) * 3600;
}

inline time_t operator""_D(const char* p, size_t)
{
	return stoi(p) * 24 * 3600;
}

inline time_t operator""_HM(const char* p, size_t)
{
	int h, m;
	char split;
	stringstream ss;
	ss << p;
	ss >> h >> ws >> split >> m;
	return h * 3600 + m * 60;
}
