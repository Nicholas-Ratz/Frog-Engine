#include <FrogEngine/Utility.h>

#ifdef FR_OS_WINDOWS

#    include <FrogEngine/Save.h>

namespace FrogEngine {
    void Save::load(const ptr data, const usize length) {}
    void Save::read(const char* file) {}
}

#endif
