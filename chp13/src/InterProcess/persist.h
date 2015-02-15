#ifndef PERSIST_H__
#define PERSIST_H__
/*
============== author : zhaoshuailong ======================
============== ���� GdbmService ��linux�±������� ========
============== ��Ҫ�ǲ鿴 m_pFile �Ĵ��� �� ���� =========
*/
#include <InterProcess/Threading.h>

namespace persist
{
    struct CGdbmContext
    {
        CGdbmContext();
        CGdbmContext(const CGdbmContext &);
        virtual ~CGdbmContext();

        std::string * m_pFile;
        boost::recursive_mutex m_mutex;
    };

    class CPersistService
    {
    public:
        CPersistService();
        ~CPersistService();

        void addPair(std::string strKey, bool& success);
        void testPair(const std::string& strKey, std::string& retStr);

    private:
        
        std::map< std::string, CGdbmContext > m_gdbmMap;
        
    };
}
#endif