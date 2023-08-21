#include QMK_KEYBOARD_H

enum layer_names {
    _BASE,
    _NUMLOCK,
    _EXTRA,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/*
 * ,---------------------------------------------------------------------------------------------------------------------------.
 * |  F1 |  F2 | Esc |  1  |  2  |  3  |  4  |  5  |  6  |  7  |  8  |  9  |  0  |  -  |  =  |Backspace|NumLk| ScrLk  | SysRQ  |
 * |---------------------------------------------------------------------------------------------------------------------------|
 * |  F3 |  F4 |  Tab  |  Q  |  W  |  E  |  R  |  T  |  Y  |  U  |  I  |  O  |  P  |  [  |  ]   |      |  7  |  8  |  9  |PrnSc|
 * |---------------------------------------------------------------------------------------------      ------------------------|
 * |  F5 |  F6 |  Ctrl  |  A  |  S  |  D  |  F  |  G  |  H  |  J  |  K  |  L  |  ;: |  '" |     Enter  |  4  |  5  |  6  |  -  |
 * |---------------------------------------------------------------------------------------------------------------------------|
 * |  F7 |  F8 |   Shift   |  Z  |  X  |  C  |  V  |  B  |  N  |  M  |  ,  |  .  |  /? |  Shift  |  |\ |  1  |  2  |  3  |  +  |
 * |----------------------------------------------------------------------------------------------------------------------     |
 * |  F9 | F10 |  Alt   | ~` |                        Space                            | CapsLk  |   0 Ins   |    . Del  |     |
 * `---------------------------------------------------------------------------------------------------------------------------'
*/
    [_BASE] = LAYOUT(
    KC_F1,  KC_F2, KC_ESC,   KC_1,    KC_2,   KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC,   TG(_NUMLOCK),     MO(_EXTRA),    KC_RGUI,
    KC_F3,  KC_F4, KC_TAB,   KC_Q,    KC_W,   KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC,            KC_HOME,   KC_UP,     KC_PGUP,   KC_PAST,
    KC_F5,  KC_F6, KC_LCTL,  KC_A,    KC_S,   KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,          KC_ENT,    KC_LEFT,   KC_P5,     KC_RGHT,   KC_PMNS,
    KC_F7,  KC_F8, KC_LSFT,           KC_Z,   KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT, KC_BSLS,   KC_END,    KC_DOWN,   KC_PGDN,   KC_PPLS,
    KC_F9,  KC_F10,KC_LALT,         KC_GRV,                                 KC_SPC,                                            KC_CAPS,       KC_INS,           KC_DEL
    ),
    [_NUMLOCK] = LAYOUT(
    KC_F1,  KC_F2, KC_ESC,   KC_1,    KC_2,   KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC,   TG(_NUMLOCK),     MO(_EXTRA),    KC_RGUI,
    KC_F3,  KC_F4, KC_TAB,   KC_Q,    KC_W,   KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC,            KC_P7,   KC_P8,       KC_P9,     KC_PAST,
    KC_F5,  KC_F6, KC_LCTL,  KC_A,    KC_S,   KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,          KC_ENT,    KC_P4,   KC_P5,       KC_P6,     KC_PMNS,
    KC_F7,  KC_F8, KC_LSFT,           KC_Z,   KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT, KC_BSLS,   KC_P1,   KC_P2,       KC_P3,     KC_PPLS,
    KC_F9,  KC_F10,KC_LALT,         KC_GRV,                                 KC_SPC,                                            KC_CAPS,       KC_P0,            KC_PDOT
    ),
    [_EXTRA] = LAYOUT(
    KC_NO,  KC_NO, KC_NO,   KC_1,    KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,    KC_NO,  KC_NO,   KC_NO,   KC_NO,     KC_NO,            KC_NO,         KC_NO,
    KC_NO,  KC_NO, KC_NO,   KC_Q,    KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,    KC_NO,  KC_NO,   KC_NO,              KC_NO,   KC_NO,       KC_NO,     KC_MUTE,
    KC_NO,  KC_NO, KC_NO,   KC_A,    KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,    KC_NO,  KC_NO,            KC_NO,     KC_NO,   KC_NO,       KC_NO,     KC_VOLD,
    KC_NO,  KC_NO, KC_NO,            KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,    KC_NO,  KC_NO,   KC_NO,   KC_NO,     KC_NO,   KC_NO,       KC_NO,     KC_VOLU,
    KC_NO,  KC_NO, KC_NO,            KC_NO,                                 QK_BOOT,                                           KC_NO,         KC_BRID,          KC_BRIU
    )
};

/*
    K35,    K34,   KA7,		 KA6,     KA5,	  KA4,     KA3,     KA2,     KA1,     KA0,     K97,     K96,     K95,     K94,     K93,     K92,	   K57,        K23,			  K22, \
    K33,    K32,   K91,      K90,     K87, 	  K86,     K85,     K84,     K83,     K82,     K81,     K80,     K77,     K76,     K75,				   K21,     K20,     K17,     K16, \
    K31,    K30,   K73,      K72,     K71, 	  K70,     K67,     K66,     K65,     K64,     K63,     K62,     K61,     K60,				K74,       K15,     K14,     K13,     K12, \
    K27,    K26,   K56,               K54, 	  K53,     K52,     K51,     K50,     K47,     K46,     K45,     K44,     K43,     K42,     K41,       K11,     K10,     K07,     K04, \
    K25,    K24,   K40,            K55,                                    K37,											   K36,           K06,              K05 \
*/

// bool process_record_user(uint16_t keycode, keyrecord_t *record) {
//     switch (keycode) {
//     case KC_RGUI:
//         if (record->event.pressed) {
//             writePinLow(LED_SCROLL_LOCK_PIN);
//         } else {
//             writePinHigh(LED_SCROLL_LOCK_PIN);
//         }
//         return true;
//     }
//     return true;
// }

layer_state_t layer_state_set_kb(layer_state_t state) {
    if (IS_LAYER_ON_STATE(state, _NUMLOCK)) {
        writePinLow(LED_NUM_LOCK_PIN);
    }
    else {
        writePinHigh(LED_NUM_LOCK_PIN);
    }

    if (IS_LAYER_ON_STATE(state, _EXTRA)) {
        writePinLow(LED_SCROLL_LOCK_PIN);
    }
    else {
        writePinHigh(LED_SCROLL_LOCK_PIN);
    }

    return state;
}
