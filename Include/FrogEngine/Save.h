#ifndef FROGENGINE_SAVE_H
#define FROGENGINE_SAVE_H

#include <FrogEngine/Utility.h>

namespace FrogEngine {
    struct EngineData {};

    class Save {
      public:
        Save();
        ~Save();

        void init(char* name);
        void clear();

        void load(const ptr data, usize length);
        void read(const char* file);

        void save(const ptr data, usize length);
        void write(const char* file);

      private:
        u64        id {};
        ptr        data { nullptr };
        usize      length {};
        EngineData engineData {};
    };

    static Save save;
}

#endif
