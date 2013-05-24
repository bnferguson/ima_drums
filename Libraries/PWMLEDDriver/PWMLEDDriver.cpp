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

#include <PWMLEDDriver.h>
#include <Wire.h>

PWMLEDDriver::PWMLEDDriver(uint8_t addr) {
  _i2caddr = addr;
}

boolean PWMLEDDriver::begin(void) {
 Wire.begin();
 reset();
}

void PWMLEDDriver::reset(void) {
 write8(PCA9685_MODE1, 0x0);
}

void PWMLEDDriver::setPWMFreq(float freq) {

  float prescaleval = 25000000;

  prescaleval /= 4096;
  prescaleval /= freq;
  prescaleval -= 1;
  uint8_t prescale = floor(prescaleval + 0.5);

  uint8_t oldmode = read8(PCA9685_MODE1);
  uint8_t newmode = (oldmode&0x7F) | 0x10;  // sleep
  write8(PCA9685_MODE1, newmode);           // go to sleep
  write8(PCA9685_PRESCALE, prescale);       // set the prescaler
  write8(PCA9685_MODE1, oldmode);           // tell the chip to wake
  delay(5);                                 // wait for restart
  write8(PCA9685_MODE1, oldmode | 0x80);    // reset the restart status bit

}

void PWMLEDDriver::setPWM(uint8_t num, uint16_t on, uint16_t off) {
  write8(LED0_ON_L+4*num, on);
  write8(LED0_ON_H+4*num, on >> 8);
  write8(LED0_OFF_L+4*num, off);
  write8(LED0_OFF_H+4*num, off >> 8);
}

void PWMLEDDriver::setPWMOutputMode(uint8_t mode){
  write8(PCA9685_MODE2, mode);
}

uint8_t PWMLEDDriver::read8(uint8_t addr) {
  Wire.beginTransmission(_i2caddr);
  Wire.write(addr);
  Wire.endTransmission();

  Wire.requestFrom((uint8_t)_i2caddr, (uint8_t)1);
  return Wire.read();
}

void PWMLEDDriver::write8(uint8_t addr, uint8_t d) {
  Wire.beginTransmission(_i2caddr);
  Wire.write(addr);
  Wire.write(d);
  Wire.endTransmission();
}
