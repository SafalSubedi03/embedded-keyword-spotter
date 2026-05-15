#ifndef RINGBUFFER_H
#define RINGBUFFER_H

#include <atomic>
#include <cstdint>
#include "config.h"

struct ringBuffer {
    int16_t          buffer[BufferSize];
    std::atomic<int> writeIndex{0};   // initialize inline
    std::atomic<int> readIndex{0};    // initialize inline
};

void initilize_rb(ringBuffer *rb);
void write_rb(ringBuffer *rb, const int16_t *data, int count);
int  available_rb(ringBuffer *rb);
int  read_rb(ringBuffer *rb, int16_t *out, int count);

#endif