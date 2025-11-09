#ifndef FROGENGINE_SAVE_H
#define FROGENGINE_SAVE_H

#include <FrogEngine/Pointer.h>
#include <FrogEngine/Utility.h>

namespace FrogEngine {
    class Allocator;
    class Block;

    struct EngineCache {
        u32   version { 2 };
        usize allocatorCache[3] { 3'072, 1'024, 2'048 };
    };

    class FROGENGINE_EXPORT Save {
      public:
        Save(Allocator* allocator);
        ~Save();

        void init();

      private:
        Block* block {};

        u32           id {};
        Pointer<char> configPath;
        Pointer<char> filePath;
        EngineCache   engineCache {};

        ptr   data { nullptr };
        usize dataSize {};
    };
}

#endif
