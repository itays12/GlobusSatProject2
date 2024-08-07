#include <satellite-subsystems/IsisTRXUV.h>
#include <satellite-subsystems/IsisAntS.h>
#include <hal/errors.h>
#include <hal/boolean.h>
#include "TRXVU.h"
#include "SubSystemModules/Communication/SatCommandHandler.h"
#include "SubSystemModules/Housekepping/TelemetryCollector.h"
#include "SubSystemModules/Maintenance/Maintenance.h"
#include "SysI2CAddr.h"
#include "satellite-subsystems/IsisTRXVU.h"
#include "utils.h"
#include "FRAM_FlightParameters.h"
#include "AckHandler.h"
#include <string.h>

time_unix prev_time;
int InitTrxvu()
{
	ISIStrxvuI2CAddress TRXVUAddress;
	ISIStrxvuFrameLengths TRXVUBuffer;
	ISIStrxvuBitrate TRXVUBitrate;
  int rv;

  TRXVUAddress.addressVu_rc = I2C_TRXVU_RC_ADDR;
	TRXVUAddress.addressVu_tc = I2C_TRXVU_TC_ADDR;

	TRXVUBuffer.maxAX25frameLengthTX = SIZE_TXFRAME;
	TRXVUBuffer.maxAX25frameLengthRX = SIZE_RXFRAME;

	TRXVUBitrate = trxvu_bitrate_9600;


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

	Time_getUnixEpoch(&prev_time);

	return rv;
}

int TransmitSplPacket(sat_packet_t *packet, unsigned char *avalFrames){
	//the total size of the packet is 8 + the length of the SPL data
	unsigned char length = 8 + packet->length;
	int err = IsisTrxvu_tcSendAX25DefClSign(ISIS_TRXVU_I2C_BUS_INDEX, (unsigned char*)packet, length, avalFrames);
	logError(err, "failed to initilze trxvu, IsisTrxvu_initialize returned error");
	return err;
}

int BeaconLogic(Boolean forceTX){
	time_unix beacon_interval = 10;

	if( CheckExecutionTime( prev_time, beacon_interval)){
		printf("sending beacon %u\n\r", prev_time);
		WOD_Telemetry_t wod;
		GetCurrentWODTelemetry(&wod);
		sat_packet_t cmd;
		AssembleCommand( &wod,  sizeof(WOD_Telemetry_t),  0,  0, 0, &cmd);
		TransmitSplPacket( &cmd, NULL);
		Time_getUnixEpoch(&prev_time);
	}
	return 0;
}


int GetOnlineCommand(sat_packet_t *cmd){
	ISIStrxvuRxFrame RxFrame;
    unsigned char buffer [SIZE_RXFRAME] = {0};
	RxFrame.rx_framedata = buffer;
	int err = IsisTrxvu_rcGetCommandFrame(ISIS_TRXVU_I2C_BUS_INDEX, &RxFrame);
	if (logError(err, "Error in Get Online Command, could not get command") != E_NO_SS_ERR){
		return err;
	}

  return ParseDataToCommand(buffer, cmd);
}

ISIStrxvuIdleState idle_state = trxvu_idle_state_off;
time_t last_idle_time;
time_t idle_duration;

int TRX_Logic(){
	if (idle_state != trxvu_idle_state_off && CheckExecutionTime(last_idle_time, idle_duration)){
		IsisTrxvu_tcSetIdlestate(ISIS_TRXVU_I2C_BUS_INDEX, trxvu_idle_state_off);
		idle_state = trxvu_idle_state_off;
	}

	int frame_count = GetNumberOfFramesInBuffer();
	if (frame_count > 0) {
		sat_packet_t cmd;
		int err = GetOnlineCommand(&cmd);
		if (logError(err, "Error in trx logic, could not get command") != E_NO_SS_ERR){
			return err;
		}
		SendAckPacket(ACK_RECEIVE_COMM, &cmd, NULL, 0);
		ActUponCommand(&cmd);
	}
	 BeaconLogic(FALSE);
	return 0;
}

int GetNumberOfFramesInBuffer(){
	unsigned short frame_count = 0;
	int error = IsisTrxvu_rcGetFrameCount(ISIS_TRXVU_I2C_BUS_INDEX, &frame_count);
	logError(error , "error in get frame count");
	return frame_count;
}



int SetIdle(time_t duration){
	idle_state = trxvu_idle_state_on;
	idle_duration = duration;

	Time_getUnixEpoch(&last_idle_time);
	return IsisTrxvu_tcSetIdlestate(ISIS_TRXVU_I2C_BUS_INDEX, trxvu_idle_state_on);
}
