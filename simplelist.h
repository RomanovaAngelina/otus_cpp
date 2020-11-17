#pragma once

#include <stdlib.h>
#include <memory>
#include <memory>

template<typename T, typename Allocator = std::allocator<T>>
class simple_list
{
private:
    struct Node {
        template<typename... Args>
        Node(Args&&... args)
                : element(std::forward<Args>(args)...)
        {}

        T     element;
        Node* pNext;
    };

public:

    class iterator
    {
    public:
        iterator(Node* pElement) : pCurrent(pElement) {}

        void operator++() {
            if (pCurrent)
                pCurrent = pCurrent->pNext;
        }
        T& operator*() { return pCurrent->element; }
        T& operator->() { return pCurrent->element; }
        bool operator==(const iterator& other) const { return pCurrent == other.pCurrent; }
        bool operator!=(const iterator& other) const { return pCurrent != other.pCurrent; }

    private:
        Node* pCurrent;
    };

public:

    void push_back(const T& element)
    {
        Node* pNode = allocator.allocate(1);
        std::allocator_traits<NodeAllocator>::construct(allocator, pNode, element);
        insertTail(pNode);
    }

    void push_back(T&& element)
    {
        Node* pNode = allocator.allocate(1);
        std::allocator_traits<NodeAllocator>::construct(allocator, pNode, std::move(element));
        insertTail(pNode);
    }

    template<typename... Args>
    void emplace_back(Args&&... args)
    {
        Node* pNode = allocator.allocate(1);
        std::allocator_traits<NodeAllocator>::construct(allocator, pNode,
                                                        std::forward<Args>(args)...);
        insertTail(pNode);
    }

    iterator begin() { return iterator(pHead); }
    iterator end()   { return iterator(nullptr); }

private:
    void insertTail(Node* pNode)
    {
        if (pTail) {
            pTail->pNext = pNode;
        } else {
            pHead = pNode;
        }
        pTail = pNode;
    }

private:
    // Programming language shouldn't look like that...
    using NodeAllocator = typename std::allocator_traits<Allocator>::template rebind_alloc<Node>;

    Node* pHead = nullptr;
    Node* pTail = nullptr;
    NodeAllocator allocator;
};