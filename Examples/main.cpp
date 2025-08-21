#include <FrogEngine/Window.h>

int main() {
    FrogEngine::Window               window {};
    constexpr FrogEngine::WindowInfo WINDOW_INFO {
        .title = "Example Game",
        .style = FrogEngine::WINDOWED,
    };

    window.init(&WINDOW_INFO);

    while (window.pollEvents())
        if (window.getKeyPress() & FrogEngine::KEY_ESCAPE) window.close();
    return 0;
}
