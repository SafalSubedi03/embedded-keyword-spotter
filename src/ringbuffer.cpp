#include "ringbuffer.h"

void initilize_rb(ringBuffer *rb)
{
    rb->writeIndex.store(0);
    rb->readIndex.store(0);

    for (int i = 0; i < BufferSize; i++)
        rb->buffer[i] = 0;
}

void write_rb(ringBuffer *rb, const int16_t *data, int count)
{
    int w = rb->writeIndex.load(std::memory_order_relaxed);

    for (int i = 0; i < count; i++) {
        rb->buffer[w] = data[i];
        w = (w + 1) % BufferSize;
    }

    rb->writeIndex.store(w, std::memory_order_release);
}

int available_rb(ringBuffer *rb)
{
    int w = rb->writeIndex.load(std::memory_order_acquire);
    int r = rb->readIndex.load(std::memory_order_relaxed);
    return (w - r + BufferSize) % BufferSize;
}

int read_rb(ringBuffer *rb, int16_t *out, int count)
{
    int avail = available_rb(rb);
    if (avail < count)
        count = avail;

    int r = rb->readIndex.load(std::memory_order_relaxed);

    for (int i = 0; i < count; i++) {
        out[i] = rb->buffer[r];
        r = (r + 1) % BufferSize;
    }

    rb->readIndex.store(r, std::memory_order_release);
    return count;
}