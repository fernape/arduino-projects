# Mouse-like device with Arduino

## What you will need
* Arduino (Duemilanove or similar)
* Acceleromter (Robonova 2-axis accelerometer in this case)
* 1 10K resistor
* Some cables
* 1 switch (TS1102S or similar)

## The set up
First we need to connect the accelerometer. The one I use has 6 connections (Vcc, Gnd and Signal for each axis).
Then we need to connect the swith as shown in the schematics. It needs signal, Vcc and Gnd via a 10K resistor.

I use digital pin #2 to read the value of the button and analog inputs #2 and #3 for the signals from the accelerometer.
It is trivial to change these values in the code to fit your needs.

To compile the code for the Arduino just type:

`gmake`

and then, to load it into the board:

`sudo avrdude -V -F -c stk500v1 -p m168 -b 19200 -P /dev/cuaU0 -U flash:w:mouse_device.hex`

Note: Remember to change the above command to fit your file name, Arduino version, etc.

Here is a short [video]()

![schematics](https://github.com/fernape/arduino-projects/blob/master/digital/arduimotion/Schematics.png)
