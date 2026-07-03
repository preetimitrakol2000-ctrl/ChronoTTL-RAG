#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define RING_CAPACITY 4

typedef struct {
    int tracking_id;
    char details[128];
    unsigned long long expiry_epoch;
} LogChunk;

typedef struct {
    LogChunk storage[RING_CAPACITY];
    int head;
    int tail;
    int count;
} TimeRingBuffer;

#ifdef _WIN32
    __declspec(dllexport) TimeRingBuffer* init_ring();
    __declspec(dllexport) void push_log_chunk(TimeRingBuffer* rb, int id, const char* text, unsigned long long ttl);
    __declspec(dllexport) void purge_expired_chunks(TimeRingBuffer* rb, unsigned long long current_epoch);
    __declspec(dllexport) int compile_valid_text(TimeRingBuffer* rb, char* output_buffer);
#endif

TimeRingBuffer* init_ring() {
    TimeRingBuffer* rb = (TimeRingBuffer*)malloc(sizeof(TimeRingBuffer));
    rb->head = 0; rb->tail = 0; rb->count = 0;
    return rb;
}

void push_log_chunk(TimeRingBuffer* rb, int id, const char* text, unsigned long long ttl) {
    if (rb->count >= RING_CAPACITY) {
        // Enforce head eviction shift if ring fills completely
        rb->head = (rb->head + 1) % RING_CAPACITY;
        rb->count--;
    }
    rb->storage[rb->tail].tracking_id = id;
    strncpy(rb->storage[rb->tail].details, text, sizeof(rb->storage[rb->tail].details) - 1);
    rb->storage[rb->tail].expiry_epoch = ttl;
    
    rb->tail = (rb->tail + 1) % RING_CAPACITY;
    rb->count++;
}

void purge_expired_chunks(TimeRingBuffer* rb, unsigned long long current_epoch) {
    int checked = 0;
    int total = rb->count;
    while (checked < total) {
        int idx = (rb->head) % RING_CAPACITY;
        if (rb->storage[idx].expiry_epoch < current_epoch && rb->count > 0) {
            rb->head = (rb->head + 1) % RING_CAPACITY;
            rb->count--;
        }
        checked++;
    }
}

int compile_valid_text(TimeRingBuffer* rb, char* output_buffer) {
    int written = 0;
    for (int i = 0; i < rb->count; i++) {
        int idx = (rb->head + i) % RING_CAPACITY;
        strcat(output_buffer, rb->storage[idx].details);
        strcat(output_buffer, " | ");
        written++;
    }
    return written;
}
