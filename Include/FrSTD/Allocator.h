#ifndef FROGENGINE_ARENA_H
#define FROGENGINE_ARENA_H

#include <stdlib.h>

#include "Utility.h"

namespace FrSTD::Allocators {
    class Arena {
      public:
        explicit Arena(const usize _size) : size(_size) {
            buffer = malloc(size + 15);
            if (!buffer) exit(-__LINE__);
            next = (uptr)buffer + 15 & ~15;
        }
        ~Arena() { free(buffer); }

        ptr allocate(const usize _size) {
            if (!buffer || next + _size > (uptr)buffer + size + 15) exit(-__LINE__);
            const ptr result  = (ptr)next;
            next             += _size;
            return result;
        }

        void reset(const usize _size) {
            destroy();
            size   = _size;
            buffer = malloc(size + 15);
            if (!buffer) exit(-__LINE__);
            next = (uptr)buffer + 15 & ~15;
        }
        void destroy() {
            free(buffer);
            buffer = nullptr;
        }

      private:
        ptr   buffer { nullptr };
        usize size {};
        uptr  next {};
    };
     

    template <typename T>
    class Stack {
      public:
        explicit Stack(const usize _size) : bufferSize(_size) {
            buffer = malloc(bufferSize + 15);
            if (!buffer) exit(-__LINE__);
            stack       = (T*)((uptr)buffer + 15 & ~15);
            size        = 0;
            *(u8*)stack = 0;
        }
        explicit Stack(const usize _size, const f32 _growth) : growth(_growth), bufferSize(_size) {
            buffer = malloc(bufferSize + 15);
            if (!buffer) exit(-__LINE__);
            stack                            = (T*)((uptr)buffer + 15 & ~15);
            size                             = 0;
            *(u8*)(stack + size * sizeof(T)) = 0;
        }
        ~Stack() { free(buffer); }

        T &operator[](usize i) {
            if (!buffer || i >= size) exit(-__LINE__);
            return stack[i];
        }
        const T &operator[](usize i) const {
            if (!buffer || i >= size) exit(-__LINE__);
            return stack[i];
        }

        T* data() const { return stack; }

        void push(const T value) {
            if (!buffer) exit(-__LINE__);
            if (size * sizeof(T) >= bufferSize) {
                realloc(buffer, bufferSize * growth);
                bufferSize *= growth;
            }
            stack[size++]                    = value;
            *(u8*)(stack + size * sizeof(T)) = 0;
        }
        T pop() {
            if (!buffer) exit(-__LINE__);
            if (size == 0) return {};
            T result                         = stack[--size];
            *(u8*)(stack + size * sizeof(T)) = 0;
            return result;
        }

        void reset(const usize _size) {
            destroy();
            bufferSize = _size;
            buffer     = malloc(bufferSize + 15);
            if (!buffer) exit(-__LINE__);
            stack       = (T*)((uptr)buffer + 15 & ~15);
            size        = 0;
            *(u8*)stack = 0;
        }
        void load(const T* _buffer, const usize _buffer_size, const usize _copy_size) {
            destroy();
            bufferSize = _buffer_size;
            buffer     = malloc(bufferSize + 15);
            if (!buffer) exit(-__LINE__);
            stack = (T*)((uptr)buffer + 15 & ~15);
            size  = 0;
            memcpy(buffer, _buffer, _copy_size);
        }
        void destroy() {
            free(buffer);
            buffer = nullptr;
        }

      private:
        ptr   buffer { nullptr };
        T*    stack { nullptr };
        f32   growth { 1.5 };
        usize size {};
        usize bufferSize {};
    };
}

#endif
