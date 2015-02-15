/**
 *  
 *
 *  @author panchunfeng
 *  @since  2014-06-24
 */

#ifndef __CPERFORMANCESTAT_14_06_24__
#define __CPERFORMANCESTAT_14_06_24__

#include "CSingletonT.h"
#include "log4cxx/logger.h"
#include <boost/thread/recursive_mutex.hpp>

namespace utils
{
    struct FunctionStat
    {
        double m_dCallCount;    // ���ô���(���б�̬���ò�����������double)
        double m_dTotalTime;    // ��ʱ��
        double m_dAvgTime;      // ƽ��ʱ��
        double m_dMaxTime;      // ���ʱ��
        double m_dMinTime;      // ��Сʱ��

        FunctionStat()
        {
            m_dCallCount = 0.000000;
            m_dTotalTime = 0.000000;
            m_dAvgTime = 0.000000;
            m_dMaxTime = 0.000000;
            m_dMinTime = -1.000000;
        }
    };

    class CPerformanceStat : public CSingletonT<CPerformanceStat>
    {
    public:
        CPerformanceStat();
        virtual ~CPerformanceStat();

        void init();
        void watch(std::string name, double time);
        void report();

    private:
        bool                                m_bInited;
        std::map<std::string, FunctionStat> m_statMap;
        time_t                              m_tmStart;
        boost::recursive_mutex              m_mutex; //ȫ����
    };

    class CPerformanceWatch
    {
    public:
        CPerformanceWatch(std::string name);
        virtual ~CPerformanceWatch();

    private:
        std::string         m_strName;
        double              m_dTmStart;
    };

}

#define PMW utils::CPerformanceWatch watch(__FUNCTION__);

#endif