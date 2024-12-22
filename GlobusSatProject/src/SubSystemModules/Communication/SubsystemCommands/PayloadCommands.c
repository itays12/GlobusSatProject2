#include "SubSystemModules/Communication/SubsystemCommands/PayloadCommands.h"
#include "SubSystemModules/Communication/SatCommandHandler.h"
#include "payload/updated_payload_drivers.h"
#include "hal/Drivers/I2C.h"
#include <string.h>
#include <hal/Timing/Time.h>
#include <satellite-subsystems/imepsv2_piu.h>
#include <FRAM_FlightParameters.h>




int CMD_PayloadSoftReset(sat_packet_t* cmd){
  return payloadSoftReset();
}


int CMD_PayloadTurnOn(sat_packet_t *cmd){
  return payloadTurnOn();
}


int CMD_PayloadTurnOff(sat_packet_t *cmd){
  return payloadTurnOff();
}
