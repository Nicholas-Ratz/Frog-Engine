#include <stdarg.h>
#include <stdio.h>

#include <FrSTD/Utility.h>
#include <FrogEngine/Log.h>

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

#ifdef FR_WINDOWS
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

        MessageBox(nullptr, TEXT(format), TEXT("Error"), MB_OK | MB_ICONERROR);

        exit(-1);
    }
}

#endif
