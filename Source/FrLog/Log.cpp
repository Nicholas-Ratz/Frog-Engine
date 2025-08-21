#include <stdio.h>
#include <stdlib.h>

#include <FrogEngine/Log.h>

namespace FrogEngine {
    void LogInfo(const char* message) {}

    void LogError(const char* error) {
        fprintf(stderr, "%s\n", error);
        exit(-1);
    }
}
