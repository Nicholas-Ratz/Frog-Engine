#include <stdio.h>

#include <FrogEngine/Allocator.h>
#include <FrogEngine/Log.h>
#include <FrogEngine/Save.h>
#include <FrogEngine/Window.h>

int main() {
    FrogEngine::Allocator allocator;
    allocator.init("FROGENGINE-GAME");

    FrogEngine::Save save(&allocator);
    save.init();

    FrogEngine::Window window(&allocator);
    window.init("FROG-GAME");

    constexpr FrogEngine::WindowInfo WINDOW_INFO {
        .title = "Example Game",
        .style = FrogEngine::WINDOWED,
    };
    window.open(&WINDOW_INFO);

    window.startTextInput();

    while (window.pollEvents()) {
        allocator.check();
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
