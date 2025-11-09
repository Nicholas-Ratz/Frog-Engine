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
    void FROGENGINE_EXPORT logInfo(const char* format, ...);
    /**
     * @brief Logs a warning message.
     *
     * Indicates a potential issue that does not halt execution but should be
     * reviewed (e.g., recoverable errors, potential slowdowns).
     *
     * @param format Null-terminated format string (printf-style).
     * @param ...   Variable arguments matching the format specifiers.
     */
    void FROGENGINE_EXPORT logWarning(const char* format, ...);
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
    void FROGENGINE_EXPORT logError(const char* format, ...);
}

#endif
