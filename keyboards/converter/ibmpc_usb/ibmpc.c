/*
Copyright 2010,2011,2012,2013,2019 Jun WAKO <wakojun@gmail.com>

This software is licensed with a Modified BSD License.
All of this is supposed to be Free Software, Open Source, DFSG-free,
GPL-compatible, and OK to use in both free and proprietary applications.
Additions and corrections to this file are welcome.


Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright
  notice, this list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright
  notice, this list of conditions and the following disclaimer in
  the documentation and/or other materials provided with the
  distribution.

* Neither the name of the copyright holders nor the names of
  contributors may be used to endorse or promote products derived
  from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
*/

/*
 * IBM PC keyboard protocol
 */

#include <stdbool.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include "debug.h"
#include "timer.h"
#include "wait.h"
#include "ibmpc.h"

#define WAIT(stat, ibmpc, us, err) do { \
    if (!ibmpc_wait_##stat(ibmpc, us)) { \
        self->error = err; \
        goto ERROR; \
    } \
} while (0)

IBMPC_t interface0;
#if defined(IBMPC_CLOCK_BIT1) && defined(IBMPC_DATA_BIT1)
IBMPC_t interface1;
#endif

void ibmpc_host_init(IBMPC_t* self)
{
    // initialize reset pin to HiZ
    IBMPC_RST_HIZ();
    ibmpc_init(&interface0, IBMPC_CLOCK_BIT, IBMPC_DATA_BIT);
    #if defined(IBMPC_CLOCK_BIT1) && defined(IBMPC_DATA_BIT1) && defined(IBMPC_INT_VECT1)
    ibmpc_init(&interface1, IBMPC_CLOCK_BIT1, IBMPC_DATA_BIT1);
    #endif
    ibmpc_inhibit(self);
    ibmpc_int_init(self);
    ibmpc_int_off(self);
    ibmpc_host_isr_clear(self);
}

void ibmpc_host_enable(IBMPC_t* self)
{
    ibmpc_int_on(self);
    ibmpc_idle(self);
}

void ibmpc_host_disable(IBMPC_t* self)
{
    ibmpc_int_off(self);
    ibmpc_inhibit(self);
}

int16_t ibmpc_host_send(IBMPC_t* self, uint8_t data)
{
    bool parity = true;
    self->error = IBMPC_ERR_NONE;
    uint8_t retry = 0;

    dprintf("w%02X ", data);

    // Return when receiving data
    //if (isr_state & 0x0FFF) {
    if (self->isr_state != 0x8000) {
        dprintf("isr:%04X ", self->isr_state);
        return -1;
    }

    ibmpc_int_off(self);

RETRY:
    /* terminate a transmission if we have */
    ibmpc_inhibit(self);
    wait_us(200);    // [5]p.54

    /* 'Request to Send' and Start bit */
    ibmpc_data_lo(self);
    wait_us(200);
    ibmpc_clock_hi(self);     // [5]p.54 [clock low]>100us [5]p.50
    WAIT(clock_lo, self, 15000, 1);   // [5]p.54 T13M, -10ms [5]p.50

    /* Data bit[2-9] */
    for (uint8_t i = 0; i < 8; i++) {
        wait_us(15);
        if (data&(1<<i)) {
            parity = !parity;
            ibmpc_data_hi(self);
        } else {
            ibmpc_data_lo(self);
        }
        WAIT(clock_hi, self, 50, 2);
        WAIT(clock_lo, self, 50, 3);
    }

    /* Parity bit */
    wait_us(15);
    if (parity) { ibmpc_data_hi(self); } else { ibmpc_data_lo(self); }
    WAIT(clock_hi, self, 50, 4);
    WAIT(clock_lo, self, 50, 5);

    /* Stop bit */
    wait_us(15);
    ibmpc_data_hi(self);

    /* Ack */
    WAIT(data_lo, self, 300, 6);
    WAIT(data_hi, self, 300, 7);
    WAIT(clock_hi, self, 300, 8);

#ifdef SIEMENS_PCD_SUPPORT
    // inhibit - https://github.com/tmk/tmk_keyboard/issues/747
    if (self->protocol & IBMPC_PROTOCOL_AT) {
        wait_us(15);
        ibmpc_clock_lo(self);
        wait_us(150);
    }
#endif

    // clear buffer to get response correctly
    ibmpc_host_isr_clear(self);

    ibmpc_idle(self);
    ibmpc_int_on(self);
    return ibmpc_host_recv_response(self);
ERROR:
    // Retry for Z-150 AT start bit error
    if (self->error == 1 && retry++ < 10) {
        self->error = IBMPC_ERR_NONE;
        dprintf("R ");
        goto RETRY;
    }

    self->isr_debug = self->isr_state;
    self->error |= IBMPC_ERR_SEND;
    ibmpc_inhibit(self);
    wait_ms(2);
    ibmpc_idle(self);
    ibmpc_int_on(self);
    return -1;
}

/*
 * Receive data from keyboard
 */
int16_t ibmpc_host_recv(IBMPC_t* self)
{
    int16_t ret = -1;

    // Enable ISR if buffer was full
    if (ibmpc_ringbuf_is_full(self)) {
        ibmpc_host_isr_clear(self);
        ibmpc_int_on(self);
        ibmpc_idle(self);
    }

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        ret = ibmpc_ringbuf_get(self);
    }
    if (ret != -1) dprintf("r%02X ", ret&0xFF);
    return ret;
}

int16_t ibmpc_host_recv_response(IBMPC_t* self)
{
    // Command may take 25ms/20ms at most([5]p.46, [3]p.21)
    uint8_t retry = 25;
    int16_t data = -1;
    while (retry-- && (data = ibmpc_host_recv(self)) == -1) {
        wait_ms(1);
    }
    return data;
}

void ibmpc_host_isr_clear(IBMPC_t* self)
{
    self->isr_debug = 0;
    self->protocol = 0;
    self->error = 0;
    self->isr_state = 0x8000;
    ibmpc_ringbuf_reset(self);
}

void ibmpc_isr(IBMPC_t* self)
{
    uint8_t dbit;
    dbit = IBMPC_DATA_PIN&(1<<IBMPC_DATA_BIT);

    // Timeout check
    uint8_t t;
    // use only the least byte of millisecond timer
    asm("lds %0, %1" : "=r" (t) : "p" (&timer_count));
    //t = (uint8_t)timer_count;    // compiler uses four registers instead of one
    if (self->isr_state == 0x8000) {
        self->timer_start = t;
    } else {
        // This gives 2.0ms at least before timeout
        if ((uint8_t)(t - self->timer_start) >= 5) {
            self->isr_debug = self->isr_state;
            self->error = IBMPC_ERR_TIMEOUT;
            goto ERROR;

            // timeout error recovery - start receiving new data
            // it seems to work somehow but may not under unstable situation
            //timer_start = t;
            //isr_state = 0x8000;
        }
    }

    self->isr_state = self->isr_state>>1;
    if (dbit) self->isr_state |= 0x8000;

    // isr_state: state of receiving data from keyboard
    //
    // This should be initialized with 0x8000 before receiving data and
    // the MSB '*1' works as marker to discrimitate between protocols.
    // It stores sampled bit at MSB after right shift on each clock falling edge.
    //
    // XT protocol has two variants of signaling; XT_IBM and XT_Clone.
    // XT_IBM uses two start bits 0 and 1 while XT_Clone uses just start bit 1.
    // https://github.com/tmk/tmk_keyboard/wiki/IBM-PC-XT-Keyboard-Protocol
    //
    //      15 14 13 12   11 10  9  8    7  6  5  4    3  2  1  0
    //      -----------------------------------------------------
    //      *1  0  0  0    0  0  0  0 |  0  0  0  0    0  0  0  0     Initial state(0x8000)
    //
    //       x  x  x  x    x  x  x  x |  0  0  0  0    0  0  0  0     midway(0-7 bits received)
    //       x  x  x  x    x  x  x  x | *1  0  0  0    0  0  0  0     midway(8 bits received)
    //      b6 b5 b4 b3   b2 b1 b0  1 |  0 *1  0  0    0  0  0  0     XT_IBM-midway ^1
    //      b7 b6 b5 b4   b3 b2 b1 b0 |  0 *1  0  0    0  0  0  0     AT-midway ^1
    //      b7 b6 b5 b4   b3 b2 b1 b0 |  1 *1  0  0    0  0  0  0     XT_Clone-done
    //      pr b7 b6 b5   b4 b3 b2 b1 |  0  0 *1  0    0  0  0  0     AT-midway[b0=0]
    //      b7 b6 b5 b4   b3 b2 b1 b0 |  1  0 *1  0    0  0  0  0     XT_IBM-done ^2
    //      pr b7 b6 b5   b4 b3 b2 b1 |  1  0 *1  0    0  0  0  0     AT-midway[b0=1] ^2
    //       x  x  x  x    x  x  x  x |  0  1 *1  0    0  0  0  0     illegal
    //       x  x  x  x    x  x  x  x |  1  1 *1  0    0  0  0  0     illegal
    //      st pr b7 b6   b5 b4 b3 b2 | b1 b0  0 *1    0  0  0  0     AT-done
    //       x  x  x  x    x  x  x  x |  x  x  1 *1    0  0  0  0     illegal
    //                                all other states than above     illegal
    //
    // ^1: AT and XT_IBM takes same state.
    // ^2: AT and XT_IBM takes same state in case that AT b0 is 1,
    // we have to check AT stop bit to discriminate between the two protocol.
    switch (self->isr_state & 0xFF) {
        case 0b00000000:
        case 0b10000000:
        case 0b01000000:    // ^1
        case 0b00100000:
            // midway
            goto NEXT;
            break;
        case 0b11000000:
            // XT_Clone-done
            self->isr_debug = self->isr_state;
            self->isr_state = self->isr_state>>8;
            self->protocol = IBMPC_PROTOCOL_XT_CLONE;
            goto DONE;
            break;
        case 0b10100000:    // ^2
            {
                uint8_t us = 100;
                // wait for rising and falling edge of AT stop bit to discriminate between XT and AT
                if (!self->protocol) {
                    while (!(IBMPC_CLOCK_PIN & self->clock_mask) && us) { wait_us(1); us--; }
                    while ( (IBMPC_CLOCK_PIN & self->clock_mask) && us) { wait_us(1); us--; }
                } else if (self->protocol == IBMPC_PROTOCOL_XT_IBM) {
                    us = 0;
                }

                if (us) {
                    // found stop bit: AT-midway - process the stop bit in next ISR
                    goto NEXT;
                } else {
                    // no stop bit: XT_IBM-done
                    self->isr_debug = self->isr_state;
                    self->isr_state = self->isr_state>>8;
                    self->protocol = IBMPC_PROTOCOL_XT_IBM;
                    goto DONE;
                }
            }
            break;
        case 0b00010000:
        case 0b10010000:
        case 0b01010000:
        case 0b11010000:
            // AT-done
            self->isr_debug = self->isr_state;

            // Detect AA with parity error for AT/XT Auto-Switching support
            // https://github.com/tmk/tmk_keyboard/wiki/IBM-PC-Keyboard-Converter#atxt-auto-switching
            // isr_state: st pr b7 b6   b5 b4 b3 b2 | b1 b0  0 *1    0  0  0  0
            //            1 '0' 1  0    1  0  1  0  | 1  0   0 *1    0  0  0  0
            if (self->isr_state == 0xAA90) {
                self->error = IBMPC_ERR_PARITY_AA;
                goto ERROR;
            }

            // parit bit check
            {
                // isr_state: st pr b7 b6   b5 b4 b3 b2 | b1 b0  0 *1    0  0  0  0
                uint8_t p = (self->isr_state & 0x4000) ? 1 : 0;
                p ^= (self->isr_state >> 6);
                while (p & 0xFE) {
                    p = (p >> 1) ^ (p & 0x01);
                }

                if (p == 0) {
                    self->error = IBMPC_ERR_PARITY;
                    goto ERROR;
                }
            }

            // stop bit check
            if (self->isr_state & 0x8000) {
                self->protocol = IBMPC_PROTOCOL_AT;
            } else {
                // Zenith Z-150 AT(beige/white lable) asserts stop bit as low
                // https://github.com/tmk/tmk_keyboard/wiki/IBM-PC-AT-Keyboard-Protocol#zenith-z-150-beige
                self->protocol = IBMPC_PROTOCOL_AT_Z150;
            }
            self->isr_state = self->isr_state>>6;
            goto DONE;
            break;
        case 0b01100000:
        case 0b11100000:
        case 0b00110000:
        case 0b10110000:
        case 0b01110000:
        case 0b11110000:
        default:            // xxxx_oooo(any 1 in low nibble)
            // Illegal
            self->protocol = 0;
            self->isr_debug = self->isr_state;
            self->error = IBMPC_ERR_ILLEGAL;
            goto ERROR;
            break;
    }

DONE:
#ifdef SIEMENS_PCD_SUPPORT
    // inhibit - https://github.com/tmk/tmk_keyboard/issues/747
    if (self->protocol & IBMPC_PROTOCOL_AT) {
        ibmpc_clock_lo(self);
        wait_us(150);
        ibmpc_clock_hi(self);
    }
#endif

    // store data
    ibmpc_ringbuf_put(self, self->isr_state & 0xFF);
    if (ibmpc_ringbuf_is_full(self)) {
        // Disable ISR if buffer is full
        ibmpc_int_off(self);
        // inhibit: clock_lo(ibmpc) instead of ibmpc_inhibit(ibmpc) for ISR optimization
        ibmpc_clock_lo(self);
    }
    if (ibmpc_ringbuf_is_empty(self)) {
        // buffer overflow
        self->error = IBMPC_ERR_FULL;
    }
    goto END;
ERROR:
    // inhibit: Use clock_lo() instead of ibmpc_inhibit(ibmpc) for ISR optimization
    ibmpc_clock_lo(self);
END:
    // clear for next data
    self->isr_state = 0x8000;
NEXT:
    return;
}

/* send LED state to keyboard */
void ibmpc_host_set_led(IBMPC_t* self, uint8_t led)
{
    if (0xFA == ibmpc_host_send(self, 0xED)) {
        ibmpc_host_send(self, led);
    }
}

// NOTE: With this ISR data line should be read within 5us after clock falling edge.
// Confirmed that ATmega32u4 can read data line in 2.5us from interrupt after
// ISR prologue pushs r18, r19, r20, r21, r24, r25 r30 and r31 with GCC 5.4.0
ISR(IBMPC_INT_VECT)
{
    ibmpc_isr(&interface0);
}

#if defined(IBMPC_CLOCK_BIT1) && defined(IBMPC_DATA_BIT1) && defined(IBMPC_INT_VECT1)
ISR(IBMPC_INT_VECT1)
{
    ibmpc_isr(&interface1);
}
#endif
