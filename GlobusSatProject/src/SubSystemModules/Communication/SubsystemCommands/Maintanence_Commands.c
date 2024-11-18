#include "SubSystemModules/Communication/SubsystemCommands/Maintanence_Commands.h"
#include "hal/Drivers/I2C.h"

int CMD_GenericI2C(sat_packet_t *cmd){
  CMD_GenericI2C_Params* params = (CMD_GenericI2C_Params*)cmd->data;
  I2C_write(params->adress, params->buffer, params->size);
  return 0;
}

