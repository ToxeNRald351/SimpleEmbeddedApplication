// Rotational encoder controls
// Author: Anton Havlovskyi

#ifndef SOURCES_REC_H_
#define SOURCES_REC_H_

// Define the pins used by the encoders
// First encoder
#define ENCODER1_CLK_PIN  	29  // PTA29
#define ENCODER1_DT_PIN   	28  // PTA28
#define ENCODER1_SW_PIN   	25  // PTA25
// Second encoder
#define ENCODER2_CLK_PIN 	27	// PTA27
#define ENCODER2_DT_PIN		26	// PTA26
#define ENCODER2_SW_PIN		24	// PTA24

typedef struct {
	int clk_pin;
	int dt_pin;
	int sw_pin;
	int last_state;
} encoder_t;

void rec_init_ports();
int rec_read(int encoder_pin);
int rec_process_rotation(int clk_state, int dt_state, int *last_clk_state);

#endif /* SOURCES_REC_H_ */
