#include <string.h>

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
        if (size < 1'023) {
            memcpy(textInput, text, size);
            textInput[size] = '\0';
            textIndex       = size;
        } else if (size >= 1'024) {
            memcpy(textInput, text, 1'024);
            logWarning("WINDOW: New text buffer larger than 1024");
            textInput[1'024] = '\0';
            textIndex        = 1'024;
        }
        logInfo("WINDOW: Text Input Loaded");
    }
    void Window::clearTextInput() {
        memset(textInput, 0, 1'024);
        logInfo("WINDOW: Text Input Cleared");
    }
    const char* Window::getText() const { return textInput; }
    usize       Window::getTextLength() const { return textIndex; }
    bool        Window::isTextInputEnabled() const { return textInputEnabled; }

    void Window::handleTextEvents(const u32 character) {
        if (!textInputEnabled) return;

        const WCHAR wide_character = (WCHAR)character;
        char        char_character;
        WideCharToMultiByte(CP_UTF8, 0, &wide_character, 1, &char_character, 1, nullptr, nullptr);

        switch (char_character) {
            case '\r': {
                if (textIndex >= 1'024) {
                    logWarning("WINDOW: Text input cannot be more than 1024 characters");
                    break;
                }
                textInput[textIndex] = '\n';
                textIndex++;
                break;
            }
            case '\b': {
                if (textIndex > 0) textIndex--;
                textInput[textIndex] = '\0';
                break;
            }
            default: {
                if (textIndex >= 1'024) {
                    logWarning("WINDOW: Text input cannot be more than 1024 characters");
                    break;
                }
                textInput[textIndex] = char_character;
                textIndex++;
                break;
            }
        }
    }
}

#endif
