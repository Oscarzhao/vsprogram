/**
 *  实现最大最小堆
 *
 *  @author zhangyi
 *  @since  2012-11-16
 */

#ifndef SERVER3_BASE_UTILS_HEAP_H
#define SERVER3_BASE_UTILS_HEAP_H

#include <vector>
#include <map>
using namespace std;

namespace utils
{
    template <typename TData, typename TCompareTraits>
    class CHeapT
    {
    public:
        CHeapT();
        virtual ~CHeapT();

        void setTraits(const TCompareTraits & traits);
        bool empty() const;

        void resize(size_t nSize);
        void clear();
        void reset();

        void insert(const TData & data);
        bool update(const TData & data);
        bool remove(const TData & data);

        TData top(bool * pbOK = NULL) const;

    protected:

        vector<TData>       m_data;
        map<int, size_t>    m_indexMap;
        size_t              m_nSize;
        TCompareTraits      m_traits;

        void traceUpData(size_t idx);
        void drillDownData(size_t idx);

        void mySwap(size_t idx1, size_t idx2);
    };

}

#endif
