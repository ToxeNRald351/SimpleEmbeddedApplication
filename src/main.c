// Application for embedded system controlled with two rotational encoders
// Author: Anton Havlovskyi

#include "MK60D10.h"
#include "mcg.h"
#include "io.h"
#include "uart.h"
#include "rec.h"
#include "game.h"
#include "lptmr.h"

int main(void) {

	// Transition back to FEI mode (default mode upon restart)
	MCG_FEI_BLPE();

	uart_config();

	rec_init_ports();
	encoder_t right_encoder = {ENCODER1_CLK_PIN, ENCODER1_DT_PIN, ENCODER1_SW_PIN, 0};
	encoder_t left_encoder = {ENCODER2_CLK_PIN, ENCODER2_DT_PIN, ENCODER2_SW_PIN, 0};

	init_lptmr();

	// Initialize the UART.
	// Use TERM_PORT = UART5_BASE_PTR for the 9pin serial port. (Defined in uart.h)
	// Configure for 50 MHz peripheral clock and 115200 target baud rate.
	uart_init(TERM_PORT, 50000, 115200);

    printf("\n\rWelcome to Minesweeper!\n\r");

	int was_right_pressed = rec_read(right_encoder.sw_pin);
	int was_left_pressed = rec_read(left_encoder.sw_pin);
	delay(10000);
	was_right_pressed = 0;
	was_left_pressed = 0;

	for(;;) {

		printf("\n\rPress SW1 and SW2 to start the game!\n\r");

		while (was_right_pressed == 0 || was_left_pressed == 0) {
			was_right_pressed = rec_read(right_encoder.sw_pin);
			was_left_pressed = rec_read(left_encoder.sw_pin);
		}

		while (was_right_pressed != 0 || was_left_pressed != 0) {
			was_right_pressed = rec_read(right_encoder.sw_pin);
			was_left_pressed = rec_read(left_encoder.sw_pin);
		}

		gameMain(&right_encoder, &left_encoder);
		delay(10000);
	}

	return 0;
}
