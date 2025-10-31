#include <stdarg.h>
#include <stdio.h>

#include <FrogEngine/Log.h>
#include <FrogEngine/Utility.h>

namespace FrogEngine {
    void logInfo(const char* format, ...) {
#ifdef FR_DEBUG
        char* args;

        va_start(args, format);
        printf("[INFO] ");
        vprintf(format, args);
        printf("\n");
        va_end(args);
#endif
    }

    void logWarning(const char* format, ...) {
#ifdef FR_DEBUG
        char* args;

        va_start(args, format);
        fprintf(stderr, "[WARNING] ");
        vfprintf(stderr, format, args);
        fprintf(stderr, "\n");
        va_end(args);
#endif
    }
}

#ifdef FR_OS_WINDOWS
#include <Windows.h>

namespace FrogEngine {
    void logError(const char* format, ...) {
#ifdef FR_DEBUG
        char* args;

        va_start(args, format);
        fprintf(stderr, "[ERROR] ");
        vfprintf(stderr, format, args);
        fprintf(stderr, "\n");
        va_end(args);
#endif

        MessageBoxA(nullptr, format, "Error", MB_OK | MB_ICONERROR);

        exit(-1);
    }
}

#endif
