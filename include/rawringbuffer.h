#ifndef RAWRINGBUFFER_H
#define RAWRINGBUFFER_H

#include <vector>
#include <mutex>
#include <cstdint>

class rawringbuffer {
public:
    explicit rawringbuffer(size_t size);
    void push(uint8_t value);
    void push(const uint8_t* data, size_t count);
    std::vector<uint8_t> getAndClear();
    void clear();
    size_t getSize() const { return m_size; }
    size_t getCount() const { 
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_count; 
    }

private:
    std::vector<uint8_t> m_buffer;
    size_t m_size;
    size_t m_head;
    size_t m_count;
    mutable std::mutex m_mutex;
};

#endif // RAWRINGBUFFER_H
