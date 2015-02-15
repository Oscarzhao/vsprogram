#ifndef PERSIST_H__
#define PERSIST_H__
/*
============== author : zhaoshuailong ======================
============== 测试 GdbmService 在linux下崩溃问题 ========
============== 主要是查看 m_pFile 的创建 和 析构 =========
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