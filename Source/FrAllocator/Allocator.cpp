#include <FrogEngine/Utility.h>

#ifdef FR_OS_WINDOWS
#    include <stdio.h>
#    include <stdlib.h>
#    include <string.h>

#    include <FrogEngine/Allocator.h>
#    include <FrogEngine/Log.h>
#    include <FrogEngine/Save.h>

u32 generateHash(const char* name) {
    u32 hash = 5'381;
    u8  c;

    while ((c = *name++)) hash = ((hash << 5) + hash) + c;

    return hash;
}

namespace FrogEngine {
    Allocator::Allocator() : staticBlock(this) {}
    Allocator::~Allocator() {
        free(buffer);
        logInfo("ALLOCATOR: Deallocated %zu bytes", size);
    }

    void Allocator::init(const char* name) {
        char* path = (char*)malloc(512);
        if (!path) logError("ALLOCATOR: Failed to allocate path");
        memset(path, 0, 512);

#    ifdef FR_OS_WINDOWS
        const char* local = getenv("LOCALAPPDATA");
        if (!local) logError("ALLOCATOR: Failed to retrieve LOCALAPPDATA");
        id = generateHash(name);
        snprintf(path, 512, "%s\\FrogEngine\\%u\\engine.cache", local, id);
#    endif

        logInfo("ALLOCATOR: Generated App ID");
        logInfo("  ID: %u", id);

        FILE*       file = fopen(path, "rb");
        EngineCache cache {};
        EngineCache current {};
        if (!file || fread(&cache, sizeof(EngineCache), 1, file) != 1) cache = {};
        else if (file) fclose(file);
        if (cache.version != current.version) cache = {};
        free(path);

        size       = cache.allocatorCache[0];
        staticSize = cache.allocatorCache[1];

        buffer = malloc(size + 256);
        if (!buffer) logError("ALLOCATOR: Failed to allocate buffer");
        memset(buffer, 0, size + 256);

        logInfo("ALLOCATOR: Allocated %zu bytes", size);

        uptr index = (uptr)buffer + 15 & ~15;
        staticBlock.init((ptr)index, staticSize);
        index += staticSize + 32;
        logInfo("  %zu for Static memory", staticSize);

        index = index + 15 & ~15;
    }
    void Allocator::abort() {
        free(buffer);
        logWarning("ALLOCATOR: Abort has been called");
    }

    u32          Allocator::getID() { return id; }
    ptr*         Allocator::getBuffer() { return &buffer; }
    StaticBlock* Allocator::getStaticBlock() { return &staticBlock; }
}
#endif
