
#include "common/Stdafx.h"
#include "utils/BsonHelper.h"
#include "bson_utils.h"


namespace utils
{
    void get_be_vec(vector<string>& vals, const bson::bo& param, const string& field)
    {
        bson::be element = param.getFieldDotted(field); 
        if (!element.eoo())
        {
            utils::bsonValue(element, vals);
        }
        else 
        {
            throw runtime_error((boost::format("field [%s] not found") %field).str());
        }
    }

    string getFieldVal(const bson::bo& param, const string& field, BYTE type, bool is_required, const string& defval)
    {
        bson::be element = param.getFieldDotted(field);
        string val("");
        if (!element.eoo())
        {
            switch (type)
            {
            case DT_INT32: // int
                val = (boost::format("%1%") %element.Int()).str();
                break;

            case DT_INT64: // int64
                val = (boost::format("%1%") %element.Long()).str();
                break;

            case DT_STRING: // string
                val = element.String();
                break;

            case DT_DOUBLE: // double
                val = (boost::format("%1%") %element.Double()).str();
                break;

            case DT_BOOLEAN: // boolean
                val = (boost::format("%1%") %element.Bool()).str();
                break;

            default:
                throw (boost::format("undefined data type %s, field: %s") %type %field).str();
            }
        }

        if (val.empty())
        {
            if (is_required)
            {
                throw (boost::format("null value of field [%s]") %field).str();
            }
            else
            {
                val = defval;
            }
        }

        return val;
    }

    int getFieldValInt(const bson::bo& param, const string& field, bool is_required, int defval)
    {
        bson::be element = param.getFieldDotted(field);
        if (!element.eoo())
        {
            return element.Int();
        }
        else 
        {
            if (is_required)
            {
                throw (boost::format("null value of field [%s]") %field).str();
            }
            else
            {
                return defval;
            }
        }
    }

    r_int64 getFieldValInt64(const bson::bo& param, const string& field, bool is_required, const r_int64& defval)
    {
        bson::be element = param.getFieldDotted(field); 
        if (!element.eoo())
        {
            return element.Long();
        }
        else 
        {
            if (is_required)
            {
                throw (boost::format("null value of field [%s]") %field).str();
            }
            else
            {
                return defval;
            }
        }
    }

    double getFieldValDouble(const bson::bo& param, const string& field, bool is_required, const double& defval)
    {
        bson::be element = param.getFieldDotted(field); 
        if (!element.eoo())
        {
            return element.Double();
        }
        else 
        {
            if (is_required)
            {
                throw (boost::format("null value of field [%s]") %field).str();
            }
            else
            {
                return defval;
            }
        }
    }

    string getFieldValStr(const bson::bo& param, const string& field, bool is_required, const string& defval)
    {
        string val("");
        bson::be element = param.getFieldDotted(field); 
        if (!element.eoo())
        {
            val = element.String();
        }
        
        if (val.empty())
        {
            if (is_required)
            {
                throw (boost::format("null value of field [%s]") %field).str();
            }
            else
            {
                val = defval;
            }
        }

        return val;
    }

    bool getFieldValBool(const bson::bo& param, const string& field, bool is_required, bool defval)
    {
        bson::be element = param.getFieldDotted(field); 
        if (!element.eoo())
        {
            return element.Bool();
        }
        else 
        {
            if (is_required)
            {
                throw (boost::format("null value of field [%s]") %field).str();
            }
            else
            {
                return defval;
            }
        }
    }

    void getFieldValVecStr(vector<string>& vals, const bson::bo& param, const string& field, bool is_required)
    {
        bson::be element = param.getFieldDotted(field); 
        if (!element.eoo())
        {
            utils::bsonValue(element, vals);
        }
        else 
        {
            if (is_required)
            {
                throw (boost::format("null value of field [%s]") %field).str();
            }
        }
    }

    bson::bo to_bson(const std::map<string, string>& param)
    {
        bson::bob bob;
        std::map<string, string>::const_iterator iter = param.begin();
        std::map<string, string>::const_iterator iter_end = param.end();
        for (; iter != iter_end; ++iter)
        {
            bob.append(iter->first, iter->second);
        }
        return bob.obj();
    }

    ////////////////////////////////////////////////////////////////////////////

    string get_be_val_str(const bson::bo& param, const string& key, bool is_required)
    {
        string valstr("");
        get_be_value(valstr, param, key, is_required);
        if (valstr.empty() && is_required)
        {
            throw (boost::format("value of field [%s] is empty") %key).str();
        }
        return valstr;
    }

}