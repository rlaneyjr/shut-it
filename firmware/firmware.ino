// shut-it firmware
//
// 1. turns on a LED when power is applied
// 2. waits 30 seconds
// 3. starts playing a two-tone melody
//
// Author Epp Õlekõrs
// License MIT

// https://github.com/sweebee/Arduino-home-automation/blob/master/libraries/readVcc/readVcc.h
long readVcc() {
  // Read 1.1V reference against AVcc
  // set the reference to Vcc and the measurement to the internal 1.1V reference
  #if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
    ADMUX = _BV(REFS0) | _BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  #elif defined (__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
     ADMUX = _BV(MUX5) | _BV(MUX0);
  #else
    ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  #endif

  delay(2); // Wait for Vref to settle
  ADCSRA |= _BV(ADSC); // Start conversion
  while (bit_is_set(ADCSRA,ADSC)); // measuring

  uint8_t low  = ADCL; // must read ADCL first - it then locks ADCH
  uint8_t high = ADCH; // unlocks both

  long result = (high<<8) | low;

  result = 1125300L / result; // Calculate Vcc (in mV); 1125300 = 1.1*1023*1000
  return result; // Vcc in millivolts
}

void lightOn() {
  digitalWrite(11, HIGH);
}

void lightOff() {
  digitalWrite(11, LOW);
}

void setup() {
  pinMode(11, OUTPUT); // led
  pinMode(A2, OUTPUT); // buzzer
  lightOn();

  // I am using 2 x AAA batteries in series, with the max. output voltage around 2.5v.
  if (readVcc()/1000.0 < 1.9) {
    delay(1000);
    lightOff();
    delay(1000);
    lightOn();
    delay(1000);
    lightOff();
    delay(1000);
    lightOn();
  }
  delay(30000);
}

int buzzerPin = A2;
int tempo = 200;
char notes[] = "eeeeeeegcde fffffeeeeddedg";
int duration[] = {1, 1, 2, 1, 1, 2, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2};


void playTheShit(char note, int duration) {
  char notesName[] = { 'c', 'd', 'e', 'f', 'g' };
  int tones[] = { 261, 293, 329, 349, 392 };

  for (unsigned int i = 0; i < sizeof(notesName); i++) {
    // Bind the note took from the char array to the array notesName
    if (note == notesName[i]) {
      // Bind the notesName to tones
      tone(buzzerPin, tones[i], duration);
    }
  }
}

void playMelody() {
  /* tone(A2, 1100, 100);
  delay(1000);
  tone(A2, 1319, 400);
  delay(1000); */

  // Scan each char from "notes"
  for (unsigned int i = 0; i < sizeof(notes)-1; i++) {
    if (notes[i] == ' ') {
      // If find a space it rests
      delay(duration[i] * tempo);
    } else {
      playTheShit(notes[i], duration[i] * tempo);
    }

    // Pauses between notes
    delay((tempo*2)*duration[i]);
  }
}

void loop() {
  playMelody();
}
