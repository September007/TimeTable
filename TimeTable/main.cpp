#include"inc.hpp"
#include "serialize.hpp"
#include"course_define.hpp"
#include"cmds.hpp"
//also a simple showhow of this serialization
void initialize_env()
{
	system(R"(if not exist "data/286885" mkdir "data/286885")");
	//默认命令集
	default_initial_cmds(reg_commands);
	//recover_group.do_all();
	if(all_course.empty())
	{
		decay_t<decltype(all_course)> new_all_course = {
			{"高数",32},
			{"线代",16}, 
			{"概率论",16},
			{"英语",24},
			{"unreal",32},
			{"政治",8},
			{"荒野乱斗",7}
		};
		all_course = new_all_course;
	}
	store_group.do_all();
}

int main() {

	initialize_env();
	stupid_shell({ "stupid_shell" }, cin, cout, cerr);
	//auto p = cook_new_table(vector<lesson_table_one_day>(), str2time_t("2021/08/08 00:00:00"), default_time_split, 7, 10, 2);
	return 0;
}