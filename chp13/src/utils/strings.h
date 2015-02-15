#ifndef UTILS_STRINGS_H
#define UTILS_STRINGS_H

#include <list>
#include <string>
#include <sstream>

using namespace std;

#ifdef WIN32
list<string> operator<<(const string & left, const string & right);
list<string> & operator<<(list<string> & left, const string & right);
#endif // WIN32

namespace utils
{
    typedef int (*TRANFORM_STRING_FUNC)(int a);

    string transformString(const string & originalString,TRANFORM_STRING_FUNC func1) ;

    string toUpper(const string & originalString);

    string toLower(const string & originalString);

    void string_replace( std::string &strBig, const std::string &strsrc, const std::string &strdst );
    void string_replace( std::string &strBig, char c, const::std::string & strdst );
    void bsonAddQuota(string & str);

    /**
    * 从形如192.168.1.1:80这样的字符串中解析ip和port
    */
    void getIpPort(const string& data, string& ip, unsigned short& port);

    /**
    * 从形如192.168.1.1:80这样的字符串中解析ip和port
    */
    void getIpPort(const char* data, string& ip, unsigned short& port);

    template< class T>
    string to_string(const T& value)
    {
        ostringstream os;
        os << value;
        return os.str();
    };

    string gbk2utf8(const string & strIn);

    // 将语句中所有"转为\"
    string fixSqlString(const string & content);

    // 去掉两头的引号
    string trimQuotation(string strIn);

    // 从market的字母获取中文写法
    string getMarketName(string market);

    //判断两个字符串的大小，首先长度长的字符串更大，同长度下用字典序比较大小,返回-1表示前者小，0表示相等，1表示后者小
    inline int compareStr(const string& strA, const string& strB)
    {
        size_t la = strA.length();
        size_t lb = strB.length();
        if (la < lb || (la == lb && strA.compare(strB) < 0 ) )
        {
            return -1;
        }
        else if(la == lb && strA.compare(strB) == 0)
        {
            return 0;
        }
        else
        {
            return 1;
        }
    }
}

#define G2U(arg)    utils::gbk2utf8(arg)

#endif
