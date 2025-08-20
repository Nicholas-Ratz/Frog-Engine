#include <FrogEngine/Utility.h>

#ifdef FR_WINDOWS
#include <stdio.h>
#include <windows.h>

#include <FrogEngine/Log.h>
#include <FrogEngine/Window.h>

#include "KeyMapping.h"

LRESULT CALLBACK windowProc(
    const HWND h_window, const UINT u_message, const WPARAM w_param, const LPARAM l_param) {
    FrogEngine::Window* window = nullptr;

    if (u_message == WM_NCCREATE) {
        const auto cs = (CREATESTRUCTW*)(l_param);
        window        = (FrogEngine::Window*)(cs->lpCreateParams);

        SetWindowLongPtrW(h_window, GWLP_USERDATA, (LONG_PTR)window);
    } else {
        window = (FrogEngine::Window*)(GetWindowLongPtrW(h_window, GWLP_USERDATA));
    }

    switch (u_message) {
        case WM_CLOSE     : DestroyWindow(h_window); break;
        case WM_DESTROY   : PostQuitMessage(0); break;
        case WM_KEYDOWN   : window->updateKeys(w_param, true); break;
        case WM_KEYUP     : window->updateKeys(w_param, false); break;
        case WM_SYSKEYDOWN: window->updateKeys(w_param, true); break;
        case WM_SYSKEYUP  : window->updateKeys(w_param, false); break;
        default           : break;
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
        osWindow = (OsWindow*)malloc(sizeof(OsWindow));
        if (!osWindow) LogError(MALLOC_FAILURE);
        *osWindow = OsWindow {};

        osWindow->windowClass.lpszClassName = FR_WINDOW_CLASS_NAME;
        osWindow->windowClass.hInstance     = osWindow->hInstance;
        osWindow->windowClass.hIcon =
            LoadIcon(osWindow->hInstance, MAKEINTRESOURCE(FROG_ENGINE_ICON));
        osWindow->windowClass.hCursor     = LoadCursor(nullptr, IDC_ARROW);
        osWindow->windowClass.lpfnWndProc = windowProc;


        if (!RegisterClassW(&osWindow->windowClass)) LogError(WIN_REGISTER_CLASS_FAILURE);
    }


    Window::~Window() {
        if (!UnregisterClassW(FR_WINDOW_CLASS_NAME, osWindow->hInstance))
            LogError(WIN_UNREGISTER_CLASS_FAILURE);
        free(osWindow);
        osWindow = nullptr;
    }


    void Window::init(const WindowInfo* window_info) const {
        constexpr DWORD STYLE = WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU;

        RECT rect;
        rect.left   = window_info->x;
        rect.top    = window_info->y;
        rect.right  = window_info->x + window_info->width;
        rect.bottom = window_info->y + window_info->height;

        osWindow->hWindow = CreateWindowExW(
            0,
            FR_WINDOW_CLASS_NAME,
            window_info->title,
            STYLE,
            rect.left,
            rect.top,
            rect.right - rect.left,
            rect.bottom - rect.top,
            nullptr,
            nullptr,
            osWindow->hInstance,
            (LPVOID)this);
        if (!osWindow->hWindow) LogError(WIN_CREATE_WINDOW_FAILURE);
        ShowWindow(osWindow->hWindow, SW_SHOW);
    }


    void Window::close() const { DestroyWindow(osWindow->hWindow); }


    bool Window::pollEvents() {
        MSG msg {};
        keyPress   = 0;
        keyRelease = 0;

        while (PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) return false;
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }

        return true;
    }


    void Window::updateKeys(const u64 key, const bool isDown) {
        u64 bit { 1 };

        if (key == VK_CONTROL) bit = getModifierBits(VK_LCONTROL, 48, &keyDown);
        else if (key == VK_SHIFT) bit = getModifierBits(VK_LSHIFT, 50, &keyDown);
        else if (key == VK_MENU) bit = getModifierBits(VK_LMENU, 52, &keyDown);
        else if (key < 256 && KEY_MAP[key]) bit <<= KEY_MAP[key];

        if (isDown) {
            keyPress |= bit & ~keyDown;
            keyDown  |= bit;
        } else {
            keyRelease |= bit;
            keyDown    &= ~bit;
        }
    }
}

#endif
