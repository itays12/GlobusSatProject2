#include <satellite-subsystems/IsisTRXUV.h>
#include <hal/errors.h>
#include <hal/boolean.h>
#include "TRXVU.h"
#include "SubSystemModules/Housekepping/TelemetryCollector.h"
#include "CommandDictionary.h"
#include "SPL.h"



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

//****Approved by Uri****
int TRX_Logic(){
	int frame_count=GetNumberOfFramesInBuffer();
	for(int i = 0; i < frame_count; i++){
		sat_packet_t cmd;
		int err = GetOnlineCommand(&cmd);
		if (logError(err, "Error in trx logic, could not get command") != E_NO_SS_ERR){
			return err;
		}
		ActUponCommand(&cmd);
	}
	return 0;
}

int GetNumberOfFramesInBuffer(){
	unsigned short frame_count = 0;
	int error =IsisTrxvu_rcGetFrameCount(0, &frame_count);
	logError(error , "error in get frame count");
	return frame_count;
}

//****TODO: handle errors****
int ActUponCommand(sat_packet_t *cmd){
	switch(cmd->cmd_type){
		case trxvu_cmd_type:
			trxvu_command_router(cmd);
			break;
		case eps_cmd_type:
			eps_command_router(cmd);
			break;
		case telemetry_cmd_type:
			telemetry_command_router(cmd);
			break;
		case filesystem_cmd_type:
			filesystem_command_router(cmd);
			break;
		case managment_cmd_type:
			managment_command_router(cmd);
			break;
		case ack_type:
			//TODO: handle ack packets
		case dump_type:
			//TODO: handle dump packets
	}
	return 0;
}
