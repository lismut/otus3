#pragma once

#include <list>
#include <cstdlib>
#include <new>
#include <limits>

template <typename T, size_t ElemCount=0>
struct customAlloc
{
public:
    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;

    customAlloc () = default;
    template <typename U> explicit constexpr customAlloc (const customAlloc <U, ElemCount>&) noexcept {}

    template<typename U>
    struct rebind
    {
        using other = customAlloc<U,ElemCount>;
    };

    pointer allocate(size_t n)
    {
        pointer p;
        if (!freeElems.empty() && n == 1) {
            p = freeElems.front();
            freeElems.pop_front();
        } else if (currentShift + n < ElemCount && !packs.empty()) {
            p = packs.front() + currentShift;
            currentShift += n;
        } else {
            if (!packs.empty())
                for (;currentShift < ElemCount; ++currentShift) {
                    freeElems.push_back(packs.front() + currentShift);
                }
            p = internalAlloc(n);
            currentShift = n;
        }
        return p;
    }

    [[nodiscard]] pointer allocate() {
        return allocate(1);
    }

    void deallocate(pointer p, std::size_t n=0) {
        if(p && n==0) {
            --currentShift;
            freeElems.push_back(p);
        }
    }

    template<typename U, typename ...Args>
    void construct(U *p, Args &&...args)
    {
        new(p) U(std::forward<Args>(args)...);
    }

    template<typename U>
    void destroy(U *p) {
        p->~U();
    }
private:
    std::list<pointer> packs;
    std::list<pointer> freeElems;
    void* head = nullptr;
    pointer ptr = nullptr;
    size_t currentShift = 0;
    pointer internalAlloc () {
        return internalAlloc (ElemCount);
    }
    pointer internalAlloc (size_t cnt) {
        if (cnt > std::numeric_limits<std::size_t>::max() / sizeof(T) ||
                ElemCount > std::numeric_limits<std::size_t>::max() / sizeof(T))
            throw std::bad_alloc();
        size_t sz = std::max(cnt, (size_t)ElemCount);
        auto p = static_cast<pointer>(std::malloc(sz*sizeof(T)));
        if (!p) {
            throw std::bad_alloc();
        }
        packs.push_front(p);
        return p;
    }
};
