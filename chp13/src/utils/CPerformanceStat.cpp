#include "CPerformanceStat.h"
#include "TimeTool.h"
#include "ttLog.h"
#include <utils/CSingletonT.hpp>

IMPL_SINGLETON_CLASS(utils::CPerformanceStat);

namespace utils
{
    CPerformanceStat::CPerformanceStat()
        : m_bInited(false)
    {

    }

    CPerformanceStat::~CPerformanceStat()
    {

    }

    void CPerformanceStat::init()
    {
        // 暂时不知道有什么可init的，留个坑备用吧
        m_tmStart = time(NULL);
        m_bInited = true;
    }

    void CPerformanceStat::watch(std::string name, double time)
    {
        boost::recursive_mutex::scoped_lock rlock(m_mutex);

        if (!name.empty() && m_bInited)
        {
            FunctionStat& stat = m_statMap[name];
            stat.m_dCallCount += 1.000000;
            stat.m_dTotalTime += time;
            stat.m_dAvgTime = stat.m_dTotalTime / stat.m_dCallCount;
            stat.m_dMaxTime = max(stat.m_dMaxTime, time);
            if (stat.m_dMinTime < 0)
            {
                stat.m_dMinTime = time;
            }
            else
            {
                stat.m_dMinTime = min(stat.m_dMinTime, time);
            }
        }
    }

    void CPerformanceStat::report()
    {
        if (m_bInited)
        {
            TT_PMWLOG(LLV_INFO, "#performance start: %I64d", m_tmStart);
            for (std::map<std::string, FunctionStat>::iterator it = m_statMap.begin(); it != m_statMap.end(); it++)
            {
                TT_PMWLOG(
                    LLV_INFO, 
                    "#%s,%.0f,%.0f,%.0f,%.0f,%.0f", 
                    it->first.c_str(), 
                    it->second.m_dCallCount,
                    it->second.m_dTotalTime,
                    it->second.m_dAvgTime,
                    it->second.m_dMaxTime,
                    it->second.m_dMinTime
                    );
            }
            TT_PMWLOG(LLV_INFO, "#performance end: %I64d", time(NULL));
        }
    }

    //////////////////////////////////////////////////////////////////////////

    CPerformanceWatch::CPerformanceWatch(std::string name)
        : m_strName(name)
        , m_dTmStart(utils::clockMSec())
    {

    }

    CPerformanceWatch::~CPerformanceWatch()
    {
        CPerformanceStat::instance()->watch(m_strName, utils::clockMSec() - m_dTmStart);
    }
}