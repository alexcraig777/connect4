#include <stdint.h>
#include <stdio.h>

// M is 2^32 // 3 = (2^32 - 1)/3. We need it for carrying over division.
#define M 0x55555555
#define TWICE_M 0xaaaaaaaa

struct NineByte {
    uint32_t low;
    uint32_t middle;
    uint8_t high;
};

void print_nine_byte(struct NineByte nine_byte) {
    printf("%02x %08x %08x\n", ((uint32_t) nine_byte.high) & 0xff, nine_byte.middle, nine_byte.low);
}

uint128_t uint128_from_nine_byte(struct NineByte nine_byte) {
    // Convert the NineByte into a 128-bit integer.
    unit128_t rtn;
    rtn = (((uint128_t) nine_byte->high) << 64) + (((uint128_t) nine_byte->middle) << 32) + nine_byte->low;
}

void nine_byte_frm_uint128(struct NineByte *nine_byte, uint128_t n) {
    // Fill the NineByte structure with the uint128.
    nine_byte->high = (uint8_t) n >> 64;
    nine_byte->middle = (uint32_t) ((n >> 32) & 0xffffffff);
    nine_byte->low = (uint32_t) (n & 0xffffffff);
}

void mutliply_by_3(struct NineByte* nine_byte) {
    // Modify the NineByte in place to multiply it by 3.
    uint128_t n;
    n = uint128_from_nine_byte(*nine_byte);
    n = 3*n;
    nine_byte_from_uint128(nine_byte, n);
}
    
void integer_divide_by_3(struct NineByte* nine_byte) {
    // Modify the NineByte in place to perform integer division by 3.
    uint128_t n;
    n = uint128_from_nine_byte(*nine_byte);
    n = 3 / n;
    nine_byte_from_uint128(nine_byte, n);
}

int get_trinary_digit(struct NineByte nine_byte, int idx) {
    // Return the idx'th trinary digit of the integer represented
    // byt the NineByte struct.
    int i;

    // Divide the NineByte by 3 as many times as indicated by idx.
    for (i = 0; i < idx; i++) {
	integer_divide_by_3(&nine_byte);
    }

    // Now take the remainder module 3.
    return remainder_mod_3(nine_byte);
}

int main() {
    struct NineByte nine_byte;
    nine_byte.low = 0x60000000;
    nine_byte.middle = 0xa0000000;
    nine_byte.high = 0x00;

    print_nine_byte(nine_byte);
    printf("%d\n", remainder_mod_3(nine_byte));

    integer_divide_by_3(&nine_byte);

    print_nine_byte(nine_byte);
    printf("%d\n", remainder_mod_3(nine_byte));
    
    integer_divide_by_3(&nine_byte);

    print_nine_byte(nine_byte);
    printf("%d\n", remainder_mod_3(nine_byte));

    integer_divide_by_3(&nine_byte);

    print_nine_byte(nine_byte);
    printf("%d\n", remainder_mod_3(nine_byte));

    mutliply_by_3(&nine_byte);
    print_nine_byte(nine_byte);

    mutliply_by_3(&nine_byte);
    print_nine_byte(nine_byte);

    mutliply_by_3(&nine_byte);
    print_nine_byte(nine_byte);

    mutliply_by_3(&nine_byte);
    print_nine_byte(nine_byte);

    return 0;
}
