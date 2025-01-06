#include "FS_Commands.h"
#include "FRAM_FlightParameters.h"
#include "SubSystemModules/Communication/TRXVU.h"
#include <hcc/api_fat.h>

int CMD_FileSystemError(sat_packet_t *cmd){
  // we dont call logError here, since we might call this when the filesystem is at a bad state
  FileSystemError err;
  FRAM_READ_FIELD(&err, fsError);
  return TransmitDataAsSPL_Packet(cmd, &err, sizeof(err));
}

int CMD_DeleteAllFiles(sat_packet_t *cmd){
	return f_format(f_getdrive(), F_FAT32_MEDIA);
}

int CMD_getFreeSpace(sat_packet_t* cmd){
	FN_SPACE freeSpace;
	PROPEGATE_ERROR(fm_getfreespace(f_getdrive(), &freeSpace), "getFreeSpace");
	return TransmitDataAsSPL_Packet(cmd, &freeSpace.free, sizeof(freeSpace.free));
}

int CMD_DeleteOldestFile(sat_packet_t* cmd){
	return deleteOldestFile();
}
