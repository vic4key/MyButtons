#include "gpio_buttons.h"
#include <Arduino.h>

GPIOButtons::GPIOButtons(const uint8_t pins[], uint8_t pin_count, uint8_t press_level)
    : pins_(pins), pin_count_(pin_count), press_level_(press_level) {
    uint8_t mode = (press_level_ == LOW) ? INPUT_PULLUP : INPUT;
    for (uint8_t i = 0; i < pin_count_; i++) {
        pinMode(pins_[i], mode);
    }
}

uint64_t GPIOButtons::scan() {
    uint64_t mask = 0;
    for (uint8_t i = 0; i < pin_count_; i++) {
        if (digitalRead(pins_[i]) == press_level_) {
            mask |= (1ULL << pins_[i]);
        }
    }
    return mask;
}
