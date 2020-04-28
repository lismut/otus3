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
        return internalAlloc(n);
    }

    [[nodiscard]] pointer allocate() {
        if (ElemCount > std::numeric_limits<std::size_t>::max() / sizeof(T))
            throw std::bad_alloc();
        pointer p;
        if (!freeElems.empty()) {
            p = freeElems.front();
            freeElems.pop_front();
        } else if (currentShift < ElemCount && !packs.empty()) {
            p = packs.front() + currentShift*sizeof(T);
        } else if ((p = internalAlloc())) {
            currentShift = 1;
        } else {
            throw std::bad_alloc();
        }
        return p;
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
        return internalAlloc (1);
    }
    pointer internalAlloc (size_t cnt) {
        auto p = static_cast<pointer>(std::malloc(cnt*sizeof(T)));
        if (!p) {
            throw std::bad_alloc();
        }
        packs.push_front(p);
        return p;
    }
};
