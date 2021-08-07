#include"inc.hpp"
#include "serialize.hpp"
#include"course_define.hpp"

//also a simple showhow of this serialization
void initialize_env()
{
	//system("if not exist data mkdir data");
	system(R"(if not exist "data/286885" mkdir "data/286885")");
	recover_group.do_all();
	if(all_course.empty())
	{
		decay_t<decltype(all_course)> new_all_course = {
			{"高数",32},
			{"现代",16},
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
	return 0;
}