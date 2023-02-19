#include <Wire.h>
#include "Adafruit_MPR121.h"
#include <USB-MIDI.h>

USBMIDI_CREATE_DEFAULT_INSTANCE();

const uint16_t fingering_positions[]={
  2303, 2302, 2301, 2300, 2298, 2296, 2297, 2299, 2292, 2295, 2290, 2294, 2289, 2293, 2288, 2291, 2295, 2294, 2284, 2280, 2287, 2277, 2279, 2275, 2273, 2274, 2278, 2276, 2272, 2286, 2263, 2271, 2259, 2270, 2268, 2264, 2266, 2258, 2256, 2269, 2267, 2260, 2262, 2257, 2265, 2261, 2246, 2253, 2242, 2254, 2252, 2244, 2255, 2245, 2243, 2247, 2241, 2248, 2249, 2240, 2250, 2251, 2220, 2214, 2223, 2208, 2217, 2213, 2219, 2209, 2221, 2212, 2211, 2210, 2216, 2222, 2218, 2215, 2188, 2181, 2187, 2190, 2186, 2182, 2178, 2183, 2189, 2179, 2184, 2176, 2185, 2177, 2180, 2191, 2175, 2239, 2052, 2055, 2050, 2058, 2059, 2054, 2063, 2061, 2056, 2057, 2062, 2051, 2049, 2053, 2048, 2060, 2174, 2236, 2238, 2232, 2230, 2234, 2228, 2226, 2224, 2173, 2172, 2170, 2169, 2168, 2171, 2161, 2165, 2163, 2166, 2162, 2160, 2167, 2164, 2156, 2152, 2159, 2147, 2148, 2151, 2149, 2150, 2145, 2144, 2146, 2158, 2136, 2143, 2138, 2135, 2132, 2142, 2133, 2134, 2137, 2141, 2128, 2131, 2129, 2130, 2139, 2140, 2123, 2115, 2124, 2116, 2112, 2118, 2117, 2114, 2126, 2127, 2113, 2125, 2120, 2119, 2122, 2121, 2085, 2092, 2083, 2080, 2084, 2088, 2091, 2089, 2086, 2094, 2093, 2087, 2095, 2090, 2082, 2081, 2111, 2110
};

const uint16_t midi_notes[]={
  59, 60, 61, 62, 63, 64, 64, 64, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 66, 66, 66, 67, 67, 67, 67, 67, 67, 67, 67, 67, 68, 68, 68, 68, 68, 68, 68, 68, 68, 68, 68, 68, 68, 68, 68, 68, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 71, 71, 71, 71, 71, 71, 71, 71, 71, 71, 71, 71, 71, 71, 71, 71, 71, 71, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 73, 74, 75, 76, 76, 76, 77, 77, 77, 77, 77, 77, 77, 77, 78, 78, 78, 79, 79, 79, 79, 79, 79, 79, 79, 79, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 81, 81, 81, 81, 81, 81, 81, 81, 81, 81, 81, 81, 81, 81, 81, 81, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 83, 84
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
  // Serial.println(currtouched);

  uint8_t new_note = 197;
  for (uint8_t i = 0; i < 196; i++) {
    if (currtouched == fingering_positions[i]){
      new_note = i;
      break;
    }
  }

  if (new_note != last_note) {
    if (last_note != 197){
      MIDI.sendNoteOff(midi_notes[last_note], 127, 1);
    }
    if (new_note != 197){
      MIDI.sendNoteOn(midi_notes[new_note], 127, 1);
    }
    last_note = new_note;
  }
}
