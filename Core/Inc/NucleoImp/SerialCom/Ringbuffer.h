#ifndef RINGBUFFER_H
#define RINGBUFFER_H

#include <stdint.h>
#include <cstddef>


namespace ELE3312 {

class RingBuffer {
private:
    static const size_t BUFFER_SIZE = 256;
    uint8_t buffer[BUFFER_SIZE];
    volatile size_t head;
    volatile size_t tail;

public:
    RingBuffer() : head(0), tail(0) {}

    void write(uint8_t data) {
        size_t next_head = (head + 1) % BUFFER_SIZE;
        if (next_head != tail) {
            buffer[head] = data;
            head = next_head;
        }
    }

    bool read(uint8_t &data) {
        if (head == tail) {
            return false;
        }
        data = buffer[tail];
        tail = (tail + 1) % BUFFER_SIZE;
        return true;
    }
};

}

#endif // RINGBUFFER_H
