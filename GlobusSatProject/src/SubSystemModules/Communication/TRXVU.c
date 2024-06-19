#include <satellite-subsystems/IsisTRXUV.h>
#include <hal/errors.h>
#include <hal/boolean.h>
#include "TRXVU.h"
#include "SubSystemModules/Housekeeping/TelemetryCollector.h"

//****doesnt really work, only initlizes the module****
int TRXVUInit(void)
{
    // Definition of I2C and TRXUV
	ISIStrxvuI2CAddress myTRXVUAddress[1];
	ISIStrxvuFrameLengths myTRXVUBuffers[1];
	ISIStrxvuBitrate myTRXVUBitrates[1];
    int rv;

	//I2C addresses defined
	myTRXVUAddress[0].addressVu_rc = 0x60;
	myTRXVUAddress[0].addressVu_tc = 0x61;

	//Buffer definition
	myTRXVUBuffers[0].maxAX25frameLengthTX = SIZE_TXFRAME;
	myTRXVUBuffers[0].maxAX25frameLengthRX = SIZE_RXFRAME;

	//Bitrate definition
	myTRXVUBitrates[0] = trxvu_bitrate_1200;

	//Initialize the trxvu subsystem
	rv = IsisTrxvu_initialize(myTRXVUAddress, myTRXVUBuffers, myTRXVUBitrates, 1);
	if(rv != E_NO_SS_ERR && rv != E_IS_INITIALIZED)
	{
		// we have a problem. Indicate the error. But we'll gracefully exit to the higher menu instead of
		// hanging the code
		LogError(rv, "failed to initilze trxvu, IsisTrxvu_initialize returned error");
		return rv;
	}


	return rv;
}

//****Approved by Adi****
int TRX_Logic(){
	int frame_count=GetNumberOfFramesInBuffer();
	if (frame_count>0){
		sat_packet_t cmd;
		int err = GetOnlineCommand(&cmd);
	}
	return 1;
}

int GetNumberOfFramesInBuffer(){
	unsigned short frame_count = 0;
	int error =IsisTrxvu_rcGetFrameCount(0, &frame_count);
	logError(error , "error in get frame count");
	return frame_count;
}


//****code in progess, need someone to go over commands****
int ActUponCommand(sat_packet_t *cmd){
	return 0;
}
