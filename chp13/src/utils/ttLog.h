/**
 *  包装针对log4cxx的一些处理
 *
 *  @author zhangyi
 *  @since  2012-08-16
 */

#ifndef SERVER3_BASE_UTILS_TTLOG_H
#define SERVER3_BASE_UTILS_TTLOG_H

#include <string>
#include <set>
using namespace std;
#include <utils/CSingletonT.h>
#include <boost/asio/io_service.hpp>
#include <boost/thread/thread.hpp>
#include <boost/format.hpp>
#include <log4cxx/logger.h>
#include <log4cxx/level.h>

enum ELogLevel
{
    LLV_TRACE,
    LLV_DEBUG,
    LLV_INFO,
    LLV_WARN,
    LLV_ERROR,
    LLV_FATAL,
    _C_LEVEL_COUNT
};

static const int LOG4CXX_LEVEL_INT[_C_LEVEL_COUNT] = {
    log4cxx::Level::TRACE_INT,
    log4cxx::Level::DEBUG_INT,
    log4cxx::Level::INFO_INT,
    log4cxx::Level::WARN_INT,
    log4cxx::Level::ERROR_INT,
    log4cxx::Level::FATAL_INT,
};

namespace utils
{
    void appendLogFileName(
        const string &strLoggername,
        const string &strAppenderName,
        const string &strPostFix
        );
    void removeOldAppendedFiles(
        const string &strLoggername,
        const string &strAppenderName,
        const string &strPostFix
        );

    class CLogCenter : public utils::CSingletonT<CLogCenter>
    {
    public:
        CLogCenter();
        virtual ~CLogCenter();

        void enableLog(bool bEnabled);
        bool logEnabled() const;
        bool conLogEnabled(int nID) const;
        bool fileLogEnabled(int nID) const;

        void init(string strLoggerCfgFile, bool bWatch);
        void appendDate();
        void start();

        void setConDbgID(int nID);
        void setFileDbgID(int nID);

/*
        void asyncDebug(const string & strMessage);
        void asyncStdLog(ELogLevel level, const string & strMessage);
        void asyncDbgLog(const string & strMessage);
        void asyncLog(const string & strLoggerName, ELogLevel level, const string & strMessage);
*/

    protected:

        bool m_bEnabled;

#ifdef TT_ASYNC_LOG
        boost::shared_ptr<boost::asio::io_service::work>    m_pWork;
        boost::thread   * m_pThread;
        // This code is shit...
        // 定义成static是为了让其生命周期长于rpc client....
        // 以后通过改rpc client（如让rpc client内部的io_service是shared ptr而非引用）来从根本上解决问题。
        static boost::asio::io_service s_ioService;

        void doRunIOService();
#endif // TT_ASYNC_LOG

        void doLog(const string & strLoggerName, ELogLevel level, const string & strMessage);

        int m_nConDbgID;
        std::set<int> m_fileDbgIds;
    };
}

void ttStdLogNew(const log4cxx::LoggerPtr& logger, const log4cxx::LevelPtr& level, const char * const szFormat, ... );
void ttDebugNew( const log4cxx::LoggerPtr& logger, const char * const szFormat, ... );
void ttDbgLogNew( const log4cxx::LoggerPtr& logger, int nID, const char * const szFormat, ... );

void ttDebug( int nID, const char * const szFormat, ... );
void ttStdLog( ELogLevel level, const char * const szFormat, ... );
void ttDbgLog( int nID, const char * const szFormat, ... );
void ttLog(const char * const szLoggerName, ELogLevel level, const char * const szFormat, ... );

#define TT_STDLOG(lv, szFormat, ...)    \
    if (utils::CLogCenter::instance()->logEnabled())    \
    {   \
        log4cxx::LoggerPtr logger = log4cxx::Logger::getLogger("TTStdFile");    \
        log4cxx::LevelPtr level = log4cxx::Level::toLevel(LOG4CXX_LEVEL_INT[lv]);   \
        if (logger->isEnabledFor(level))    \
        {   \
            ttStdLogNew(logger, level, szFormat,##__VA_ARGS__);\
        }   \
    }

#define TT_PMWLOG(lv, szFormat, ...)    \
    if (utils::CLogCenter::instance()->logEnabled())    \
    {   \
        log4cxx::LoggerPtr logger = log4cxx::Logger::getLogger("TTPmwFile");    \
        log4cxx::LevelPtr level = log4cxx::Level::toLevel(LOG4CXX_LEVEL_INT[lv]);   \
        if (logger->isEnabledFor(level))    \
        {   \
            ttStdLogNew(logger, level, szFormat,##__VA_ARGS__);\
        }   \
    }

#define TT_FILELOG(lv, file, szFormat, ...)    \
    if (utils::CLogCenter::instance()->logEnabled())    \
    {   \
        log4cxx::LoggerPtr logger = log4cxx::Logger::getLogger(file);    \
        log4cxx::LevelPtr level = log4cxx::Level::toLevel(LOG4CXX_LEVEL_INT[lv]);   \
        if (logger->isEnabledFor(level))    \
        {   \
        ttStdLogNew(logger, level, szFormat,##__VA_ARGS__);\
        }   \
    }

#define TT_DEBUG(nID, szFormat, ...)    \
    if (utils::CLogCenter::instance()->conLogEnabled(nID))  \
    {   \
        log4cxx::LoggerPtr logger = log4cxx::Logger::getLogger("TTConsole");    \
        if (LOG4CXX_UNLIKELY(logger->isDebugEnabled())) \
        {   \
            ttDebugNew(logger, szFormat,##__VA_ARGS__);\
        }   \
    }

#define TT_DBGLOG(nID, szFormat, ...)   \
    if (utils::CLogCenter::instance()->fileLogEnabled(nID)) \
    {   \
        log4cxx::LoggerPtr logger = log4cxx::Logger::getLogger("TTDbgFile");    \
        if (LOG4CXX_UNLIKELY(logger->isDebugEnabled())) \
        {   \
            ttDbgLogNew(logger, nID, szFormat,##__VA_ARGS__);\
        }   \
    }

#define XT_DEBUG(nID, f)    \
    if (utils::CLogCenter::instance()->conLogEnabled(nID))  \
    {   \
        log4cxx::LoggerPtr logger = log4cxx::Logger::getLogger("TTConsole");    \
        if (LOG4CXX_UNLIKELY(logger->isDebugEnabled())) \
        {   \
            ::log4cxx::helpers::MessageBuffer oss_; \
            logger->forcedLog(::log4cxx::Level::getDebug(), oss_.str(oss_ << (f).str()), LOG4CXX_LOCATION); \
        }   \
    }

#define XT_FILELOG(lv, file, f)    \
    if (utils::CLogCenter::instance()->logEnabled())    \
    {   \
        log4cxx::LoggerPtr logger = log4cxx::Logger::getLogger(file);    \
        log4cxx::LevelPtr level = log4cxx::Level::toLevel(LOG4CXX_LEVEL_INT[lv]);   \
        if (logger->isEnabledFor(level))    \
        {   \
          ::log4cxx::helpers::MessageBuffer oss_; \
          logger->forcedLog(level, oss_.str(oss_ << (f).str()), LOG4CXX_LOCATION);    \
         }   \
    }

#define XT_STDLOG(lv, f)    \
    if (utils::CLogCenter::instance()->logEnabled())    \
    {   \
        log4cxx::LoggerPtr logger = log4cxx::Logger::getLogger("TTStdFile");    \
        log4cxx::LevelPtr level = log4cxx::Level::toLevel(LOG4CXX_LEVEL_INT[lv]);   \
        if (logger->isEnabledFor(level))    \
        {   \
            ::log4cxx::helpers::MessageBuffer oss_; \
            logger->forcedLog(level, oss_.str(oss_ << (f).str()), LOG4CXX_LOCATION);    \
        }   \
    }

#ifdef WIN32
#define XT_OBJLOG(lv, f)    \
    if (utils::CLogCenter::instance()->logEnabled())    \
    {   \
        log4cxx::LoggerPtr logger = log4cxx::Logger::getLogger("TTStdFile");    \
        log4cxx::LevelPtr level = log4cxx::Level::toLevel(LOG4CXX_LEVEL_INT[lv]);   \
        if (logger->isEnabledFor(level))    \
        {   \
            std::stringstream ss;   \
            ss << "[" << this << "] [" << __FUNCTION__ << "] "; \
            ::log4cxx::helpers::MessageBuffer oss_; \
            logger->forcedLog(level, oss_.str(oss_ << ss.str() << (f).str()), LOG4CXX_LOCATION);    \
        }   \
    }
#else
#define XT_OBJLOG(lv, f)    \
    if (utils::CLogCenter::instance()->logEnabled())    \
    {   \
        log4cxx::LoggerPtr logger = log4cxx::Logger::getLogger("TTStdFile");    \
        log4cxx::LevelPtr level = log4cxx::Level::toLevel(LOG4CXX_LEVEL_INT[lv]);   \
        if (logger->isEnabledFor(level))    \
        {   \
            std::stringstream ss;   \
            ss << "[" << this << "] [" << typeid(*this).name() << "::" << __FUNCTION__ << "] "; \
            ::log4cxx::helpers::MessageBuffer oss_; \
            logger->forcedLog(level, oss_.str(oss_ << ss.str() << (f).str()), LOG4CXX_LOCATION);    \
        }   \
    }
#endif // WIN32

#define XT_FUNLOG(lv, f)    \
    if (utils::CLogCenter::instance()->logEnabled())    \
    {   \
        log4cxx::LoggerPtr logger = log4cxx::Logger::getLogger("TTStdFile");    \
        log4cxx::LevelPtr level = log4cxx::Level::toLevel(LOG4CXX_LEVEL_INT[lv]);   \
        if (logger->isEnabledFor(level))    \
        {   \
            std::stringstream ss;   \
            ss << "[" << __FUNCTION__ << "] "; \
            ::log4cxx::helpers::MessageBuffer oss_; \
            logger->forcedLog(level, oss_.str(oss_ << ss.str() << (f).str()), LOG4CXX_LOCATION);    \
        }   \
    }

#define XT_DBGLOG(nID, f)   \
    if (utils::CLogCenter::instance()->fileLogEnabled(nID)) \
    {   \
        log4cxx::LoggerPtr logger = log4cxx::Logger::getLogger("TTDbgFile");    \
        if (LOG4CXX_UNLIKELY(logger->isDebugEnabled())) \
        {   \
            ::log4cxx::helpers::MessageBuffer oss_; \
            logger->forcedLog(::log4cxx::Level::getDebug(), oss_.str(oss_ << (boost::format("[dbg_id=%1%] %2%") % nID % (f).str()).str()), LOG4CXX_LOCATION); \
        }   \
    }

#define BF  boost::format

#define STD_LOG_DEBUG(log) XT_STDLOG(LLV_DEBUG, log)
#define STD_LOG_INFO(log)  XT_STDLOG(LLV_INFO, log)
#define STD_LOG_WARN(log)  XT_STDLOG(LLV_WARN, log)
#define STD_LOG_ERROR(log) XT_STDLOG(LLV_ERROR, log)
#define STD_LOG_FATAL(log) XT_STDLOG(LLV_FATAL, log)
#define STD_LOG_TRACE(log) XT_STDLOG(LLV_TRACE, log)

#endif
