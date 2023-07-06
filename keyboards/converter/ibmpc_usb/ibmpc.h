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

#pragma once

#include <stdbool.h>
#include "wait.h"

/*
 * IBM PC keyboard protocol
 *
 * PS/2 Resources
 * --------------
 * [1] The PS/2 Mouse/Keyboard Protocol
 * http://www.computer-engineering.org/ps2protocol/
 * Concise and thorough primer of PS/2 protocol.
 *
 * [2] Keyboard and Auxiliary Device Controller
 * http://www.mcamafia.de/pdf/ibm_hitrc07.pdf
 * Signal Timing and Format
 *
 * [3] Keyboards(101- and 102-key)
 * http://www.mcamafia.de/pdf/ibm_hitrc11.pdf
 * Keyboard Layout, Scan Code Set, POR, and Commands.
 *
 * [4] PS/2 Reference Manuals
 * http://www.mcamafia.de/pdf/ibm_hitrc07.pdf
 * Collection of IBM Personal System/2 documents.
 *
 * [5] TrackPoint Engineering Specifications for version 3E
 * https://web.archive.org/web/20100526161812/http://wwwcssrv.almaden.ibm.com/trackpoint/download.html
 */
#define IBMPC_ACK         0xFA
#define IBMPC_RESEND      0xFE
#define IBMPC_SET_LED     0xED

#define IBMPC_PROTOCOL_NO       0
#define IBMPC_PROTOCOL_AT       0x10
#define IBMPC_PROTOCOL_AT_Z150  0x11
#define IBMPC_PROTOCOL_XT       0x20
#define IBMPC_PROTOCOL_XT_IBM   0x21
#define IBMPC_PROTOCOL_XT_CLONE 0x22

// Error numbers
#define IBMPC_ERR_NONE        0
#define IBMPC_ERR_PARITY      0x01
#define IBMPC_ERR_PARITY_AA   0x02
#define IBMPC_ERR_SEND        0x10
#define IBMPC_ERR_TIMEOUT     0x20
#define IBMPC_ERR_FULL        0x40
#define IBMPC_ERR_ILLEGAL     0x80

#define IBMPC_LED_SCROLL_LOCK 0
#define IBMPC_LED_NUM_LOCK    1
#define IBMPC_LED_CAPS_LOCK   2

/* ring buffer */
// Size should be power of 2
#define RINGBUF_SIZE    16

typedef struct {
    volatile uint16_t isr_debug;
    volatile uint8_t protocol;
    volatile uint8_t error;
    volatile uint16_t isr_state;
    uint8_t timer_start;

    /* private: */
    uint8_t rb_head;
    uint8_t rb_tail;
    uint8_t rb_buffer[RINGBUF_SIZE];

    uint8_t clock_bit, data_bit;
    uint8_t clock_mask, data_mask;

} IBMPC_t;

void ibmpc_host_init(IBMPC_t*);
void ibmpc_host_enable(IBMPC_t*);
void ibmpc_host_disable(IBMPC_t*);
int16_t ibmpc_host_send(IBMPC_t*, uint8_t);
int16_t ibmpc_host_recv_response(IBMPC_t*);
int16_t ibmpc_host_recv(IBMPC_t*);
void ibmpc_host_isr_clear(IBMPC_t*);
void ibmpc_host_set_led(IBMPC_t*, uint8_t);
void ibmpc_isr(IBMPC_t*);

static inline void ibmpc_init(IBMPC_t* self, uint8_t clock, uint8_t data)
{
    self->isr_debug = IBMPC_ERR_NONE;
    self->protocol = IBMPC_PROTOCOL_NO;
    self->error = IBMPC_ERR_NONE;
    self->isr_state = 0x8000;
    self->timer_start = 0;
    self->clock_bit = clock;
    self->data_bit = data;
    self->clock_mask = 1 << clock;
    self->data_mask = 1 << data;
}

static inline void ibmpc_clock_lo(IBMPC_t* self)
{
    IBMPC_CLOCK_PORT &= ~self->clock_mask;
    IBMPC_CLOCK_DDR  |=  self->clock_mask;
}

static inline void ibmpc_clock_hi(IBMPC_t* self)
{
    /* input with pull up */
    IBMPC_CLOCK_DDR  &= ~self->clock_mask;
    IBMPC_CLOCK_PORT |=  self->clock_mask;
}

static inline bool clock_in(IBMPC_t* self)
{
    IBMPC_CLOCK_DDR  &= ~self->clock_mask;
    IBMPC_CLOCK_PORT |=  self->clock_mask;
    wait_us(1);
    return IBMPC_CLOCK_PIN & self->clock_mask;

}

static inline void ibmpc_data_lo(IBMPC_t* self)
{
    IBMPC_DATA_PORT &= ~self->data_mask;
    IBMPC_DATA_DDR  |=  self->data_mask;
}

static inline void ibmpc_data_hi(IBMPC_t* self)
{
    /* input with pull up */
    IBMPC_DATA_DDR  &= ~self->data_mask;
    IBMPC_DATA_PORT |=  self->data_mask;
}

static inline bool ibmpc_data_in(IBMPC_t* self)
{
    IBMPC_DATA_DDR  &= ~self->data_mask;
    IBMPC_DATA_PORT |=  self->data_mask;
    wait_us(1);
    return IBMPC_DATA_PIN & self->data_mask;
}

static inline uint16_t ibmpc_wait_clock_lo(IBMPC_t* self, uint16_t us)
{
    while (clock_in(self) && us) { asm(""); wait_us(1); us--; }
    return us;
}
static inline uint16_t ibmpc_wait_clock_hi(IBMPC_t* self, uint16_t us)
{
    while (!clock_in(self) && us) { asm(""); wait_us(1); us--; }
    return us;
}
static inline uint16_t ibmpc_wait_data_lo(IBMPC_t* self, uint16_t us)
{
    while (ibmpc_data_in(self) && us)  { asm(""); wait_us(1); us--; }
    return us;
}
static inline uint16_t ibmpc_wait_data_hi(IBMPC_t* self, uint16_t us)
{
    while (!ibmpc_data_in(self) && us)  { asm(""); wait_us(1); us--; }
    return us;
}

/* idle state that device can send */
static inline void ibmpc_idle(IBMPC_t* self)
{
    ibmpc_clock_hi(self);
    ibmpc_data_hi(self);
}

/* inhibit device to send(AT), soft reset(XT) */
static inline void ibmpc_inhibit(IBMPC_t* self)
{
    ibmpc_clock_lo(self);
    ibmpc_data_hi(self);
}

/* inhibit device to send(XT) */
static inline void ibmpc_inhibit_xt(IBMPC_t* self)
{
    ibmpc_clock_hi(self);
    ibmpc_data_lo(self);
}

static inline void ibmpc_int_init(IBMPC_t* self)
{
    // interrupt at falling edge
    if (self->clock_bit < 4) {
        EICRA |= (0x2 << ((self->clock_bit&0x3)*2));
    } else {
        EICRB |= (0x2 << ((self->clock_bit&0x3)*2));
    }
}

static inline void ibmpc_int_on(IBMPC_t* self)
{
    EIFR  |= self->clock_mask;
    EIMSK |= self->clock_mask;
}

static inline void ibmpc_int_off(IBMPC_t* self)
{
    EIMSK &= ~self->clock_mask;
}

/*
 * ring buffer
 */

static inline bool ibmpc_ringbuf_is_empty(IBMPC_t* self)
{
    return (self->rb_head == self->rb_tail);
}

static inline int16_t ibmpc_ringbuf_get(IBMPC_t* self)
{
    if (ibmpc_ringbuf_is_empty(self)) return -1;
    uint8_t data = self->rb_buffer[self->rb_tail];
    self->rb_tail++;
    self->rb_tail &= (RINGBUF_SIZE - 1);
    return  data;
}

static inline void ibmpc_ringbuf_put(IBMPC_t* self, uint8_t data)
{
    self->rb_buffer[self->rb_head] = data;
    self->rb_head++;
    self->rb_head &= (RINGBUF_SIZE - 1);
}


static inline bool ibmpc_ringbuf_is_full(IBMPC_t* self)
{
    return (((self->rb_head + 1) & (RINGBUF_SIZE - 1)) == self->rb_tail);
}

static inline void ibmpc_ringbuf_reset(IBMPC_t* self)
{
    self->rb_head = 0;
    self->rb_tail = 0;
}
