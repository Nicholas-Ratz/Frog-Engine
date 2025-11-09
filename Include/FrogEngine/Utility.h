#ifndef FROGENGINE_UTILITY_H
#define FROGENGINE_UTILITY_H

#if defined(_WIN32) || defined(_WIN64)
#    define FR_OS_WINDOWS
#elif defined(__linux__) || defined(linux) || defined(__linux)
#    define FR_OS_LINUX
#elif defined(__APPLE__) && defined(__MACH__)
#    include <TargetConditional.h>
#    if TARGET_OS_MAC == 1
#        define FR_OS_MAC
#    endif
#endif

#if defined(__SHARED__)
#    if defined(_WIN32) || defined(_WIN64)
#        define FROGENGINE_EXPORT __declspec(dllexport)
#        define FROGENGINE_IMPORT __declspec(dllimport)
#        define FR_SHARED
#    else
#        define FROGENGINE_EXPORT __attribute__((visibility("default")))
#        define FROGENGINE_IMPORT __attribute__((visibility("default")))
#        define FR_SHARED
#    endif
#else
#    define FROGENGINE_EXPORT
#    define FROGENGINE_IMPORT
#endif

#ifndef NDEBUG
#    define FR_DEBUG
#else
#    define FR_RELEASE
#endif

#include <stdint.h>

typedef int8_t  i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef float  f32;
typedef double f64;

typedef void*     ptr;
typedef uintptr_t uptr;
typedef size_t    usize;

#define I8_MAX  INT8_MAX;
#define I16_MAX INT16_MAX;
#define I32_MAX INT32_MAX;
#define I64_MAX INT64_MAX;
#define I8_MIN  INT8_MIN;
#define I16_MIN INT16_MIN;
#define I32_MIN INT32_MIN;
#define I64_MIN INT64_MIN;

#define U8_MAX  UINT8_MAX;
#define U16_MAX UINT16_MAX;
#define U32_MAX UINT32_MAX;
#define U64_MAX UINT64_MAX;

#endif
