#include <FrogEngine/Utility.h>

#ifdef FR_OS_WINDOWS
#    include <Windows.h>

#    include <FrogEngine/Log.h>
#    include <FrogEngine/Window.h>

namespace FrogEngine {
    void Window::startTextInput() {
        textInputEnabled = true;
        logInfo("%sWINDOW%s: Text Input Activated", FR_LOG_FORMAT_BLUE, FR_LOG_FORMAT_RESET);
    }
    void Window::stopTextInput() {
        textInputEnabled = false;
        logInfo("%sWINDOW%s: Text Input Deactivated", FR_LOG_FORMAT_BLUE, FR_LOG_FORMAT_RESET);
    }
    void Window::loadTextInput(const char* text, const u32 size) {
        if (size < 1'024) {
            memcpy(textInput, text, size);
            textInput[size] = '\0';
            textIndex       = size;
        } else {
            memcpy(textInput, text, 1'024);
            logWarning(
                "%sWINDOW%s: New text buffer larger than 1024",
                FR_LOG_FORMAT_BLUE,
                FR_LOG_FORMAT_RESET);
            textInput[1'023] = '\0';
            textIndex        = 1'024;
        }
        logInfo("%sWINDOW%s: Text Input Loaded", FR_LOG_FORMAT_BLUE, FR_LOG_FORMAT_RESET);
    }
    void Window::clearTextInput() {
        memset(textInput, 0, 1'024);
        logInfo("%sWINDOW%s: Text Input Cleared", FR_LOG_FORMAT_BLUE, FR_LOG_FORMAT_RESET);
    }
    const Pointer<char> Window::getText() const { return textInput; }
    usize               Window::getTextLength() const { return textIndex; }
    bool                Window::isTextInputEnabled() const { return textInputEnabled; }

    void Window::handleTextEvents(const u32 character) {
        if (!textInputEnabled) return;

        const WCHAR wide_character = (WCHAR)character;
        char        char_character;
        WideCharToMultiByte(CP_UTF8, 0, &wide_character, 1, &char_character, 1, nullptr, nullptr);

        switch (char_character) {
            case '\r': {
                if (textIndex >= 1'024) {
                    logWarning(
                        "%sWINDOW%s: Text input cannot be more than 1024 characters",
                        FR_LOG_FORMAT_BLUE,
                        FR_LOG_FORMAT_RESET);
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
                    logWarning(
                        "%sWINDOW%s: Text input cannot be more than 1024 characters",
                        FR_LOG_FORMAT_BLUE,
                        FR_LOG_FORMAT_RESET);
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
