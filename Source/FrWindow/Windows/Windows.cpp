#include <FrogEngine/Utility.h>
#include <FrogEngine/Window.h>

#include <windows.h>

constexpr char FR_CLASS_NAME[]{"FrWindowClass"};

#ifdef FR_WINDOWS

namespace FrogEngine {
    struct OSWindow {
        WNDCLASS windowClass{};

        HINSTANCE hInstance{GetModuleHandle(nullptr)};
        HWND hWindow{};
    };

    Window::Window() {
        osWindow = new OSWindow();

        osWindow->windowClass.lpszClassName = TEXT(FR_CLASS_NAME);
        osWindow->windowClass.hInstance = osWindow->hInstance;
        osWindow->windowClass.hIcon = LoadIcon(nullptr, IDI_WINLOGO);
        osWindow->windowClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
        osWindow->windowClass.lpfnWndProc = nullptr; // TODO

        RegisterClass(&osWindow->windowClass);
    }

    Window::~Window() { delete osWindow; }

    bool Window::init(const WindowInfo *window_info) {
        constexpr DWORD STYLE = WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU;

        RECT rect;
        rect.left = window_info->x;
        rect.top = window_info->y;
        rect.right = window_info->x + window_info->width;
        rect.bottom = window_info->y + window_info->height;

        AdjustWindowRect(&rect, STYLE, false);

        return true;
    }
}

#endif
