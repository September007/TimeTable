#include "pch.h"
#include"../TimeTable/time_transform.hpp"
#include<vector>
using std::vector;
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
TEST(time_transform,dayWho)
{
	vector<time_t >days = {
		//str2time_t("1970/01/01 00:00:00"),
		str2time_t("1970/01/02 00:00:00"),
		str2time_t("1970/01/03 00:00:00"),
		str2time_t("1970/02/01 00:00:00"),
		str2time_t("1970/02/02 00:00:00"),

		str2time_t("1970/02/03 00:00:00"),
		str2time_t("2011/01/01 00:00:00"),
		str2time_t("2011/02/01 00:00:00"),
		str2time_t("2011/02/02 00:00:00"),
	}, daysMoreAlmostOneDay = {
		//str2time_t("1970/01/01 23:59:59"),
		str2time_t("1970/01/02 23:59:59"),
		str2time_t("1970/01/03 23:59:59"),
		str2time_t("1970/02/01 23:59:59"),
		str2time_t("1970/02/02 23:59:59"),

		str2time_t("1970/02/03 23:59:59"),
		str2time_t("2011/01/01 23:59:59"),
		str2time_t("2011/02/01 23:59:59"),
		str2time_t("2011/02/02 23:59:59"),
	};
	vector<int> dws, dwms;
	for (auto day : days)
		dws.push_back(dayWho(day));
	for (auto day : daysMoreAlmostOneDay)
		dwms.push_back(dayWho(day));
	for (int i = 0; i < days.size(); ++i)
		EXPECT_EQ(dws[i], dwms[i]);
	EXPECT_EQ(dws[0], 1);
	EXPECT_EQ(dws[1], 2);
	EXPECT_EQ(dws[2], 31);
	EXPECT_EQ(dws[3], 32);
	EXPECT_EQ(dws[4], 33);
	EXPECT_EQ(dws[6] - dws[5], 31);
	EXPECT_EQ(dws[7] - dws[6], 1);
}

TEST(time_transeform,time_format)
{
	EXPECT_EQ(format("{1}{0}", 1, 2), "21");
	EXPECT_EQ(dateFormat(str2time_t("2023/12/13 13:14:15"),"YYYY*MM*DD--HH24[MI[SS]]"), "2023*12*13--13[14[15]]");
}