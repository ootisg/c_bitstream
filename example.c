#include "bitstream.h"

int main() {
    BitStream* s = malloc(sizeof(BitStream));
    BitStream* s2 = malloc(sizeof(BitStream));
    char* sample_data = "The quick brown fox jumps over the lazy dog";
    int sample_data_len = strlen(sample_data) + 8;
    int sample_bit_len = sample_data_len * 8;
    BitStream_init_filled(s, sample_data_len, sample_data);
    BitStream_init_empty(s2, sample_data_len);
    int bits_left = sample_bit_len;
    while (bits_left >= 32) {
        int bits_to_add = rand() % 30 + 1;
        int popped_bits = BitStream_read(s, bits_to_add);
        printf("Moving %d bits\n", bits_to_add);
        BitStream_write(s2, popped_bits, bits_to_add);
        bits_left -= bits_to_add;
    }
    printf("%s\n", s2->data);
    return 0;
}