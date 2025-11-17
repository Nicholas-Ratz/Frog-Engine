#include <FrogEngine/Allocator.h>
#include <FrogEngine/Log.h>
#include <FrogEngine/Save.h>
#include <FrogEngine/Window.h>

using namespace FrogEngine;

int main() {
    Allocator allocator;
    allocator.init("FROGENGINE-EXAMPLE");

    Save save(&allocator);
    save.init();

    Window window(&allocator);
    window.init("FROG-GAME");

    constexpr WindowInfo WINDOW_INFO {
        .title = "Example Game",
        .style = WINDOWED,
    };
    window.open(&WINDOW_INFO);

    window.startTextInput();

    while (window.pollEvents()) {
        if (window.getKeyPress() & KEY_ESCAPE) {
            window.close();
            break;
        }
        if (window.getKeyPress() & KEY_ENTER) window.setWindowTitle(window.getText());
        if (window.getKeyPress() & INPUT_ANY) logInfo("%s", (char*)window.getText());
    }

    window.stopTextInput();

    return 0;
}
