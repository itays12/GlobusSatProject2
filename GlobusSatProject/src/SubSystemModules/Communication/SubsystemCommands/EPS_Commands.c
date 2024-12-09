#include "EPS_Commands.h"
#include "SubSystemModules/Communication/SPL.h"
#include "SubSystemModules/Communication/SatCommandHandler.h"
#include "SubSystemModules/Communication/TRXVU.h"
#include "SubSystemModules/PowerManagment/EPS.h"
#include "SubSystemModules/PowerManagment/EPSOperationModes.h"
#include "satellite-subsystems/IsisSolarPanelv2.h"
#include "utils.h"

int CMD_UpdateThresholdVoltages(sat_packet_t *cmd) {
  UpdateThresholdVoltages_Params* params = (UpdateThresholdVoltages_Params*)cmd->data;
  UpdateThresholdVoltages(params->volts);
  return 0;
}

int CMD_GetThresholdVoltages(sat_packet_t *cmd) {
  EpsModeVolts voltages = GetThresholdVoltages();
  sat_packet_t packet;
  PROPEGATE_ERROR(AssembleCommand(&voltages, sizeof(EpsModeVolts), eps_cmd_type,
                                  GET_THRESHOLD_VOLTAGES, 0, &packet),
                  "AssembleCommand");
  PROPEGATE_ERROR(TransmitSplPacket(&packet, NULL), "TransmitSplPacket");
  return 0;
}
// eps modes
int CMD_EnterFullMode(sat_packet_t *cmd) { return EnterFullMode(); }

int CMD_EnterCriticalMode(sat_packet_t *cmd) { return EnterCriticalMode(); }

int CMD_EnterSafeMode(sat_packet_t *cmd) { return EnterSafeMode(); }

int CMD_GetCurrentMode(sat_packet_t *cmd) { return GetSystemState(); }

int CMD_SetChannels3V3_On(sat_packet_t *cmd) {
  return GomEpsSetSingleOutput(EPS_I2C_BUS_INDEX, gomeps_channel_3,
                               gomeps_channel_on, 0);
}

int CMD_SetChannels3V3_Off(sat_packet_t *cmd) {
  return GomEpsSetSingleOutput(EPS_I2C_BUS_INDEX, gomeps_channel_3,
                               gomeps_channel_off, 0);
}

int CMD_SetChannels5V_On(sat_packet_t *cmd) {
  return GomEpsSetSingleOutput(EPS_I2C_BUS_INDEX, gomeps_channel_5,
                               gomeps_channel_on, 0);
}

int CMD_SetChannels5V_Off(sat_packet_t *cmd) {
  return GomEpsSetSingleOutput(EPS_I2C_BUS_INDEX, gomeps_channel_5,
                               gomeps_channel_off, 0);
}
// solar panel
int CMD_SolarPanelWake(sat_packet_t *cmd) { return IsisSolarPanelv2_wakeup(); }

int CMD_SolarPanelSleep(sat_packet_t *cmd) { return IsisSolarPanelv2_sleep(); }

int CMD_GetSolarPanelState(sat_packet_t *cmd) {
  IsisSolarPanelv2_State_t solar_state = IsisSolarPanelv2_getState();
  sat_packet_t packet;
  PROPEGATE_ERROR(AssembleCommand(&solar_state , sizeof(IsisSolarPanelv2_State_t), eps_cmd_type,
                                  GET_SOLAR_PANEL_STATE, 0, &packet),
                  "AssembleCommand");
  PROPEGATE_ERROR(TransmitSplPacket(&packet, NULL), "TransmitSplPacket");
  return 0;
}
