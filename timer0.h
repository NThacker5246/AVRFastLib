#ifndef TIMER0_H
#define TIMER0_H

// volatile uint32_t ovf = 0;

// void upTime0(){
//     TIMSK0 = 3;
//     TCCR0B = 2; //64
//     OCR0A = 250;
//     sei();
// }

// ISR(TIMER0_COMPA_vect){
//     ovf += 1;
// }

// ISR(TIMER0_OVF_vect){
//     TCNT0 = 0;
// }

// uint32_t rmillis() {return ovf >> 3;}

// uint32_t micros(){
//     byte sre = SREG;
//     cli();
//     uint32_t ticks = 250 * ovf + TCNT0;
//     SREG = sre;
//     return ticks << 8;
// }

// uint32_t millis(){
//     byte sre = SREG;
//     cli();
//     uint32_t ticks = (ovf >> 3) + ((TCNT0 > 127) ? 1 : 0);
//     SREG = sre;
//     return ticks;
// }

#define MICROSECONDS_PER_TIMER0_OVERFLOW (clockCyclesToMicroseconds(64 * 256))
#define MILLIS_INC (MICROSECONDS_PER_TIMER0_OVERFLOW / 1000)
#define FRACT_INC ((MICROSECONDS_PER_TIMER0_OVERFLOW % 1000) >> 3)
#define FRACT_MAX (1000 >> 3)
#define MICROS_MULT (64 / clockCyclesPerMicrosecond())
volatile unsigned long timer0_overflow_count = 0;
volatile unsigned long timer0_millis = 0;
static unsigned char timer0_fract = 0;

void upTime0() {
  sei();
  TCCR0A = (1 << WGM01) | (1 << WGM00);
  TCCR0B = (1 << CS01) | (1 << CS00);
  TIMSK0 |= (1 << TOIE0);
} ISR(TIMER0_OVF_vect) {
  timer0_millis += MILLIS_INC;
  timer0_fract += FRACT_INC;
  if (timer0_fract >= FRACT_MAX) {
    timer0_fract -= FRACT_MAX;
    timer0_millis++;
  }
  timer0_overflow_count++;
}

unsigned long millis() {
  uint8_t oldSREG = SREG;   // запомнинаем были ли включены прерывания
  cli();                    // выключаем прерывания
  unsigned long m = timer0_millis;  // перехватить значение
  SREG = oldSREG;           // если прерывания не были включены - не включаем и наоборот
  return m;                 // вернуть миллисекунды
}

unsigned long micros() {
  uint8_t oldSREG = SREG;     // запомнинаем были ли включены прерывания
  cli();                      // выключаем прерывания
  unsigned long m = timer0_overflow_count;  // счет переполнений
  uint8_t t = TCNT0;                        // считать содержимое счетного регистра
  if ((TIFR0 & _BV(TOV0)) && (t < 255))     // инкремент по переполнению
    m++;
  SREG = oldSREG;                 // если прерывания не были включены - не включаем и наоборот
  return (long)(((m << 8) + t) * MICROS_MULT); // вернуть микросекунды
}

uint32_t rmillis(){
    return timer0_millis;
}

#endif