#ifndef myTurnouts_h
#define myTurnouts_h
#include <Arduino.h>
#include "IODevice.h"
class TurnoutStates {
  public:
    static void set(const char* key, const uint8_t state);
    static void get(const char* key, const VPIN vpin);
};
#endif