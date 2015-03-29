# LG intelligent sensor

## What you will need
* Arduino (Duemilanove or similar)
* max232
* 4 1 micro F. capacitors
* Some cables
* 1 photoresistor
* A couple of resistors to *calibrate* your measures (mine were 10K each)

## The set up
The setup is pretty simple. You need to connect the capacitors as it is
indicated in the [max232 datasheet](http://datasheets.maximintegrated.com/en/ds/MAX220-MAX249.pdf).
The 10K resistors are not strictly necessary, but I helped me to get a better
range of measures.

Connect the Arduino's Tx pin to T1in and connect T1out to LG TV RS232 pin #2 (Rd).
Make sure everything has a common GND (connect RS232 pin #5 to
Arduino's GND pin).

![schematics](https://github.com/fernape/arduino-projects/blob/master/digital/lg_intelligent_sensor/LG-intelligent-sensor.png))
![real connection](https://github.com/fernape/arduino-projects/blob/master/digital/lg_intelligent_sensor/shot.jpg)
