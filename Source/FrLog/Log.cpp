#include <stdio.h>
#include <stdlib.h>

#include <FrogEngine/Log.h>

namespace FrogEngine {
    void LogInfo(const char* message) {}

    void LogError(const u32 error) {
        switch (error) {
            case MALLOC_FAILURE            : fprintf(stderr, "Malloc Failure\n"); break;
            case WIN_REGISTER_CLASS_FAILURE: fprintf(stderr, "Register Window Failure\n"); break;
            case WIN_CREATE_WINDOW_FAILURE : fprintf(stderr, "Create Window Failure\n"); break;
            case WIN_UNREGISTER_CLASS_FAILURE:
                fprintf(stderr, "Unregister Window Failure\n");
                break;
            default: break;
        }

        exit(error);
    }
}
