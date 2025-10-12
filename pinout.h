#ifndef PINOUT_H
#define PINOUT_H

void dw(uint8_t pin, uint8_t state) {
  if (pin < 8) bitWrite(PORTD, pin, state);
  else if (pin < 14) bitWrite(PORTB, (pin - 8), state);
  else bitWrite(PORTC, (pin - 14), state);
}

bool dr(uint8_t pin) {
  if (pin < 8) bitRead(PIND, pin);
  else if (pin < 14) bitRead(PINB, pin - 8);
  else bitRead(PINC, pin - 14);
}

void pm(uint8_t pin, uint8_t state) {
  switch (state) {
    case INPUT:
      if (pin < 8) bitWrite(DDRD, pin, 0);
      else if (pin < 14) bitWrite(DDRB, pin - 8, 0);
      else bitWrite(DDRC, pin - 14, 0);
      break;
    case OUTPUT:
      if (pin < 8) bitWrite(DDRD, pin, 1);
      else if (pin < 14) bitWrite(DDRB, pin - 8, 1);
      else bitWrite(DDRC, pin - 14, 1);
      break;
    case INPUT_PULLUP:
      if (pin < 8) {
        bitWrite(DDRD, pin, 0);
        bitWrite(PORTD, pin, 1);
      }
      else if (pin < 14) {
        bitWrite(DDRD, pin - 8, 0);
        bitWrite(PORTB, pin - 8, 1);
      }
      else {
        bitWrite(DDRC, pin - 14, 0);
        bitWrite(PORTC, pin - 14, 1);
      }
      break;
  }
}

void dt(uint8_t pin) {
  if (pin < 8) bitToggle(PORTD, pin);
  else if (pin < 14) bitToggle(PORTB, pin - 8);
  else bitToggle(PORTC, pin - 14);
}
#endif
