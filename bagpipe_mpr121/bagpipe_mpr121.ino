#include <Wire.h>
#include "Adafruit_MPR121.h"
#include <USB-MIDI.h>

USBMIDI_CREATE_DEFAULT_INSTANCE();

uint8_t number_of_notes = 197;

const uint16_t fingering_positions[]={
  1279, 1278, 1277, 1276, 1274, 1272, 1273, 1275, 1270, 1268, 1264, 1269, 1267, 1266, 1265, 1271, 1270, 1271, 1256, 1260, 1263, 1255, 1252, 1250, 1251, 1253, 1249, 1248, 1254, 1262, 1242, 1247, 1244, 1233, 1240, 1232, 1238, 1243, 1235, 1246, 1241, 1239, 1237, 1234, 1236, 1245, 1231, 1218, 1230, 1216, 1219, 1224, 1227, 1225, 1223, 1217, 1229, 1220, 1226, 1222, 1228, 1221, 1189, 1196, 1191, 1197, 1187, 1190, 1184, 1193, 1186, 1188, 1198, 1194, 1195, 1185, 1192, 1199, 1152, 1155, 1163, 1167, 1153, 1162, 1166, 1160, 1165, 1161, 1164, 1158, 1159, 1157, 1154, 1156, 1151, 1215, 1033, 1035, 1029, 1025, 1028, 1027, 1038, 1036, 1030, 1034, 1032, 1026, 1037, 1024, 1039, 1031, 1150, 1214, 1204, 1206, 1208, 1202, 1212, 1210, 1200, 1149, 1148, 1146, 1144, 1145, 1147, 1143, 1138, 1137, 1140, 1136, 1139, 1141, 1142, 1128, 1132, 1135, 1121, 1122, 1120, 1126, 1124, 1125, 1127, 1123, 1134, 1114, 1113, 1111, 1110, 1116, 1117, 1112, 1106, 1109, 1104, 1118, 1107, 1115, 1108, 1119, 1105, 1091, 1090, 1094, 1093, 1089, 1096, 1103, 1095, 1098, 1092, 1099, 1097, 1101, 1100, 1102, 1088, 1071, 1056, 1069, 1062, 1070, 1057, 1066, 1059, 1067, 1058, 1065, 1064, 1060, 1063, 1061, 1068, 1087, 1086
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
  Serial.println(currtouched);

  uint8_t new_note = number_of_notes;
  for (uint8_t i = 0; i < number_of_notes - 1; i++) {
    if (currtouched == fingering_positions[i]){
      new_note = i;
      break;
    }
  }

  if (new_note != last_note) {
    if (last_note != number_of_notes){
      MIDI.sendNoteOff(midi_notes[last_note], 127, 1);
    }
    if (new_note != number_of_notes){
      MIDI.sendNoteOn(midi_notes[new_note], 127, 1);
    }
    last_note = new_note;
  }
}
