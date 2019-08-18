# <a src=""><ing alt="BikeLightDuino image" src="https://raw.githubusercontent.com/neverhags/BikeLightDuino/master/image.jpeg" style="width:100px"/></a>BikeLightDuino  <p style="font-size:12px"> 8x8 Matrix led example</p>

## Requeriments:
 * HC-05 or HC-06 (Bluetooth device)
 * Photoresistor 10K/100K  (don't really care) - Connect to: VIN and A4 (SENSOR_PIN)
 * A resistor 10 times bigger than the photoresistor - Connect to: GND and A4 (SENSOR_PIN)
 * 8 220ohm~1k Ohm (to Rows or Cols, don't use directly)


## This code provide 4 display modes:

Hold [H]: Runing 

Left [L]: Turn Left

Right [R]: Turn Right

Stop [A]: ...before i crash/die

Stop [S]: Serial On/Off

Stop [V]: Needs less light to turn on

Stop [B]: Needs more light to turn on

All comands must be sended using the serial port on pin 10 (RX) and 11 (TX), you could connect a wifi or bluetooth to control it:

        SoftwareSerial SerialBT(10, 11); // RX, TX

## Pinout: 

        // Define all columns and rows pins
            #define COL_1 0
            #define COL_2 1
            #define COL_3 12
            #define COL_4 13
            #define COL_5 A0
            #define COL_6 A1
            #define COL_7 A2
            #define COL_8 A3
            #define ROW_1 2
            #define ROW_2 3
            #define ROW_3 4
            #define ROW_4 5
            #define ROW_5 6
            #define ROW_6 7
            #define ROW_7 8
            #define ROW_8 9

            #define SENSOR_PIN A4

This project use a photoresistor in pin A4 to turn on/off the matrix it can be controled using V and B command to ajust the intensity in real:


            int sensorValue = analogRead(SENSOR_PIN);


*Note: i aren't use EEPROM, if you want to save the config you must programm that feature.*