#include "SubSystemModules/Communication/SubsystemCommands/Maintanence_Commands.h"
#include "SubSystemModules/Communication/TRXVU.h"
#include "SysI2CAddr.h"
#include "hal/Drivers/I2C.h"
#include "satellite-subsystems/imepsv2_piu.h"
#include "utils.h"

int CMD_GenericI2C(sat_packet_t *cmd) {
  CMD_GenericI2C_Params *params = (CMD_GenericI2C_Params *)cmd->data;
  I2C_write(params->adress, params->buffer, params->size);
  return 0;
}

int CMD_GetSatUptime(sat_packet_t *cmd) {
  unsigned int up_time = Time_getUptimeSeconds();
  PROPEGATE_ERROR(TransmitDataAsSPL_Packet(cmd, &up_time, sizeof(unsigned int)),
                  "TransmitSplPacket");
  return 0;
}

int CMD_GetSatTime(sat_packet_t *cmd) {
  unsigned int cur_time;
  PROPEGATE_ERROR(Time_getUnixEpoch(&cur_time), "GetTime");
  PROPEGATE_ERROR(
      TransmitDataAsSPL_Packet(cmd, &cur_time, sizeof(unsigned int)),
      "TransmitSplPacket");
  return 0;
}

int CMD_SoftTRXVU_ComponenetReset(sat_packet_t *cmd) {
  return IsisTrxvu_softReset(ISIS_TRXVU_I2C_BUS_INDEX);
}

int CMD_HardTRXVU_ComponenetReset(sat_packet_t *cmd) {
  return IsisTrxvu_hardReset(ISIS_TRXVU_I2C_BUS_INDEX);
}

int CMD_ResetComponent(sat_packet_t *cmd) {
  char reset_ = cmd->data[0];
  imepsv2_piu__replyheader_t reply_header;
  switch (reset_) {
  case reset_software:

    break;
  case reset_hardware:

    break;
  case reset_eps:
    imepsv2_piu__reset(0, &reply_header);
    break;
  case reset_trxvu_hard:

    break;
  case reset_trxvu_soft:

    break;
  case reset_ant_SideA:

    break;
  case reset_ant_SideB:

    break;
  case reset_filesystem:

    break;
  }
}
