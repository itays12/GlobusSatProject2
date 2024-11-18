#include "SubSystemModules/Communication/SubsystemCommands/Maintanence_Commands.h"
#include "hal/Drivers/I2C.h"

int CMD_GenericI2C(sat_packet_t *cmd){
  CMD_GenericI2C_Params* params = (CMD_GenericI2C_Params*)cmd->data;
  I2C_write(params->adress, params->buffer, params->size);
  return 0;
}
int CMD_GetSatUptime(){
unsigned int cur_time;
PROPEGATE_ERROR(Time_getUnixEpoch(&cur_time), "GetTime");
  sat_packet_t cmd;
  PROPEGATE_ERROR(TransmitDataAsSPL_Packet(&cmd, &cur_time, sizeof(unsigned int)), "TransmitSplPacket");
  return 0;

}


