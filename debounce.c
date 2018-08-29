#include "debounce.h"

#define FALSE 0
#define TRUE 1

extern unsigned int g1msTimer;

typedef enum {off,on} LEDState;

typedef struct LED_Structure {
    char *LED_Port;
    unsigned char LED_Bit;
} LED;

void SetLEDState(LED *MyLED,LEDState state);

void InitializeSwitch(SwitchDefine *Switch,char *SwitchPort,unsigned char SwitchBit,
		unsigned char HoldTime,unsigned char ReleaseTime)
{
	Switch->CurrentState = DbExpectHigh;

	Switch->SwitchPort = SwitchPort;
	Switch->SwitchPortBit = SwitchBit;

	Switch->HoldTime = HoldTime; // units equal milliseconds
	Switch->ReleaseTime = ReleaseTime; // units equal milliseconds
	Switch->EventTime = 0;
}

SwitchStatus GetSwitch(SwitchDefine *Switch)
{
    if(*Switch->SwitchPort & Switch->SwitchPortBit)
    {
     return Low;
    }
    else
    {
     return High;
    }
}

SwitchStatus Debouncer(SwitchDefine *Switch)
{

	SwitchStatus CurrentSwitchReading;
	SwitchStatus DebouncedSwitchStatus = Low;
	unsigned char X1;
	unsigned char X0;
	DbState NextState;
	int ElapsedTime = 0;

	NextState = Switch->CurrentState;
	// First, determine the current inputs, X1 and X0.
	CurrentSwitchReading = GetSwitch(Switch);
	if(CurrentSwitchReading==Low)
	{
	    X0 = FALSE;
	}
	else
	{
	    X0 = TRUE;
	}

	//calculate elapsed time from last event.
	ElapsedTime = (unsigned int)(g1msTimer - Switch->EventTime);

	if((Switch->CurrentState == DbValidateHigh && ElapsedTime > Switch->HoldTime) || (Switch->CurrentState == DbValidateLow && ElapsedTime > Switch->ReleaseTime) )
	{
	    X1 = TRUE;
	}
	else
	{
	    X1 = FALSE;
	}
	// Next, based on the input values and the current state, determine the next state.
	switch (Switch->CurrentState) {
		case DbExpectHigh:
		    if(X0==1) //X0=1
		    {
	             NextState = DbValidateHigh;
		    }
		break;
		case DbValidateHigh:
            if(X0==0) //X0=0
            {
                NextState = DbExpectHigh;
            }
            else //X0=1
            {
                if(X1==1) //X1=1,X0=1
                {
                    NextState = DbExpectLow;
                }
            }
		break;
		case DbExpectLow:
            if(X0==0) //X0=0
            {
                 NextState = DbValidateLow;
            }
		break;
		case DbValidateLow:
            if(X0==1) //X0=1
            {
                NextState = DbExpectLow;
            }
            else //X0=0
            {
                if(X1==1) //X1=1,X0=1
                {
                    NextState = DbExpectHigh;
                }
            }
		break;
		default: NextState = DbExpectHigh;
	}
	
	// Perform the output function based on the inputs and current state.
	switch (Switch->CurrentState) {
		case DbExpectHigh:
		    DebouncedSwitchStatus = Low;
		    if(X0 == TRUE){
		        Switch->EventTime = g1msTimer;
		    }
			SET_DEBUG1_PIN_LOW; SET_DEBUG0_PIN_LOW; //assign 0 0 to current state
		break;
		case DbValidateHigh:
		    if(X0 == TRUE && X1 == TRUE)
		    {
	            DebouncedSwitchStatus = High;
		    }
		    else
		    {
	            DebouncedSwitchStatus = Low;
		    }
			SET_DEBUG1_PIN_LOW; SET_DEBUG0_PIN_HIGH; //assign 0 1
		break;
		case DbExpectLow:
            DebouncedSwitchStatus = High;
            if(X0 == FALSE){
                Switch->EventTime = g1msTimer;
            }
			SET_DEBUG1_PIN_HIGH; SET_DEBUG0_PIN_LOW; // assign 1 0
		break;
		case DbValidateLow:
            if(X0 == FALSE && X1 == TRUE)
            {
                DebouncedSwitchStatus = Low;
            }
            else
            {
                DebouncedSwitchStatus = High;
            }
			SET_DEBUG1_PIN_HIGH; SET_DEBUG0_PIN_HIGH; // assign 1 1
		break;
	}
	
	// Finally, update the current state.

	    Switch->CurrentState = NextState;

	return DebouncedSwitchStatus;
}


