#include"pch.h"
#include<iostream>
#include<iosfwd>
#include"boost/coroutine/all.hpp"
#include"../TimeTable/cmd_parse.hpp"
using namespace std;
using std::istream;


struct Cls
{
	int a, b;
	std::istream& in = cin;
	void f() { cout << __func__ << endl; }
	int fi(int i)
	{
		cout << __func__ << endl;
		return i;
	}
};
#ifdef X64
using tempInt = int64_t;
#else
using tempInt = int32_t;
#endif
template<typename PT>
tempInt Pointer2Int(PT p) { return *(tempInt*)&p; };
template<typename PT>
PT Int2Pointer(tempInt i, PT p = nullptr)
{
	PT ret;
	*(tempInt*)&ret = i;
	return ret;
}
template<typename retType, typename ...argsType>
retType  turnCall(std::istream& in, retType(std::istream::* mfp)(argsType...), argsType&&...args)
{
	retType(std::istream:: * p)(argsType...);
	p = (void*)(1);
	return (in.*mfp)();
};
string s;
void parser(boost::coroutines::coroutine<void>::pull_type& sink1)
{
	auto p = s.begin();
	while (1)
	{
		if (p == s.end())
			sink1();
		else {
			char c = *p++;
			cout << "get :" << c << endl;
			if (c == 'e')return;
		}
	}
}

int mm()
{
	boost::coroutines::coroutine<void>::push_type source(parser);
	while (1)
	{
		source();
		s += s.size() + 'a';
		if (s.back() == 'f')break;
	}
	return 0;
}
TEST(cmd_parse, getOneCommand)
{
	stringstream ss;
	ss << R"(cm p1 p2 "p3" "p4
	p4" )";
	auto p1 = getOneCommand(ss);
	EXPECT_EQ(p1, vector<string>({ "cm", "p1", "p2", "p3", "p4\n\tp4" }));
	ss = stringstream();
	ss << R"(cm2 p\"p \"\"\")";
	auto p2 = getOneCommand(ss);
	EXPECT_EQ(p2, vector<string>({ "cm2","p\"p","\"\"\"" }));
};


TEST(cmd_parse,comand_parser)
{
	reg_commands.insert({
		make_pair<string,command_task_type>("echo",[](const vector<string> pms,istream& in,ostream& ous,ostream& errs)->int
		{
			for (int i = 1; i < pms.size(); ++i)
				ous << pms[i] << endl;
			return 0;
		}),
		make_pair<string,command_task_type>("ls",[](const vector<string> pms,istream& in,ostream& ous,ostream& errs)->int
		{
			for (int i = 0; i < 5; ++i)
				ous << i << endl;
			return 0;
		})
		});
	command_parser cp;
	stringstream ss[3];
	ss[0] << R"( echo ab "cd" \"\"ef\"\")";
	ss[1] << R"(ls bb)";
	ss[2] << R"(none ss)";
	stringstream ouss[3];
	cp(getOneCommand(ss[0]), cin, ouss[0]);
	cp(getOneCommand(ss[1]), cin, ouss[1]);
	cp(getOneCommand(ss[2]), cin, ouss[2]);
	EXPECT_EQ(ouss[0].str(), "ab\ncd\n\"\"ef\"\"\n");
	EXPECT_EQ(ouss[1].str(), "0\n1\n2\n3\n4\n");
	EXPECT_EQ(ouss[2].str(), "");
}