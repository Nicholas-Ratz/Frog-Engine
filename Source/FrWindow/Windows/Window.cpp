#include <FrogEngine/Utility.h>

#ifdef FR_WINDOWS
#include <Windows.h>

#include <FrogEngine/Log.h>
#include <FrogEngine/Window.h>

#include "Windows.h"

namespace FrogEngine {
    Window::Window() {
        if (!SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2))
            logError("WINDOWS, Failed to set DPI awareness");

        osWindow = (OsWindow*)malloc(sizeof(OsWindow));
        if (!osWindow) logError("Failed to allocate OsWindow");
        *osWindow = OsWindow {};

        textInput = (char*)malloc(TEXT_INPUT_ALIGNMENT * sizeof(char));
        if (!textInput) logError("Failed to allocate textInput");
        textAllocated = TEXT_INPUT_ALIGNMENT;
        textInput[0]  = 0;

        osWindow->windowClass.lpszClassName = FR_WINDOW_CLASS_NAME;
        osWindow->windowClass.hInstance     = osWindow->hInstance;
        osWindow->windowClass.hIcon =
            LoadIcon(osWindow->hInstance, MAKEINTRESOURCE(FROG_ENGINE_ICON));
        osWindow->windowClass.hCursor       = LoadCursor(nullptr, IDC_ARROW);
        osWindow->windowClass.lpfnWndProc   = windowProc;
        osWindow->windowClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);

        if (!RegisterClass(&osWindow->windowClass))
            logError("WINDOWS, Failed to register window class");

        logInfo("Window Class Registered");
    }

    Window::~Window() {
        if (!UnregisterClass(FR_WINDOW_CLASS_NAME, osWindow->hInstance))
            logError("WINDOWS, Failed to unregister window class");
        logInfo("Window Class Unregistered");
        free(textInput);
        free(osWindow);
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
                    logError("WINDOWS, Failed to adjust window rect");
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
            default: logError("Invalid window style");
        }

        osWindow->hWindow = CreateWindowEx(
            0,
            FR_WINDOW_CLASS_NAME,
            TEXT(window_info->title),
            style,
            rect.left,
            rect.top,
            rect.right - rect.left,
            rect.bottom - rect.top,
            nullptr,
            nullptr,
            osWindow->hInstance,
            this);
        if (!osWindow->hWindow) logError("WINDOWS, Failed to create window");
        logInfo("Window Created");
    }
    void Window::close() const {
        DestroyWindow(osWindow->hWindow);
        logInfo("Window Destroyed");
    }

    bool Window::pollEvents() {
        if (!IsWindow(osWindow->hWindow)) return false;

        MSG msg {};
        mousePress        = 0;
        mouseRelease      = 0;
        keyPress          = 0;
        keyRelease        = 0;
        keySpecialPress   = 0;
        keySpecialRelease = 0;

        u64 bit = 0;
        if (GetAsyncKeyState(VK_LCONTROL) & 0x80'00) bit |= 1ull << 48;
        if (GetAsyncKeyState(VK_RCONTROL) & 0x80'00) bit |= 1ull << 49;
        if (GetAsyncKeyState(VK_LSHIFT) & 0x80'00) bit |= 1ull << 50;
        if (GetAsyncKeyState(VK_RSHIFT) & 0x80'00) bit |= 1ull << 51;
        if (GetAsyncKeyState(VK_LMENU) & 0x80'00) bit |= 1ull << 52;
        if (GetAsyncKeyState(VK_RMENU) & 0x80'00) bit |= 1ull << 53;
        constexpr u64 MODIFIER_REGION = 0b11'1111ull << 48;
        keyPress                      = bit & ~keyDown;
        keyRelease                    = keyDown & ~bit & MODIFIER_REGION;
        keyDown                       = keyDown & ~MODIFIER_REGION | bit;

        POINT point;
        if (!GetCursorPos(&point) || !ScreenToClient(osWindow->hWindow, &point))
            logError("WINDOWS, Failed to get cursor position");
        mouseX = point.x;
        mouseY = point.y;

        int message_count = MAX_INPUT_POLLING;
        while (message_count-- && PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) return false;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        return true;
    }

    void Window::setWindowTitle(const char* title) {
        windowInfo.title = title;
        if (!SetWindowText(osWindow->hWindow, TEXT(title)))
            logError("WINDOWS, Failed to set window title");
        logInfo("Window Title Updated");
    }
    void Window::setWindowPos(const i32 x, const i32 y) {
        if (!SetWindowPos(
                osWindow->hWindow, HWND_TOP, x, y, windowInfo.width, windowInfo.height, 0))
            logError("WINDOWS, Failed to set window pos");
        windowInfo.x = x;
        windowInfo.y = y;
        logInfo("Window Position Updated");
    }
    void Window::setWindowSize(const i32 width, const i32 height) {
        if (!SetWindowPos(
                osWindow->hWindow, HWND_TOP, windowInfo.x, windowInfo.y, width, height, 0))
            logError("WINDOWS, Failed to set window size");
        windowInfo.width  = width;
        windowInfo.height = height;
        logInfo("Window Size Updated");
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
                logError("WINDOWS, Failed to get window rect");
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
            default: logError("Invalid window style");
        }

        if (!SetWindowLongPtr(osWindow->hWindow, GWL_STYLE, style))
            logError("WINDOWS, Failed to set window style");
        if (!SetWindowPos(
                osWindow->hWindow,
                HWND_TOP,
                rect.left,
                rect.top,
                rect.right - rect.left,
                rect.bottom - rect.top,
                SWP_FRAMECHANGED | SWP_NOZORDER))
            logError("WINDOWS, Failed to set window pos");
        if (!UpdateWindow(osWindow->hWindow)) logError("WINDOWS, Failed to update window");
        if (!InvalidateRect(osWindow->hWindow, nullptr, TRUE))
            logError("WINDOWS, Failed to invalidate window");

        windowInfo.style = window_style;
        logInfo("Window Style Updated");
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
}

#endif
