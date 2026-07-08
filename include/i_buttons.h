#pragma once

#include <stdint.h>

class IButtons {
public:
    virtual ~IButtons() = default;

    /**
     * @brief Scan all buttons and return a 64-bit bitmask.
     *        Bit `id` is 1 if the button is currently pressed, 0 otherwise.
     */
    virtual uint64_t scan() = 0;
};
