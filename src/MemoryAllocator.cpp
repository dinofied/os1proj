#include "../h/MemoryAllocator.hpp"
#include "../h/ajmoPrintati.hpp"

//MemoryAllocator MemoryAllocator::instance;

void* MemoryAllocator::mem_alloc(size_t size) {
    if (size <= 0) {
        return nullptr;
    }

    MemoryChunk* seeker = freeMemChunks;
    while (seeker && seeker->sizeInBlocks < size + 1) seeker = seeker->next;

    if (!seeker) return nullptr;

    void* allocatedAddress = (void*)((uint64)seeker + MEM_BLOCK_SIZE);

    MemoryChunk* newChunk = seeker;
    newChunk = (MemoryChunk*)((uint64)newChunk + (size + 1) * MEM_BLOCK_SIZE); // mozda promeniti uint u char* ?! konsultuj silju
    newChunk->sizeInBlocks = seeker->sizeInBlocks - size - 1; // ukoliko je ostatak = 1; alociraj i njega
    newChunk->next = seeker->next;
    newChunk->prev = seeker->prev;
    if (newChunk->prev) newChunk->prev->next = newChunk;
    else freeMemChunks = newChunk;
    if (newChunk->next) newChunk->next->prev = newChunk;

    seeker->sizeInBlocks = size + 1;

    // __putc('A');
    // __putc(':');
    // __putc('\n');
    // printFreeChunks(); // debugging
    // __putc('\n');
    return allocatedAddress;
};

int MemoryAllocator::mem_free(void* ptr) {
    MemoryChunk* newChunk = (MemoryChunk*)((uint64)ptr - MEM_BLOCK_SIZE);

    MemoryChunk* seeker = 0;
    if (freeMemChunks && (uint64)ptr > (uint64)freeMemChunks) {
        for (seeker = freeMemChunks; seeker->next && (uint64)seeker->next < (uint64)newChunk; seeker = seeker->next);
    }

    newChunk->prev = seeker;
    if (seeker) newChunk->next = seeker->next;
    else newChunk->next = freeMemChunks;
    if (newChunk->next) newChunk->next->prev = newChunk;
    if (seeker) {
        seeker->next = newChunk;
    }
    else
        freeMemChunks = newChunk;

    tryToJoinChunks(newChunk);
    tryToJoinChunks(seeker);

    // __putc('F');
    // __putc(':');
    // __putc('\n');
    // printFreeChunks(); // debugging
    // __putc('\n');
    return 0;
};

int MemoryAllocator::tryToJoinChunks(MemoryChunk* chunk) {
    if (!chunk) return -1;
    if (chunk->next && ((uint64)chunk + (chunk->sizeInBlocks * MEM_BLOCK_SIZE) == (uint64)chunk->next)) {
        chunk->sizeInBlocks += chunk->next->sizeInBlocks;
        chunk->next = chunk->next->next;
        if (chunk->next) {
            chunk->next->prev = chunk;
        }
        return 0;
    }
    return -1;
};

void MemoryAllocator::printFreeChunks() {
    MemoryChunk* surfer = freeMemChunks;
    while (surfer) {
        printajBolan((uint64)surfer);
        surfer = surfer->next;
    }
};

MemoryAllocator::MemoryAllocator() {
    //__putc('b');
    size_t size = (((uint64)HEAP_END_ADDR  - 1) - (uint64)HEAP_START_ADDR);
    size = size / MEM_BLOCK_SIZE;
    freeMemChunks = (MemoryChunk*)HEAP_START_ADDR;
    freeMemChunks->sizeInBlocks = size;
    freeMemChunks->next = nullptr;
    freeMemChunks->prev = nullptr;
    // __putc('c');
    //
    // printajBolan((uint64)size);
    // __putc('\n');
};