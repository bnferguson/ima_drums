#include <Wire.h>
#include <Serial.h>
#include <math.h>
#include <PWMLEDDriver.h>

// How many Channels we're driving on this board
// 12'' Drum - 7 Channels
// 16'' Drum - 7 Channels
// 20'' Drum - 10 Channels
// 24'' Drum - 11 Channels
#define NUMBER_OF_CHANNELS 11

// How long to fade out. A higher fraction is longer.
#define FADE_MULTIPLIER 0.92

// Do we write to the channels in a random order?
// Setting to true creates a twinkling pattern.
// Setting to false keeps even fades.
#define USE_RANDOM_WRITES true

// Defines how many cycles before decrementing
// maxSignal. Higher means it decrements slower
#define CEILING_RESET_COUNTER 100

// called this way, it uses the default address 0x40
PWMLEDDriver pwm = PWMLEDDriver();

// PWM Frequency, 60 is good for default, but with random writes 120 is noticably smoother.
uint8_t PWMF = 120;

// hold the duty cycles we'll be sending to the channels
static double channelBrightness[NUMBER_OF_CHANNELS];

// The default order we write to the channels in
static int defaultWriteOrder[NUMBER_OF_CHANNELS];

// Largest signal we've seen from the sensor
int maxSignal = 0;
int ceilingCounter = 0;

// The interval that determines if a channel is active or not based on the input from 0-100.
float interval = 100/NUMBER_OF_CHANNELS - 1;


void setup() {
  pwm.begin();
  randomSeed (analogRead (0));

  uint8_t outMode = OUTMODEINVOn | OUTMODEOCHACK | OUTMODEDRVOD | OUTMODEOENHZ;
  pwm.setPWMOutputMode(outMode);
  pwm.setPWMFreq(PWMF);

  // Establish the default write order.
  for (int index = 0; index < NUMBER_OF_CHANNELS; index++) {
    defaultWriteOrder[index] = index;
  }

  resetChannels();
}

void resetChannels() {
  for (int thisLED = 0; thisLED < NUMBER_OF_CHANNELS; thisLED++) {
    setChannelPulse(thisLED, 0);
  }
}

// used to set the pulse length in percentage
// e.g. setChannelPulse(0, 50) is a 50% duty cycle pulse width.
void setChannelPulse(uint8_t n, double pulse) {

  if (pulse >= 100) {
    pwm.setPWM(n, 0, 4096);
  }
  else {
    if (pulse <= 0) {
      pwm.setPWM(n, 4096, 0);
    }
    else {
      pulse /= 100;
      pulse *= 4096;

      pwm.setPWM(n, 0, 4096 - pulse);
    }
  }
}


void loop() {
  int activeChannels = 1;
  long sensorValue;

  sensorValue = analogRead(0);

  adjustCeiling(sensorValue);

  // map it to the range of the analog out:
  float mappedSensorValue = map(sensorValue, 0, maxSignal, 0, 100);
  activeChannels = int(mappedSensorValue/interval);

  setBaseChannelBrightness(mappedSensorValue, activeChannels);
  setChannelBrightness(activeChannels);

  if(USE_RANDOM_WRITES) {
    sendRandomizedChannelValuesToController();
  } else {
    sendChannelValuesToController();
  }
}

// Makes sure that the we're mapping to sensor values
// we're actually using
void adjustCeiling(long sensorValue) {
  if(sensorValue > maxSignal) {
    maxSignal = sensorValue;
  }
  if(maxSignal > 1) {
    if(ceilingCounter >= CEILING_RESET_COUNTER) {
      maxSignal *= 1/log(maxSignal);
      ceilingCounter = 0;
    } else {
      ceilingCounter++;
    }
  }
}

// Since each Channel's brightness is off the previous one, we start by setting the
// first channel's brightness. If the sensor value is enough to activate a channel
// we set it, else we begin fading it.
void setBaseChannelBrightness(float mappedSensorValue, int activeChannels) {
  if(activeChannels > 0) {
    channelBrightness[0] = mappedSensorValue;
  } else {
    channelBrightness[0] = fadeBrightness(channelBrightness[0]);
  }
}

// Return an adjusted input. If it gets too low, just set it to 0
// (controller freaks out when you start sending it really low numbers)
float fadeBrightness(float brightness) {
  if(brightness < 0.01) {
    return 0;
  } else {
    return brightness * FADE_MULTIPLIER;
  }
}

// Here we set the rest of the channels. If the channels are active, we update their values.
// If they are not, we fade them.
void setChannelBrightness(int activeChannels) {
   for (int index = 1; index < NUMBER_OF_CHANNELS; index++) {
    if(index < activeChannels) {
      channelBrightness[index] = 0.9 * channelBrightness[index -1];
    } else {
      channelBrightness[index] = fadeBrightness(channelBrightness[index]);
    }
  }
}

// Sends the channel brightness values in random order for a sparkling effect.
void sendRandomizedChannelValuesToController() {
  int writeOrder[NUMBER_OF_CHANNELS];
  memcpy(writeOrder, &defaultWriteOrder[0], NUMBER_OF_CHANNELS * sizeof(int));

  // Shuffle the write order. But not completely.
  // Makes for a more interesting attack when some
  // are in the default position. Fully random is too random
  for (int a=0; a<NUMBER_OF_CHANNELS; a++) {
    int r = random(a,NUMBER_OF_CHANNELS);
    writeOrder[r] = writeOrder[a];
  }

  for (int index = 0; index < NUMBER_OF_CHANNELS; index++) {
    setChannelPulse(writeOrder[index], channelBrightness[index]);
  }

}

// Sends the channel brightness values in default order. Very even fading.
void sendChannelValuesToController() {
  for (int index = 0; index < NUMBER_OF_CHANNELS; index++) {
    setChannelPulse(index, channelBrightness[index]);
  }
}

