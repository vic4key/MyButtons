#pragma once

#include "i_buttons.h"
#include <stdint.h>
#include <Arduino.h>

class GPIOButtons : public IButtons {
public:
    /**
     * @brief Construct a GPIOButtons instance.
     * @param pins        Array of GPIO pin numbers.
     * @param pin_count   Number of pins in the array.
     * @param press_level The logic level read when a button is pressed (hardware-dependent):
     *   - HIGH: button connects to VCC when pressed → module has VCC pin → pinMode INPUT
     *   - LOW:  button connects to GND when pressed → module has GND only → pinMode INPUT_PULLUP
     */
    GPIOButtons(const uint8_t pins[], uint8_t pin_count, uint8_t press_level = HIGH);

    /**
     * @brief Read all GPIO pins and return a bitmask.
     *        Bit index = GPIO pin number.
     */
    virtual uint64_t scan() override;

private:
    const uint8_t* pins_;
    uint8_t pin_count_;
    uint8_t press_level_;
};
