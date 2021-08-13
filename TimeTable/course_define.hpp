//here define two class course and lesson_table_one_day ,
// two inline reference var
// and function cook_table who cook a new lesson_table_one_day by the info of tables before new table.whoDay
#pragma once
#include"inc.hpp"
#include<random>
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
	bool operator <(const course& r)const {
		return name < r.name || name == r.name && time_val < r.time_val;
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
	static stringstream& store(stringstream& ss, const course& c) {
		//ss << format("{}", c);
		ss << formlize(c.name) << formlize(to_string(c.time_val));
		return ss;
	}
};
struct lesson_table_one_day
{
	//第几天 construct from whoDay(time_t)
	int whoDay;
	vector<pair<pair<time_t, time_t>, course>> time_management;
	static lesson_table_one_day recover(stringstream& ss)
	{
		lesson_table_one_day ret;
		ret.whoDay = dataManager::recover<int>(ss);
		ret.time_management = dataManager::recover<decltype(time_management)>(ss);
		return ret;
	}
	static stringstream& store(stringstream& ss, const lesson_table_one_day& data)
	{
		dataManager::store(ss, data.whoDay);
		dataManager::store(ss, data.time_management);
		return ss;
	}
};
#define rep_path "data/286885/"

#define here_register(type,name) __auto_userDefine_register(type,name,,rep_path#name".ini")
using vectorCourse = vector<course>;
using vectorTable = vector<lesson_table_one_day>;
inline vector<course>& all_course_sub() {
	static here_register(vectorCourse, sta_all_course);
	return sta_all_course;
}
inline vector<lesson_table_one_day>& all_table_sub() {
	static here_register(vectorTable, sta_all_table);
	return sta_all_table;
}
inline  vector<course>& all_course = all_course_sub();
inline vector<lesson_table_one_day>& all_table = all_table_sub();
struct time_split_manage
{
	using split_key_type = string;
	using split_form_type = vector<pair<time_t, time_t>>;
	map<split_key_type, split_form_type> sols;
	time_split_manage() = default;
	split_form_type get_time_split(split_key_type key)
	{
		auto p = sols.find(key);
		if (p == sols.end()) { return default_split_form; }
		else return p->second;
	}
	static stringstream & store(stringstream&ss,const time_split_manage&data)
	{
		dataManager::store(ss, data.sols);
		return ss;
	}
	static time_split_manage recover(stringstream&ss)
	{
		time_split_manage ret;
		ret.sols=dataManager::recover<decltype(time_split_manage::sols)>(ss);
		return ret;
	}
	static inline split_form_type default_split_form = {
		{"9:00"_HM,"10:00"_HM},
	{"10:00"_HM,"11:00"_HM},
	{"11:00"_HM,"12:00"_HM},
	{"2:00"_HM,"3:00"_HM},
	{"3:00"_HM,"4:00"_HM},
	{"4:00"_HM,"5:00"_HM},
	{"8:00"_HM,"9:00"_HM} };
};
//全局管理
inline_reference_var_helper(time_split_manage, time_split_manager);
/***************************************/
/************ cook lesson table ********/
/***************************************/
inline int getCircle(const vector<course>& cs = all_course)
{
	int cont = 0;
	for (auto& c : cs)
		cont += c.time_val;
	return cont;
}
inline vector<pair<time_t, time_t>> default_time_split = {
	{"9:00"_HM,"10:00"_HM},
	{"10:00"_HM,"11:00"_HM},
	{"11:00"_HM,"12:00"_HM},
	{"2:00"_HM,"3:00"_HM},
	{"3:00"_HM,"4:00"_HM},
	{"4:00"_HM,"5:00"_HM},
	{"8:00"_HM,"9:00"_HM}
}; 
/// <summary>
/// 计算接下来几天的课程安排
/// </summary>
/// <param name="record">以前的课程安排，其中的末尾往前的head_circle_count*circle_count的安排对结果有影响</param>
/// <param name="new_table_time">新课程安排的时间</param>
/// <param name="time_management">假设的今后的时间安排</param>
/// <param name="cook_length">要生成的课表长度</param>
/// <param name="head_count">需要考虑的之前的课表长度，按天数计算</param>
/// <param name="tail_circle_count">之后的课表长度（指当前课表安排后），按周期计算</param>
/// <returns>接下来的课表</returns>
inline vector<lesson_table_one_day >
cook_new_table(const vector<lesson_table_one_day>& record, time_t new_table_time, const vector<pair<time_t, time_t>>& time_management = default_time_split, int cook_length = 1, int head_count = 1, int tail_circle_count = 1)
{
	vector<lesson_table_one_day >ret(cook_length);

	int new_day = dayWho(new_table_time);
	auto circle = getCircle();//周期长
	//尾部分
	int pool_length = circle * tail_circle_count;
	{
		int perDay = 0;
		for (auto& les : time_management)
			perDay += (les.second - les.first);
		perDay /= 3600;
		//需要生成的课表部分  perDay*cook_length
		pool_length += perDay * cook_length;
	}
	//需要担心的 之前的课表
	map<course, time_t> mci_head_worry;
	time_t all_pass_count = 0;
	for (auto& lt : record)
	{
		if (lt.whoDay >= new_day - head_count)//头前在意的课表
			for (auto& ls : lt.time_management)
			{
				auto lesLength = ls.first.second - ls.first.first;
				//课程计时
				mci_head_worry[ls.second] += lesLength;
				all_pass_count += lesLength;
				//more action
			}
	}
	//转换计时单位为小时
	for (auto& mci : mci_head_worry)
		mci.second /= 3600;
	all_pass_count /= 3600;
	pool_length += all_pass_count;
	map<course, long long> course_rest;
	long long act_pool_length = 0;
	for (auto& c : all_course)
	{
		auto fd = mci_head_worry.find(c);
		if (fd == mci_head_worry.end())
		{
			auto p = make_pair(c, long long(c.time_val) * pool_length / circle);
			act_pool_length += p.second;
			course_rest.insert(move(p));
		}
		else
		{
			auto p = make_pair(c, long long(c.time_val) * pool_length / circle - fd->second);
			act_pool_length += p.second;
			course_rest.insert(move(p));
		}
	}

	//不考虑性能啦
	auto cut_one_at = [&](long long pos)
	{
		act_pool_length--;
		for (auto& c : course_rest)
		{
			if (c.second <= 0)continue;
			pos -= c.second;
			if (pos < 0)
			{
				c.second--;
				return c.first;
			}
		}
		throw std::exception("check me");
	};
	auto randObj = mt19937_64();
	for (int i = 0; i<int(ret.size()); ++i)
	{
		auto& lesTable = ret[i];
		lesTable.whoDay = new_day + i;
		auto& lesss = lesTable.time_management;
		for (auto& lesTime : time_management)
		{

			auto randNum = randObj();
			auto randPos = randNum % act_pool_length;
			auto c = cut_one_at(randPos);
			lesss.push_back(make_pair(lesTime, move(c)));
		}
	}
	return ret;
}
//今日是否缺少计划，0否，或posetive interger who is today's serial number
inline int isTodayNeedPlan(const vector<lesson_table_one_day>& record = all_table)
{
	time_t today;
	time(&today);
	int todayWhoDay = dayWho(today);
	for (auto& ltod : record)
		if (ltod.whoDay == todayWhoDay)
		{
			return 0;
		}
	return todayWhoDay;
}
template<class Determine = function<bool(const lesson_table_one_day&)>>
inline void outputPlan(const lesson_table_one_day& lt, ostream& ous, Determine d = [](const lesson_table_one_day& l) {return true; })
{

	if (d(lt) != true)return;
	auto tmm = lt.time_management;
	sort(tmm.begin(), tmm.end());
	ous << dateFormat(undoDayWho(lt.whoDay), "Day: MM/DD\n");
	for (auto& p : tmm)
	{
		ous << format("{0}--{1}:{2}\n", dateFormat(p.first.first, "HH24:MI"), dateFormat(p.first.second, "HH24:MI"), p.second.name);
	}
}
template<typename elemT,typename _comp>
inline void fliter_vector(vector<elemT>& vs, _comp cmp)
{
	vector<elemT> nvs;
	for (auto& e : vs)
		if (cmp(e))
			nvs.emplace_back(e);
	vs.swap(nvs);
}