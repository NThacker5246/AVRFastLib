#ifndef TIMER0_H
#define TIMER0_H

volatile uint32_t ovf = 0;

void upTime0() {
  TIMSK0 |= 3;
  TCCR0B = 4; //64
  OCR0A = 250;
}

ISR(TIMER0_COMPA_vect) {
  ovf += 1;
}

uint32_t millis() {
  byte sre = SREG;
  cli();
  uint32_t ticks = ovf >> 2;
  SREG = sre;
  return ticks;
}

uint32_t rmillis() {
  return (ovf >> 2);
}


uint32_t micros() {
  byte sre = SREG;
  cli();
  uint32_t ticks = 250 * ovf + TCNT0;
  SREG = sre;
  return ticks << 2;
}
#endif
