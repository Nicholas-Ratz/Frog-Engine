#include <stdio.h>
#include <stdlib.h>

#include <FrogEngine/Allocator.h>
#include <FrogEngine/Log.h>
#include <FrogEngine/Save.h>
#include <FrogEngine/Utility.h>

u32 generateHash(const char* name) {
    u32 hash = 5'381;
    u8  c;

    while ((c = *name++)) hash = ((hash << 5) + hash) + c;

    return hash;
}

namespace FrogEngine {
    Allocator::Allocator() : staticBlock(this), dynamicBlock(this) {}
    Allocator::~Allocator() {
        free(buffer);
        logInfo(
            "%sALLOCATOR%s: Deallocated %zu bytes",
            FR_LOG_FORMAT_YELLOW,
            FR_LOG_FORMAT_RESET,
            size);
    }

    void Allocator::init(const char* name) {
        char* path = (char*)malloc(512);
        if (!path)
            logError(
                "%sALLOCATOR%s: Failed to allocate path",
                FR_LOG_FORMAT_YELLOW,
                FR_LOG_FORMAT_RESET);
        memset(path, 0, 512);

        const char* base_path { nullptr };
#ifdef FR_OS_WINDOWS
        base_path = getenv("LOCALAPPDATA");
#else
        base_path = getenv("XDG_CONFIG_HOME");
        if (!basePath) {
            base_path = getenv("HOME");
            if (!base_path)
                logError(
                    "%sSAVE%s: Neither XDG_CONFIG_HOME nor HOME is set",
                    FR_LOG_FORMAT_CYAN,
                    FR_LOG_FORMAT_RESET);
        }
#endif
        id = generateHash(name);
        snprintf(path, 512, "%s\\FrogEngine\\%u\\engine.cache", base_path, id);

        logInfo("%sALLOCATOR%s: Generated App ID", FR_LOG_FORMAT_YELLOW, FR_LOG_FORMAT_RESET);
        logInfo("  ID: %u", id);

        FILE*       file = fopen(path, "rb");
        EngineCache cache {};
        EngineCache current {};
        if (!file || fread(&cache, sizeof(EngineCache), 1, file) != 1) cache = {};
        else if (file) fclose(file);
        if (cache.version != current.version) cache = {};
        free(path);

        dynamicSize = cache.allocatorCache;
        size        = staticSize + dynamicSize;

        buffer = malloc(size + 256);
        if (!buffer)
            logError(
                "%sALLOCATOR%s: Failed to allocate buffer",
                FR_LOG_FORMAT_YELLOW,
                FR_LOG_FORMAT_RESET);
        memset(buffer, 0, size + 256);

        logInfo(
            "%sALLOCATOR%s: Allocated %zu bytes", FR_LOG_FORMAT_YELLOW, FR_LOG_FORMAT_RESET, size);

        uptr index = (uptr)buffer + 15 & ~15;
        staticBlock.init((ptr)index, staticSize);
        index += staticSize + 32;
        logInfo("  %zu for static memory", staticSize);

        index = index + 15 & ~15;
        dynamicBlock.init((ptr)index, dynamicSize);
        logInfo("  %zu for dynamic memory", dynamicSize);
    }
    void Allocator::resize(usize _size) {
        size   = _size;
        buffer = realloc(buffer, size + 256);
        if (!buffer)
            logError(
                "%sALLOCATOR%s: Failed to reallocate buffer",
                FR_LOG_FORMAT_YELLOW,
                FR_LOG_FORMAT_RESET);
        logInfo(
            "%sALLOCATOR%s: Reallocated buffer to %zu bytes",
            FR_LOG_FORMAT_YELLOW,
            FR_LOG_FORMAT_RESET,
            size);

        uptr index = (uptr)buffer + 15 & ~15;
        staticBlock.setBuffer(buffer);
        index += staticSize + 32;
        logInfo("  %zu for static memory", staticSize);

        index = index + 15 & ~15;
        dynamicBlock.init((ptr)index, dynamicSize);
        logInfo("  %zu for dynamic memory", dynamicSize);
    }
    void Allocator::abort() {
        free(buffer);
        logWarning(
            "%sALLOCATOR%s: Abort has been called", FR_LOG_FORMAT_YELLOW, FR_LOG_FORMAT_RESET);
    }

    u32          Allocator::getID() { return id; }
    ptr*         Allocator::getBuffer() { return &buffer; }
    usize        Allocator::getSize() { return size; }
    StaticBlock* Allocator::getStaticBlock() { return &staticBlock; }
}
