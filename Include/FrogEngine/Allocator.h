#ifndef FROGENGINE_ALLOCATOR_H
#define FROGENGINE_ALLOCATOR_H

#include <FrogEngine/Pointer.h>
#include <FrogEngine/Utility.h>

namespace FrogEngine {
    class Save;
    class Allocator;
    class Block;

    class Block {
      public:
        Block(Allocator* _allocator);
        ~Block();

        void init(ptr _buffer, usize _size);
        void check();

        Pointer<u8> alloc(usize _size);

        void setBuffer(ptr _buffer, usize _size);

        const ptr getBuffer() const;

      private:
        Allocator* allocator { nullptr };

        ptr   buffer { nullptr };
        usize size {};
        uptr  index {};
    };

    class FROGENGINE_MODULE Allocator {
      public:
        Allocator();
        ~Allocator();

        void init(const char* name);
        void abort();
        void check();

        u32    getID();
        Block* getSaveBlock();
        Block* getWindowBlock();

      private:
        u32 id {};

        ptr   buffer { nullptr };
        usize size {};

        usize saveSize {};
        Block saveBlock;
        usize windowSize {};
        Block windowBlock;
    };
}

#endif
