/*
Copyright 2012 Jun Wako <wakojun@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include QMK_KEYBOARD_H

/* Scan Code Set 1
        ,-----------------------------------------------.
        |F13|F14|F15|F16|F17|F18|F19|F20|F21|F22|F23|F24|
,---.   |-----------------------------------------------|     ,-----------.     ,-----------.
|Esc|   |F1 |F2 |F3 |F4 |F5 |F6 |F7 |F8 |F9 |F10|F11|F12|     |PrS|ScL|Pau|     |VDn|VUp|Mut|
`---'   `-----------------------------------------------'     `-----------'     `-----------'
,-----------------------------------------------------------. ,-----------. ,---------------.
|  `|  1|  2|  3|  4|  5|  6|  7|  8|  9|  0|  -|  =|JPY|Bsp| |Ins|Hom|PgU| |NmL|  /|  *|  -|
|-----------------------------------------------------------| |-----------| |---------------|
|Tab  |  Q|  W|  E|  R|  T|  Y|  U|  I|  O|  P|  [|  ]|  \  | |Del|End|PgD| |  7|  8|  9|  +|
|-----------------------------------------------------------| `-----------' |---------------|
|CapsL |  A|  S|  D|  F|  G|  H|  J|  K|  L|  ;|  '|  #|Entr|               |  4|  5|  6|KP,|
|-----------------------------------------------------------|     ,---.     |---------------|
|Shft|  <|  Z|  X|  C|  V|  B|  N|  M|  ,|  .|  /| RO|Shift |     |Up |     |  1|  2|  3|Ent|
|-----------------------------------------------------------| ,-----------. |---------------|
|Ctl|Gui|Alt|MHEN|     Space      |HENK|KANA|Alt|Gui|App|Ctl| |Lef|Dow|Rig| |      0|  .|KP=|
`-----------------------------------------------------------' `-----------' `---------------' */

/* const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    LAYOUT_scanset_1(
                  KC_F13, KC_F14, KC_F15, KC_F16, KC_F17, KC_F18, KC_F19, KC_F20, KC_F21, KC_F22, KC_F23, KC_F24,
        KC_ESC,   KC_F1,  KC_F2,  KC_F3,  KC_F4,  KC_F5,  KC_F6,  KC_F7,  KC_F8,  KC_F9,  KC_F10, KC_F11, KC_F12,           KC_PSCR, KC_SCRL, KC_PAUS,            KC_VOLD, KC_VOLU, KC_MUTE,

        KC_GRV,  KC_1,    KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_INT3, KC_BSPC,   KC_INS,  KC_HOME, KC_PGUP,   KC_NUM,  KC_PSLS, KC_PAST, KC_PMNS,
        KC_TAB,  KC_Q,    KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I, KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS,            KC_DEL,  KC_END,  KC_PGDN,   KC_P7,   KC_P8,   KC_P9,   KC_PPLS,
        KC_CAPS, KC_A,    KC_S, KC_D, KC_F, KC_G, KC_H, KC_J, KC_K, KC_L,    KC_SCLN, KC_QUOT, KC_NUHS, KC_ENT,                                          KC_P4,   KC_P5,   KC_P6,   KC_PCMM,
        KC_LSFT, KC_NUBS, KC_Z, KC_X, KC_C, KC_V, KC_B, KC_N, KC_M, KC_COMM, KC_DOT,  KC_SLSH, KC_INT1, KC_RSFT,                     KC_UP,              KC_P1,   KC_P2,   KC_P3,   KC_PEQL,
        KC_LCTL, KC_LGUI, KC_LALT, KC_INT5,     KC_SPC,     KC_INT4, KC_INT2, KC_RALT, KC_RGUI, KC_APP, KC_RCTL,            KC_LEFT, KC_DOWN, KC_RGHT,   KC_P0,            KC_PDOT, KC_PENT
    ),
}; */

/* Scan Code Set 2
         ,-----------------------------------------------.
         |F13|F14|F15|F16|F17|F18|F19|F20|F21|F22|F23|F24|
 ,---.   |-----------------------------------------------|     ,-----------.     ,-----------.
 |Esc|   |F1 |F2 |F3 |F4 |F5 |F6 |F7 |F8 |F9 |F10|F11|F12|     |PrS|ScL|Pau|     |VDn|VUp|Mut|
 `---'   `-----------------------------------------------'     `-----------'     `-----------'
 ,-----------------------------------------------------------. ,-----------. ,---------------.
 |  `|  1|  2|  3|  4|  5|  6|  7|  8|  9|  0|  -|  =|JPY|Bsp| |Ins|Hom|PgU| |NmL|  /|  *|  -|
 |-----------------------------------------------------------| |-----------| |---------------|
 |Tab  |  Q|  W|  E|  R|  T|  Y|  U|  I|  O|  P|  [|  ]|  \  | |Del|End|PgD| |  7|  8|  9|  +|
 |-----------------------------------------------------------| `-----------' |---------------|
 |CapsL |  A|  S|  D|  F|  G|  H|  J|  K|  L|  ;|  '|XXX|Entr|               |  4|  5|  6|KP,|
 |-----------------------------------------------------------|     ,---.     |---------------|
 |Shft|  <|  Z|  X|  C|  V|  B|  N|  M|  ,|  .|  /| RO|Shift |     |Up |     |  1|  2|  3|Ent|
 |-----------------------------------------------------------| ,-----------. |---------------|
 |Ctl|Gui|Alt|MHEN|     Space      |HENK|KANA|Alt|Gui|App|Ctl| |Lef|Dow|Rig| |  #|  0|  .|KP=|
 `-----------------------------------------------------------' `-----------' `---------------' */

/* const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    LAYOUT_scanset_2(
                  KC_F13, KC_F14, KC_F15, KC_F16, KC_F17, KC_F18, KC_F19, KC_F20, KC_F21, KC_F22, KC_F23, KC_F24,
        KC_ESC,   KC_F1,  KC_F2,  KC_F3,  KC_F4,  KC_F5,  KC_F6,  KC_F7,  KC_F8,  KC_F9,  KC_F10, KC_F11, KC_F12,           KC_PSCR, KC_SCRL, KC_PAUS,            KC_VOLD, KC_VOLU, KC_MUTE,

        KC_GRV,  KC_1,    KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_INT3, KC_BSPC,   KC_INS,  KC_HOME, KC_PGUP,   KC_NUM,  KC_PSLS, KC_PAST, KC_PMNS,
        KC_TAB,  KC_Q,    KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I, KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS,            KC_DEL,  KC_END,  KC_PGDN,   KC_P7,   KC_P8,   KC_P9,   KC_PPLS,
        KC_CAPS, KC_A,    KC_S, KC_D, KC_F, KC_G, KC_H, KC_J, KC_K, KC_L,    KC_SCLN, KC_QUOT,          KC_ENT,                                          KC_P4,   KC_P5,   KC_P6,   KC_PCMM,
        KC_LSFT, KC_NUBS, KC_Z, KC_X, KC_C, KC_V, KC_B, KC_N, KC_M, KC_COMM, KC_DOT,  KC_SLSH, KC_INT1, KC_RSFT,                     KC_UP,              KC_P1,   KC_P2,   KC_P3,   KC_PEQL,
        KC_LCTL, KC_LGUI, KC_LALT, KC_INT5,     KC_SPC,     KC_INT4, KC_INT2, KC_RALT, KC_RGUI, KC_APP, KC_RCTL,            KC_LEFT, KC_DOWN, KC_RGHT,   KC_NUHS, KC_P0,   KC_PDOT, KC_PENT
    ),
}; */

/* Scan Code Set 3
              ,-----------------------------------------------.
              |F13|F14|F15|F16|F17|F18|F19|F20|F21|F22|F23|F24|
              |-----------------------------------------------|
              |F1 |F2 |F3 |F4 |F5 |F6 |F7 |F8 |F9 |F10|F11|F12|
              `-----------------------------------------------'
,-------. ,-----------------------------------------------------------. ,-----------. ,---------------.
|Mut|HEN| |  `|  1|  2|  3|  4|  5|  6|  7|  8|  9|  0|  -|  =|JPY| BS| |  /|PgU|PgD| |Esc|NmL|ScL|  *|
|-------| |-----------------------------------------------------------| |-----------| |---------------|
|VUp|Pau| |Tab  |  Q|  W|  E|  R|  T|  Y|  U|  I|  O|  P|  [|  ]|    \| |End|Ins|Del| |  7|  8|  9|  +|
|-------| |-----------------------------------------------------------| `-----------' |-----------|---|
|VDn|MHE| |CapsL |  A|  S|  D|  F|  G|  H|  J|  K|  L|  ;|  '|  #| Ret| |XXX|Up |XXX| |  4|  5|  6|  -|
|-------| |-----------------------------------------------------------| ,-----------. |---------------|
|PrS|App| |Shif|  \|  Z|  X|  C|  V|  B|  N|  M|  ,|  ,|  /| RO| Shift| |Lef|Hom|Rig| |  1|  2|  3|Ent|
|-------| |-----------------------------------------------------------| `-----------' |-----------|---|
|Gui|Gui| |Ctrl|    | Alt | XXX |  Space  | XXX | KANA| Alt |    |Ctrl| |XXX|Dow|XXX| |  ,|  0|  .|  =|
`-------' `----'    `---------------------------------------'    `----'  ---`---'---  `---------------' */

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    LAYOUT_scanset_3(
                        KC_F13, KC_F14, KC_F15, KC_F16, KC_F17, KC_F18, KC_F19, KC_F20, KC_F21, KC_F22, KC_F23, KC_F24,
                        KC_F1,  KC_F2,  KC_F3,  KC_F4,  KC_F5,  KC_F6,  KC_F7,  KC_F8,  KC_F9,  KC_F10, KC_F11, KC_F12,

    KC_MUTE, KC_INT4,   KC_GRV,  KC_1,    KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_INT3, KC_BSPC,   KC_PSLS, KC_PGUP, KC_PGDN,   KC_ESC,  KC_NUM, KC_SCRL, KC_PAST,
    KC_VOLU, KC_PAUS,   KC_TAB,  KC_Q,    KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I, KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS,            KC_END,  KC_INS,  KC_DEL,    KC_P7,   KC_P8,  KC_P9,   KC_PPLS,
    KC_VOLD, KC_INT5,   KC_CAPS, KC_A,    KC_S, KC_D, KC_F, KC_G, KC_H, KC_J, KC_K, KC_L,    KC_SCLN, KC_QUOT, KC_NUHS, KC_ENT,                      KC_UP,              KC_P4,   KC_P5,  KC_P6,   KC_PMNS,
    KC_PSCR, KC_APP,    KC_LSFT, KC_NUBS, KC_Z, KC_X, KC_C, KC_V, KC_B, KC_N, KC_M, KC_COMM, KC_DOT,  KC_SLSH, KC_INT1, KC_RSFT,            KC_LEFT, KC_HOME, KC_RGHT,   KC_P1,   KC_P2,  KC_P3,   KC_PENT,
    KC_LGUI, KC_RGUI,   KC_LCTL,        KC_LALT,                     KC_SPC,                    KC_INT2, KC_RALT,       KC_RCTL,                    KC_DOWN,            KC_PCMM, KC_P0,  KC_PDOT, KC_PEQL
    ),
};
