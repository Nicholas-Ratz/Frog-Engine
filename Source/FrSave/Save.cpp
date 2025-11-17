#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#include <FrogEngine/Allocator.h>
#include <FrogEngine/Log.h>
#include <FrogEngine/Save.h>
#include <FrogEngine/Utility.h>

#ifdef FR_OS_WINDOWS
#    include <direct.h>
#    define mkdir(path, mode) _mkdir(path)
#else
#    include <sys/stat.h>
#    define mkdir(path, mode) mkdir(path, mode)
#endif

namespace FrogEngine {
    Save::Save(Allocator* allocate) {
        block = allocate->getStaticBlock();
        id    = allocate->getID();

        configPath = block->alloc(512);
        filePath   = block->alloc(512);
    }
    Save::~Save() {}

    void Save::init() {
        if (configPath[0] != '\0') {
            logWarning(
                "%sSAVE%s: Called init() after initialization",
                FR_LOG_FORMAT_BRIGHT_GREEN,
                FR_LOG_FORMAT_RESET);
            return;
        }

        const char* base_path { nullptr };

#ifdef FR_OS_WINDOWS
        base_path = getenv("LOCALAPPDATA");
        if (!base_path)
            logError(
                "%sSAVE%s: LOCALAPPDATA not found",
                FR_LOG_FORMAT_BRIGHT_GREEN,
                FR_LOG_FORMAT_RESET);
#else
        base_path = getenv("XDG_CONFIG_HOME");
        if (!basePath) {
            base_path = getenv("HOME");
            if (!base_path)
                logError(
                    "%sSAVE%s: Neither XDG_CONFIG_HOME nor HOME is set",
                    FR_LOG_FORMAT_BRIGHT_GREEN,
                    FR_LOG_FORMAT_RESET);
        }
#endif

        if (snprintf(configPath, 512, "%s/FrogEngine", base_path) >= 512)
            logError(
                "%sSAVE%s: FrogEngine save path too long",
                FR_LOG_FORMAT_BRIGHT_GREEN,
                FR_LOG_FORMAT_RESET);
        if (mkdir(configPath, 0755) != 0 && errno != EEXIST)
            logError(
                "%sSAVE%s: Failed to create directory at %s (errno: %d)",
                FR_LOG_FORMAT_BRIGHT_GREEN,
                FR_LOG_FORMAT_RESET,
                configPath.get(),
                errno);

        if (snprintf(configPath, 512, "%s/FrogEngine/%u", base_path, id) >= 512)
            logError(
                "%sSAVE%s: Game save path too long",
                FR_LOG_FORMAT_BRIGHT_GREEN,
                FR_LOG_FORMAT_RESET);
        if (mkdir(configPath, 0755) != 0 && errno != EEXIST)
            logError(
                "%sSAVE%s: Failed to create directory at %s (errno: %d)",
                FR_LOG_FORMAT_BRIGHT_GREEN,
                FR_LOG_FORMAT_RESET,
                configPath.get(),
                errno);
        if (errno != EEXIST)
            logInfo(
                "%sSAVE%s: Created path at %s",
                FR_LOG_FORMAT_BRIGHT_GREEN,
                FR_LOG_FORMAT_RESET,
                configPath.get());

        if (snprintf(filePath, 512, "%s/engine.cache", (char*)configPath.get()) >= 512)
            logError(
                "%sSAVE%s: Engine cache save path too long",
                FR_LOG_FORMAT_BRIGHT_GREEN,
                FR_LOG_FORMAT_RESET);

        FILE* file = fopen(filePath, "rb");
        if (!file) {
            if (errno != ENOENT)
                logError(
                    "%sSAVE%s: Failed to read %s. Code %i\n",
                    FR_LOG_FORMAT_BRIGHT_GREEN,
                    FR_LOG_FORMAT_RESET,
                    filePath.get(),
                    errno);

            engineCache = {};
            file        = fopen(filePath, "wb");
            if (!file || fwrite(&engineCache, sizeof(EngineCache), 1, file) != 1)
                logError(
                    "%sSAVE%s: Failed to write %s. Code %i",
                    FR_LOG_FORMAT_BRIGHT_GREEN,
                    FR_LOG_FORMAT_RESET,
                    filePath.get(),
                    errno);
            fclose(file);
            logInfo(
                "%sSAVE%s: Created file %s",
                FR_LOG_FORMAT_BRIGHT_GREEN,
                FR_LOG_FORMAT_RESET,
                filePath.get());
            return;
        }

        if (fread(&engineCache, sizeof(EngineCache), 1, file) != 1) {
            fclose(file);
            logWarning(
                "%sSAVE%s: Failed to read %s. Code %i",
                FR_LOG_FORMAT_BRIGHT_GREEN,
                FR_LOG_FORMAT_RESET,
                filePath.get(),
                errno);
            logWarning("  Rewriting file");
            remove(filePath);
            engineCache = {};
            file        = fopen(filePath, "wb");
            if (!file || fwrite(&engineCache, sizeof(EngineCache), 1, file) != 1)
                logError(
                    "%sSAVE%s: Failed to write %s. Code %i",
                    FR_LOG_FORMAT_BRIGHT_GREEN,
                    FR_LOG_FORMAT_RESET,
                    filePath.get(),
                    errno);
            fclose(file);
        } else {
            fclose(file);
            logInfo(
                "%sSAVE%s: Opened file %s",
                FR_LOG_FORMAT_BRIGHT_GREEN,
                FR_LOG_FORMAT_RESET,
                filePath.get());
            EngineCache current = {};
            if (engineCache.version != current.version) {
                logWarning(
                    "%sSAVE%s: Version mismatch. Found %u, expected %u",
                    FR_LOG_FORMAT_BRIGHT_GREEN,
                    FR_LOG_FORMAT_RESET,
                    engineCache.version,
                    current.version);
                logWarning("  Rewriting file");
                engineCache = {};
                file        = fopen(filePath, "wb");
                if (!file || fwrite(&engineCache, sizeof(EngineCache), 1, file) != 1)
                    logError(
                        "%sSAVE%s: Failed to write %s. Code %i",
                        FR_LOG_FORMAT_BRIGHT_GREEN,
                        FR_LOG_FORMAT_RESET,
                        filePath.get(),
                        errno);
                fclose(file);
            }
        }
        logInfo(
            "%sSAVE%s: Closed file %s",
            FR_LOG_FORMAT_BRIGHT_GREEN,
            FR_LOG_FORMAT_RESET,
            filePath.get());
    }
}
