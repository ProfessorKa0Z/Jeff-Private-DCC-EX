// setup for sensorCAM on an ESP32-CAM NANO NUMDigitalPins <= 80
// assume 700 is first vpin (set with ...CREATE(700,80,0x11)
//
//         id vPin	      	 I/O  bsNo. pin   

SETUP("<Z 100 700 0>");     // set up for control OUTPUT on vpin #00
// start of up to 80 sensors numbered bsNo's 100 to 197 (OCT) (0/0 to 9/7)

SETUP("<S 100 700 0>");     // first sensor (S00) (reference) 
SETUP("<S 101 701 0>");    
SETUP("<S 102 702 0>"); 
SETUP("<S 103 703 0>"); 
SETUP("<S 104 704 0>"); 
SETUP("<S 105 705 0>"); 
SETUP("<S 107 707 0>");  // S06 resrved
SETUP("<S 110 708 0>");     
SETUP("<S 111 709 0>");     // Note: suggested id is (OCT) b/s format; vpin is DEC.
SETUP("<S 112 710 0>");     // myFilter.cpp REQUIRES this format for bsNo to vPin conversion
SETUP("<S 113 711 0>");
SETUP("<S 114 712 0>");

/*
SETUP("<S 113 711 0>");
SETUP("<S 114 712 0>");
//etc.                   
//SETUP("<S 120 716 0>");
//SETUP("<S 121 717 0>");
//SETUP("<S 122 718 0>");
//SETUP("<S 123 719 0>");
//
//SETUP("<S 196 778 0>");
//SETUP("<S 197 779 0>");
*/
