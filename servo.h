#ifndef SERVO_H
#define SERVO_H

#include "pinout.h"
#include "timer0.h"

class Servo {
	public:

	Servo(uint8_t pin) : _pin(pin) {
		upTime0();
		pm(_pin, 1);
	}

	void tick(){
		switch(_state & 1){
			case 0:
				if(micros() - _tmr >= _duty){
					dw(_pin, 0);
					_tmr = micros();
					++_state;
				}
				break;
			case 1:
				if(micros() - _tmr >= 20000 - _duty){
					dw(_pin, 1);
					_tmr = micros();
					++_state;
				}
				break;
		}
	}

	void write(uint16_t angle){
		_duty = (angle * 10.5f) + 500;
	}

	private:
	const uint8_t _pin;
	uint8_t _state;
	uint16_t _duty;
	uint32_t _tmr;
};

#endif