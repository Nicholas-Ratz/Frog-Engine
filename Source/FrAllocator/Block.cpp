#include <FrogEngine/Allocator.h>
#include <FrogEngine/Log.h>
#include <FrogEngine/Pointer.h>
#include <FrogEngine/Utility.h>

namespace FrogEngine {
    Block::Block(Allocator* _allocator) : allocator(_allocator) {}
    Block::~Block() {}

    void Block::init(ptr _buffer, usize _size) {
        buffer = _buffer;
        size   = _size;

        *(u32*)((uptr)buffer + size)     = 'DATA';
        *(u32*)((uptr)buffer + size + 4) = 'BLOK';
    }
    void Block::check() {
        if (*(u32*)((uptr)buffer + size) != 'DATA' || *(u32*)((uptr)buffer + size + 4) != 'BLOK') {
            allocator->abort();
            logError("ALLOCATOR: Block wrote out of bounds");
        }
    }

    Pointer<u8> Block::alloc(usize _size) {
        if (index + _size > size) {
            logError(
                "ALLOCATOR: Tried to alloc %zu when only %zu is allocated", index + _size, size);
            allocator->abort();
        }

        Pointer<u8> result(index, (uptr*)&buffer);
        index += _size;
        return result;
    }

    void Block::setBuffer(ptr _buffer, usize _size) {
        buffer = _buffer;
        size   = _size;
    }

    const ptr Block::getBuffer() const { return buffer; }
}
