# MyButtons

A simple Arduino/ESP32 button library for handling a list of buttons - configure once, trigger a debounced pressed event via callback, or check pressed states anytime.

- 🕹️  Works with both **active-high** (VCC) and **active-low** (GND) buttons
- 🧹  Built-in **debounce** — no bouncing, no delays
- 📞  **Callback** or **polling** — pick your style
- 🔋  Up to **64 buttons**, AVR to ESP32

---

## 📦 Installation

### Arduino IDE
1. **Library Manager**: Search for `MyButtons` and click Install.
2. **Manual**: Download the repo as ZIP, then Sketch → Include Library → Add .ZIP Library.

---

## 🚀 Quick Start

```cpp
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
```

---

## 🔌 Wiring

<details>
<summary>🔽 Click to expand wiring diagrams…</summary>

### `press_level = HIGH` (default for `GPIOButtons`)
Button connects GPIO pin → **VCC** when pressed. Requires **external pull-down** resistor (10kΩ to GND).

```
 GPIO ◄─[button]─► VCC (3.3V/5V)
 GPIO ◄─[10kΩ]───► GND
```

### `press_level = LOW` (default for `ButtonManager`, e.g. 4-button modules)
Button connects GPIO pin → **GND** when pressed. Uses **internal pull-up** — no external resistor needed.

```
 GPIO ◄─[button]─► GND
```

> 💡 **Rule of thumb**: If your module has a **VCC pin** → use `HIGH`. If it only has **GND** → use `LOW`.
</details>

---

## 📖 API Reference

<details>
<summary>🔽 Click to expand API docs…</summary>

### `ButtonManager`
| Method | Returns | Description |
|---|---|---|
| `ButtonManager(pins, count, press_level)` | — | Constructor with direct GPIO pins |
| `ButtonManager(IButtons& buttons)` | — | Constructor with custom button source |
| `update()` | `void` | Call in `loop()` — scans, debounces, detects edges |
| `set_on_pressed_callback(fn)` | `void` | Register `void fn(uint8_t id)` callback |
| `has_pressed()` | `bool` | True if unread press events exist in queue |
| `read_pressed()` | `uint8_t` | Get oldest press ID, or `0xFF` if empty |
| `is_pressed(uint8_t id)` | `bool` | True if button `id` is currently held |
| `any_pressed()` | `bool` | True if any button is currently held |

### `GPIOButtons`
| Method | Returns | Description |
|---|---|---|
| `GPIOButtons(pins, count, press_level)` | — | Constructor |
| `scan()` | `uint64_t` | Read all pins, return bitmask |

### `IButtons` (interface)
| Method | Returns | Description |
|---|---|---|
| `scan()` | `uint64_t` | Pure virtual — implement for custom button sources |
</details>

---

## 📝 License

Released under the [MIT](LICENSE) license


## 📞 Contact
Feel free to contact via [Twitter](https://twitter.com/vic4key) / [Gmail](mailto:vic4key@gmail.com) / [Blog](https://blog.vic.onl/) / [Website](https://vic.onl/)