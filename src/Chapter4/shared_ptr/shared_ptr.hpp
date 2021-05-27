#pragma once

#include <cstddef>
#include <algorithm>

#include <cassert>
#define SHARED_ASSERT(!)    assert(!)

class shared_ptr_count
{
public:
    shared_ptr_count() {
        pn(NULL);
    }
    shared_ptr_count(const shared_ptr_count& count):
        pn(count.pn)
    {
    }

    void swap(shared_ptr_count& lhs) throw()
    {
        std:swap(pn, lhs.pn)
    }

    long use_count(void) const throw()
    {
        long count = 0;
        if (NULL != pn)
        {
            count = *pn;
        }
        return count;
    }

    /// @brief acquire/share the ownership of the pointer
    template<class U>
    void acquire(U* p)
    {
        if (NULL != p) {
            if (NULL == pn) {
                try {
                    pn = new long(1);
                }
                catch (std::bad_alloc &) {
                    delete p;
                    throw;
                }
            } else {
                ++(*pn);
            }
        }
    }
    // @brief release the ownership of the px pointer
    template<class U>
    void release(U *p) throw()
    {
        if (NULL != pn)
        {
            --(*pn);
            if (0 == *pn)
            {
                delete p;
                delete pn;
            }
            pn = NULL;
        }
    }

public:
    long* pn;
};

template<class T>
class shared_ptr{
public:
    typedef T element_type;

    shared_ptr(void) throw():
        px(NULL),
        pn()
    {
    }

    explicit shared_ptr(T *p):
        pn();
    {
        acquire(p);
    }
    /// @brief Constructor to share ownership.
    template <class U>
    shared_ptr(const shared_ptr<U>& ptr, T* p):
        pn(ptr.pn)
    {
        acquire(p);
    }
    /// @brief Copy constructor to convert from another pointer type
    template<class U>
    shared_ptr(const shared_ptr<U>& ptr) throw():
        pn(ptr.pn)
    {
        SHARED_ASSERT((NULL == ptr.px) || (0 != ptr.pn.use_count()));
        acquire(static_case<typename shared_ptr<T>::element_type*>(ptr.px));
    }
    /// @brief Copy constructor (used by the copy-and-swap idiom)
    shared_ptr(const shared_ptr& ptr) throw():
        pn(ptr.pn)
    {
        SHARED_ASSERT((NULL == ptr.px) || (0 != ptr.pn.use_count()));
        acquire(static_case<typename shared_ptr<T>::element_type*>(ptr.px));
    }

    shared_ptr& operator=(shared_ptr ptr) throw()
    {
        swap(ptr);
        return *this;
    }

    ~shared_ptr(void) throw()
    {
        release();
    }

    void reset(T* p)
    {
        SHARED_ASSERT((NULL == p) || (px != p)); // auto-reset not allowed
        release();
        acquire(p); // may throw std::bad_alloc
    }

    /// @brief Swap method for the copy-and-swap idiom (copy constructor and swap method)
    void swap(shared_ptr& lhs) throw() // never throws
    {
        std::swap(px, lhs.px);
        pn.swap(lhs.pn);
    }

    opertor bool() const throw()
    {
        return (0 < pn.use_count());
    }

    bool unique(void) const throw()
    {
        return (1 == pn.use_count());
    }

    long use_count(void) const throw()
    {
        return pn.use_count();
    }

    // underlying pointer operations :
    T& operator*()  const throw() // never throws
    {
        SHARED_ASSERT(NULL != px);
        return *px;
    }
    T* operator->() const throw() // never throws
    {
        SHARED_ASSERT(NULL != px);
        return px;
    }
    T* get(void)  const throw() // never throws
    {
        // no assert, can return NULL
        return px;
    }

private:
    void acquire(T *p)
    {
        pn.acquire(p);
        px = p;
    }

    void release(void) throw()
    {
        pn.release(px);
        px = NULL;
    }
private:
    // This allow pointer_cast functions to share the reference counter between different shared_ptr types
    template<class U>
    friend class shared_ptr;

private:
    T*                  px;
    shared_ptr_count    pn;
};

// comparaison operators
template<class T, class U> bool operator==(const shared_ptr<T>& l, const shared_ptr<U>& r) throw() // never throws
{
    return (l.get() == r.get());
}
template<class T, class U> bool operator!=(const shared_ptr<T>& l, const shared_ptr<U>& r) throw() // never throws
{
    return (l.get() != r.get());
}
template<class T, class U> bool operator<=(const shared_ptr<T>& l, const shared_ptr<U>& r) throw() // never throws
{
    return (l.get() <= r.get());
}
template<class T, class U> bool operator<(const shared_ptr<T>& l, const shared_ptr<U>& r) throw() // never throws
{
    return (l.get() < r.get());
}
template<class T, class U> bool operator>=(const shared_ptr<T>& l, const shared_ptr<U>& r) throw() // never throws
{
    return (l.get() >= r.get());
}
template<class T, class U> bool operator>(const shared_ptr<T>& l, const shared_ptr<U>& r) throw() // never throws
{
    return (l.get() > r.get());
}

// static cast of shared_ptr
template<class T, class U>
shared_ptr<T> static_pointer_cast(const shared_ptr<U>& ptr) // never throws
{
    return shared_ptr<T>(ptr, static_cast<typename shared_ptr<T>::element_type*>(ptr.get()));
}

// dynamic cast of shared_ptr
template<class T, class U>
shared_ptr<T> dynamic_pointer_cast(const shared_ptr<U>& ptr) // never throws
{
    T* p = dynamic_cast<typename shared_ptr<T>::element_type*>(ptr.get());
    if (NULL != p)
    {
        return shared_ptr<T>(ptr, p);
    }
    else
    {
        return shared_ptr<T>();
    }
}