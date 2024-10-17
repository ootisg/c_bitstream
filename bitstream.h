#ifndef BITSTREAM_H
#define BITSTREAM_H

// This implementation is not portable. It should compile fine on x86-64 platforms, but no guarantees for anything else.

#include "stdlib.h"
#include "stdio.h"
#include "stdint.h"
#include "string.h"
#include "limits.h"

struct BitStream {
    uint8_t* data;
    int position;
};

typedef struct BitStream BitStream;

void BitStream_init_empty(BitStream* stream, int max_size) {
    stream->data = calloc(max_size, sizeof(uint8_t));
    stream->position = 0;
}

void BitStream_init_filled(BitStream* stream, int data_size, uint8_t* data) {
    stream->data = malloc(data_size * sizeof(uint8_t));
    memcpy(stream->data, data, data_size);
    stream->position = 0;
}

// num_bits is assumed to be <= 32
void BitStream_write(BitStream* stream, uint32_t data, int num_bits) {
    uint32_t offset_raw = ((stream->position & 0x3F) + num_bits) >> 6;
    uint64_t* dest = (uint64_t*)(( (uint32_t*)stream->data) + offset_raw ) + (stream->position / 64);
    int offset = (stream->position & 0x3F) - (offset_raw << 5);
    uint64_t mask = __builtin_bswap64((uint64_t)data << (64 - (num_bits + offset)));
    *dest = *dest | mask;
    stream->position += num_bits;
}

// num_bits is assumed to be <= 32
uint32_t BitStream_read(BitStream* stream, int num_bits) {
    uint64_t src_mask = (0x01 << num_bits) - 1;
    uint32_t offset_raw = ((stream->position & 0x3F) + num_bits) >> 6;
    uint64_t* src_val = (uint64_t*)(( (uint32_t*)stream->data) + offset_raw ) + (stream->position / 64);
    int offset = (stream->position & 0x3F) - (offset_raw << 5);
    uint32_t shifted = (__builtin_bswap64(*src_val) >> (64 - (num_bits + offset)));
    stream->position += num_bits;
    return shifted;
}

void BitStream_print(BitStream* stream) {
    int i;
    for (i = 0; i < stream->position / 8 + 1; i++) {
        printf("%02x", stream->data[i]);
    }
    printf("\n");
}

#endif