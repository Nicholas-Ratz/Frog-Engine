#include <FrogEngine/Save.h>
#include <FrogEngine/Utility.h>

u64 generateHash(const char* name) {
    u64 hash = 5'381;
    u8  c;

    while ((c = *name++)) hash = ((hash << 5) + hash) + c;

    return hash;
}

namespace FrogEngine {
    Save::Save() {}
    Save::~Save() {}

    void Save::init(char* name) { id = generateHash(name); }
}
