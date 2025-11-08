#ifndef FROGENGINE_POINTER_H
#define FROGENGINE_POINTER_H

#include <FrogEngine/Utility.h>

namespace FrogEngine {
    class Block;
    struct OsWindow;

    template <typename T>
    class Pointer {
      public:
        Pointer() = default;
        explicit Pointer(usize _offset, uptr* _base) : offset(_offset), base(_base) {}
        ~Pointer() = default;

        template <typename U>
        Pointer(const Pointer<U> &other) : offset(other.offset), base(other.base) {}

        T* get() const { return (T*)(offset + *base); }

        T &operator*() const { return *get(); }
        T* operator->() const { return get(); }

        Pointer operator+(usize n) const { return Pointer(offset + n, base); }
        Pointer operator-(usize n) const { return Pointer(offset - n, base); }

        T &operator[](usize n) const { return get()[n]; }

        operator T*() const { return get(); }
        operator bool() const { return get() != nullptr; }

        usize offset {};
        uptr* base {};
    };
}

#endif
