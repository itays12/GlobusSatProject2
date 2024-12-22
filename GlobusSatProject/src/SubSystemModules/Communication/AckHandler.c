#include <string.h>
#include "AckHandler.h"
#include "TRXVU.h"
#include "hal/errors.h"
#include "utils.h"

int SendAckPacket(ack_subtype_t acksubtype, sat_packet_t *cmd, unsigned char *data, unsigned short length){
	sat_packet_t packet;

	packet.ID = cmd->ID;
	packet.cmd_type = ack_type;
	packet.cmd_subtype = acksubtype;
	//TODO: add assert that length does not exceed MAX_COMMAND_DATA_LENGTH
	packet.length = length;
	if(data != NULL){
		memcpy(packet.data, data, length);
	}

	unsigned char avalFrames = 0;
	int err = logError(TransmitSplPacket(&packet, &avalFrames), "SendAckPacket");
	if(err != E_NO_SS_ERR){
		return err;
	}

	return 0;
}
