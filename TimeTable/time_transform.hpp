#pragma once
#include <cstdio>
#include <ctime>
#include<string>
using std::string;
inline string time_t2_string(time_t time1,const char* format="%4.4d/%2.2d/%2.2d %2.2d:%2.2d:%2.2d")
{
    struct tm tm1;
    char szTime[128];

#ifdef WIN32
    tm1 = *localtime(&time1);
#else
    localtime_r(&time1, &tm1);
#endif
    sprintf(szTime,format ,
        tm1.tm_year + 1900, tm1.tm_mon + 1, tm1.tm_mday,
        tm1.tm_hour, tm1.tm_min, tm1.tm_sec);
    return string(szTime);
}


//ָ��YYYYMMDDHH24MISS�͵�ʱ�䣬��ʽ��Ϊtime_t�͵�ʱ��
inline time_t str2time_t(const string &szTime,const char* format="%4d/%2d/%2d %2d:%2d:%2d")
{
    struct tm tm1;
    time_t time1;

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

    time1 = mktime(&tm1);
    return time1;
}


//��������������������������������
//��Ȩ����������ΪCSDN������nanhaizhixin����ԭ�����£���ѭCC 4.0 BY - SA��ȨЭ�飬ת���븽��ԭ�ĳ������Ӽ���������
//ԭ�����ӣ�https ://blog.csdn.net/nanhaizhixin/article/details/8349668

//
inline int dayWho(time_t tt)
{
    return (tt + 1) / (24 * 60 * 60) * (24 * 60 * 60);
}