#include "TrxvuTestingDemo.h"
#include "hal/Utility/util.h"
#include "hal/Timing/Time.h"
#include "SubSystemModules/Maintenance/Maintenance.h"
#include "SubSystemModules/Communication/TRXVU.h"
#include "stdio.h"

Boolean BeaconLogicTest(){
	int WaitTime;
	printf( "\n\r Select seconds to wait for Beacon Logic Test: \n\r");
	while(UTIL_DbguGetIntegerMinMax(& WaitTime, 1, 121) == 0);
	time_unix prev_time;
	Time_getUnixEpoch(&prev_time);

	while( CheckExecutionTime( prev_time,  WaitTime)){
		BeaconLogic(TRUE);
	}
  return TRUE;
}
Boolean  TRX_LogicTest(){
	int WaitTime;
	printf( "\n\r Select seconds to wait for Beacon Logic Test: \n\r");
	while(UTIL_DbguGetIntegerMinMax(& WaitTime, 1, 121) == 0);
	time_unix prev_time;
	Time_getUnixEpoch(&prev_time);

	while( CheckExecutionTime( prev_time,  WaitTime)){
		 TRX_Logic(TRUE);
	}
  return TRUE;
}

Boolean TransmitTest(){
	//Buffers and variables definition
		unsigned char testBuffer1[10]  = { 0 };
		unsigned char txCounter = 0;
		unsigned char avalFrames = 0;
		unsigned int timeoutCounter = 0;

		sat_packet_t packet;
		printf( "\n\r enter ID: \n\r");
			while(UTIL_DbguGetHexa32(&packet.ID) == 0);

		printf( "\n\r enter subtype: \n\r");
			while(UTIL_DbguGetHexa32(&packet.cmd_subtype) == 0);

		printf( "\n\r enter type: \n\r");
			UTIL_DbguGetHexa32(&packet.cmd_type);


	    int i;
	    unsigned int temp;

	    for(i = 0; i < 10; i++){
	    	if(UTIL_DbguGetHexa32(&temp)){
	    		testBuffer1[i] = (unsigned char)(temp);
	    	}
	    	else
	    	{
	    		printf("/r/n Invalid input. Please enter a valid hexadecimal number.\r\n");
	    		i=i-1;
	    	}
	    }

	    TransmitSplPacket( &packet, &avalFrames);

  return TRUE;
}



Boolean MainTrxvuTestBench(){
	int selection = 0;
		Boolean offerMoreTests = TRUE;
		printf( "\n\r Select a test to perform: \n\r");
		printf("\t 1)  Beacon Logic\n\r");
		printf("\t 2) TRX Logic \n\r");
		printf("\t 3) Transmit Test\n\r");


	while(UTIL_DbguGetIntegerMinMax(&selection, 1, 19) == 0){};

		switch(selection) {
		case 1:
			offerMoreTests = BeaconLogicTest();
			break;
		case 2:
			offerMoreTests = TRX_LogicTest();
			break;
		case 3:
			offerMoreTests = TransmitTest();
			break;
		}
}

