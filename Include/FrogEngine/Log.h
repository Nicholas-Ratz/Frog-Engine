#ifndef FROGENGINE_LOG_H
#define FROGENGINE_LOG_H

#include <FrogEngine/Utility.h>

namespace FrogEngine {
    void FROGENGINE_MODULE logInfo(const char* format, ...);
    void FROGENGINE_MODULE logWarning(const char* format, ...);
    void FROGENGINE_MODULE logError(const char* format, ...);
}

#endif
