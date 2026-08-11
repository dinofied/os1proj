#ifndef PROJECT_BASE_MEMORYALLOCATOR_HPP
#define PROJECT_BASE_MEMORYALLOCATOR_HPP

#include "../lib/hw.h"
#include "../lib/console.h"

class MemoryAllocator {
public:

    static MemoryAllocator& getInstance() {
        static MemoryAllocator instance;
        return instance;
    }
    void* mem_alloc(size_t size);
    int mem_free(void* ptr);

private:

    struct MemoryChunk {
        MemoryChunk* next = nullptr;
        MemoryChunk* prev = nullptr;
        size_t sizeInBlocks;
    };
    MemoryChunk* freeMemChunks = nullptr;
    int tryToJoinChunks(MemoryChunk* chunk);

    MemoryAllocator();
    //static MemoryAllocator instance; // not sure if should be removed

    void printFreeChunks();

};


#endif //PROJECT_BASE_MEMORYALLOCATOR_HPP
