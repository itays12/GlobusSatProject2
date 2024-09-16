#include "EPS_Commands.h"

int CMD_UpdateThresholdVoltages(sat_packet_t *cmd){
  return 0;
}

int CMD_GetThresholdVoltages(sat_packet_t *cmd){
  return 0;
}

int CMD_UpdateSmoothingFactor(sat_packet_t *cmd){
  return 0;
}

int CMD_RestoreDefaultAlpha(sat_packet_t *cmd){
  return 0;
}

int CMD_RestoreDefaultThresholdVoltages(sat_packet_t *cmd){
  return 0;
}

int CMD_GetSmoothingFactor(sat_packet_t *cmd){
  return 0;
}

// eps modes
int CMD_EnterCruiseMode(sat_packet_t *cmd){
    return EnterCruiseMode();
}

int CMD_EnterFullMode(sat_packet_t *cmd){
    return EnterFullMode();
}

int CMD_EnterCriticalMode(sat_packet_t *cmd){
    return EnterCriticalMode();
}

int CMD_EnterSafeMode(sat_packet_t *cmd){
    return  EnterSafeMode();
}

int CMD_GetCurrentMode(sat_packet_t *cmd){
    return GetSystemState();
}


int CMD_EPS_NOP(sat_packet_t *cmd){
  return 0;
}

int CMD_EPS_ResetWDT(sat_packet_t *cmd){
  return 0;
}

int CMD_EPS_SetChannels(sat_packet_t *cmd){
  return 0;
}

int CMD_SetChannels3V3_On(sat_packet_t *cmd){
    return GomEpsSetSingleOutput(EPS_I2C_BUS_INDEX, gomeps_channel_3, gomeps_channel_on, 0);
}

int CMD_SetChannels3V3_Off(sat_packet_t *cmd){
    return GomEpsSetSingleOutput(EPS_I2C_BUS_INDEX, gomeps_channel_3, gomeps_channel_off, 0);
}

int CMD_SetChannels5V_On(sat_packet_t *cmd){
  return GomEpsSetSingleOutput(EPS_I2C_BUS_INDEX, gomeps_channel_5, gomeps_channel_on, 0);
}

int CMD_SetChannels5V_Off(sat_packet_t *cmd){
  return GomEpsSetSingleOutput(EPS_I2C_BUS_INDEX, gomeps_channel_5, gomeps_channel_off, 0);
}

int CMD_GetEpsParemeter(sat_packet_t *cmd){
  return 0;
}

int CMD_SetEpsParemeter(sat_packet_t *cmd){
  return 0;
}

int CMD_ResetParameter(sat_packet_t *cmd){
  return 0;
}

int CMD_ResetConfig(sat_packet_t *cmd){
  return 0;
}

int CMD_LoadConfig(sat_packet_t *cmd){
  return 0;
}

int CMD_SaveConfig(sat_packet_t *cmd){
  return 0;
}

// solar panel
void CMD_SolarPanelWake(sat_packet_t *cmd){
    IsisSolarPanelv2_wakeup();
}

void CMD_SolarPanelSleep(sat_packet_t *cmd){
    IsisSolarPanelv2_sleep();
}

IsisSolarPanelv2_State_t CMD_GetSolarPanelState(sat_packet_t *cmd){
    return IsisSolarPanelv2_getState();
}
