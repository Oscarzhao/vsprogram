#ifndef XT_TRADER_API_IMP_H_
#define XT_TRADER_API_IMP_H_
#include <string>
#include <XtTraderApi/XtTraderApi.h>

namespace xti
{
    class XtTraderApiImp: public XtTraderApi
    {
    public:
        XtTraderApiImp(const char * address);
        virtual void setCallback(XtTraderApiCallback* pCallback) ;

        virtual bool init() ;

        virtual void userLogin(const char* userName, const char* password, int nRequestId);
        virtual void reqOrderDetail(const char* accountID, int nRequestId);
        virtual void reqOrderDetail(const char* accountID, int nRequestId,int nOrderID);
    private:
        std::string             m_address;
        std::string             m_strUserName;
        std::string             m_strSessionTag;
        std::string             m_strMacAddr;

        XtTraderApiCallback*    m_callback;
    };
}
#endif
