#include "utils/TTError.h"
#include "utils/BsonHelper.h"
#include "utils/STLHelper.h"
#include "utils/CSingletonT.hpp"

IMPL_SINGLETON_CLASS(utils::TTErrorManager);

namespace utils
{
    static int baseInitilaze()
    {
        TTErrorManager::instance()->addErrors(genBaseErrorMap());
        return 0;
    }
    static int s_baseTemp = baseInitilaze();

    TTError::TTError()
        :m_nErrorID(0)
    {

    }

    TTError::TTError(int errorID)
        :m_nErrorID(errorID)
    {
        m_strErrorMsg = TTErrorManager::instance()->getError(errorID);
    }

    TTError::TTError(int errorID, std::string msg)
        :m_nErrorID(errorID),
        m_strErrorMsg(msg)
    {

    }

    TTError::~TTError()
    {

    }

    void TTError::setErrorId(int id)
    {
        m_nErrorID = id;
    }

    void TTError::setErrorMsg(string msg)
    {
        m_strErrorMsg = msg;
    }

    TTError TTError::parse(bson::bo obj)
    {
        TTError ret;
        ret.bsonValueFromObj(obj);
        return ret;
    }

    bson::bo TTError::makeErrorBson(int errorID, std::string msg)
    {
        TTError error(errorID, msg);
        bson::bob builder;
        error.appendElements(builder);
        return builder.obj();
    }

    bson::bo TTError::makeErrorBson(int errorID)
    {
        std::string msg = TTErrorManager::instance()->getError(errorID);
        return makeErrorBson(errorID, msg);
    }

    bson::bo TTError::makeErrorBson(std::string msg)
    {
        return makeErrorBson(TT_ERROR_DEFAULT, msg);
    }

    int TTError::errorID() const 
    {
        return m_nErrorID;
    }

    bool TTError::isSuccess() const
    {
        return !(*this);
    }

    std::string TTError::errorMsg() const 
    {
        return m_strErrorMsg;
    }

    bson::bo TTError::toBson() const
    {
        return makeErrorBson(m_nErrorID, m_strErrorMsg);
    }

    void TTErrorManager::addErrors(const std::map<unsigned int, std::string>& errors)
    {
        for (std::map<unsigned int, std::string>::const_iterator iter = errors.begin(); iter != errors.end(); ++iter)
        {
            utils::replace(m_errors, iter->first, iter->second);
        }
    }

    std::string TTErrorManager::getError(unsigned int id) const
    {
        return utils::getMapValue(m_errors, id);
    }
};
