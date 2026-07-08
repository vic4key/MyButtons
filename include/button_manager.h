#pragma once

#include <stdint.h>
#include <functional>
#include <Arduino.h>

#include "i_buttons.h"
#include "gpio_buttons.h"

#ifndef lengthof
#define lengthof(arr)(sizeof(arr) / sizeof(arr[0]))
#endif  // lengthof

class ButtonManager {
public:
    /**
     * @brief Construct with direct GPIO pins.
     *        Automatically creates and owns a GPIOButtons instance.
     * @param pins        Array of GPIO pin numbers.
     * @param pin_count   Number of pins in the array.
     * @param press_level The logic level read when a button is pressed (hardware-dependent):
     *   - HIGH: button connects to VCC when pressed → module has VCC pin → pinMode INPUT
     *   - LOW:  button connects to GND when pressed → module has GND only → pinMode INPUT_PULLUP
     */
    ButtonManager(const uint8_t pins[], uint8_t pin_count, uint8_t press_level = LOW);

    /**
     * @brief Construct with a custom IButtons source (e.g. mock, expander, etc.).
     * @param buttons Reference to any class implementing IButtons.
     */
    ButtonManager(IButtons& buttons);

    ~ButtonManager();

    // Non-copyable
    ButtonManager(const ButtonManager&) = delete;
    ButtonManager& operator=(const ButtonManager&) = delete;

    // Movable
    ButtonManager(ButtonManager&&) = default;
    ButtonManager& operator=(ButtonManager&&) = default;

    /**
     * @brief Must be called periodically (in loop()).
     *        Performs scan, debounce, edge detection, and event queuing.
     */
    void update();

    /**
     * @brief Register a callback invoked on each press event.
     * @param callback Function taking the pressed button's id.
     */
    void set_on_pressed_callback(std::function<void(uint8_t)> callback);

    // --- Polling API (Serial-like) ---

    /**
     * @brief Check if unread press events exist in the queue.
     * @return true if at least one event is available.
     */
    bool has_pressed();

    /**
     * @brief Read (consume) the oldest press event from the queue.
     * @return The button id, or 0xFF if the queue is empty.
     */
    uint8_t read_pressed();

    // --- Level-based API ---

    /**
     * @brief Check whether a specific button is currently held down.
     * @param id The button id (GPIO pin number).
     * @return true if the button is being pressed right now.
     */
    bool is_pressed(uint8_t id);

    /**
     * @brief Check whether any button is currently held down.
     * @return true if at least one button is pressed.
     */
    bool any_pressed();

private:
    static const uint8_t QUEUE_SIZE = 8;
    static const uint32_t DEBOUNCE_DELAY = 30; // milliseconds

    IButtons* buttons_source_;
    bool owns_source_;

    uint64_t prev_mask_ = 0;
    uint64_t current_mask_ = 0;
    uint64_t debounce_candidate_ = 0;

    uint32_t last_debounce_time_ = 0;

    // Circular queue for press events
    uint8_t queue_[QUEUE_SIZE];
    uint8_t queue_head_ = 0;
    uint8_t queue_tail_ = 0;
    uint8_t queue_count_ = 0;

    std::function<void(uint8_t)> pressed_callback_ = nullptr;

    void enqueue(uint8_t id);
};
