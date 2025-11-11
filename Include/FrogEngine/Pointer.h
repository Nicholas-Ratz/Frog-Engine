#ifndef FROGENGINE_POINTER_H
#define FROGENGINE_POINTER_H

#ifdef FR_DEBUG
#    include <stdlib.h>
#endif

#include <FrogEngine/Log.h>
#include <FrogEngine/Utility.h>

namespace FrogEngine {
    class StaticBlock;
    struct OsWindow;

    template <typename T>
    class Pointer {
      public:
        Pointer() = default;
        explicit Pointer(usize _offset, uptr* _base, usize _size, ptr* _buffer) :
            offset(_offset),
            base(_base)
#ifdef FR_DEBUG
            ,
            size(_size),
            buffer(_buffer)
#endif
        {
        }
        ~Pointer() = default;

        template <typename U>
        Pointer(const Pointer<U> &other) :
            offset(other.getOffset()),
            base(other.getBase())
#ifdef FR_DEBUG
            ,
            size(other.getSize()),
            buffer(other.getBuffer())
#endif
        {
        }

        T* get() const { return (T*)(offset + *base); }

        T &operator*() const { return *get(); }
        T* operator->() const { return get(); }

        Pointer operator+(usize n) const {
#ifdef FR_DEBUG
            if (n >= size) {
                free(*buffer);
                logError(
                    "ALLOCATOR: Tried to access out of bounds.\n[ERROR]   This error will not be "
                    "checked for in release");
            } else
#endif
                return Pointer(offset + n, base);
        }
        Pointer operator-(usize n) const {
#ifdef FR_DEBUG
            if (n >= size) {
                free(*buffer);
                logError(
                    "ALLOCATOR: Tried to access out of bounds.\n[ERROR]   This error will not be "
                    "checked for in release");
            } else
#endif
                return Pointer(offset - n, base);
        }

        T &operator[](usize n) const {
#ifdef FR_DEBUG
            if (n >= size) {
                free(*buffer);
                logError(
                    "ALLOCATOR: Tried to access out of bounds.\n[ERROR]   This error will not be "
                    "checked for in release");
                return get()[0];
            } else
#endif
                return get()[n];
        }

        operator T*() const { return get(); }
        operator bool() const { return get() != nullptr; }

        usize getOffset() const { return offset; }
        uptr* getBase() const { return base; }
#ifdef FR_DEBUG
        usize getSize() const { return size; }
        ptr*  getBuffer() const { return buffer; }
#endif

      private:
        usize offset {};
        uptr* base {};

#ifdef FR_DEBUG
        usize size {};
        ptr*  buffer { nullptr };
#endif
    };
}

#endif
