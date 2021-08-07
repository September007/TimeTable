#pragma once
#include"inc.hpp"
#include"initializer.hpp"
#include"serialize.hpp"
//define class attribute

struct course {
	string name = "";
	uint32_t time_val = 0;
	bool operator ==(const course& r)const {
		return name == r.name && time_val == r.time_val;
	}
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
};
struct course_table_one_day
{
	time_t whoDay;
	vector<pair<pair<time_t, time_t>, course>> time_management;
	static course_table_one_day recover(stringstream&ss)
	{
		course_table_one_day ret;
		ret.whoDay = dataManager::recover<time_t>(ss);
		ret.time_management = dataManager::recover<decltype(time_management)>(ss);
		return ret;
	}
	static stringstream&store(stringstream&ss,const course_table_one_day&data)
	{
		dataManager::store(ss, data.whoDay);
		dataManager::store(ss, data.time_management);
		return ss;
	}
};
extern vector<course> all_course;
extern vector<course_table_one_day> all_table;
