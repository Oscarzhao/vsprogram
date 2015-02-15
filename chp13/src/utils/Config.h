/**
* 通用配置文件的读写实现
* 
* @author   huangjian
* @date     2011-12-31
* @version  3.0
*/


#ifndef __UTILS_CONFIGURATION_H__
#define __UTILS_CONFIGURATION_H__

#include <vector>
#include <map>
#include <boost/thread.hpp>
#include <string>
using namespace std;

namespace utils
{

    class Configuration
    {
    public:
        Configuration(string configFilePath = "", bool needToLower = true);
        virtual ~Configuration();

    public:
        /* 加载配置文件 */
        bool load(string configFilePath = "");

        /* 解析配置 */
        bool parse(string content);

        /* 保存配置到文件 */
        bool save();

        /* 将内容展示成string */
        string dump();

        /* 保存内容 */
        bool save(string content);

        /* 读取某个section的某个属性的字符串值 */
        string read(string section, string property);

        /* 读取某个section的某个属性的整数值 */
        int readInt(string section, string property, int defaultValue);

        /* 读取某个section的某个属性的浮点数值 */
        double readDouble(const string& section, string property, double defaultValue);

        /* 读取某个section的某个属性的字符串列表值 */
        vector<string> readVector(const string& section, string property, string sep);

        /* 写入某个section的某个属性的字符串值 */
        void write(string section, string property, string value);

        /* 写入某个section的某个属性的整数值 */
        void write(string section, string property, int value);

        /* 写入某个section的某个属性的浮点数值 */
        void write(string section, string property, double value);

        /* 写入某个section的某个属性的字符串列表值 */
        void write(string& section, string property, vector<string>& value, string sep);

        /* 读取section列表 */
        vector<string> getSections();

        /* 读取某个section的属性列表 */
        vector<string> getProperties(const string& section);

    private:
        string              m_configFilePath;   // 配置文件路径
        bool                     m_needToLower;        // 是否需要强制小写
        map<string, map<string,string> > m_properties; // 属性表
        boost::recursive_mutex  m_lock; 
    };

    typedef boost::shared_ptr<Configuration> ConfigurationPtr;
}

#endif

