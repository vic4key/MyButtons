#include <Arduino.h>
#include "my_buttons.h"

enum ButtonId {
    BTN_K1 = 10,
    BTN_K2 = 11,
    BTN_K3 = 12,
    BTN_K4 = 13,
};

uint8_t pins[] = {BTN_K1, BTN_K2, BTN_K3, BTN_K4};
ButtonManager bm(pins, lengthof(pins));

void on_press(uint8_t id) {
    Serial.printf("Pressed: %d\n", id);
}

void setup() {
    Serial.begin(115200);
    // bm.set_on_pressed_callback(on_press);
}

void loop() {
    bm.update();

    if (bm.has_pressed()) {
        uint8_t id = bm.read_pressed();
        Serial.printf("Poll: %d\n", id);
    }

    // if (manager.is_pressed(BTN_K1)) {
    //     Serial.println("Holding K1");
    // }
}