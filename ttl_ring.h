#ifndef TTL_RING_H
#define TTL_RING_H

typedef struct LogChunk LogChunk;
typedef struct TimeRingBuffer TimeRingBuffer;
TimeRingBuffer* init_ring();
void push_log_chunk(TimeRingBuffer* rb, int id, const char* text, unsigned long long ttl);
void purge_expired_chunks(TimeRingBuffer* rb, unsigned long long current_epoch);
int compile_valid_text(TimeRingBuffer* rb, char* output_buffer);

#endif
