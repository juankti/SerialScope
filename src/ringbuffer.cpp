#include "ringbuffer.h"

ringbuffer::ringbuffer(size_t size) : m_size(size), m_head(0), m_count(0){
    m_buffer.resize(size,0.0);
}

void ringbuffer::push(double value){
    std::lock_guard<std::mutex> lock(m_mutex);
    m_buffer[m_head]=value;
    m_head = (1+m_head)%m_size;
    if (m_count < m_size) m_count++;
}

void ringbuffer::push(const double* data, size_t count) {
    std::lock_guard<std::mutex> lock(m_mutex);
    for (size_t i = 0; i < count; ++i) {
        m_buffer[m_head] = data[i];
        m_head = (m_head + 1) % m_size;
        if (m_count < m_size) m_count++;
    }
}

std::vector<double> ringbuffer::getLast(size_t count) const{
    std::lock_guard<std::mutex> lock(m_mutex);
    std::vector<double> result;

    if (count> m_size) count = m_size;
    result.reserve(count);

    size_t current = m_head;
    for (size_t i =0;i<count;i++){
        if (current==0) current = m_size;
        current--;
        result.push_back(m_buffer[current]);
    } //data is in order newest -> oldest


    return std::vector<double>(result.rbegin(),result.rend()); //and is returned reversed
}

std::vector<double> ringbuffer::getAll() const {
    std::lock_guard<std::mutex> lock(m_mutex);
    std::vector<double> result;
    result.reserve(m_count);
    
    size_t start = (m_count == m_size) ? m_head : 0;
    for (size_t i = 0; i < m_count; ++i) {
        result.push_back(m_buffer[(start + i) % m_size]);
    }
    return result;
}

void ringbuffer::clear(){
    std::lock_guard<std::mutex> lock(m_mutex);
    m_head=0;
    m_count=0;
}

std::vector<double> ringbuffer::getAndClear() {
    std::lock_guard<std::mutex> lock(m_mutex);
    std::vector<double> result;
    result.reserve(m_count);
    
    size_t start = (m_count == m_size) ? m_head : 0;
    for (size_t i = 0; i < m_count; ++i) {
        result.push_back(m_buffer[(start + i) % m_size]);
    }
    m_head = 0;
    m_count = 0;
    return result;
}
