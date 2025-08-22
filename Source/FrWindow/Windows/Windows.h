#ifndef FROGENGINE_WINDOWS_H
#define FROGENGINE_WINDOWS_H

#include <FrogEngine/Utility.h>

#ifdef FR_WINDOWS
#include <Windows.h>

#include <FrogEngine/Window.h>

constexpr TCHAR FR_WINDOW_CLASS_NAME[] { TEXT("FR_WINDOW_CLASS") };

namespace FrogEngine {
    struct OsWindow {
        WNDCLASS  windowClass {};
        HINSTANCE hInstance { GetModuleHandle(nullptr) };
        HWND      hWindow {};
    };
}

inline LRESULT CALLBACK windowProc(
    const HWND h_window, const UINT u_message, const WPARAM w_param, const LPARAM l_param) {
    FrogEngine::Window* window = nullptr;

    if (u_message == WM_NCCREATE) {
        const auto cs = (CREATESTRUCTW*)l_param; // NOLINT
        window        = (FrogEngine::Window*)cs->lpCreateParams;

        SetWindowLongPtr(h_window, GWLP_USERDATA, (LONG_PTR)window);
    } else {
        window = (FrogEngine::Window*)GetWindowLongPtr(h_window, GWLP_USERDATA); // NOLINT
    }

    switch (u_message) {
        case WM_CLOSE      : DestroyWindow(h_window); break;
        case WM_DESTROY    : PostQuitMessage(0); break;
        case WM_CHAR       : window->handleTextEvents((WCHAR)w_param); break;
        case WM_KEYDOWN    : window->handleKeyEvents(w_param, true); break;
        case WM_KEYUP      : window->handleKeyEvents(w_param, false); break;
        case WM_SYSKEYDOWN : window->handleKeyEvents(w_param, true); return 0;
        case WM_SYSKEYUP   : window->handleKeyEvents(w_param, false); return 0;
        case WM_LBUTTONDOWN: window->handleMouseEvents(FrogEngine::MOUSE_LEFT, true); break;
        case WM_LBUTTONUP  : window->handleMouseEvents(FrogEngine::MOUSE_LEFT, false); break;
        case WM_RBUTTONDOWN: window->handleMouseEvents(FrogEngine::MOUSE_RIGHT, true); break;
        case WM_RBUTTONUP  : window->handleMouseEvents(FrogEngine::MOUSE_RIGHT, false); break;
        case WM_MBUTTONDOWN: window->handleMouseEvents(FrogEngine::MOUSE_MIDDLE, true); break;
        case WM_MBUTTONUP  : window->handleMouseEvents(FrogEngine::MOUSE_MIDDLE, false); break;
        case WM_SYSCOMMAND:
            if (w_param == SC_KEYMENU) return 0;
        default: break;
    }

    return DefWindowProc(h_window, u_message, w_param, l_param);
}

#endif
#endif
