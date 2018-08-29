#ifndef FSM_DEBOUNCE_H_
#define FSM_DEBOUNCE_H_

#include <msp430.h>
#include "LED.h"
#include "pushbutton.h"
#include "debug_pins.h"

// Constant Definitions
#define HIGH_THRESHOLD 10		//Definitions for debounce Times for the P1.3 button (ms)
#define LOW_THRESHOLD 20

// Type Definitions
typedef enum {
	DbExpectHigh, DbValidateHigh, DbExpectLow, DbValidateLow
} DbState;

typedef enum {
	Low, High
} SwitchStatus;

typedef struct {
	DbState CurrentState;			// Current state of the FSM
	char * SwitchPort;				// Input port associated with switch
	unsigned char SwitchPortBit;	// Port pin associated with switch
	unsigned char HoldTime;			// Time switch must remain On
	unsigned char ReleaseTime;		// Time switch must remain Off after being On
	unsigned int EventTime;			// Records time of an event
} SwitchDefine;

// Function Prototypes
void InitializeSwitch(SwitchDefine *Switch,char *SwitchPort,unsigned char SwitchBit,
		unsigned char HoldTime,unsigned char ReleaseTime);

//This function returns the instantaneous value of the selected switch
SwitchStatus GetSwitch(SwitchDefine *Switch);

//This function debounces a switch input
SwitchStatus Debouncer(SwitchDefine *Switch);

#endif
