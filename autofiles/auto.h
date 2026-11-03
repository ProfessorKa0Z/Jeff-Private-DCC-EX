
//-- Assign SensorCam vpins to sensors
#define sensor01 701
#define sensor02 702
#define sensor03 703
#define sensor04 704
#define sensor05 705
#define sensor07 707 
#define sensor10 708
#define sensor11 709  
#define sensor12 710 
#define sensor13 711
#define sensor14 712

//-- clockwise and counterclockwise block logic
#include "clauto.h"
#include "ccauto.h"

//-- Startup and speed routines
ALIAS(locostartups)
ALIAS(drive)
ALIAS(abortseq)

#define trainspeed 80

SEQUENCE(drive)  // adjust for locomotives where desired speed is not trainspeed
	PRINT("Driving")
	FWD(trainspeed)
	IFLOCO(102)	
		SPEED(90)
		RETURN
	ENDIF
RETURN

SEQUENCE(abortseq)
	PRINT("Aborting")
	ESTOP
	KILLALL
DONE

SEQUENCE(locostartups)
	FON(0)  // headlight on

	IFLOCO(102) 
	//	PRINT("DB BR01 102 starting up")
		FON(1)
		DELAY(5000)
		FON(22)		// departure
		DELAY(5000)
		FON(4)		// conductor whistle
		DELAY(2000)
		RETURN
	  ENDIF
  
	IFLOCO(439) 
		PRINT("DB BR110 439 starting up")
		FON(1)
		DELAY(5000)
		FON(7)		// platform
		DELAY(5000)
		FON(14)		// departure signal
		DELAY(2000)
		RETURN
	ENDIF
  
	IFLOCO(495) 
		PRINT("DB BR18.4 495 starting up")
		FON(2)
		DELAY(5000)
		FON(16)		// departure
		DELAY(5000)
		FON(19)		// conductor whistle
		DELAY(2000)
		RETURN
	ENDIF

	IFLOCO(633)
		PRINT("DB BR442 633 starting up")
		FON(1)
		DELAY(5000)
		FON(9)		// departure
		DELAY(5000)
		FON(10)		// conductor whistle
		DELAY(2000)
		RETURN
	ENDIF

	PRINT("No startup defined for this locomotive")
RETURN
