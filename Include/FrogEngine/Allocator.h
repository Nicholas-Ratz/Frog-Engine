#ifndef FROGENGINE_ALLOCATOR_H
#define FROGENGINE_ALLOCATOR_H

#include <FrogEngine/Pointer.h>
#include <FrogEngine/Utility.h>

namespace FrogEngine {
    class Allocator;

    class DynamicBlock {
      public:
        DynamicBlock(Allocator* _allocator);
        ~DynamicBlock();

        void init();

      private:
        ptr   buffer { nullptr };
        usize size {};
    };

    class StaticBlock {
      public:
        StaticBlock(Allocator* _allocator);
        ~StaticBlock();

        void        init(ptr _buffer, usize _size);
        Pointer<u8> alloc(usize _size);

        const ptr getBuffer() const;
        usize     getSize() const;

      private:
        Allocator* allocator { nullptr };

        ptr   buffer { nullptr };
        usize size {};
        uptr  index {};
    };

    class FROGENGINE_EXPORT Allocator {
      public:
        Allocator();
        ~Allocator();

        void init(const char* name);
        void abort();

        u32          getID();
        ptr*         getBuffer();
        StaticBlock* getStaticBlock();

      private:
        u32 id {};

        ptr   buffer { nullptr };
        usize size {};

        usize       staticSize {};
        StaticBlock staticBlock;
    };
}

#endif
