#include "SubSystemModules/Communication/SubsystemCommands/Maintanence_Commands.h"
#include "SubSystemModules/Communication/TRXVU.h"
#include "SysI2CAddr.h"
#include "hal/Drivers/I2C.h"
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
  return isis_vu_e__reset_hw_rx(ISIS_TRXVU_I2C_BUS_INDEX);
}

int CMD_HardTRXVU_ComponenetReset(sat_packet_t *cmd) {
  return isis_vu_e__reset_hw_rx(ISIS_TRXVU_I2C_BUS_INDEX);
}
