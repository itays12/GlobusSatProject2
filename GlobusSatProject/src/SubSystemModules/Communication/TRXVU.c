#include <satellite-subsystems/IsisTRXUV.h>
#include <satellite-subsystems/IsisAntS.h>
#include <hal/errors.h>
#include <hal/boolean.h>
#include "TRXVU.h"
#include "SubSystemModules/Communication/SatCommandHandler.h"
#include "SubSystemModules/Housekepping/TelemetryCollector.h"
#include "SysI2CAddr.h"
#include "freertos/projdefs.h"
#include "satellite-subsystems/IsisTRXVU.h"
#include "utils.h"
#include "FRAM_FlightParameters.h"
#include "AckHandler.h"
#include <string.h>

static xSemaphoreHandle xIsTransmitting;

void muteTransmission(time_unix mute_time){
	time_unix unmuteTime;
	logError(Time_getUnixEpoch(&unmuteTime), "muteTransmission");

	unmuteTime += mute_time;
	logError(FRAM_WRITE_FIELD(&unmuteTime, trxMuteTime), "muteTransmission");
}

void unmuteTransmission(){
	time_unix currentTime;
	logError(Time_getUnixEpoch(&currentTime), "muteTransmission");

	logError(FRAM_WRITE_FIELD(&currentTime, trxMuteTime), "muteTransmission");
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

Boolean IsTransmitting() {
	if(pdTRUE == xSemaphoreTake(xIsTransmitting,0)){
		xSemaphoreGive(xIsTransmitting);
		return FALSE;
	}
	return TRUE;
}

Boolean checkTransmissionAllowed(){
  if (IsTransmitting()){
    logError(-1, "SempahoreLocked");
    return FALSE;
  }
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
		logError(rv, "IsisTrxvu_initialize");
		return rv;
	}
	ISISantsI2Caddress antsAdress;
	antsAdress.addressSideA = ANTS_I2C_SIDE_A_ADDR;
	antsAdress.addressSideB = ANTS_I2C_SIDE_B_ADDR;
	IsisAntS_initialize(&antsAdress, 1);
	xIsTransmitting = xSemaphoreCreateMutex();



	return rv;
}


int TransmitDataAsSPL_Packet(sat_packet_t *cmd, void* data, unsigned short length){
  memcpy(cmd->data, data, length);
  cmd->length = length;
  return TransmitSplPacket(cmd, NULL);
}

int TransmitSplPacket(sat_packet_t *packet, unsigned char *avalFrames){
	if (!checkTransmissionAllowed()){
		return 0;
	}
	if (xSemaphoreTake(xIsTransmitting,SECONDS_TO_TICKS(1)) != pdTRUE)
		return E_GET_SEMAPHORE_FAILED;



	//the total size of the packet is 8 + the length of the SPL data
	unsigned char length = 8 + packet->length;
	int err = IsisTrxvu_tcSendAX25DefClSign(ISIS_TRXVU_I2C_BUS_INDEX, (unsigned char*)packet, length, avalFrames);
  xSemaphoreGive(xIsTransmitting);
	logError(err, "TransmitSplPacket");
	return err;
}

int sendBeacon(){
	WOD_Telemetry_t wod;
	sat_packet_t cmd;
	PROPEGATE_ERROR(AssembleCommand( &wod,  sizeof(WOD_Telemetry_t),  0,  0, 0, &cmd), "AssembleCommand");
  PROPEGATE_ERROR(TransmitSplPacket( &cmd, NULL), "TransmitSplPacket");
  return 0;
}

int BeaconLogic(){
	time_unix curTime;
	Time_getUnixEpoch(&curTime);

	time_unix beaconSendTime;
	FRAM_READ_FIELD(&beaconSendTime, beaconSendTime);
	if(curTime > beaconSendTime){
    int err = sendBeacon();
    if (err != E_NO_SS_ERR){
      return err;
    }
		time_unix beaconInterval;
		FRAM_READ_FIELD(&beaconInterval, beaconInterval);

		beaconSendTime += beaconInterval;
		FRAM_WRITE_FIELD(&beaconSendTime, beaconSendTime);


	}
	return 0;
}



void changeBeaconTime(time_unix time){
	FRAM_WRITE_FIELD(&time, beaconInterval);
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

		SendAckPacket(ACK_RECEIVE_COMM, cmd.ID, NULL, 0);
		ActUponCommand(&cmd);
	}
	 BeaconLogic();
	return 0;
}

int GetNumberOfFramesInBuffer(){
	unsigned short frame_count = 0;
	PROPEGATE_ERROR(IsisTrxvu_rcGetFrameCount(ISIS_TRXVU_I2C_BUS_INDEX, &frame_count) , "GetNumberOfFramesInBuffer");
	return frame_count;
}
