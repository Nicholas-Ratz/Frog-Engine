#ifndef FR_WINDOW_H
#define FR_WINDOW_H

#include <FrogEngine/Utility.h>

#ifdef FR_WINDOWS
#define FROG_ENGINE_ICON 101
#endif

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

        void init(const WindowInfo* window_info) const;
        void close() const;

        bool pollEvents();
        void updateKeys(u64 key, bool isDown);

        u64 keyPress {};
        u64 keyDown {};
        u64 keyRelease {};

      private:
        OsWindow* osWindow { nullptr };
    };

    enum Key : u64 {
        KEY_NONE = 0,
        KEY_ANY  = 18'446'744'073'709'551'615llu,

        KEY_A     = 1llu << 0,
        KEY_B     = 1llu << 1,
        KEY_C     = 1llu << 2,
        KEY_D     = 1llu << 3,
        KEY_E     = 1llu << 4,
        KEY_F     = 1llu << 5,
        KEY_G     = 1llu << 6,
        KEY_H     = 1llu << 7,
        KEY_I     = 1llu << 8,
        KEY_J     = 1llu << 9,
        KEY_K     = 1llu << 10,
        KEY_L     = 1llu << 11,
        KEY_M     = 1llu << 12,
        KEY_N     = 1llu << 13,
        KEY_O     = 1llu << 14,
        KEY_P     = 1llu << 15,
        KEY_Q     = 1llu << 16,
        KEY_R     = 1llu << 17,
        KEY_S     = 1llu << 18,
        KEY_T     = 1llu << 19,
        KEY_U     = 1llu << 20,
        KEY_V     = 1llu << 21,
        KEY_W     = 1llu << 22,
        KEY_X     = 1llu << 23,
        KEY_Y     = 1llu << 24,
        KEY_Z     = 1llu << 25,
        KEY_0     = 1llu << 26,
        KEY_1     = 1llu << 27,
        KEY_2     = 1llu << 28,
        KEY_3     = 1llu << 29,
        KEY_4     = 1llu << 30,
        KEY_5     = 1llu << 31,
        KEY_6     = 1llu << 32,
        KEY_7     = 1llu << 33,
        KEY_8     = 1llu << 34,
        KEY_9     = 1llu << 35,
        KEY_SPACE = 1llu << 36,

        KEY_SEMI_COLON    = 1llu << 37,
        KEY_FORWARD_SLASH = 1llu << 38,
        KEY_BACKTICK      = 1llu << 39,
        KEY_LEFT_BRACKET  = 1llu << 40,
        KEY_BACKSLASH     = 1llu << 41,
        KEY_RIGHT_BRACKET = 1llu << 42,
        KEY_APOSTROPHE    = 1llu << 43,
        KEY_COMMA         = 1llu << 44,
        KEY_MINUS         = 1llu << 45,
        KEY_PERIOD        = 1llu << 46,
        KEY_EQUALS        = 1llu << 47,

        KEY_LEFT_CONTROL  = 1llu << 48,
        KEY_RIGHT_CONTROL = 1llu << 49,
        KEY_LEFT_SHIFT    = 1llu << 50,
        KEY_RIGHT_SHIFT   = 1llu << 51,
        KEY_LEFT_ALT      = 1llu << 52,
        KEY_RIGHT_ALT     = 1llu << 53,

        KEY_CAPS_LOCK = 1llu << 54,
        KEY_TAB       = 1llu << 55,
        KEY_ENTER     = 1llu << 56,
        KEY_BACKSPACE = 1llu << 57,
        KEY_ESCAPE    = 1llu << 58,
        KEY_DELETE    = 1llu << 59,

        KEY_LEFT  = 60,
        KEY_RIGHT = 61,
        KEY_UP    = 62,
        KEY_DOWN  = 63,
    };
}

#endif
