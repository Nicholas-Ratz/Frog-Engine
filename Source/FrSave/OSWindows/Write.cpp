#include <FrogEngine/Utility.h>

#ifdef FR_OS_WINDOWS

#    include <FrogEngine/Save.h>

namespace FrogEngine {
    void Save::save(const ptr data, const usize length) {}
    void Save::write(const char* file) {}
}

#endif
