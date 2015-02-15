/**
 *  实现最大最小堆
 *
 *  @author zhangyi
 *  @since  2012-11-16
 */

#include <utils/CHeapT.h>

namespace utils
{

    template <typename TData, typename TCompareTraits>
    CHeapT<TData, TCompareTraits>::CHeapT()
        : m_nSize(0)
    {

    }

    template <typename TData, typename TCompareTraits>
    CHeapT<TData, TCompareTraits>::~CHeapT()
    {

    }

    template <typename TData, typename TCompareTraits>
    void CHeapT<TData, TCompareTraits>::setTraits( const TCompareTraits & traits )
    {
        m_traits = traits;
    }

    template <typename TData, typename TCompareTraits>
    bool CHeapT<TData, TCompareTraits>::empty() const
    {
        return m_nSize == 0;
    }

    template <typename TData, typename TCompareTraits>
    void CHeapT<TData, TCompareTraits>::resize( size_t nSize )
    {
        m_data.resize(nSize);
        m_nSize = nSize;
    }

    template <typename TData, typename TCompareTraits>
    void CHeapT<TData, TCompareTraits>::clear()
    {
        m_data.clear();
        m_indexMap.clear();
        m_nSize = 0;
    }

    template <typename TData, typename TCompareTraits>
    void CHeapT<TData, TCompareTraits>::reset()
    {
        m_indexMap.clear();
        m_nSize = 0;
    }

    template <typename TData, typename TCompareTraits>
    void CHeapT<TData, TCompareTraits>::insert( const TData & data )
    {
        // 确保存储空间
        if (m_nSize == m_data.size())
        {
            m_data.push_back(TData());
        }
        ++m_nSize;
        
        // 设置数据
        m_data[m_nSize - 1] = data;
        m_indexMap[m_traits.key(data)] = m_nSize - 1;

        // 向上追溯
        traceUpData(m_nSize - 1);
    }

    template <typename TData, typename TCompareTraits>
    bool CHeapT<TData, TCompareTraits>::update( const TData & data )
    {
        int key = m_traits.key(data);
        map<int, size_t>::iterator iter = m_indexMap.find(key);
        if (m_indexMap.end() == iter)
        {
            return false;
        }

        size_t idx = iter->second;
        if (idx >= m_nSize)
        {
            return false;
        }

        m_data[idx] = data;

        traceUpData(idx);
        drillDownData(idx);

        return true;
    }

    template <typename TData, typename TCompareTraits>
    bool CHeapT<TData, TCompareTraits>::remove( const TData & data )
    {
        int key = m_traits.key(data);
        map<int, size_t>::iterator iter = m_indexMap.find(key);
        if (m_indexMap.end() == iter)
        {
            return false;
        }

        size_t idx = iter->second;
        if (idx >= m_nSize)
        {
            return false;
        }

        m_indexMap.erase(iter);
        swap(m_data[idx], m_data[m_nSize - 1]);
        --m_nSize;
        
        traceUpData(idx);
        drillDownData(idx);

        return true;
    }

    template <typename TData, typename TCompareTraits>
    TData CHeapT<TData, TCompareTraits>::top( bool * pbOK/* = NULL*/ ) const
    {
        if (!m_data.empty())
        {
            if (NULL != pbOK)
            {
                *pbOK = true;
            }
            return m_data[0];
        }
        if (NULL != pbOK)
        {
            *pbOK = false;
        }
        return TData();
    }

    template <typename TData, typename TCompareTraits>
    void CHeapT<TData, TCompareTraits>::traceUpData( size_t idx )
    {
        if (idx > 0)
        {
            size_t parentIdx = (idx - 1) / 2;
            int nCompare = m_traits.compareValue(m_data[idx], m_data[parentIdx]);
            if (nCompare < 0)
            {
                mySwap(idx, parentIdx);
                traceUpData(parentIdx);
            }
        }
    }

    template <typename TData, typename TCompareTraits>
    void CHeapT<TData, TCompareTraits>::drillDownData( size_t idx )
    {
        size_t idxLeftChild = idx * 2 + 1;
        size_t idxRightChild = idxLeftChild + 1;

        bool bLeftChildSmaller = (idxLeftChild < m_nSize) && (m_traits.compareValue(m_data[idx], m_data[idxLeftChild]) > 0);
        bool bRightChildSmaller = (idxRightChild < m_nSize) && (m_traits.compareValue(m_data[idx], m_data[idxRightChild]) > 0);
        if (bLeftChildSmaller)
        {
            if (bRightChildSmaller)
            {
                if (m_traits.compareValue(m_data[idxLeftChild], m_data[idxRightChild]) < 0)
                {
                    mySwap(idx, idxLeftChild);
                    drillDownData(idxLeftChild);
                }
                else
                {
                    mySwap(idx, idxRightChild);
                    drillDownData(idxRightChild);
                }
            }
            else
            {
                mySwap(idx, idxLeftChild);
                drillDownData(idxLeftChild);
            }
        }
        else
        {
            if (bRightChildSmaller)
            {
                mySwap(idx, idxRightChild);
                drillDownData(idxRightChild);
            }
        }
    }

    template <typename TData, typename TCompareTraits>
    void CHeapT<TData, TCompareTraits>::mySwap( size_t idx1, size_t idx2 )
    {
        swap(m_data[idx1], m_data[idx2]);
        m_indexMap[m_traits.key(m_data[idx1])] = idx1;
        m_indexMap[m_traits.key(m_data[idx2])] = idx2;
    }

}
