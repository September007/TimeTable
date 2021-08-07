#include "pch.h"
#include"../TimeTable/time_transform.hpp"
TEST(time_transform,justThis)
{
	string timeStrs[] = {
		"1970/02/01 01:01:00",
		"2000/12/30 23:59:59",
		"2021/08/07 18:20:00"
	};
	string timeStrs2[] = {
		"1970-02-01 01/01/00",
		"2000-12-30 23/59/59",
		"2021-08-07 18/20/00"
	};
	for(auto str:timeStrs)
	{
		auto tempTT = str2time_t(str, "%4d/%2d/%2d %2d:%2d:%2d");
		auto circleResult = time_t2_string(tempTT, "%4.4d/%2.2d/%2.2d %2.2d:%2.2d:%2.2d");
		EXPECT_EQ(str, circleResult);
	}
	for(auto str:timeStrs2)
	{
		auto tempTT = str2time_t(str, "%4d-%2d-%2d %2d/%2d/%2d");
		auto circleResult = time_t2_string(tempTT, "%4.4d-%2.2d-%2.2d %2.2d/%2.2d/%2.2d");
		EXPECT_EQ(str, circleResult);
	}
}