
#ifndef MAINTANENCE_COMMANDS_H_
#define MAINTANENCE_COMMANDS_H_

#include "SubSystemModules/Communication/SatCommandHandler.h"

typedef enum __attribute__((__packed__)) reset_type_t {
  reset_software,
  reset_hardware,
  reset_eps,
  reset_trxvu_hard,
  reset_trxvu_soft,
  reset_ant_SideA,
  reset_ant_SideB,
  reset_filesystem
} reset_type_t;

typedef struct __attribute__((__packed__)) CMD_GenericI2C_Params {
  unsigned int adress;
  int size;
  unsigned char buffer[100];
} CMD_GenericI2C_Params;

int CMD_GenericI2C(sat_packet_t *cmd); // done

int CMD_FRAM_ReadAndTransmitt(sat_packet_t *cmd); // X

int CMD_FRAM_WriteAndTransmitt(sat_packet_t *cmd); // X

int CMD_FRAM_Start(sat_packet_t *cmd); // X

int CMD_FRAM_Stop(sat_packet_t *cmd); // X

int CMD_FRAM_GetDeviceID(sat_packet_t *cmd); // X

int CMD_FRAM_ReStart(sat_packet_t *cmd); // X

typedef __attribute__((__packed__))struct UpdateSatTime_Params{
	time_unix time;
}UpdateSatTime_Params;

int CMD_UpdateSatTime(sat_packet_t *cmd); // done

int CMD_GetSatTime(sat_packet_t *cmd); // done

int CMD_GetSatUptime(sat_packet_t *cmd);

int CMD_SoftTRXVU_ComponenetReset(sat_packet_t *cmd);

int CMD_HardTRXVU_ComponenetReset(sat_packet_t *cmd);

/*!
 * @brief 	starts a reset according to 'reset_type_t'
 * 			The function will raise the reset flag and send an ACK
 * before starting the reset.
 * @return 	0 on success
 * 			Error code according to <hal/errors.h>
 */
int CMD_ResetComponent(sat_packet_t *cmd); // not finished

#endif /* MAINTANENCE_COMMANDS_H_ */
