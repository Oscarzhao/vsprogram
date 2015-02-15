#include "common/Stdafx.h"

#include "log4cxx/propertyconfigurator.h"
#include "utils/TrafficWatch.h"
#include "utils/Threading.h"
#include "common/log.h"

using namespace utils;
using namespace log4cxx;


CTrafficWatch::CTrafficWatch(boost::asio::io_service& ioservice, LoggerPtr logger, int reportInterval )
:m_ioservice(ioservice),
m_reportTimer(m_ioservice),
m_logger(logger)
{
    m_reportInterval = reportInterval;
    if (m_reportInterval > 0)
    {
        m_reportTimer.expires_from_now(boost::posix_time::seconds(m_reportInterval));
        m_reportTimer.async_wait(boost::bind(&CTrafficWatch::report, this, boost::asio::placeholders::error));
    }
}

CTrafficWatch::~CTrafficWatch()
{
    boost::system::error_code error;
    m_reportTimer.cancel(error);
}

void CTrafficWatch::watch(string name, double traffic)
{
    boost::recursive_mutex::scoped_lock rlock(m_mutex);

    map<string, TrafficWatchItem>::iterator iter = m_trafficTable.find(name);

    if (iter == m_trafficTable.end())
    {
        TrafficWatchItem twItem;
        twItem.lastTime = (int)time(NULL);
        twItem.currentTraffic = traffic;
        twItem.totalTraffic = traffic;
        twItem.currentCount = 1;
        twItem.totalCount = 1;

        m_trafficTable[name] = twItem;
    }
    else
    {
        iter->second.currentTraffic += traffic;
        iter->second.totalTraffic += traffic;
        iter->second.currentCount += 1;
        iter->second.totalCount += 1;
    }
}


void CTrafficWatch::report(const boost::system::error_code& error)
{
    if (!error)
    {
        LOG4CXX_INFO(m_logger, "--------------- traffic report ---------------");

        boost::recursive_mutex::scoped_lock rlock(m_mutex);

        map<string, TrafficWatchItem>::iterator iter = m_trafficTable.begin();

        for ( ; iter != m_trafficTable.end(); iter++)
        {
            int curTime = (int)time(NULL);
            double timeSpan = curTime - iter->second.lastTime;
            double speed = 0;
            double frequency = 0;
            if (timeSpan > 1e-5)
            {
                speed = iter->second.currentTraffic / timeSpan / 1024;
                frequency = iter->second.currentCount / timeSpan;
            }

            iter->second.lastTime = curTime;
            iter->second.currentTraffic = 0;
            iter->second.currentCount  = 0;

            string log = (boost::format("%s: %.2fK/s[%.2fM], %.2fpkg/s[%.2fMpkg], %.2fB/pkg") \
                %(iter->first) %speed %(iter->second.totalTraffic/(1024*1024)) %(frequency) \
                %(iter->second.totalCount/(1024*1024)) %(iter->second.totalTraffic/iter->second.totalCount)).str();
            LOG4CXX_INFO(m_logger, log.c_str());
        }
        LOG4CXX_INFO(m_logger, "");

        if (m_reportInterval)
        {
            m_reportTimer.expires_from_now(boost::posix_time::seconds(m_reportInterval));
            m_reportTimer.async_wait(boost::bind(&CTrafficWatch::report, this, boost::asio::placeholders::error));
        }
    }
}


void CTrafficWatch::print(string msg)
{
    if (m_logger != NULL)
    {
        LOG4CXX_INFO(m_logger, msg.c_str());
    }
}


void CTrafficWatch::cancel(string name)
{
    boost::recursive_mutex::scoped_lock rlock(m_mutex);
    map<string, TrafficWatchItem>::iterator iter = m_trafficTable.find(name);

    if (iter != m_trafficTable.end())
    {
        m_trafficTable.erase(iter);
    }
}

void CTrafficWatch::setTimeInterval(int interval)
{
    boost::recursive_mutex::scoped_lock rlock(m_mutex);
    m_reportInterval = interval;

    if (m_reportInterval > 0)
    {
        boost::system::error_code error;
        m_reportTimer.cancel(error);
        m_reportTimer.expires_from_now(boost::posix_time::seconds(m_reportInterval));
        m_reportTimer.async_wait(boost::bind(&CTrafficWatch::report, this, boost::asio::placeholders::error));
    }
}


void CTrafficWatch::setLogger(log4cxx::LoggerPtr logger)
{
    m_logger = logger;
}

utils::TrafficWatchPtr CTrafficWatch::getWatcher()
{
    static utils::RecMutex s_mutex;
    static bool isFirst = true;
    static boost::asio::io_service s_ioservice;
    static boost::asio::io_service::work s_work(s_ioservice);       
    static utils::TrafficWatchPtr watcher(new utils::CTrafficWatch(s_ioservice, log4cxx::Logger::getLogger("TTStdFile")));
    utils::RecLock lock(s_mutex);
    if (isFirst)
    {
        boost::thread(boost::bind(&boost::asio::io_service::run, boost::ref(s_ioservice)));
        isFirst = false;
    }
    return watcher;
}
