
/**
* XML文件的常用函数
* 
* @author   zld
* @date     2013-03-15
* @version  1.0
*/


#ifndef __UTILS_XML_CONFIGURATION_H__
#define __UTILS_XML_CONFIGURATION_H__

#include <vector>
#include <map>
#include <tinyxml/tinyxml.h>
#include <string>

using namespace std;

namespace utils
{
    class XMLConfig
    {
        public:
            static void setConfig(string filepath, string firstChildElement, const vector< pair<string, string> >& attributes);
            static vector<string> getConfig(string filepath, string firstChildElement, const vector<string>& attributeNames);
    };
}

#endif //__UTILS_XML_CONFIGURATION_H__
