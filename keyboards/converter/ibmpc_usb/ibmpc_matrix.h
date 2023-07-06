#pragma once

#include <avr/pgmspace.h>
#include "matrix.h"
#include "ibmpc.h"
#include "unimaps.h"

#define ID_STR(id)  (id == 0xFFFF ? "_NONE" : \
                    (id == 0xFFFE ? "_ERROR" : \
                    (id == 0xFFFD ? "_Z150" : \
                    (id == 0xFFFC ? "_IBM" : \
                    (id == 0xFFFB ? "_CLONE" : \
                    (id == 0x0000 ? "_IBM84" : \
                     ""))))))

#define ROW(code)      ((code>>4)&0x07)
#define COL(code)      (code&0x0F)

#ifdef IBMPC_MOUSE_ENABLE
extern uint8_t ibmpc_mouse_buttons(void);
#endif

typedef enum { NONE, PC_XT, PC_AT, PC_TERMINAL, PC_MOUSE } keyboard_kind_t;
#define KEYBOARD_KIND_STR(kind) \
    (kind == PC_XT ? "XT" :   \
     kind == PC_AT ? "AT" :   \
     kind == PC_TERMINAL ? "TERMINAL" :   \
     kind == PC_MOUSE ? "MOUSE" :   \
     "NONE")

static matrix_row_t matrix[MATRIX_ROWS];

/******** States *******/
typedef enum {
    INIT,
    WAIT_SETTLE,
    AT_RESET,
    XT_RESET,
    XT_RESET_WAIT,
    XT_RESET_DONE,
    WAIT_AA,
    WAIT_AABF,
    WAIT_AABFBF,
    READ_ID,
    SETUP,
    LOOP,
    ERROR,
    ERROR_PARITY_AA,
} Converter_state;

typedef enum {
    CS1_INIT,
    CS1_E0,
    // Pause: E1 1D 45, E1 9D C5 [a]
    CS1_E1,
    CS1_E1_1D,
    CS1_E1_9D,
} CS1_state;

typedef enum {
    CS2_INIT,
    CS2_F0,
    CS2_E0,
    CS2_E0_F0,
    // Pause
    CS2_E1,
    CS2_E1_14,
    CS2_E1_F0,
    CS2_E1_F0_14,
    CS2_E1_F0_14_F0,
#ifdef CS2_80CODE_SUPPORT
    CS2_80,
    CS2_80_F0,
#endif
} CS2_state;

typedef enum {
    CS3_READY,
    CS3_F0,
#ifdef G80_2551_SUPPORT
    // G80-2551 four extra keys around cursor keys
    CS3_G80,
    CS3_G80_F0,
#endif
} CS3_state;
/***********************/


/******* functions/"methods" independent of IBMPCConverter *******/
uint8_t cs1_e0code(uint8_t);
uint8_t translate_5576_cs2(uint8_t);
#ifdef CS2_80CODE_SUPPORT
uint8_t cs2_80code(uint8_t);
#endif
uint8_t translate_5576_cs3(uint8_t);
uint8_t translate_televideo_dec_cs3(uint8_t);
// NOTE: Below is unused...
uint8_t ibmpcconverter_translate_5576_cs2_e0(uint8_t);
/*******************************************************/

typedef struct {
    IBMPC_t* ibmpc;
    uint16_t keyboard_id;
    keyboard_kind_t keyboard_kind;
    uint8_t current_protocol;
    uint16_t init_time;

    /* state */
    Converter_state state;
    CS1_state state_cs1;
    CS2_state state_cs2;
    CS3_state state_cs3;

} IBMPCConverter_t;

/******* "constructor" *******/
void ibmpcconverter_init(IBMPCConverter_t* self, IBMPC_t* ibmpc) {

    self->keyboard_id = 0x0000;
    self->keyboard_kind = NONE;
    self->current_protocol = 0;
    self->state = INIT;
    self->state_cs1 = CS1_INIT;
    self->state_cs2 = CS2_INIT;
    self->state_cs3 = CS3_READY;
    for (uint8_t i=0; i < MATRIX_ROWS; i++) matrix[i] = 0x00; /*matrix_clear()*/
    self->ibmpc = ibmpc;
    ibmpc_host_init(self->ibmpc);
}
/*****************************/

/******* "methods" *******/
void ibmpcconverter_set_led(IBMPCConverter_t*, uint8_t); // special case - used as a function pointer
uint8_t ibmpcconverter_process_interface(IBMPCConverter_t*);
int8_t ibmpcconverter_process_cs1(IBMPCConverter_t*, uint8_t);
int8_t ibmpcconverter_process_cs2(IBMPCConverter_t*, uint8_t);
int8_t ibmpcconverter_process_cs3(IBMPCConverter_t*, uint8_t);
uint8_t ibmpcconverter_cs2_e0code(IBMPCConverter_t*, uint8_t);
int16_t ibmpcconverter_read_wait(IBMPCConverter_t*, uint16_t);
uint16_t ibmpcconverter_read_keyboard_id(IBMPCConverter_t*);
/*************************/

/********** Matrix Ops **********/
uint8_t ibmpcconverter_to_unimap(IBMPCConverter_t* self, uint8_t code) {
    uint8_t row = ROW(code);
    uint8_t col = COL(code);
    switch (self->keyboard_kind) {
        case PC_XT:
            return pgm_read_byte(unimap_cs1[row][col]);
        case PC_AT:
            return pgm_read_byte(unimap_cs2[row][col]);
        case PC_TERMINAL:
            return pgm_read_byte(unimap_cs3[row][col]);
        default:
            return KC_NO;
    }
}

static inline void ibmpcconverter_matrix_clear(void) {
    for (uint8_t i=0; i < MATRIX_ROWS; i++) matrix[i] = 0x00;
}

static inline matrix_row_t ibmpcconverter_matrix_get_row(uint8_t row) {
    return matrix[row];
}

bool matrix_is_on(uint8_t row, uint8_t col) {
    return (matrix[row] & (1<<col));
}

void matrix_print(void) {
#if (MATRIX_COLS <= 8)
    print("r/c 01234567\n");
#elif (MATRIX_COLS <= 16)
    print("r/c 0123456789ABCDEF\n");
#elif (MATRIX_COLS <= 32)
    print("r/c 0123456789ABCDEF0123456789ABCDEF\n");
#endif

    for (uint8_t row = 0; row < MATRIX_ROWS; row++) {

#if (MATRIX_COLS <= 8)
        xprintf("%02X: %08b%s\n", row, bitrev(matrix_get_row(row)),
#elif (MATRIX_COLS <= 16)
        xprintf("%02X: %016b%s\n", row, bitrev16(matrix_get_row(row)),
#elif (MATRIX_COLS <= 32)
        xprintf("%02X: %032b%s\n", row, bitrev32(matrix_get_row(row)),
#endif
#ifdef MATRIX_HAS_GHOST
        matrix_has_ghost_in_row(row) ?  " <ghost" : ""
#else
        ""
#endif
        );
    }
}

// translate to Unimap before storing in matrix
static inline void matrix_make(IBMPCConverter_t* self, uint8_t code) {
    uint8_t u = ibmpcconverter_to_unimap(self, code);
    if (u > 0x7F) return;
    if (!matrix_is_on(ROW(u), COL(u))) {
        matrix[ROW(u)] |= 1<<COL(u);
    }
}

static inline void matrix_break(IBMPCConverter_t* self, uint8_t code) {
    uint8_t u = ibmpcconverter_to_unimap(self, code);
    if (u > 0x7F) return;
    if (matrix_is_on(ROW(u), COL(u))) {
        matrix[ROW(u)] &= ~(1<<COL(u));
    }
}
/*******************************/

/********** Mouse Mode **********/
#ifdef IBMPC_MOUSE_ENABLE
enum {
    MOUSE_DEFAULT  = 0, // Default three-button
    MOUSE_INTELLI  = 3, // Intellimouse Explorer 3-button & wheel
    MOUSE_EXPLORER = 4, // Intellimouse Explorer 5-button & wheel
    MOUSE_LOGITECH = 9  // Logitech PS/2++
} mouse_id = MOUSE_DEFAULT;
uint8_t mouse_btn = 0;

void mouse_read_status(IBMPCConverter_t* self, uint8_t *s) {
    ibmpc_host_send(self->ibmpc, 0xE9);
    s[0] = ibmpc_host_recv_response(self->ibmpc);
    s[1] = ibmpc_host_recv_response(self->ibmpc);
    s[2] = ibmpc_host_recv_response(self->ibmpc);
    xprintf("S[%02X %02X %02X] ", s[0], s[1], s[2]);
}
#endif
/********************************/
