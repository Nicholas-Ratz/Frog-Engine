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
    Allocator::Allocator() : saveBlock(this), windowBlock(this) {}
    Allocator::~Allocator() {
        free(buffer);
        logInfo("ALLOCATOR: Deallocated %zu bytes", size);
    }

    void Allocator::init(const char* name) {
        char* path = (char*)malloc(512);
        if (!path) logError("ALLOCATOR: Failed to allocate path");
        memset(path, 0, 512);

        const char* local = getenv("LOCALAPPDATA");
        if (!local) logError("ALLOCATOR: Failed to retrieve LOCALAPPDATA");
        id = generateHash(name);
        snprintf(path, 512, "%s\\FrogEngine\\%u\\engine.cache", local, id);

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
        saveSize   = cache.allocatorCache[1];
        windowSize = cache.allocatorCache[2];

        buffer = malloc(size + 256);
        if (!buffer) logError("ALLOCATOR: Failed to allocate buffer");
        memset(buffer, 0, size + 256);

        logInfo("ALLOCATOR: Allocated %zu bytes", size);

        uptr index = (uptr)buffer + 15 & ~15;
        saveBlock.init((ptr)index, saveSize);
        index += saveSize + 32;
        logInfo("  %zu for Save", saveSize);

        index = index + 15 & ~15;
        windowBlock.init((ptr)index, windowSize);
        index += windowSize + 32;
        logInfo("  %zu for Window", windowSize);

        index = index + 15 & ~15;
    }
    void Allocator::abort() {
        free(buffer);
        logWarning("ALLOCATOR: Abort has been called");
    }
    void Allocator::check() {
        saveBlock.check();
        windowBlock.check();
    }

    u32    Allocator::getID() { return id; }
    Block* Allocator::getSaveBlock() { return &saveBlock; }
    Block* Allocator::getWindowBlock() { return &windowBlock; }
}
#endif
