 //voice.hpp should be put on th head
#define WAV_PLAY
#include"Voice.hpp"
#include"inc.hpp"
#include "serialize.hpp"
#include"course_define.hpp"
#include"exclusive_cmds.hpp"
//also a simple showhow of this serialization
void initialize_env()
{
	system(R"(if not exist "data/286885" mkdir "data/286885")");
	//Ĭ�����  reg_commands ��command_parser ���캯����Ĭ�ϲ���
	exclusive_default_initial_cms(reg_commands,reg_extend_commands);
	recover_group.do_all();
	if(all_course.empty())
	{
		decay_t<decltype(all_course)> new_all_course = {
			{"����",32},
			{"�ߴ�",16}, 
			{"������",16},
			{"Ӣ��",24},
			{"unreal",32},
			{"����",8},
			{"��Ұ�Ҷ�",7}
		};
		all_course = new_all_course;
	}
	store_group.do_all();
}

int main() {
	PlaySound(L"C:/Windows/Media/Alarm02.wav",NULL, SND_ASYNC);
	initialize_env();
	stupid_shell({ "stupid_shell" }, cin, cout, cerr);
	return 0;
}