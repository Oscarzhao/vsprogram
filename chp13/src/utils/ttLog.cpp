/**
 *  包装针对log4cxx的一些处理
 *
 *  @author zhangyi
 *  @since  2012-08-16
 */

#include <common/Stdafx.h>
#include <utils/ttLog.h>
#include <utils/ProcessHelper.h>
#include <utils/commonFunc.h>
#pragma warning (disable: 4231 4275 4251)
#include <log4cxx/propertyconfigurator.h>
#include <log4cxx/fileappender.h>
#include <log4cxx/helpers/transcoder.h>
#include <boost/filesystem/operations.hpp>
#include <boost/lexical_cast.hpp>
#include <utils/CSingletonT.hpp>
#include <utils/strings.h>
#include <utils/TimeTool.h>
#include <utils/ttLog.h>
#pragma warning (disable: 4231)
#include <log4cxx/propertyconfigurator.h>
#include <log4cxx/fileappender.h>
#include <log4cxx/helpers/transcoder.h>
#include <boost/lexical_cast.hpp>
#include <sys/types.h>
#ifndef _WIN32
#include <stdarg.h>
#endif

IMPL_SINGLETON_CLASS(utils::CLogCenter);

// zhangyi : 2012-08-15
// fuck，打开这个宏会发现io service post了N次，实际上只执行少数次打log
// 原因不明，work是有的，线程也一直在跑，也并没有阻塞……
// 先暂时同步的打log吧……
// #define TT_ASYNC_LOG

namespace utils
{

#if 0

    void _removeLogAppenderImpl( log4cxx::LoggerPtr logger, const string &strAppenderName )
    {
        if (NULL != logger)
        {
            LOG4CXX_DECODE_CHAR(appenderName, strAppenderName);
            logger->removeAppender(appenderName);
        }
    }

    void appendPIDtoLogFile(
        const string &strLoggername,
        const string &strAppenderName
        )
    {
        log4cxx::LoggerPtr logger = log4cxx::Logger::getLogger(strLoggername);

        string strOldLogFile = _getLogFileName(logger, strAppenderName);
        string strNewLogFile;

        // location of last '.'
        size_t idxDot = strOldLogFile.rfind('.');
        if (string::npos == idxDot)
        {
            strNewLogFile = strOldLogFile + "_" + boost::lexical_cast<string>(utils::ProcessHelper::getPid());
        }
        else
        {
            strNewLogFile = strOldLogFile.substr(0, idxDot) + "_" + boost::lexical_cast<string>(utils::ProcessHelper::getPid()) + strOldLogFile.substr(idxDot, strOldLogFile.size() - idxDot);
        }

        _resetLogFileNameImpl(logger, strAppenderName, strNewLogFile);
    }

    void removeLoggerAppender(
        const string &strLoggername,
        const string &strAppenderName
        )
    {
        log4cxx::LoggerPtr logger = log4cxx::Logger::getLogger(strLoggername);
        _removeLogAppenderImpl(logger, strAppenderName);
    }

#endif

    string _getLogFileName(
        const log4cxx::LoggerPtr & logger,
        const string &strAppenderName
        )
    {
        if (NULL != logger)
        {
            LOG4CXX_DECODE_CHAR(appenderName, strAppenderName);
            log4cxx::FileAppenderPtr appender = logger->getAppender(appenderName);

            if (NULL != appender)
            {
#if LOG4CXX_LOGCHAR_IS_WCHAR
                return wsToString(appender->getFile());
#else
                return appender->getFile();
#endif // LOG4CXX_LOGCHAR_IS_WCHAR
            }
        }

        return "";
    }

    void _resetLogFileNameImpl(
        log4cxx::LoggerPtr logger,
        const string &strAppenderName,
        const string &strLogFileName
        )
    {
        log4cxx::helpers::Pool p;

        if (NULL != logger)
        {
            LOG4CXX_DECODE_CHAR(appenderName, strAppenderName);
            log4cxx::FileAppenderPtr appender = logger->getAppender(appenderName);

            if (NULL != appender)
            {
                LOG4CXX_DECODE_CHAR(logFileName, strLogFileName);
                appender->setFile(logFileName);

                appender->activateOptions(p);
            }
        }
    }

    string _getNewLogFileName(const string & strOldLogFile, const string & strPostFix)
    {
        string strNewLogFile;

        // location of last '.'
        size_t idxDot = strOldLogFile.rfind('.');
        if (string::npos == idxDot)
        {
            strNewLogFile = strOldLogFile + "_" + strPostFix;
        }
        else
        {
            strNewLogFile = strOldLogFile.substr(0, idxDot) + "_" + strPostFix + strOldLogFile.substr(idxDot, strOldLogFile.size() - idxDot);
        }

        return strNewLogFile;
    }

    void appendLogFileName(
        const string &strLoggername,
        const string &strAppenderName,
        const string &strPostFix
        )
    {
        log4cxx::LoggerPtr logger = log4cxx::Logger::getLogger(strLoggername);

        string strOldLogFile = _getLogFileName(logger, strAppenderName);
        string strNewLogFile = _getNewLogFileName(strOldLogFile, strPostFix);

        _resetLogFileNameImpl(logger, strAppenderName, strNewLogFile);
    }

    void removeOldAppendedFiles(
        const string &strLoggername,
        const string &strAppenderName,
        const string &strPostFix
        )
    {
        log4cxx::LoggerPtr logger = (strLoggername.empty())
            ? log4cxx::Logger::getRootLogger()
            : log4cxx::Logger::getLogger(strLoggername);

        string strOldLogFile = _getLogFileName(logger, strAppenderName);
        string strNewLogFile = _getNewLogFileName(strOldLogFile, strPostFix);
        utils::string_replace(strNewLogFile, "\\", "/");
        
        string strSearchPattern;
        size_t idxDot = strOldLogFile.rfind('.');
        if (string::npos == idxDot)
        {
            strSearchPattern = strOldLogFile;
        }
        else
        {
            strSearchPattern = strOldLogFile.substr(0, idxDot);
        }
        utils::string_replace(strSearchPattern, "\\", "/");

        boost::filesystem::path p(strOldLogFile);
        p = p.parent_path();
        boost::system::error_code ec;
        if (boost::filesystem::is_directory(p, ec))
        {
            for (boost::filesystem::recursive_directory_iterator it(p, ec), eit; it != eit; ++it)
            {
                boost::filesystem::path subPath = it->path();
                string strFileName = subPath.string();
                utils::string_replace(strFileName, "\\", "/");
                if (
                    !boost::filesystem::is_directory(subPath, ec)
                    && strFileName != strNewLogFile
                    && 0 == strFileName.find(strSearchPattern)
                    )
                {
                    size_t idxDot = strFileName.rfind('.');
                    // 以client.log加日期为例
                    // 删除client.log和client_xxxxxxxx.log（旧日期）
                    if (
                        strFileName.npos != idxDot
                        && (idxDot == strSearchPattern.length() + 9 || idxDot == strSearchPattern.length())
                        )
                    {
                        boost::filesystem::remove(subPath, ec);
                    }
                }
            }
        }
    }

    //////////////////////////////////////////////////////////////////////////
    // CLogCenter
    //////////////////////////////////////////////////////////////////////////

#ifdef TT_ASYNC_LOG
    boost::asio::io_service CLogCenter::s_ioService;
#endif // TT_ASYNC_LOG

    CLogCenter::CLogCenter()
        : m_bEnabled(true)
        , m_nConDbgID(0)
#ifdef TT_ASYNC_LOG
        , m_pThread(NULL)
#endif // TT_ASYNC_LOG
    {

    }

    CLogCenter::~CLogCenter()
    {
#ifdef TT_ASYNC_LOG
        if (!s_ioService.stopped())
        {
            s_ioService.stop();
            while (!s_ioService.stopped())
            {
            }
        }
#endif // TT_ASYNC_LOG
    }

    void CLogCenter::enableLog( bool bEnabled )
    {
        m_bEnabled = bEnabled;
    }

    bool CLogCenter::logEnabled() const
    {
        return m_bEnabled;
    }

    bool CLogCenter::conLogEnabled( int nID ) const
    {
        return m_bEnabled && (nID == m_nConDbgID);
    }

    bool CLogCenter::fileLogEnabled( int nID ) const
    {
        return m_bEnabled && (m_fileDbgIds.end() != m_fileDbgIds.find(nID));
    }

    void CLogCenter::init( string strLoggerCfgFile, bool bWatch )
    {
        // zhangyi : 2012-11-15
        // string->wstring，解决有中文路径时无法打LOG的问题。
        wstring wsLogCfgFile = sToWString(strLoggerCfgFile);
        if (bWatch)
        {
            log4cxx::PropertyConfigurator::configureAndWatch(wsLogCfgFile);
        }
        else
        {
            log4cxx::PropertyConfigurator::configure(wsLogCfgFile);
        }

#ifdef TT_ASYNC_LOG
        m_pWork = boost::shared_ptr<boost::asio::io_service::work> (new boost::asio::io_service::work(s_ioService));
#endif // TT_ASYNC_LOG
    }

    void CLogCenter::appendDate()
    {
        time_t nNow = time(NULL);
        r_int32 dt = utils::d8_secondToDate(nNow);
        string strDate = (boost::format("%08d") % dt).str();

        removeOldAppendedFiles("TTStdFile", "fa1", strDate);
        removeOldAppendedFiles("TTDbgFile", "fa2", strDate);
        appendLogFileName("TTStdFile", "fa1", strDate);
        appendLogFileName("TTDbgFile", "fa2", strDate);
    }

    void CLogCenter::start()
    {
#ifdef TT_ASYNC_LOG
        m_pThread = new boost::thread(boost::bind(&CLogCenter::doRunIOService, this));
#endif // TT_ASYNC_LOG
    }

    void CLogCenter::setConDbgID( int nID )
    {
        m_nConDbgID = nID;
    }

    void CLogCenter::setFileDbgID( int nID )
    {
        m_fileDbgIds.insert(nID);
    }

/*
    void CLogCenter::asyncDebug( const string & strMessage )
    {
        asyncLog("TTConsole", LLV_DEBUG, strMessage);
    }

    void CLogCenter::asyncStdLog( ELogLevel level, const string & strMessage )
    {
        asyncLog("TTStdFile", level, strMessage);
    }

    void CLogCenter::asyncDbgLog( const string & strMessage )
    {
        asyncLog("TTDbgFile", LLV_DEBUG, strMessage);
    }

    void CLogCenter::asyncLog( const string & strLoggerName, ELogLevel level, const string & strMessage )
    {
#ifdef TT_ASYNC_LOG
        s_ioService.post(boost::bind(&CLogCenter::doLog, this, level, strLoggerName, strMessage));
#else
        doLog(strLoggerName, level, strMessage);
#endif // TT_ASYNC_LOG
    }
*/

#ifdef TT_ASYNC_LOG
    void CLogCenter::doRunIOService()
    {
        boost::system::error_code ec;
        s_ioService.run(ec);
    }
#endif // TT_ASYNC_LOG

    void CLogCenter::doLog( const string & strLoggerName, ELogLevel level, const string & strMessage )
    {
        if (m_bEnabled)
        {
            LOG4CXX_LOG(log4cxx::Logger::getLogger(strLoggerName), log4cxx::Level::toLevel(LOG4CXX_LEVEL_INT[level]), strMessage.c_str());
        }
    }
}

void ttDebug( int nID, const char * const szFormat, ... )
{
    if (utils::CLogCenter::instance()->conLogEnabled(nID))
    {
        va_list v;
        va_start(v, szFormat);

        static const int c_sMaxBuf = 4096;
        char szBuffer[c_sMaxBuf];
        memset(szBuffer, 0, c_sMaxBuf * sizeof(char));
        int nBuf = vsnprintf(szBuffer, c_sMaxBuf, szFormat, v);

        if (nBuf < 0)
        {
            szBuffer[c_sMaxBuf - 1] = 0;
        }
        if (nBuf < c_sMaxBuf)
        {
            LOG4CXX_LOG(log4cxx::Logger::getLogger("TTConsole"), log4cxx::Level::toLevel(LOG4CXX_LEVEL_INT[LLV_DEBUG]), szBuffer);
        }

        va_end(v);
    }
}

void ttStdLog( ELogLevel level, const char *const szFormat, ... )
{
    if (utils::CLogCenter::instance()->logEnabled())
    {
        va_list v;
        va_start(v, szFormat);

        static const int c_sMaxBuf = 4096;
        char szBuffer[c_sMaxBuf];
        memset(szBuffer, 0, c_sMaxBuf * sizeof(char));
        int nBuf = vsnprintf(szBuffer, c_sMaxBuf, szFormat, v);

        if (nBuf < 0)
        {
            szBuffer[c_sMaxBuf - 1] = 0;
        }
        if (nBuf < c_sMaxBuf)
        {
            LOG4CXX_LOG(log4cxx::Logger::getLogger("TTStdFile"), log4cxx::Level::toLevel(LOG4CXX_LEVEL_INT[level]), szBuffer);
        }

        va_end(v);
    }
}

void ttDbgLog( int nID, const char *const szFormat, ... )
{
    if (utils::CLogCenter::instance()->fileLogEnabled(nID))
    {
        va_list v;
        va_start(v,szFormat);

        static const int c_sMaxBuf = 4096;
        char szBuffer[c_sMaxBuf];
        memset(szBuffer, 0, c_sMaxBuf * sizeof(char));
        int nBuf = vsnprintf(szBuffer, c_sMaxBuf, szFormat, v);

        if (nBuf < 0)
        {
            szBuffer[c_sMaxBuf - 1] = 0;
        }
        if (nBuf < c_sMaxBuf)
        {
            LOG4CXX_LOG(log4cxx::Logger::getLogger("TTDbgFile"), log4cxx::Level::toLevel(LOG4CXX_LEVEL_INT[LLV_DEBUG]), (boost::format("[dbg_id=%1%] %2%") % nID % szBuffer).str());
        }

        va_end(v);
    }
}

void ttLog( const char * const szLoggerName, ELogLevel level, const char * const szFormat, ... )
{
    va_list v;
    va_start(v,szFormat);

    static const int c_sMaxBuf = 4096;
    char szBuffer[c_sMaxBuf];
    memset(szBuffer, 0, c_sMaxBuf * sizeof(char));
    int nBuf = vsnprintf(szBuffer, c_sMaxBuf, szFormat, v);

    if (nBuf < 0)
    {
        szBuffer[c_sMaxBuf - 1] = 0;
    }
    if (nBuf < c_sMaxBuf)
    {
        LOG4CXX_LOG(log4cxx::Logger::getLogger(szLoggerName), log4cxx::Level::toLevel(LOG4CXX_LEVEL_INT[level]), szBuffer);
    }

    va_end(v);
}

void ttDebugNew( const log4cxx::LoggerPtr& logger, const char * const szFormat, ... )
{
    va_list v;
    va_start(v, szFormat);

    static const int c_sMaxBuf = 4096;
    char szBuffer[c_sMaxBuf];
    memset(szBuffer, 0, c_sMaxBuf * sizeof(char));
    int nBuf = vsnprintf(szBuffer, c_sMaxBuf, szFormat, v);

    if (nBuf < 0)
    {
        szBuffer[c_sMaxBuf - 1] = 0;
    }
    if (nBuf < c_sMaxBuf)
    {
        ::log4cxx::helpers::MessageBuffer oss_;
        logger->forcedLog(::log4cxx::Level::getDebug(), oss_.str(oss_ << szBuffer), LOG4CXX_LOCATION);
    }
    va_end(v);
}

void ttStdLogNew(const log4cxx::LoggerPtr& logger, const log4cxx::LevelPtr& level, const char * const szFormat, ... )
{
    va_list v;
    va_start(v, szFormat);

    static const int c_sMaxBuf = 4096;
    char szBuffer[c_sMaxBuf];
    memset(szBuffer, 0, c_sMaxBuf * sizeof(char));
    int nBuf = vsnprintf(szBuffer, c_sMaxBuf, szFormat, v);

    if (nBuf < 0)
    {
        szBuffer[c_sMaxBuf - 1] = 0;
    }
    if (nBuf < c_sMaxBuf)
    {
        ::log4cxx::helpers::MessageBuffer oss_; 
        logger->forcedLog(level, oss_.str(oss_ << szBuffer), LOG4CXX_LOCATION); 
    }
    va_end(v);
}

void ttDbgLogNew( const log4cxx::LoggerPtr& logger, int nID, const char * const szFormat, ... )
{
    va_list v;
    va_start(v, szFormat);

    static const int c_sMaxBuf = 4096;
    char szBuffer[c_sMaxBuf];
    memset(szBuffer, 0, c_sMaxBuf * sizeof(char));
    int nBuf = vsnprintf(szBuffer, c_sMaxBuf, szFormat, v);

    if (nBuf < 0)
    {
        szBuffer[c_sMaxBuf - 1] = 0;
    }
    if (nBuf < c_sMaxBuf)
    {
        ::log4cxx::helpers::MessageBuffer oss_;
        logger->forcedLog(::log4cxx::Level::getDebug(), oss_.str(oss_ << (boost::format("[dbg_id=%1%] %2%") % nID % szBuffer).str()), LOG4CXX_LOCATION); \
    }
    va_end(v);
}