#include <FrogEngine/Utility.h>

#ifdef FR_OS_WINDOWS
#    include <direct.h>
#    include <stdio.h>
#    include <stdlib.h>
#    include <string.h>

#    include <FrogEngine/Log.h>
#    include <FrogEngine/Save.h>

u32 generateHash(const char* name) {
    u32 hash = 5'381;
    u8  c;

    while ((c = *name++)) hash = ((hash << 5) + hash) + c;

    return hash;
}

namespace FrogEngine {
    Save::Save() {
        configPath = (char*)malloc(512);
        if (!configPath) logError("SAVE: Failed to allocate configPath");
        memset(configPath, 0, 512);

        filePath = (char*)malloc(512);
        if (!filePath) logError("SAVE: Failed to allocate filePath");
        memset(filePath, 0, 512);
    }
    Save::~Save() {
        free(configPath);
        free(filePath);
    }

    void Save::init(const char* name) {
        if (configPath[0] != 0) {
            logWarning("SAVE: Called init() after initialization");
            return;
        }
        id = generateHash(name);
        logInfo("SAVE: Generated App ID", id);
        logInfo("  ID: %u", id);

        const char* local_path = getenv("LOCALAPPDATA");
        if (!local_path) logError("SAVE: LOCALAPPDATA not set");

        i32 n = snprintf(configPath, 512, "%s\\FrogEngine", local_path);
        if (n >= 512) logError("SAVE: FrogEngine save path too long");
        if (mkdir(configPath) != 0 && errno != EEXIST)
            logError("SAVE: Failed to create directory at %s", configPath);

        n += snprintf(configPath + n, 512 - n, "\\%u", id);
        if (n >= 512) logError("SAVE: Game save path too long");
        if (mkdir(configPath) != 0 && errno != EEXIST)
            logError("SAVE: Failed to create directory at %s", configPath);

        logInfo("SAVE: Created path at %s", configPath);

        snprintf(filePath, 512, "%s\\engine.sav", configPath);

        FILE* file = fopen(filePath, "rb");
        if (!file) {
            if (errno != ENOENT) logError("SAVE: Failed to read %s. Code %i\n", filePath, errno);

            engineData = {};
            file       = fopen(filePath, "wb");
            if (!file || fwrite(&engineData, sizeof(EngineData), 1, file) != 1)
                logError("SAVE: Failed to write %s. Code %i", filePath, errno);
            fclose(file);
            return;
        }

        if (fread(&engineData, sizeof(engineData), 1, file) != 1) {
            fclose(file);
            logWarning("SAVE: Failed to read %s. Code %i", filePath, errno);
            logWarning("  Rewriting file");
            remove(filePath);
            engineData = {};
            file       = fopen(filePath, "wb");
            if (!file || fwrite(&engineData, sizeof(EngineData), 1, file) != 1)
                logError("SAVE: Failed to write %s. Code %i", filePath, errno);
            fclose(file);
        } else {
            fclose(file);
            EngineData current = {};
            if (engineData.version != current.version) {
                logWarning(
                    "SAVE: Version mismatch. Found %u, expected %u",
                    engineData.version,
                    current.version);
                logWarning("  Rewriting file");
                engineData = {};
                file       = fopen(filePath, "wb");
                if (!file || fwrite(&engineData, sizeof(EngineData), 1, file) != 1)
                    logError("SAVE: Failed to write %s. Code %i", filePath, errno);
                fclose(file);
            }
        }
    }
}

#endif
