// Default MCP23017s at addresses 0x20 and 0x21: vpins 164 - 195 control H-bridges for turnouts
HAL(EXSensorCAM, SENSORCAM_VPIN,  80, 0x11) //SENSORCAM _VPIN defined in config.h
HAL(EXIOExpander, 800, 62, 0x65) // EX-IOExpander for signals

//---------- Pin definitions and latches ----------
// Turnout board has common pwm and stby pins for all drivers
ALIAS(pwm, 164) // GPA0 (164) - PWM
ALIAS(stby, 165) // GPA1 (165) - STBY

#define trk_pwr -10 // pin 10 connected to pin 11 to sense motor shield PWM state

HAL(Bitmap,1000,2)  // flags

ALIAS(trnblock, 254) // turnout sequences will reserve or free this block to moderate control of pwm and stby pins [must be less than 255 for free/reserve]

ALIAS(prog_red, 843)	// Turnout and LED for programming track join (pin 47 & 49 expander)
ALIAS(prog_green,845)	// 843 is red; 845 is green (active-low; common anode to 5v)
ALIAS(prog_amber,1000)	// unconnected;  just defined so amber will turn off LED
ALIAS(prog_sig, 843)

ALIAS(cl6flag,1001) // flag to indicate train in block 6 is clockwise

//---------- Control and indication of programming track ---------- 

SIGNAL(prog_red,prog_amber,prog_green) // bicolor LED on prog track;  set to amber to turn off

ALIAS(prog_t,13)
PIN_TURNOUT(prog_t, 0, "Prog track join") // turnout to join or unjoin programming track

AUTOSTART AMBER(prog_sig) CLOSE(prog_t) DONE // on startup, turn off LED and set turnout state

ONSENSOR(trk_pwr)  // monitor changes in track power
  IF(trk_pwr) CLOSE(prog_t) // upon power-on; set join turnout closed (will light red LED)
  ELSE AMBER(prog_sig) CLOSE(prog_t) // upon power-off, turn off LED; set join turnout closed
  ENDIF
DONE

	// THROWN is joined state. If power on, join and light green LED; if power off, just revert to close
  	// CLOSED is unjoined state. If power on, unjoin and light red LED; if power off, do nothing 
ONTHROW(prog_t) IF(trk_pwr) JOIN GREEN(prog_sig) PRINT("Programming track joined") ELSE CLOSE(prog_t) ENDIF DONE
ONCLOSE(prog_t) IF(trk_pwr) UNJOIN RED(prog_sig) PRINT("Programming track unjoined") ENDIF DONE

//---------- Relay for outer yard power
ALIAS(yard_t,11)
ALIAS(yard_pin,857) // gate of MOSFET controlling relay (pin A11)
PIN_TURNOUT(yard_t, 0, "Yard power")
AUTOSTART IFCLOSED(yard_t) CLOSE(yard_t) ELSE THROW(yard_t) ENDIF DONE  // Initialize:  match EEPROM state
ONTHROW(yard_t) SET(yard_pin) DONE
ONCLOSE(yard_t) RESET(yard_pin) DONE

//---------- Automations etc. ----------

#include "autofiles/auto.h"

// Virtual turnout to refresh all SensorCAM sensors

ALIAS(refresh,14)
VIRTUAL_TURNOUT(refresh,"Refresh SensorCAM")
ONTHROW(refresh)
  PARSE("<N r 00>")
  CLOSE(refresh)
DONE


ALIAS(camcheck)
AUTOSTART SEQUENCE(camcheck) // check sensor 20 (vpin 716) for SensorCAM response. Can be any undefined sensor.
  DELAY(1000)
  PARSE("<NL 20>") // latch sensor
  DELAY(500)
  IF(700+020)
    PARSE("<No 20>") // unlatch sensor
    DELAY(500)
    IFNOT(700+020)
      LCD(7,"SensorCAM connected")
      DELAY(60000) // update every 60 seconds
      FOLLOW(camcheck)
    ENDIF
  ENDIF
  LCD(7,"SensorCAM INACTIVE")
  DELAY(5000) // check again in 5 seconds
FOLLOW(camcheck)

//---------- Turnouts ----------

// Unitrack turnouts controlled by TB6612 H-bridges
// Each turnout associated with a red/green signal
// define TB6612 in1 / in2 pins for each turnout
// Since pwm and stby are common to all H-bridges, a reserve/free block is used to prevent simultaneous operation
// Hold both control pins high (short brake) for 3 msec to provide slow decay (may be unnecessary)

AUTOSTART RESET(stby) RESET(pwm) DONE // H-bridges off at start

#define PULSE 40  // Set the duration of the pulse (ms) 40 msec needed with 14v / 9400µF CDU and Kato double crossover for reliable throw
#define decay 3   // Set the duration of the brake (ms)

#define SINGLE_COIL_TURNOUT(t, in2, in1, sig, desc) \
PIN_TURNOUT(t, 0, desc) \
AUTOSTART \
  RESET(in1) RESET(in2) \
  IFTHROWN(t) RED(sig) ELSE GREEN(sig) ENDIF \
DONE \
ONCLOSE(t) \
	RESERVE(trnblock) SET(stby) SET(in1) RESET(in2) SET(pwm) DELAY(PULSE) SET(in2) DELAY(decay) RESET(pwm) RESET(in1) RESET(in2) RESET(stby) FREE(trnblock)  \
  GREEN(sig) \
DONE \
ONTHROW(t) \
	RESERVE(trnblock) SET(stby) SET(in2) RESET(in1) SET(pwm) DELAY(PULSE) SET(in1) DELAY(decay) RESET(pwm) RESET(in1) RESET(in2) RESET(stby) FREE(trnblock)   \
  RED(sig) \
DONE

// H-bridge inputs MCP23017 expander pins #1 GPA2-GPB7; #2 GPA0-GPA5: vpins 166 - 185 
// turnout defined as id, h-bridge output pins, indicator signal
SINGLE_COIL_TURNOUT(1, 166, 167, 811, "Crossover")
SINGLE_COIL_TURNOUT(2, 169, 168, 809, "Upper passing")
SINGLE_COIL_TURNOUT(3, 170, 171, 807, "Lower passing")
SINGLE_COIL_TURNOUT(4, 173, 172, 805, "Inner lead")
SINGLE_COIL_TURNOUT(5, 174, 175, 803, "Inner ladder")
SINGLE_COIL_TURNOUT(6, 177, 176, 801, "Prog lead")
SINGLE_COIL_TURNOUT(7, 178, 179, 816, "Prog ladder")
SINGLE_COIL_TURNOUT(8, 181, 180, 812, "Yard lead")
SINGLE_COIL_TURNOUT(9, 183, 182, 842, "Yard ladder 1" )
SINGLE_COIL_TURNOUT(10,184, 185, 814, "Yard ladder 2")

// signals for turnout indicators
SIGNAL(811, 0, 810)  //1 13,12
SIGNAL(809, 0, 808)  //2 11,10
SIGNAL(807, 0, 806)  //3 9,8
SIGNAL(805, 0, 804)  //4 7,6
SIGNAL(803, 0, 802)  //5 5,4
SIGNAL(801, 0, 800)  //6 3,2
SIGNAL(816, 0, 817)  //7 19, 18
SIGNAL(812, 0, 813)  //8 15,14
SIGNAL(842, 0, 844)  //9 48,46
SIGNAL(814, 0, 815)  //10 17,16


//---------- Routes ----------
ALIAS(PASSOUT)
ALIAS(PASSIN)
ALIAS(YD1)
ALIAS(YD2)
ALIAS(YD3)

#define REST 100 // delay between throws to allow more CDU recharge
ROUTE(PASSOUT, "Outer passing siding") THROW(2) DELAY(REST) THROW(3) DONE
ROUTE(PASSIN, "Inner passing siding") CLOSE(2) DELAY(REST) CLOSE(3) DONE
ROUTE(YD1,"Yard track 1") THROW(8) DELAY(REST) CLOSE(9) DONE
ROUTE(YD2,"Yard track 2") THROW(8) DELAY(REST) THROW(9) DELAY(REST) THROW(10) DONE
ROUTE(YD3,"Yard track 3") THROW(8) DELAY(REST) THROW(9) DELAY(REST) CLOSE(10) DONE

#include "autofiles/roster.h"
