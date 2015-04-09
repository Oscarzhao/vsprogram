#include <iostream>
#include <XtTraderApi/XtTraderApi.h>

int main(int argc, char * argv[])
{
    using namespace xti;
    
    void (XtTraderApi::*userLogin0)(const char*, const char*, int) = &XtTraderApi::userLogin;
    void (XtTraderApi::*reqOrderDetailSI)(const char*, int)   = &XtTraderApi::reqOrderDetail;
    void (XtTraderApi::*reqOrderDetailSII)(const char*, int, int)  = &XtTraderApi::reqOrderDetail;

    std::cout << "XtTraderApi::userLogin: " << userLogin0 << std::endl;
    std::cout << "XtTraderApi::reqOrderDetailSI: " << reqOrderDetailSI << std::endl;
    std::cout << "XtTraderApi::reqOrderDetailSII: " << reqOrderDetailSII << std::endl;

    system("pause");
    return 0;

}