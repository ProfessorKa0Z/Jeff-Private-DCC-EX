//---- Entry points (counterclockwise)

ALIAS(ccauto4)
AUTOMATION(ccauto4,"CC: outer pass")
	PRINT("Counter-clockwise automation starting (4)")
  IFRESERVE(4)
  ELSE
  	PRINT("Block 4 not free")
 	FOLLOW(abortseq)
  ENDIF
	CALL(locostartups)
	CALL(drive)
	AT(sensor04)
FOLLOW(401) // block 4 chooser

ALIAS(ccauto5)
AUTOMATION(ccauto5,"CC: inner pass")
	PRINT("Counter-clockwise automation starting (5)")
	IFRESERVE(5)
 	ELSE
 		PRINT("Block 5 not free")
 		FOLLOW(abortseq)
  	ENDIF
	CALL(locostartups)
	CALL(drive)
	AT(sensor05)
FOLLOW(501)  // block 5 chooser

ALIAS(ccautooy)
AUTOMATION(ccautooy,"CC: outer yard")
	PRINT("Counter-clockwise automation starting (Outer yard)")
	CALL(locostartups)
	RESERVE(7)
	THROW(8)
	CALL(drive)
	START(791) // closer for yard
FOLLOW(701)  // chooser for block 7

SEQUENCE(791)
	AFTER(sensor11)  // train from outer yard completely on the main now
	CLOSE(8)
DONE


//---- Block logic (counterclockwise)

SEQUENCE(401)  // choose an exit from block 4; trigger on sensor04
	AT(sensor04)
	IFRESERVE(7)
		PRINT("401 choosing 7")
		FOLLOW(471)
	ENDIF
	IFRESERVE(8)
		PRINT("401 choosing 8")
		FOLLOW(481)
	ENDIF
	SPEED(1)  //  no free path found; halt and keep trying until one opens
	DELAY(1000)
FOLLOW(401) 

SEQUENCE(471)
	RESERVE(1)  // need top path
	PRINT("471 proceeding top crossover")
	THROW(2)
	DELAY(HPULSE)
	CLOSE(1)
  	DELAY(HPULSE)
 	CLOSE(8) // in case outer yard is thrown
	CALL(drive)
	START(497) // block 4-7 clearer
	AT(sensor11)
FOLLOW(701) // 7 chooser

SEQUENCE(497)
	AFTER(sensor04)
	FREE(4)
	PRINT("4-7 clearer freeing 4")
	AFTER(sensor01)
	FREE(1)
	PRINT("4-7 clearer freeing 1")
DONE

SEQUENCE(481)
	RESERVE(1) // cross crossover, need both upper and lower
	RESERVE(2)
	PRINT("481 proceeding cross crossover")
	THROW(2)
	DELAY(HPULSE)
	THROW(1)
	CALL(drive)
	START(498)  // block 4-8 clearer
	AT(sensor13)
FOLLOW(861)

SEQUENCE(498)
	AFTER(sensor04)
	FREE(4)
	PRINT("4-8 clearer freeing 4")
	AFTER(sensor02)
	FREE(1)
	FREE(2)
	PRINT("4-8 clearer freeing 1 2")
DONE

SEQUENCE(501)  
	AT(sensor05)
	IFRESERVE(7)
		PRINT("501 choosing 7")
		FOLLOW(571)
	ENDIF
	IFRESERVE(8)
		PRINT("501 choosing 8")
		FOLLOW(581)
	ENDIF
	SPEED(1)  // no available path; keep trying
	DELAY(1000) 
FOLLOW(501) 

SEQUENCE(571)
	RESERVE(1)  // need top path
	PRINT("571 proceeding top crossover")
	CLOSE(2)
	DELAY(HPULSE)
	CLOSE(1)
 	DELAY(HPULSE)
 	CLOSE(8) // in case outer yard is thrown
	CALL(drive)
	START(597) // block 5-7 clearer
	AT(sensor11)
FOLLOW(701) // 7 chooser

SEQUENCE(597)
	AFTER(sensor05)
	FREE(5)
	PRINT("5-7 clearer freeing 5")
	AFTER(sensor01)
	FREE(1)
	PRINT("5-7 clearer freeing 1")
DONE

SEQUENCE(581)
	RESERVE(1) // cross crossover, need both upper and lower
	RESERVE(2)
	PRINT("581 proceeding cross crossover")
  	CLOSE(2)
	DELAY(HPULSE)
	THROW(1)
	CALL(drive)
	START(598)  // block 5-8 clearer
	AT(sensor13)
FOLLOW(861)

SEQUENCE(598)
	AFTER(sensor05)
	FREE(5)
	PRINT("5-8 clearer freeing 5")
	AFTER(sensor02)
	FREE(1)
	FREE(2)
	PRINT("5-8 clearer freeing 1 2")
DONE

SEQUENCE(601)  
	AT(sensor03)
	IFRESERVE(7)
		PRINT("601 choosing 7")
		FOLLOW(671)
	ENDIF
	IFRESERVE(8)
		PRINT("601 choosing 8")
		FOLLOW(681)
	ENDIF
	SPEED(1)
	DELAY(1000)
FOLLOW(601) 

SEQUENCE(671)
  	RESERVE(1) // cross crossover, need both upper and lower
 	RESERVE(2)
  	PRINT("671 proceeding cross crossover")
  	THROW(1)
  	DELAY(HPULSE)
  	CLOSE(8)  // yard
	CALL(drive)
	START(697) // block 6-7 clearer
	AT(sensor11)
FOLLOW(701) // 7 chooser

SEQUENCE(697)
	AFTER(sensor03)
	FREE(6)
	PRINT("6-7 clearer freeing 6")
	AFTER(sensor01)
	FREE(1)
	FREE(2)
	PRINT("6-7 clearer freeing 1 2")
DONE

SEQUENCE(681)
	RESERVE(2)  // need bottom path
	PRINT("681 proceeding bottom crossover")
	CLOSE(1)	
	CALL(drive)
	START(698)  // block 6-8 clearer
	AT(sensor13)
FOLLOW(861)

SEQUENCE(698)
	AFTER(sensor03)
	FREE(6)
	PRINT("6-8 clearer freeing 6")
	AFTER(sensor02)
	FREE(2)
	PRINT("6-8 clearer freeing 2")
DONE

SEQUENCE(701)  
	AT(sensor11)
	IFRESERVE(4)
		PRINT("701 choosing 4")
		FOLLOW(741)
	ENDIF
	IFRESERVE(5)
		PRINT("701 choosing 5")
		FOLLOW(751)
	ENDIF
	SPEED(1)
	DELAY(1000)
FOLLOW(701) 

SEQUENCE(741)  // 4 is already reserved by chooser
	THROW(3)
	CALL(drive)
  	PRINT("741 starting")
	START(794) // block 7-4 clearer
FOLLOW(401) // 4 cc chooser

SEQUENCE(794)
	AFTER(sensor10) // for 74 and 75 must wait until turnout clear, since CL train on other track may be waiting for turnout
	FREE(7)
	PRINT("7-4 clearer freeing 7")
DONE

SEQUENCE(751)  // 5 is already reserved by chooser
	PRINT("751 starting")
	CLOSE(3)
	CALL(drive)
	START(795) // block 7-5 clearer
FOLLOW(501) // 5 cc chooser

SEQUENCE(795)
	AFTER(sensor07)
	FREE(7)
	PRINT("7-5 clearer freeing 7")
DONE

SEQUENCE(861)  // includes attempt to resolve stalemate by heading onto programming track
	PRINT("861 starting")
	IFRESERVE(6)
	ELSE  // 6 was not free
		IF(cl6flag) // they're heading right for us!
			FOLLOW(progescape)
		ELSE	
			RESERVE(6) // 6 not free, but occupying train is heading away from us so wait
		ENDIF
	ENDIF
	RESET(cl6flag)  // we've reserved 6 at this point so clear clockwise flag
	CLOSE(6) // in case prog lead thrown
	CALL(drive)
	START(896)  // block 8 clearer
FOLLOW(601)  //6 cc chooser

SEQUENCE(896)
	AFTER(sensor12)
	FREE(8)
	PRINT("8-6 clearer freeing 8")
DONE

ALIAS(progescape)
SEQUENCE(progescape)
	PRINT("Divert to prog track")
  	THROW(prog_t) // join programming track
	THROW(6)
	CALL(drive) // advance onto prog track
	AFTER(sensor14) // off the main *** train must be shorter than sensor14 -> end of programming track
	SPEED(1)
	CLOSE(6)
	PRINT("Divert off main, freeing 8")
	FREE(8)
	AT(sensor13)  // clockwise train must have reserved 8 at his point
	PRINT("Divert starting backup")
	RESERVE(8)
	THROW(6)
	REV(trainspeed)
	AFTER(sensor13) // backed up onto main *** train must be short enough not to back into crossover
	SPEED(1)
	CLOSE(6)
	CLOSE(prog_t)
	PRINT("Divert finished backup")
	CALL(drive)
	AT(sensor13)  // see if 6 is free now...
FOLLOW(861)
