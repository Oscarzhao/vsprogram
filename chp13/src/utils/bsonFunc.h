/**
 *  bson基础解析函数
 *
 *  @author zhangyi
 *  @since  2012-03-19
 */

#ifndef SERVER3_BASE_UTILS_BSONFUNC_H
#define SERVER3_BASE_UTILS_BSONFUNC_H

#include <bson/src/bson.h>
#include <common/type.h>
#include <string>
using namespace std;

namespace utils
{
    string bsonToString(bson::bo obj);

    bson::bo bsonNull();

    bson::bo bsonSuccess();

    inline bool safeParseBsonBool( bool &bOut, const bson::bo &obj, const char *szParam )
    {
        bson::be element = obj.getFieldDotted(szParam);
        if (!element.eoo() && bson::Bool == element.type())
        {
            bOut = element.Bool();
            return true;
        }
        return false;
    }

    inline bool safeParseBsonInt( int &nOut, const bson::bo &obj, const char *szParam )
    {
        bson::be element = obj.getFieldDotted(szParam);
        if (!element.eoo() && (bson::NumberInt == element.type() || bson::NumberLong == element.type()))
        {
            nOut = element.numberInt();
            return true;
        }
        return false;
    }

    inline bool safeParseBsonLong( r_int64 &nOut, const bson::bo &obj, const char *szParam )
    {
        bson::be element = obj.getFieldDotted(szParam);
        if (!element.eoo() && bson::NumberLong == element.type())
        {
            nOut = element.numberLong();
            return true;
        }
        return false;
    }

    inline bool safeParseBsonDouble( double &dOut, const bson::bo &obj, const char *szParam )
    {
        bson::be element = obj.getFieldDotted(szParam);
        if (!element.eoo() && bson::NumberDouble == element.type())
        {
            dOut = element.numberDouble();
            return true;
        }
        return false;
    }

    inline bool safeParseBsonString( string &strOut, const bson::bo &obj, const char *szParam )
    {
        bson::be element = obj.getFieldDotted(szParam);
        if (!element.eoo() && bson::String == element.type())
        {
            strOut = element.String();
            return true;
        }
        return false;
    }

    inline bool safeParseBsonObject( bson::bo &boOut, const bson::bo &obj, const char *szParam )
    {
        bson::be element = obj.getFieldDotted(szParam);
        if (!element.eoo() && bson::Object == element.type())
        {
            boOut = element.Obj();
            return true;
        }
        return false;
    }

    inline bool safeParseBsonBin( string & strOut, const bson::bo &obj, const char *szParam )
    {
        bson::be element = obj.getFieldDotted(szParam);
        if (!element.eoo() && bson::BinData == element.type())
        {
            int nLen = 0;
            const char * szOut = element.binData(nLen);
            strOut = string(szOut, nLen);
            return true;
        }
        return false;
    }

    inline bool safeParseBsonArray( vector<bson::be> &vOut, const bson::bo &obj, const char *szParam )
    {
        bson::be element = obj.getFieldDotted(szParam);
        if (!element.eoo() && bson::Array == element.type())
        {
            vOut = element.Array();
            return true;
        }
        return false;
    }

    bson::bo utf8StringToBson(string str);
}

#endif
