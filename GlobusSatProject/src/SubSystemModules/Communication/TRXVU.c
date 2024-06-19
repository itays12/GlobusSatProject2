#include <satellite-subsystems/IsisTRXUV.h>
#include <satellite-subsystems/IsisAntS.h>
#include <hal/errors.h>
#include <hal/boolean.h>
#include "TRXVU.h"
#include "SubSystemModules/Housekepping/TelemetryCollector.h"
#include "CommandDictionary.h"
#include "SPL.h"
#include "SysI2CAddr.h"
#include "utils.h"



//****not sure if it works, will have to check at testing****
int TRXVUInit(void)
{
    // Definition of I2C and TRXUV
	ISIStrxvuI2CAddress TRXVUAddress;
	ISIStrxvuFrameLengths TRXVUBuffer;
	ISIStrxvuBitrate TRXVUBitrate;
    int rv;

	//I2C addresses defined
    TRXVUAddress.addressVu_rc = I2C_TRXVU_RC_ADDR;
	TRXVUAddress.addressVu_tc = I2C_TRXVU_TC_ADDR;

	//Buffer definition
	TRXVUBuffer.maxAX25frameLengthTX = SIZE_TXFRAME;
	TRXVUBuffer.maxAX25frameLengthRX = SIZE_RXFRAME;

	//Bitrate definition
	TRXVUBitrate = trxvu_bitrate_9600;

	//Initialize the trxvu subsystem
	rv = IsisTrxvu_initialize(&TRXVUAddress, &TRXVUBuffer, &TRXVUBitrate, 1);
	if(rv != E_NO_SS_ERR && rv != E_IS_INITIALIZED)
	{
		// we have a problem. Indicate the error. But we'll gracefully exit to the higher menu instead of
		// hanging the code
		logError(rv, "failed to initilze trxvu, IsisTrxvu_initialize returned error");
		return rv;
	}
	ISISantsI2Caddress antsAdress;
	antsAdress.addressSideA = ANTS_I2C_SIDE_A_ADDR;
	antsAdress.addressSideB = ANTS_I2C_SIDE_B_ADDR;
	IsisAntS_initialize(&antsAdress, 1);

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
	int err;
	switch(cmd->cmd_type){
		case trxvu_cmd_type:
			err = trxvu_command_router(cmd);
			break;
		case eps_cmd_type:
			err = eps_command_router(cmd);
			break;
		case telemetry_cmd_type:
			err = telemetry_command_router(cmd);
			break;
		case filesystem_cmd_type:
			err = filesystem_command_router(cmd);
			break;
		case managment_cmd_type:
			err = managment_command_router(cmd);
			break;
		case ack_type:
			//TODO: handle ack packets
			break;
		case dump_type:
			//TODO: handle dump packets
			break;
	}
	if (logError(err, "failed to run command: ") != E_NO_SS_ERR){
		return err;
	}
	return 0;
}
