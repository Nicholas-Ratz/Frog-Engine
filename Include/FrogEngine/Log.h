#ifndef FR_LOG_H
#define FR_LOG_H

#include <FrogEngine/Utility.h>

namespace FrogEngine {
    enum ErrorCode : u32 {
        MALLOC_FAILURE = 0x00'00'00'00,

        WIN_REGISTER_CLASS_FAILURE   = 0x01'01'00'00,
        WIN_CREATE_WINDOW_FAILURE    = 0x01'01'00'01,
        WIN_UNREGISTER_CLASS_FAILURE = 0x01'01'00'02,
    };

    void LogInfo(const char* message);
    void LogError(u32 error);
}

#endif
