/**
 *  
 *
 *  @author xujun
 *  @since  2012-9-8
 */
#ifndef _TTError_2012_9_8_h__
#define _TTError_2012_9_8_h__

#include <bson/src/bson.h>
#include "utils/CSingletonT.h"
#include "utils/TTErrorDefines.h"
#include "utils/BsonHelper.h"

namespace utils
{
    class TTError
    {
    public:
        TTError();
        TTError(int errorID);
        TTError(int errorID, std::string msg);
        ~TTError();

        static TTError parse(bson::bo obj);
        static bson::bo makeErrorBson(int errorID, std::string msg);
        static bson::bo makeErrorBson(int errorID);
        static bson::bo makeErrorBson(std::string msg);

        void setErrorId(int id);
        void setErrorMsg(string msg);
        bool isSuccess() const;
        int errorID() const;
        std::string errorMsg() const;
        bson::bo toBson() const;

        // 此处兼容之前代码
        void bsonValueFromObj(bson::bo obj)
        {
            if (!obj.isEmpty())
            {
                bson::be element = obj.getFieldDotted("ErrorID");
                if (!element.eoo())
                {
                    m_nErrorID = element.numberInt();          
                    element = obj.getFieldDotted("ErrorMsg");
                    m_strErrorMsg = element.String();
                } else {
                    m_nErrorID = TT_ERROR_DEFAULT;
                    m_strErrorMsg = obj.toString();
                }
            }
        };

        void appendElements(bson::bob& objBuilder) const
        {
            utils::appendToBuilder(objBuilder, "ErrorID", m_nErrorID);
            utils::appendToBuilder(objBuilder, "ErrorMsg", m_strErrorMsg);
        };

        static void init() ;

        bool operator!() const  // true if no error
        {
            return m_nErrorID == 0;
        }

    protected:
    private:
        int m_nErrorID;
        std::string m_strErrorMsg;
    };

    OBJ_BSON_BUILDER_DECLEAR(TTError);

    class TTErrorManager 
        : public CSingletonT<TTErrorManager>
    {
    public:
        void addErrors(const std::map<unsigned int, std::string>& errors);
        std::string getError(unsigned int id) const;

    private:
        std::map<unsigned int, std::string> m_errors;
    };

    typedef boost::shared_ptr<TTError> TTErrorPtr;
}

#endif // TTError_h__