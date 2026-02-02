#include "ringbuffer.h"

ringbuffer::ringbuffer(size_t size) : m_size(size), m_head(0){
    m_buffer.resize(size,0.0);
}

void ringbuffer::push(double value){
    std::lock_guard<std::mutex> lock(m_mutex);
    m_buffer[m_head]=value;
    m_head = (1+m_head)%m_size;
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

std::vector<double> ringbuffer::getAll() const{
    return getLast(m_size);
}

void ringbuffer::clear(){
    std::lock_guard<std::mutex> lock(m_mutex);
    std::fill(m_buffer.begin(), m_buffer.end(),0.0);
    m_head=0;
}
