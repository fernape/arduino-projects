# Bubble generator with Arduino

## What you will need
* Arduino (Duemilanove or similar)
* Sound sensor 
* 3 LEDs (green, yellow, and red)
* 3 resistors to prevent burning out the LEDs (230 Ohms in my case)
* A diode
* A 100 uF capacitor
* A [power transistor](http://www.infineon.com/dgdl/irf540n.pdf?fileId=5546d462533600a4015355e396cb199f) 
* Some cables
* A potentiometer (optional)
* A [low power fan] (https://www.igogo.es/product341187.html)
* A mini bucket for the mix (I use a butter container)
* Some tape
* Some cardboard
* Water, dish soap & glycerine for the mix

## The set up
Below you can find the schematics for the project. There are a couple of important things to notice, though.

The first thing is that we need to control the fan with the arduino but the outputs of the arduino give ~20 mA that is not enought to drive the motor so we need to connect the fan to Vcc and Gnd via a power transistor (one that allows for more current to pass through). In addition we need to add a diode to prevent the [inductive kickback](https://www.maximintegrated.com/en/glossary/definitions.mvp/term/Inductive%20Kickback/gpk/175). When we switch the motor off, we get a spike of negative voltage that could damage our circuit.

In addition, the motor of the fan drains current from our circuit and that can generate noise that affects the servo making it move erratically. To avoid this, we can use a capacitor so the fan sucks the power from it and does not affect the Arduino too much.

Last, I used a potentionmeter (optional) to regulate the values returned by the sound sensor so we don't need to touch the software should we want to adjust the sensibility of the whole thing.

It is also important to make a good mix. If you use too much glycerine the superficial tension could prevent the bubbles from separating from the stick. If you use too few bubbles could explode even before they are completely formed.

For the structure I used a cardboard box and some cardboard sheets. It is important to clear the space behind the fan so it can suck some air from the rear (I made some wholes in the box)

To compile the code for the Arduino just type:

`gmake`

and then, to load it into the board:

`sudo avrdude -V -F -c stk500v1 -p m168 -b 19200 -P /dev/cuaU0 -U flash:w:sound_detector.hex`

Note: Remember to change the above command to fit your file name, Arduino version, etc.

Here is a short [video]()

The schematics

![schematics](https://github.com/fernape/arduino-projects/raw/master/sound_and_bubbles/Sound_Bubbles_schematic.png)

Here are some detailed pictures:

![photo1](https://github.com/fernape/arduino-projects/raw/master/sound_and_bubbles/images/IMG_20170114_174827.jpg)
![photo2](https://github.com/fernape/arduino-projects/raw/master/sound_and_bubbles/images/IMG_20170114_174834.jpg)
![photo3](https://github.com/fernape/arduino-projects/raw/master/sound_and_bubbles/images/IMG_20170114_174901.jpg)
![photo4](https://github.com/fernape/arduino-projects/raw/master/sound_and_bubbles/images/IMG_20170114_174909.jpg)
![photo5](https://github.com/fernape/arduino-projects/raw/master/sound_and_bubbles/images/IMG_20170121_194715.jpg)
![photo6](https://github.com/fernape/arduino-projects/raw/master/sound_and_bubbles/images/IMG_20170121_194745.jpg)
