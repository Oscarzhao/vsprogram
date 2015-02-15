
#ifndef __UTILS_BSON_EXT_H__
#define __UTILS_BSON_EXT_H__

#include "mathex.h"
#include "bson/src/bson.h"


namespace utils
{
    /** 
     * 不适用于直接获取char类型，需要通过获取int类型再转char
     */

    template<typename T>
    bool get_be_value(T& val, const bson::bo& param, const string& key, bool is_required = false)
    {
        bson::be element = param.getFieldDotted(key);
        if (!element.eoo())
        {
            try 
            {
                switch (element.type())
                {
                case bson::NumberInt: // int
                    val = boost::lexical_cast<T>(element.Int());
                    return true;

                case bson::NumberLong: // int64
                    val = boost::lexical_cast<T>(element.Long());
                    return true;

                case bson::String: // string
                    val = boost::lexical_cast<T>(element.String());
                    return true;

                case bson::NumberDouble: // double
                    val = boost::lexical_cast<T>(element.Double());
                    return true;

                case bson::Bool: // boolean
                    val = boost::lexical_cast<T>(element.Bool());
                    return true;

                default:
                    break;
                }
            }
            catch (const boost::bad_lexical_cast& e)
            {
                std::cerr << "get bson value error, " << e.what() << std::endl;
                //NULL;
            }
        }
        else
        {
            if (is_required)
            {
                throw (boost::format("value of field [%s] not found") %key).str();
            }
        }
        
        return false;
    }

    template<typename T>
    T get_be_value_ext(const bson::bo& param, const string& key, const T& defval)
    {
        T val;
        if (!get_be_value(val, param, key))
        {
            val = defval;
        }
        return val;
    }

    string get_be_val_str(const bson::bo& param, const string& key, bool is_required = false);

    // -------------------------------------------------------------------------

    template<typename T>
    void get_be_val(T& val, const bson::bo& param, const char* key)
    {
        bson::be element = param.getFieldDotted(key);
        if (!element.eoo())
        {
            switch (element.type())
            {
            case bson::NumberInt: // int
                val = boost::lexical_cast<T>(element.Int());
                break;

            case bson::NumberLong: // int64
                val = boost::lexical_cast<T>(element.Long());
                break;

            case bson::String: // string
                val = boost::lexical_cast<T>(element.String());
                break;

            case bson::NumberDouble: // double
                val = boost::lexical_cast<T>(element.Double() + FLOAT_ERROR);
                break;

            case bson::Bool: // boolean
                val = boost::lexical_cast<T>(element.Bool());
                break;

            default:
                throw std::invalid_argument((boost::format("undef value type of field [%s]") %key).str());
            }
        }
        else
        {
            throw (boost::format("value of field [%s] not found") %key).str();
        }
    }

    static void get_be_val_char(char& val, const bson::bo& param, const char* key)
    {
        int tmp(0);
        get_be_val(tmp, param, key);
        val = (char)tmp;
    }

    template<typename T>
    T get_be_val(const bson::bo& param, const string& key)
    {
        T val;
        get_be_val(val, param, key.c_str());
        return val;
    }

    void get_be_vec(vector<string>& vals, const bson::bo& param, const string& field);
    
    /*
    boost::any get_be_val(const bson::bo& param, const char* key)
    {
        bson::be element = param.getFieldDotted(key);
        if (!element.eoo())
            switch (element.type())
            {
            case bson::NumberInt: // int
                return element.Int();

            case bson::NumberLong: // int64
                return element.Long();

            case bson::String: // string
                return element.String();

            case bson::NumberDouble: // double
                return element.Double();

            case bson::Bool: // boolean
                return element.Bool();

            default:
                throw std::exception((boost::format("undef value type of field [%s]") %key).str().c_str());
            }
        }
        else
        {
            throw (boost::format("value of field [%s] not found") %key).str();
        }
    }*/

    // -------------------------------------------------------------------------

    /*
     * @brief 定义了协议中的类型
     */
    typedef enum
    {
        DT_INT32       = 0x01,
        DT_INT64       = 0x02,
        DT_DOUBLE      = 0x03,
        DT_STRING      = 0x04,
        DT_BOOLEAN     = 0x05,
        DT_UNKNOWN     = 0xFF
    } DATA_TYPE;

    /** 
     * @brief 从param中获取字段值
     * @param 
     *        param - 字段集合
     *        field - 字段名称
     *        type  - 字段值类型
     *        is_required - 字段值是否必需（要求值也不可为空），true - 必需，false - 可选，默认false
     *        defval - 默认值，以字符串的形式给出
     * @return 字段值，以字符串形式给出
     */
    string getFieldVal(const bson::bo& param, const string& field, 
        BYTE type = DT_STRING, bool is_required = false, const string& defval = "");

    // 字段值类型和返回类型一致时使用
    r_int32 getFieldValInt(const bson::bo& param, const string& field, bool is_required = false, r_int32 defval = 0);
    r_int64 getFieldValInt64(const bson::bo& param, const string& field, bool is_required = false, const r_int64& defval = 0);
    double getFieldValDouble(const bson::bo& param, const string& field, bool is_required = false, const double& defval = 0.0);
    string getFieldValStr(const bson::bo& param, const string& field, bool is_required = false, const string& defval = "");
    bool getFieldValBool(const bson::bo& param, const string& field, bool is_required = false, bool defval = false);
    void getFieldValVecStr(vector<string>& vals, const bson::bo& param, const string& field, bool is_required);

    bson::bo to_bson(const std::map<string, string>& param);
}

#endif // #ifndef __UTILS_BSON_EXT_H__
