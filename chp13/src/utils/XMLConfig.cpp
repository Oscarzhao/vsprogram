
/**
* XML文件的常用函数
* 
* @author   zld
* @date     2013-03-15
* @version  1.0
*/

#include <common/Stdafx.h>
#include <utils/XMLConfig.h>
#include <utils/strings.h>
#include "log4cxx/logger.h"
#include <boost/filesystem.hpp>
#include "ttLog.h"
using namespace std;

namespace utils
{ 
    void XMLConfig::setConfig(string filepath, string firstChildElement, const vector< pair<string, string> >& attributes)
    {
        TiXmlDocument *pDoc = new TiXmlDocument();
        if (NULL == pDoc)
        {
            return;
        }
        pDoc->LoadFile(filepath.c_str(), TIXML_ENCODING_UNKNOWN);
        TT_STDLOG(LLV_INFO, "LoadFile firstChildElement is %s",firstChildElement.c_str());
        const TiXmlElement *pRootElement = pDoc->RootElement();
        if (pRootElement != NULL)
        {
            TT_STDLOG(LLV_INFO, "pRootElement is not null %d",attributes.size());
            TiXmlElement * pGlobalElement = const_cast<TiXmlElement *>(pRootElement->FirstChildElement(firstChildElement.c_str()));
            if (NULL != pGlobalElement)
            {
                TT_STDLOG(LLV_INFO, "pGlobalElement is not null %d",attributes.size());
                for (vector< pair<string, string> >::const_iterator iter = attributes.begin(); iter != attributes.end(); iter++)
                {
                    pGlobalElement->SetAttribute(iter->first.c_str(), iter->second.c_str());
                    TT_STDLOG(LLV_INFO, "SetAttribute is %s %s",iter->first.c_str(), iter->second.c_str());
                }
            }
        }
        else
        {
            TiXmlElement * pNewRootElement = new TiXmlElement("TTDaemonConfig");
            TT_STDLOG(LLV_INFO, "pGlobalElement is not null %d",attributes.size());
            if (NULL != pNewRootElement)
            {
                TiXmlElement * pGlobalElement = new TiXmlElement(firstChildElement.c_str());
                if (pGlobalElement != NULL)
                {
                    for (vector< pair<string, string> >::const_iterator iter = attributes.begin(); iter != attributes.end(); iter++)
                    {
                        pGlobalElement->SetAttribute(iter->first.c_str(), iter->second.c_str());
                        TT_STDLOG(LLV_INFO, "SetAttribute is %s %s",iter->first.c_str(), iter->second.c_str());
                    }
                    pNewRootElement->LinkEndChild(pGlobalElement);
                }
                pDoc->LinkEndChild(pNewRootElement);
                //delete(pGlobalElement);
            }
        }
        pDoc->SaveFile(filepath.c_str());
        delete(pDoc);
        TT_STDLOG(LLV_INFO, "filepath is %s",filepath.c_str());
    }
    
    vector<string> XMLConfig::getConfig(string strConfigFile, string firstChildElement, const vector<string>& attributeNames)
    {
        vector<string> ret;
        TiXmlDocument doc(utils::trimQuotation(strConfigFile).c_str());
        if (!doc.LoadFile(TIXML_ENCODING_UNKNOWN))
        {
            return ret;
        }

        const TiXmlElement *pRootElement = doc.RootElement();
        if (NULL != pRootElement)
        {
            try
            {
                const TiXmlElement * pConfigElement = pRootElement->FirstChildElement(firstChildElement.c_str());
                if (NULL != pConfigElement)
                {
                    ret.resize(attributeNames.size());
                    size_t i = 0;
                    for (vector< string >::const_iterator iter = attributeNames.begin(); iter != attributeNames.end(); iter++)
                    {
                        const char * tmpchr = pConfigElement->Attribute(iter->c_str());
                        if (tmpchr)
                        {
                            ret[i] = tmpchr;
                        }
                        else
                        {
                            ret[i] = "";
                        }
                        //ret[i] = pConfigElement->Attribute(iter->c_str());
                        i++;
                    }
                }
            }
            catch (...)
            {
            }
        }
        return ret;
    }
}
