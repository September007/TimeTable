#pragma once
#include"cmds.hpp"
#include"course_define.hpp"
#include"time_transform.hpp"
inline int example(const command_parser& cp, const vector<string>& vs, istream& ins, ostream& ous, ostream& errs)
{
	ous << R"(
	echo blabla
	sys cls
	sys pause
	sys echo blabla
	stupid_shell
	exit
	dexit

	//operation on TimeTable
	add plan [-l <interger>]
	show plan [rest|recent|all>
	del plan
	fresh plan [-r <interger> <interger>]
	)";
	return 0;
}
inline int show_plan(const vector<string>& vs, istream&, ostream& ous, ostream&)
{
	auto today = todayWhoDay();
	vector<lesson_table_one_day> vl = all_table;
	sort(vl.begin(), vl.end(), [](lesson_table_one_day& l, lesson_table_one_day& r) {return l.whoDay < r.whoDay; });
	auto sps = transCmdParasTomap(vs);
	auto print = [&vl, &ous](function<bool(const lesson_table_one_day&)> f)
	{
		for (auto& l : vl)
			outputPlan(l, ous, f);
	};
#define dec_lam [&](const lesson_table_one_day&l)
	//rest plan
	if (sps["rest"].first)
		print(dec_lam{ return l.whoDay >= today; });
	else if (sps["recent"].first)
	{
		//todo:...
	}
	else if (sps["all"].first)
	{
		for (auto& l : vl)
			outputPlan(l, ous);
	}
	else if (sps["-n"].first)
	{
		int nextN = stoi(vs[sps["-n"].second + 1]);
		print(dec_lam{ return l.whoDay >= today && l.whoDay <= today + nextN; });
	}
	else
	{//default:rest
		for (auto& l : vl)
		{
			if (l.whoDay >= today)
			{
				outputPlan(l, ous);
			}
		}

	}
	return 0;
}

inline int add_plan(const vector<string>& vs, istream& ins, ostream& ous, ostream&)
{
	//need para
	int cook_len, head_count = 1, tail_circle_count = 1;
	auto mps = transCmdParasTomap(vs);
	if (!mps["-l"].first)
	{
		ous << "请输入计划长度:";
		ins >> cook_len;
	}
	else
	{
		cook_len = stoi(vs[mps["-l"].second + 1]);
	}
	auto cnt = cook_new_table(all_table, undoDayWho(todayWhoDay()), default_time_split, cook_len, head_count, tail_circle_count);
	for (auto& nt : cnt)
		outputPlan(nt, ous);
	//是否保存
willstore:
	ous << "是否保存 Y/N[Y]" << flush;
	char c;
	ins >> c;;
	toupper(c);
	if (ins && (c != 'Y' && c != 'N'))
		goto willstore;
	if (c == 'Y')
	{
		all_table.insert(all_table.end(), cnt.begin(), cnt.end());
		store_group.do_one("sta_all_table");
	}
	return 0;
}

inline int del_plan(const vector<string>& vs, istream& ins, ostream& ous, ostream&)
{
	//need para
	auto mps = transCmdParasTomap(vs);
	if (mps["all"].first)
	{
		all_table.clear();
	}
	else
	{
		int l, r;
		if (mps["-r"].first != true)
		{
			ous << "请输入删除范围，当前日为0,del [l,r)" << flush;
			ins >> l >> r;
		}
		l = stoi(vs[mps["-r"].second + 1]);
		r = stoi(vs[mps["-r"].second + 2]);
		decay_t<decltype(all_table)> nat;
		auto today = todayWhoDay();
		for (auto& t : all_table)
			if (t.whoDay >= today + l && t.whoDay < today + r)
				;
			else
				nat.emplace_back(t);
		nat.swap(all_table);
	}
	store_group.do_one("sta_all_table");
	return 0;
}
inline int del_repeat(const vector<string>& vs, istream&, ostream& ous, ostream&)
{
	map<int, lesson_table_one_day> temp;
	for (auto& l : all_table)
		if (temp.find(l.whoDay) == temp.end())
			temp.emplace(l.whoDay, l);
	all_table.clear();
	for (auto& t : temp)
		all_table.emplace_back(t.second);
	store_group.do_one("sta_all_table");
	return 0;
}
inline int fresh_plan(const vector<string>& vs, istream& ins, ostream& ous, ostream& errs)
{
	auto sps = transCmdParasTomap(vs);
	int l, r;
	auto today = todayWhoDay();
	if (sps["-r"].first)
	{
		l = stoi(vs[sps["-r"].second + 1]);
		r = stoi(vs[sps["-r"].second + 2]);
	}
	else
	{
	inputLR:
		ous << "请输入更改范围[l,r),今日为0" << flush;
		ins >> l >> r;
		if (l > r)
		{
			ous << "l>r!!!\n" << flush;
			goto inputLR;
		}
	}
fresh:
	auto nt = cook_new_table(all_table, undoDayWho(today + l), default_time_split, r - l);
	ous << "there is new plan:\n" << flush;
	for (auto& p : nt)
		outputPlan(p, ous);
confirm:
	ous << "is that what you want? Y/N/R:" << flush;
	char c;
	ins >> c;
	c = toupper(c);
	if (c != 'Y' && c != 'N' && c != 'R')
		goto confirm;
	switch (c)
	{
	case 'Y':
		fliter_vector(all_table, [&](const lesson_table_one_day& ls) {return ls.whoDay<today + l || ls.whoDay>today + r; });
		all_table.insert(all_table.end(), nt.begin(), nt.end());
		store_group.do_one("sta_all_table");
		break;
	case 'N':
		break;
	case 'R':
		goto fresh;
	}

	return 0;
};
inline void exclusive_default_initial_cms(msc& ncs, msec& cs)
{
	default_initial_cmds(ncs, cs);
	ncs["show plan"] = show_plan;
	ncs["add plan"] = add_plan;
	ncs["del plan"] = del_plan;
	ncs["del repeat"] = del_repeat;
	ncs["fresh plan"] = fresh_plan;

	ncs.erase(ncs.find("example"));
	cs["example"] = example;
}