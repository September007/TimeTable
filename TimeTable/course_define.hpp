#pragma once
#include"inc.hpp"
#include"initializer.hpp"
#include"serialize.hpp"
//define class attribute

struct course {
	string name = "";
	uint32_t time_val = 0;
	static course recover(stringstream& ss) {
		ss >> ws;
		course ret;
		string t;
		ret.name = undoFormlize(ss);
		t = undoFormlize(ss);
		ret.time_val = stoull(t);
		return ret;
	}
	static stringstream& store(stringstream& ss,const course& c) {
		 //ss << format("{}", c);
		ss << formlize(c.name) << formlize(to_string( c.time_val));
		 return ss;
	}
	bool operator ==(const course& r)const {
		return name == r.name && time_val == r.time_val;
	}
};
//template < typename _CharT>
//struct std::formatter<course, _CharT> : std::formatter<string, _CharT>
//{
//	template <typename _FormatContext>
//	auto format(const course& v, _FormatContext& format_context )
//	{
//		auto Ite = std::formatter<string, _CharT>::format(v.name, format_context);
//		Ite = '\n';
//		Ite = std::formatter<uint32_t, _CharT>().format(v.time_val, format_context);
//		return Ite;
//	}
//};
extern vector<course> all_course;
