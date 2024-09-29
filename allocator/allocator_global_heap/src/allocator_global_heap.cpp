#include "not_implemented.h"

#include "allocator_global_heap.h"

allocator_global_heap::allocator_global_heap(
    logger *logger) : _logger(logger)
{

}

allocator_global_heap::~allocator_global_heap()
{

}

allocator_global_heap::allocator_global_heap(
    allocator_global_heap &&other) noexcept
{
    _logger = std::move(other._logger);
    other._logger = nullptr;
}

allocator_global_heap &allocator_global_heap::operator=(
    allocator_global_heap &&other) noexcept
{
    if (this != &other)
    {
        _logger = std::move(other._logger);
        other._logger = nullptr;
    }
    return *this;
}

[[nodiscard]] void *allocator_global_heap::allocate(
    size_t value_size,
    size_t values_count)
{
    size_t request_size = value_size * values_count;
    debug_with_guard("Requested size:");
    void * pointer = nullptr;
    try{
        pointer = operator new(request_size * sizeof(allocator *) + sizeof(size_t));
    }
    catch (std::bad_alloc const& ex)
    {
        error_with_guard("Memory allocation problem");
        throw;
    }
    *reinterpret_cast<allocator **>(pointer) = this;
    *reinterpret_cast<size_t *>(reinterpret_cast<unsigned char *>(pointer) + sizeof(allocator *)) = request_size;
    information_with_guard("Memory was allocated: " + std::to_string(request_size));
    return (reinterpret_cast<unsigned char *>(pointer) + sizeof(allocator *) + sizeof(size_t));
}

void allocator_global_heap::deallocate(
    void *at)
{
    void * to_del = (reinterpret_cast<unsigned char *>(at) - sizeof(allocator *) - sizeof(size_t));
    if (this != *reinterpret_cast<allocator **>(to_del))
    {
        _logger->error("can't deallocate block from other instance");
        throw std::logic_error("can't deallocate block from other instance");
    }
    size_t block_size = *reinterpret_cast<size_t *>(reinterpret_cast<allocator **>(to_del) + 1);
    std::string result;
    char * byte = reinterpret_cast<char *>(to_del);
    for (size_t i = 0; i < block_size; ++i){
        result += *(byte++);
    }
    debug_with_guard("array of bytes: " + result);
    information_with_guard("memory was deallocated");
    ::operator delete(to_del);
}

inline logger *allocator_global_heap::get_logger() const
{
    return _logger;
}

inline std::string allocator_global_heap::get_typename() const noexcept
{
    return typeid(*this).name();
}