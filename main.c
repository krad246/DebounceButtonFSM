#include <msp430.h>
#include "LED.h"
#include "pushbutton.h"
#include "timerA.h"
#include "debounce.h"
#include "debug_pins.h"

// Global Variables
unsigned int g1msTimeout;		// This variable is incremented by the interrupt handler and
								// decremented by a software call in the main loop.
unsigned int g1msTimer;			// This variable is incremented in ManageSoftwareTimers

// Function Prototypes
void ConfigureClockModule();
void ManageSoftwareTimers(void);

void main(void)
{
	volatile SwitchStatus PushButtonStatus;
	SwitchDefine PushButton;

    WDTCTL = WDTPW | WDTHOLD;		// Stop watchdog timer
    ConfigureClockModule();

    // Initialize hardware.
    ConfigureTimerA();
	InitializeLEDPortPins();
	InitializePushButtonPortPin();

	// Initialize the pushbutton switch.
	InitializeSwitch(&PushButton,(char *) &PUSHBUTTON_PORT_IN,(unsigned char) PUSHBUTTON_BIT,
			HIGH_THRESHOLD,LOW_THRESHOLD);

	// Also, initialize port pins used for debug.
	SET_DEBUG1_PIN_LOW;
	SET_DEBUG1_PIN_AS_AN_OUTPUT;
	SET_DEBUG0_PIN_LOW;
	SET_DEBUG0_PIN_AS_AN_OUTPUT;

	_enable_interrupts();		// interrupts enabled

	// Loop forever
	while(1) {
		ManageSoftwareTimers();
		PushButtonStatus = Debouncer(&PushButton);
		if (PushButtonStatus == Low){ // Switch is inactive
			TURN_OFF_LED1;
		}
		else {
			TURN_ON_LED1;
		}
	}
}

void ConfigureClockModule()
{
	// Configure Digitally Controlled Oscillator (DCO) using factory calibrations
	DCOCTL  = CALDCO_1MHZ;
	BCSCTL1 = CALBC1_1MHZ;
}

void ManageSoftwareTimers(void)
{
	if (g1msTimeout){
		g1msTimeout--;
		g1msTimer++;
	}
}
