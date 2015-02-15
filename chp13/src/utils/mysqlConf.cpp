/**
 *  mysql配置
 *
 *  @author zhangyi
 *  @since  2012-09-06
 */

#include <utils/mysqlConf.h>
#include "utils/BsonHelper.h"

namespace utils
{

    CMySQLConf::CMySQLConf()
        : m_port(0)
    {

    }

    void CMySQLConf::bsonValueFromObj(bson::bo obj)
    {
        utils::safeParseBson(obj, "m_host", this->m_host);
        utils::safeParseBson(obj, "m_port", this->m_port);
        utils::safeParseBson(obj, "m_db", this->m_db);
        utils::safeParseBson(obj, "m_user", this->m_user);
        utils::safeParseBson(obj, "m_password", this->m_password);
    }

    bson::string CMySQLConf::toParam() const
    {
        return m_host + "," + boost::lexical_cast<string>(m_port) + ","
            + m_db + "," + m_user + "," + m_password + ",";
    }

}
