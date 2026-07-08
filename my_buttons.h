#pragma once

/**
 * @file my_buttons.h
 * @brief Main include header for the MyButtons library.
 *
 * Include this single header to use all MyButtons classes:
 *   - IButtons       (abstract interface)
 *   - GPIOButtons    (GPIO implementation)
 *   - ButtonManager  (debounce, events, polling)
 */

#include "i_buttons.h"
#include "gpio_buttons.h"
#include "button_manager.h"
