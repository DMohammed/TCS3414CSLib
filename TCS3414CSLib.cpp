/****************************************************************************/
//
// TCS3414CSLib is an Arduino library to communicate to and obtain values from
// the TCS3414CS RGB color sensor.
//
// This work took minor parts (declarations)
// from the existing code by FrankieChu from www.seeedstudio.com
// available at https://github.com/Seeed-Studio/Grove_I2C_Color_Sensor

// Copyright (C) 2014, J.F. Omhover (jf.omhover@gmail.com)
//
// This file is part of TCS3414CSLib
//
// TCS3414CSLib is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// TCS3414CSLib is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with TCS3414CSLib.  If not, see <http://www.gnu.org/licenses/>.
//
/******************************************************************************/

#include <Arduino.h>
#include <Wire.h>
#include "TCS3414CSLib.h"

void _writeReg(byte reg, byte val) {
  Wire.beginTransmission(TCS3414CS_ADDR);
  Wire.write(reg | CMD_WRITE | CMD_TRANSACTION_BYTE); // systematically use
                                                      // CMD_WRITE bit to write
                                                      // values into registers
  Wire.write(val);
  Wire.endTransmission();
}

void TCS3414CS::init(int mode) {
  switch (mode) {
  case TCS3414CS_LEVELINTERRUPTMODE:
    // the process below will set an interrupt each time the value of the CLEAR
    // channel goes above 65530 (overflow, right ?)
    setLevelThreshold(REG_LOW_THRESH, 0x0000);
    setLevelThreshold(REG_HIGH_THRESH, 4000);
    _writeReg(
        REG_INT_SOURCE,
        INT_SOURCE_CLEAR); // set the interrupt source to the CLEAR channel
    break;
  case TCS3414CS_FREEMODE:
  default:
    _writeReg(REG_TIMING, INTEG_MODE_FREE | INTEG_PARAM_INTTIME_12MS);
    _writeReg(REG_INT, INTR_CTL_DISABLE);
    setGain(GAIN_1, PRESCALER_1); // set default gain value and prescaler value
    //      _writeReg(REG_CTL, 0x03); // Enable ADCs;
    break;
  };
}

void TCS3414CS::start() { enableADC(); }
void TCS3414CS::stop() { disableADC(); }

void TCS3414CS::getRGB(uint16_t *red, uint16_t *green, uint16_t *blue,
                       uint16_t *clr) {
  Wire.beginTransmission(TCS3414CS_ADDR);
  Wire.write(CMD_WRITE | REG_BLOCK_READ);
  Wire.endTransmission();

  delay(20);
  Wire.requestFrom(TCS3414CS_ADDR, 8);
  if(Wire.read()==8){

  }
  byte *b = (byte *)green;
  b[0] = Wire.read();
  b[1] = Wire.read();
  b = (byte *)red;
  b[0] = Wire.read();
  b[1] = Wire.read();
  b = (byte *)blue;
  b[0] = Wire.read();
  b[1] = Wire.read();
  b = (byte *)clr;
  b[0] = Wire.read();
}

void TCS3414CS::getValues(uint16_t *values) {
  Wire.beginTransmission(TCS3414CS_ADDR);
  Wire.write(CMD_WRITE | REG_BLOCK_READ);
  Wire.endTransmission();

  Wire.requestFrom(TCS3414CS_ADDR, 8);
  byte *b = (byte *)values;
  for (int i = 0; i < 8; i++) {
    b[i] = Wire.read();
  }
}

void TCS3414CS::disableADC() { _writeReg(REG_CTL, CTL_POWER); }

void TCS3414CS::enableADC() { _writeReg(REG_CTL, CTL_ADC_EN | CTL_POWER); }

void TCS3414CS::powerOn() { _writeReg(REG_CTL, CTL_POWER); }

void TCS3414CS::setLevelThreshold(byte reg, uint16_t thresh) {
  byte *b = (byte *)&thresh;
  Wire.beginTransmission(TCS3414CS_ADDR);
  Wire.write(reg | CMD_WRITE | CMD_TRANSACTION_WORD);
  Wire.write(b[0]);
  Wire.write(b[1]);
  Wire.endTransmission();
}

void TCS3414CS::setIntegrationTime(byte itime) { _writeReg(REG_TIMING, itime); }

void TCS3414CS::setGain(byte gain, byte prescaler) {
  _writeReg(REG_GAIN, gain | prescaler);
}

void TCS3414CS::clearInterrupt() {
  Wire.beginTransmission(TCS3414CS_ADDR);
  Wire.write(CMD_CLEARINT);
  Wire.endTransmission();
}
