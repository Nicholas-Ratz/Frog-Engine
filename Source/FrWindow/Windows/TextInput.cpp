#include <FrogEngine/Utility.h>

#ifdef FR_WINDOWS
#include <Windows.h>

#include <FrogEngine/Log.h>
#include <FrogEngine/Window.h>

#include "Windows.h"

namespace FrogEngine {
    void Window::startTextInput() { textInputEnabled = true; }
    void Window::stopTextInput() { textInputEnabled = false; }
    void Window::loadTextInput(const char* text, const u32 length) {
        free(textInput);
        textAllocated = TEXT_INPUT_ALIGNMENT * (length / TEXT_INPUT_ALIGNMENT + 1);
        textInput     = (char*)malloc(textAllocated);
        if (!textInput) LogError("Failed to allocate text input");
        strcpy_s(textInput, length, text);
        textIndex            = length;
        textInput[textIndex] = 0;
    }
    void Window::clearTextInput() {
        free(textInput);
        textAllocated = TEXT_INPUT_ALIGNMENT;
        textInput     = (char*)malloc(TEXT_INPUT_ALIGNMENT * sizeof(char));
        if (!textInput) LogError("Failed to allocate text input");
        textIndex    = 0;
        textInput[0] = 0;
    }
    void Window::setTextIndex(const u32 index) {
        if (index >= textAllocated) return;
        textIndex = index;
    }
    const char* Window::getText() const { return textInput; }
    u32         Window::getTextIndex() const { return textIndex; }
    bool        Window::isTextInputEnabled() const { return textInputEnabled; }

    void Window::handleTextEvents(const char character) {
        if (!textInputEnabled) return;
        if (textIndex >= textAllocated - 1) {
            textAllocated += TEXT_INPUT_ALIGNMENT;
            textInput      = (char*)realloc(textInput, textAllocated);
            if (!textInput) LogError("Failed to reallocate text input");
            textInput[textAllocated] = 0;
        }
        switch (character) {
            case '\r': {
                textInput[textIndex++] = '\n';
                textInput[textIndex]   = 0;
                break;
            }
            case '\b': {
                if (textIndex > 0) textInput[--textIndex] = 0;
                break;
            }
            default: {
                textInput[textIndex++] = character;
                textInput[textIndex]   = 0;
                break;
            }
        }
    }
}

#endif
