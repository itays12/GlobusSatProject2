#include "TRXVU_Commands.h"
#include "SubSystemModules/Communication/TRXVU.h"

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
