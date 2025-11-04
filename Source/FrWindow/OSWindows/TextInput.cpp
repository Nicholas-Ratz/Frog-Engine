#include <FrogEngine/Utility.h>

#ifdef FR_OS_WINDOWS
#    include <Windows.h>

#    include <FrogEngine/Log.h>
#    include <FrogEngine/Window.h>

namespace FrogEngine {
    void Window::startTextInput() {
        textInputEnabled = true;
        logInfo("WINDOW: Text Input Activated");
    }
    void Window::stopTextInput() {
        textInputEnabled = false;
        logInfo("WINDOW: Text Input Deactivated");
    }
    void Window::loadTextInput(const char* text, const u32 size) {
        const usize buffer_size = TEXT_INPUT_BUFFER_SIZE * (size / TEXT_INPUT_BUFFER_SIZE + 1);
        textInputBuffer.load(text, buffer_size, size);
        logInfo("WINDOW: Text Input Loaded");
    }
    void Window::clearTextInput() {
        textInputBuffer.reset(TEXT_INPUT_BUFFER_SIZE);
        logInfo("WINDOW: Text Input Cleared");
    }
    const char* Window::getText() const { return textInputBuffer.data(); }
    bool        Window::isTextInputEnabled() const { return textInputEnabled; }

    void Window::handleTextEvents(const u32 character) {
        if (!textInputEnabled) return;

        const WCHAR wide_character = (WCHAR)character;
        char        char_character;
        WideCharToMultiByte(CP_UTF8, 0, &wide_character, 1, &char_character, 1, nullptr, nullptr);

        switch (char_character) {
            case '\r': textInputBuffer.push('\n'); break;
            case '\b': textInputBuffer.pop(); break;
            default  : textInputBuffer.push(char_character); break;
        }
    }
}

#endif
