#include "quantum.h"

void led_init_ports(void) {
    setPinOutput(LED_NUM_LOCK_PIN);
    setPinOutput(LED_CAPS_LOCK_PIN);
    setPinOutput(LED_SCROLL_LOCK_PIN);

    writePinHigh(LED_CAPS_LOCK_PIN);
    writePinHigh(LED_SCROLL_LOCK_PIN);
    writePinHigh(LED_NUM_LOCK_PIN);
}

void matrix_init_kb(void) {
    matrix_init_user();
}

void led_update_ports(led_t led_state) {
    if (led_state.caps_lock) {
        writePinLow(LED_CAPS_LOCK_PIN);
    } else {
        writePinHigh(LED_CAPS_LOCK_PIN);
    }

#ifdef DEFAULT_MAP
    if (led_state.scroll_lock) {
        writePinLow(LED_SCROLL_LOCK_PIN);
    } else {
        writePinHigh(LED_SCROLL_LOCK_PIN);
    }

    if (led_state.num_lock) {
        writePinLow(LED_NUM_LOCK_PIN);
    } else {
        writePinHigh(LED_NUM_LOCK_PIN);
    }
#endif
}
