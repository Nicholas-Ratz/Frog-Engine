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
#    include <Windows.h>

namespace FrogEngine {
    void logError(const char* format, ...) {
#    ifdef FR_DEBUG
        char* args;

        va_start(args, format);
        int length = vsnprintf(0, 0, format, args);
        va_end(args);
        if (length >= 0) {
            char* str = (char*)malloc(length + 10);
            if (str) {
                va_start(args, format);
                vsnprintf(str + 8, length + 1, format, args);
                va_end(args);
                memcpy(str, "[ERROR] ", 8);
                str[8 + length] = '\n';
                str[9 + length] = 0;

                MessageBoxA(nullptr, str, "Error", MB_OK | MB_ICONERROR);
                free(str);
            }
        }
#    endif
        exit(-1);
    }
}

#endif
