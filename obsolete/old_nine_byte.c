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

void mutliply_by_3(struct NineByte* nine_byte) {
    // Modify the NineByte in place to multiply it by 3.
    int overflow_low, overflow_mid;

    // Compute how much we need to add to the middle uint32 to
    // account for overflow from the low spot.
    if (nine_byte->low <= M) {
	overflow_low = 0;
    } else if (nine_byte->low <= TWICE_M) {
	overflow_low = 1;
    } else {
	overflow_low = 2;
    }

    // Compute the overflow to add to the high uint8.
    if (nine_byte->middle < M) {
	// Overflow will always be 0.
	overflow_mid = 0;
    } else if (nine_byte->middle == M) {
	// Overflow will be 0 if there is no low overflow;
	// otherwise it will be 1.
	if (overflow_low == 0) {
	    overflow_mid = 0;
	} else {
	    overflow_mid = 1;
	}
    } else if (nine_byte->middle < TWICE_M) {
	printf("In middle range!\n");
	// There wil always be 1 overflow.
	overflow_mid = 1;
    } else if (nine_byte->middle == TWICE_M) {
	// Overflow will be 2 if there is 2 overflow from
	// below; otherwise it will be 1.
	if (overflow_low == 2) {
	    overflow_mid = 2;
	} else {
	    overflow_mid = 1;
	}
    } else {
	overflow_mid = 2;
    }

    nine_byte->low = 3 * nine_byte->low;

    nine_byte->middle = 3 * nine_byte->middle + overflow_low;

    nine_byte->high = 3 * nine_byte->high + overflow_mid;
}

int remainder_mod_3(struct NineByte nine_byte) {
    return ((nine_byte.low % 3) + (nine_byte.middle % 3) + (nine_byte.high % 3)) % 3;
}

void integer_divide_by_3(struct NineByte* nine_byte) {
    // Modify the NineByte in place to divide it by 3.
    uint32_t high_remainder, middle_remainder, low_remainder, mid_high_remainder;

    // First divide the most significant byte by 3, saving off the remainder.
    high_remainder = nine_byte->high % 3;
    nine_byte->high = nine_byte->high / 3;

    // Now store off the remainder of the middle divided by 3.
    middle_remainder = nine_byte->middle % 3;

    // Now compute the new middle value.
    nine_byte->middle = ((nine_byte->middle / 3) +
			 (M * high_remainder) +
			 ((middle_remainder + high_remainder) / 3));

    // Now compute the effective remainder for the high and middle combined.
    mid_high_remainder = (middle_remainder + high_remainder) % 3;

    // Finally, compute the new low value.
    low_remainder = nine_byte->low % 3;
    
    nine_byte->low = ((nine_byte->low / 3) +
			 (M * mid_high_remainder) +
			 ((low_remainder + mid_high_remainder) / 3));
};

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
