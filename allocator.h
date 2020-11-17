#pragma once
#include <iostream>
#include <assert.h>
#include <cstdint>
#include <bitset>
#include <vector>

#define BLOCK_PREFIX "Block(" << this << ")::"

template <typename T, size_t nCapacity>
class my_allocator
{
public:
    using value_type = T;
    // Definitions above are required by travis-ci compiler (gcc 4.8.4)
    using pointer         = value_type*;
    using const_pointer   = const pointer;
    using reference       = value_type&;
    using const_reference = const reference;

    T* allocate(size_t nTotal)
    {
        T* pChunk = nullptr;
        // searchinkg for free chunk in existance blocks
        for (Block& block : blocks) {
            pChunk = block.getChunks(nTotal);
            if (pChunk)
                return pChunk;
        }
        // no free chunks - creating new block
        size_t nNewBlockCapacity = blocks.empty() ? nCapacity : 2 * blocks.back().capacity();
        if (nNewBlockCapacity < nTotal)
            nNewBlockCapacity = nTotal;
        Block newBlock(nNewBlockCapacity);
        pChunk = newBlock.getChunks(nTotal);
        blocks.push_back(std::move(newBlock));
        return pChunk;
    }

    void deallocate(T*, size_t)
    {
        // deallocating is not supported
        // all memory will be freed in destructor
    }


    template<class... Args >
    void construct(T* p, Args&&... args )
    {
        ::new(p) T(std::forward<Args>(args)...);
    }

    void destroy(T* p) { p->~T(); }

    // rebind has been added for building on travis (gcc 4.8.4 requires rebind definition)
    template<class U> struct rebind { using other = my_allocator<U, nCapacity> ; };

private:

    class Block
    {
    public:
        Block(size_t nTotalChunks)
                : pChunks(reinterpret_cast<T*>(malloc(nTotalChunks * sizeof(T)))),
                  nTotalChunks(nTotalChunks), nUsedChunks(0)
        {}
        Block(Block const& other) = delete;
        Block(Block&& other)
        {
            operator=(std::move(other));
        }
        ~Block() {
            free(pChunks);
        }

        Block& operator=(Block && other)
        {
            std::swap(pChunks,      other.pChunks);
            std::swap(nTotalChunks, other.nTotalChunks);
            std::swap(nUsedChunks,  other.nUsedChunks);
            return *this;
        }

        size_t capacity() const { return nTotalChunks; }
        bool hasFreeChunks(size_t nRequiredTotal) const;
        T* getChunks(size_t nRequiredTotal);

    private:
        T* pChunks = nullptr;
        size_t nTotalChunks = 0;
        size_t nUsedChunks = 0;
    };

    std::vector<Block> blocks;
};

template<typename T, size_t nCapacity>
bool my_allocator<T, nCapacity>::Block::hasFreeChunks(size_t nRequiredTotal) const
{
    return pChunks && nUsedChunks + nRequiredTotal < nTotalChunks;
}

template<typename T, size_t nCapacity>
T* my_allocator<T, nCapacity>::Block::getChunks(size_t nRequiredTotal)
{
    if (!hasFreeChunks(nRequiredTotal))
        return nullptr;
    T* pChunk = pChunks + nUsedChunks;
    nUsedChunks += nRequiredTotal;
    return pChunk;
}