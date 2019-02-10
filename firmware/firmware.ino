// shut-it firmware
//
// 1. turns on a LED when power is applied
// 2. waits 30 seconds
// 3. starts playing a two-tone melody
//
// Author Epp Õlekõrs
// License MIT

void setup() {
  pinMode(11, OUTPUT); // led
  pinMode(A2, OUTPUT); // buzzer
  lightOn();
  delay(30000);
}

void lightOn() {
  digitalWrite(11, HIGH);
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
