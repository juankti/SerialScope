#include "rawringbuffer.h"

rawringbuffer::rawringbuffer(size_t size) : m_size(size), m_head(0), m_count(0) {
    m_buffer.resize(size, 0);
}

void rawringbuffer::push(uint8_t value) {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_buffer[m_head] = value;
    m_head = (m_head + 1) % m_size;
    if (m_count < m_size) {
        m_count++;
    }
}

void rawringbuffer::push(const uint8_t* data, size_t count) {
    std::lock_guard<std::mutex> lock(m_mutex);
    for (size_t i = 0; i < count; ++i) {
        m_buffer[m_head] = data[i];
        m_head = (m_head + 1) % m_size;
        if (m_count < m_size) {
            m_count++;
        }
    }
}

std::vector<uint8_t> rawringbuffer::getAndClear() {
    std::lock_guard<std::mutex> lock(m_mutex);
    std::vector<uint8_t> result;
    if (m_count == 0) return result;

    result.reserve(m_count);
    size_t tail = (m_head + m_size - m_count) % m_size;

    if (tail < m_head) {
        result.insert(result.end(), m_buffer.begin() + tail, m_buffer.begin() + m_head);
    } else {
        result.insert(result.end(), m_buffer.begin() + tail, m_buffer.end());
        result.insert(result.end(), m_buffer.begin(), m_buffer.begin() + m_head);
    }

    m_count = 0;
    return result;
}

void rawringbuffer::clear() {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_count = 0;
    m_head = 0;
}
