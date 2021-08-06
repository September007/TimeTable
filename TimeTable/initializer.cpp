#include"initializer.hpp"

int task_register(task_lib& tl, string str, task_type tsk,bool willReg)
{
	if (str == "endtask")
		tl.do_it(willReg);
	else
		tl.add_task(pair<string, task_type>(str, tsk));
	return 0;
}
