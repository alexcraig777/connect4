#include <project.h>

#include "led_control.h"

void signal_illegal_move() {
    // Flash the status LED to signal that the user
    // tried to make an illegal move.

    // Store off the original register value so we can
    // restore it at the end.
    uint8 orig_reg_value;
    orig_reg_value = Control_Reg_1_Read();
    
    int i;
    for (i = 0; i < 5; i++) {
        // Turn on the LED.
        led_on();
        CyDelay(100);
        // Turn off the LED.
        led_off();
        CyDelay(100);
    }
    
    // Reset the LED to mode 1.
    Control_Reg_1_Write(orig_reg_value);
}

void led_breathe() {
    // Set the lut to pass through the signal from
    // the PWMs that are breathing.
    Control_Reg_1_Write(0b000);
}

void led_blink() {
    // Set the lut to pass through the signal from
    // the 1 Hz clock.
    Control_Reg_1_Write(0b001);
}

void led_on() {
    Control_Reg_1_Write(0b110);
}

void led_off() {
    Control_Reg_1_Write(0b010);
}
