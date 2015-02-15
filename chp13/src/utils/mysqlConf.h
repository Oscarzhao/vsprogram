/**
 *  mysql配置
 *
 *  @author zhangyi
 *  @since  2012-09-06
 */

#ifndef SERVER3_BASE_UTILS_MYSQLCONF_H
#define SERVER3_BASE_UTILS_MYSQLCONF_H

#ifdef WIN32
#include <winsock2.h>
#else
#endif

#include <mysql/mysql.h>
#include <boost/any.hpp>
#include <boost/function.hpp>
#include <string>
#include <vector>
#include <bson/src/bson.h>

using namespace std;

namespace utils
{

    typedef boost::function<void(MYSQL_RES*,bool)> TQueryCallback;

    class CMySQLConf
    {
    public:
        CMySQLConf();

        string  m_host;
        int     m_port;
        string  m_db;
        string  m_user;
        string  m_password;

        void bsonValueFromObj(bson::bo obj);

        string toParam() const;
    };
}

#endif
