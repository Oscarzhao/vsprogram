
/**
*  操作系统相关信息获取
*
*  @uathor zld
*  @since  2013-06-05
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
#include <iphlpapi.h>
#include <Nb30.h>  
#pragma comment(lib, "Wbemuuid.lib")
#pragma comment(lib, "kernel32.lib")
#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "Netapi32.lib")  
#else
#include <unistd.h>
#include <stdio.h>
#include "net/if.h"
#include <sys/ioctl.h>

#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#endif
#include <boost/algorithm/string.hpp>

#pragma warning (disable : 4800)
/*get mac address on window platform*/
#if defined(_MSC_VER)
#include <snmp.h>
#pragma comment(lib, "snmpapi.lib")
#pragma comment(lib, "Ws2_32.lib")
#endif
namespace utils
{    
#if defined(_MSC_VER)

    // -------------------------------------------------------------------------

    string getLocalIP4Win()
    {
        WORD wVersionRequested = MAKEWORD(2, 2);
        WSADATA wsaData;
        if (WSAStartup(wVersionRequested, &wsaData) != 0)
            return "";

        char szHostName[256] = {0};
        if (gethostname(szHostName, sizeof(szHostName)) == SOCKET_ERROR)
            return "";

        struct hostent* hHost = gethostbyname(szHostName);
        if (hHost == NULL || hHost->h_addr_list[0] == NULL)
            return "";

        memset(szHostName, 0, sizeof(szHostName));
        strcpy(szHostName, inet_ntoa(*(struct in_addr *)hHost->h_addr_list[0]));

        return string(szHostName);
    }

    int getMacByIP(char* szMac, int* pnMacLen, const char* szIp)
    {
        HRESULT hr;
        IPAddr  ipAddr;
        ULONG   pulMac[2];
        ULONG   ulLen;
        char strMacAddr[100] = {0};
        ipAddr = inet_addr(szIp);
        memset(pulMac, 0xff, sizeof(pulMac));
        ulLen = 6;
        hr = SendARP(ipAddr, 0, pulMac, &ulLen);
        if(hr != NO_ERROR)
        {
            //printf("Nothing Find!");
            return 1;
        }
        unsigned char* mac_addr = (unsigned char*)pulMac;
        sprintf(strMacAddr, "%02X-%02X-%02X-%02X-%02X-%02X", mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);

        if (*pnMacLen <= (int)strlen(strMacAddr))
            return 2;
        strcpy(szMac, strMacAddr);
        *pnMacLen = strlen(szMac);

        return 0;
    }

    int getLocalMacAddr(char* szMac, int* pnMacLen, char* szIp/* = NULL */)
    {
        if (szIp != NULL)
            return getMacByIP(szMac, pnMacLen, szIp);

        WORD wVersionRequested = MAKEWORD(2, 2);
        WSADATA wsaData;
        if (WSAStartup(wVersionRequested, &wsaData) != 0)
            return 1;

        char szHostName[256] = {0};
        if (gethostname(szHostName, sizeof(szHostName)) == SOCKET_ERROR)
            return 2;

        struct hostent* hHost = gethostbyname(szHostName);
        if (hHost == NULL || hHost->h_addr_list[0] == NULL)
            return 3;

        memset(szHostName, 0, sizeof(szHostName));
        strcpy(szHostName, inet_ntoa(*(struct in_addr *)hHost->h_addr_list[0]));

        return getMacByIP(szMac, pnMacLen, szHostName);
    }

#else

    std::string get_local_mac()
    {
        int sockfd;
        struct ifreq buffer;

        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd <= 0) 
        { 
            fputs("can not create socket!\n", stderr);
            return "";
        }

        memset(&buffer, 0, sizeof(struct ifreq));
        strncpy(buffer.ifr_name, "eth0", sizeof(buffer.ifr_name)-1);

        char mac_addr[48] = {0};
        if (ioctl(sockfd, SIOCGIFHWADDR, &buffer) < 0)
        {
            fputs("ioctl error!\n", stderr);
        }
        else
        {
            sprintf(mac_addr,"%02x%02x%02x%02x%02x%02x",
                (unsigned char)buffer.ifr_hwaddr.sa_data[0], 
                (unsigned char)buffer.ifr_hwaddr.sa_data[1],
                (unsigned char)buffer.ifr_hwaddr.sa_data[2],
                (unsigned char)buffer.ifr_hwaddr.sa_data[3],
                (unsigned char)buffer.ifr_hwaddr.sa_data[4],
                (unsigned char)buffer.ifr_hwaddr.sa_data[5]);
        }

        close(sockfd);
        return std::string(mac_addr);
    }

    string get_local_ip()
    {
        char hname[128];
        struct hostent *hent;
        int i;

        gethostname(hname, sizeof(hname));

        //hent = gethostent();
        hent = gethostbyname(hname);

        std::string local_ip("");
        char tmp[32] = {0};
        printf("hostname: %s/naddress list: ", hent->h_name);
        for(i = 0; hent->h_addr_list[i]; i++) 
        {
                printf("%s/t", inet_ntoa(*(struct in_addr*)(hent->h_addr_list[i])));
                sprintf(tmp, "%s", inet_ntoa(*(struct in_addr*)(hent->h_addr_list[i])));
                local_ip = std::string(tmp);
        }
        return local_ip;
    }

#endif

    string getLocalIP()
    {
#if defined(_MSC_VER)
        return getLocalIP4Win();
#else
        return get_local_ip();
#endif
    }

    string getLocalMac()
    {
        string mac("");

#if defined(_MSC_VER)
        char szMac[32] = {0};
        int macLen = sizeof(szMac);
        if (getLocalMacAddr(szMac, &macLen, NULL) == 0)
        {
            mac = std::string(szMac);
        }
        
#else
        mac = get_local_mac();
#endif
        if (mac.empty())
        {
            return "010203040506";
        }
        else
        {
            return mac;
        }
    }

    string getMac()
    {
        return getLocalMac();
    }
}
