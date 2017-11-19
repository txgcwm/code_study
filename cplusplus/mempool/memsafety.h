#ifndef SERVER_MEMPOOLSAFTY_H
#define SERVER_MEMPOOLSAFTY_H

#include <mutex>

#include "mempool.h"

/* 注意：创建和回收整个内存池两个操作不是线程安全的，
 * 这里没有锁保护是因为因为这两个操作一般在主线程中，
 * 使用的时候才会出现多线程同时操作
 */
template<typename Type>
class CMemoryPoolSafty : public CMemoryPool<Type>
{    
public:
    //创建sizeof(Type) * nNumOfElements大小的空间
    bool Create(uint nNumOfElements)
    {
        return CMemoryPool<Type>::Create(nNumOfElements);
    }
    
    //释放内存池空间
    void Destroy()
    {
        CMemoryPool<Type>::Destroy();
    }
    
    //从内存池中获取一个对象空间
    Type* Alloc()
    {
        std::lock_guard<std::mutex> guard(m_utex);
        Type* pType;
        pType = CMemoryPool<Type>::Alloc();

        return pType;
    }
    
    //内存池回收一个对象空间
    void Free(Type* pElement)
    {
        std::lock_guard<std::mutex> guard(m_utex);
        CMemoryPool<Type>::Free(pElement);
    }

private:
    std::mutex  m_utex;
};
#endif //SERVER_MEMPOOLSAFTY_H
