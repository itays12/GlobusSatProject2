#include <satellite-subsystems/IsisTRXUV.h>
#include <satellite-subsystems/IsisAntS.h>
#include <hal/errors.h>
#include <hal/boolean.h>
#include "TRXVU.h"
#include "SubSystemModules/Housekepping/TelemetryCollector.h"
#include "SubSystemModules/Maintenance/Maintenance.h"
#include "SysI2CAddr.h"
#include "utils.h"
#include "FRAM_FlightParameters.h"
#include "AckHandler.h"

//****not sure if it works, will have to check at testing****
int TRXVUInit()
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

int TransmitSplPacket(sat_packet_t *packet, unsigned char *avalFrames){
	//the total size of the packet is 8 + the length of the SPL data
	unsigned char length = 8 + packet->length;
	int err = IsisTrxvu_tcSendAX25DefClSign(ISIS_TRXVU_I2C_BUS_INDEX, (unsigned char*)packet, length, avalFrames);
	logError(err, "failed to initilze trxvu, IsisTrxvu_initialize returned error");
	return err;
}

time_unix prev_time;
int AssembleCommand(unsigned char *data, unsigned short data_length, char type, char subtype,unsigned int id, sat_packet_t *cmd){

	cmd->length =data_length ;
	cmd->cmd_type =type ;
	cmd->cmd_subtype =subtype ;
	cmd->ID =id;
	memcpy(cmd->data, data, data_length);
	return 0;
}
int BeaconLogic(Boolean forceTX){
	time_unix beacon_interval;
	FRAM_read((unsigned char*)&beacon_interval, BEACON_INTERVAL_TIME_ADDR,BEACON_INTERVAL_TIME_SIZE);

	if( CheckExecutionTime( prev_time, beacon_interval)){
		WOD_Telemetry_t wod;
		GetCurrentWODTelemetry(&wod);
		sat_packet_t cmd;
		AssembleCommand( &wod,  sizeof(WOD_Telemetry_t),  0,  0, 0, &cmd);
		TransmitSplPacket( &cmd, NULL);
		Time_getUnixEpoch(&prev_time);

	}
	return 0;


//****Approved by Uri****
int TRX_Logic(){
	int frame_count=GetNumberOfFramesInBuffer();
	for(int i = 0; i < frame_count; i++){
		sat_packet_t cmd;
		int err = GetOnlineCommand(&cmd);
		if (logError(err, "Error in trx logic, could not get command") != E_NO_SS_ERR){
			return err;
		}
		SendAckPacket(ACK_RECEIVE_COMM, &cmd, NULL, 0);
		ActUponCommand(&cmd);
		 BeaconLogic( forceTX);
	}
	return 0;
}

int GetNumberOfFramesInBuffer(){
	unsigned short frame_count = 0;
	int error = IsisTrxvu_rcGetFrameCount(0, &frame_count);
	logError(error , "error in get frame count");
	return frame_count;
}

