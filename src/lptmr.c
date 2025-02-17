// Low power timer configuration for pseudo random numbers generation
// Author: Anton Havlovskyi

#include <stdlib.h>
#include "MK60D10.h"
#include "lptmr.h"

unsigned int counter = 0;

void init_lptmr() {
    // Enable the clock for LPTMR
    SIM_SCGC5 |= SIM_SCGC5_LPTIMER_MASK;

    // Disable the LPTMR to configure it
    LPTMR0_CSR = 0x00;

    // Configure the clock source and prescaler
    // Use LPO (1 kHz clock) as the clock source and bypass the prescaler
    LPTMR0_PSR = LPTMR_PSR_PCS(1) | LPTMR_PSR_PBYP_MASK;

    // Set the compare value for the timer
    // For a 1-second interval with 1 kHz clock, set compare value to 1000
    LPTMR0_CMR = 1000 - 1;

    // 5. Enable interrupts (optional)
    LPTMR0_CSR |= LPTMR_CSR_TIE_MASK;  // Enable interrupt
    NVIC_EnableIRQ(LPTMR0_IRQn);       // Enable in NVIC

    // Start the timer
    LPTMR0_CSR |= LPTMR_CSR_TEN_MASK;
}

void LPTMR0_IRQHandler() {
    // Clear the interrupt flag
    LPTMR0_CSR |= LPTMR_CSR_TCF_MASK;

    srand(counter++);
}
