#include "FS_Commands.h"
#include "FRAM_FlightParameters.h"
#include "SubSystemModules/Communication/TRXVU.h"

int CMD_FileSystemError(sat_packet_t *cmd){
  // we dont call logError here, since we might call this when the filesystem is at a bad state
  FileSystemError err;
  FRAM_READ_FIELD(&err, fsError);
  return TransmitDataAsSPL_Packet(cmd, &err, sizeof(err));
}
