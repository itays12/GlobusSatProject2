#include "SatCommandHandler.h"
#include "CommandDictionary.h"
#include "SPL.h"
#include <string.h>
#include <stddef.h>
#include "SubSystemModules/Communication/AckHandler.h"
#include "utils.h"


int ActUponCommand(sat_packet_t *cmd){
	int err = 0;
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
	}
	if (logError(err, "ActUponCommand") != E_NO_SS_ERR){
    SendAckPacket(ACK_ERR, cmd->ID, &err, sizeof(err));
		return err;
	}
	return 0;
}

int AssembleCommand(void *data, unsigned short data_length, char type, char subtype,unsigned int id, sat_packet_t *cmd){

	cmd->length = data_length ;
	cmd->cmd_type = type ;
	cmd->cmd_subtype = subtype ;
	cmd->ID = id;
	memcpy(cmd->data, data, data_length);

	if(data==NULL){
		return null_pointer_error;
	}
	else{
		return command_succsess;
	}
}


int ParseDataToCommand(unsigned char *data, sat_packet_t *cmd){
  if (data == NULL || cmd == NULL){
    return null_pointer_error;
  }

  unsigned int id = *(data + offsetof(sat_packet_t, ID));
  char cmd_type = *(data + offsetof(sat_packet_t, cmd_type));
  char cmd_subtype = *(data + offsetof(sat_packet_t, cmd_subtype));
  unsigned short len = *(data + offsetof(sat_packet_t, length));
  
  AssembleCommand(data + offsetof(sat_packet_t, data), len, cmd_type, cmd_subtype, id, cmd);
  
  return command_succsess;
}
