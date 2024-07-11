#include <hal/boolean.h>
#include "utils.h"
#include "hal/Timing/Time.h"
#include "hcc/api_fat.h"
#include "maintenance.h"
#include "FRAM_FlightParameters.h"

//****Approved by Uri****
Boolean CheckExecutionTime(time_unix prev_time, time_unix period){
	unsigned int cur_time = 0;
	int err = Time_getUnixEpoch(&cur_time);

	logError(err,"error in CheckExecutionTime Time_g");

	if(cur_time-prev_time>=period)
		return TRUE;
	else return FALSE;
}
Boolean CheckExecTimeFromFRAM(unsigned int fram_time_addr, time_unix period){
//period meaning deadline? how to get from fram time adress to the saved fram time
}

void DeleteOldFiles(int minFreeSpace){
	FN_SPACE freeSpace;
	fm_getfreespace(0,&freeSpace);
	if (freeSpace.free < minFreeSpace){
		deleteOldestFile();
	}
}
void deleteOldestFile(){
	//need to fill after finished telemetry
	//f_delete( filename )
	//f_findnext
}
Boolean IsFS_Corrupted(){
	FN_SPACE freeSpace;
	fm_getfreespace(0, &freeSpace);
	if(freeSpace.bad>0)
		return TRUE;
	return FALSE;
}
void SaveSatTimeInFRAM(unsigned int time_addr){
	unsigned int cur_time;
	int err = Time_getUnixEpoch(&cur_time);
	FRAM_write(&cur_time, time_addr, sizeof(unsigned int));
}
