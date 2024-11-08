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

void muteTransmission(time_unix mute_time){
	time_unix unmuteTime;
	logError(Time_getUnixEpoch(&unmuteTime), "muteTransmission");

	unmuteTime += mute_time;
	logError(FRAM_WRITE_FIELD(&unmuteTime, trxMuteTime), "muteTransmission");
}

void getMuteTime(time_unix* mute_time){
	logError(FRAM_READ_FIELD(&mute_time, trxMuteTime), "getMuteTime");
}

void muteTRXVU(){
	Boolean mute = TRUE;
	logError(FRAM_WRITE_FIELD(&mute, trxMute), "unmuteTRXVU");
}

void unmuteTRXVU(){
	Boolean mute = FALSE;
	logError(FRAM_WRITE_FIELD(&mute, trxMute), "unmuteTRXVU");
}

int isMuted(Boolean* isMuted){
	return FRAM_READ_FIELD(isMuted, trxMute);
}

Boolean checkTransmissionAllowed(){
	Boolean mute;
	logError(isMuted(&mute), "checkTransmissionAllowed");
	if (mute){
		return FALSE;
	}
	time_unix curTime;
	time_unix muteTime;

	logError(Time_getUnixEpoch(&curTime), "checkTransmissionAllowed");
	getMuteTime(&muteTime);


	if (curTime > muteTime){
		return TRUE;
	}else{
		return FALSE;
	}

}

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


	return rv;
}

int TransmitSplPacket(sat_packet_t *packet, unsigned char *avalFrames){
	if (!checkTransmissionAllowed()){
		return 0;
	}
	//the total size of the packet is 8 + the length of the SPL data
	unsigned char length = 8 + packet->length;
	int err = IsisTrxvu_tcSendAX25DefClSign(ISIS_TRXVU_I2C_BUS_INDEX, (unsigned char*)packet, length, avalFrames);
	logError(err, "TransmitSplPacket");
	return err;
}

int BeaconLogic(){
	time_unix curTime;
	Time_getUnixEpoch(&curTime);

	time_unix beaconSendTime;
	FRAM_READ_FIELD(&beaconSendTime, beaconSendTime);
	if(curTime > beaconSendTime){
		WOD_Telemetry_t wod;
		GetCurrentWODTelemetry(&wod);
		sat_packet_t cmd;
		AssembleCommand( &wod,  sizeof(WOD_Telemetry_t),  0,  0, 0, &cmd);
		TransmitSplPacket( &cmd, NULL);

		time_unix beaconInterval;
		FRAM_READ_FIELD(&beaconInterval, beaconInterval);

		beaconSendTime += beaconInterval;
		FRAM_WRITE_FIELD(&beaconSendTime, beaconSendTime);


	}
	return 0;
}

void changeBeaconTime(time_unix time){
	FRAM_READ_FIELD(&time, beaconInterval);
}


int GetOnlineCommand(sat_packet_t *cmd){
	ISIStrxvuRxFrame RxFrame;
    unsigned char buffer [SIZE_RXFRAME] = {0};
	RxFrame.rx_framedata = buffer;
	PROPEGATE_ERROR(IsisTrxvu_rcGetCommandFrame(ISIS_TRXVU_I2C_BUS_INDEX, &RxFrame), "GetOnlineCommand");

  return ParseDataToCommand(buffer, cmd);
}

int TRX_Logic(){

	int frame_count = GetNumberOfFramesInBuffer();
	if (frame_count > 0) {
		sat_packet_t cmd;
		PROPEGATE_ERROR(GetOnlineCommand(&cmd), "TRX_Logic");

		SendAckPacket(ACK_RECEIVE_COMM, &cmd, NULL, 0);
		ActUponCommand(&cmd);
	}
	 BeaconLogic(FALSE);
	return 0;
}

int GetNumberOfFramesInBuffer(){
	unsigned short frame_count = 0;
	int error = IsisTrxvu_rcGetFrameCount(ISIS_TRXVU_I2C_BUS_INDEX, &frame_count);
	logError(error , "GetNumberOfFramesInBuffer");
	return frame_count;
}
