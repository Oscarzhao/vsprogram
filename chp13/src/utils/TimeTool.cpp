#include "common/Stdafx.h"
#include <boost/lexical_cast.hpp>
#include "utils/TimeTool.h"
#ifdef _WIN32_WINNT
#include <windows.h>
#else
#include<sys/time.h>
#endif


boost::system_time utils::fromNow(double seconds)
{
    boost::xtime xt;
    boost::xtime_get(&xt, boost::TIME_UTC);
    xt.sec += (int)seconds;
    xt.nsec += (seconds - (int)seconds) * 1e9;
    return xt;
}

void utils::sleep(double seconds)
{
    boost::thread::sleep(fromNow(seconds));
}

string utils::getStrTimeByTime(r_int32 time)
{
    char* str = new char[9];
    sprintf(str, "%02d:%02d:%02d", time / 10000, time % 10000 / 100, time % 100);
    string t(str);
    delete str;
    return t;
}

string utils::getFullTextTimeByTime(double dTime, bool onlyToday)
{
    r_int32 date, time;
    utils::secondsToTime((int)dTime, date, time);
    char* str = new char[24];
    if (onlyToday)
    {
        sprintf(str, "%02d:%02d:%02d", time / 10000, time % 10000 / 100, time % 100);
    }
    else
    {
        sprintf(str, "%04d年%02d月%02d日 %02d:%02d:%02d", date /10000,  date % 10000 / 100, date % 100, time / 10000, time % 10000 / 100, time % 100);
    }
    string t(str);
    delete str;
    return t;
}

r_int32 utils::d8_getSecondFromZero(r_int64 second)
{
    const r_int64 c_mod = 8 * 3600;
    return (second + c_mod) % 86400;
}

r_int32 utils::d8_getSecondFromTodayZero()
{
    r_int64 nowsecond = time(NULL);
    return d8_getSecondFromZero(nowsecond);
}

r_int64 utils::secondsToTime(r_int32 seconds)
{
    time_t _seconds = seconds;
    r_int64 _time;
    struct tm tt;

    LOCAL_TIME(_seconds, tt);

    _time = (tt.tm_year + 1900) * 10000 + (tt.tm_mon + 1) * 100 + tt.tm_mday;
    _time = _time * 1000000 + tt.tm_hour * 10000 + tt.tm_min * 100 + tt.tm_sec;

    return _time;
}

r_int64 utils::millisecondsToTime(r_int64 milliseconds)
{
    time_t _seconds = milliseconds / 1000;
    r_int64 _time;
    struct tm tt;

    LOCAL_TIME(_seconds, tt);

    _time = (tt.tm_year + 1900) * 10000 + (tt.tm_mon + 1) * 100 + tt.tm_mday;
    _time = _time * 1000000 + tt.tm_hour * 10000 + tt.tm_min * 100 + tt.tm_sec;

    return _time;
}

r_int32 utils::secondsToDate(r_int64 seconds)
{
    time_t _seconds = seconds;
    struct tm tt;

    LOCAL_TIME(_seconds, tt);

    return (tt.tm_year + 1900) * 10000 + (tt.tm_mon + 1) * 100 + tt.tm_mday;
}

r_int32 utils::millisecondsToDate(r_int64 milliseconds)
{
    return secondsToDate(milliseconds / 1000);
}

void utils::secondsToTime(r_int32 seconds, r_int32& date, r_int32& time)
{
    time_t _seconds = seconds;
    struct tm tt;

    LOCAL_TIME(_seconds, tt);
    
    date = (tt.tm_year + 1900) * 10000 + (tt.tm_mon + 1) * 100 + tt.tm_mday;
    time = tt.tm_hour * 10000 + tt.tm_min * 100 + tt.tm_sec;
}

void utils::millisecondsToTime(r_int64 milliseconds, r_int32& date, r_int32& time)
{
    time_t _seconds = milliseconds / 1000;
    struct tm tt;

    LOCAL_TIME(_seconds, tt);

    date = (tt.tm_year + 1900) * 10000 + (tt.tm_mon + 1) * 100 + tt.tm_mday;
    time = tt.tm_hour * 10000 + tt.tm_min * 100 + tt.tm_sec;
}

r_int64 utils::timeToSeconds(r_int32 year, r_int32 month, r_int32 day, r_int32 hour, r_int32 minute, r_int32 second)
{
    if (0 >= (int) (month -= 2))
    {
        /**//* 1..12 -> 11,12,1..10 */
        month += 12;      /**//* Puts Feb last since it has leap day */
        year -= 1;
    }

    return ((((r_int32) (year / 4 - year / 100 + year / 400 + 367 * month / 12 + day) + year * 365 - 719499) * 24 + hour - 8) /**//* now have hours */ /* we are in china which is in east 8th ditrict */
        * 60 + minute) /**//* now have minutes */
        * 60 + second; /**//* finally seconds */
}

void utils::secondsToDateTime( r_int64 theSeconds, r_int32 & year, r_int32 & month, r_int32 & day, r_int32 & hour, r_int32 & minute, r_int32 & second )
{
    r_int32 nDate = 0;
    r_int32 nTime = 0;
    utils::d8_secondToDateTime(theSeconds, nDate, nTime);

    year = nDate / 10000;
    nDate %= 10000;
    month = nDate / 100;
    nDate %= 100;
    day = nDate;

    hour = nTime / 10000;
    nTime %= 10000;
    minute = nTime / 100;
    nTime %= 100;
    second = nTime;
}

r_int64 utils::timeToMilliseconds(r_int32 year, r_int32 month, r_int32 day, r_int32 hour, r_int32 minute, r_int32 second)
{
    return (r_int64)timeToSeconds(year, month, day, hour, minute, second) * 1000;
}

r_int64 utils::timeToSeconds(r_int32 date, r_int32 time)
{
    return timeToSeconds(date / 10000, date % 10000 / 100, date % 100, time / 10000, time % 10000 / 100, time % 100);
}

r_int64 utils::timeToMilliseconds(r_int32 date, r_int32 time)
{
    return (r_int64)timeToSeconds(date, time) * 1000;
}

r_int64 utils::timeToSeconds(r_int64 datetime)
{
    r_int64 _date = datetime / 1000000;
    r_int64 _time = datetime % 1000000;

    return timeToSeconds(_date / 10000, _date % 10000 / 100, _date % 100, _time / 10000, _time % 10000 / 100, _time % 100);
}

r_int64 utils::timeToMilliseconds(r_int64 datetime)
{
    return (r_int64)timeToSeconds(datetime) * 1000;
}

r_int32 utils::d8_secondToDate(const r_int64& second)
{
    time_t _seconds = second + 8 * 3600;
    struct tm tt;
    UTC_TIME(_seconds, tt);
    return (tt.tm_year + 1900) * 10000 + (tt.tm_mon + 1) * 100 + tt.tm_mday;
}

void utils::d8_secondToDateTime(const r_int32& second, r_int32& date, r_int32& time)
{
    time_t _seconds = second + 8 * 3600;
    struct tm tt;

    UTC_TIME(_seconds, tt);

    date  = (tt.tm_year + 1900) * 10000 + (tt.tm_mon + 1) * 100 + tt.tm_mday;    
    time = tt.tm_hour * 10000 + tt.tm_min * 100 + tt.tm_sec;
}

r_int32 utils::d8_datetimeToSecond(const r_int32& date, const r_int32& time)
{
    r_int32 year = date / 10000;
    r_int32 month = date % 10000 / 100;
    r_int32 day = date % 100;
    r_int32 hour = time / 10000;
    r_int32 minute = time % 10000 / 100;
    r_int32 second = time % 100;

    if (0 >= (int) (month -= 2))
    {
        month += 12;
        year -= 1;
    }

    return ((((r_int32) (year / 4 - year / 100 + year / 400 + 367 * month / 12 + day) + year * 365 - 719499) * 24 + hour - 8)
        * 60 + minute) 
        * 60 + second; 
}

r_int64 utils::timeToSeconds(string timeStr)
{
    r_int64 datetime = 0;

    boost::algorithm::replace_all(timeStr, "-", "");
    boost::algorithm::replace_all(timeStr, " ", "");
    boost::algorithm::replace_all(timeStr, ":", "");

    datetime = boost::lexical_cast<r_int64>(timeStr);

    return timeToSeconds(datetime);
}

r_int64 utils::timeToMilliseconds(string timeStr)
{
    return (r_int64)timeToSeconds(timeStr) * 1000;
}

static double floattime();
double utils::clock()
{
#ifdef _WIN32_WINNT
    // 此段代码在多线程下有Bug, 慎用
    //static LARGE_INTEGER ctrStart;
    //static LARGE_INTEGER proc_freq ={0};
    //if (proc_freq.QuadPart==0)
    //{
    //    QueryPerformanceFrequency(&proc_freq);
    //    QueryPerformanceCounter(&ctrStart);
    //}
    //LARGE_INTEGER now;
    //QueryPerformanceCounter(&now);
    //return ((double)now.QuadPart-(double)ctrStart.QuadPart)/(double)proc_freq.QuadPart;

    static boost::posix_time::ptime s_start = boost::posix_time::from_time_t(0);
    boost::posix_time::ptime now = boost::posix_time::microsec_clock::universal_time();
    boost::posix_time::time_duration d = now - s_start;
    return (double)d.total_milliseconds() / 1000;
#else
    static double startedTime = 0.0;
    if(startedTime<1e-3)
        startedTime = floattime();
    return floattime()-startedTime;
#endif

    //static boost::posix_time::ptime s_start = boost::posix_time::from_time_t(0);
    //boost::posix_time::ptime now = boost::posix_time::microsec_clock::universal_time();
    //boost::posix_time::time_duration d = now - s_start;
    //return (double)d.total_milliseconds() / 1000;
}

double utils::clockMSec()
{
    return utils::clock() * 1000.0;
}

double utils::floattime()
{
    /* There are three ways to get the time:
    (1) gettimeofday() -- resolution in microseconds
    (2) ftime() -- resolution in milliseconds
    (3) time() -- resolution in seconds
    In all cases the return value is a float in seconds.
    Since on some systems (e.g. SCO ODT 3.0) gettimeofday() may
    fail, so we fall back on ftime() or time().
    Note: clock resolution does not imply clock accuracy! */
#ifndef _WIN32_WINNT
    timeval tv;
    gettimeofday(&tv, 0); //gettimeofday does not support TZ adjust on Linux.
    return (double)tv.tv_sec + tv.tv_usec*0.000001;
#endif 
    return 0.0;
}

bool utils::isInCallAuction(const string &market, const r_int32 time)
{
    bool isAuction = false;
    if(isStockMarket(market))
    {
        if(time >= 91500 && time < 92500)
            isAuction = true;
        else if(market == MARKET_SHENZHEN)
        {
            if(time >= 145700 && time <= 150000)
                isAuction = true;
        }
    }

    return isAuction;
}