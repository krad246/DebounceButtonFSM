/*
 * debug_pins.h
 *
 *  Created on: Mar 9, 2016
 *      Author: Todd DeLong
 */

#ifndef DEBUG_PINS_H_
#define DEBUG_PINS_H_

// Debug pins

// TP9 on GPIO board
#define DEBUG1_BIT					BIT2
#define DEBUG1_PORT					P2OUT
#define DEBUG1_DIR					P2DIR
#define SET_DEBUG1_PIN_AS_AN_OUTPUT	DEBUG1_DIR |= DEBUG1_BIT
#define SET_DEBUG1_PIN_HIGH			DEBUG1_PORT |= DEBUG1_BIT
#define SET_DEBUG1_PIN_LOW			DEBUG1_PORT &= ~DEBUG1_BIT

// TP7 on GPIO board
#define DEBUG0_BIT					BIT0
#define DEBUG0_PORT					P2OUT
#define DEBUG0_DIR					P2DIR
#define SET_DEBUG0_PIN_AS_AN_OUTPUT	DEBUG0_DIR |= DEBUG0_BIT
#define SET_DEBUG0_PIN_HIGH			DEBUG0_PORT |= DEBUG0_BIT
#define SET_DEBUG0_PIN_LOW			DEBUG0_PORT &= ~DEBUG0_BIT

#endif /* DEBUG_PINS_H_ */
