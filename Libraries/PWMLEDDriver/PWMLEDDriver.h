/***************************************************
  This is a library for our Adafruit 16-channel PWM & Servo driver

  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/products/815

  These displays use I2C to communicate, 2 pins are required to
  interface. For Arduino UNOs, thats SCL -> Analog 5, SDA -> Analog 4

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
 ****************************************************/

/****************************************************
  The Adafruit supplied PWMServoDriver library modified by
  John A. Anderson/Sailindaze to improve flexibility and allow
  access to further setup options on the board - specifically
  meant to drive LEDs.
*****************************************************/

#ifndef _PWMLEDDriver_H
#define _PWMLEDDriver_H

#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#define PCA9685_MODE1 0x00
#define PCA9685_MODE2 0x01

#define OUTMODEINVOn    0x10        // bit 4 of mode2 register controls output inverson
#define OUTMODEINVOff   0x00
#define OUTMODEOCHACK   0x04        // bit 3 of mode2 register controls output change timing
#define OUTMODEOCHStop  0x00
#define OUTMODEDRVOD    0x00        // bit 2 of mode2 register controls output driver type
#define OUTMODEDRVTotem 0x02
#define OUTMODEOENLow   0x00        // bits 0 and 1 of mode2 register control output enable function
#define OUTMODEOENOutd  0x01
#define OUTMODEOENHZ    0x10

#define PCA9685_SUBADR1 0x2
#define PCA9685_SUBADR2 0x3
#define PCA9685_SUBADR3 0x4

#define PCA9685_PRESCALE 0xFE

#define LED0_ON_L 0x6
#define LED0_ON_H 0x7
#define LED0_OFF_L 0x8
#define LED0_OFF_H 0x9

#define ALLLED_ON_L 0xFA
#define ALLLED_ON_H 0xFB
#define ALLLED_OFF_L 0xFC
#define ALLLED_OFF_H 0xFD


class PWMLEDDriver {
 public:
  PWMLEDDriver(uint8_t addr = 0x40);
  boolean begin(void);
  void reset(void);
  void setPWMFreq(float freq);
  void setPWM(uint8_t num, uint16_t on, uint16_t off);
  void setPWMOutputMode(uint8_t PWMOutMode);

 private:
  uint8_t _i2caddr;

  uint8_t read8(uint8_t addr);
  void write8(uint8_t addr, uint8_t d);
};

#endif
