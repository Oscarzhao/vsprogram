#ifndef BSON_HELPER_20120416_H_
#define BSON_HELPER_20120416_H_

#include "bson/src/bson.h"
#include "utils/commonFunc.h"
#include <vector>
using namespace std;

namespace utils
{

#define CODEC_DECLEAR() \
public :\
    static Codec& instance()\
    {\
    static Codec acodec;\
    return acodec;\
}\
private:\
    Codec(){};\
    Codec(const Codec& acodec);\
    Codec& operator=(const Codec& rhs); 

    //////////////////////////
    //普通类型
    /////////////////////////
    template <typename Type>
    class Codec
    {
        CODEC_DECLEAR();
    public :
        void bsonValue(const bson::be& bdata, Type& value)
        {
            value.bsonValue(bdata);
        }

        void appendToBuilder(bson::bob& builder, const char* key, const Type& data)
        {
            data.appendToBuilder(builder, key);
        }

        void appendToArrayBuilder(bson::BSONArrayBuilder& builder, const Type& data)
        {
            data.appendToArrayBuilder(builder);
        }

        void appendToBuilderV2(bson::bob& builder, const char* key, const Type& data)
        {
            data.appendToBuilder(builder, key);
        }

        void appendToArrayBuilderV2(bson::BSONArrayBuilder& builder, const Type& data)
        {
            data.appendToArrayBuilder(builder);
        }
    };

#define DECLEAR_BASE_TYPE(Type, func)\
    template <>\
    class Codec<Type>\
    {\
    CODEC_DECLEAR();\
    public :\
    void bsonValue(const bson::be& bdata, Type& value)\
    {\
        value = bdata.func();\
    }\
    void appendToBuilder(bson::bob& builder, const char* key, const Type& data)\
    {\
    builder.append(key, data);\
    }\
    void appendToArrayBuilder(bson::BSONArrayBuilder& builder, const Type& data)\
    {\
    builder.append(data);\
    }\
    void appendToBuilderV2(bson::bob& builder, const char* key, const Type& data)\
    {\
        appendToBuilder(builder, key, data);\
    }\
    void appendToArrayBuilderV2(bson::BSONArrayBuilder& builder, const Type& data)\
    {\
        appendToArrayBuilder(builder, data);\
    }\
    }\
    /* end */

    DECLEAR_BASE_TYPE(bool, Bool);
    DECLEAR_BASE_TYPE(int, numberInt);
    DECLEAR_BASE_TYPE(long long, numberLong);
    DECLEAR_BASE_TYPE(double, numberDouble);
    //DECLEAR_BASE_TYPE(string, String);

    //////////////////////////
    //string
    /////////////////////////
    template <>
    class Codec<std::string>
    {
        CODEC_DECLEAR();
    public :
        void bsonValue(const bson::be& bdata, std::string& value)
        {
            if (!bdata.eoo() && bdata.type() == bson::String)
            {
                value = bdata.String();
            }
        }

        void appendToBuilder(bson::bob& builder, const char* key, const std::string& data)
        {
            builder.append(key, data);
        }

        void appendToArrayBuilder(bson::BSONArrayBuilder& builder, const std::string& data)
        {
            builder.append(data);
        }

        void appendToBuilderV2(bson::bob& builder, const char* key, const std::string& data)
        {
            appendToBuilder(builder, key, data);
        }

        void appendToArrayBuilderV2(bson::BSONArrayBuilder& builder, const std::string& data)
        {
            appendToArrayBuilder(builder, data);
        }
    };

    //////////////////////////
    //bson::bo
    /////////////////////////
    template <>
    class Codec<bson::bo>
    {
        CODEC_DECLEAR();
        public :
        void bsonValue(const bson::be& bdata, bson::bo& value)
        {   
            value = bdata.Obj().copy();
        }
        void appendToBuilder(bson::bob& builder, const char* key, const bson::bo& data)
        {
            builder.append(key, data);
        }
        void appendToArrayBuilder(bson::BSONArrayBuilder& builder, const bson::bo& data)
        {
            builder.append(data);
        }
        void appendToBuilderV2(bson::bob& builder, const char* key, const bson::bo& data)
        {
            appendToBuilder(builder, key, data);
        }
        void appendToArrayBuilderV2(bson::BSONArrayBuilder& builder, const bson::bo& data)
        {
            appendToArrayBuilder(builder, data);
        }
    };

    //////////////////////////
    //容器类型
    /////////////////////////
    template <typename Type>
    class Codec< vector<Type> >
    {
        CODEC_DECLEAR();
    public :
        void bsonValue(const bson::be& bdata, vector<Type>& value)
        {
            vector<bson::be> elements = bdata.Array();
            size_t esize = elements.size();
            value.resize(esize);
            for (size_t i = 0; i < esize; ++i)
            {
                Codec<Type>::instance().bsonValue(elements[i], value[i]);
            }
        }

        void appendToBuilder(bson::bob& builder, const char* key, const vector<Type>& data)
        {
            bson::BSONArrayBuilder arrayBuilder(builder.subarrayStart(key));
            for (typename vector<Type>::const_iterator iter = data.begin(); iter != data.end(); ++iter)
            {
                Codec<Type>::instance().appendToArrayBuilder(arrayBuilder, *iter);
            }
            arrayBuilder.done();
        }

        void appendToArrayBuilder(bson::BSONArrayBuilder& builder, const vector<Type>& data)
        {
            bson::BSONArrayBuilder arrayBuilder(builder.subarrayStart());
            for (typename vector<Type>::const_iterator iter = data.begin(); iter != data.end(); ++iter)
            {
                Codec<Type>::instance().appendToArrayBuilder(arrayBuilder, *iter);
            }
            arrayBuilder.done();
        }

        void appendToBuilderV2(bson::bob& builder, const char* key, const vector<Type>& data)
        {
            bson::BSONArrayBuilder arrayBuilder(builder.subarrayStart(key));
            for (typename vector<Type>::const_iterator iter = data.begin(); iter != data.end(); ++iter)
            {
                Codec<Type>::instance().appendToArrayBuilderV2(arrayBuilder, *iter);
            }
            arrayBuilder.done();
        }

        void appendToArrayBuilderV2(bson::BSONArrayBuilder& builder, const vector<Type>& data)
        {
            bson::BSONArrayBuilder arrayBuilder(builder.subarrayStart());
            for (typename vector<Type>::const_iterator iter = data.begin(); iter != data.end(); ++iter)
            {
                Codec<Type>::instance().appendToArrayBuilderV2(arrayBuilder, *iter);
            }
            arrayBuilder.done();
        }
    };

    template <typename Type>
    class Codec< list<Type> >
    {
        CODEC_DECLEAR();
    public :
        void bsonValue(const bson::be& bdata, list<Type>& value)
        {
            value.clear();
            
            vector<bson::be> elements = bdata.Array();
            for (size_t i = 0; i < elements.size(); ++i)
            {
                Type v;
                Codec<Type>::instance().bsonValue(elements[i], v);
                value.push_back(v);
            }
        }

        void appendToBuilder(bson::bob& builder, const char* key, const list<Type>& data)
        {
            bson::BSONArrayBuilder arrayBuilder(builder.subarrayStart(key));
            for (typename list<Type>::const_iterator iter = data.begin(); iter != data.end(); ++iter)
            {
                Codec<Type>::instance().appendToArrayBuilder(arrayBuilder, *iter);
            }
            arrayBuilder.done();
        }

        void appendToArrayBuilder(bson::BSONArrayBuilder& builder, const list<Type>& data)
        {
            bson::BSONArrayBuilder arrayBuilder(builder.subarrayStart());
            for (typename list<Type>::const_iterator iter = data.begin(); iter != data.end(); ++iter)
            {
                Codec<Type>::instance().appendToArrayBuilder(arrayBuilder, *iter);
            }
            arrayBuilder.done();
        }

        void appendToBuilderV2(bson::bob& builder, const char* key, const list<Type>& data)
        {
            bson::BSONArrayBuilder arrayBuilder(builder.subarrayStart(key));
            for (typename list<Type>::const_iterator iter = data.begin(); iter != data.end(); ++iter)
            {
                Codec<Type>::instance().appendToArrayBuilderV2(arrayBuilder, *iter);
            }
            arrayBuilder.done();
        }

        void appendToArrayBuilderV2(bson::BSONArrayBuilder& builder, const list<Type>& data)
        {
            bson::BSONArrayBuilder arrayBuilder(builder.subarrayStart());
            for (typename list<Type>::const_iterator iter = data.begin(); iter != data.end(); ++iter)
            {
                Codec<Type>::instance().appendToArrayBuilderV2(arrayBuilder, *iter);
            }
            arrayBuilder.done();
        }
    };

    template <typename Type>
    class Codec< set<Type> >
    {
        CODEC_DECLEAR();
    public :
        void bsonValue(const bson::be& bdata, set<Type>& value)
        {
            value.clear();

            vector<bson::be> elements = bdata.Array();
            for (size_t i = 0; i < elements.size(); ++i)
            {
                Type v;
                Codec<Type>::instance().bsonValue(elements[i], v);
                value.insert(v);
            }
        }

        void appendToBuilder(bson::bob& builder, const char* key, const set<Type>& data)
        {
            bson::BSONArrayBuilder arrayBuilder(builder.subarrayStart(key));
            for (typename set<Type>::const_iterator iter = data.begin(); iter != data.end(); ++iter)
            {
                Codec<Type>::instance().appendToArrayBuilder(arrayBuilder, *iter);
            }
            arrayBuilder.done();
        }

        void appendToArrayBuilder(bson::BSONArrayBuilder& builder, const set<Type>& data)
        {
            bson::BSONArrayBuilder arrayBuilder(builder.subarrayStart());
            for (typename set<Type>::const_iterator iter = data.begin(); iter != data.end(); ++iter)
            {
                Codec<Type>::instance().appendToArrayBuilder(arrayBuilder, *iter);
            }
            arrayBuilder.done();
        }

        void appendToBuilderV2(bson::bob& builder, const char* key, const set<Type>& data)
        {
            bson::BSONArrayBuilder arrayBuilder(builder.subarrayStart(key));
            for (typename set<Type>::const_iterator iter = data.begin(); iter != data.end(); ++iter)
            {
                Codec<Type>::instance().appendToArrayBuilderV2(arrayBuilder, *iter);
            }
            arrayBuilder.done();
        }

        void appendToArrayBuilderV2(bson::BSONArrayBuilder& builder, const set<Type>& data)
        {
            bson::BSONArrayBuilder arrayBuilder(builder.subarrayStart());
            for (typename set<Type>::const_iterator iter = data.begin(); iter != data.end(); ++iter)
            {
                Codec<Type>::instance().appendToArrayBuilderV2(arrayBuilder, *iter);
            }
            arrayBuilder.done();
        }
    };

    template <typename Key, typename Value>
    class Codec< std::map<Key, Value> >
    {
        CODEC_DECLEAR();
    public :
        void bsonValue(const bson::be& bdata, std::map<Key, Value>& value)
        {
            vector<bson::be> elements = bdata.Array();
            size_t esize = elements.size();
            Key key;
            for (size_t i = 0; i < esize; ++i)
            {
                if (i % 2 == 0)
                {
                    Codec<Key>::instance().bsonValue(elements[i], key);
                } else {
                    Value data;
                    Codec<Value>::instance().bsonValue(elements[i], data);
                    value.insert(make_pair(key, data));
                }
            }
        }

        void appendToBuilder(bson::bob& builder, const char* key, const std::map<Key, Value>& data)
        {
            bson::BSONArrayBuilder arrayBuilder(builder.subarrayStart(key));
            for (typename map<Key, Value>::const_iterator iter = data.begin(); iter != data.end(); ++iter)
            {
                Codec<Key>::instance().appendToArrayBuilder(arrayBuilder, iter->first);
                Codec<Value>::instance().appendToArrayBuilder(arrayBuilder, iter->second);
            }
            arrayBuilder.done();
        }

        void appendToArrayBuilder(bson::BSONArrayBuilder& builder, const std::map<Key, Value>& data)
        {
            bson::BSONArrayBuilder arrayBuilder(builder.subarrayStart());
            for (typename map<Key, Value>::const_iterator iter = data.begin(); iter != data.end(); ++iter)
            {
                Codec<Key>::instance().appendToArrayBuilder(arrayBuilder, iter->first);
                Codec<Value>::instance().appendToArrayBuilder(arrayBuilder, iter->second);
            }
            arrayBuilder.done();
        }

        void appendToBuilderV2(bson::bob& builder, const char* key, const std::map<Key, Value>& data)
        {
            bson::BSONArrayBuilder arrayBuilder(builder.subarrayStart(key));
            for (typename map<Key, Value>::const_iterator iter = data.begin(); iter != data.end(); ++iter)
            {
                Codec<Key>::instance().appendToArrayBuilderV2(arrayBuilder, iter->first);
                Codec<Value>::instance().appendToArrayBuilderV2(arrayBuilder, iter->second);
            }
            arrayBuilder.done();
        }

        void appendToArrayBuilderV2(bson::BSONArrayBuilder& builder, const std::map<Key, Value>& data)
        {
            bson::BSONArrayBuilder arrayBuilder(builder.subarrayStart());
            for (typename map<Key, Value>::const_iterator iter = data.begin(); iter != data.end(); ++iter)
            {
                Codec<Key>::instance().appendToArrayBuilderV2(arrayBuilder, iter->first);
                Codec<Value>::instance().appendToArrayBuilderV2(arrayBuilder, iter->second);
            }
            arrayBuilder.done();
        }
    };

    template <typename Key, typename Value>
    class Codec< std::pair<Key, Value> >
    {
        CODEC_DECLEAR();
    public :
        void bsonValue(const bson::be& bdata, std::pair<Key, Value>& value)
        {
            vector<bson::be> elements = bdata.Array();
            if (elements.size() == 2)
            {
                Codec<Key>::instance().bsonValue(elements[0], value.first);
                Codec<Value>::instance().bsonValue(elements[1], value.second);                
            }
        }

        void appendToBuilder(bson::bob& builder, const char* key, const std::pair<Key, Value>& data)
        {
            bson::BSONArrayBuilder arrayBuilder(builder.subarrayStart(key));
            Codec<Key>::instance().appendToArrayBuilder(arrayBuilder, data.first);
            Codec<Value>::instance().appendToArrayBuilder(arrayBuilder, data.second);
            arrayBuilder.done();
        }

        void appendToArrayBuilder(bson::BSONArrayBuilder& builder, const std::pair<Key, Value>& data)
        {
            bson::BSONArrayBuilder arrayBuilder(builder.subarrayStart());
            Codec<Key>::instance().appendToArrayBuilder(arrayBuilder, data.first);
            Codec<Value>::instance().appendToArrayBuilder(arrayBuilder, data.second);
            arrayBuilder.done();
        }

        void appendToBuilderV2(bson::bob& builder, const char* key, const std::pair<Key, Value>& data)
        {
            bson::BSONArrayBuilder arrayBuilder(builder.subarrayStart(key));
            Codec<Key>::instance().appendToArrayBuilderV2(arrayBuilder, data.first);
            Codec<Value>::instance().appendToArrayBuilderV2(arrayBuilder, data.second);
            arrayBuilder.done();
        }

        void appendToArrayBuilderV2(bson::BSONArrayBuilder& builder, const std::pair<Key, Value>& data)
        {
            bson::BSONArrayBuilder arrayBuilder(builder.subarrayStart());
            Codec<Key>::instance().appendToArrayBuilderV2(arrayBuilder, data.first);
            Codec<Value>::instance().appendToArrayBuilderV2(arrayBuilder, data.second);
            arrayBuilder.done();
        }
    };


    /************************************************************************/
    /* 普通指针                                                             */
    /************************************************************************/
    template <typename Type>
    class Codec<Type*>
    {
        CODEC_DECLEAR();
    public :
        void bsonValue(const bson::be& bdata, Type*& value)
        {
            value = new Type();
            if (NULL != value)
                bsonValue(bdata, *value);            
        }

        void appendToBuilder(bson::bob& builder, const char* key, const Type*& data)
        {
            if (NULL != data)
                appendToBuilder(builder, key, *data);
        }

        void appendToArrayBuilder(bson::BSONArrayBuilder& builder, const Type*& data)
        {
            if (NULL != data) 
                appendToArrayBuilder(builder, *data);
        }

        void appendToBuilderV2(bson::bob& builder, const char* key, const Type*& data)
        {
            if (NULL != data)
                appendToBuilderV2(builder, key, *data);
        }

        void appendToArrayBuilderV2(bson::BSONArrayBuilder& builder, const Type*& data)
        {
            if (NULL != data) 
                appendToArrayBuilderV2(builder, *data);
        }
    };

    //////////////////////////
    //智能指针
    /////////////////////////
    template <typename Type>
    class Codec< boost::shared_ptr<Type> >
    {
        CODEC_DECLEAR();
    public :
        void bsonValue(const bson::be& bdata, boost::shared_ptr<Type>& value)
        {            
            value = boost::shared_ptr<Type>(new Type());
            if (NULL != value)
                bsonValue(bdata, *value);         
        }

        void appendToBuilder(bson::bob& builder, const char* key, const boost::shared_ptr<Type>& data)
        {
            if (NULL != data)
                appendToBuilder(builder, key, *data);
        }

        void appendToArrayBuilder(bson::BSONArrayBuilder& builder, const boost::shared_ptr<Type>& data)
        {
            if (NULL != data) 
                appendToArrayBuilder(builder, *data);
        }

        void appendToBuilderV2(bson::bob& builder, const char* key, const boost::shared_ptr<Type>& data)
        {
            if (NULL != data)
                appendToBuilderV2(builder, key, *data);
        }

        void appendToArrayBuilderV2(bson::BSONArrayBuilder& builder, const boost::shared_ptr<Type>& data)
        {
            if (NULL != data) 
                appendToArrayBuilderV2(builder, *data);
        }
    };

    template <typename Type>
    inline void bsonValue(const bson::be& bdata, Type& data)
    {
        if (!bdata.eoo())
        {
            Codec<Type>::instance().bsonValue(bdata, data);
        }
    }

    template <typename Type>
    inline void appendToBuilder(bson::bob& builder, const char* key, const Type& data)
    {
        Codec<Type>::instance().appendToBuilder(builder, key, data);
    }

    template <typename Type>
    inline void appendToArrayBuilder(bson::BSONArrayBuilder& builder, const Type& data)
    {
        Codec<Type>::instance().appendToArrayBuilder(builder, data);
    }


    template <typename Type>
    inline void appendToBuilderV2(bson::bob& builder, const char* key, const Type& data)
    {
        Codec<Type>::instance().appendToBuilderV2(builder, key, data);
    }

    template <typename Type>
    inline void appendToArrayBuilderV2(bson::BSONArrayBuilder& builder, const Type& data)
    {
        Codec<Type>::instance().appendToArrayBuilderV2(builder, data);
    }

    template <typename Type>
    inline bool safeParseBson(bson::bo bdata, const char* key, Type& value)
    {
        bson::be element = bdata.getFieldDotted(key);
        if (!element.eoo())
        {
            Codec<Type>::instance().bsonValue(element, value);
            return true;
        } else {
            return false;
        }
    }
}

#define OBJ_BSON_BUILDER_DECLEAR(Type)\
template <>\
class Codec<Type>\
{\
    CODEC_DECLEAR();\
public :\
    void appendToBuilder(bson::bob& builder, const char* key, const Type& data)\
    {\
        bson::bob objBuilder(builder.subobjStart(key));\
        data.appendElements(objBuilder);\
        objBuilder.done();\
    }\
    void appendToArrayBuilder(bson::BSONArrayBuilder& builder, const Type& data)\
    {\
        bson::bob objBuilder(builder.subobjStart());\
        data.appendElements(objBuilder);\
        objBuilder.done();\
}\
    void appendToBuilderV2(bson::bob& builder, const char* key, const Type& data)\
    {\
    appendToBuilder(builder, key, data);\
}\
    void appendToArrayBuilderV2(bson::BSONArrayBuilder& builder, const Type& data)\
    {\
    appendToArrayBuilder(builder, data);\
}\
    void bsonValue(const bson::be& bdata, Type& data)\
    {\
        bson::bo obj = bdata.Obj();\
        data.bsonValueFromObj(obj);\
    };\
};\
\
template <>\
class Codec<Type*>\
{\
    CODEC_DECLEAR();\
public :\
    void appendToBuilder(bson::bob& builder, const char* key, const Type*& data)\
    {\
        bson::bob objBuilder(builder.subobjStart(key));\
        if (NULL != data)\
            data->appendElements(objBuilder);\
        objBuilder.done();\
    }\
    void appendToArrayBuilder(bson::BSONArrayBuilder& builder, const Type*& data)\
    {\
        bson::bob objBuilder(builder.subobjStart());\
        if (NULL != data)\
            data->appendElements(objBuilder);\
        objBuilder.done();\
    }\
    void appendToBuilderV2(bson::bob& builder, const char* key, const Type*& data)\
    {\
    appendToBuilder(builder, key, data);\
}\
    void appendToArrayBuilderV2(bson::BSONArrayBuilder& builder, const Type*& data)\
    {\
    appendToArrayBuilder(builder, data);\
}\
    void bsonValue(const bson::be& bdata, Type*& data)\
    {\
        data = new Type();\
        bson::bo obj = bdata.Obj();\
        if (NULL != data)\
            data->bsonValueFromObj(obj);\
    };\
};\
\
template <>\
class Codec<boost::shared_ptr<Type> >\
{\
    CODEC_DECLEAR();\
public :\
    void appendToBuilder(bson::bob& builder, const char* key, const boost::shared_ptr<Type>& data)\
    {\
        bson::bob objBuilder(builder.subobjStart(key));\
        if (NULL != data)\
            data->appendElements(objBuilder);\
        objBuilder.done();\
    }\
    void appendToArrayBuilder(bson::BSONArrayBuilder& builder, const boost::shared_ptr<Type>& data)\
    {\
        bson::bob objBuilder(builder.subobjStart());\
        if (NULL != data)\
            data->appendElements(objBuilder);\
        objBuilder.done();\
    }\
    void appendToBuilderV2(bson::bob& builder, const char* key, const boost::shared_ptr<Type>& data)\
    {\
        appendToBuilder(builder, key, data);\
    }\
    void appendToArrayBuilderV2(bson::BSONArrayBuilder& builder, const boost::shared_ptr<Type>& data)\
    {\
        appendToArrayBuilder(builder, data);\
    }\
    void bsonValue(const bson::be& bdata, boost::shared_ptr<Type>& data)\
    {\
        data = boost::shared_ptr<Type>(new Type());\
        bson::bo obj = bdata.Obj();\
        if (NULL != data)\
            data->bsonValueFromObj(obj);\
    };\
};\
\
/* end */


#define IDATA_BSON_BUILDER_DECLEAR(Type)\
    template <>\
class Codec<Type>\
{\
    CODEC_DECLEAR();\
public :\
    void appendToBuilder(bson::bob& builder, const char* key, const Type& data)\
    {\
    bson::bob objBuilder(builder.subobjStart(key));\
    data.appendElements(objBuilder);\
    objBuilder.done();\
    }\
    void appendToArrayBuilder(bson::BSONArrayBuilder& builder, const Type& data)\
    {\
    bson::bob objBuilder(builder.subobjStart());\
    data.appendElements(objBuilder);\
    objBuilder.done();\
}\
    void appendToBuilderV2(bson::bob& builder, const char* key, const Type& data)\
    {\
    bson::bob objBuilder(builder.subobjStart(key));\
    data.appendElements_v2(objBuilder);\
    objBuilder.done();\
}\
    void appendToArrayBuilderV2(bson::BSONArrayBuilder& builder, const Type& data)\
    {\
    bson::bob objBuilder(builder.subobjStart());\
    data.appendElements_v2(objBuilder);\
    objBuilder.done();\
}\
    void bsonValue(const bson::be& bdata, Type& data)\
    {\
    bson::bo obj = bdata.Obj();\
    data.bsonValueFromObj(obj);\
    };\
};\
    \
    template <>\
class Codec<Type*>\
{\
    CODEC_DECLEAR();\
public :\
    void appendToBuilder(bson::bob& builder, const char* key, const Type*& data)\
    {\
    bson::bob objBuilder(builder.subobjStart(key));\
    if (NULL != data)\
    data->appendElements(objBuilder);\
    objBuilder.done();\
    }\
    void appendToArrayBuilder(bson::BSONArrayBuilder& builder, const Type*& data)\
    {\
    bson::bob objBuilder(builder.subobjStart());\
    if (NULL != data)\
    data->appendElements(objBuilder);\
    objBuilder.done();\
    }\
    void appendToBuilderV2(bson::bob& builder, const char* key, const Type*& data)\
    {\
    bson::bob objBuilder(builder.subobjStart(key));\
    if (NULL != data)\
    data->appendElements_v2(objBuilder);\
    objBuilder.done();\
}\
    void appendToArrayBuilderV2(bson::BSONArrayBuilder& builder, const Type*& data)\
    {\
    bson::bob objBuilder(builder.subobjStart());\
    if (NULL != data)\
    data->appendElements_v2(objBuilder);\
    objBuilder.done();\
}\
    void bsonValue(const bson::be& bdata, Type*& data)\
    {\
    data = new Type();\
    bson::bo obj = bdata.Obj();\
    if (NULL != data)\
    data->bsonValueFromObj(obj);\
    };\
};\
    \
    template <>\
class Codec<boost::shared_ptr<Type> >\
{\
    CODEC_DECLEAR();\
public :\
    void appendToBuilder(bson::bob& builder, const char* key, const boost::shared_ptr<Type>& data)\
    {\
    bson::bob objBuilder(builder.subobjStart(key));\
    if (NULL != data)\
    data->appendElements(objBuilder);\
    objBuilder.done();\
    }\
    void appendToArrayBuilder(bson::BSONArrayBuilder& builder, const boost::shared_ptr<Type>& data)\
    {\
    bson::bob objBuilder(builder.subobjStart());\
    if (NULL != data)\
    data->appendElements(objBuilder);\
    objBuilder.done();\
    }\
    void appendToBuilderV2(bson::bob& builder, const char* key, const boost::shared_ptr<Type>& data)\
    {\
    bson::bob objBuilder(builder.subobjStart(key));\
    if (NULL != data)\
    data->appendElements_v2(objBuilder);\
    objBuilder.done();\
    }\
    void appendToArrayBuilderV2(bson::BSONArrayBuilder& builder, const boost::shared_ptr<Type>& data)\
    {\
    bson::bob objBuilder(builder.subobjStart());\
    if (NULL != data)\
    data->appendElements_v2(objBuilder);\
    objBuilder.done();\
    }\
    void bsonValue(const bson::be& bdata, boost::shared_ptr<Type>& data)\
    {\
    data = boost::shared_ptr<Type>(new Type());\
    bson::bo obj = bdata.Obj();\
    if (NULL != data)\
    data->bsonValueFromObj(obj);\
    };\
};\
    \
    /* end */

#define ENUM_BSON_BUILDER_DECLEAR(Type)\
template <>\
class Codec<Type>\
{\
CODEC_DECLEAR();\
public :\
void appendToBuilder(bson::bob& builder, const char* key, const Type& data)\
{\
    builder.append(key, (int)data);\
}\
void appendToArrayBuilder(bson::BSONArrayBuilder& builder, const Type& data)\
{\
builder.append((int)data);\
}\
void appendToBuilderV2(bson::bob& builder, const char* key, const Type& data)\
{\
appendToBuilder(builder, key, data);\
}\
void appendToArrayBuilderV2(bson::BSONArrayBuilder& builder, const Type& data)\
{\
appendToArrayBuilder(builder, data);\
}\
void bsonValue(const bson::be& bdata, Type& data)\
{\
int v = 0;\
Codec<int>::instance().bsonValue(bdata, v);\
data = (Type)(v);\
};\
};\
    /* end */


#define BSON_PARAM_CHECK( v, keyName, param) \
    try\
    {\
        utils::bsonValue(param[keyName], v);\
    }\
    catch (const bson::bo& e)\
    {\
        throw bson::bob().append("error", (boost::format("parse %1% exception: %2%") % keyName % e.toString()).str()).obj();\
    }\
    catch (const bson::assertion& e)\
    {\
        throw bson::bob().append("error", (boost::format("parse %1% exception: %2%") % keyName % e.msg).str()).obj();\
    }\
    catch (...)\
    {\
        throw bson::bob().append("error", (boost::format("parse %1% exception: unknown error") % keyName).str()).obj();\
    }\
    /* PARAM_CHECK END */

#define BSON_PARAM_TYPE_CHECK(type, v, keyName, param) \
    type v;\
    BSON_PARAM_CHECK(v, keyName, param)\
    /* PARAM_CHECK_WITH_TYPE END */


#endif
