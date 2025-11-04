#include <stdio.h>

#include <FrogEngine/Allocator.h>
#include <FrogEngine/Save.h>
#include <FrogEngine/Window.h>

int main() {
    FrogEngine::Save save;
    save.init("FROG ENGINE GAME");

    FrogEngine::Window window;
    window.init("FROG-GAME");

    constexpr FrogEngine::WindowInfo WINDOW_INFO {
        .title = "Example Game",
        .style = FrogEngine::WINDOWED,
    };
    window.open(&WINDOW_INFO);

    window.startTextInput();

    while (window.pollEvents()) {
        if (window.getKeyPress() & FrogEngine::KEY_ESCAPE) {
            window.close();
            break;
        }
        if (window.getKeyPress() & FrogEngine::KEY_ENTER) window.setWindowTitle(window.getText());
        if (window.getKeyPress() & FrogEngine::INPUT_ANY) printf("%s\n", window.getText());
    }

    window.stopTextInput();

    return 0;
}
