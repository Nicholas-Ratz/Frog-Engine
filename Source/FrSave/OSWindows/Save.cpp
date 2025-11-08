#include <FrogEngine/Utility.h>

#ifdef FR_OS_WINDOWS
#    include <direct.h>
#    include <errno.h>
#    include <stdio.h>
#    include <stdlib.h>

#    include <FrogEngine/Allocator.h>
#    include <FrogEngine/Log.h>
#    include <FrogEngine/Save.h>

namespace FrogEngine {
    Save::Save(Allocator* allocate) {
        block = allocate->getSaveBlock();
        id    = allocate->getID();

        configPath = block->alloc(512);
        filePath   = block->alloc(512);
    }
    Save::~Save() {}

    void Save::init() {
        if (configPath[0] != 0) {
            logWarning("SAVE: Called init() after initialization");
            return;
        }
        const char* local_path = getenv("LOCALAPPDATA");
        if (!local_path) logError("SAVE: LOCALAPPDATA not found");

        if (snprintf(configPath, 512, "%s\\FrogEngine", local_path) >= 512)
            logError("SAVE: FrogEngine save path too long");
        if (mkdir(configPath) != 0 && errno != EEXIST)
            logError("SAVE: Failed to create directory at %s", configPath.get());

        if (snprintf(configPath, 512, "%s\\FrogEngine\\%u", local_path, id) >= 512)
            logError("SAVE: Game save path too long");
        if (mkdir(configPath) != 0 && errno != EEXIST)
            logError("SAVE: Failed to create directory at %s", configPath.get());
        if (errno != EEXIST) logInfo("SAVE: Created path at %s", configPath.get());

        snprintf(filePath, 512, "%s\\engine.cache", (char*)configPath.get());

        FILE* file = fopen(filePath, "rb");
        if (!file) {
            if (errno != ENOENT)
                logError("SAVE: Failed to read %s. Code %i\n", filePath.get(), errno);

            engineCache = {};
            file        = fopen(filePath, "wb");
            if (!file || fwrite(&engineCache, sizeof(EngineCache), 1, file) != 1)
                logError("SAVE: Failed to write %s. Code %i", filePath.get(), errno);
            fclose(file);
            logInfo("SAVE: Created file %s", filePath.get());
            return;
        }

        if (fread(&engineCache, sizeof(EngineCache), 1, file) != 1) {
            fclose(file);
            logWarning("SAVE: Failed to read %s. Code %i", filePath.get(), errno);
            logWarning("  Rewriting file");
            remove(filePath);
            engineCache = {};
            file        = fopen(filePath, "wb");
            if (!file || fwrite(&engineCache, sizeof(EngineCache), 1, file) != 1)
                logError("SAVE: Failed to write %s. Code %i", filePath.get(), errno);
            fclose(file);
        } else {
            fclose(file);
            logInfo("SAVE: Opened file %s", filePath.get());
            EngineCache current = {};
            if (engineCache.version != current.version) {
                logWarning(
                    "SAVE: Version mismatch. Found %u, expected %u",
                    engineCache.version,
                    current.version);
                logWarning("  Rewriting file");
                engineCache = {};
                file        = fopen(filePath, "wb");
                if (!file || fwrite(&engineCache, sizeof(EngineCache), 1, file) != 1)
                    logError("SAVE: Failed to write %s. Code %i", filePath.get(), errno);
                fclose(file);
            }
        }
        logInfo("SAVE: Closed file %s", filePath.get());
    }
}

#endif
