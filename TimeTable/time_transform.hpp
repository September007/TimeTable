#pragma once
//#include<timezoneapi.h>
#include <cstdio>
#include <ctime>
#include<string>
using std::string;
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
    localtime_r(&time1, &tm1);
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


//return the serial number from 1970.1.1
inline int dayWho(time_t tt)
{
    constexpr int daySeconds = 24 * 60 * 60;
    return (tt+8*3600) / daySeconds;
}