#include "TRXVU_Commands.h"
#include "SubSystemModules/Communication/TRXVU.h"
#include "SubSystemModules/Communication/dump.h"
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

int CMD_StartDump(sat_packet_t *cmd){
  dump_arguments_t* args = (dump_arguments_t* ) cmd->data;
  startDump(args);
  return 0;
}

int CMD_SendDumpAbortRequest(sat_packet_t *cmd){
  abortDump(); 
  return 0;
}

int CMD_ForceDumpAbort(sat_packet_t *cmd){
  forceAbortDump();
  return 0;
}
