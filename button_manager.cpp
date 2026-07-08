#include "button_manager.h"

// ---- Constructors ----

ButtonManager::ButtonManager(const uint8_t pins[], uint8_t pin_count, uint8_t press_level)
    : buttons_source_(new GPIOButtons(pins, pin_count, press_level)),
      owns_source_(true) {
}

ButtonManager::ButtonManager(IButtons& buttons)
    : buttons_source_(&buttons),
      owns_source_(false) {
}

ButtonManager::~ButtonManager() {
    if (owns_source_) {
        delete buttons_source_;
    }
}

// ---- Core update ----

void ButtonManager::update() {
    uint64_t raw_mask = buttons_source_->scan();

    // Same as stable state → no debounce needed
    if (raw_mask == current_mask_) {
        debounce_candidate_ = current_mask_;
        last_debounce_time_ = millis();
        return;
    }

    // raw_mask != current_mask_ : a change is happening

    // If the candidate changed, restart the debounce timer
    if (raw_mask != debounce_candidate_) {
        debounce_candidate_ = raw_mask;
        last_debounce_time_ = millis();
        return;
    }

    // Same candidate held long enough → accept new state
    if ((millis() - last_debounce_time_) >= DEBOUNCE_DELAY) {
        // Rising edges: bits that are 1 now but were 0 before
        uint64_t rising = raw_mask & ~current_mask_;

        prev_mask_ = current_mask_;
        current_mask_ = raw_mask;

        // Process all rising edges (button press events)
        uint64_t temp = rising;
        while (temp) {
            int bit = __builtin_ctzll(temp);
            uint8_t id = (uint8_t)bit;
            enqueue(id);
            if (pressed_callback_) {
                pressed_callback_(id);
            }
            temp &= temp - 1; // clear lowest set bit
        }
    }
}

// ---- Callback ----

void ButtonManager::set_on_pressed_callback(std::function<void(uint8_t)> callback) {
    pressed_callback_ = callback;
}

// ---- Polling API (Serial-like) ----

bool ButtonManager::has_pressed() {
    return queue_count_ > 0;
}

uint8_t ButtonManager::read_pressed() {
    if (queue_count_ == 0) {
        return 0xFF;
    }
    uint8_t id = queue_[queue_tail_];
    queue_tail_ = (queue_tail_ + 1) % QUEUE_SIZE;
    queue_count_--;
    return id;
}

// ---- Level-based API ----

bool ButtonManager::is_pressed(uint8_t id) {
    return (current_mask_ >> id) & 1ULL;
}

bool ButtonManager::any_pressed() {
    return current_mask_ != 0;
}

// ---- Internal helpers ----

void ButtonManager::enqueue(uint8_t id) {
    if (queue_count_ >= QUEUE_SIZE) {
        // Queue full: discard the oldest event
        queue_tail_ = (queue_tail_ + 1) % QUEUE_SIZE;
        queue_count_--;
    }
    queue_[queue_head_] = id;
    queue_head_ = (queue_head_ + 1) % QUEUE_SIZE;
    queue_count_++;
}
