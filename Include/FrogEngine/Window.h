#ifndef FR_WINDOW_H
#define FR_WINDOW_H

#include <FrogEngine/Utility.h>

namespace FrogEngine {
    struct OsWindow;

    enum class WindowStyle : u8 {
        DEFAULT = 0,
    };

    struct WindowInfo {
        const wchar_t* title { L"Frog Engine Game" };
        i32            width { 640 };
        i32            height { 480 };
        i32            x { 100 };
        i32            y { 100 };
        WindowStyle    style { WindowStyle::DEFAULT };
    };

    class Window {
      public:
        Window();
        ~Window();

        bool init(const WindowInfo* window_info);
        [[nodiscard]]
        bool checkStatus() const;
        bool pollEvents();

      private:
        OsWindow* osWindow { nullptr };
        u16       resultStatus {};
        u64       keyboardInputs {};
    };
}

#endif
