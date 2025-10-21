#ifndef FROGENGINE_WINDOW_H
#define FROGENGINE_WINDOW_H

#include <FrSTD/Allocator.h>
#include <FrSTD/Utility.h>

#ifdef FR_WINDOWS
#include <Windows.h>
#endif


namespace FrogEngine {
    struct OsWindow;

    constexpr usize TEXT_INPUT_BUFFER_SIZE { 256 };
    constexpr u32   MAX_INPUT_POLLING { 16 };

    enum WindowStyle : u8 {
        WINDOWED   = 0,
        BORDERLESS = 1,
        FULLSCREEN = 2,
    };

    struct WindowInfo {
        const char* title { "Frog Engine Game" };
        i32         width { 640 };
        i32         height { 480 };
        i32         x { 100 };
        i32         y { 100 };
        WindowStyle style { WINDOWED };
        const char* icon { nullptr };
    };

    class Window {
      public:
#ifdef FR_WINDOWS
        explicit Window(const char* class_name);
        const char* className { "FR_CLASS_NAME" };
#endif

        Window();
        ~Window();

        void init(const WindowInfo* window_info);
        void close() const;
        bool pollEvents();

        void setWindowTitle(const char* title);
        void setWindowPos(i32 x, i32 y);
        void setWindowSize(i32 width, i32 height);
        void setWindowStyle(WindowStyle window_style);

        const char* getWindowTitle() const;
        void        getWindowPos(i32* x, i32* y) const;
        void        getWindowSize(i32* width, i32* height) const;
        WindowStyle getWindowStyle() const;

        void        startTextInput();
        void        stopTextInput();
        void        loadTextInput(const char* text, u32 size);
        void        clearTextInput();
        const char* getText() const;
        bool        isTextInputEnabled() const;

        void getMousePos(i32* x, i32* y) const;
        void getRelativeMousePos(f32* x, f32* y) const;
        u8   getMousePress() const;
        u8   getMouseDown() const;
        u8   getMouseRelease() const;
        u64  getKeyPress() const;
        u64  getKeyDown() const;
        u64  getKeyRelease() const;
        u64  getSpecialKeyPress() const;
        u64  getSpecialKeyDown() const;
        u64  getSpecialKeyRelease() const;

        void handleMouseEvents(u64 bit, bool isDown);
        void handleKeyEvents(u64 input, bool isDown);
        void handleTextEvents(u32 character);

        void updateWindowsRect();

      private:
        WindowInfo windowInfo {};

        OsWindow* osWindow { nullptr };

        bool textInputEnabled { false };

        FrSTD::Allocators::Stack<char> textInputBuffer { TEXT_INPUT_BUFFER_SIZE };

        u8  mousePress { 0 };
        u8  mouseDown { 0 };
        u8  mouseRelease { 0 };
        i32 mouseX { 0 };
        i32 mouseY { 0 };

        u64 keyPress { 0 };
        u64 keyDown { 0 };
        u64 keyRelease { 0 };
        u64 keySpecialPress { 0 };
        u64 keySpecialDown { 0 };
        u64 keySpecialRelease { 0 };
    };

    enum Input : u64 {
        INPUT_NONE       = 0llu,
        INPUT_ANY        = 18'446'744'073'709'551'615llu,
        KEY_ALPHANUMERIC = 137'438'953'471llu,
        KEY_CHARACTER    = 281'474'976'710'655llu,
        KEY_MODIFIER     = 1'152'640'029'630'136'320llu,
        KEY_ARROWS       = 17'293'822'569'102'704'640llu,
        KEY_FN           = 16'777'215llu,
        KEY_NUMPAD       = 2'199'006'478'336llu,
        KEY_SPECIAL      = 68'169'720'922'112llu,

        MOUSE_LEFT   = 1llu << 0,
        MOUSE_RIGHT  = 1llu << 1,
        MOUSE_MIDDLE = 1llu << 2,

        KEY_A = 1llu << 0,
        KEY_B = 1llu << 1,
        KEY_C = 1llu << 2,
        KEY_D = 1llu << 3,
        KEY_E = 1llu << 4,
        KEY_F = 1llu << 5,
        KEY_G = 1llu << 6,
        KEY_H = 1llu << 7,
        KEY_I = 1llu << 8,
        KEY_J = 1llu << 9,
        KEY_K = 1llu << 10,
        KEY_L = 1llu << 11,
        KEY_M = 1llu << 12,
        KEY_N = 1llu << 13,
        KEY_O = 1llu << 14,
        KEY_P = 1llu << 15,
        KEY_Q = 1llu << 16,
        KEY_R = 1llu << 17,
        KEY_S = 1llu << 18,
        KEY_T = 1llu << 19,
        KEY_U = 1llu << 20,
        KEY_V = 1llu << 21,
        KEY_W = 1llu << 22,
        KEY_X = 1llu << 23,
        KEY_Y = 1llu << 24,
        KEY_Z = 1llu << 25,

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

        KEY_LEFT  = 1llu << 60,
        KEY_RIGHT = 1llu << 61,
        KEY_UP    = 1llu << 62,
        KEY_DOWN  = 1llu << 63,

        KEY_F1  = 1llu << 0,
        KEY_F2  = 1llu << 1,
        KEY_F3  = 1llu << 2,
        KEY_F4  = 1llu << 3,
        KEY_F5  = 1llu << 4,
        KEY_F6  = 1llu << 5,
        KEY_F7  = 1llu << 6,
        KEY_F8  = 1llu << 7,
        KEY_F9  = 1llu << 8,
        KEY_F10 = 1llu << 9,
        KEY_F11 = 1llu << 10,
        KEY_F12 = 1llu << 11,
        KEY_F13 = 1llu << 12,
        KEY_F14 = 1llu << 13,
        KEY_F15 = 1llu << 14,
        KEY_F16 = 1llu << 15,
        KEY_F17 = 1llu << 16,
        KEY_F18 = 1llu << 17,
        KEY_F19 = 1llu << 18,
        KEY_F20 = 1llu << 19,
        KEY_F21 = 1llu << 20,
        KEY_F22 = 1llu << 21,
        KEY_F23 = 1llu << 22,
        KEY_F24 = 1llu << 23,

        KEY_NUMPAD_0 = 1llu << 24,
        KEY_NUMPAD_1 = 1llu << 25,
        KEY_NUMPAD_2 = 1llu << 26,
        KEY_NUMPAD_3 = 1llu << 27,
        KEY_NUMPAD_4 = 1llu << 28,
        KEY_NUMPAD_5 = 1llu << 29,
        KEY_NUMPAD_6 = 1llu << 30,
        KEY_NUMPAD_7 = 1llu << 31,
        KEY_NUMPAD_8 = 1llu << 32,
        KEY_NUMPAD_9 = 1llu << 33,

        KEY_NUMPAD_CLEAR    = 1llu << 34,
        KEY_NUMPAD_ADD      = 1llu << 35,
        KEY_NUMPAD_SUBTRACT = 1llu << 36,
        KEY_NUMPAD_MULTIPLY = 1llu << 37,
        KEY_NUMPAD_DIVIDE   = 1llu << 38,
        KEY_NUMPAD_PERIOD   = 1llu << 39,
        KEY_NUMPAD_LOCK     = 1llu << 40,

        KEY_SPECIAL_INSERT    = 1llu << 41,
        KEY_SPECIAL_HOME      = 1llu << 42,
        KEY_SPECIAL_PAGE_UP   = 1llu << 43,
        KEY_SPECIAL_END       = 1llu << 44,
        KEY_SPECIAL_PAGE_DOWN = 1llu << 45,
    };
}

#endif
