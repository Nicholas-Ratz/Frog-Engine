#include <FrogEngine/Utility.h>

#ifdef FR_WINDOWS
#include <Windows.h>

#include <FrogEngine/Log.h>
#include <FrogEngine/Window.h>

namespace FrogEngine {
    void Window::startTextInput() {
        textInputEnabled = true;
        logInfo("Text Input Activated");
    }
    void Window::stopTextInput() {
        textInputEnabled = false;
        logInfo("Text Input Deactivated");
    }
    void Window::loadTextInput(const char* text, const u32 length) {
        free(textInput);
        textAllocated = TEXT_INPUT_ALIGNMENT * (length / TEXT_INPUT_ALIGNMENT + 1);
        textInput     = (char*)malloc(textAllocated);
        if (!textInput) logError("Failed to allocate text input");
        strcpy_s(textInput, length, text);
        textIndex            = length;
        textInput[textIndex] = 0;
        logInfo("Text Input Loaded");
    }
    void Window::clearTextInput() {
        free(textInput);
        textAllocated = TEXT_INPUT_ALIGNMENT;
        textInput     = (char*)malloc(TEXT_INPUT_ALIGNMENT * sizeof(char));
        if (!textInput) logError("Failed to allocate text input");
        textIndex    = 0;
        textInput[0] = 0;
        logInfo("Text Input Cleared");
    }
    void Window::setTextIndex(const u32 index) {
        if (index >= textAllocated) return;
        textIndex = index;
        logInfo("Text Index Set");
    }
    const char* Window::getText() const { return textInput; }
    u32         Window::getTextIndex() const { return textIndex; }
    bool        Window::isTextInputEnabled() const { return textInputEnabled; }

    void Window::handleTextEvents(const u32 character) {
        if (!textInputEnabled) return;

        const WCHAR wide_character = (WCHAR)character;
        char        char_character;
        WideCharToMultiByte(CP_UTF8, 0, &wide_character, 1, &char_character, 1, nullptr, nullptr);

        switch (char_character) {
            case '\r': {
                textInput[textIndex++] = '\n';
                if (textIndex < textAllocated) textInput[textIndex] = 0;
                break;
            }
            case '\b': {
                if (textIndex > 0) textInput[--textIndex] = 0;
                break;
            }
            default: {
                textInput[textIndex++] = char_character;
                if (textIndex < textAllocated) textInput[textIndex] = 0;
                break;
            }
        }
        if (textIndex >= textAllocated - 1) {
            textAllocated += TEXT_INPUT_ALIGNMENT;
            textInput      = (char*)realloc(textInput, textAllocated);
            if (!textInput) logError("Failed to reallocate text input");
        }
        if ((i32)textIndex < (i32)(textAllocated - TEXT_INPUT_ALIGNMENT - 1)) {
            textAllocated -= TEXT_INPUT_ALIGNMENT;
            textInput      = (char*)realloc(textInput, textAllocated);
            if (!textInput) logError("Failed to reallocate text input");
        }
    }
}

#endif
