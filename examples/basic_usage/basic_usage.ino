/**
 * basic_usage.ino
 * Example sketch demonstrating MyButtons library.
 *
 * Wiring: each button connects the GPIO pin to VCC (3.3V / 5V) when pressed.
 *         Use external pull-down resistors (e.g. 10kΩ) from each pin to GND.
 */

#include "my_buttons.h"

// -------------------------------------------------------------------
// Define button ids using GPIO pin numbers (enum makes code readable)
// -------------------------------------------------------------------
enum ButtonId {
    BTN_A = 4,
    BTN_B = 16,
    BTN_C = 17,
    BTN_D = 18
};

uint8_t pins[] = {BTN_A, BTN_B, BTN_C, BTN_D};

// Create the manager with direct GPIO – simplest approach
ButtonManager bm(pins, 4);

// -------------------------------------------------------------------
// Callback: called automatically on each press event
// -------------------------------------------------------------------
void on_press(uint8_t id) {
    if (id == BTN_A) {
        // This runs ONCE per press of BTN_A
    }
}

void setup() {
    Serial.begin(115200);

    // Register the callback (optional)
    bm.set_on_pressed_callback(on_press);
}

void loop() {
    // Must be called every loop iteration
    bm.update();

    // --------------- Polling API (Serial-like) ---------------
    if (bm.has_pressed()) {
        uint8_t id = bm.read_pressed();
        Serial.printf("Button %d was just pressed\n", id);
    }

    // --------------- Level-based API ---------------
    if (bm.is_pressed(BTN_B)) {
        // Runs continuously while BTN_B is held down
        Serial.println("Holding BTN_B...");
    }

    if (bm.any_pressed()) {
        // At least one button is currently held
    }
}
