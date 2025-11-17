#include <FrogEngine/Utility.h>

#include <cstring>

#ifdef FR_OS_WINDOWS

#    include <Windows.h>

#    include <FrogEngine/Allocator.h>
#    include <FrogEngine/Log.h>
#    include <FrogEngine/Pointer.h>
#    include <FrogEngine/Window.h>

inline LRESULT CALLBACK windowProc(
    const HWND h_window, const UINT u_message, const WPARAM w_param, const LPARAM l_param) {
    FrogEngine::Window* window = nullptr;

    if (u_message == WM_NCCREATE) {
        const auto cs = (CREATESTRUCTA*)l_param;
        window        = (FrogEngine::Window*)cs->lpCreateParams;

        SetWindowLongPtrA(h_window, GWLP_USERDATA, (LONG_PTR)window);
    } else {
        window = (FrogEngine::Window*)GetWindowLongPtrA(h_window, GWLP_USERDATA);
    }

    switch (u_message) {
        case WM_CLOSE           : DestroyWindow(h_window); break;
        case WM_DESTROY         : PostQuitMessage(0); break;
        case WM_WINDOWPOSCHANGED: window->updateWindowsRect(); break;
        case WM_CHAR            : window->handleTextEvents(w_param); break;
        case WM_KEYDOWN         : window->handleKeyEvents(w_param, true); break;
        case WM_KEYUP           : window->handleKeyEvents(w_param, false); break;
        case WM_SYSKEYDOWN      : window->handleKeyEvents(w_param, true); return 0;
        case WM_SYSKEYUP        : window->handleKeyEvents(w_param, false); return 0;
        case WM_LBUTTONDOWN     : window->handleMouseEvents(FrogEngine::MOUSE_LEFT, true); break;
        case WM_LBUTTONUP       : window->handleMouseEvents(FrogEngine::MOUSE_LEFT, false); break;
        case WM_RBUTTONDOWN     : window->handleMouseEvents(FrogEngine::MOUSE_RIGHT, true); break;
        case WM_RBUTTONUP       : window->handleMouseEvents(FrogEngine::MOUSE_RIGHT, false); break;
        case WM_MBUTTONDOWN     : window->handleMouseEvents(FrogEngine::MOUSE_MIDDLE, true); break;
        case WM_MBUTTONUP       : window->handleMouseEvents(FrogEngine::MOUSE_MIDDLE, false); break;
        case WM_SYSCOMMAND:
            if (w_param == SC_KEYMENU) return 0;
        default: break;
    }

    return DefWindowProc(h_window, u_message, w_param, l_param);
}

namespace FrogEngine {
    struct OsWindow {
        WNDCLASSEXA windowClass { 0 };
        HINSTANCE   hInstance {};
        HWND        hWindow {};
    };

    Window::Window(Allocator* allocator) {
        block = allocator->getStaticBlock();

        osWindow  = block->alloc(sizeof(OsWindow));
        textInput = block->alloc(1'024);
    }

    Window::~Window() {
        if (!UnregisterClassA(className, osWindow->hInstance))
            logError(
                "%sWINDOW%s: Failed to unregister window class: %lx",
                FR_LOG_FORMAT_BLUE,
                FR_LOG_FORMAT_RESET,
                GetLastError());
        logInfo("%sWINDOW%s: Window Class Unregistered", FR_LOG_FORMAT_BLUE, FR_LOG_FORMAT_RESET);
    }

    void Window::init(const char* class_name) {
        if (className[0] != 0) {
            logWarning(
                "%sWINDOW%s: init() called after initialization",
                FR_LOG_FORMAT_BLUE,
                FR_LOG_FORMAT_RESET);
            return;
        }

        if (!SetProcessDPIAware())
            logWarning(
                "%sWINDOW%s: Failed to set DPI awareness: %lx",
                FR_LOG_FORMAT_BLUE,
                FR_LOG_FORMAT_RESET,
                GetLastError());

        osWindow->hInstance = GetModuleHandleA(nullptr);

        memcpy(className, class_name, 16);
        className[15] = 0;

        osWindow->windowClass.cbSize        = sizeof(WNDCLASSEXA);
        osWindow->windowClass.lpszClassName = className;
        osWindow->windowClass.hInstance     = osWindow->hInstance;
        osWindow->windowClass.hCursor       = LoadCursorA(nullptr, IDC_ARROW);
        osWindow->windowClass.lpfnWndProc   = windowProc;
        osWindow->windowClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);

        HICON icon = LoadIconA(osWindow->hInstance, MAKEINTRESOURCEA(APP_ICON));
        if (!icon)
            logWarning(
                "%sWINDOW%s: Failed to load icon: %lu",
                FR_LOG_FORMAT_BLUE,
                FR_LOG_FORMAT_RESET,
                GetLastError());
        else {
            osWindow->windowClass.hIcon   = icon;
            osWindow->windowClass.hIconSm = icon;
        }

        logInfo("%sWINDOW%s: Window Class Registered", FR_LOG_FORMAT_BLUE, FR_LOG_FORMAT_RESET);
        logInfo("  Name: %s", className);

        if (!RegisterClassExA(&osWindow->windowClass))
            logError(
                "%sWINDOW%s: Failed to register window class: %lx",
                FR_LOG_FORMAT_BLUE,
                FR_LOG_FORMAT_RESET,
                GetLastError());
    }
    void Window::open(const WindowInfo* window_info) {
        if (osWindow->hWindow) {
            logWarning(
                "%sWINDOW%s: open() called after open", FR_LOG_FORMAT_BLUE, FR_LOG_FORMAT_RESET);
            return;
        }

        if (!window_info) windowInfo = {};
        else windowInfo = *window_info;
        memcpy(windowTitle, windowInfo.title, 128);
        windowTitle[127] = 0;

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
                    logWarning(
                        "%sWINDOW%s: Failed to adjust window rect: %lx",
                        GetLastError(),
                        FR_LOG_FORMAT_BLUE,
                        FR_LOG_FORMAT_RESET);
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
                logWarning(
                    "%sWINDOW%s: Invalid window style", FR_LOG_FORMAT_BLUE, FR_LOG_FORMAT_RESET);
                style |= WS_OVERLAPPEDWINDOW;
                if (!AdjustWindowRect(&rect, style, FALSE))
                    logWarning(
                        "%sWINDOW%s: Failed to adjust window rect: %lx",
                        FR_LOG_FORMAT_BLUE,
                        FR_LOG_FORMAT_RESET,
                        GetLastError());
        }

        osWindow->hWindow = CreateWindowExA(
            0,
            className,
            windowTitle,
            style,
            rect.left,
            rect.top,
            rect.right - rect.left,
            rect.bottom - rect.top,
            nullptr,
            nullptr,
            osWindow->hInstance,
            this);
        if (!osWindow->hWindow)
            logError(
                "%sWINDOW%s: Failed to create window: %lx",
                FR_LOG_FORMAT_BLUE,
                FR_LOG_FORMAT_RESET,
                GetLastError());
        logInfo("%sWINDOW%s: Window Created", FR_LOG_FORMAT_BLUE, FR_LOG_FORMAT_RESET);
        logInfo("  Title: %s", windowTitle);
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
        logInfo("%sWINDOW%s: Window Destroyed", FR_LOG_FORMAT_BLUE, FR_LOG_FORMAT_RESET);
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
            logWarning(
                "%sWINDOW%s: Failed to get cursor position: %lx",
                FR_LOG_FORMAT_BLUE,
                FR_LOG_FORMAT_RESET,
                GetLastError());
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
        memcpy(windowTitle, title, 128);
        windowTitle[127] = 0;
        if (!SetWindowTextA(osWindow->hWindow, windowTitle))
            logWarning(
                "%sWINDOW%s: Failed to set window title: %lx",
                GetLastError(),
                FR_LOG_FORMAT_BLUE,
                FR_LOG_FORMAT_RESET);
        logInfo("%sWINDOW%s: Window Title Updated", FR_LOG_FORMAT_BLUE, FR_LOG_FORMAT_RESET);
        logInfo("  Title: %s", title);
    }
    void Window::setWindowPos(const i32 x, const i32 y) {
        if (!SetWindowPos(
                osWindow->hWindow, HWND_TOP, x, y, windowInfo.width, windowInfo.height, 0))
            logWarning(
                "%sWINDOW%s: Failed to set window pos: %lx",
                GetLastError(),
                FR_LOG_FORMAT_BLUE,
                FR_LOG_FORMAT_RESET);
        windowInfo.x = x;
        windowInfo.y = y;
        logInfo("%sWINDOW%s: Window Position Updated", FR_LOG_FORMAT_BLUE, FR_LOG_FORMAT_RESET);
        logInfo("  Position: (%i, %i)", windowInfo.x, windowInfo.y);
    }
    void Window::setWindowSize(const i32 width, const i32 height) {
        if (!SetWindowPos(
                osWindow->hWindow, HWND_TOP, windowInfo.x, windowInfo.y, width, height, 0))
            logWarning(
                "%sWINDOW%s: Failed to set window size: %lx",
                FR_LOG_FORMAT_BLUE,
                FR_LOG_FORMAT_RESET,
                GetLastError());
        windowInfo.width  = width;
        windowInfo.height = height;
        logInfo("%sWINDOW%s: Window Size Updated", FR_LOG_FORMAT_BLUE, FR_LOG_FORMAT_RESET);
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
                logWarning(
                    "%sWINDOW%s: Failed to get window rect: %lx",
                    FR_LOG_FORMAT_BLUE,
                    FR_LOG_FORMAT_RESET,
                    GetLastError());
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
            default:
                logWarning(
                    "%sWINDOW%s: Invalid window style", FR_LOG_FORMAT_BLUE, FR_LOG_FORMAT_RESET);
                return;
        }

        if (!SetWindowLongPtr(osWindow->hWindow, GWL_STYLE, style))
            logError(
                "%sWINDOW%s: Failed to set window style: %lx",
                FR_LOG_FORMAT_BLUE,
                FR_LOG_FORMAT_RESET,
                GetLastError());
        if (!SetWindowPos(
                osWindow->hWindow,
                HWND_TOP,
                rect.left,
                rect.top,
                rect.right - rect.left,
                rect.bottom - rect.top,
                SWP_FRAMECHANGED | SWP_NOZORDER))
            logWarning(
                "%sWINDOW%s: Failed to set window pos: %lx",
                FR_LOG_FORMAT_BLUE,
                FR_LOG_FORMAT_RESET,
                GetLastError());
        if (!UpdateWindow(osWindow->hWindow))
            logWarning(
                "%sWINDOW%s: Failed to update window: %lx",
                FR_LOG_FORMAT_BLUE,
                FR_LOG_FORMAT_RESET,
                GetLastError());
        if (!InvalidateRect(osWindow->hWindow, nullptr, TRUE))
            logWarning(
                "%sWINDOW%s: Failed to invalidate window: %lx",
                FR_LOG_FORMAT_BLUE,
                FR_LOG_FORMAT_RESET,
                GetLastError());

        windowInfo.style = window_style;
        logInfo("%sWINDOW%s: Window Style Updated", FR_LOG_FORMAT_BLUE, FR_LOG_FORMAT_RESET);
    }
    const char* Window::getWindowTitle() const { return windowTitle; }
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
