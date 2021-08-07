//this file define a class and that a object who store task
//and a method to register task to one previous object
#pragma once
#include "inc.hpp"
using task_type = function<void(void)>;
struct _comp {
	bool operator()(const pair<string, task_type>& l, const pair<string, task_type>& r) const{ return l.first < r.first; };
};
class task_lib {
	set < pair<string, task_type>,_comp> tasks;
	mutex _m;
public:
	task_lib() {};
	void add_task(const pair<string, task_type>& p) {
		tasks.insert(p);
	}
	void do_it(bool willRec = false) {
		lock_guard<mutex> lg(_m);
		for (auto& t : tasks)
		{
			t.second();
			//if (willRec);
		}
		tasks.clear();
	}
};

inline_reference_var_helper(task_lib, static_var_initer);

inline int task_register(task_lib& tl, string str, task_type tsk, bool willReg)
{
	if (str == "endtask")
		tl.do_it(willReg);
	else
		tl.add_task(pair<string, task_type>(str, tsk));
	return 0;
}
