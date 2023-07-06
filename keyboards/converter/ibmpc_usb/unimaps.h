/*
Copyright 2019 Markus Fritsche <fritsche.markus@gmail.com>

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

/*
   these matrices map the scan codes as produced by matrix_scan to
   the artificial layout as defined by ibmpc_usb.h LAYOUT.
   Each codeset (1, 2, and 3) is mapped to 127 keys of LAYOUT.

   There are some quirks with prefixed scancodes as shown in
   process_cs1, process_cs2, and process_cs3.

   This is done to use XT- (codeset 1), AT- (codeset 2),
   and terminal-keyboards without recompiling the keymap and
   to potentially support VIA in the future.
*/
#pragma once
#include "ibmpc_usb.h"

// See https://github.com/tmk/tmk_keyboard/blob/master/tmk_core/common/unimap.h for translation of UNIMAP_X to hex code

/* Original Mapping
    { UNIMAP_NO,    UNIMAP_ESC,   UNIMAP_1,     UNIMAP_2,     UNIMAP_3,     UNIMAP_4,     UNIMAP_5,     UNIMAP_6,       // 00-07
      UNIMAP_7,     UNIMAP_8,     UNIMAP_9,     UNIMAP_0,     UNIMAP_MINUS, UNIMAP_EQUAL, UNIMAP_BSPACE,UNIMAP_TAB   }, // 08-0F
    { UNIMAP_Q,     UNIMAP_W,     UNIMAP_E,     UNIMAP_R,     UNIMAP_T,     UNIMAP_Y,     UNIMAP_U,     UNIMAP_I,       // 10-17
      UNIMAP_O,     UNIMAP_P,     UNIMAP_LBRC,  UNIMAP_RBRC,  UNIMAP_ENTER, UNIMAP_LCTL,  UNIMAP_A,     UNIMAP_S,    }, // 18-1F
    { UNIMAP_D,     UNIMAP_F,     UNIMAP_G,     UNIMAP_H,     UNIMAP_J,     UNIMAP_K,     UNIMAP_L,     UNIMAP_SCLN,    // 20-27
      UNIMAP_QUOTE, UNIMAP_GRAVE, UNIMAP_LSHIFT,UNIMAP_BSLASH,UNIMAP_Z,     UNIMAP_X,     UNIMAP_C,     UNIMAP_V,    }, // 28-2F
    { UNIMAP_B,     UNIMAP_N,     UNIMAP_M,     UNIMAP_COMMA, UNIMAP_DOT,   UNIMAP_SLASH, UNIMAP_RSHIFT,UNIMAP_PAST,    // 30-37
      UNIMAP_LALT,  UNIMAP_SPACE, UNIMAP_CAPS,  UNIMAP_F1,    UNIMAP_F2,    UNIMAP_F3,    UNIMAP_F4,    UNIMAP_F5    }, // 38-3F
    { UNIMAP_F6,    UNIMAP_F7,    UNIMAP_F8,    UNIMAP_F9,    UNIMAP_F10,   UNIMAP_NLCK,  UNIMAP_SLCK,  UNIMAP_P7,      // 40-47
      UNIMAP_P8,    UNIMAP_P9,    UNIMAP_PMNS,  UNIMAP_P4,    UNIMAP_P5,    UNIMAP_P6,    UNIMAP_PPLS,  UNIMAP_P1    }, // 48-4F
    { UNIMAP_P2,    UNIMAP_P3,    UNIMAP_P0,    UNIMAP_PDOT,  UNIMAP_PSCR,  UNIMAP_PAUSE, UNIMAP_NUBS,  UNIMAP_F11,     // 50-57
      UNIMAP_F12,   UNIMAP_PEQL,  UNIMAP_LGUI,  UNIMAP_RGUI,  UNIMAP_APP,   UNIMAP_MUTE,  UNIMAP_VOLD,  UNIMAP_VOLU  }, // 58-5F
    { UNIMAP_UP,    UNIMAP_LEFT,  UNIMAP_DOWN,  UNIMAP_RIGHT, UNIMAP_F13,   UNIMAP_F14,   UNIMAP_F15,   UNIMAP_F16,     // 60-67
      UNIMAP_F17,   UNIMAP_F18,   UNIMAP_F19,   UNIMAP_F20,   UNIMAP_F21,   UNIMAP_F22,   UNIMAP_F23,   UNIMAP_PENT  }, // 68-6F
    { UNIMAP_KANA,  UNIMAP_INSERT,UNIMAP_DELETE,UNIMAP_RO,    UNIMAP_HOME,  UNIMAP_END,   UNIMAP_F24,   UNIMAP_PGUP,    // 70-77
      UNIMAP_PGDN,  UNIMAP_HENK,  UNIMAP_RCTL,  UNIMAP_MHEN,  UNIMAP_RALT,  UNIMAP_JYEN,  UNIMAP_PCMM,  UNIMAP_PSLS  }, // 78-7F
};
*/

const uint8_t map_cs1[MATRIX_ROWS][MATRIX_COLS] = {
    { XXX , 0x1a, 0x1c, 0x1d, 0x1e, 0x1f, 0x20, 0x21, \
    0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x29, 0x33 }, /* 08-0F */
    { 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x3b, \
    0x3c, 0x3d, 0x3e, 0x3f, 0x57, 0x76, 0x4b, 0x4c }, /* 18-1F */
    { 0x4d, 0x4e, 0x4f, 0x50, 0x51, 0x52, 0x53, 0x54, \
    0x55, 0x1b, 0x5f, 0x56, 0x61, 0x62, 0x63, 0x64 }, /* 28-2F */
    { 0x65, 0x66, 0x67, 0x68, 0x69, 0x6a, 0x6c, 0x2f, \
    0x77, 0x78, 0x4a, 0x0d, 0x0e, 0x0f, 0x10, 0x11 }, /* 38-3F */
    { 0x12, 0x13, 0x14, 0x15, 0x16, 0x2d, 0x31, 0x44, \
    0x45, 0x46, 0x30, 0x59, 0x5a, 0x5b, 0x47, 0x70 }, /* 48-4F */
    { 0x71, 0x72, 0x7d, 0x7e, 0x5e, 0x48, 0x60, 0x17, \
    0x18, XXX , 0x75, 0x74, 0x5d, XXX , XXX , XXX  }, /* 58-5F */
    { 0x58, 0x6d, 0x7b, 0x6f, 0x01, 0x02, 0x03, 0x04, \
    0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x73 }, /* 68-6F */
    { XXX , 0x2a, 0x41, XXX , 0x2b, 0x42, 0x0c, 0x2c, \
    0x43, XXX , 0x7a, XXX , 0x79, XXX,  0x5c, 0x2e }, /* 78-7F */
};

/*
 const uint8_t PROGMEM unimap_cs2[MATRIX_ROWS][MATRIX_COLS] = {
    { UNIMAP_PAUS,  UNIMAP_F9,    UNIMAP_F7,    UNIMAP_F5,    UNIMAP_F3,    UNIMAP_F1,    UNIMAP_F2,    UNIMAP_F12,     // 00-07
      UNIMAP_F13,   UNIMAP_F10,   UNIMAP_F8,    UNIMAP_F6,    UNIMAP_F4,    UNIMAP_TAB,   UNIMAP_GRV,   UNIMAP_RALT  }, // 08-0F
    { UNIMAP_F14,   UNIMAP_LALT,  UNIMAP_LSHIFT,UNIMAP_KANA,  UNIMAP_LCTL,  UNIMAP_Q,     UNIMAP_1,     UNIMAP_LGUI,    // 10-17
      UNIMAP_F15,   UNIMAP_RCTL,  UNIMAP_Z,     UNIMAP_S,     UNIMAP_A,     UNIMAP_W,     UNIMAP_2,     UNIMAP_RGUI  }, // 18-1F
    { UNIMAP_F16,   UNIMAP_C,     UNIMAP_X,     UNIMAP_D,     UNIMAP_E,     UNIMAP_4,     UNIMAP_3,     UNIMAP_APP,     // 20-27
      UNIMAP_F17,   UNIMAP_SPACE, UNIMAP_V,     UNIMAP_F,     UNIMAP_T,     UNIMAP_R,     UNIMAP_5,     UNIMAP_HOME  }, // 28-2F
    { UNIMAP_F18,   UNIMAP_N,     UNIMAP_B,     UNIMAP_H,     UNIMAP_G,     UNIMAP_Y,     UNIMAP_6,     UNIMAP_DEL,     // 30-37
      UNIMAP_F19,   UNIMAP_INS,   UNIMAP_M,     UNIMAP_J,     UNIMAP_U,     UNIMAP_7,     UNIMAP_8,     UNIMAP_DOWN  }, // 38-3F
    { UNIMAP_F20,   UNIMAP_COMMA, UNIMAP_K,     UNIMAP_I,     UNIMAP_O,     UNIMAP_0,     UNIMAP_9,     UNIMAP_RIGHT,   // 40-47
      UNIMAP_F21,   UNIMAP_DOT,   UNIMAP_SLASH, UNIMAP_L,     UNIMAP_SCOLON,UNIMAP_P,     UNIMAP_MINUS, UNIMAP_UP    }, // 48-4F
    { UNIMAP_F22,   UNIMAP_RO,    UNIMAP_QUOTE, UNIMAP_LEFT,  UNIMAP_LBRC,  UNIMAP_EQUAL, UNIMAP_PGDN,  UNIMAP_F23,     // 50-57
      UNIMAP_CAPS,  UNIMAP_RSHIFT,UNIMAP_ENTER, UNIMAP_RBRC,  UNIMAP_END,   UNIMAP_BSLASH,UNIMAP_PGUP,  UNIMAP_F24   }, // 58-5F
    { UNIMAP_PSLS,  UNIMAP_NUBS,  UNIMAP_PENT,  UNIMAP_PEQL,  UNIMAP_HENK,  UNIMAP_VOLD,  UNIMAP_BSPACE,UNIMAP_MHEN,    // 60-67
      UNIMAP_NUHS,  UNIMAP_P1,    UNIMAP_JYEN,  UNIMAP_P4,    UNIMAP_P7,    UNIMAP_PCMM,  UNIMAP_VOLU,  UNIMAP_MUTE  }, // 68-6F
    { UNIMAP_P0,    UNIMAP_PDOT,  UNIMAP_P2,    UNIMAP_P5,    UNIMAP_P6,    UNIMAP_P8,    UNIMAP_ESC,   UNIMAP_NLCK,    // 70-77
      UNIMAP_F11,   UNIMAP_PPLS,  UNIMAP_P3,    UNIMAP_PMNS,  UNIMAP_PAST,  UNIMAP_P9,    UNIMAP_SLCK,  UNIMAP_PSCR  }, // 78-7F
};
 */

const uint8_t map_cs2[MATRIX_ROWS][MATRIX_COLS] = {
    { 0x48, 0x42, 0x40, 0x3e, 0x3c, 0x3a, 0x3b, 0x45  ,
      0x68, 0x43, 0x41, 0x3f, 0x3d, 0x2b, 0x35, 0x7e }, \
    { 0x69, 0x7a, 0x79, 0x00, 0x78, 0x14, 0x1e, 0x7b  ,
      0x6a, 0x7c, 0x1d, 0x16, 0x04, 0x1a, 0x1f, 0x7f }, \
    { 0x6b, 0x06, 0x1b, 0x07, 0x08, 0x21, 0x20, 0x65  ,
      0x6c, 0x2c, 0x19, 0x09, 0x17, 0x15, 0x16, 0x4a }, \
    { 0x6d, 0x11, 0x05, 0x0b, 0x0a, 0x1c, 0x23, 0x4c  ,
      0x6e, 0x49, 0x10, 0x0d, 0x18, 0x24, 0x25, 0x51 }, \
    { 0x6f, 0x36, 0x0e, 0x0c, 0x12, 0x26, 0x26, 0x4f  ,
      0x70, 0x37, 0x38, 0x0f, 0x33, 0x13, 0x2d, 0x52 }, \
    { 0x71, 0x75, 0x34, 0x50, 0x2f, 0x2e, 0x4e, 0x72  ,
      0x39, 0x7d, 0x28, 0x30, 0x40, 0x31, 0x4b, 0x73 }, \
    { 0x54, 0x64, 0x58, 0x67, 0x76, 0x01, 0x2a, 0x77  ,
      0x32, 0x59, 0x74, 0x5c, 0x5f, 0x66, 0x02, 0x03 }, \
    { 0x62, 0x63, 0x5a, 0x5d, 0x5e, 0x60, 0x29, 0x53  ,
      0x44, 0x57, 0x5b, 0x56, 0x55, 0x61, 0x47, 0x46 },
};


/*
 const uint8_t PROGMEM unimap_cs3[MATRIX_ROWS][MATRIX_COLS] = {
    { UNIMAP_KANA,  UNIMAP_LGUI,  UNIMAP_PSCR,  UNIMAP_VOLD,  UNIMAP_VOLU,  UNIMAP_MUTE,  UNIMAP_HENK,  UNIMAP_F1,      // 00-07
      UNIMAP_F13,   UNIMAP_RGUI,  UNIMAP_APP,   UNIMAP_MHEN,  UNIMAP_PAUS,  UNIMAP_TAB,   UNIMAP_GRV,   UNIMAP_F2    }, // 08-0F
    { UNIMAP_F14,   UNIMAP_LCTL,  UNIMAP_LSHIFT,UNIMAP_NUBS,  UNIMAP_CAPS,  UNIMAP_Q,     UNIMAP_1,     UNIMAP_F3,      // 10-17
      UNIMAP_F15,   UNIMAP_LALT,  UNIMAP_Z,     UNIMAP_S,     UNIMAP_A,     UNIMAP_W,     UNIMAP_2,     UNIMAP_F4    }, // 18-1F
    { UNIMAP_F16,   UNIMAP_C,     UNIMAP_X,     UNIMAP_D,     UNIMAP_E,     UNIMAP_4,     UNIMAP_3,     UNIMAP_F5,      // 20-27
      UNIMAP_F17,   UNIMAP_SPACE, UNIMAP_V,     UNIMAP_F,     UNIMAP_T,     UNIMAP_R,     UNIMAP_5,     UNIMAP_F6    }, // 28-2F
    { UNIMAP_F18,   UNIMAP_N,     UNIMAP_B,     UNIMAP_H,     UNIMAP_G,     UNIMAP_Y,     UNIMAP_6,     UNIMAP_F7,      // 30-37
      UNIMAP_F19,   UNIMAP_RALT,  UNIMAP_M,     UNIMAP_J,     UNIMAP_U,     UNIMAP_7,     UNIMAP_8,     UNIMAP_F8    }, // 38-3F
    { UNIMAP_F20,   UNIMAP_COMMA, UNIMAP_K,     UNIMAP_I,     UNIMAP_O,     UNIMAP_0,     UNIMAP_9,     UNIMAP_F9,      // 40-47
      UNIMAP_F21,   UNIMAP_DOT,   UNIMAP_SLASH, UNIMAP_L,     UNIMAP_SCOLON,UNIMAP_P,     UNIMAP_MINUS, UNIMAP_F10   }, // 48-4F
    { UNIMAP_F22,   UNIMAP_RO,    UNIMAP_QUOTE, UNIMAP_NUHS,  UNIMAP_LBRC,  UNIMAP_EQUAL, UNIMAP_F11,   UNIMAP_F23,     // 50-57
      UNIMAP_RCTL,  UNIMAP_RSHIFT,UNIMAP_ENTER, UNIMAP_RBRC,  UNIMAP_BSLASH,UNIMAP_JYEN,  UNIMAP_F12,   UNIMAP_F24   }, // 58-5F
    { UNIMAP_DOWN,  UNIMAP_LEFT,  UNIMAP_HOME,  UNIMAP_UP,    UNIMAP_END,   UNIMAP_INS,   UNIMAP_BSPACE,UNIMAP_PSLS,    // 60-67
      UNIMAP_PCMM,  UNIMAP_P1,    UNIMAP_RIGHT, UNIMAP_P4,    UNIMAP_P7,    UNIMAP_DEL,   UNIMAP_PGUP,  UNIMAP_PGDN  }, // 68-6F
    { UNIMAP_P0,    UNIMAP_PDOT,  UNIMAP_P2,    UNIMAP_P5,    UNIMAP_P6,    UNIMAP_P8,    UNIMAP_ESC,   UNIMAP_NLCK,    // 70-77
      UNIMAP_PEQL,  UNIMAP_PENT,  UNIMAP_P3,    UNIMAP_PMNS,  UNIMAP_PPLS,  UNIMAP_P9,    UNIMAP_SLCK,  UNIMAP_PAST  }, // 78-7F
};
 */

const uint8_t map_cs3[MATRIX_ROWS][MATRIX_COLS] = {
    { XXX , 0x74, 0x5e, 0x48, 0x31, 0x19, 0x1a, 0x0d, \
    0x01, 0x75, 0x5d, 0x49, 0x32, 0x33, 0x1b, 0x0e }, /* 08-0F */
    { 0x02, 0x76, 0x5f, 0x60, 0x4a, 0x34, 0x1c, 0x0f, \
    0x03, 0x77, 0x61, 0x4c, 0x4b, 0x35, 0x1d, 0x10 }, /* 18-1F */
    { 0x04, 0x63, 0x62, 0x4d, 0x36, 0x1f, 0x1e, 0x11, \
    0x05, 0x78, 0x64, 0x4e, 0x38, 0x37, 0x20, 0x12 }, /* 28-2F */
    { 0x06, 0x66, 0x65, 0x50, 0x4f, 0x39, 0x21, 0x13, \
    0x07, 0x79, 0x67, 0x51, 0x3a, 0x22, 0x23, 0x14 }, /* 38-3F */
    { 0x08, 0x68, 0x52, 0x3b, 0x3c, 0x25, 0x24, 0x15, \
    0x09, 0x69, 0x6a, 0x53, 0x54, 0x3d, 0x26, 0x16 }, /* 48-4F */
    { 0x0a, 0x6b, 0x55, 0x56, 0x3e, 0x27, 0x17, 0x0b, \
    0x7a, 0x6c, 0x57, 0x3f, 0x40, 0x28, 0x18, 0x0c }, /* 58-5F */
    { 0x7b, 0x6d, 0x2b, 0x58, 0x41, 0x42, 0x29, 0x2a, \
    0x5c, 0x70, 0x6f, 0x59, 0x44, 0x43, 0x2b, 0x2c }, /* 68-6F */
    { 0x7d, 0x7e, 0x71, 0x5a, 0x5b, 0x45, 0x2d, 0x2e, \
    0x7f, 0x73, 0x72, 0x5c, 0x47, 0x46, 0x2f, 0x30 }, /* 78-7F */
};
