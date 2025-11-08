#include <FrogEngine/Utility.h>

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
        block = allocator->getWindowBlock();

        osWindow  = block->alloc(sizeof(OsWindow));
        textInput = block->alloc(1'024);
    }

    Window::~Window() {
        if (!UnregisterClassA(className, ((OsWindow*)osWindow)->hInstance))
            logError("WINDOW: Failed to unregister window class: %lx", GetLastError());
        logInfo("WINDOW: Window Class Unregistered");
    }

    void Window::init(const char* class_name) {
        if (className[0] != 0) {
            logWarning("WINDOW: init() called after initialization");
            return;
        }

        OsWindow* os_window_ptr = ((OsWindow*)osWindow);

        if (!SetProcessDPIAware())
            logWarning("WINDOW: Failed to set DPI awareness: %lx", GetLastError());

        os_window_ptr->hInstance = GetModuleHandleA(nullptr);

        strncpy(className, class_name, 128);
        className[127] = 0;

        os_window_ptr->windowClass.cbSize        = sizeof(WNDCLASSEXA);
        os_window_ptr->windowClass.lpszClassName = className;
        os_window_ptr->windowClass.hInstance     = os_window_ptr->hInstance;
        os_window_ptr->windowClass.hCursor       = LoadCursorA(nullptr, IDC_ARROW);
        os_window_ptr->windowClass.lpfnWndProc   = windowProc;
        os_window_ptr->windowClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);

        HICON icon = LoadIconA(os_window_ptr->hInstance, MAKEINTRESOURCEA(APP_ICON));
        if (!icon) logWarning("WINDOW: Failed to load icon: %lu", GetLastError());
        else {
            os_window_ptr->windowClass.hIcon   = icon;
            os_window_ptr->windowClass.hIconSm = icon;
        }

        logInfo("WINDOW: Window Class Registered");
        logInfo("  Name: %s", className);

        if (!RegisterClassExA(&os_window_ptr->windowClass))
            logError("WINDOW: Failed to register window class: %lx", GetLastError());
    }
    void Window::open(const WindowInfo* window_info) {
        OsWindow* os_window_ptr = ((OsWindow*)osWindow);

        if (os_window_ptr->hWindow) {
            logWarning("WINDOW: open() called after open");
            return;
        }

        if (!window_info) windowInfo = {};
        else windowInfo = *window_info;

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
                    logWarning("WINDOW: Failed to adjust window rect: %lx", GetLastError());
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
                    logWarning("WINDOW: Failed to adjust window rect: %lx", GetLastError());
        }

        os_window_ptr->hWindow = CreateWindowExA(
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
            os_window_ptr->hInstance,
            this);
        if (!os_window_ptr->hWindow)
            logError("WINDOW: Failed to create window: %lx", GetLastError());
        logInfo("WINDOW: Window Created");
        logInfo("  Title: %s", window_info->title);
        logInfo("  Size: (%i, %i)", window_info->width, window_info->height);
        logInfo("  Position: (%i, %i)", window_info->x, window_info->y);

        GetClientRect(os_window_ptr->hWindow, &rect);
        windowInfo.x      = rect.left;
        windowInfo.y      = rect.top;
        windowInfo.width  = rect.right - rect.left;
        windowInfo.height = rect.bottom - rect.top;
    }
    void Window::close() const {
        DestroyWindow(((OsWindow*)osWindow)->hWindow);
        logInfo("WINDOW: Window Destroyed");
    }

    bool Window::pollEvents() {
        if (!IsWindow(((OsWindow*)osWindow)->hWindow)) return false;

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
        if (!GetCursorPos(&point) || !ScreenToClient(((OsWindow*)osWindow)->hWindow, &point))
            logWarning("WINDOW: Failed to get cursor position: %lx", GetLastError());
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
        if (!SetWindowTextA(((OsWindow*)osWindow)->hWindow, title))
            logWarning("WINDOW: Failed to set window title: %lx", GetLastError());
        logInfo("WINDOW: Window Title Updated");
        logInfo("  Title: %s", title);
    }
    void Window::setWindowPos(const i32 x, const i32 y) {
        if (!SetWindowPos(
                ((OsWindow*)osWindow)->hWindow,
                HWND_TOP,
                x,
                y,
                windowInfo.width,
                windowInfo.height,
                0))
            logWarning("WINDOW: Failed to set window pos: %lx", GetLastError());
        windowInfo.x = x;
        windowInfo.y = y;
        logInfo("WINDOW: Window Position Updated");
        logInfo("  Position: (%i, %i)", windowInfo.x, windowInfo.y);
    }
    void Window::setWindowSize(const i32 width, const i32 height) {
        if (!SetWindowPos(
                ((OsWindow*)osWindow)->hWindow,
                HWND_TOP,
                windowInfo.x,
                windowInfo.y,
                width,
                height,
                0))
            logWarning("WINDOW: Failed to set window size: %lx", GetLastError());
        windowInfo.width  = width;
        windowInfo.height = height;
        logInfo("WINDOW: Window Size Updated");
        logInfo("  Size: (%i, %i)", windowInfo.width, windowInfo.height);
    }
    void Window::setWindowStyle(const WindowStyle window_style) {
        if (windowInfo.style == window_style) return;

        OsWindow* os_window_ptr = ((OsWindow*)osWindow);

        DWORD style { WS_VISIBLE };
        RECT  rect;
        rect.left   = windowInfo.x;
        rect.top    = windowInfo.y;
        rect.right  = windowInfo.x + windowInfo.width;
        rect.bottom = windowInfo.y + windowInfo.height;
        if (windowInfo.style != FULLSCREEN) {
            if (!GetWindowRect(os_window_ptr->hWindow, &rect)) {
                logWarning("WINDOW: Failed to get window rect: %lx", GetLastError());
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

        if (!SetWindowLongPtr(os_window_ptr->hWindow, GWL_STYLE, style))
            logError("WINDOW: Failed to set window style: %lx", GetLastError());
        if (!SetWindowPos(
                os_window_ptr->hWindow,
                HWND_TOP,
                rect.left,
                rect.top,
                rect.right - rect.left,
                rect.bottom - rect.top,
                SWP_FRAMECHANGED | SWP_NOZORDER))
            logWarning("WINDOW: Failed to set window pos: %lx", GetLastError());
        if (!UpdateWindow(os_window_ptr->hWindow))
            logWarning("WINDOW: Failed to update window: %lx", GetLastError());
        if (!InvalidateRect(os_window_ptr->hWindow, nullptr, TRUE))
            logWarning("WINDOW: Failed to invalidate window: %lx", GetLastError());

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

        GetClientRect(((OsWindow*)osWindow)->hWindow, &rect);

        windowInfo.x      = rect.left;
        windowInfo.y      = rect.top;
        windowInfo.width  = rect.right - rect.left;
        windowInfo.height = rect.bottom - rect.top;
    }
}

#endif
