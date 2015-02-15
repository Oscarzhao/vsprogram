/**
 *  bson基础解析函数
 *
 *  @author zhangyi
 *  @since  2012-03-19
 */

#include <utils/bsonFunc.h>
#include <bson/src/util/json.h>
#include <utils/convFunc.h>

namespace utils
{
    
    string bsonToString( bson::bo obj )
    {
        string str;
        
        try
        {
            str = obj.toString();
        }
        catch (...)
        {

        }

        return str;
    }

    bson::bo bsonNull()
    {
        bson::bo obj = bson::bo();
        return obj;
    }

    bson::bo bsonSuccess()
    {
        bson::bo obj = bson::bob().append("success", true).obj();
        return obj;
    }

    bson::bo utf8StringToBson(string str)
    {
        string strJson = str;
        utils::safeUtf82Gbk(strJson);
        bson::bo obj = bson::fromjson(strJson);
        return obj;
    }
}
