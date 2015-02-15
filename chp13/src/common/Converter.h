/**
* 周期转换之后的计算
* 
* @author   xujun
* @date     2011-12-29
* @time     10:47
* @version  1.0
*/

#ifndef CONVERTER_H_
#define CONVERTER_H_

namespace converter
{
    // 拆分周期类型 （hub推送的都是每秒的数据, 根据需要产生其他多周期的数据）
    enum PeriodSpinType{
        PERIOD_SPIN_NON     = 0,        // 无需处理
        PERIOD_SPIN_ADD     = 1,        // 累加值
        PERIOD_SPIN_MAX     = 2,        // 最大值
        PERIOD_SPIN_MIN     = 3,        // 最小值
        PERIOD_SPIN_MIRROR  = 4,        // 镜像值
        PERIOD_SPIN_FIRST   = 5,        // 第一个值
    };

    // 根据周期属性取得周期值
    template <typename T>
    T getPeriodData(const r_int32 meta, const r_int32 period, const r_int32 spinType, const r_int64 lastTime, const T &lastValue, const r_int64 nowTime, const T &nowValue)
    {
        switch (spinType)
        {
        case PERIOD_SPIN_NON:
            {
                return nowValue;
            }
            break;
        case PERIOD_SPIN_ADD:
            {
                return lastValue + nowValue;
            }
            break;
        case PERIOD_SPIN_MAX:
            {
                return max(lastValue, nowValue);
            }
        break;
        case PERIOD_SPIN_MIN:
            {
                return min(lastValue, nowValue);
            }
            break;
        case PERIOD_SPIN_MIRROR:
            {
                return nowValue;
            }
            break;
        case PERIOD_SPIN_FIRST:
            {
                if (period < 86400000)
                {
                    return (lastTime % period == nowTime % period) ? lastTime : nowValue;
                } else {
                    return ( (lastTime - 8 * 3600) % period == (nowTime - 8 * 3600) % period) ? lastTime : nowValue;
                }
            }
            break;
        }
        return nowValue;
    }

}

#endif //CONVERTER_H_