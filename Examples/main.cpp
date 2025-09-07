#include <stdio.h>

#include <FrSTD/Allocator.h>
#include <FrogEngine/Window.h>

int main() {
    FrogEngine::Window               window;
    constexpr FrogEngine::WindowInfo WINDOW_INFO {
        .title = "Example Game",
        .style = FrogEngine::WINDOWED,
    };
    
    window.init(&WINDOW_INFO);

    window.startTextInput();

    while (window.pollEvents()) {
        if (window.getKeyPress() & FrogEngine::INPUT_ANY) printf("%s\n", window.getText());
        if (window.getKeyPress() & FrogEngine::KEY_ESCAPE) window.close();
    }

    window.stopTextInput();

    return 0;
}
