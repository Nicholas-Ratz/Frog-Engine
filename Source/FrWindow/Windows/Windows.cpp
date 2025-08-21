#include <FrogEngine/Utility.h>

#ifdef FR_WINDOWS
#include <stdio.h>
#include <windows.h>

#include <FrogEngine/Log.h>
#include <FrogEngine/Window.h>

#include "KeyMapping.h"

constexpr u32 TEXT_INPUT_ALIGNMENT { 256 };
constexpr u32 MAX_INPUT_POLLING { 16 };

char wCharToChar(const WCHAR input) {
    char out;
    if (WideCharToMultiByte(CP_ACP, 0, &input, 1, &out, 1, nullptr, nullptr) > 0) return out;
    return ' ';
}
const wchar_t* charToWChar(const char* input) {
    if (!input) FrogEngine::LogError("Invalid window title");
    const int size = MultiByteToWideChar(CP_UTF8, 0, input, -1, nullptr, 0);
    if (size == 0) FrogEngine::LogError("Invalid window title");

    const auto wide_title = (wchar_t*)malloc(size * sizeof(wchar_t));
    if (!wide_title) FrogEngine::LogError("Failed to allocate wide title");
    if (MultiByteToWideChar(CP_UTF8, 0, input, -1, wide_title, size) == 0)
        FrogEngine::LogError("Invalid window title");

    return wide_title;
}

LRESULT CALLBACK windowProc(
    const HWND h_window, const UINT u_message, const WPARAM w_param, const LPARAM l_param) {
    FrogEngine::Window* window = nullptr;

    if (u_message == WM_NCCREATE) {
        const auto cs = (CREATESTRUCTW*)l_param; // NOLINT
        window        = (FrogEngine::Window*)(cs->lpCreateParams);

        SetWindowLongPtrW(h_window, GWLP_USERDATA, (LONG_PTR)window);
    } else {
        window = (FrogEngine::Window*)GetWindowLongPtrW(h_window, GWLP_USERDATA); // NOLINT
    }

    switch (u_message) {
        case WM_CLOSE     : DestroyWindow(h_window); break;
        case WM_DESTROY   : PostQuitMessage(0); break;
        case WM_CHAR      : window->handleTextEvents(wCharToChar((WCHAR)w_param)); break;
        case WM_KEYDOWN   : window->handleKeyEvents(w_param, true); break;
        case WM_KEYUP     : window->handleKeyEvents(w_param, false); break;
        case WM_SYSKEYDOWN: window->handleKeyEvents(w_param, true); return 0;
        case WM_SYSKEYUP  : window->handleKeyEvents(w_param, false); return 0;
        case WM_SYSCOMMAND:
            if (w_param == SC_KEYMENU && l_param == VK_F10) return 0;
            break;
        case WM_LBUTTONDOWN: window->handleMouseEvents(FrogEngine::MOUSE_LEFT, true); break;
        case WM_LBUTTONUP  : window->handleMouseEvents(FrogEngine::MOUSE_LEFT, false); break;
        case WM_RBUTTONDOWN: window->handleMouseEvents(FrogEngine::MOUSE_RIGHT, true); break;
        case WM_RBUTTONUP  : window->handleMouseEvents(FrogEngine::MOUSE_RIGHT, false); break;
        case WM_MBUTTONDOWN: window->handleMouseEvents(FrogEngine::MOUSE_MIDDLE, true); break;
        case WM_MBUTTONUP  : window->handleMouseEvents(FrogEngine::MOUSE_MIDDLE, false); break;
        default            : break;
    }

    return DefWindowProcW(h_window, u_message, w_param, l_param);
}

namespace FrogEngine {
    constexpr wchar_t FR_WINDOW_CLASS_NAME[] { L"FR_WINDOW_CLASS" };

    struct OsWindow {
        WNDCLASSW windowClass {};
        HINSTANCE hInstance { GetModuleHandle(nullptr) };
        HWND      hWindow {};
    };

    Window::Window() {
        if (!SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2))
            LogError("WINDOWS, Failed to set DPI awareness");

        osWindow = (OsWindow*)malloc(sizeof(OsWindow));
        if (!osWindow) LogError("Failed to allocate OsWindow");
        *osWindow = OsWindow {};

        textInput = (char*)malloc(TEXT_INPUT_ALIGNMENT * sizeof(char));
        if (!textInput) LogError("Failed to allocate textInput");
        textAllocated = TEXT_INPUT_ALIGNMENT;
        memset(textInput, 0, textAllocated);

        osWindow->windowClass.lpszClassName = FR_WINDOW_CLASS_NAME;
        osWindow->windowClass.hInstance     = osWindow->hInstance;
        osWindow->windowClass.hIcon =
            LoadIcon(osWindow->hInstance, MAKEINTRESOURCE(FROG_ENGINE_ICON));
        osWindow->windowClass.hCursor       = LoadCursor(nullptr, IDC_ARROW);
        osWindow->windowClass.lpfnWndProc   = windowProc;
        osWindow->windowClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);


        if (!RegisterClassW(&osWindow->windowClass))
            LogError("WINDOWS, Failed to register window class");
    }

    Window::~Window() {
        if (!UnregisterClassW(FR_WINDOW_CLASS_NAME, osWindow->hInstance))
            LogError("WINDOWS, Failed to unregister window class");
        free(osWindow);
        free(textInput);
        osWindow = nullptr;
    }

    void Window::init(const WindowInfo* window_info) {
        windowInfo = *window_info;

        DWORD style { WS_VISIBLE };

        RECT rect;
        rect.left   = windowInfo.x;
        rect.top    = windowInfo.y;
        rect.right  = windowInfo.x + windowInfo.width;
        rect.bottom = windowInfo.y + windowInfo.height;

        switch (window_info->style) {
            case WINDOWED: {
                style |= WS_OVERLAPPEDWINDOW;
                if (!AdjustWindowRect(&rect, style, FALSE))
                    LogError("WINDOWS, Failed to adjust window rect");
                break;
            }
            case BORDERLESS: {
                style |= WS_POPUP;
                break;
            }
            case FULLSCREEN: {
                style       |= WS_POPUP | WS_MAXIMIZE;
                rect.left    = 0;
                rect.top     = 0;
                rect.right   = GetSystemMetrics(SM_CXSCREEN);
                rect.bottom  = GetSystemMetrics(SM_CYSCREEN);
                break;
            }
            default: LogError("Invalid window style");
        }

        const wchar_t* wide_title = charToWChar(window_info->title);

        osWindow->hWindow = CreateWindowExW(
            0,
            FR_WINDOW_CLASS_NAME,
            wide_title,
            style,
            rect.left,
            rect.top,
            rect.right - rect.left,
            rect.bottom - rect.top,
            nullptr,
            nullptr,
            osWindow->hInstance,
            this);
        free((void*)wide_title);
        if (!osWindow->hWindow) LogError("WINDOWS, Failed to create window");
    }
    void Window::close() const { DestroyWindow(osWindow->hWindow); }

    bool Window::pollEvents() {
        if (!IsWindow(osWindow->hWindow)) return false;

        MSG msg {};
        mousePress        = 0;
        mouseRelease      = 0;
        keyPress          = 0;
        keyRelease        = 0;
        keySpecialPress   = 0;
        keySpecialRelease = 0;

        getModifierBits(&keyPress, &keyDown, &keyRelease);

        POINT point;
        if (!GetCursorPos(&point) || !ScreenToClient(osWindow->hWindow, &point))
            LogError("WINDOWS, Failed to get cursor position");
        mouseX = point.x;
        mouseY = point.y;

        int message_count = MAX_INPUT_POLLING;
        while (message_count-- && PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) return false;
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }

        return true;
    }

    void Window::setWindowTitle(const char* title) {
        windowInfo.title          = title;
        const wchar_t* wide_title = charToWChar(title);

        SetWindowTextW(osWindow->hWindow, wide_title);
        free((void*)wide_title);
    }
    void Window::setWindowPos(const i32 x, const i32 y) {
        SetWindowPos(osWindow->hWindow, HWND_TOP, x, y, windowInfo.width, windowInfo.height, 0);
        windowInfo.x = x;
        windowInfo.y = y;
    }
    void Window::setWindowSize(const i32 width, const i32 height) {
        SetWindowPos(osWindow->hWindow, HWND_TOP, windowInfo.x, windowInfo.y, width, height, 0);
        windowInfo.width  = width;
        windowInfo.height = height;
    }
    void Window::setWindowStyle(const WindowStyle window_style) {
        if (windowInfo.style == window_style) return;

        DWORD style { WS_VISIBLE };
        RECT  rect;
        rect.left   = windowInfo.x;
        rect.top    = windowInfo.y;
        rect.right  = windowInfo.x + windowInfo.width;
        rect.bottom = windowInfo.y + windowInfo.height;
        if (windowInfo.style != FULLSCREEN) {
            if (!GetWindowRect(osWindow->hWindow, &rect))
                LogError("WINDOWS, Failed to get window rect");
            windowInfo.x      = rect.left;
            windowInfo.y      = rect.top;
            windowInfo.width  = rect.right - rect.left;
            windowInfo.height = rect.bottom - rect.top;
        }

        switch (window_style) {
            case WINDOWED: {
                style |= WS_OVERLAPPEDWINDOW;
                break;
            }
            case BORDERLESS: style |= WS_POPUP; break;
            case FULLSCREEN: {
                style       |= WS_POPUP | WS_MAXIMIZE;
                rect.left    = 0;
                rect.top     = 0;
                rect.right   = GetSystemMetrics(SM_CXSCREEN);
                rect.bottom  = GetSystemMetrics(SM_CYSCREEN);
                break;
            }
            default: LogError("Invalid window style");
        }

        if (!SetWindowLongPtrW(osWindow->hWindow, GWL_STYLE, style))
            LogError("WINDOWS, Failed to set window style");
        if (!SetWindowPos(
                osWindow->hWindow,
                HWND_TOP,
                rect.left,
                rect.top,
                rect.right - rect.left,
                rect.bottom - rect.top,
                SWP_FRAMECHANGED | SWP_NOZORDER))
            LogError("WINDOWS, Failed to set window pos");
        if (!UpdateWindow(osWindow->hWindow)) LogError("WINDOWS, Failed to update window");
        if (!InvalidateRect(osWindow->hWindow, nullptr, TRUE))
            LogError("WINDOWS, Failed to invalidate window");

        windowInfo.style = window_style;
    }
    const char* Window::getWindowTitle() const { return windowInfo.title; }
    void        Window::getWindowPos(i32* x, i32* y) const {
        *x = windowInfo.x;
        *y = windowInfo.y;
    }
    void Window::getWindowSize(i32* width, i32* height) const {
        *width  = windowInfo.width;
        *height = windowInfo.height;
    }
    WindowStyle Window::getWindowStyle() const { return windowInfo.style; }

    void Window::startTextInput() { textInputEnabled = true; }
    void Window::stopTextInput() { textInputEnabled = false; }
    void Window::loadTextInput(const char* text, const u32 length) {
        free(textInput);
        textAllocated = TEXT_INPUT_ALIGNMENT * (length / TEXT_INPUT_ALIGNMENT + 1);
        textInput     = (char*)malloc(textAllocated);
        if (!textInput) LogError("Failed to allocate text input");
        textIndex = 0;
        memset(textInput, 0, textAllocated);
        strcpy_s(textInput, length, text);
    }
    void Window::clearTextInput() {
        free(textInput);
        textInput = (char*)malloc(TEXT_INPUT_ALIGNMENT * sizeof(char));
        if (!textInput) LogError("Failed to allocate text input");
        textAllocated = TEXT_INPUT_ALIGNMENT;
        textIndex     = 0;
        memset(textInput, 0, textAllocated);
    }
    void Window::setTextIndex(const u32 index) {
        if (index >= textAllocated) return;
        textIndex = index;
    }
    const char* Window::getText() const { return textInput; }
    u32         Window::getTextIndex() const { return textIndex; }
    bool        Window::isTextInputEnabled() const { return textInputEnabled; }

    void Window::getMousePos(i32* x, i32* y) const {
        *x = mouseX;
        *y = mouseY;
    }
    u8  Window::getMousePress() const { return mousePress; }
    u8  Window::getMouseDown() const { return mouseDown; }
    u8  Window::getMouseRelease() const { return mouseRelease; }
    u64 Window::getKeyPress() const { return keyPress; }
    u64 Window::getKeyDown() const { return keyDown; }
    u64 Window::getKeyRelease() const { return keyRelease; }
    u64 Window::getSpecialKeyPress() const { return keySpecialPress; }
    u64 Window::getSpecialKeyDown() const { return keySpecialDown; }
    u64 Window::getSpecialKeyRelease() const { return keySpecialRelease; }

    void Window::handleMouseEvents(const u64 bit, const bool isDown) {
        if (isDown) {
            mousePress |= bit & ~mouseDown;
            mouseDown  |= bit;
            return;
        }
        mouseRelease |= bit;
        mouseDown    &= ~bit;
    }
    void Window::handleKeyEvents(const u64 input, const bool isDown) {
        if (input > 255 || KEY_MAP[(u8)input] == 255) return;
        u64 bit = KEY_MAP[input];

        if (bit >= 64) {
            bit = 1llu << (bit - 64);
            if (isDown) {
                keySpecialPress |= bit & ~keySpecialDown;
                keySpecialDown  |= bit;
                return;
            }
            keySpecialRelease |= bit;
            keySpecialDown    &= ~bit;
            return;
        }

        bit = 1llu << bit;
        if (isDown) {
            keyPress |= bit & ~keyDown;
            keyDown  |= bit;
            return;
        }
        keyRelease |= bit;
        keyDown    &= ~bit;
    }
    void Window::handleTextEvents(const char character) {
        if (!textInputEnabled) return;
        if (textIndex == textAllocated) {
            textInput = (char*)realloc(textInput, textAllocated + TEXT_INPUT_ALIGNMENT);
            if (!textInput) LogError("Failed to reallocate text input");
            memset(textInput + textAllocated, 0, TEXT_INPUT_ALIGNMENT);
            textAllocated += TEXT_INPUT_ALIGNMENT;
        }
        switch (character) {
            case '\r': {
                textInput[textIndex++] = '\n';
                break;
            }
            case '\b': {
                if (textIndex > 0) textInput[--textIndex] = 0;
                break;
            }
            default: {
                textInput[textIndex++] = character;
                break;
            }
        }
    }

}

#endif
