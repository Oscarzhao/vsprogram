#include <boost/python/module.hpp>
#include <boost/python/def.hpp>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <XtTraderApi/XtTraderApi.h>
#include <iostream>
using namespace boost::python;
namespace xti
{

    class XtTraderApiWrap : public XtTraderApi, public wrapper<XtTraderApi>
    {
    public:
        //void setCallback(XtTraderApiCallback *pCallback)
        //{
        //    this->get_override("setCallback")(pCallback);
        //}

        void setCallback(void *pCallback)
        {
            this->get_override("setCallback")(pCallback);
        }

        bool init()
        {
            return this->get_override("init")();
        }
        void destroy()
        {
            this->get_override("destroy")();
        }
        void join()
        {
            this->get_override("join")();
        }

        void userLogin(const char* userName, const char* password, int nRequestId)
        {
            this->get_override("userLogin")(userName, password, nRequestId);
        }

        void reqOrderDetail(const char* accountID, int nRequestId)
        {
            this->get_override("reqOrderDetail")(accountID, nRequestId);
        }

        void reqOrderDetail(const char* accountID, int nRequestId,int nOrderID)
        {
            this->get_override("reqOrderDetail")(accountID, nRequestId, nOrderID);
        }
    };

    class XtTraderApiCallbackWrap : public XtTraderApiCallback, public wrapper<XtTraderApiCallback>
    {
    public:
        void onConnected(bool success, const char* errorMsg)
        {
            if( override onConnected = this->get_override("onConnected"))
                onConnected(success, errorMsg);
            XtTraderApiCallback::onConnected(success, errorMsg);
        }

        void default_onConnected(bool success, const char* errorMsg)
        {
            this->XtTraderApiCallback::onConnected(success, errorMsg);
        }

        void onUserLogin(const char* userName, const char* password, int nRequestId)
        {
            if( override onUserLogin = this->get_override("onUserLogin"))
                onUserLogin( userName,  password, nRequestId);
            XtTraderApiCallback::onUserLogin( userName,  password, nRequestId);
        }

        void default_onUserLogin(const char* userName, const char* password, int nRequestId)
        {
            this->XtTraderApiCallback::onUserLogin(userName, password, nRequestId);
        }
    };

    boost::python::str m_str2ArrayWrapGet(CString const * cs)
    {
        boost::python::list li;
        int arrLen = strlen(cs->m_strArray);
        for(int i = 0;i < arrLen; i++)  li.append(cs->m_strArray[i]);
        boost::python::str tmp = "";
        return tmp.join(li);
    }

    void m_strArrayWrapperSet(CString * const cs, boost::python::str pyStr)
    {
        int len = boost::python::len(pyStr);

        for(int i = 0; i < len; i++)
            cs->m_strArray[i] = boost::python::extract<char>(pyStr[i]);
        cs->m_strArray[len] = '\0';
    }

    boost::python::list m_nStocksWrapGet(CString const * cs)
    {
        boost::python::list li;
        int arrLen = 500;
        for(int i = 0;i < arrLen; i++)  li.append(cs->m_nStocks[i]);
        return li;
    }

    void m_nStocksWrapperSet(CString * const cs, boost::python::list li)
    {
        int len = boost::python::len(li);

        for(int i = 0; i < len; i++)
            cs->m_nStocks[i] = boost::python::extract<int>(li[i]);
    }


    boost::python::list m_strMarketWrapperGet(CString const * cs)
    {
        boost::python::list li;
        int arrLen = 500;
        boost::python::str emptyStr = "";
        for(int i = 0;i < arrLen; i++)  
        {
            boost::python::list innerLi;
            int innerLen = strlen(cs->m_strMarket[i]);
            for (int j = 0; j < innerLen; j++)
            {
                innerLi.append(cs->m_strMarket[i][j]);
            }
            li.append(emptyStr.join(innerLi));
        }
        return li;
    }

    void m_strMarketWrapperSet(CString * const cs, boost::python::list strLi)
    {
        int len = boost::python::len(strLi);
        for(int i = 0; i < len; i++)
        {
            int strLen = boost::python::len(strLi[i]);
            for (int j = 0; j < strLen; j++ )
            {
                cs->m_strMarket[i][j] = boost::python::extract<char>(strLi[i][j]);
            }
            cs->m_strMarket[i][strLen] = '\0';
        }
    }

    BOOST_PYTHON_MODULE(XtTraderPyApi)
    {
        def("greet", greet);

        class_<World>("World")
            .def("getMsg", &World::getMsg)
            .def("setMsg", &World::setMsg)
            .def("readCString", &World::readCString)
            ;

        class_<std::vector<int> >("IntVec")
            .def(vector_indexing_suite<std::vector<int> >())
            ;
        class_<CString>("CString")
            .def_readwrite("m_nInt", &CString::m_nInt)
            .add_property("m_strArray", m_str2ArrayWrapGet, m_strArrayWrapperSet)
            .add_property("m_nStocks", m_nStocksWrapGet, m_nStocksWrapperSet)
            .add_property("m_strMarket", m_strMarketWrapperGet, m_strMarketWrapperSet)
            .def_readwrite("m_vec", &CString::m_vec)
            ;

        void (XtTraderApi::*reqOrderDetailSI)(const char*, int)   = &XtTraderApi::reqOrderDetail;
        void (XtTraderApi::*reqOrderDetailSII)(const char*, int, int)  = &XtTraderApi::reqOrderDetail;

        class_<XtTraderApiWrap, boost::noncopyable>("XtTraderApi")
            .def("createXtTraderApi", &XtTraderApi::createXtTraderApi, return_value_policy<manage_new_object>())
            .staticmethod("createXtTraderApi")
            .def("setCallback", &XtTraderApiWrap::setCallback)
            //.def("setCallback", pure_virtual(&XtTraderApi::setCallback))
            .def("init", pure_virtual(&XtTraderApi::init))
            .def("userLogin", pure_virtual(&XtTraderApi::userLogin))
            .def("reqOrderDetail", pure_virtual(reqOrderDetailSI))
            .def("reqOrderDetail", pure_virtual(reqOrderDetailSII))
            ;
        class_<XtTraderApiCallbackWrap, boost::noncopyable>("XtTraderApiCallback")
            .def("onConnected", &XtTraderApiCallback::onConnected, &XtTraderApiCallbackWrap::default_onConnected)
            .def("onUserLogin", &XtTraderApiCallback::onUserLogin, &XtTraderApiCallbackWrap::default_onUserLogin)
            ;
    }
}