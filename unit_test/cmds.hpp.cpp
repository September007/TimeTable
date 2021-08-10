#include"pch.h"
#include"../TimeTable/cmds.hpp"

struct X
{
	int id;
	int dod(int i,ostream&ous){
		ous << id << " is do" << i << endl;
		return id + 1;
	}
};
TEST(cmds,rebind)
{
	X x{ .id = 9 };
	stringstream ss;
	auto p = bind(&X::dod,x,placeholders::_1,placeholders::_2);
	auto reet=p(8,ss);
	EXPECT_EQ(ss.str(), "9 is do8\n");
	EXPECT_EQ(reet, 10);
	
}