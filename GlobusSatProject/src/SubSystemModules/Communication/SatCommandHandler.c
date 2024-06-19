#include "SatCommandHandler.h"
#include "CommandDictionary.h"
#include "SPL.h"


int ActUponCommand(sat_packet_t *cmd){
	int err;
	switch(cmd->cmd_type){
		case trxvu_cmd_type:
			err = trxvu_command_router(cmd);
			break;
		case eps_cmd_type:
			err = eps_command_router(cmd);
			break;
		case telemetry_cmd_type:
			err = telemetry_command_router(cmd);
			break;
		case filesystem_cmd_type:
			err = filesystem_command_router(cmd);
			break;
		case managment_cmd_type:
			err = managment_command_router(cmd);
			break;
		case ack_type:
			//TODO: handle ack packets
			break;
		case dump_type:
			//TODO: handle dump packets
			break;
	}
	if (logError(err, "failed to run command: ") != E_NO_SS_ERR){
		return err;
	}
	return 0;
}
