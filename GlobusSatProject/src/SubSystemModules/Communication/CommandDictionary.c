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
		printf("dumping\n");
		err = CMD_StartDump(cmd);
		break;
	case ABORT_DUMP_SUBTYPE:
		err = CMD_SendDumpAbortRequest(cmd);
		break;
	case FORCE_ABORT_DUMP_SUBTYPE:
		err = CMD_ForceDumpAbort(cmd);
		break;
	case SET_BEACON_CYCLE_TIME:
		err = CMD_SetBeaconInterval(cmd);
		break;
	}
	return err;
}

int telemetry_command_router(sat_packet_t *cmd) {
	int err = 0;
	switch (cmd->cmd_subtype) {
	case DELETE_FILE:
		CMD_DeleteOldestFile(cmd);
		break;
	case DELETE_ALL_FILES:
		CMD_DeleteAllFiles(cmd);
		break;
	case GET_FREE_SPACE:
		CMD_getFreeSpace(cmd);
		break;
	}
	return 0;
}

int managment_command_router(sat_packet_t *cmd) {
	int err = 0;
	switch (cmd->cmd_subtype) {
	case UPDATE_SAT_TIME:
		err = CMD_UpdateSatTime(cmd);
		break;
	case GET_SAT_UPTIME:
		err = CMD_GetSatUptime(cmd);
		break;
	case GET_SAT_TIME:
		err = CMD_GetSatTime(cmd);
		break;
	case TRXVU_HARD_RESET_SUBTYPE:
		err = CMD_HardTRXVU_ComponenetReset(cmd);
		break;
	case TRXVU_SOFT_RESET_SUBTYPE:
		err = CMD_SoftTRXVU_ComponenetReset(cmd);
		break;
	}
	return err;
}
