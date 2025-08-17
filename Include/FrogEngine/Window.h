#ifndef FR_WINDOW_H
#define FR_WINDOW_H

#include <FrogEngine/Utility.h>

namespace FrogEngine {
    enum class WindowStyle : u8 {
        HELLO = 0,
    };

    struct WindowInfo {
        const char *title;

        i32 width, height;
        i32 x, y;

        WindowStyle style;
    };

    struct OSWindow;

    class Window {
    public:
        Window();

        ~Window();

        bool init(const WindowInfo *window_info);

        bool pollEvents();

    private:
        OSWindow *osWindow;
    };
}

#endif
