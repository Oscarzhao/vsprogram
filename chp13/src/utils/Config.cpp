/**
* 通用配置文件的读写实现
* 
* @author   huangjian
* @date     2011-12-31
* @version  3.0
*/


#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/format.hpp>
#include "utils/Config.h"
#include "utils/FileHelper.h"
#include <utils/strings.h>

using namespace std;
using namespace utils;


Configuration::Configuration(string configFilePath, bool needToLower)
{
    m_configFilePath = trimQuotation(configFilePath);
    m_needToLower = needToLower;
}

Configuration::~Configuration()
{

}

/* 加载配置文件 */
bool Configuration::load(string configFilePath)
{
    boost::recursive_mutex::scoped_lock lock(m_lock);

    if (!configFilePath.empty())
    {
        m_configFilePath = trimQuotation(configFilePath);
    }

    string content ;
    FileHelper::open(m_configFilePath, content);
    return parse(content);
}

bool Configuration::parse(string content)
{
    boost::recursive_mutex::scoped_lock lock(m_lock);
    m_properties.clear();

    string section;
    vector<string> lines;
    boost::split(lines, content, boost::is_any_of("\r\n"));

    for (size_t i = 0; i < lines.size(); i++)
    {
        string line = lines[i];
        boost::algorithm::trim(line);

        /* 去掉注释后面的部分 */
        size_t semi_pos = line.find("#");
        if (semi_pos >= 0 && semi_pos < line.size())
        {
            line = line.substr(0, semi_pos);
        }
        boost::algorithm::trim(line);
        /* 每行至少两个字符才能构成有效信息 */
        if (line.size() < 2)  
        {
            continue;
        }

        /* 新的section开始 */
        if (line[0] == '[' && line[line.size() - 1] == ']')
        {
            section = line.substr(1, line.size() - 2);
            boost::algorithm::trim(section);
            if (m_needToLower)
            {
                boost::algorithm::to_lower(section);
            }            
            m_properties[section] = map<string, string>();
        }
        else
        {
            int pos = line.find("=");
            if (pos > 0)
            {
                string property = line.substr(0, pos);
                string value = line.substr(pos + 1);
                boost::algorithm::trim(property);
                if (m_needToLower)
                {
                    boost::algorithm::to_lower(property);
                }                
                boost::algorithm::trim(value);
                m_properties[section][property] = value;
            }
        }
    }
    return true;
}

/* 保存配置文件 */
bool Configuration::save()
{
    boost::recursive_mutex::scoped_lock lock(m_lock);
    string data = dump();
    return save(data);
}

string Configuration::dump()
{
    stringstream buffer;
    boost::recursive_mutex::scoped_lock lock(m_lock);
    map<string, map<string, string> >::const_iterator iter;
    for (iter = m_properties.begin(); iter != m_properties.end(); iter++)
    {
        buffer << "[" << iter->first << "]\n";
        map<string, string>::const_iterator iter2;
        for (iter2 = iter->second.begin(); iter2 != iter->second.end(); iter2++)
        {
            buffer << iter2->first << "=" << iter2->second << "\n";
        }
        buffer << "\n";
    }
    return buffer.str();
}

bool Configuration::save(string content)
{
    boost::recursive_mutex::scoped_lock lock(m_lock);

    FILE* outf = fopen(m_configFilePath.c_str(), "wb");
    if (outf == NULL)
    {
        return false;
    }

    size_t pos = 0;
    size_t bytesWritten = 0;
    while(pos < content.size())
    {
        bytesWritten = fwrite(&content[pos], 1, content.size() - pos, outf);
        pos += bytesWritten;

        /* 文件出错*/
        if (bytesWritten <= 0)
        {
            fclose(outf);
            return false;
        }
    }
    fclose(outf);
    return true;
}

/* 读取某个section的某个属性的字符串值 */
string Configuration::read(string section, string property)
{
    string sec = section;
    string prop = property;
    string ret;

    if (m_needToLower)
    {
        boost::algorithm::to_lower(sec);
        boost::algorithm::to_lower(prop);
    }
    boost::algorithm::trim(sec);
    boost::algorithm::trim(prop);

    boost::recursive_mutex::scoped_lock lock(m_lock);

    map<string, map<string, string> >::iterator iter = m_properties.find(sec);
    if (iter != m_properties.end())
    {
        if (iter->second.find(prop) != iter->second.end())
        {
            ret = iter->second[prop];
        }
    }

    return ret;
}

/* 读取某个section的某个属性的整数值 */
int Configuration::readInt(string section, string property, int defaultValue)
{
    string strNum = read(section, property);
    int ret = defaultValue;
    try
    {
        if (strNum.empty())
        {
            return ret;
        }
        ret = boost::lexical_cast<int>(strNum);
    }
    catch (...)
    {

    }
    return ret;
}

/* 读取某个section的某个属性的浮点数值 */
double Configuration::readDouble(const string& section, string property, double defaultValue)
{
    string strNum = read(section, property);
    double ret = defaultValue;
    try
    {
        ret = boost::lexical_cast<double>(strNum);
    }
    catch (...)
    {

    }
    return ret;
}

/* 读取某个section的某个属性的字符串列表值 */
vector<string> Configuration::readVector(const string& section, string property, string sep)
{
    string items = read(section, property);
    vector<string> ret;
    boost::split(ret, items, boost::is_any_of(sep));
    for (size_t i = 0; i < ret.size(); i++)
    {
        boost::algorithm::trim(ret[i]);
    }

    // 删除最后一个空元素
    if (ret.size() > 0 && ret.back().empty())
    {
        ret.pop_back();
    }

    return ret;
}

/* 写入某个section的某个属性的字符串值 */
void Configuration::write(string section, string property, string value)
{
    string sec = section;
    string prop = property;
    string val = value;

    if (m_needToLower)
    {
        boost::algorithm::to_lower(sec);
        boost::algorithm::to_lower(prop);
    }
    boost::algorithm::trim(sec);
    boost::algorithm::trim(prop);
    boost::algorithm::trim(val);

    boost::recursive_mutex::scoped_lock lock(m_lock);
    map<string, map<string, string> >::iterator iter = m_properties.find(sec);
    if (iter != m_properties.end())
    {
        iter->second[prop] = val;
    }
    else
    {
        m_properties[sec] = map<string, string>();
        m_properties[sec][prop] = val;
    }
}

/* 写入某个section的某个属性的整数值 */
void Configuration::write(string section, string property, int value)
{
    string val = (boost::format("%1%") % value).str();
    write(section, property, val);
}

/* 写入某个section的某个属性的浮点数值 */
void Configuration::write(string section, string property, double value)
{
    string val = (boost::format("%1%") % value).str();
    write(section, property, val);
}

/* 写入某个section的某个属性的字符串列表值 */
void Configuration::write(string& section, string property, vector<string>& value, string sep)
{
    string val;
    for (size_t i = 0; i < value.size(); i++)
    {
        val += value[i] + sep;
    }
    write(section, property, val);
}

/* 读取section列表 */
vector<string> Configuration::getSections()
{
    boost::recursive_mutex::scoped_lock lock(m_lock);

    vector<string> sections;
    map<string, map<string, string> >::const_iterator iter;
    for (iter = m_properties.begin(); iter != m_properties.end(); iter++)
    {
        sections.push_back(iter->first);
    }
    return sections;
}

/* 读取某个section的属性列表 */
vector<string> Configuration::getProperties(const string& section)
{
    vector<string> properties;
    string sec = section;

    if (m_needToLower)
    {
        boost::algorithm::to_lower(sec);
    }    

    boost::recursive_mutex::scoped_lock lock(m_lock);
    map<string, map<string, string> >::const_iterator iter = m_properties.find(sec);
    if (iter != m_properties.end())
    {
        map<string, string>::const_iterator iter2;
        for (iter2 = iter->second.begin(); iter2 != iter->second.end(); iter2++)
        {
            properties.push_back(iter2->first);
        }
    }

    return properties;
}