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

#include <FrogEngine/Utility.h>

#define FR_LOG_FORMAT_RESET         "\033[0m"
#define FR_LOG_FORMAT_BOLD          "\033[1m"
#define FR_LOG_FORMAT_DIM           "\033[2m"
#define FR_LOG_FORMAT_ITALIC        "\033[3m"
#define FR_LOG_FORMAT_UNDERLINE     "\033[4m"
#define FR_LOG_FORMAT_BLINK         "\033[5m"
#define FR_LOG_FORMAT_STRIKETHROUGH "\033[9m"

#define FR_LOG_FORMAT_BLACK   "\033[30m"
#define FR_LOG_FORMAT_RED     "\033[31m"
#define FR_LOG_FORMAT_GREEN   "\033[32m"
#define FR_LOG_FORMAT_YELLOW  "\033[33m"
#define FR_LOG_FORMAT_BLUE    "\033[34m"
#define FR_LOG_FORMAT_MAGENTA "\033[35m"
#define FR_LOG_FORMAT_CYAN    "\033[36m"
#define FR_LOG_FORMAT_WHITE   "\033[37m"

#define FR_LOG_FORMAT_BRIGHT_BLACK   "\033[90m"
#define FR_LOG_FORMAT_BRIGHT_RED     "\033[91m"
#define FR_LOG_FORMAT_BRIGHT_GREEN   "\033[92m"
#define FR_LOG_FORMAT_BRIGHT_YELLOW  "\033[93m"
#define FR_LOG_FORMAT_BRIGHT_BLUE    "\033[94m"
#define FR_LOG_FORMAT_BRIGHT_MAGENTA "\033[95m"
#define FR_LOG_FORMAT_BRIGHT_CYAN    "\033[96m"
#define FR_LOG_FORMAT_BRIGHT_WHITE   "\033[97m"

#define FR_LOG_FORMAT_BG_BLACK   "\033[40m"
#define FR_LOG_FORMAT_BG_RED     "\033[41m"
#define FR_LOG_FORMAT_BG_GREEN   "\033[42m"
#define FR_LOG_FORMAT_BG_YELLOW  "\033[43m"
#define FR_LOG_FORMAT_BG_BLUE    "\033[44m"
#define FR_LOG_FORMAT_BG_MAGENTA "\033[45m"
#define FR_LOG_FORMAT_BG_CYAN    "\033[46m"
#define FR_LOG_FORMAT_BG_WHITE   "\033[47m"

#define FR_LOG_FORMAT_BG_BRIGHT_BLACK   "\033[100m"
#define FR_LOG_FORMAT_BG_BRIGHT_RED     "\033[101m"
#define FR_LOG_FORMAT_BG_BRIGHT_GREEN   "\033[102m"
#define FR_LOG_FORMAT_BG_BRIGHT_YELLOW  "\033[103m"
#define FR_LOG_FORMAT_BG_BRIGHT_BLUE    "\033[104m"
#define FR_LOG_FORMAT_BG_BRIGHT_MAGENTA "\033[105m"
#define FR_LOG_FORMAT_BG_BRIGHT_CYAN    "\033[106m"
#define FR_LOG_FORMAT_BG_BRIGHT_WHITE   "\033[107m"

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
        printf("%s[INFO] %s", FR_LOG_FORMAT_GREEN, FR_LOG_FORMAT_RESET);
        vprintf(format, args);
        printf("\n%s", FR_LOG_FORMAT_RESET);
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
        fprintf(stderr, "%s[WARNING] %s", FR_LOG_FORMAT_BRIGHT_YELLOW, FR_LOG_FORMAT_RESET);
        vfprintf(stderr, format, args);
        fprintf(stderr, "\n%s", FR_LOG_FORMAT_RESET);
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
                printf(
                    "%s%s%s%s",
                    FR_LOG_FORMAT_BRIGHT_RED,
                    FR_LOG_FORMAT_BOLD,
                    FR_LOG_FORMAT_ITALIC,
                    str);
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
