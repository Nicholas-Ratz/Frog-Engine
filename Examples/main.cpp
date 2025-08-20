#include <FrogEngine/Window.h>

int main() {
    FrogEngine::Window               window {};
    constexpr FrogEngine::WindowInfo WINDOW_INFO {};
    window.init(&WINDOW_INFO);

    while (window.pollEvents())
        if (window.keyPress & FrogEngine::KEY_ESCAPE) window.close();

    return 0;
}
