#ifndef RINGBUFFER_H
#define RINGBUFFER_H

#include <vector>
#include <mutex>

class ringbuffer{
public:
    explicit ringbuffer(size_t size);
    void push(double value);
    void push(const double* data, size_t count);
    std::vector<double> getLast(size_t count) const;
    std::vector<double> getAll() const;
    std::vector<double> getAndClear();
    void clear();
    size_t getSize() const {return m_size;}

private:
    std::vector<double> m_buffer;
    size_t m_size;
    size_t m_head;
    size_t m_count;
    mutable std::mutex m_mutex;
};

#endif // RINGBUFFER_H
