#include "common/Stdafx.h"
#include "utils/CrashHandler.h"
#include "common/loadlib.h"
#include "utils/commonFunc.h"

namespace utils
{
    void init_crashHandler(string strApp, string dumpDir)
    {
#ifdef WIN32
        void* lib = tt_load("crash.dll");
        if( lib == NULL )
        {
            TT_STDLOG(LLV_ERROR, "Failed to load crash");
            return;
        }

        void* func = tt_sym(lib, "init_handler_in_process");
        if(func == NULL)
        {
            TT_STDLOG(LLV_ERROR, "Failed to get func init_in_process_handler") ;
            return;
        }

        typedef bool (*Func)(const wchar_t *, const wchar_t *);
        Func f = (Func)func;
        wstring wstrApp = utils::sToWString(strApp);
        wstring wdumpDir = utils::sToWString(dumpDir);
        f(wstrApp.c_str(), wdumpDir.c_str());
#endif
    }

}