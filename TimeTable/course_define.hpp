//here define two class course and course_table_one_day ,
// two inline reference var
// and function cook_table who cook a new course_table_one_day by the info of tables before new table.whoDay
#pragma once
#include"inc.hpp"
#include"initializer.hpp"
#include"serialize.hpp"
#include"time_transform.hpp"
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
#define rep_path "data/286885/"

#define here_register(type,name) __auto_userDefine_register(type,name,,rep_path#name".ini")
using vectorCourse = vector<course>;
using vectorTable = vector<course_table_one_day>;
inline vector<course>& all_course_sub() {
	static here_register(vectorCourse, sta_all_course);
	return sta_all_course;
}
inline vector<course_table_one_day>& all_table_sub() {
	static here_register(vectorTable, sta_all_table);
	return sta_all_table;
}
inline  vector<course> &all_course=all_course_sub();
inline vector<course_table_one_day> &all_table= all_table_sub();

inline course_table_one_day cook_new_table(const vector<course_table_one_day> & record,time_t new_table_time)
{
	course_table_one_day ret;
	auto p = localtime(&new_table_time);
	int dayWho;
	return ret;
}