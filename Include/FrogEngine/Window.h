/**
 * @file Window.h
 * @brief Window Module
 *
 * This module provides cross-platform window creation and input handling.
 * It abstracts OS-specific window management and provides a unified interface
 * for window properties, text input, mouse, and keyboard events.
 */

#ifndef FROGENGINE_WINDOW_H
#define FROGENGINE_WINDOW_H

#include <FrogEngine/Utility.h>

#ifdef FR_OS_WINDOWS
#    include <Windows.h>
#    define APP_ICON 888
#endif

namespace FrogEngine {
    struct OsWindow;
    class Allocator;
    class Block;

    constexpr u32 MAX_INPUT_POLLING { 16 };

    /**
     * @enum WindowStyle
     * @brief Defines the visual style of the window.
     */
    enum WindowStyle : u8 {
        WINDOWED   = 0, ///< Standard window with title bar and borders
        BORDERLESS = 1, ///< Window without decorations (title bar, borders)
        FULLSCREEN = 2, ///< Fullscreen mode using native resolution
    };

    /**
     * @struct WindowInfo
     * @brief Configuration structure for initializing a window.
     *
     * This struct is passed to Window::init() to specify initial window properties.
     * All fields have sensible defaults.
     *
     * @see Window::init()
     */
    struct WindowInfo {
        const char* title { "Frog Engine Game" }; ///< Window title bar text
        i32         width { 640 };                ///< Initial width in pixels
        i32         height { 480 };               ///< Initial height in pixels
        i32         x { 100 };                    ///< Initial X position on screen
        i32         y { 100 };                    ///< Initial Y position on screen
        WindowStyle style { WINDOWED };           ///< Initial window style
    };

    /**
     * @class Window
     * @brief Manages window creation, input polling, and OS interaction.
     *
     * The Window class provides a platform-agnostic interface for:
     * - Creating and managing native OS windows
     * - Handling keyboard, mouse, and text input
     * - Querying and modifying window properties
     *
     * @note There is only ever one window per class.
     * @note On Windows the parameterized constructor should be used in release.
     * @note Currently only Windows is supported, but more support may come in the future.
     * @see WindowInfo
     */
    class FROGENGINE_MODULE Window {
      public:
        /**
         * @brief Default constructor.
         *
         * Initializes member variables to safe defaults. Call init() to finish window setup and
         * open() to create window.
         *
         * @note Window Icons, as of now, only work for the parameterized function.
         *
         * @see init() open()
         */
        explicit Window(Allocator* allocator);
        /**
         * @brief Destructor.
         *
         * Automatically closes the window and releases OS resources.
         */
        ~Window();


        /**
         * @brief Initializes the internal OS window systems.
         * @param class_name String containing name of internal OS name
         *
         * @note Must be called before any other window operations.
         */
        void init(const char* class_name);
        /**
         * @brief Open the window.
         * @param window_info Contains important information for the window creation.
         */
        void open(const WindowInfo* window_info);
        /**
         * @brief Closes the window and releases resources.
         *
         * Sends close event to OS and cleans up internal state.
         */
        void close() const;
        /**
         * @brief Polls and processes pending OS events.
         * @return true if the window should continue running, false if quit has been requested.
         *
         * Should be called once per frame. Updates input state and dispatches events.
         */
        bool pollEvents();


        /**
         * @brief Sets the window title.
         * @param title Null-terminated string for the new title.
         */
        void setWindowTitle(const char* title);
        /**
         * @brief Sets the window position on screen.
         * @param x New X coordinate in screen space
         * @param y New Y coordinate in screen space
         */
        void setWindowPos(i32 x, i32 y);
        /**
         * @brief Sets the window client area size.
         * @param width New width in pixels
         * @param height New height in pixels
         */
        void setWindowSize(i32 width, i32 height);
        /**
         * @brief Changes the window style (windowed, borderless, fullscreen).
         * @param window_style New style from WindowStyle enum
         */
        void setWindowStyle(WindowStyle window_style);


        /**
         * @brief Gets the current window title.
         * @return Pointer to null-terminated title string (valid until next change)
         */
        const char* getWindowTitle() const;
        /**
         * @brief Retrieves current window position.
         * @param[out] x Pointer to store X coordinate
         * @param[out] y Pointer to store Y coordinate
         */
        void        getWindowPos(i32* x, i32* y) const;
        /**
         * @brief Retrieves current window size.
         * @param[out] width Pointer to store width in pixels
         * @param[out] height Pointer to store height in pixels
         */
        void        getWindowSize(i32* width, i32* height) const;
        /**
         * @brief Gets the current window style.
         * @return Current WindowStyle value
         */
        WindowStyle getWindowStyle() const;


        /**
         * @brief Enables text input mode.
         *
         * Enables appending char input to end of Text buffer.
         * @see stopTextInput(), loadTextInput()
         */
        void        startTextInput();
        /**
         * @brief Disables text input mode.
         *
         * Stops writing to Text buffer.
         * @note Does not clear.
         */
        void        stopTextInput();
        /**
         * @brief Pre-loads text into the input buffer.
         * @param text Source text to insert
         * @param size Length of text (excluding null terminator)
         *
         * Useful for setting default values in text fields.
         */
        void        loadTextInput(const char* text, u32 size);
        /**
         * @brief Clears all text from the input buffer.
         */
        void        clearTextInput();
        /**
         * @brief Gets the current text input buffer.
         * @return Null-terminated string of current text
         *
         * @note Pointer directly connected to internal Text buffer.
         */
        const char* getText() const;
        /**
         * @brief Checks if text input is currently active.
         * @return true if text input is enabled
         */
        bool        isTextInputEnabled() const;


        /**
         * @brief Gets current mouse position over entire screen.
         * @param[out] x Mouse X position
         * @param[out] y Mouse Y position
         */
        void getMousePos(i32* x, i32* y) const;
        /**
         * @brief Gets current mouse position in window client coordinates.
         * @param[out] x Mouse X position
         * @param[out] y Mouse Y position
         *
         */
        void getRelativeMousePos(f32* x, f32* y) const;
        /**
         * @brief Gets bitmask of mouse buttons pressed this frame.
         * @return Bitfield using MOUSE_LEFT, MOUSE_RIGHT, MOUSE_MIDDLE
         * @see Input
         */
        u8   getMousePress() const;
        /**
         * @brief Gets bitmask of currently held mouse buttons.
         * @return Bitfield of held buttons
         */
        u8   getMouseDown() const;
        /**
         * @brief Gets bitmask of mouse buttons released this frame.
         * @return Bitfield of released buttons
         */
        u8   getMouseRelease() const;
        /**
         * @brief Gets bitmask of alphanumeric/special keys pressed this frame.
         * @return 64-bit mask using KEY_* constants
         */
        u64  getKeyPress() const;
        /**
         * @brief Gets bitmask of currently held keys.
         * @return 64-bit mask of held keys
         */
        u64  getKeyDown() const;
        /**
         * @brief Gets bitmask of keys released this frame.
         * @return 64-bit mask of released keys
         */
        u64  getKeyRelease() const;
        /**
         * @brief Gets bitmask of special keys (arrows, function, numpad) pressed this frame.
         * @return 64-bit mask using KEY_LEFT, KEY_F1, etc.
         */
        u64  getSpecialKeyPress() const;
        /**
         * @brief Gets bitmask of currently held special keys.
         * @return 64-bit mask
         */
        u64  getSpecialKeyDown() const;
        /**
         * @brief Gets bitmask of special keys released this frame.
         * @return 64-bit mask
         */
        u64  getSpecialKeyRelease() const;


        /**
         * @brief Handles mouse button state changes.
         * @param bit Mouse button bit (MOUSE_LEFT, etc.)
         * @param isDown true if pressed, false if released
         *
         * @note Called internally by OS event loop.
         * @note Not recommended to call this function.
         */
        void handleMouseEvents(u64 bit, bool isDown);
        /**
         * @brief Handles keyboard key state changes.
         * @param input Key code from Input enum
         * @param isDown true if pressed, false if released
         * @note Not recommended to call this function.
         */
        void handleKeyEvents(u64 input, bool isDown);
        /**
         * @brief Handles Unicode character input.
         * @param character UTF-32 code point
         *
         * Called when text input is active and user types a character.
         * @note Not recommended to call this function.
         */
        void handleTextEvents(u32 character);


        /**
         * @brief Updates internal window rectangle after style/position changes.
         *
         * @note Called internally after window modifications.
         */
        void updateWindowsRect();

      private:
        Block* block {};

        char className[128] = { 0 };

        WindowInfo windowInfo {};

        OsWindow* osWindow { nullptr };

        bool textInputEnabled { false };

        char* textInput { nullptr };
        usize textIndex {};

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

    /**
     * @enum Input
     * @brief Bitfield constants for keyboard and mouse input.
     *
     * Each key or button is assigned a unique bit in a 64-bit mask.
     * Use bitwise operations to test for multiple keys.
     *
     * @par Example
     * @code
     * if (window.getKeyDown() & (KEY_W | KEY_UP)) { moveForward(); }
     * @endcode
     *
     * @note Some constants overlap in bit position across categories (e.g. KEY_F1 uses bit 0 in
     * special keys). Use getSpecialKey*() functions for function/arrow/numpad keys.
     */
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
