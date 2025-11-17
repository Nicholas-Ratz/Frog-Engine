#ifndef FROGENGINE_SAVE_H
#define FROGENGINE_SAVE_H

#include <FrogEngine/Pointer.h>
#include <FrogEngine/Utility.h>

namespace FrogEngine {
    class Allocator;
    class StaticBlock;

    struct EngineCache {
        u32   version { 1 };
        usize allocatorCache { 1'024 };
    };

    class FROGENGINE_EXPORT Save {
      public:
        Save(Allocator* allocator);
        ~Save();

        void init();

      private:
        StaticBlock* block {};

        u32           id {};
        Pointer<char> configPath;
        Pointer<char> filePath;
        EngineCache   engineCache {};

        ptr   data { nullptr };
        usize dataSize {};
    };
}

#endif
