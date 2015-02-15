#ifndef __PROXY_CLIENT_READ_DB_MYSQL_H__
#define __PROXY_CLIENT_READ_DB_MYSQL_H__
#include <common/Stdafx.h>
#include <mysql/mysql.h>
#include <boost/asio.hpp>
#include <utils/Threading.h>
#include <common/LoginCommand.h>
/**
 *	从 MySQL数据库中读取 用户登录白名单
 *  @author: zhaoshuailong
 *  @date:   2014-10-20
 */
namespace ttservice
{
    struct MySQLLoginInfo
    {
    public:
        MySQLLoginInfo(std::string mysqlServerIP, unsigned int mysqlServerPort, std::string mysqlUserName, std::string mysqlPassword, std::string mysqlDatabase)
            :serverIp(mysqlServerIP), serverPort(mysqlServerPort), UserName(mysqlUserName), Password(mysqlPassword), DatabaseName(mysqlDatabase){}
    public:
        std::string         serverIp;
        unsigned int        serverPort;
        std::string         UserName;
        std::string         Password;
        std::string         DatabaseName;
    };

    class CMySQL
    {
    public:
        CMySQL(const struct MySQLLoginInfo& info, std::string mysqlTableName, boost::asio::io_service& ios);
        ~CMySQL();
    public:
        bool init();
        bool getWhiteList(std::vector< std::string >& vWhiteList);
    private:
        void pingDataBase(const boost::system::error_code& error);

    private:
        struct MySQLLoginInfo           m_mysqlLoginInfo;
        std::string                     m_mysqlTable;

        MYSQL*                          m_mysql;
        boost::asio::deadline_timer     m_timer;
        utils::RecMutex                 m_mutex;
    };
}
#endif