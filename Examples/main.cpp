#include <stdio.h>

#include <FrogEngine/Window.h>

int main() {
    FrogEngine::Window               window("EXAMPLE_CLASS");
    constexpr FrogEngine::WindowInfo WINDOW_INFO {
        .title = "Example Game",
        .style = FrogEngine::WINDOWED,
    };
    window.init(&WINDOW_INFO);

    window.startTextInput();

    while (window.pollEvents()) {
        if (window.getKeyPress() & FrogEngine::KEY_ESCAPE) {
            window.close();
            break;
        }
        if (window.getKeyPress() & FrogEngine::INPUT_ANY) printf("%s\n", window.getText());
    }

    window.stopTextInput();

    return 0;
}
