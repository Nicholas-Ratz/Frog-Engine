#include <FrogEngine/Allocator.h>
#include <FrogEngine/Log.h>
#include <FrogEngine/Pointer.h>
#include <FrogEngine/Utility.h>

namespace FrogEngine {
    StaticBlock::StaticBlock(Allocator* _allocator) : allocator(_allocator) {}
    StaticBlock::~StaticBlock() {}

    void StaticBlock::init(ptr _buffer, usize _size) {
        buffer = _buffer;
        size   = _size;
    }

    Pointer<u8> StaticBlock::alloc(usize _size) {
        if (index + _size > size) {
            logError(
                "ALLOCATOR: Tried to alloc %zu when only %zu is allocated", index + _size, size);
            allocator->abort();
        }

        logInfo("ALLOCATOR: Allocated %zu out of %zu static memory", index + _size, size);
        Pointer<u8> result(index, (uptr*)&buffer, _size, allocator->getBuffer());
        index += _size;
        return result;
    }

    const ptr StaticBlock::getBuffer() const { return buffer; }
    usize     StaticBlock::getSize() const { return size; }
}
