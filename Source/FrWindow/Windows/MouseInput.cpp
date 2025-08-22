#include <FrogEngine/Utility.h>

#ifdef FR_WINDOWS
#include <FrogEngine/Window.h>

namespace FrogEngine {
    void Window::getMousePos(i32* x, i32* y) const {
        *x = mouseX;
        *y = mouseY;
    }

    void Window::handleMouseEvents(const u64 bit, const bool isDown) {
        if (isDown) {
            mousePress |= bit & ~mouseDown;
            mouseDown  |= bit;
            return;
        }
        mouseRelease |= bit;
        mouseDown    &= ~bit;
    }
}

#endif
