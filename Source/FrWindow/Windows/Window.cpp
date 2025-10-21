#include <FrSTD/Utility.h>
#ifdef FR_WINDOWS
#include <Windows.h>

#include <FrogEngine/Icon.h>
#include <FrogEngine/Log.h>
#include <FrogEngine/Window.h>

#include "Windows.h"

namespace FrogEngine {
    Window::Window(const char* class_name) : className(class_name) {
        if (!SetProcessDPIAware())
            logWarning("WINDOWS: Failed to set DPI awareness: %lx", GetLastError());

        osWindow = (OsWindow*)malloc(sizeof(OsWindow));
        if (!osWindow) logError("Failed to allocate OsWindow");
        *osWindow = OsWindow {};

        osWindow->windowClass.cbSize        = sizeof(WNDCLASSEXA);
        osWindow->windowClass.lpszClassName = className;
        osWindow->windowClass.hInstance     = osWindow->hInstance;
        osWindow->windowClass.hCursor       = LoadCursorA(nullptr, IDC_ARROW);
        osWindow->windowClass.lpfnWndProc   = windowProc;
        osWindow->windowClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);

        HICON icon = LoadIconA(osWindow->hInstance, MAKEINTRESOURCE(APP_ICON));
        if (!icon) logError("WINDOWS: Failed to load icon: %lx", GetLastError());
        osWindow->windowClass.hIcon   = icon;
        osWindow->windowClass.hIconSm = icon;

        logInfo("WINDOWS: Window Class Registered");
        logInfo("  Name: %s", className);

        if (!RegisterClassExA(&osWindow->windowClass))
            logError("WINDOWS: Failed to register window class: %lx", GetLastError());
    }

    Window::Window() {
        if (!SetProcessDPIAware())
            logWarning("WINDOWS: Failed to set DPI awareness: %lx", GetLastError());

        osWindow = (OsWindow*)malloc(sizeof(OsWindow));
        if (!osWindow) logError("Failed to allocate OsWindow");
        *osWindow = OsWindow {};

        osWindow->windowClass.cbSize        = sizeof(WNDCLASSEXA);
        osWindow->windowClass.lpszClassName = className;
        osWindow->windowClass.hInstance     = osWindow->hInstance;
        osWindow->windowClass.hCursor       = LoadCursorA(nullptr, IDC_ARROW);
        osWindow->windowClass.lpfnWndProc   = windowProc;
        osWindow->windowClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);

        logInfo("WINDOWS: Window Class Registered");
        logInfo("  Name: %s", className);

        if (!RegisterClassExA(&osWindow->windowClass))
            logError("WINDOWS: Failed to register window class: %lx", GetLastError());
    }

    Window::~Window() {
        if (!UnregisterClassA(className, osWindow->hInstance))
            logError("WINDOWS: Failed to unregister window class: %lx", GetLastError());
        logInfo("WINDOWS: Window Class Unregistered");
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
                    logWarning("WINDOWS: Failed to adjust window rect: %lx", GetLastError());
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
            default:
                logWarning("Invalid window style");
                style |= WS_OVERLAPPEDWINDOW;
                if (!AdjustWindowRect(&rect, style, FALSE))
                    logWarning("WINDOWS: Failed to adjust window rect: %lx", GetLastError());
        }

        osWindow->hWindow = CreateWindowExA(
            0,
            className,
            window_info->title,
            style,
            rect.left,
            rect.top,
            rect.right - rect.left,
            rect.bottom - rect.top,
            nullptr,
            nullptr,
            osWindow->hInstance,
            this);
        if (!osWindow->hWindow) logError("WINDOWS: Failed to create window: %lx", GetLastError());
        logInfo("WINDOWS: Window Created");
        logInfo("  Title: %s", window_info->title);
        logInfo("  Size: (%i, %i)", window_info->width, window_info->height);
        logInfo("  Position: (%i, %i)", window_info->x, window_info->y);

        GetClientRect(osWindow->hWindow, &rect);
        windowInfo.x      = rect.left;
        windowInfo.y      = rect.top;
        windowInfo.width  = rect.right - rect.left;
        windowInfo.height = rect.bottom - rect.top;
    }
    void Window::close() const {
        DestroyWindow(osWindow->hWindow);
        logInfo("WINDOWS: Window Destroyed");
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
            logWarning("WINDOWS: Failed to get cursor position: %lx", GetLastError());
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
        if (!SetWindowTextA(osWindow->hWindow, title))
            logWarning("WINDOWS: Failed to set window title: %lx", GetLastError());
        logInfo("WINDOWS: Window Title Updated");
        logInfo("  Title: %s", title);
    }
    void Window::setWindowPos(const i32 x, const i32 y) {
        if (!SetWindowPos(
                osWindow->hWindow, HWND_TOP, x, y, windowInfo.width, windowInfo.height, 0))
            logWarning("WINDOWS: Failed to set window pos: %lx", GetLastError());
        windowInfo.x = x;
        windowInfo.y = y;
        logInfo("WINDOWS: Window Position Updated");
        logInfo("  Position: (%i, %i)", windowInfo.x, windowInfo.y);
    }
    void Window::setWindowSize(const i32 width, const i32 height) {
        if (!SetWindowPos(
                osWindow->hWindow, HWND_TOP, windowInfo.x, windowInfo.y, width, height, 0))
            logWarning("WINDOWS: Failed to set window size: %lx", GetLastError());
        windowInfo.width  = width;
        windowInfo.height = height;
        logInfo("WINDOWS: Window Size Updated");
        logInfo("  Size: (%i, %i)", windowInfo.width, windowInfo.height);
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
            if (!GetWindowRect(osWindow->hWindow, &rect)) {
                logWarning("WINDOWS: Failed to get window rect: %lx", GetLastError());
                return;
            }
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
            default: logWarning("Invalid window style"); return;
        }

        if (!SetWindowLongPtr(osWindow->hWindow, GWL_STYLE, style))
            logError("WINDOWS: Failed to set window style: %lx", GetLastError());
        if (!SetWindowPos(
                osWindow->hWindow,
                HWND_TOP,
                rect.left,
                rect.top,
                rect.right - rect.left,
                rect.bottom - rect.top,
                SWP_FRAMECHANGED | SWP_NOZORDER))
            logWarning("WINDOWS: Failed to set window pos: %lx", GetLastError());
        if (!UpdateWindow(osWindow->hWindow))
            logWarning("WINDOWS: Failed to update window: %lx", GetLastError());
        if (!InvalidateRect(osWindow->hWindow, nullptr, TRUE))
            logWarning("WINDOWS: Failed to invalidate window: %lx", GetLastError());

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

    void Window::updateWindowsRect() {
        RECT rect;

        GetClientRect(osWindow->hWindow, &rect);

        windowInfo.x      = rect.left;
        windowInfo.y      = rect.top;
        windowInfo.width  = rect.right - rect.left;
        windowInfo.height = rect.bottom - rect.top;
    }
}

#endif
