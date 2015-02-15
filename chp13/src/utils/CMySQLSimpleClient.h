/**
 *  mysql简易客户端
 *
 *  @author zhangyi
 *  @since  2012-10-24
 */

#ifndef SERVER3_BASE_UTILS_CMYSQLSIMPLECLIENT_H
#define SERVER3_BASE_UTILS_CMYSQLSIMPLECLIENT_H

#ifdef WIN32
    #include <winsock2.h>
#else
#endif

#include <mysql/mysql.h>
#include <boost/any.hpp>
#include <string>
#include <vector>
#include <utils/mysqlConf.h>

using namespace std;

namespace utils
{

    class CMySQLSimpleClient
    {
    public:
        CMySQLSimpleClient();
        virtual ~CMySQLSimpleClient();

    public:

        void start(CMySQLConf conf);
        void stop();

        void execute(const string& sql);

        /**
        * 执行查询sql语句
        * @return 返回查询结果集
        * @remark 该函数为异步函数。
        *         1\ 查询结果集的释放须调用freeQueryRes()函数。
        *         2\ 支持二进制数据, 比如\0, 更多的需要转义的接口暂时没有加
        */
        MYSQL_RES * query(const string& sql);
        
        /**
        * 释放查询结果集
        */
        void freeQueryRes(MYSQL_RES* res);

        const MYSQL * connection() const;

    protected:

        bool doConnect();
        void doDisconnect();

        MYSQL *                                 m_connection;        /* mysql数据库连接句柄 */        
        CMySQLConf                              m_conf;
    };
}

#endif // m__MONITOR_MYSQL_CLIENT_m_H_

