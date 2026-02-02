#ifndef RINGBUFFER_H
#define RINGBUFFER_H

#include <vector>
#include <mutex>

class ringbuffer{
public:
    explicit ringbuffer(size_t size);
    void push(double value);
    std::vector<double> getLast(size_t count) const;
    std::vector<double> getAll() const;
    void clear();
    size_t getSize() const {return m_size;}

private:
    std::vector<double> m_buffer;
    size_t m_size;
    size_t m_head;
    mutable std::mutex m_mutex;
};

#endif // RINGBUFFER_H
