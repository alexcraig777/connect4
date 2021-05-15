#ifndef NINE_BYTE_H
#define NINE_BYTE_H

#include <stdint.h>

struct NineByte {
    uint32_t low;
    uint32_t middle;
    uint8_t high;
};

void fill_nine_byte(struct NineByte* nine_byte_ptr, int initial_value);

void copy_nine_byte(struct NineByte* dest, struct NineByte* source);

void multiply_by_3(struct NineByte* nine_byte);

void add_nine_bytes(struct NineByte* dest, struct NineByte* addend);

int get_trinary_digit(struct NineByte nine_byte, int idx);

#endif
