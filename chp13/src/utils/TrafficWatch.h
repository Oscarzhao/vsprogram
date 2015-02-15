#ifndef __UTILS_TRAFFIC_WATCH_H__
#define __UTILS_TRAFFIC_WATCH_H__

#include "log4cxx/logger.h"
#include <boost/asio.hpp>
#include <boost/asio/io_service.hpp>
#include <string>
#include "net/NetCommon.h"

using namespace std;

namespace utils
{
    
    /**
    * <p>流量统计工具</p>
    * @remark 所有函数均线程安全
    */

    typedef struct _TrafficWatchItem
    {
        int lastTime;           //最近一次统计时间
        double currentTraffic;   //当前流量
        double totalTraffic;     //总流量
        double currentCount;       //当前统计数次
        double totalCount;         //总统计次数 

    }TrafficWatchItem;

    class NET_EXPORT CTrafficWatch
    {
    public:
        /**
        * @param m_reportInterval 打印流量统计信息的周期，单位：秒，默认30秒
        */
        CTrafficWatch( boost::asio::io_service& ioservice, log4cxx::LoggerPtr logger = log4cxx::LoggerPtr(), int reportInterval = 30);
        ~CTrafficWatch();

    public:
        /**
        * <p>流量监视</p>
        * @param name 流量的名称
        * @param traffic 流量值
        */
        void watch(string name, double traffic);

        /**
        * <p>取消监视</p>
        * 
        * @param name 流量的名称
        * @return non
        * @exception non
        */
        void cancel(string name);

        /**
        * <p>设置报告流量的时间间隔</p>
        * 
        * @param[in]  interval 以秒为单位
        * @return non
        * @exception non
        */
        void setTimeInterval(int interval);

        /**
        * <p>设置日志</p>
        * 
        * @param[in]  loger 日志
        * @return non
        * @exception non
        */
        void setLogger(log4cxx::LoggerPtr logger);

        static boost::shared_ptr<CTrafficWatch> getWatcher();

    private:
        /**
        * <p>定时打印流量统计信息</p>
        */
        void report(const boost::system::error_code& error);

        void print(string msg);


    private:
        boost::asio::io_service& m_ioservice;
        boost::asio::deadline_timer m_reportTimer;
        boost::recursive_mutex m_mutex; //全局锁
        map<string, TrafficWatchItem> m_trafficTable; //流量统计表
        int m_reportInterval;
        bool m_isReport;
        log4cxx::LoggerPtr m_logger;
    };
    typedef boost::shared_ptr<CTrafficWatch> TrafficWatchPtr;
}


#endif // #ifndef __UTILS_TRAFFIC_WATCH_H__