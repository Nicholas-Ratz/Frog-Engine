#include <FrogEngine/Allocator.h>
#include <FrogEngine/Utility.h>

namespace FrogEngine {
    DynamicBlock::DynamicBlock(Allocator* _allocator) : allocator(_allocator) {}
    DynamicBlock::~DynamicBlock() {}

    void DynamicBlock::init(ptr _buffer, usize _size) {
        buffer = _buffer;
        size   = _size;
    }
    void        DynamicBlock::resize(usize _size) {}
    Pointer<u8> alloc(usize _size) {}
    Pointer<u8> realloc(Pointer<u8> pointer, usize _old, usize _new) {}
    void        dealloc(Pointer<u8> pointer, usize _size) {}
}
