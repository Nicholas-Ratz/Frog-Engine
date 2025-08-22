#include <stdio.h>
#include <stdlib.h>

#include <FrogEngine/Window.h>

#include "../Source/FrWindow/Windows/Windows.h"

int main() {
    FrogEngine::Window               window {};
    constexpr FrogEngine::WindowInfo WINDOW_INFO {
        .title = "Example Game",
        .style = FrogEngine::WINDOWED,
    };

    window.init(&WINDOW_INFO);

    window.startTextInput();

    while (window.pollEvents()) {
        system("cls");
        if (window.getKeyPress() & FrogEngine::KEY_ESCAPE) window.close();
        printf(
            "Key: %llx, %llx, %llx\n",
            window.getKeyPress(),
            window.getKeyDown(),
            window.getKeyRelease());
        printf(
            "Special: %llx, %llx, %llx\n",
            window.getSpecialKeyPress(),
            window.getSpecialKeyDown(),
            window.getSpecialKeyRelease());
        printf(
            "Mouse: %x, %x, %x\n",
            window.getMousePress(),
            window.getMouseDown(),
            window.getMouseRelease());
        int x;
        int y;
        window.getMousePos(&x, &y);
        printf("(%i, %i)\n", x, y);
        printf("Text: %s\n", window.getText());

        Sleep(1);
    }

    window.stopTextInput();

    return 0;
}
