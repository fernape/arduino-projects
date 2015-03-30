# LG jedi-like volume control :)

## What you will need
* Arduino (Duemilanove or similar)
* max232
* 4 1 micro F. capacitors
* Some cables
* 1 SRF05 ultrasonic sensor

## The set up
The setup is pretty simple. You need to connect the capacitors as it is
indicated in the [max232 datasheet](http://datasheets.maximintegrated.com/en/ds/MAX220-MAX249.pdf).

There are two main files, one for the single pin configuration and the other
to be used with two different lines for trigger and echo. In the example I use
the single pin configuration.

Connect the Arduino's Tx pin to T1in and connect T1out to LG TV RS232 pin #2 (Rd).
Make sure everything has a common GND (connect RS232 pin #5 to Arduino's GND pin).

![schematics](https://github.com/fernape/arduino-projects/blob/master/digital/lg_ultrasound_vol_control/LG-volume-control.png)
![real connection](https://github.com/fernape/arduino-projects/blob/master/digital/lg_ultrasound_vol_control/shot.png)
