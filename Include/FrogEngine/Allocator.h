#ifndef FROGENGINE_ALLOCATOR_H
#define FROGENGINE_ALLOCATOR_H

#include <FrogEngine/Utility.h>

namespace FrogEngine {
    class Save;
    class Allocator;

    class Block {
      public:
        Block(Allocator* _allocator);
        ~Block();

        void init(ptr _buffer, usize _size);
        void check();

        ptr alloc(usize _size);

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
