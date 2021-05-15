#include "nine_byte.h"

#include <stdint.h>
#include <stdio.h>

// M is 2^32 // 3 = (2^32 - 1)/3. We need it for carrying over division.
#define M 0x55555555
#define TWICE_M 0xaaaaaaaa


void fill_nine_byte(struct NineByte* nine_byte_ptr, int initial_value) {
    // Fill in the NineByte structure with an initial low uint32 and
    // zero the other fields.
    nine_byte_ptr->low = (uint32_t) initial_value;
    nine_byte_ptr->middle = 0;
    nine_byte_ptr->high = 0;
}

void copy_nine_byte(struct NineByte* dest, struct NineByte* source) {
    dest->low = source->low;
    dest->middle = source->middle;
    dest->high = source->high;
}

void multiply_by_3(struct NineByte* nine_byte) {
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

void add_nine_bytes(struct NineByte* dest, struct NineByte* addend) {
    // Add dest and addend together and put the result in dest.
    int low_carry, mid_carry;
    low_carry = 0;
    mid_carry = 0;

    // We need to save off the addend values (for comparison
    // purposes, to decide on carries) in case the addend is
    // the same as the dest, in which case the values will
    // change when we operate on it.
    uint32_t addend_low, addend_middle;
    addend_low = addend->low;
    addend_middle = addend->middle;

    // Form the low uint32 and decide whether we need to carry.
    dest->low = dest->low + addend->low;

    if (dest->low < addend_low) {
	// We must have overflowed this uint32, so we need to add
	// a carry.
	low_carry = 1;
    } else {
	// There was no carry from the low uint32.
	low_carry = 0;
    }

    // Now form the middle uint32.
    dest->middle = dest->middle + addend->middle + low_carry;

    // Now decide whether we need a carry for the high uint8.
    if (low_carry) {
	if (dest->middle <= addend_middle) {
	    mid_carry = 1;
	} else {
	    mid_carry = 0;
	}
    } else {
	if (dest->middle < addend_middle) {
	    mid_carry = 1;
	} else {
	    mid_carry = 0;
	}
    }

    dest->high = dest->high + addend->high + mid_carry;
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

/*
void print_nine_byte(struct NineByte nine_byte) {
    printf("%02x %08x %08x\n", ((uint32_t) nine_byte.high) & 0xff,
           nine_byte.middle, nine_byte.low);
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

    multiply_by_3(&nine_byte);
    print_nine_byte(nine_byte);

    multiply_by_3(&nine_byte);
    print_nine_byte(nine_byte);

    multiply_by_3(&nine_byte);
    print_nine_byte(nine_byte);

    multiply_by_3(&nine_byte);
    print_nine_byte(nine_byte);

    add_nine_bytes(&nine_byte, &nine_byte);
    print_nine_byte(nine_byte);

    return 0;
}
*/
