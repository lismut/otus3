//
// Created by ALavrov on 02.03.2020.
//
#pragma once

#include <memory>

template<class T, class Allocator = std::allocator<T> >
class containerList {
private:
    struct node {
        T elem;
        node* next = nullptr;

        explicit node(T& value) : elem(value) {};
    };
    Allocator allocInstance;
    node* head = nullptr;
    size_t size = 0;
    using NodeAlloc = typename std::allocator_traits<Allocator>::template rebind_alloc<node>;
    NodeAlloc nodeAllocInstance;

    void deleteNode(node* one) {
        nodeAllocInstance.destroy(one);
        nodeAllocInstance.deallocate(one, 1);
    }
    node* makeNode(T& newVal) {
        node* newNode = nodeAllocInstance.allocate(1);
        nodeAllocInstance.construct(newNode, newVal);
        return newNode;
    }
public:
    containerList() = default;
    containerList(containerList&) = delete;
    containerList(containerList&&) = delete;
    ~containerList() noexcept {
        while (head != nullptr) {
            node *tmp = head;
            head = head->next;
            deleteNode(tmp);
        }
    }
    template<bool isConst>
    class myIterator : public std::iterator < std::forward_iterator_tag, T> {
        using valType = typename std::conditional<isConst, const T, T>::type;
        using ptrType = typename std::conditional<isConst, const T*, T*>::type;
        using refType = typename std::conditional<isConst, const T&, T&>::type;
        node* current;
    public:
        explicit myIterator(node* ptr) : current(ptr) {}
        myIterator operator++() {
            myIterator tmp = *this;
            current = current->next;
            return tmp;
        }
        refType operator*() {
            return current->elem;
        }
        ptrType operator->() {
            return &(current->elem);
        }
        bool operator==(const myIterator& rhs) const {
            return current == rhs.current;
        }
        bool operator!=(const myIterator& rhs) const {
            return current != rhs.current;
        }
    };
    using iterator = myIterator<false>;
    using iteratorConst = myIterator<true>;
    [[nodiscard]] bool empty() const {
        return head == nullptr;
    }
    const T& front() const{
        if (empty()) {
            throw std::range_error("empty list");
        }
        return head->elem;
    }
    T& front() {
        const T& constVal = front();
        return const_cast<T>(constVal);
    }
    void pushFront(T& val) {
        node* newFront = makeNode(val);
        newFront->next = head;
        head = newFront;
        ++size;
    }
    void popFront(T& val) {
        val = head->elem;
        node* oldNode = head;
        head = head->next;
        deleteNode(oldNode);
        --size;
    }
    size_t Size() const{
        return size;
    }
    iterator begin() {
        return iterator(head);
    }
    iterator end() {
        return iterator(nullptr);
    }
    iteratorConst cbegin() const{
        return iteratorConst(head);
    }
    iteratorConst cend() const {
        return iteratorConst(nullptr);
    }
};
