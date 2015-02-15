/**
* 定义一些log的宏, 在使用rootLogger的时候可以方便使用
* 
* @author   xujun
* @date     2012-1-6
* @time     10:27
* @version  1.0
*/

#ifndef LOG_H_
#define LOG_H_

#include <log4cxx/logger.h>

#define TT_LOG_DEBUG(log)   LOG4CXX_DEBUG( log4cxx::Logger::getRootLogger(), log )
#define TT_LOG_TRACE(log)   LOG4CXX_TRACE( log4cxx::Logger::getRootLogger(), log )
#define TT_LOG_INFO(log)    LOG4CXX_INFO( log4cxx::Logger::getRootLogger(), log )
#define TT_LOG_WARN(log)    LOG4CXX_WARN( log4cxx::Logger::getRootLogger(), log )
#define TT_LOG_ERROR(log)   LOG4CXX_ERROR( log4cxx::Logger::getRootLogger(), log )
#define TT_LOG_FATAL(log)   LOG4CXX_FATAL( log4cxx::Logger::getRootLogger(), log )

#define TT_LOG_DEBUG_EX(logger, log)   LOG4CXX_DEBUG( logger, log )
#define TT_LOG_TRACE_EX(logger, log)   LOG4CXX_TRACE( logger, log )
#define TT_LOG_INFO_EX(logger, log)    LOG4CXX_INFO( logger, log )
#define TT_LOG_WARN_EX(logger, log)    LOG4CXX_WARN( logger, log )
#define TT_LOG_ERROR_EX(logger, log)   LOG4CXX_ERROR( logger, log )
#define TT_LOG_FATAL_EX(logger, log)   LOG4CXX_FATAL( logger, log )


#endif