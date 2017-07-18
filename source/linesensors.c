#include "hardware/linesensors.h"

#include <avr/io.h>

linesensors_t linesensors;
linesensors_t threshold;

void
linesensors_init()
{
  ADCSRA |= ((1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0));
  ADMUX |= (1 << REFS0);
  ADCSRA |= (1 << ADEN);
  ADCSRA |= (1 << ADSC);
  ADMUX |= (1 << ADLAR);
}

void
linesensors_update()
{
  ADMUX &= 0xF0;
  ADCSRA |= (1 << ADSC);
  while (ADCSRA & (1 << ADSC))
    ;
  linesensors.left = ADCH;
  ADMUX += 1;
  ADCSRA |= (1 << ADSC);
  while (ADCSRA & (1 << ADSC))
    ;
  linesensors.midl = ADCH;
  ADMUX += 1;
  ADCSRA |= (1 << ADSC);
  while (ADCSRA & (1 << ADSC))
    ;
  linesensors.mid = ADCH;
  ADMUX += 1;
  ADCSRA |= (1 << ADSC);
  while (ADCSRA & (1 << ADSC))
    ;
  linesensors.midr = ADCH;
  ADMUX += 1;
  ADCSRA |= (1 << ADSC);
  while (ADCSRA & (1 << ADSC))
    ;
  linesensors.right = ADCH;
}

void
linesensors_set_threshold(linesensors_t _threshold)
{
  threshold.left = _threshold.left;
  threshold.midl = _threshold.midl;
  threshold.mid = _threshold.mid;
  threshold.midr = _threshold.midr;
  threshold.right = _threshold.right;
}

line_t
linesensors_get()
{
  linesensors_update();
  line_t line;
  line = (linesensors.left >= threshold.left) << 0;
  line |= (linesensors.midl >= threshold.midl) << 1;
  line |= (linesensors.mid >= threshold.mid) << 2;
  line |= (linesensors.midr >= threshold.midr) << 3;
  line |= (linesensors.right >= threshold.right) << 4;
  return line;
}