#include "TRXVU.h"
#include "AckHandler.h"
#include "FRAM_FlightParameters.h"
#include "SubSystemModules/Communication/SatCommandHandler.h"
#include "SubSystemModules/Housekepping/TelemetryCollector.h"
#include "SysI2CAddr.h"
#include "freertos/projdefs.h"
#include "utils.h"
#include <hal/boolean.h>
#include <hal/errors.h>
#include <stdint.h>
#include <string.h>

#include <satellite-subsystems/isis_ants.h>
#include <satellite-subsystems/isis_vu_e.h>

static xSemaphoreHandle xIsTransmitting;

void muteTransmission(time_unix mute_time) {
	time_unix unmuteTime;
	logError(Time_getUnixEpoch(&unmuteTime), "muteTransmission");

	unmuteTime += mute_time;
	logError(FRAM_WRITE_FIELD(&unmuteTime, trxMuteTime), "muteTransmission");
}

void unmuteTransmission() {
	time_unix currentTime;
	logError(Time_getUnixEpoch(&currentTime), "muteTransmission");

	logError(FRAM_WRITE_FIELD(&currentTime, trxMuteTime), "muteTransmission");
}

void getMuteTime(time_unix *mute_time) {
	logError(FRAM_READ_FIELD(&mute_time, trxMuteTime), "getMuteTime");
}

void muteTRXVU() {
	Boolean mute = TRUE;
	logError(FRAM_WRITE_FIELD(&mute, trxMute), "unmuteTRXVU");
}

void unmuteTRXVU() {
	Boolean mute = FALSE;
	logError(FRAM_WRITE_FIELD(&mute, trxMute), "unmuteTRXVU");
}

int isMuted(Boolean *isMuted) {
	return FRAM_READ_FIELD(isMuted, trxMute);
}

Boolean IsTransmitting() {
	if (pdTRUE == xSemaphoreTake(xIsTransmitting, 1)) {
		xSemaphoreGive(xIsTransmitting);
		return FALSE;
	}
	return TRUE;
}

Boolean checkTransmissionAllowed() {
	if (IsTransmitting()) {
		logError(-1, "SempahoreLocked");
		return FALSE;
	}
	Boolean mute;
	logError(isMuted(&mute), "checkTransmissionAllowed");
	if (mute) {
		printf("i am muted\n");
		return FALSE;
	}

	time_unix curTime;
	time_unix muteTime;

	logError(Time_getUnixEpoch(&curTime), "checkTransmissionAllowed");
	getMuteTime(&muteTime);

	if (curTime > muteTime || muteTime == 0) {
		return TRUE;
	} else {
		return FALSE;
	}
}

int InitTrxvu() {
	ISIS_VU_E_t TRXVUAddress;

	TRXVUAddress.rxAddr = I2C_TRXVU_RC_ADDR;
	TRXVUAddress.txAddr = I2C_TRXVU_TC_ADDR;
	TRXVUAddress.maxSendBufferLength = SIZE_TXFRAME;
	TRXVUAddress.maxReceiveBufferLength = SIZE_RXFRAME;

	PROPEGATE_ERROR(ISIS_VU_E_Init(&TRXVUAddress, 1), "ISISTrxInit");

	PROPEGATE_ERROR(isis_vu_e__set_bitrate(0, isis_vu_e__bitrate__9600bps),
			"setBitRate");

	ISIS_ANTS_t antsAdress[] = { { .i2cAddr = ANTS_I2C_SIDE_A_ADDR }, {
			.i2cAddr = ANTS_I2C_SIDE_B_ADDR } };
	ISIS_ANTS_Init(antsAdress, 2);
	xIsTransmitting = xSemaphoreCreateMutex();

	uint16_t rssi;
	PROPEGATE_ERROR(FRAM_READ_FIELD(&rssi, RSSI), "FramReadField");
	PROPEGATE_ERROR(isis_vu_e__set_tx_thr_rssi(0, rssi), "setRssi");

	isis_vu_e__set_bitrate(0, isis_vu_e__bitrate__9600bps);

	isis_vu_e__set_tx_freq(0, 436400);
	isis_vu_e__set_rx_freq(0, 145970);
	isis_vu_e__set_tx_pll_powerout(0, 0xCFEF);
	return 0;
}

int TransmitDataAsSPL_Packet(sat_packet_t *cmd, void *data,
		unsigned short length) {
	memcpy(cmd->data, data, length);
	cmd->length = length;
	return TransmitSplPacket(cmd, NULL);
}

int TransmitSplPacket(sat_packet_t *packet, unsigned char *_) {
	if (!checkTransmissionAllowed()) {
		return 0;
	}
	if (xSemaphoreTake(xIsTransmitting, SECONDS_TO_TICKS(1)) != pdTRUE)
		return E_GET_SEMAPHORE_FAILED;

	// the total size of the packet is 8 + the length of the SPL data
	size_t length = 8 + packet->length;
	unsigned char avalFrames = 0;
	int err = isis_vu_e__send_frame(ISIS_TRXVU_I2C_BUS_INDEX,
			(unsigned char *) packet, length, &avalFrames);
	xSemaphoreGive(xIsTransmitting);
	logError(err, "TransmitSplPacket");
	return err;
}

int sendBeacon() {
	WOD_Telemetry_t wod;
	GetCurrentWODTelemetry(&wod);
	sat_packet_t cmd;
	PROPEGATE_ERROR(
			AssembleCommand(&wod, sizeof(WOD_Telemetry_t), trxvu_cmd_type, TRANSMIT_BEACON, 0, &cmd),
			"AssembleCommand");
	PROPEGATE_ERROR(TransmitSplPacket(&cmd, NULL), "TransmitSplPacket");
	return 0;
}

int BeaconLogic() {
	time_unix curTime;
	Time_getUnixEpoch(&curTime);

	time_unix beaconSendTime;
	FRAM_READ_FIELD(&beaconSendTime, beaconSendTime);

	if (curTime > beaconSendTime) {
		int err = sendBeacon();
		if (err != E_NO_SS_ERR) {
			return err;
		}
		time_unix beaconInterval;
		FRAM_READ_FIELD(&beaconInterval, beaconInterval);

		curTime += beaconInterval;
		FRAM_WRITE_FIELD(&curTime, beaconSendTime);
	}
	return 0;
}

void changeBeaconTime(time_unix time) {
	FRAM_WRITE_FIELD(&time, beaconInterval);
}

int GetOnlineCommand(sat_packet_t *cmd) {
	isis_vu_e__get_frame__from_t RxFrame;
	unsigned char buffer[SIZE_RXFRAME] = { 0 };
	RxFrame.data = buffer;
	PROPEGATE_ERROR(
			isis_vu_e__get_frame(ISIS_TRXVU_I2C_BUS_INDEX, &RxFrame),
			"GetOnlineCommand");
	PROPEGATE_ERROR(isis_vu_e__remove_frame(ISIS_TRXVU_I2C_BUS_INDEX),"RemoveFrame");

	return ParseDataToCommand(buffer, cmd);
}

int TRX_Logic() {
	int frame_count = GetNumberOfFramesInBuffer();

	if (frame_count > 0) {
		sat_packet_t cmd;
		PROPEGATE_ERROR(GetOnlineCommand(&cmd), "TRX_Logic");

		if ((cmd.ID >> 24) == 15) {
			SendAckPacket(ACK_RECEIVE_COMM, cmd.ID, NULL, 0);
			ActUponCommand(&cmd);
		}
	}

	BeaconLogic();
	return 0;
}

int GetNumberOfFramesInBuffer() {
	unsigned short frame_count = 0;
	PROPEGATE_ERROR(
			isis_vu_e__get_frame_count(ISIS_TRXVU_I2C_BUS_INDEX, &frame_count),
			"GetNumberOfFramesInBuffer");
	return frame_count;
}
