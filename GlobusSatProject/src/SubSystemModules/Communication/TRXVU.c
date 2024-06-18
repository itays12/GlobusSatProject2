#include <satellite-subsystems/IsisTRXUV.h>
#include <hal/errors.h>
#include <hal/boolean.h>
#include "TRXVU.h"

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
		TRACE_ERROR("\n\r IsisTrxvu_initialize() failed; err=%d! Exiting ... \n\r", rv);
		return rv;
	}


	return rv;
}
////////////////////new code
//////////Approved by Adi
int TRX_Logic(){
	int frame_count=GetNumberOfFramesInBuffer();
	if (frame_count>0){

	}
	return 1;
}
int GetNumberOfFramesInBuffer(){
	unsigned short frame_count = 0;
	int error =IsisTrxvu_rcGetFrameCount(0, &frame_count);
	logError(error , "error in get frame count");
	return frame_count;
}
//////////
/////////code in process
int ActUponCommand(sat_packet_t *cmd){

}
int BeaconLogic(Boolean forceTX){
	time_unix period = 60;//An educated guess, slide 7

	if( CheckExecutionTime( g_beacon_interval_time, period)){
		GetCurrentWODTelemetry(WOD_Telemetry_t *wod);
	}

}
//////////Approved by Uri
Boolean CheckExecutionTime(time_unix prev_time, time_unix period){
	Time cur_t_Time;
	cur_t_Time =Time_get( &cur_t_Time);

	logError(cur_t_Time,"error in CheckExecutionTime Time_get");

	time_unix cur_t_time_unix = Time_convertTimeToEpoch(&cur_t_time_unix);

	if(prev_time+period>=cur_t_time_unix)
		return TRUE;
	else return FALSE;
}
/////
void GetCurrentWODTelemetry(WOD_Telemetry_t *wod){


}
int AssembleCommand(unsigned char *data, unsigned short data_length, char type, char subtype,unsigned int id, sat_packet_t *cmd);
{

}
//////
