C code for 6.115 final project.

The project goal is to program a PSoC to play Connect Four against
a human while displaying the board on a TFT screen and taking
input from buttons.

The project code is organized in 4 parts:
 (1) The core code (which will be run on the PSoC and can be tested
     on a PC in Linux) is in first level of the main directory.

 (2) Code that can only be run on the PSoC is in psoc-specific.

 (3) Code to test the core code in Linux is in pc-tests.

 (4) Obsolete code (RIP my heap implementation) is in obsolete.
