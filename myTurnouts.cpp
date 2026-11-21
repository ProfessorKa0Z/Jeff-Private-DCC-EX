#include "myTurnouts.h"
#include "Preferences.h"
static const char TURNOUT_NAMESPACE[] = "turnoutstates";

void TurnoutStates::set(const char* key, const uint8_t state) {
  Preferences prefs;
  prefs.begin(TURNOUT_NAMESPACE, false); // Read-write
  prefs.putUChar(key, state);
  prefs.end();  
}
void TurnoutStates::get(const char* key, const VPIN vpin) {
  Preferences prefs;
  prefs.begin(TURNOUT_NAMESPACE, true); // Read-only
  uint8_t state = prefs.getUChar(key, 0); // Default to 0 if key not found
  prefs.end();
  IODevice::write(vpin, state); 
}
/*
#include "myTurnouts.h"
#ifdef ARDUINO_ARCH_ESP32
#include "Preferences.h"
#include "IODevice.h"

static const char TURNOUT_NAMESPACE[] = "turnoutstates";

void TurnoutStates::set(const char* key, uint8_t state) {
  Preferences prefs;
  prefs.begin(TURNOUT_NAMESPACE, false); // Read-write
  prefs.putUChar(key, state);
  prefs.end();  
}
void TurnoutStates::get(const char* key, uint16_t vpin) {
  Preferences prefs;
  prefs.begin(TURNOUT_NAMESPACE, true); // Read-only
  uint8_t state = prefs.getUChar(key, 0); // Default to 0 if key not found
  prefs.end();
  IODevice::write(vpin, state); // Write the state to the specified VPIN
};

#endif
*/