/**
 *  操作系统相关系统调用
 *
 *  @uathor zhangyi
 *  @since  2012-06-05
 */

#include <utils/sysutils.h>
#include <utils/commonFunc.h>
#include <utils/strings.h>

#ifdef WIN32
#include <windows.h>
#include <ShellAPI.h>
#include <tchar.h>
#include <intrin.h>
#include <ShlObj.h>
#include <Psapi.h>
#include <comdef.h>
#include <Wbemidl.h>
#include <process.h>
#pragma comment(lib, "Wbemuuid.lib")
#pragma comment(lib, "kernel32.lib")
#pragma comment(lib, "Psapi.lib")

#else
#include <unistd.h>
#include <stdio.h>
#endif

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

#pragma warning (disable : 4800)

namespace utils
{
    int getPid()
    {
#ifdef WIN32
        return _getpid();
#else
        return getpid();
#endif
    }

    string getAppName()
    {
        string ret ;
#ifdef WIN32
        HANDLE handler = GetCurrentProcess();
        TCHAR Buffer[MAX_PATH];
        if (GetModuleFileNameEx(handler, 0, Buffer, MAX_PATH))
        {
            // At this point, buffer contains the full path to the executable
        }
        else
        {
            // You better call GetLastError() here
        }
        wstring appName(Buffer);
        ret = wsToString(appName);
        // 返回的数据如:g:\server5\ttservice\trunk\_runtime\Debug\XtService.exe
        // 而期望得到的是XtService, 做字符串字段截取
        size_t pos = ret.find_last_of("\\");
        ret = ret.substr(pos + 1, ret.size() - pos -1);
        boost::algorithm::replace_all(ret, ".exe", "");
#else
        //ret = getprogname();
#endif
        return ret;
    }

    void filter_enter(char *str)
    {
        int len, i;
        i = 0;
        len = strlen(str);
        while(i<len)
        {
            if( (str[i] == '\n') || (str[i] == '\r') )
                break;
            i++;
        }
        str[i] = 0;
    }

    string getAppFullPath()
    {
#ifdef WIN32

        TCHAR szRootPath[MAX_PATH] = { 0 };
        ::GetModuleFileName(NULL, szRootPath, MAX_PATH);
    #ifdef _UNICODE
        char szRootPathA[MAX_PATH] = { 0 };
        ::WideCharToMultiByte(936, 0, szRootPath, MAX_PATH, szRootPathA, MAX_PATH, " ", NULL);
        return szRootPathA;
    #else
        return szRootPath;
    #endif

#else

        //pid
        int pid = getpid();

        //cwd
        char cwd_buf[512] = {0};
        char tmp_buf[256] = {0};
        sprintf(tmp_buf, "/proc/%d/cwd", pid);
        int count = readlink(tmp_buf, cwd_buf, sizeof(cwd_buf) );
        if( count > 0 && count < sizeof(cwd_buf) )
        {
            cwd_buf[ count ] = '\0';
        }
        string strCwd = cwd_buf;

        //pname
        FILE *fp = NULL;
        char buf[256] = {0};
        char readerbuf[256] = {0};
        sprintf(buf, "ps -p %d -o comm --no-header", pid);
        if ((fp = popen(buf, "r")) != NULL)
        {
            bzero(readerbuf, sizeof(readerbuf));
            fgets(readerbuf, sizeof(readerbuf), fp);
            filter_enter(readerbuf);
            fclose(fp);
        }
        string strName = readerbuf;

        return  strCwd + "/" + strName;

#endif
    }

    string getAppDir()
    {
        return utils::getDir(getAppFullPath());
    }

    string getAppDataDir()
    {
#ifdef WIN32

        char szAppDir[MAX_PATH] = { 0 };
        ::SHGetFolderPathA(NULL, CSIDL_APPDATA, NULL, SHGFP_TYPE_CURRENT, szAppDir);
        return string(szAppDir);

#else
        boost::filesystem::path appDir(getAppDir());
        return appDir.parent_path().string();
#endif // WIN32
    }

    unsigned int startProcess( const string & appFullPath, const string & args )
    {
        string workingDir = utils::getDir(appFullPath);
        string strArgs = args;
        if (strArgs[0] != '\"')
        {
            strArgs = string("\"") + strArgs + "\"";
        }

#ifdef WIN32

        SHELLEXECUTEINFOA info;
        info.cbSize = sizeof(SHELLEXECUTEINFOA);
        info.fMask = SEE_MASK_NOCLOSEPROCESS;   // 不加这个hProcess无法得到真正的句柄
        info.hwnd = NULL;
        info.lpVerb = NULL;
        info.lpFile = appFullPath.c_str();
        info.lpParameters = strArgs.c_str();
        info.lpDirectory = workingDir.c_str();
        info.nShow = SW_SHOW;

        ::ShellExecuteExA(&info);

        if (NULL != info.hProcess)
        {
            int nPid = ::GetProcessId(info.hProcess);
            ::CloseHandle(info.hProcess);
            return nPid;
        }

#else

        // to do : under linux ?

#endif

        return INVALID_PID;
    }

    unsigned int startProcessWithOutQuotation( const string & appFullPath, const string & args )
    {
        string workingDir = utils::getDir(appFullPath);

#ifdef WIN32

        char szArgs[MAX_PATH];
        memset(szArgs, 0, MAX_PATH);
        sprintf(szArgs, "\"%s\" %s", appFullPath.c_str(), args.c_str());

        STARTUPINFOA si;
        ZeroMemory( &si, sizeof(si) );
        si.cb = sizeof(si);

        PROCESS_INFORMATION pi;
        ZeroMemory( &pi, sizeof(pi) );

        ::CreateProcessA(NULL, szArgs, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
        return pi.dwProcessId;

#else

        // to do : under linux ?

#endif

        return INVALID_PID;
    }

    unsigned int startProcess( const string & appFullPath, const list<string> & args )
    {
        string param;
        for (list<string>::const_iterator cIter = args.begin(); cIter != args.end(); cIter++)
        {
            param += "\"";
            param += *cIter;
            param += "\" ";
        }

        if (!param.empty())
        {
            param[param.size() - 1] = '\0';
        }

        return startProcess(appFullPath, param);
    }

    void killProcesses( string strProcessName )
    {
#ifdef WIN32

        static const int c_sMaxProcesses = 1024;
        DWORD aProcesses[c_sMaxProcesses];
        ZeroMemory(aProcesses, c_sMaxProcesses * sizeof(DWORD));
        DWORD cbNeeded = 0, cProcesses = 0, i = 0;
        DWORD dwCurrentPID = ::GetCurrentProcessId();

        if (::EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded))
        {
            // 找到进程列表
            cProcesses = cbNeeded / sizeof(DWORD);
            for (i = 0; i < cProcesses; i++)
            {
                // 对每个进程，首先判断PID。
                if (NULL != aProcesses[i])
                {
                    // PID判断通过
                    char szProcessName[MAX_PATH];
                    ZeroMemory(szProcessName, MAX_PATH * sizeof(char));
                    HANDLE hProcess = ::OpenProcess(
                        PROCESS_QUERY_INFORMATION | PROCESS_VM_READ | PROCESS_TERMINATE,
                        FALSE, aProcesses[i]);
                    if (NULL != hProcess)
                    {
                        // 打开进程信息
                        HMODULE hMod = NULL;
                        DWORD cbNeeded = 0;
                        if (::EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeeded))
                        {
                            ::GetModuleBaseNameA(hProcess, hMod, szProcessName, sizeof(szProcessName) / sizeof(TCHAR));
                            if (0 == strProcessName.compare(szProcessName))
                            {
                                ::TerminateProcess(hProcess, 0);
                            }
                        }
                        ::CloseHandle(hProcess);
                    }
                }
            }
        }

#else

#endif
    }

    void killProcessesFull( string strProcessFullPath )
    {
#ifdef WIN32

        utils::string_replace(strProcessFullPath, "/", "\\");

        static const int c_sMaxProcesses = 1024;
        DWORD aProcesses[c_sMaxProcesses];
        ZeroMemory(aProcesses, c_sMaxProcesses * sizeof(DWORD));
        DWORD cbNeeded = 0, cProcesses = 0, i = 0;
        DWORD dwCurrentPID = ::GetCurrentProcessId();

        if (::EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded))
        {
            // 找到进程列表
            cProcesses = cbNeeded / sizeof(DWORD);
            for (i = 0; i < cProcesses; i++)
            {
                // 对每个进程，首先判断PID。
                if (NULL != aProcesses[i])
                {
                    // PID判断通过
                    char szProcessFullPath[MAX_PATH];
                    ZeroMemory(szProcessFullPath, MAX_PATH * sizeof(char));
                    HANDLE hProcess = ::OpenProcess(
                        PROCESS_QUERY_INFORMATION | PROCESS_VM_READ | PROCESS_TERMINATE,
                        FALSE, aProcesses[i]);
                    if (NULL != hProcess)
                    {
                        // 打开进程信息
                        HMODULE hMod = NULL;
                        DWORD cbNeeded = 0;
                        if (::EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeeded))
                        {
                            ::GetModuleFileNameExA(hProcess, hMod, szProcessFullPath, sizeof(szProcessFullPath) / sizeof(TCHAR));
                            if (0 == stricmp(szProcessFullPath, strProcessFullPath.c_str()))
                            {
                                ::TerminateProcess(hProcess, 0);
                            }
                        }
                        ::CloseHandle(hProcess);
                    }
                }
            }
        }

#endif // WIN32
    }

    bool isProcessAlive( unsigned int nPid, string strProcessName )
    {
#ifdef WIN32

        bool bRes = false;

        HANDLE hProcess = ::OpenProcess(PROCESS_VM_READ | PROCESS_QUERY_INFORMATION, FALSE, nPid);
        if (NULL != hProcess)
        {
            // 打开进程信息
            HMODULE hMod = NULL;
            DWORD cbNeeded = 0;
            if (::EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeeded))
            {
                char szProcessName[MAX_PATH];
                ZeroMemory(szProcessName, MAX_PATH * sizeof(char));
                ::GetModuleBaseNameA(hProcess, hMod, szProcessName, sizeof(szProcessName) / sizeof(TCHAR));
                if (0 == strProcessName.compare(szProcessName))
                {
                    DWORD dwStatus = STILL_ACTIVE;
                    if (::GetExitCodeProcess(hProcess, &dwStatus) && (STILL_ACTIVE == dwStatus))
                    {
                        bRes = true;
                    }
                }
            }
            else
            {
                // 此函数用于判断进程是否活着，如果为否会再次起进程（void CDaemonServer::do_watch()）
                // 取不到进程信息，保存起见，认为其活着，否则外围会反复起进程

                // zhangyi : 2013-06-19
                // 现在看来还真有可能查失败，先认为是死了吧……
                bRes = false;
            }
            ::CloseHandle(hProcess);
        }

        return bRes;

#else
        return false;
#endif
    }

    bool isProcessAlive( string strProcessName )
    {
#ifdef _WIN32
        static const int c_sMaxProcesses = 1024;
        DWORD aProcesses[c_sMaxProcesses];
        ZeroMemory(aProcesses, c_sMaxProcesses * sizeof(DWORD));
        DWORD cbNeeded = 0, cProcesses = 0, i = 0;
        DWORD dwCurrentPID = ::GetCurrentProcessId();

        if (::EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded))
        {
            // 找到进程列表
            cProcesses = cbNeeded / sizeof(DWORD);
            for (i = 0; i < cProcesses; i++)
            {
                // 对每个进程，首先判断PID。
                if (NULL != aProcesses[i])
                {
                    // PID判断通过
                    char szProcessName[MAX_PATH];
                    ZeroMemory(szProcessName, MAX_PATH * sizeof(char));
                    HANDLE hProcess = ::OpenProcess(
                        PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,
                        FALSE, aProcesses[i]);
                    if (NULL != hProcess)
                    {
                        // 打开进程信息
                        HMODULE hMod = NULL;
                        DWORD cbNeeded = 0;
                        bool bRes = false;
                        if (::EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeeded))
                        {
                            ::GetModuleBaseNameA(hProcess, hMod, szProcessName, sizeof(szProcessName) / sizeof(TCHAR));
                            if (0 == strProcessName.compare(szProcessName))
                            {
                                DWORD dwStatus = STILL_ACTIVE;
                                if (::GetExitCodeProcess(hProcess, &dwStatus) && (STILL_ACTIVE == dwStatus))
                                {
                                    bRes = true;
                                }
                            }
                        }
                        ::CloseHandle(hProcess);
                        if (bRes)
                        {
                            return true;
                        }
                    }
                }
            }
        }
#endif
        return false;
    }

    bool isProcessAliveFull( string strProcessFullPath, string & strMsg )
    {
#ifdef _WIN32
        static const int c_sMaxProcesses = 1024;
        DWORD aProcesses[c_sMaxProcesses];
        ZeroMemory(aProcesses, c_sMaxProcesses * sizeof(DWORD));
        DWORD cbNeeded = 0, cProcesses = 0, i = 0;
        DWORD dwCurrentPID = ::GetCurrentProcessId();

        utils::string_replace(strProcessFullPath, "/", "\\");

        if (::EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded))
        {
            // 找到进程列表
            cProcesses = cbNeeded / sizeof(DWORD);
            for (i = 0; i < cProcesses; i++)
            {
                // 对每个进程，首先判断PID。
                if (NULL != aProcesses[i])
                {
                    // PID判断通过
                    char szProcessFullPath[MAX_PATH];
                    ZeroMemory(szProcessFullPath, MAX_PATH * sizeof(char));
                    HANDLE hProcess = ::OpenProcess(
                        PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,
                        FALSE, aProcesses[i]);
                    if (NULL != hProcess)
                    {
                        // 打开进程信息
                        HMODULE hMod = NULL;
                        DWORD cbNeeded = 0;
                        bool bRes = false;
                        if (::EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeeded))
                        {
                            ::GetModuleFileNameExA(hProcess, hMod, szProcessFullPath, sizeof(szProcessFullPath) / sizeof(TCHAR));
                            if (0 == stricmp(szProcessFullPath, strProcessFullPath.c_str()))
                            {
                                DWORD dwStatus = STILL_ACTIVE;
                                if (::GetExitCodeProcess(hProcess, &dwStatus))
                                {
                                    if (STILL_ACTIVE == dwStatus)
                                    {
                                        bRes = true;
                                    }
                                    else
                                    {
                                        strMsg + "Pid=" + boost::lexical_cast<string>(aProcesses[i])
                                            + "dwStatus = " + boost::lexical_cast<string>(dwStatus);
                                    }
                                }
                                else
                                {
                                    strMsg + "Pid=" + boost::lexical_cast<string>(aProcesses[i])
                                        + "GetExitCodeProcess fail = " + boost::lexical_cast<string>(::GetLastError());
                                }
                            }
                        }
                        else
                        {
                            strMsg = strMsg + "Pid=" + boost::lexical_cast<string>(aProcesses[i])
                                + "EnumProcessModules fail = " + boost::lexical_cast<string>(::GetLastError());
                        }
                        ::CloseHandle(hProcess);
                        if (bRes)
                        {
                            return true;
                        }
                    }
                    else
                    {
                        strMsg = strMsg + "Pid=" + boost::lexical_cast<string>(aProcesses[i])
                            + "OpenProcess fail = " + boost::lexical_cast<string>(::GetLastError());
                    }
                }
            }
        }
        else
        {
            strMsg = "EnumProcesses fail = " + boost::lexical_cast<string>(::GetLastError());
        }
#endif
        return false;
    }

    string getCpuId()
    {
        string cpuId;
#ifdef WIN32

        int cpu_info[4] = {-1};
        __cpuid(cpu_info, 1);
        bool isSupport = (cpu_info[3] & (1<<18));     // 是否为1代表CPU是否存在序列号
        if (false == isSupport)                     // 不支持，返回false
        {
            return cpuId;
        }
        cpuId.resize(12);
        memcpy(&cpuId[8], &cpu_info[0], 4);          // 最高位的两个WORD

        __cpuid(cpu_info, 3);                       // 执行cpuid，参数为 eax = 3
        memcpy(&cpuId[0], &cpu_info[2], 8);          // ecx 和 edx为低位的4个WORD

#endif // WIN32
        return cpuId;
    }

#ifdef WIN32
    static bool GetWMIInfo(string system, string data, list<string>& ret, string nameSpace = "root\\cimv2")
    {
        HRESULT hRes;

        hRes = CoInitialize(NULL);
        if (hRes != S_OK && hRes != S_FALSE)
            return false;

        hRes = CoInitializeSecurity(NULL, -1, NULL, NULL, RPC_C_AUTHN_LEVEL_PKT,
            RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE, 0);
        if (hRes != S_OK && hRes != RPC_E_TOO_LATE) {
            CoUninitialize();
            return false;
        }
        IWbemLocator* pIWbemLocator = NULL;
        if (CoCreateInstance(CLSID_WbemAdministrativeLocator, NULL,
            CLSCTX_INPROC_SERVER | CLSCTX_LOCAL_SERVER, IID_IUnknown, (void **)&pIWbemLocator) != S_OK) {
                CoUninitialize();
                return false;
        }

        IWbemServices* pWbemServices = NULL;
        if (pIWbemLocator->ConnectServer(_bstr_t(L"ROOT\\CIMV2"), NULL, NULL, NULL, 0, NULL, NULL,
            &pWbemServices) != S_OK) {
                CoUninitialize();
                return false;
        }
        hRes = CoSetProxyBlanket(pWbemServices, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, NULL,
            RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE);
        if (FAILED(hRes)) {
            pWbemServices->Release();
            pIWbemLocator->Release();
            CoUninitialize();
            return false;
        }

        IEnumWbemClassObject* pEnumerator = NULL;
        string query = string("Select * from ") + system;
        hRes = pWbemServices->ExecQuery(
            _bstr_t(L"WQL"), _bstr_t(sToWString(query).c_str()),
            WBEM_FLAG_RETURN_IMMEDIATELY, NULL, &pEnumerator);
        if (hRes != S_OK) {
            pWbemServices->Release();
            pIWbemLocator->Release();
            CoUninitialize();
            return false;
        }

        IWbemClassObject *pClassObject;
        ULONG uReturn = 0;
        int row = 0;
        bool rt = false;
        while (pEnumerator) {
            hRes = pEnumerator->Next(WBEM_INFINITE, 1, &pClassObject, &uReturn);
            if(0 == uReturn) {
                if (rt)
                    break;
                else {
                    pIWbemLocator->Release();
                    pWbemServices->Release();
                    pEnumerator->Release();
                    CoUninitialize();
                    return false;
                }
            }
            if(hRes != S_OK) {
                pWbemServices->Release();
                pIWbemLocator->Release();
                pEnumerator->Release();
                pClassObject->Release();
                CoUninitialize();
                return false;
            }

            VARIANT vProp;
            VariantInit(&vProp);
            hRes = pClassObject->Get(bstr_t(sToWString(data).c_str()), 0, &vProp, 0, 0);
            if(hRes != S_OK){
                pWbemServices->Release();
                pIWbemLocator->Release();
                pEnumerator->Release();
                pClassObject->Release();
                CoUninitialize();
                return false;
            }
            ret.push_back(_com_util::ConvertBSTRToString(vProp.bstrVal));
            rt = true;
        }
        pIWbemLocator->Release();
        pWbemServices->Release();
        pEnumerator->Release();
        pClassObject->Release();
        CoUninitialize();

        return true;
    }
#endif // WIN32

    string getDiskId()
    {
        string ret;

#ifdef WIN32

        list<string> serialList;
        if (GetWMIInfo("Win32_PhysicalMedia", "SerialNumber", serialList))
        {
            for (list<string>::iterator iter = serialList.begin(); iter != serialList.end(); ++iter)
            {
                ret += *iter;
                ret += "__";
            }
        }

#endif // WIN32

        return ret;
    }

    string getBiosId()
    {
        string ret;

#ifdef WIN32

        list<string> serialList;
        if (GetWMIInfo("Win32_BIOS", "SerialNumber", serialList))
        {
            for (list<string>::iterator iter = serialList.begin(); iter != serialList.end(); ++iter)
            {
                ret += *iter;
                ret += "__";
            }
        }

#endif // WIN32

        return ret;
    }

}
