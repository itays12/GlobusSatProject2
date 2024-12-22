#include "EPS_Commands.h"
#include "SubSystemModules/Communication/SPL.h"
#include "SubSystemModules/Communication/SatCommandHandler.h"
#include "SubSystemModules/Communication/TRXVU.h"
#include "SubSystemModules/PowerManagment/EPS.h"
#include "SubSystemModules/PowerManagment/EPSOperationModes.h"
#include "satellite-subsystems/IsisSolarPanelv2.h"
#include "satellite-subsystems/imepsv2_piu.h"
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

int CMD_GetCurrentMode(sat_packet_t *cmd) { 
  int state = GetSystemState();
  return TransmitDataAsSPL_Packet(cmd, &state, sizeof(state));
}

int CMD_SetChannels3V3_On(sat_packet_t *cmd) {
  imepsv2_piu__replyheader_t reply_header;
  return imepsv2_piu__outputbuschannelon(0,imepsv2_piu__imeps_channel__channel_3v3_perm, &reply_header);
}

int CMD_SetChannels3V3_Off(sat_packet_t *cmd) {
  imepsv2_piu__replyheader_t reply_header;
  return imepsv2_piu__outputbuschanneloff(0,imepsv2_piu__imeps_channel__channel_3v3_perm, &reply_header);
}

int CMD_SetChannels5V_On(sat_packet_t *cmd) {
  imepsv2_piu__replyheader_t reply_header;
  return imepsv2_piu__outputbuschannelon(0,imepsv2_piu__imeps_channel__channel_5v_perm, &reply_header);
}

int CMD_SetChannels5V_Off(sat_packet_t *cmd) {
  imepsv2_piu__replyheader_t reply_header;
  return imepsv2_piu__outputbuschanneloff(0,imepsv2_piu__imeps_channel__channel_5v_perm, &reply_header);
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
