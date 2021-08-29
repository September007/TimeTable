#pragma once
//#include<timezoneapi.h>
#include <cstdio>
#include <ctime>
#include<string>
#include<format>
using std::format;
using std::string;
using std::to_string;
using std::_Buffer_to_uppercase;
//#define time_zone 

inline string time_t2_string(time_t tt,const char* format="%4.4d/%2.2d/%2.2d %2.2d:%2.2d:%2.2d")
{
    struct tm tm1;
    char szTime[128];
#if defined(time_zone)
    //+8时区
    tt -= 8 * 3600;
#endif
#ifdef WIN32
    tm1 = *localtime(&tt);
#else
    localtime_s(&tm1, &tt);
#endif
    sprintf(szTime,format ,
        tm1.tm_year + 1900, tm1.tm_mon + 1, tm1.tm_mday,
        tm1.tm_hour, tm1.tm_min, tm1.tm_sec);
    return string(szTime);
}


//指定YYYYMMDDHH24MISS型的时间，格式化为time_t型的时间
inline time_t str2time_t(const string &szTime,const char* format="%4d/%2d/%2d %2d:%2d:%2d")
{
    struct tm tm1;
    time_t tt;

    sscanf(szTime.c_str(), format,
        &tm1.tm_year,
        &tm1.tm_mon,
        &tm1.tm_mday,
        &tm1.tm_hour,
        &tm1.tm_min,
        &tm1.tm_sec);

    tm1.tm_year -= 1900;
    tm1.tm_mon--;
   // tm1.tm_hour += 7;

    tm1.tm_isdst = -1;

    tt = mktime(&tm1);
    if (tt == -1)throw std::exception("tm out of range!!");
#if defined(time_zone)
	//+8时区
    tt += 8 * 60 * 60;
#endif
    return tt;
}

inline constexpr int daySeconds = 24 * 60 * 60;
//time_t is long long ,but normal date range does not overflow from int
//return the serial number from 1970.1.1
inline int dayWho(time_t tt)
{
    return (tt+8*3600) / daySeconds;
}
inline int todayWhoDay()
{
    time_t today;
    time(&today);
    int todayWhoDay = dayWho(today);
    return todayWhoDay;
}
inline time_t undoDayWho(int daywho)
{
    return daywho * daySeconds - 8 * 3600;
}
inline string dateFormat(time_t tt,const string &fs="YYYY/MM/DD HH24:MI:SS")
{
    string ret = fs;
    tm ftm;
#ifdef WIN32
    ftm = *localtime(&tt);
#else
    localtime_s(&ftm, &tt);
#endif
    _Buffer_to_uppercase(ret.data(), ret.data()+ret.size());
    auto rep = [](string& ret, string::size_type _Off, size_t _Nx, string _Right)
    {
        if (_Off != string::npos)
            ret.replace(_Off, _Nx, _Right);
    };
    rep(ret,ret.find("YYYY"),sizeof "YYYY"-1,to_string(ftm.tm_year+1900));
    rep(ret,ret.find("MM"), 2, format("{:0>2}", ftm.tm_mon+1));
    rep(ret,ret.find("DD"), 2, format("{:0>2}", ftm.tm_mday));
    rep(ret,ret.find("HH24"), 4, format("{:0>2}",ftm.tm_hour));
    rep(ret,ret.find("HH"), 2, format("{:0>2}", ftm.tm_hour%12));
    rep(ret,ret.find("MI"), 2, format("{:0>2}", ftm.tm_min));
    rep(ret,ret.find("SS"), 2, format("{:0>2}", ftm.tm_sec));
    return ret;
}
