
// Big "1337" logo (7 lines tall, 28 characters wide)
const char* big_1337[] = {
    "      .o     .oooo.  ",
    "    .d88   .dP""Y88b ",
    "  .d'888         ]8P' ",
    ".d'  888       .d8P' ",
    "88ooo888oo   .dP'    ",
    "     888   .oP     .o",
    "    o888o  8888888888"
};


// Function to write a character to the screen
void write_char(char c, int position, unsigned short color) {
    unsigned short* screen = (unsigned short*)0xB8000;
    screen[position] = (unsigned short)c | color;
}

// Function to display the big "1337" logo
void display_big_logo(int start_row, int start_col, unsigned short color) {
    unsigned short* screen = (unsigned short*)0xB8000;

    for (int row = 0; row < 7; row++) {
        for (int col = 0; col < 21; col++) {
            int pos = (start_row + row) * 80 + (start_col + col);
            write_char(big_1337[row][col], pos, color);
        }
    }
}

// Function to center the big "1337" logo on the screen
void center_big_logo(unsigned short color) {
    int start_row = (25 - 7) / 2; // Center vertically
    int start_col = (80 - 28) / 2; // Center horizontally
    display_big_logo(start_row, start_col, color);
}

// Function to animate the light effect
void animate_light_effect() {
    // Initial color (dark)
    unsigned short color = 0x0100;

    // Gradually increase brightness
    for (int step = 0; step < 5; step++) {
        color += 0x1000; // Increase brightness
        center_big_logo(color);

        // Simple delay loop (adjust for speed)
        for (volatile int i = 0; i < 1000000; i++);
    }
}

// Function to add a flickering effect
void flicker_effect() {
    unsigned short color = 0x1F00; // Blue background

    for (int i = 0; i < 10; i++) {
        // Randomly adjust brightness
        color += (i % 2 == 0) ? 0x1000 : -0x1000;
        center_big_logo(color);

        // Simple delay loop (adjust for speed)
        for (volatile int j = 0; j < 500000; j++);
    }
}

// Kernel main function
void kernel_main() {
    // Clear the screen
    for (int i = 0; i < 80 * 25; i++) {
        write_char(' ', i, 0x0F00); // White on black
    }

    // Animate the light effect
    animate_light_effect();

    // Display the final big "1337" logo with full brightness
    center_big_logo(0x1F00); // Blue background

    // Add flickering effect
    flicker_effect();

    while (1); // Halt
}