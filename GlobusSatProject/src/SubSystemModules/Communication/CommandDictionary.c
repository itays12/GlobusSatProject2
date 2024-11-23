#include "CommandDictionary.h"
#include "SubSystemModules/Communication/SPL.h"
#include "SubSystemModules/Communication/SubsystemCommands/EPS_Commands.h"
#include "SubSystemModules/Communication/SubsystemCommands/TRXVU_Commands.h"

int trxvu_command_router(sat_packet_t *cmd){
  switch (cmd->cmd_subtype){
    case MUTE_TRXVU:
      CMD_MuteTRXVU(cmd);
      break;
    case UNMUTE_TRXVU: 
      CMD_UnMuteTRXVU(cmd);
      break;
  }
	return 0;
}

int eps_command_router(sat_packet_t *cmd){
  switch (cmd->cmd_subtype){
    case GET_THRESHOLD_VOLTAGES: 
      CMD_GetThresholdVoltages(cmd);
      break;
    case UPDATE_THRESHOLD_VOLTAGES:
      CMD_UpdateThresholdVoltages(cmd);
      break;
    case GET_SOLAR_PANEL_STATE:
      CMD_GetSolarPanelState(cmd);
      break;
    case SOLAR_PANEL_SLEEP:
      CMD_SolarPanelSleep(cmd);
      break;
    case SOLAR_PANEL_WAKE:
      CMD_SolarPanelWake(cmd);
      break;
  }
	return 0;
}

int telemetry_command_router(sat_packet_t *cmd){
	return 0;
}

int filesystem_command_router(sat_packet_t *cmd){
	return 0;
}

int managment_command_router(sat_packet_t *cmd){
	return 0;
}
