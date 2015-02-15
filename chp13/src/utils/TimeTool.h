#ifndef UTILS_TIME_H
#define UTILS_TIME_H

#ifdef WIN32
#define LOCAL_TIME(t, result) localtime_s(&result, &t);
#else
#define LOCAL_TIME(t, result) localtime_r(&t, &result);
#endif

#ifdef WIN32
#define UTC_TIME(t, result) gmtime_s(&result, &t);
#else
#define UTC_TIME(t, result) gmtime_r(&t, &result);
#endif

#include <string>
#include <boost/thread/thread_time.hpp>
#include <common/type.h>

namespace utils
{
    using namespace std;

    
    /**
    * 得到boost::system_time格式的时间，该时间为与当前时间相差seconds秒
    */
    boost::system_time fromNow(double seconds);

    /**
    * 使当前线程sleep一定的时间
    * @param seconds: 休息的时间长短，单位为秒，不允许seconds < 0
    */
    void sleep(double seconds);

    /**
    * 秒数到日期时间
    */
    r_int64 secondsToTime(r_int32 seconds);

    /**
    * 毫秒数到日期时间
    */
    r_int64 millisecondsToTime(r_int64 milliseconds);

    /**
    * 秒数到日期
    */
    r_int32 secondsToDate(r_int64 seconds);

    /**
    * 毫秒数到日期
    */
    r_int32 millisecondsToDate(r_int64 milliseconds);

    /**
    * 秒数到日期时间
    */
    void secondsToTime(r_int32 seconds, r_int32& date, r_int32& time);

    /**
    * 毫秒数到日期时间
    */
    void millisecondsToTime(r_int64 milliseconds, r_int32& date, r_int32& time);

    /**
    * 日期时间到秒数
    */
    r_int64 timeToSeconds(r_int32 year, r_int32 month, r_int32 day, r_int32 hour, r_int32 minute, r_int32 second);

    /**
     *  秒数到日期时间
     */
    void secondsToDateTime(r_int64 theSeconds, r_int32 & year, r_int32 & month, r_int32 & day, r_int32 & hour, r_int32 & minute, r_int32 & second);

    /**
    * 日期时间到毫秒数
    */
    r_int64 timeToMilliseconds(r_int32 year, r_int32 month, r_int32 day, r_int32 hour, r_int32 minute, r_int32 second);

    /**
    * 日期时间到秒数
    */
    r_int64 timeToSeconds(r_int32 date, r_int32 time);

    /**
    * 日期时间到毫秒数
    */
    r_int64 timeToMilliseconds(r_int32 date, r_int32 time);

    /**
    * 日期时间到秒数
    * @param timeStr 标准格式如 "2009-07-14 01:13:30"
    */
    r_int64 timeToSeconds(string timeStr);

    /**
    * 日期时间到毫秒数
    * @param timeStr 标准格式如 "2009-07-14 01:13:30"
    */
    r_int64 timeToMilliseconds(string timeStr);

    /**
    * 日期时间到秒数
    * @param datetime - 格式为： 20090714011330 (==>"2009-07-14 01:13:30")
    */
    r_int64 timeToSeconds(r_int64 datetime);

    /**
    * 日期时间到毫秒数
    */
    r_int64 timeToMilliseconds(r_int64 datetime);

    /**
    * 秒数到20111101这样的日期，东八区
    */
    r_int32 d8_secondToDate(const r_int64& second);
    void d8_secondToDateTime(const r_int32& second, r_int32& date, r_int32& time);
    r_int32 d8_datetimeToSecond(const r_int32& date, const r_int32& time);

    /**
     *  获取某一个时刻相对于当天0点的秒偏移，东八区
     */
    r_int32 d8_getSecondFromZero(r_int64 second);

    /**
    * 获取当前时刻对今天0点的秒偏移，东八区
    */
    r_int32 d8_getSecondFromTodayZero();

    /*
    得到系统从开始运行到现在的秒数，毫秒级别精度计时，windows下精度能达到1e-5秒
    */
    double clock();

    /*
    得到系统从开始运行到现在的毫秒数，毫秒级别精度计时，windows下精度能达到1e-5秒
    */
    double clockMSec();

    /*
    得到当前时间点，毫秒级别精度计时，windows下精度能达到1e-5秒
    */
    double floattime();
    
    /*
    得到任何一个时点所在天的第一个毫秒
    */
    inline r_int64 d8_getmSecondFromZero(r_int64 second)
    {
        const r_int64 c_mod = 8 * 3600000;
        return second-(second + c_mod) % 86400000;
    }
    /*
    70年1月1日周四
    得到任何一个时点是周几
    */
    inline r_int8 getWeekDay(r_int64 time)
    {
        r_int8 weekday = (time / 86400000 + 5) % 7;
        if (weekday == 0)
        {
            return 7;
        }
        return weekday;
    }

    /*
    70年1月1日周四
    得到任何一个时点所在周的最后一个毫秒
    */
    inline r_int64 getWeekEnd(r_int64 time)
    {
        return (7 - getWeekDay(time)) * 86400000 + (time / 86400000 + 1) * 86400000 - 1;
    }

    /*
    从90504到“09：05：04”的转换
    */
    string getStrTimeByTime(r_int32 time);

    /*
    从70年的时间偏差拿到x年x月x日 14:23:22文字的func
    当onlyToday为真的时候，返回的只有14:23:22
    */
    string getFullTextTimeByTime(double time, bool onlyToday = true);

    /*
    得到任何一个时点所在月的最后一个毫秒
    */
    inline r_int64 getMonthEnd(r_int64 time)
    {
        r_int32 date = utils::millisecondsToDate(time);
        r_int32 year = date / 10000;
        r_int32 month = date % 10000 / 100;
        r_int32 day = 31;
        if (month == 2) 
        {
            if (year % 400 == 0 || (year % 4 == 0 && year % 100 != 0)) //闰年
            {
                day = 29;
            }
            else
            {
                day = 28;
            }
        }
        else if (month == 4 || month == 6 || month == 9 || month == 11)
        {
            day = 30;
        }

        return utils::timeToMilliseconds(date + day - date % 100, 0) + 86399999;
    }

    /*
    得到任何一个时点所在季度的最后一个毫秒
    */
    inline r_int64 getQuarterEnd(r_int64 time)
    {
        r_int32 date = utils::millisecondsToDate(time);
        r_int32 year = date / 10000;
        r_int32 month = date % 10000 / 100;
        r_int32 day = 31;
        if (month < 4) 
        {
            month = 3;
            day = 31;
        }
        else if (month > 3 && month < 7)
        {
            month = 6;
            day = 30;
        }
        else if (month > 6 && month < 10)
        {
            month = 9;
            day = 30;
        }
        else
        {
            month = 12;
            day = 31;
        }
        return utils::timeToMilliseconds(year * 10000 + month * 100 + day, 0) + 86399999;
    }

    /*
    得到任何一个时点所在半年的最后一个毫秒
    */
    inline r_int64 getHalfYearEnd(r_int64 time)
    {
        r_int32 date = utils::millisecondsToDate(time);
        r_int32 year = date / 10000;
        r_int32 month = date % 10000 / 100;
        r_int32 day = 31;
        if (month < 7) 
        {
            month = 6;
            day = 30;
        }
        else
        {
            month = 12;
        }

        return utils::timeToMilliseconds(year * 10000 + month * 100 + day, 0) + 86399999;
    }

    /*
    得到任何一个时点所在年的最后一个毫秒
    */
    inline r_int64 getYearEnd(r_int64 time)
    {
        r_int32 date = utils::millisecondsToDate(time);
        return utils::timeToMilliseconds(date / 10000 * 10000 + 1231, 0) + 86399999;
    }

    bool isInCallAuction(const string &market, const r_int32 time);
}

#endif //UTILS_Time_H