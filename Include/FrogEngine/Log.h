/**
 * @file Log.h
 * @brief Logging Module
 *
 * This module declares the core logging interface for FrogEngine. It provides three variadic
 * functions for emitting log messages at different severity levels: informational, warning, and
 * error.
 *
 * The logging functions follow a `printf`-style formatting convention, allowing flexible message
 * composition with variable arguments.
 *
 * These functions are disregarded in release mode.
 */
#ifndef FROGENGINE_LOG_H
#define FROGENGINE_LOG_H

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <FrogEngine/Utility.h>

namespace FrogEngine {
    /**
     * @brief Logs an informational message.
     *
     * Use this for general diagnostic output, progress reporting, or
     * non-critical runtime information.
     *
     * @param format Null-terminated format string (printf-style).
     * @param ...   Variable arguments matching the format specifiers.
     */
    inline void logInfo(const char* format, ...) {
#if defined(FR_DEBUG) || defined(FR_LOG)
        char* args;

        va_start(args, format);
        printf("[INFO] ");
        vprintf(format, args);
        printf("\n");
        va_end(args);
#endif
    }

    /**
     * @brief Logs a warning message.
     *
     * Indicates a potential issue that does not halt execution but should be
     * reviewed (e.g., recoverable errors, potential slowdowns).
     *
     * @param format Null-terminated format string (printf-style).
     * @param ...   Variable arguments matching the format specifiers.
     */
    inline void logWarning(const char* format, ...) {
#if defined(FR_DEBUG) || defined(FR_LOG)
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
#endif
/**
 * @brief Logs an error message.
 *
 * Reports critical failures that may affect program stability or correctness
 * (e.g., missing resources, invalid states, failed allocations).
 *
 * @param format Null-terminated format string (printf-style).
 * @param ...   Variable arguments matching the format specifiers.
 *
 * @note If called app will cleanly exit and abort.
 */
namespace FrogEngine {
    inline void logError(const char* format, ...) {
#if defined(FR_DEBUG) || defined(FR_LOG)
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
#    ifdef FR_OS_WINDOWS
                MessageBoxA(nullptr, str, "Error", MB_OK | MB_ICONERROR);
#    endif
                free(str);
            }
        }
#endif
        exit(-1);
    }
}

#endif
