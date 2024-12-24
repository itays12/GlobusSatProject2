
#ifndef EPS_COMMANDS_H_
#define EPS_COMMANDS_H_

#include "SubSystemModules/Communication/SatCommandHandler.h"

typedef struct __attribute__ ((__packed__)) UpdateThresholdVoltages_Params{
  EpsModeVolts volts;
}UpdateThresholdVoltages_Params;

int CMD_UpdateThresholdVoltages(sat_packet_t *cmd);

int CMD_GetThresholdVoltages(sat_packet_t *cmd);

int CMD_RestoreDefaultThresholdVoltages(sat_packet_t *cmd);

int CMD_GetCurrentMode(sat_packet_t *cmd);

int CMD_EPS_ResetWDT(sat_packet_t *cmd);

int CMD_EPS_SetChannels(sat_packet_t *cmd);

int CMD_SetChannels3V3_On(sat_packet_t *cmd);

int CMD_SetChannels3V3_Off(sat_packet_t *cmd);

int CMD_SetChannels5V_On(sat_packet_t *cmd);

int CMD_SetChannels5V_Off(sat_packet_t *cmd);

int CMD_GetEpsParemeter(sat_packet_t *cmd);

int CMD_SetEpsParemeter(sat_packet_t *cmd);

int CMD_ResetParameter(sat_packet_t *cmd);

int CMD_ResetConfig(sat_packet_t *cmd);

int CMD_LoadConfig(sat_packet_t *cmd);

int CMD_SaveConfig(sat_packet_t *cmd);

// solar panel
int CMD_SolarPanelWake(sat_packet_t *cmd);

int CMD_SolarPanelSleep(sat_packet_t *cmd);

int CMD_GetSolarPanelState(sat_packet_t *cmd);

#endif /* EPS_COMMANDS_H_ */
