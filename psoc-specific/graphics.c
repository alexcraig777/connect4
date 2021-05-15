// This module is my wrapper over top of the TFT interface.
// It is the only module that should include tft.h.

#include "graphics.h"

#include "../small_frame.h"
#include "tft.h"

int color0 = 0x0000;
int color1 = 0x00f0;
int color2 = 0xffff;

void display_frame(struct FramePosition* position) {
    int ridx;
    int cidx;

    uint16 SC, EC, SP, EP;
    
    int i;
    
    int colors[3] = {color0, color1, color2};
    
    for (ridx = 0; ridx < 6; ridx++) {
        for (cidx = 0; cidx < 7; cidx++) {
            // Initialize row and page points for this square.
            EC = 240 - (18 + 30*cidx);
            SC = 240 - (42 + 30*cidx);
            
            SP = 3 + 30*ridx;
            EP = 27 + 30*ridx;
            
            write8_a0(0x2A);                 	// Column Address Set
            write8_a1(SC >> 8);                 // set SC[15:0]
            write8_a1(SC & 0x00FF);
            write8_a1(EC >> 8);                 // set EC[15:0]
            write8_a1(EC & 0x00FF);
            write8_a0(0x2B);                 	// Page Address Set
            write8_a1(SP >> 8);                 // set SP[15:0]
            write8_a1(SP & 0x00FF);
            write8_a1(EP >> 8);                 // set EP[15:0]
            write8_a1(EP & 0x00FF);
            write8_a0(0x2C);                    // Memory Write
            
            for (i = 0; i < 625; i++) {
                write8_a1(colors[get_at_col_row(position, cidx, ridx)] >> 8);
                write8_a1(colors[get_at_col_row(position, cidx, ridx)] & 0xff);
            }
            write8_a0(0x00);                    // NOP
        }
    }
}

void display_winner(int winner) {
    uint16 SC, EC, SP, EP;
    int i;
    
    int colors[3] = {color0, color1, color2};

    
    SC = 18;
    EC = 240 - 18;
    
    SP = 30 * 7;
    EP = 300;

    write8_a0(0x2A);                 	// Column Address Set
    write8_a1(SC >> 8);                 // set SC[15:0]
    write8_a1(SC & 0x00FF);
    write8_a1(EC >> 8);                 // set EC[15:0]
    write8_a1(EC & 0x00FF);
    write8_a0(0x2B);                 	// Page Address Set
    write8_a1(SP >> 8);                 // set SP[15:0]
    write8_a1(SP & 0x00FF);
    write8_a1(EP >> 8);                 // set EP[15:0]
    write8_a1(EP & 0x00FF);
    write8_a0(0x2C);                    // Memory Write}
    
    for (i = 0; i < 21000; i++) {
        write8_a1(colors[winner] >> 8);
        write8_a1(colors[winner] & 0xff);
    }
    write8_a0(0x00);                    // NOP
}