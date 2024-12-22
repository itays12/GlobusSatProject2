#include "CommandDictionary.h"
#include "SubSystemModules/Communication/SPL.h"
#include "SubSystemModules/Communication/SubsystemCommands/EPS_Commands.h"
#include "SubSystemModules/Communication/SubsystemCommands/TRXVU_Commands.h"

int eps_command_router(sat_packet_t *cmd) {
  int err = 0;
  switch (cmd->cmd_subtype) {
  case GET_THRESHOLD_VOLTAGES:
    err = CMD_GetThresholdVoltages(cmd);
    break;
  case UPDATE_THRESHOLD_VOLTAGES:
    err = CMD_UpdateThresholdVoltages(cmd);
    break;
  case GET_SOLAR_PANEL_STATE:
    err = CMD_GetSolarPanelState(cmd);
    break;
  case SOLAR_PANEL_SLEEP:
    err = CMD_SolarPanelSleep(cmd);
    break;
  case SOLAR_PANEL_WAKE:
    err = CMD_SolarPanelWake(cmd);
    break;
  case SET_3V3_CHANNELS_ON:
    err = CMD_SetChannels3V3_On(cmd);
    break;
  case SET_3V3_CHANNELS_OFF:
    err = CMD_SetChannels3V3_Off(cmd);
    break;
  case SET_5V_CHANNELS_ON:
    err = CMD_SetChannels5V_On(cmd);
    break;
  case SET_5V_CHANNELS_OFF:
    err = CMD_SetChannels5V_Off(cmd);
    break;
  }
  return err;
}

int trxvu_command_router(sat_packet_t *cmd) {
  int err = 0;
  switch (cmd->cmd_subtype) {
  case MUTE_TRXVU:
    err = CMD_MuteTRXVU(cmd);
    break;
  case UNMUTE_TRXVU:
    err = CMD_UnMuteTRXVU(cmd);
    break;
  case PING:
    err = CMD_Ping(cmd);
    break;
  case DUMP_TIME_RANGE:
    err = CMD_StartDump(cmd);
  case ABORT_DUMP_SUBTYPE:
    err = CMD_SendDumpAbortRequest(cmd);
  case FORCE_ABORT_DUMP_SUBTYPE:
    err = CMD_ForceDumpAbort(cmd);
    case SET_BEACON_CYCLE_TIME:
    err = CMD_SetBeaconInterval(cmd);
  }
  return err;
}

int telemetry_command_router(sat_packet_t *cmd) {
  return 0;
}

int filesystem_command_router(sat_packet_t *cmd) { return 0; }

int managment_command_router(sat_packet_t *cmd) { return 0; }
