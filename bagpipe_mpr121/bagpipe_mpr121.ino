#include <Wire.h>
#include "Adafruit_MPR121.h"
#include <USB-MIDI.h>

USBMIDI_CREATE_DEFAULT_INSTANCE();

const uint16_t fingering_positions[]={
  2303, // SI -> DO#
  // primera octava
  2302, // DO -> RE
  2300, // RE -> MI
  2298, // MIb -> FA
  2296, // MI -> FA#
  2290, // FA -> SOL
  2288, // FA -> SOL
  2287, // SOL -> LA (DO#)
  2286, // SOL -> LA (RE)
  2284, // SOL -> LA (MI)
  2282, // SOL -> LA (FA)
  2280, // SOL -> LA (FA#)
  2274, // SOL -> LA
  2272, // SOL -> LA
  2256, // LAb -> SIb (SOL)
  2260, // LAb -> SIb
  2240, // LA -> SI
  2242, // LA -> SI
  2208, // SIb -> DO
  2210, // SIb -> DO
  2176, // SI -> DO#
  2178, // SI -> DO#
  // segunda octava
  2174, // DO -> RE
  2172, // RE -> MI
  2168, // MI -> FA#
  2160, // FA -> SOL
  2144, // SOL -> LA
  2112 // LA -> SI
};

char notes[][20]={
  "SI -> DO#",
  // primera octava
  "DO -> RE",
  "RE -> MI",
  "MIb -> FA",
  "MI -> FA#",
  "FA -> SOL",
  "FA -> SOL",
  "SOL -> LA (DO#)",
  "SOL -> LA (RE)",
  "SOL -> LA (MI)",
  "SOL -> LA (FA)",
  "SOL -> LA (FA#)",
  "SOL -> LA",
  "SOL -> LA",
  "LAb -> SIb (SOL)",
  "LAb -> SIb",
  "LA -> SI",
  "LA -> SI",
  "SIb -> DO",
  "SIb -> DO",
  "SI -> DO#",
  "SI -> DO#",
  // segunda octava
  "DO -> RE",
  "RE -> MI",
  "MI -> FA#",
  "FA -> SOL",
  "SOL -> LA",
  "LA -> SI",
  // silencio
  "SILENCE"
};

const uint16_t midi_notes[]={
  59, // SI -> DO#
  // primera octava
  60, // DO -> RE
  62, // RE -> MI
  63, // MIb -> FA
  64, // MI -> FA#
  65, // FA -> SOL
  65, // FA -> SOL
  67, // SOL -> LA (DO#)
  67, // SOL -> LA (RE)
  67, // SOL -> LA (MI)
  67, // SOL -> LA (FA)
  67, // SOL -> LA (FA#)
  67, // SOL -> LA
  67, // SOL -> LA
  68, // LAb -> SIb (SOL)
  68, // LAb -> SIb
  69, // LA -> SI
  69, // LA -> SI
  70, // SIb -> DO
  70, // SIb -> DO
  71, // SI -> DO#
  71, // SI -> DO#
  // segunda octava
  72, // DO -> RE
  74, // RE -> MI
  76, // MI -> FA#
  77, // FA -> SOL
  79, // SOL -> LA
  81 // LA -> SI
};

// You can have up to 4 on one i2c bus but one is enough for testing!
Adafruit_MPR121 cap = Adafruit_MPR121();

// Keeps track of the last pins touched
// so, we know when buttons are 'released'
uint8_t last_note = 0;
uint16_t currtouched = 0;


void setup () {
  Serial.begin(9600);

  while (!Serial) { // needed to keep leonardo/micro from starting too fast!
    delay(10);
  }

  Serial.println("Adafruit MPR121 Capacitive Touch sensor test");

  // Default address is 0x5A, if tied to 3.3V its 0x5B
  // If tied to SDA its 0x5C and if SCL then 0x5D
  if (!cap.begin(MPR121_I2CADDR_DEFAULT)) {
    Serial.println("MPR121 not found, check wiring?");
    while (1);
  }
  Serial.println("MPR121 found!");

  MIDI.begin(1);
  Serial.println("MIDI intialized!");
}

void loop() {
  // Get the currently touched pads
  currtouched = cap.touched();
  Serial.println(currtouched);

  uint8_t new_note = 27;
  for (uint8_t i = 0; i < 26; i++) {
    if (currtouched == fingering_positions[i]){
      new_note = i;
      break;
    }
  }
  //Serial.print("New note ");Serial.println(notes[new_note]);

  if (new_note != last_note) {
    if (last_note != 27){
      //Serial.print("Stopping ");Serial.println(notes[last_note]);
      MIDI.sendNoteOff(midi_notes[last_note], 127, 1);
    }
    if (new_note != 27){
      //Serial.print("Sending ");Serial.println(notes[new_note]);
      MIDI.sendNoteOn(midi_notes[new_note], 127, 1);
    }
    //Serial.print("New note ");Serial.println(notes[new_note]);
    last_note = new_note;
  }

  return;

  // put a delay so it isn't overwhelming
  delay(1);
}
