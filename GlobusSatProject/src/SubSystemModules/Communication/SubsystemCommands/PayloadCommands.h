/*
 * PayloadCommands.h
 *
 *  Created on: 10 בדצמ 2024
 *      Author: Owner
 */

#ifndef PAYLOADCOMMANDS_H_
#define PAYLOADCOMMANDS_H_

#include "SubSystemModules/Communication/SatCommandHandler.h"
#include "payload/updated_payload_drivers.h"

#include "hal/Drivers/I2C.h"
#include <string.h>
#include <hal/Timing/Time.h>
#include <satellite-subsystems/imepsv2_piu.h>
#include <FRAM_FlightParameters.h>



#endif /* PAYLOADCOMMANDS_H_ */


int CMD_PayloadSoftReset(sat_packet_t* cmd);

int CMD_PayloadTurnOff(sat_packet_t* cmd);

int CMD_PayloadTurnOn(sat_packet_t* cmd);
