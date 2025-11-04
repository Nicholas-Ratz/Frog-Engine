#ifndef FROGENGINE_SAVE_H
#define FROGENGINE_SAVE_H

#include <FrogEngine/Utility.h>

namespace FrogEngine {
    struct EngineData {
        u32 version { 1 };
    };

    class FROGENGINE_MODULE Save {
      public:
        Save();
        ~Save();

        void init(const char* name);
        void clear();

        void load(const ptr data, usize length);
        void read(const char* file);

        void save(const ptr data, usize length);
        void write(const char* file);

      private:
        u32        id {};
        char*      configPath { nullptr };
        char*      filePath { nullptr };
        ptr        data { nullptr };
        usize      length {};
        EngineData engineData {};
    };

    static Save save;
}

#endif
