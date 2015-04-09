//  Copyright Joel de Guzman 2002-2004. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//  Hello World Example from the tutorial
//  [Joel de Guzman 10/9/2002]

#include <boost/python/module.hpp>
#include <boost/python/def.hpp>
#include "XtTraderApi/XtTraderApi.h"
#include "XtTraderApi/XtTraderApiImp.h"

namespace xti
{
    void World::setMsg(std::string msg)
    {
        this->msg = msg;
    }

    std::string World::getMsg() const
    {
        return msg;
    }

    void World::readCString(const CString* cs)
    {
        std::cout << cs->m_strArray << std::endl;
    }

    XtTraderApi * XtTraderApi::createXtTraderApi(const char* address)
    {
        XtTraderApi* pInstance = new XtTraderApiImp(address);
        return pInstance;
    }

    char const* greet(const char* greeting)
    {
        return greeting;
    }
}
