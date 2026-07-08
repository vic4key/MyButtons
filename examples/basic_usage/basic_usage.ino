/**
 * basic_usage.ino
 * Example sketch demonstrating MyButtons library.
 *
 * Wiring (active-high, press_level = HIGH):
 *   GPIO ◄─[button]─► VCC, with external 10kΩ pull-down to GND
 *
 * Wiring (active-low, press_level = LOW):
 *   GPIO ◄─[button]─► GND, no external resistor needed (INPUT_PULLUP)
 */

#include <my_buttons.h>

enum ButtonId {
    BTN_FAN   = 10,
    BTN_LIGHT = 11,
    BTN_RESET = 12,
    BTN_POWER = 13,
};

const uint8_t pins[] = {BTN_FAN, BTN_LIGHT, BTN_RESET, BTN_POWER};
ButtonManager bm(pins, 4, LOW); // The 3rd arg `press_level`: HIGH (active-high) or LOW (active-low).

void setup() {
    Serial.begin(115200);
}

void loop() {
    bm.update(); // Must call first, every loop iteration

    if (bm.has_pressed()) {          // Any button pressed?
        uint8_t id = bm.read_pressed();
        Serial.printf("Button %d pressed", id);
    }

    if (bm.is_pressed(BTN_LIGHT)) {  // Is BTN_LIGHT currently pressed?
        // Continuous action while holding
    }
}