// Default MCP23017s at addresses 0x20 and 0x21: vpins 164 - 195 control H-bridges for turnouts

HAL(EXIOExpander, 800, 62, 0x65) // EX-IOExpander for signals

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

SIGNAL(843, 0, 845)

