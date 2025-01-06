
#ifndef TRXVU_COMMANDS_H_
#define TRXVU_COMMANDS_H_

#include "SubSystemModules/Communication/TRXVU.h"
#include "SubSystemModules/Communication/AckHandler.h"
#include "SubSystemModules/Communication/SatCommandHandler.h"

// wait time for taking semaphores
#define WAIT_TIME_SEM_DUMP	3
#define WAIT_TIME_SEM_TX	3
#define MAX_TRANS_TIME  	259200    // max time of transponder - 72 hours


#define ANTENNA_DEPLOYMENT_TIMEOUT 10 //<! in seconds
int CMD_Ping(sat_packet_t* cmd);//Done
typedef struct __attribute__((__packed__)) StartDump_Params{
	time_unix t_start;
	time_unix t_end;
	tlm_type_t dump_type;
}StartDump_Params;

int CMD_StartDump(sat_packet_t *cmd);//Done

int CMD_SendDumpAbortRequest(sat_packet_t *cmd);

int CMD_ForceDumpAbort(sat_packet_t *cmd);

typedef struct MuteTRXVU_Params{
  time_unix mute_time;
}MuteTRXVU_Params;

int CMD_MuteTRXVU(sat_packet_t *cmd);

//TODO
int CMD_SetIdleState(sat_packet_t *cmd);
//TODO
int CMD_SetTransponder(sat_packet_t *cmd);
//TODO
int CMD_SetRSSITransponder(sat_packet_t *cmd);
//DOne
int CMD_UnMuteTRXVU(sat_packet_t *cmd);
typedef struct SetBeaconInterval_Params{
  time_unix beacon_time;
}SetBeaconInterval_Params;
//Done
int CMD_SetBeaconInterval(sat_packet_t *cmd);

int CMD_GetBeaconInterval(sat_packet_t *cmd);

//Unimportent
int CMD_GetTxUptime(sat_packet_t *cmd);

int CMD_GetRxUptime(sat_packet_t *cmd);

int CMD_AntSetArmStatus(sat_packet_t *cmd);

int CMD_AntGetArmStatus(sat_packet_t *cmd);

int CMD_AntGetUptime(sat_packet_t *cmd);

int CMD_AntCancelDeployment(sat_packet_t *cmd);

int CMD_AntennaDeploy(sat_packet_t *cmd);

int CMD_StopReDeployment(sat_packet_t *cmd);

#endif
/*COMMANDS_H_ */
