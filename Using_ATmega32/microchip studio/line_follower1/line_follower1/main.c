#define F_CPU 8000000UL  // Define clock frequency
#include <avr/io.h>
#include <util/delay.h>

// Define motor control pins
#define MOTOR_LEFT_FORWARD  PD0  // IN2 of L298N connected to D0
#define MOTOR_RIGHT_FORWARD PD1  // IN3 of L298N connected to D1

// Define sensor input pins
#define LEFT_SENSOR  PB0  // Left IR sensor connected to B0
#define RIGHT_SENSOR PB1  // Right IR sensor connected to B1

void motor_stop() {
	PORTD &= ~(1 << MOTOR_LEFT_FORWARD);  // Stop left motor
	PORTD &= ~(1 << MOTOR_RIGHT_FORWARD); // Stop right motor
}

void motor_forward() {
	PORTD |= (1 << MOTOR_LEFT_FORWARD);   // Move left motor forward
	PORTD |= (1 << MOTOR_RIGHT_FORWARD);  // Move right motor forward
}

void motor_left() {
	PORTD &= ~(1 << MOTOR_RIGHT_FORWARD); // Stop right motor
	PORTD |= (1 << MOTOR_LEFT_FORWARD);   // Move left motor forward
}

void motor_right() {
	PORTD &= ~(1 << MOTOR_LEFT_FORWARD);  // Stop left motor
	PORTD |= (1 << MOTOR_RIGHT_FORWARD);  // Move right motor forward
}

int main(void) {
	// Set motor control pins as output
	DDRD |= (1 << MOTOR_LEFT_FORWARD) | (1 << MOTOR_RIGHT_FORWARD);

	// Set sensor input pins as input
	DDRB &= ~((1 << LEFT_SENSOR) | (1 << RIGHT_SENSOR));

	while(1) {
		// Read sensor values
		uint8_t left_sensor_value = PINB & (1 << LEFT_SENSOR);
		uint8_t right_sensor_value = PINB & (1 << RIGHT_SENSOR);

		// If both sensors detect the line (1, 1), move forward
		if (left_sensor_value != 0 && right_sensor_value != 0) {
			motor_forward();
		}
		// If both sensors are off the line (0, 0), stop
		else if (left_sensor_value == 0 && right_sensor_value == 0) {
			motor_stop();
		}
		// If the left sensor detects the line (1, 0), turn left
		else if (left_sensor_value != 0 && right_sensor_value == 0) {
			motor_left();
		}
		// If the right sensor detects the line (0, 1), turn right
		else if (left_sensor_value == 0 && right_sensor_value != 0) {
			motor_right();
		}

		_delay_ms(100);  // Small delay for stability
	}
}
