#ifndef RINGBUFFER_H
#define RINGBUFFER_H

#include "config.h"
#include <stdint.h>

struct ringBuffer {
    int writeIndex;
    int readIndex;
    bool isAvailable;
    int16_t buffer[BufferSize];
};

// API (declarations only)
void initilize_rb(ringBuffer* rb);
void write_rb(ringBuffer* rb, int16_t data, int datalen = 1);
int16_t read_rb(ringBuffer* rb);

#endif