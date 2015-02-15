#include <InterProcess/persist.h>

namespace persist
{
    CGdbmContext::CGdbmContext():
        m_pFile(NULL)
        {

        }
        CGdbmContext::CGdbmContext( const CGdbmContext & rhs)
            :m_pFile( rhs.m_pFile )
        {

        }

        CGdbmContext::~CGdbmContext()
        {
            std::cout << "[CGdbmContext::~CGdbmContext] enter\n";
            if( NULL != m_pFile )
            {
                std::cout << "[CGdbmContext::~CGdbmContext] free m_pFile\n";
                free(m_pFile);
            }
            std::cout << "[CGdbmContext::~CGdbmContext] leave\n";
        }

        CPersistService::CPersistService()
        {

        }

        CPersistService::~CPersistService()
        {
            m_gdbmMap.clear();
        }

        void CPersistService::addPair(std::string strKey, bool& success )
        {
            std::cout << "[CPersistService::addPair] enter\n";
            std::map< std::string, CGdbmContext >::iterator iter;
            iter = m_gdbmMap.find(strKey);
            if(iter != m_gdbmMap.end())
            {
                success = false;
                return;
            }

            std::cout << "[CPersistService::addPair] nextLine: \"CGdbmContext& context = m_gdbmMap[strKey]\" shall be executed\n";
            CGdbmContext& context = m_gdbmMap[strKey];
            utils::RecLock lock(context.m_mutex);
            if(NULL == context.m_pFile)
            {
                context.m_pFile = new std::string(strKey);
                success = true;
            }
            std::cout << "[CPersistService::addPair] leave\n";
        }
        void CPersistService::testPair(const std::string& strKey, std::string& retStr )
        {
            std::map< std::string, CGdbmContext >::iterator iter = m_gdbmMap.find(strKey);
            if(m_gdbmMap.end() == iter)
            {
                return;
            }
            else
            {
                utils::RecLock lock(iter->second.m_mutex);
                retStr = *(iter->second.m_pFile);
            }
        }
}