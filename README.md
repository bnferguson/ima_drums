Reactive LED Drums
=====================================

Arduino Code for commission by the Indianpolis Museum of Art for their Winter Solstice drum circle in 2012. [John Anderson](http://sailindaze.blogspot.com/) did all of the electronics work, PWM driver code, and initial Arduino prototype. [Tom Streit](http://cargocollective.com/bigdudesculpture) and [Jordan Munson](http://www.jordanmunson.com/) did the physical drums, and [Brandon Ferguson](http://github.com/bnferguson) did the code.

You can see the drums in action [here](http://www.youtube.com/watch?v=t9sxI8FDv84) and [here](http://www.youtube.com/watch?feature=player_embedded&v=uGqpzUWrqMg), care of John Anderson.

Hardware
--------------------
In order to use this code you'll need an [Arduino](http://arduino.cc), an [Adafruit 16-Channel 12-bit PWM/Servo Driver - I2C interface - PCA9685](http://www.adafruit.com/products/815), a Piezo (or some other analog input), and a load of LEDs and resistors.

The input is Analog 0 on the Arduino and the output is via the PWM board. You can set the number of channels used in the code.


Compiling
-------------------
In order to get this to compile you'll need to move the PWMLEDDriver directory in Libraries over to your Libraries directory in your Sketchbooks directory (like any other library).

Once that's there you should have no problem compiling!


Usage Notes
---------------------
For the most part the code is commented and driven by the constants defined at the top of the ima_drums.ino file. There are four main variables you can change

`NUMBER_OF_CHANNELS` - How many channels to use on the PWM board. If you use more than is set you'll notice some of your LED circuits won't come on. If you use less you'll notice that you have some very sensitive circuits. The default is `11`.

`FADE_MULTIPLIER` - This is the param that determines how much to cut the current duty cycle by. That is, it determines how long the circuits take to fade. Larger fraction means longer. The default is `0.92`.

`USE_RANDOM_WRITES` - Turning this on causes the program to write to the circuits in a random manner. This produces a neat flickering effect in some of the circuits while others continue to fade smoothly. If you disable it the circuits will fade smoothly and predictably. The default is `true`.

`CEILING_RESET_COUNTER` - This program adjusts sensitivity as it is used. We essentially set a ceiling based on the largest signal we've seen in the last `CEILING_RESET_COUNTER` cycles. Setting this high means it resets slower, setting it low makes it reset faster. The default is `100`.



