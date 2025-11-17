#ifndef FROGENGINE_POINTER_H
#define FROGENGINE_POINTER_H

#if defined(FR_DEBUG) || defined(FR_MEMORY_SAFE)
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
        explicit Pointer(
            usize _offset, uptr* _base, usize _size, ptr* _buffer, usize _negativeOffset) :
            offset(_offset),
            base(_base)
#if defined(FR_DEBUG) || defined(FR_MEMORY_SAFE)
            ,
            size(_size),
            buffer(_buffer),
            negativeOffset(_negativeOffset)
#endif
        {
        }
        ~Pointer() = default;

        template <typename U>
        Pointer(const Pointer<U> &other) :
            offset(other.getOffset()),
            base(other.getBase())
#if defined(FR_DEBUG) || defined(FR_MEMORY_SAFE)
            ,
            size(other.getSize()),
            buffer(other.getBuffer()),
            negativeOffset(other.getNegativeOffset())
#endif
        {
        }

        T* get() const { return (T*)(offset + *base); }

        T &operator*() const { return *get(); }
        T* operator->() const { return get(); }

        Pointer operator+(usize n) const {
#if defined(FR_DEBUG) || defined(FR_MEMORY_SAFE)
            if (n > size) {
                free(*buffer);
                logError(
                    "%sALLOCATOR%s: Tried to access out of bounds\n[ERROR]   This error will not "
                    "be checked for in release",
                    FR_LOG_FORMAT_YELLOW,
                    FR_LOG_FORMAT_RESET);
            } else return Pointer(offset + n, base, size - n, buffer, negativeOffset + n);
#else
            return Pointer(offset + n, base, 0, nullptr, 0);
#endif
        }
        Pointer operator-(usize n) const {
#if defined(FR_DEBUG) || defined(FR_MEMORY_SAFE)
            if (n > negativeOffset) {
                free(*buffer);
                logError(
                    "%sALLOCATOR%s: Tried to access out of bounds\n[ERROR]   This error will not "
                    "be checked for in release",
                    FR_LOG_FORMAT_YELLOW,
                    FR_LOG_FORMAT_RESET);
            } else return Pointer(offset - n, base, size + n, buffer, negativeOffset - n);
#else
            return Pointer(offset - n, base, 0, nullptr, 0);
#endif
        }

        T &operator[](usize n) const {
#if defined(FR_DEBUG) || defined(FR_MEMORY_SAFE)
            if (n >= size) {
                free(*buffer);
                logError(
                    "%sALLOCATOR%s: Tried to access out of bounds\n[ERROR]   This error will not "
                    "be checked for in release",
                    FR_LOG_FORMAT_YELLOW,
                    FR_LOG_FORMAT_RESET);
                return get()[0];
            } else
#endif
                return get()[n];
        }

        operator T*() const { return get(); }
        operator bool() const { return get() != nullptr; }

        usize getOffset() const { return offset; }
        uptr* getBase() const { return base; }
#if defined(FR_DEBUG) || defined(FR_MEMORY_SAFE)
        usize getSize() const { return size; }
        ptr*  getBuffer() const { return buffer; }
        usize getNegativeOffset() const { return negativeOffset; }
#endif

      private:
        usize offset {};
        uptr* base {};

#if defined(FR_DEBUG) || defined(FR_MEMORY_SAFE)
        usize size {};
        ptr*  buffer { nullptr };
        usize negativeOffset { 0 };
#endif
    };
}

#endif
