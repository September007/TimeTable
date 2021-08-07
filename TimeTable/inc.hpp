#pragma once
#pragma warning(disable:4996)
#include<iostream>
#include<fstream>
#include<sstream>
#include<vector>
#include<map>
#include<unordered_map>
#include<set>
#include<functional>
#include<mutex>
//#include<format>
#include<type_traits>
//#include<timezoneapi.h>
using namespace std;

#define _assert(flag) if(!(flag))throw 1;
#define trace_line cout<<"at:"<<__LINE__<<endl
#define trace_arg(arg) cout<<#arg<<arg<<endl
#define inline_reference_var_helper(type,name) inline type& name##_sub(){static type sta_##name;return sta_##name;}inline type &name=name##_sub();

//stringstream 流的转换
inline stringstream& ssOS2SS(ostream & ssOS) {
	auto* p = (stringstream*)&ssOS;
	return *p;
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
