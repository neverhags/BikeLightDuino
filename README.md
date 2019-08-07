# 8x8 Matrix led example to mount on a bike

### This code provide 4 display modes:

Hold [H]: Runing 

Left [L]: Turn Left

Right [R]: Turn Right

Stop [A]: ...before i die

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

This project use a photoresistor to turn on/off the matrix in pin A4:

            int sensorValue = analogRead(A4);
