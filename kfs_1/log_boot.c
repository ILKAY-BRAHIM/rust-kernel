unsigned short gradient_colors[] = {
    0x0F00, // White on black
    0x1F00, // Blue on black
    0x2F00, // Green on black
    0x4F00, // Red on black
    0x8F00, // Gray on black
    0x0F00, // White on black (cycle back)
};

void display_logo(int position, unsigned short* colors) {
    unsigned short* screen = (unsigned short*)0xB8000;
    screen[position] = '1' | colors[0];
    screen[position + 1] = '3' | colors[1];
    screen[position + 2] = '3' | colors[2];
    screen[position + 3] = '7' | colors[3];
}

void animate_light_effect() {
    int center_row = 12; // Middle of 25 rows
    int center_col = (80 - 4) / 2; // "1337" is 4 characters
    int position = center_row * 80 + center_col;

    // Initial gradient colors (dark)
    unsigned short colors[] = {0x0100, 0x0200, 0x0400, 0x0800};

    // Gradually increase brightness
    for (int step = 0; step < 5; step++) {
        // Update colors to simulate light
        for (int i = 0; i < 4; i++) {
            colors[i] += 0x1000; // Increase brightness
        }

        // Display the logo with updated colors
        display_logo(position, colors);

        // Simple delay loop (adjust for speed)
        for (volatile int i = 0; i < 1000000; i++);
    }
}

void flicker_effect(int position) {
    unsigned short colors[] = {0x1F00, 0x2F00, 0x4F00, 0x8F00};

    for (int i = 0; i < 10; i++) {
        // Randomly adjust brightness
        for (int j = 0; j < 4; j++) {
            colors[j] += (i % 2 == 0) ? 0x1000 : -0x1000;
        }

        display_logo(position, colors);

        // Simple delay loop (adjust for speed)
        for (volatile int k = 0; k < 500000; k++);
    }
}

void kernel_main() {
    // Clear the screen
    for (int i = 0; i < 80 * 25; i++) {
        write_char(' ', i, 0x0F00); // White on black
    }

    // Animate the light effect
    animate_light_effect();

    // Display the final "1337" logo with full brightness
    int center_row = 12; // Middle of 25 rows
    int center_col = (80 - 4) / 2; // "1337" is 4 characters
    int position = center_row * 80 + center_col;

    unsigned short final_colors[] = {0x1F00, 0x2F00, 0x4F00, 0x8F00};
    display_logo(position, final_colors);

    // Add flickering effect
    flicker_effect(position);

    while (1); // Halt
}