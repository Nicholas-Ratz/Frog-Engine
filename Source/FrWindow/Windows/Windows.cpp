#include <stdio.h>
#include <windows.h>

#include <FrogEngine/Utility.h>
#include <FrogEngine/Window.h>

constexpr wchar_t FR_WINDOW_CLASS_NAME[] { L"FrWindowClass" };

#ifdef FR_WINDOWS
namespace FrogEngine {
    struct OsWindow {
        WNDCLASSW windowClass {};
        HINSTANCE hInstance { GetModuleHandle(nullptr) };
        HWND      hWindow {};
    };

    Window::Window() {
        osWindow = (OsWindow*)malloc(sizeof(OsWindow));
        if ((resultStatus |= !osWindow)) return;
        *osWindow = OsWindow {};

        osWindow->windowClass.lpszClassName = FR_WINDOW_CLASS_NAME;
        osWindow->windowClass.hInstance     = osWindow->hInstance;
        osWindow->windowClass.hIcon         = LoadIcon(nullptr, IDI_WINLOGO);
        osWindow->windowClass.hCursor       = LoadCursor(nullptr, IDC_ARROW);
        osWindow->windowClass.lpfnWndProc   = nullptr; // TODO

        resultStatus |= !RegisterClassW(&osWindow->windowClass);
    }

    Window::~Window() {
        UnregisterClassW(FR_WINDOW_CLASS_NAME, osWindow->hInstance);

        free(osWindow);
        osWindow = nullptr;
    }

    bool Window::init(const WindowInfo* window_info) {
        constexpr DWORD STYLE = WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU;

        RECT rect;
        rect.left   = window_info->x;
        rect.top    = window_info->y;
        rect.right  = window_info->x + window_info->width;
        rect.bottom = window_info->y + window_info->height;

        AdjustWindowRect(&rect, STYLE, false);

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
            nullptr
        );
        if ((resultStatus |= !osWindow->hWindow)) return false;

        ShowWindow(osWindow->hWindow, SW_SHOW);
        return true;
    }

    bool Window::checkStatus() const {
        if (resultStatus) {
            const DWORD error          = GetLastError();
            LPWSTR      message_buffer = nullptr;

            FormatMessageW(
                FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
                nullptr,
                error,
                0,
                (LPWSTR)&message_buffer,
                0,
                nullptr
            );

            printf("%S\n", message_buffer);

            LocalFree(message_buffer);
            return false;
        }
        return true;
    }
}
#endif
