/**
 *  通用函数
 *
 *  @author zhangyi
 *  @since  2012-08-10
 */

#include <utils/commonFunc.h>
#include <utils/mathex.h>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>

#ifdef BOOST_WINDOWS_API
#include <Windows.h>
#endif

namespace utils
{

    string getDir( const string &fileName )
    {
        size_t idx = fileName.rfind('/');
        if (string::npos == idx)
        {
            idx = fileName.rfind('\\');
        }
        if (string::npos != idx)
        {
            return fileName.substr(0, idx);
        }
        return fileName;
    }

    string dirString( const boost::filesystem::path &path )
    {
#if defined BOOST_WINDOWS_API

        return wsToString(path.c_str());

#else

        return path.c_str();

#endif // BOOST_WINDOWS_API
    }

    string wsToString(wstring str)
    {
        if (0 == str.compare(L""))
        {
            return "";
        }

#ifdef WIN32

        int nLen = WideCharToMultiByte(CP_ACP, 0, str.c_str(), -1, NULL, 0, NULL, NULL);
        if (nLen<= 0) return string("");
        char* pszDst = new char[nLen];
        if (NULL == pszDst) return string("");
        WideCharToMultiByte(CP_ACP, 0, str.c_str(), -1, pszDst, nLen, NULL, NULL);
        pszDst[nLen -1] = 0;
        string strTemp(pszDst);
        delete [] pszDst;
        return strTemp;
#else
        size_t nLen = wcstombs(NULL, str.c_str(), 0) + 1;
        if(nLen <= 0) return string("");
        char* pszDst = new char[nLen];
        if (NULL == pszDst) return string("");
        memset(pszDst, 0, nLen);
        nLen = wcstombs(pszDst, str.c_str(), nLen);
        if(nLen <= 0) return string("");
        string strTemp(pszDst);
        delete [] pszDst;
        return strTemp;

#endif // WIN32
    }

    wstring sToWString(string str)
    {
#ifdef WIN32

        int nLen = str.length();
        int nSize = MultiByteToWideChar(CP_ACP, 0, (LPCSTR)str.c_str(), nLen, 0, 0);
        if(nSize <= 0) return NULL;
        wchar_t *pwszDst = new wchar_t[nSize+1];
        if( NULL == pwszDst) return NULL;
        MultiByteToWideChar(CP_ACP, 0,(LPCSTR)str.c_str(), nLen, pwszDst, nSize);
        pwszDst[nSize] = 0;
        if( pwszDst[0] == 0xFEFF) // skip Oxfeff
            for(int i = 0; i < nSize; i ++)
                pwszDst[i] = pwszDst[i+1];
        wstring wchar_tString(pwszDst);
        delete [] pwszDst;
        return wchar_tString;

#else
        int nLen = str.length();
        int nSize = mbstowcs(NULL, str.c_str(), 0) + 1;
        if(nSize <= 0) return NULL;
        wchar_t *pwszDst = new wchar_t[nSize];
        if( NULL == pwszDst) return NULL;
        wmemset(pwszDst, 0, nSize);
        nSize = mbstowcs(pwszDst, str.c_str(), nLen);
        if(nSize <= 0) return NULL;
        if( pwszDst[0] == 0xFEFF) // skip Oxfeff
            for(int i = 0; i < nSize; i ++)
                pwszDst[i] = pwszDst[i+1];
        wstring wchar_tString(pwszDst);
        delete [] pwszDst;
        return wchar_tString;

#endif // WIN32
    }

    string formatDoubleNumber( const long double d, FFormatDoubleNumberFlag flag /*= FDF_NONE*/ )
    {
        long double dPrice = d;
        if (utils::isZero(dPrice))
        {
            dPrice = 0.000;
        }

        int nPrecision = flag & FDF_PRECISION_MASK;
        
        string str;
        if (utils::isLess(abs(dPrice), 1e36))
        {
            string strFormat = "%." + boost::lexical_cast<string>(nPrecision) + "f";
            if (flag & FDF_PERCENTAGE)
            {
                str = (boost::format(strFormat) % (dPrice * 100.0)).str() + "%";
            }
            else
            {
                if (utils::isLess(abs(dPrice), 1e4) || !(flag & FDF_AUTO_TRIM))
                {
                    str = (boost::format(strFormat) % (dPrice)).str();
                }
                else if (utils::isLess(abs(dPrice), 1e8))
                {
                    str = (boost::format(strFormat) % (dPrice / 1e4)).str() + "万";
                }
                else
                {
                    str = (boost::format(strFormat) % (dPrice / 1e8)).str() + "亿";
                }
            }
        }
        else
        {
            str = "";
        }

        if (!str.empty() && flag & FDF_SEGMENTED)
        {
            int idx = str.find('.');
            int i = idx - 3;
            while (i > 0 && str[i - 1] >= '0' && str[i - 1] <= '9')
            {
                str.insert(i, ",");
                i -= 3;
            }
        }

        return str;
    }

}
