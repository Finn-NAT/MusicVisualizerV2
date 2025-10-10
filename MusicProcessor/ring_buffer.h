#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#define RING_BUFFER_MAX_SIZE    65536
#define RING_BUFFER_DATA_TYPE   float

typedef RING_BUFFER_DATA_TYPE ring_buffer_data_t;

// Ring buffer struct
typedef struct {
    ring_buffer_data_t *buffer;
    size_t size;      
    size_t current;         
} ring_buffer_t;

// Khởi tạo ring buffer
bool ring_buffer_init(ring_buffer_t *rb, size_t size);
// Giải phóng bộ nhớ
void ring_buffer_free(ring_buffer_t *rb);
// Đẩy dữ liệu vào buffer
size_t ring_buffer_write(ring_buffer_t *rb, const ring_buffer_data_t *data, size_t len);
// Đọc dữ liệu ra khỏi buffer
void ring_buffer_read_all(ring_buffer_t *rb, ring_buffer_data_t *data);

#ifdef __cplusplus
}
#endif

#endif // RING_BUFFER_H
