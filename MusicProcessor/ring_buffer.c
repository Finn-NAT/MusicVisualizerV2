#include "ring_buffer.h"
#include <stdlib.h>
#include <string.h>

bool ring_buffer_init(ring_buffer_t *rb, size_t size) {
    if (!rb || size == 0) return false;
    rb->buffer = (float*)calloc(size, sizeof(float));
    if (!rb->buffer) return false;
    if(size > RING_BUFFER_MAX_SIZE) {
        rb->size = RING_BUFFER_MAX_SIZE;
    }else {
        rb->size = size;
    }
    rb->current = 0;
    return true;
}

void ring_buffer_free(ring_buffer_t *rb) {
    if (rb && rb->buffer) {
        free(rb->buffer);
        rb->buffer = NULL;
        rb->size = 0;
        rb->current = 0;
    }
}

size_t ring_buffer_write(ring_buffer_t *rb, const ring_buffer_data_t *data, size_t len) {
    if (!rb || !rb->buffer || !data || len == 0) return 0;
    if(len > rb->size - rb->current) {
        memcpy(rb->buffer + rb->current, data, (rb->size - rb->current) * sizeof(ring_buffer_data_t));
        memcpy(rb->buffer, data + (rb->size - rb->current), (len - (rb->size - rb->current)) * sizeof(ring_buffer_data_t));
    } else {
        memcpy(rb->buffer + rb->current, data, len * sizeof(ring_buffer_data_t));
    }
    rb->current = (rb->current + len) % rb->size;
    return len;
}

void ring_buffer_read_all(ring_buffer_t *rb, ring_buffer_data_t *data) {
    if (!rb || !rb->buffer || !data) return;
    memcpy(data, rb->buffer + rb->current, (rb->size - rb->current) * sizeof(ring_buffer_data_t));
    memcpy(data + (rb->size - rb->current), rb->buffer, rb->current * sizeof(ring_buffer_data_t));
}
