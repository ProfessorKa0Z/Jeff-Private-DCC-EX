

//---- Entry points (clockwise)
ALIAS(clauto4)
AUTOMATION(clauto4,"CL: outer pass")
	PRINT("Clockwise automation starting (4)")
	// start in block 4 and head to block 7
	IFRESERVE(4)
  	ELSE
  		PRINT("Block 4 not free")
 		FOLLOW(abortseq)
  	ENDIF
	CALL(locostartups)
	CALL(drive)
	AT(sensor10)
FOLLOW(47)

ALIAS(clprog)
ROUTE(clprog, "CL: prog track")
  PRINT("Reading programming track")
  READ_LOCO
  IFLOCO(-1)
    PRINT("No locomotive found")
    DONE
  ENDIF
  THROW(prog_t) // join prog track and change LED
  DELAY(1000)
  PRINT("Startup locomotive on programming track")
  CALL(locostartups)
  RESERVE(8)
  PRINT("Driving locomotive on programming track")
  THROW(6)
  CALL(drive)
  START(990) // spawn closer for prog
FOLLOW(800)  // chooser for block 8

SEQUENCE(990)
	AFTER(sensor13)  // train entirely on the main in block 8
	CLOSE(6)
	CLOSE(prog_t) // unjoin
	PRINT("990 closing prog")
DONE

ALIAS(clyard)
AUTOMATION(clyard, "CL: inner yard")
  PRINT("Clockwise automation starting (inner yard)")
  CALL(locostartups)
  RESERVE(6)
  THROW(4)
  CALL(drive)
  START(690) // spawn closer for yard
FOLLOW(800)  // chooser for block 8

SEQUENCE(690)
	AFTER(sensor03)  // train entirely on the main in block 6
	CLOSE(4) // close off yard
DONE
	
//----  Block logic (clockwise)

SEQUENCE(68)
	PRINT("68 starting")
	RESERVE(8)
	CLOSE(6) // in case prog lead thrown
	CALL(drive)
	START(6980)  // block 6 clearer
FOLLOW(800)

SEQUENCE(6980)
	AFTER(sensor12)  // should be ok since we have block 8
	FREE(6)
	PRINT("6-8 clearer freeing 6")
	// note that cl6flag is still set, but will be cleared by next cc reserve
DONE
	
SEQUENCE(800)  // choose an exit from block 8; trigger on sensor02
	AT(sensor02)
	IFRESERVE(4)
		PRINT("800 choosing 4")
		FOLLOW(84)
	ENDIF
	IFRESERVE(5)
		PRINT("800 choosing 5")
		FOLLOW(85)
	ENDIF
	IFRESERVE(6)
		SET(cl6flag) // if stalemate cc train will divert to programming track
		PRINT("800 choosing 6")
		FOLLOW(86)
	ENDIF
	STOP  //  no free path found; halt and keep trying until one opens
	DELAY(1000)
FOLLOW(800)  

SEQUENCE(84)
	RESERVE(2) // cross crossover, need both upper and lower
	RESERVE(1)
	PRINT("84 proceeding cross crossover")
	THROW(1)
	DELAY(PULSE)
	THROW(2)
	CALL(drive)
	START(8940)  // block 8-4 clearer
	AT(sensor10)
FOLLOW(47)

SEQUENCE(8940)
	AFTER(sensor02) // clearers revised to clear lagging block once train on crossover
	FREE(8)
	PRINT("8-4 clearer freeing 8")
	AFTER(sensor04)
	FREE(2)
	FREE(1)
	PRINT("8-4 clearer freeing 2 1")
DONE

SEQUENCE(85)  
	RESERVE(2)  // cross-path, need upper and lower
	RESERVE(1)
	PRINT("85 proceeding cross crossover")
	THROW(1)
	DELAY(PULSE)
	CLOSE(2)
	CALL(drive)
	START(8950) // 8-5 clearer
	AT(sensor07)
FOLLOW(57)

SEQUENCE(8950)
	AFTER(sensor02) 
	FREE(8)
	PRINT("8-5 clearer freeing 8")
	AFTER(sensor05)
	FREE(2)
	FREE(1)
	PRINT("8-5 clearer freeing 2 1")
DONE

SEQUENCE(86)
	RESERVE(2) // need lower path free
	PRINT("86 proceeding lower crossover")
	CLOSE(1)
	DELAY(PULSE)
	CLOSE(4) // in case yard lead thrown
	CALL(drive)
	START(8960) //8-6 clearer
	AT(sensor12)
FOLLOW(68)

SEQUENCE(8960)
	AFTER(sensor02) 
	FREE(8)
	PRINT("8-6 clearer freeing 8")
	AFTER(sensor03)
	FREE(2)
	PRINT("8-6 clearer freeing 2")
DONE

SEQUENCE(47)
	RESERVE(7)
	THROW(3)
	DELAY(PULSE)
	CLOSE(8) // in case outer yard lead thrown
	CALL(drive)
	START(4970)
FOLLOW(700)

SEQUENCE(4970)  // clearers revised to not wait until turnout cleared
	AFTER(sensor10)
	FREE(4)
	PRINT("4-7 clearer freeing 4")
DONE

SEQUENCE(57)
	RESERVE(7)
	CLOSE(3)
	DELAY(PULSE)
	CLOSE(8) 
	CALL(drive)
	START(5970)
FOLLOW(700)

SEQUENCE(5970)
	AFTER(sensor07)
	FREE(5)
	PRINT("5-7 clearer freeing 5")
DONE

SEQUENCE(700)  // choose an exit from block 7; trigger on sensor01
	AT(sensor01)
	IFRESERVE(4)
		PRINT("700 choosing 4")
		FOLLOW(74)
	ENDIF
	IFRESERVE(5)
		PRINT("700 choosing 5")
		FOLLOW(75)
	ENDIF
	IFRESERVE(6)
		SET(cl6flag)
		PRINT("700 choosing 6")
		FOLLOW(76)
	ENDIF
	SPEED(1) // no free path found; keep trying until one opens
	DELAY(1000)
FOLLOW(700)  

SEQUENCE(74)
	RESERVE(1)  // need top path
	PRINT("74 proceeding top crossover")
	CLOSE(1)
	DELAY(PULSE)
	THROW(2)
	CALL(drive)
	START(7940)
	AT(sensor10)
FOLLOW(47)

SEQUENCE(7940)
	AFTER(sensor01)
	FREE(7)
	PRINT("7-4 clearer freeing 7")
	AFTER(sensor04)
	FREE(1)
	PRINT("7-4 clearer freeing 1")
DONE

SEQUENCE(75)
	RESERVE(1)  // need top path
	PRINT("75 proceeding top crossover")
	CLOSE(1)
	DELAY(PULSE)
	CLOSE(2)
	CALL(drive)
	START(7950)
	AT(sensor07)
FOLLOW(57)

SEQUENCE(7950)
	AFTER(sensor01)
	FREE(7)
	PRINT("7-5 clearer freeing 7")
	AFTER(sensor05)
	FREE(1)
	PRINT("7-5 clearer freeing 1")
DONE

SEQUENCE(76)
	RESERVE(2)  // need cross path
	RESERVE(1)
	PRINT("76 proceeding cross crossover")
	THROW(1)
	DELAY(PULSE)
	CLOSE(4) // in case yard lead thrown
	CALL(drive)
	START(7960)
	AT(sensor12)
FOLLOW(68)

SEQUENCE(7960)
	AFTER(sensor01)
	FREE(7)
	PRINT("7-6 clearer freeing 7")
	AFTER(sensor03)
	FREE(2)
	FREE(1)
	PRINT("7-6 clearer freeing 1 2")
DONE
