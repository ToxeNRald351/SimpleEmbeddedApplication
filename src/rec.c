// Rotational encoder controls
// Author: Anton Havlovskyi

#include "MK60D10.h"
#include "rec.h"

int last_clk_state_1 = 0;
int last_clk_state_2 = 0;

int rec_read(int encoder_pin) {
    return (PTA->PDIR & (1 << encoder_pin)) ? 0 : 1;
}

int rec_process_rotation(int clk_state, int dt_state, int *last_clk_state) {
    int return_value = 0;	// Initial 0 - no rotation detected

    if (clk_state != *last_clk_state) {
        // If CLK has changed, determine direction
        if (dt_state == clk_state) {
            // Clockwise rotation
            return_value = 1;
        }
        else {
            // Counterclockwise rotation
            return_value = -1;
        }
    }
    *last_clk_state = clk_state;  // Store the state for future compare
    return return_value;
}

void rec_init_ports() {
	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;

	// Set MUX values to select GPIO mode for the pins
	PORTA->PCR[ENCODER1_CLK_PIN] = PORT_PCR_MUX(0x01);
	PORTA->PCR[ENCODER1_DT_PIN]  = PORT_PCR_MUX(0x01);
	PORTA->PCR[ENCODER1_SW_PIN]  = PORT_PCR_MUX(0x01);

	PORTA->PCR[ENCODER2_CLK_PIN] = PORT_PCR_MUX(0x01);
	PORTA->PCR[ENCODER2_DT_PIN]  = PORT_PCR_MUX(0x01);
	PORTA->PCR[ENCODER2_SW_PIN]  = PORT_PCR_MUX(0x01);

	// Set the pins as input (GPIO)
	PTA->PDDR &= ~(1 << ENCODER1_CLK_PIN);
	PTA->PDDR &= ~(1 << ENCODER1_DT_PIN);
	PTA->PDDR &= ~(1 << ENCODER1_SW_PIN);

	PTA->PDDR &= ~(1 << ENCODER2_CLK_PIN);
	PTA->PDDR &= ~(1 << ENCODER2_DT_PIN);
	PTA->PDDR &= ~(1 << ENCODER2_SW_PIN);
}
