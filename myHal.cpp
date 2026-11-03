//  Sample myHal.cpp file.
//
// To use this file, copy it to myHal.cpp and uncomment the directives and/or
// edit them to satisfy your requirements.  If you only want to use up to
// two MCP23017 GPIO Expander modules and/or up to two PCA9685 Servo modules,
// then you don't need this file as DCC++EX configures these for free!

// Note that if the file has a .cpp extension it WILL be compiled into the build
// and the halSetup() function WILL be invoked.
//
// To prevent this, temporarily rename the file to myHal.txt or similar.
//

// The #if directive prevent compile errors for Uno and Nano by excluding the
//  HAL directives from the build.
#if !defined(IO_NO_HAL)

// Include devices you need.
#include "IODevice.h"
//#include "IO_HALDisplay.h"  // Auxiliary display devices (LCD/OLED)
//#include "IO_HCSR04.h"    // Ultrasonic range sensor
//#include "IO_VL53L0X.h"   // Laser time-of-flight sensor
//#include "IO_DFPlayer.h"  // MP3 sound player
//#include "IO_TouchKeypad.h  // Touch keypad with 16 keys
//#include "IO_EXTurntable.h"   // Turntable-EX turntable controller
//#include "IO_EXFastClock.h"  // FastClock driver
//#include "IO_PCA9685pwm.h"
#include "IO_EXSensorCAM.h"   // sensorCAM driver
//#include "IODeviceList.h"
//#include "IO_PCF8574.h"

//==========================================================================
// The function halSetup() is invoked from CS if it exists within the build.
// The setup calls are included between the open and close braces "{ ... }".
// Comments (lines preceded by "//") are optional.
//==========================================================================

void halSetup() {
 
  //=======================================================================
  // The following directive defines a PCA9685 PWM Servo driver module.
  //=======================================================================
  // The parameters are:
  //   First Vpin=100
  //   Number of VPINs=16 (numbered 100-115)
  //   I2C address of module=0x40
  /*
    PCA9685::create(100, 16, 0x60);
    PCA9685::create(116, 16, 0x61);
    PCA9685::create(132, 16, 0x62);
  */

  //=======================================================================
  // The following directive defines an MCP23017 16-port I2C GPIO Extender module.
  //=======================================================================
  // The parameters are:
  //   First Vpin=196
  //   Number of VPINs=16 (numbered 196-211)
  //   I2C address of module=0x22

  I2CManager.forceClock(100000);



 // EXSensorCAM::create(700, 80, 0x11); - now create in myautomation



}
#endif
