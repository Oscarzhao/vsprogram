#include <XtTraderApi/XtTraderApiImp.h>
#include <iostream>
namespace xti
{
    XtTraderApiImp::XtTraderApiImp(const char* address)
        : m_address(address)
        , m_callback(NULL)
    {
    }

    bool XtTraderApiImp::init()
    {
        std::cout << "[XtTraderApiImp::init]" << std::endl;
        return true;
    }

    void XtTraderApiImp::setCallback(XtTraderApiCallback* pCallback) 
    {
        m_callback = pCallback;
    }

    void XtTraderApiImp::userLogin(const char* userName, const char* password, int nRequestId)
    {
        std::cout << "[XtTraderApiImp::userLogin]" << std::endl;
    }

    void XtTraderApiImp::reqOrderDetail(const char* accountID, int nRequestId)
    {
        std::cout << "[XtTraderApiImp::reqOrderDetail] two params" << std::endl;
    }

    void XtTraderApiImp::reqOrderDetail(const char* accountID, int nRequestId,int nOrderID)
    {
        std::cout << "[XtTraderApiImp::reqOrderDetail] three params" << std::endl;
    }
}