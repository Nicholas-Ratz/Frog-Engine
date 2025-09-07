#include <FrSTD/Utility.h>

#ifdef FR_WINDOWS
#include <Windows.h>

#include <FrogEngine/Window.h>

constexpr u8 KEY_MAP[256] = {
    255, 255, 255, 255, 255, 255, 255, 255, // 8
    57,  55,  255, 255, 98,  56,  255, 255, // 16
    255, 255, 255, 255, 54,  255, 255, 255, // 24
    255, 255, 255, 58,  255, 255, 255, 255, // 32
    36,  107, 109, 108, 106, 60,  62,  61,  // 40
    63,  255, 255, 255, 255, 105, 59,  255, // 48
    26,  27,  28,  29,  30,  31,  32,  33,  // 56
    34,  35,  255, 255, 255, 255, 255, 255, // 64
    255, 0,   1,   2,   3,   4,   5,   6,   // 72
    7,   8,   9,   10,  11,  12,  13,  14,  // 80
    15,  16,  17,  18,  19,  20,  21,  22,  // 88
    23,  24,  25,  255, 255, 255, 255, 255, // 96
    88,  89,  90,  91,  92,  93,  94,  95,  // 104
    96,  97,  101, 99,  255, 100, 103, 102, // 112
    64,  65,  66,  67,  68,  69,  70,  71,  // 120
    72,  73,  74,  75,  76,  77,  78,  79,  // 128
    80,  81,  82,  83,  84,  85,  86,  87,  // 136
    255, 255, 255, 255, 255, 59,  255, 255, // 144
    104, 255, 255, 255, 255, 255, 255, 255, // 152
    255, 255, 255, 255, 255, 255, 255, 255, // 160
    255, 255, 255, 255, 255, 255, 255, 255, // 168
    255, 255, 255, 255, 255, 255, 255, 255, // 176
    255, 255, 255, 255, 255, 255, 255, 255, // 184
    255, 255, 37,  47,  44,  45,  46,  38,  // 192
    39,  255, 255, 255, 255, 255, 255, 255, // 200
    255, 255, 255, 255, 255, 255, 255, 255, // 208
    255, 255, 255, 255, 255, 255, 255, 255, // 216
    255, 255, 255, 40,  41,  42,  43,  255, // 224
    255, 255, 255, 255, 255, 255, 255, 255, // 232
    255, 255, 255, 255, 255, 255, 255, 255, // 240
    255, 255, 255, 255, 255, 255, 255, 255, // 248
    255, 255, 255, 255, 255, 255, 255, 255  // 256
};

namespace FrogEngine {
    u8  Window::getMousePress() const { return mousePress; }
    u8  Window::getMouseDown() const { return mouseDown; }
    u8  Window::getMouseRelease() const { return mouseRelease; }
    u64 Window::getKeyPress() const { return keyPress; }
    u64 Window::getKeyDown() const { return keyDown; }
    u64 Window::getKeyRelease() const { return keyRelease; }
    u64 Window::getSpecialKeyPress() const { return keySpecialPress; }
    u64 Window::getSpecialKeyDown() const { return keySpecialDown; }
    u64 Window::getSpecialKeyRelease() const { return keySpecialRelease; }

    void Window::handleKeyEvents(const u64 input, const bool isDown) {
        if (input > 255 || KEY_MAP[(u8)input] == 255) return;
        u64 bit = KEY_MAP[input];

        if (bit >= 64) {
            bit = 1llu << (bit - 64);
            if (isDown) {
                keySpecialPress |= bit & ~keySpecialDown;
                keySpecialDown  |= bit;
                return;
            }
            keySpecialRelease |= bit;
            keySpecialDown    &= ~bit;
            return;
        }

        bit = 1llu << bit;
        if (isDown) {
            keyPress |= bit & ~keyDown;
            keyDown  |= bit;
            return;
        }
        keyRelease |= bit;
        keyDown    &= ~bit;
    }
}

#endif
