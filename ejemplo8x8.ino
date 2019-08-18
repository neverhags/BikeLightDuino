#include <SoftwareSerial.h>
// update from SAnwandter: https://create.arduino.cc/projecthub/SAnwandter1/programming-8x8-led-matrix-23475a
// Heilner Garcia heilnergarcia@gmail.com

// Define all columns and rows pins
#define ROW_1 13
#define ROW_2 12
#define ROW_3 1
#define ROW_4 0
#define ROW_5 A2
#define ROW_6 A1
#define ROW_7 A0
#define ROW_8 A3

#define COL_1 2
#define COL_2 3
#define COL_3 4
#define COL_4 5
#define COL_5 6
#define COL_6 7
#define COL_7 8
#define COL_8 9

#define SENSOR_PIN A4

// Set interval time (microseconds)
int intervalT = 85;
// Set light sensor interval time (milliseconds)
int lightInt = 1000;
// H by defoult
int inChar = 72;
// Set initial sensor reference
float sensorMaxV = 2.5;
// inicialize a tmp var to save the last state from inChar
int lastInChar = 72;

const byte rows[] = {
    ROW_1, ROW_2, ROW_3, ROW_4, ROW_5, ROW_6, ROW_7, ROW_8
};
const byte col[] = {
  COL_1,COL_2, COL_3, COL_4, COL_5, COL_6, COL_7, COL_8
};

// Declare all "images" dataset
byte ALL[] = {B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000};
byte ND[] = {B1111111,B1111111,B1111111,B1111111,B1111111,B1111111,B1111111,B1111111};

byte HOLD1[] = {B11111111,B11111111,B11000011,B11000011,B11000011,B11000011,B11111111,B11111111};
byte HOLD2[] = {B11111111,B10000001,B10111101,B10100101,B10100101,B10111101,B10000001,B11111111};
byte HOLD3[] = {B00000000,B01111110,B01000010,B01011010,B01011010,B01000010,B01111110,B00000000};

byte LEFT1[] = {B00000000,B01111111,B01111111,B01100000,B01101111,B01101111,B01101100,B01101100};
byte LEFT2[] = {B11111111,B11111111,B11000000,B11011111,B11011111,B11011000,B11011011,B11011011};
byte LEFT3[] = {B11111111,B10000000,B10111111,B10111111,B10110000,B10110111,B10110111,B10110110};

byte RIGHT1[] = {B11011011,B11011011,B00011011,B11111011,B11111011,B00000011,B11111111,B11111111};
byte RIGHT2[] = {B01101101,B11101101,B11101101,B00001101,B11111101,B11111101,B00000001,B11111111};
byte RIGHT3[] = {B10110110,B00110110,B11110110,B11110110,B00000110,B11111110,B11111110,B00000000};

unsigned long timeCount = 0;    // This use micros or millis to set time register
unsigned long sensorTimeCount = 0;    // This use micros or millis to set time register
String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete
SoftwareSerial SerialBT(10, 11); // RX, TX
bool serialOff = true;


void setup() {
    // Open serial port
    SerialBT.begin(57600);

    // Set all used pins to OUTPUT
    // This is very important! If the pins are set to input
    // the display will be very dim.
    for (byte i = 0; i <= 17 ; i++)
      if(i!=10&&i!=11) // 10 & 11 needed to BT device
        pinMode(i, OUTPUT);
    SerialBT.print("Starting...\r\n");
}

void loop() {
   if (SerialBT.available()) {
      char c = (char)SerialBT.read();
      // don't take new line chars
      if((int)c != 13 && (int)c != 10)
        inChar = (int)c;
      SerialBT.println(inChar);
   }
   int sensorValue = analogRead(SENSOR_PIN);
    // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
    float voltage = sensorValue * (5.0 / 1023.0);
    // print out the value you read:
    if(millis() - sensorTimeCount > lightInt && !serialOff){
      SerialBT.print("v:");
      SerialBT.print(voltage);
      SerialBT.print(" - Ref: ");
      SerialBT.println(5-sensorMaxV); // show level required
      sensorTimeCount = millis();
    }

    // Voltage controller: must be independent from sensorMaxV
    switch (inChar) {
       case 83: // S: Activate or deactivate 
          serialOff = !serialOff;
          inChar = lastInChar;
        break;
       case 86: // V: Require more light to activate
          if(sensorMaxV < 5) 
            sensorMaxV = sensorMaxV+0.5;
          inChar = lastInChar;
        break;
        case 66: // B: require less light to activate
          if(sensorMaxV > 0)
              sensorMaxV -=0.5;
            inChar = lastInChar;
        break;
    }
    if(lastInChar != inChar)
      lastInChar = inChar;
    // I know isn't the best way but was a fast and clear way...
    if(voltage < sensorMaxV)
      switch(inChar){
        case 76: // L
          if(millis() - timeCount <  intervalT) {
            drawScreen(LEFT1);
          } else if (millis() - timeCount <  intervalT * 2) {
            drawScreen(LEFT2);
            }else if (millis() - timeCount <  intervalT * 3) {
              drawScreen(LEFT3);
            } else {
            // back to the start
            timeCount = millis();
          }
        break;
        case 82: // R
          if(millis() - timeCount <  intervalT) {
              drawScreen(RIGHT1);
              } else if (millis() - timeCount < intervalT *  2) {
              drawScreen(RIGHT2);
              }else if (millis() - timeCount < intervalT *  3) {
                drawScreen(RIGHT3);
              } else {
              // back to the start
              timeCount = millis();
            }
        break;
        case 65: // A
          drawScreen(ALL);
        break;
        default: // H... 72
          if(millis() - timeCount <  intervalT) {
            drawScreen(HOLD1);
            } else if (millis() - timeCount < intervalT *  2) {
            drawScreen(HOLD2);
            }else if (millis() - timeCount < intervalT *  3) {
              drawScreen(HOLD3);
            } else {
            // back to the start
            timeCount = millis();
          }
        break;
      }
}


void  drawScreen(byte buffer2[]) {
   // Turn on each row in series
    for (byte i = 0; i < 8; i++) {       // count next row
        digitalWrite(rows[i], HIGH);    //initiate whole row
        for (byte a = 0; a < 8; a++)    // count next row
        {
          // if You set (~buffer2[i] >> a) then You will have positive
          digitalWrite(col[a], (buffer2[i] >> a) & 0x01); // initiate whole column

          delayMicroseconds(100);       // uncoment deley for diferent speed of display
          //delayMicroseconds(1000);
          //delay(10);
          //delay(100);

          digitalWrite(col[a], 1);      // reset whole column
        }
        digitalWrite(rows[i], LOW);     // reset whole row
        // otherwise last row will intersect with next row
    }
}

  /* this is siplest resemplation how for loop is working with each row.
    digitalWrite(COL_1, (~b >> 0) & 0x01); // Get the 1st bit: 10000000
    digitalWrite(COL_2, (~b >> 1) & 0x01); // Get the 2nd bit: 01000000
    digitalWrite(COL_3, (~b >> 2) & 0x01); // Get the 3rd bit: 00100000
    digitalWrite(COL_4, (~b >> 3) & 0x01); // Get the 4th bit: 00010000
    digitalWrite(COL_5, (~b >> 4) & 0x01); // Get the 5th bit: 00001000
    digitalWrite(COL_6, (~b >> 5) & 0x01); // Get the 6th bit: 00000100
    digitalWrite(COL_7, (~b >> 6) & 0x01); // Get the 7th bit: 00000010
    digitalWrite(COL_8, (~b >> 7) & 0x01); // Get the 8th bit: 00000001
}*/
