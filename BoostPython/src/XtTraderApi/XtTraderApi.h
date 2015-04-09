#ifndef XT_TRADER_API_H_
#define XT_TRADER_API_H_
#include <string>
#include <XtTraderApi/XtDef.h>

namespace xti
{
    struct XT_API_EXPORT CString
    {
        CString():m_nInt(0){strcpy(m_strArray, "undefined");}
        int      m_nInt;
        char     m_strArray[32];
        char     m_strMarket[500][32];
        int      m_nStocks[500];
        std::vector<int>  m_vec;
    };

    class XT_API_EXPORT World
    {
    public:
        void setMsg(std::string msg) ;
        std::string getMsg() const ;
        void readCString(const CString* cs);
    private:
        std::string msg;
    };

    class XtTraderApiCallback;

    class XT_API_EXPORT XtTraderApi
    {
    public:
        virtual ~XtTraderApi(){};
        static XtTraderApi* createXtTraderApi(const char* address);

        virtual void setCallback(XtTraderApiCallback* pCallback) = 0 ;

        virtual bool init() = 0;

        virtual void userLogin(const char* userName, const char* password, int nRequestId) = 0;
        virtual void reqOrderDetail(const char* accountID, int nRequestId) = 0;
        virtual void reqOrderDetail(const char* accountID, int nRequestId,int nOrderID) = 0;
    };

    class XT_API_EXPORT XtTraderApiCallback
    {
    public:
        virtual ~XtTraderApiCallback(){};
        virtual void onConnected(bool success, const char* errorMsg) {};
        virtual void onUserLogin(const char* userName, const char* password, int nRequestId) {};
    };


    XT_API_EXPORT char const* greet(const char* greeting);
}
#endif
