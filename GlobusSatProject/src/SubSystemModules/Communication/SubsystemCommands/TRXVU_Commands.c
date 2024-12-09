#include "TRXVU_Commands.h"
#include "SubSystemModules/Communication/TRXVU.h"
#include "SysI2CAddr.h"
#include "satellite-subsystems/IsisTRXVU.h"

int CMD_MuteTRXVU(sat_packet_t *cmd) {
  MuteTRXVU_Params *params = (MuteTRXVU_Params *)cmd->data;
  if (params->mute_time == 0) {
    muteTRXVU();
  } else {
    muteTransmission(params->mute_time);
  }
  return 0;
}

int CMD_UnMuteTRXVU(sat_packet_t *cmd) {
  unmuteTRXVU();
  unmuteTransmission();
  return 0;
}

int CMD_TrasmitBeacon(sat_packet_t *cmd){
  sendBeacon();
  return 0;
}

int CMD_GetBaudRate(sat_packet_t *cmd){
  ISIStrxvuTransmitterState transmitterState;
  IsisTrxvu_tcGetState(ISIS_TRXVU_I2C_BUS_INDEX, &transmitterState);
  ISIStrxvuBitrateStatus baudRate = transmitterState.fields.transmitter_bitrate; 

  sat_packet_t packet;
  PROPEGATE_ERROR(AssembleCommand(&baudRate, sizeof(ISIStrxvuBitrate), eps_cmd_type,
                                  GET_BAUD_RATE, 0, &packet),
                  "AssembleCommand");
  PROPEGATE_ERROR(TransmitSplPacket(&packet, NULL), "TransmitSplPacket");
  return 0;
}

int CMD_SetBaudRate(sat_packet_t *cmd){
  SetBaudRate_Params* params = (SetBaudRate_Params*)cmd->data;
  ISIStrxvuBitrate bitrate;
  switch(params->bitrate){
    case trxvu_bitratestatus_1200:
      bitrate = trxvu_bitrate_1200;
      break;

    case trxvu_bitratestatus_2400:
      bitrate = trxvu_bitrate_2400;
      break;

    case trxvu_bitratestatus_4800:
      bitrate = trxvu_bitrate_4800;
      break;

    case trxvu_bitratestatus_9600:
      bitrate = trxvu_bitrate_9600;
      break;
  }

  PROPEGATE_ERROR(IsisTrxvu_tcSetAx25Bitrate(ISIS_TRXVU_I2C_BUS_INDEX, bitrate), "SetAx25Bitrate");
  return 0;
}
