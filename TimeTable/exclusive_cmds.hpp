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
	if (sps["-rest"].first)
		print(dec_lam{ return l.whoDay >= today; });
	else if (sps["-recent"].first)
	{
		//todo:...
	}
	else if (sps["-all"].first)
	{
		for (auto& l : vl)
			outputPlan(l, ous);
	}
	else if (sps["-n"].first)
	{
		int nextN = stoi(vs[sps["-n"].second + 1]);
		print(dec_lam{ return l.whoDay >= today && l.whoDay <= today + nextN; });
	}else if(sps["-certain"].first)
	{
		int day = stoi(vs[sps["-certain"].second + 1]);
		print(dec_lam{ return l.whoDay == day + today; });
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

inline int show_course(const vector<string>& vs, istream& ins, ostream& ous, ostream&)
{
	ous << "all courses list below:" << endl;
	if (all_course.size())ous << format("{:<10} {:<10}\n", "name", "val");
	for (auto& c : all_course)
		ous << format("{:<10} {:<10}\n", c.name, c.time_val);
	return 0;
}
inline int add_plan(const vector<string>& vs, istream& ins, ostream& ous, ostream&)
{
	//need para
	int cook_len, head_count = 1, tail_circle_count = 1;
	int today = undoDayWho(todayWhoDay()), firstDay = today;
	auto mps = transCmdParasTomap(vs);
	if(mps["-range"].first)
	{
		firstDay = today + stoi(vs[mps["-range"].second + 1]);
		cook_len = stoi(vs[mps["-range"].second + 2]) - stoi(vs[mps["-range"].second + 1]);
	}
	else
	{
		if (!mps["-len"].first)
		{
			ous << "请输入计划长度:";
			ins >> cook_len;
		}
		else
		{
			cook_len = stoi(vs[mps["-len"].second + 1]);
		}
	}
	auto cnt = cook_new_table(all_table, firstDay, default_time_split, cook_len, head_count, tail_circle_count);
	for (auto& nt : cnt)
		outputPlan(nt, ous);
	//是否保存
willstore:
	ous << "是否保存 Y/N[Y]" << flush;
	char c;
	ins >> c;
	c=toupper(c);
	if (ins && (c != 'Y' && c != 'N'))
		goto willstore;
	if (c == 'Y')
	{
		all_table.insert(all_table.end(), cnt.begin(), cnt.end());
		store_group.do_one("sta_all_table");
	}
	return 0;
}

inline int add_course(const vector<string>& vs, istream& ins, ostream& ous, ostream&)
{
	auto mps = transCmdParasTomap(vs);
	//need para
	string cname;
	unsigned int val;
	if(mps["-name"].first)
	{
		cname = vs[mps["-name"].second + 1];
	}else
	{
		ous << "输入课程名:(回车结束)" << flush;
		getline(ins, cname);
	}
	if(mps["-val"].first)
	{
		val = stoi(vs[mps["-val"].second+1]);
	}
	else
	{
		ous << "输入课程 时权:" << flush;
		ins >> val;
	}
	auto nc = course{ cname,val };
	all_course.push_back(nc);
	store_group.do_one("sta_all_course");
	ous << format("add new course: .name:{},.time_val:{}", cname, val);
	return 0;
	
}
inline int del_plan(const vector<string>& vs, istream& ins, ostream& ous, ostream&)
{
	//need para
	auto mps = transCmdParasTomap(vs);
	if (mps["-all"].first)
	{
		all_table.clear();
	}
	else
	{
		int l, r;
		if (mps["-range"].first != true)
		{
			ous << "请输入删除范围，当前日为0,del [l,r)" << flush;
			ins >> l >> r;
		}
		else {
			l = stoi(vs[mps["-range"].second + 1]);
			r = stoi(vs[mps["-range"].second + 2]);
		}
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

inline int del_course(const vector<string>& vs, istream& ins, ostream& ous, ostream&)
{
	auto mps = transCmdParasTomap(vs);
	string name;
	if(mps["-all"].first)
	{
		all_course.clear();
	}else
	if(mps["-name"].first)
	{
		name = vs[mps["-name"].second + 1];
	}else
	{
		ous << "输入课程名:(回车结束)" << flush;
		getline(ins, name);
	}
	fliter_vector(all_course, [&](const course& c) {return c.name != name; });
	store_group.do_one("sta_all_course");
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
	map<string,course> temp2;
	for (auto& l : all_course)
		if (temp2.find(l.name) == temp2.end())
			temp2.emplace(l.name, l);
	all_course.clear();
	for (auto& t : temp2)
		all_course.emplace_back(t.second);
	store_group.do_all();
	return 0;
}
inline int fresh_plan(const vector<string>& vs, istream& ins, ostream& ous, ostream& errs)
{
	auto sps = transCmdParasTomap(vs);
	int l, r;
	auto today = todayWhoDay();
	if (sps["-range"].first)
	{
		l = stoi(vs[sps["-range"].second + 1]);
		r = stoi(vs[sps["-range"].second + 2]);
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

inline int fresh_course(const vector<string>& vs, istream& ins, ostream& ous, ostream&errs)
{
	show_course(vs, ins, ous, errs);
	auto mps = transCmdParasTomap(vs);
	string from, to;
	if (mps["-from"].first)
	{
		from = vs[mps["-from"].second + 1];
	}else
	{
		ous << "源课程名:（回车结束)" << flush;
		getline(ins, from);
	}
	if (mps["-to"].first)
	{
		to = vs[mps["-to"].second + 1];
	}
	else
	{
		ous << "目的课程名:（回车结束)" << flush;
		getline(ins, to);
	}
	for_each(all_table.begin(), all_table.end(), [&](lesson_table_one_day& lt)
		{
			for (auto& l : lt.time_management)
				if (l.second.name == from)
					l.second.name = to;
		});
	for_each(all_course.begin(), all_course.end(), [&](course& l)
		{
				if (l.name == from)
					l.name = to;
		});
	store_group.do_all();
	ous << "all info changed" << endl;
	return 0;
}
inline void exclusive_default_initial_cms(msc& ncs, msec& cs)
{
	default_initial_cmds(ncs, cs);
	ncs["show plan"] = { "展示计划 ", show_plan };
	ncs["add plan"] = { "添加计划 \n 用法示例:\n add plan -len 10\nadd plan -range 0 10",add_plan };
	ncs["del plan"] = { "删除计划 \n用法示例:\n del plan -all\ndel plan -range -100 100",del_plan };
	ncs["fresh plan"] = { "刷新计划 \n用法示例:\n fresh plan -range 0 100",fresh_plan };

	ncs["show course"] = { "展示课程", show_course };
	ncs["add course"] = { "添加课程 \n用法示例:\nadd course -name yoga -val 10", add_course };
	ncs["del course"] = { "删除课程\n用法示例:\ndel course -name baba\ndel course -all",del_course };
	ncs["fresh course"] = { "刷新课程\n用法示例:\nfresh course -from \"高数\" -to \"baba\"",fresh_course };
	
	ncs["del repeat"] = { "删除重复课程和重复安排", del_repeat };
	
	ncs.erase(ncs.find("example"));
	cs["example"] = { "示例",example };
}