#include QMK_KEYBOARD_H

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
    LAYOUT(
      KC_F1,  KC_F2, KC_ESC,   KC_1,    KC_2,   KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC,   KC_NUM,         KC_SCRL,         KC_SYRQ,
      KC_F3,  KC_F4, KC_TAB,   KC_Q,    KC_W,   KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC,            KC_HOME,   KC_UP,     KC_PGUP,   KC_PAST,
      KC_F5,  KC_F6, KC_LCTL,  KC_A,    KC_S,   KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,          KC_ENT,    KC_LEFT,   KC_P5,     KC_RGHT,   KC_PMNS,
      KC_F7,  KC_F8, KC_LSFT,           KC_Z,   KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT, KC_BSLS,   KC_END,    KC_DOWN,   KC_PGDN,   KC_PPLS,
      KC_F9,  KC_F10,KC_LALT,         KC_GRV,                              KC_SPC,                                     KC_CAPS,               KC_INS,        KC_DEL
    )
};
