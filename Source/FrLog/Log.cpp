#include <stdio.h>
#include <stdlib.h>

#include <FrogEngine/Log.h>
#include <FrogEngine/Utility.h>

namespace FrogEngine {
    void logInfo(const char* message) {
#ifndef NDEBUG
        printf("[INFO]: %s\n", message);
#endif
    }
}

#ifdef FR_WINDOWS
#include <Windows.h>

namespace FrogEngine {
    void logError(const char* error) {
#ifndef NDEBUG
        fprintf(stderr, "[ERROR]: %s\n", error);
#endif

        MessageBox(nullptr, TEXT(error), TEXT("Error"), MB_OK | MB_ICONERROR);

        exit(-1);
    }
}

#endif
