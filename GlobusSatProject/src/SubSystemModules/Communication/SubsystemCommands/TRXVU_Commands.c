#include "TRXVU_Commands.h"
#include "SubSystemModules/Communication/TRXVU.h"
#include "SubSystemModules/Communication/dump.h"
#include "SysI2CAddr.h"

int CMD_Ping(sat_packet_t *cmd) {
	return TransmitSplPacket(cmd, NULL);
}

int CMD_MuteTRXVU(sat_packet_t *cmd) {
	MuteTRXVU_Params *params = (MuteTRXVU_Params *) cmd->data;
	muteTransmission(params->mute_time);
	return 0;
}

int CMD_UnMuteTRXVU(sat_packet_t *cmd) {
	unmuteTransmission();
	return 0;
}

int CMD_SetBeaconInterval(sat_packet_t *cmd) {
	SetBeaconInterval_Params *params = (SetBeaconInterval_Params *) cmd->data;
	changeBeaconTime(params->beacon_time);
	return 0;
}

int CMD_TrasmitBeacon(sat_packet_t *cmd) {
	sendBeacon();
	return 0;
}

int CMD_StartDump(sat_packet_t *cmd) {
	StartDump_Params *args = (StartDump_Params *) cmd->data;
	dump_arguments_t dump_args = { 0 };
	dump_args.id = cmd->ID;
	dump_args.dump_type = args->dump_type;
	dump_args.t_start = args->t_start;
	dump_args.t_end = args->t_end;
	startDump(&dump_args);
	return 0;
}

int CMD_SendDumpAbortRequest(sat_packet_t *cmd) {
	abortDump();
	return 0;
}

int CMD_ForceDumpAbort(sat_packet_t *cmd) {
	forceAbortDump();
	return 0;
}
