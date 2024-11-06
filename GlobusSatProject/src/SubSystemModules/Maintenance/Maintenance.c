#include <hal/boolean.h>
#include "utils.h"
#include "hal/Timing/Time.h"
#include "hcc/api_fat.h"
#include "Maintenance.h"
#include "FRAM_FlightParameters.h"

//****Approved by Uri****
void Maintenance(){
	DeleteOldFiles(0);//0 - min space - need to be determined
	IsFS_Corrupted();
	if(IsGroundCommunicationWDTKick()){
		;//needs to reset
	}
}
Boolean CheckExecutionTime(time_unix prev_time, time_unix period){
	unsigned int cur_time = 0;
	int err = Time_getUnixEpoch(&cur_time);

	logError(err,"error in CheckExecutionTime Time_g");

	if(cur_time-prev_time>=period)
		return TRUE;
	else
		return FALSE;
}

/*Boolean CheckExecTimeFromFRAM(unsigned int fram_time_addr, time_unix period){
	time_unix prev_time = FRAM_READ_FIELD(fram_time_addr, 4);
	return (CheckExecutionTime(prev_time, period));
}*/

void deleteOldestFile(){
	//need to fill after finished telemetry
	//f_delete( filename )
	//f_findnext
}

void DeleteOldFiles(int minFreeSpace){
	FN_SPACE freeSpace;
	fm_getfreespace(0,&freeSpace);
	while(freeSpace.free < minFreeSpace){
		deleteOldestFile();
	  fm_getfreespace(0,&freeSpace);
	}
}



Boolean IsFS_Corrupted(){
	FN_SPACE freeSpace;
	fm_getfreespace(0, &freeSpace);
	if(freeSpace.bad>0)
		return TRUE;
	return FALSE;
}

void ResetGroundCommWDT(){
	unsigned int time_from_last_gc;
	Time_getUnixEpoch(&time_from_last_gc);
	FRAM_WRITE_FIELD(&time_from_last_gc, lastCommTime);
		//put in trx logic
}

Boolean IsGroundCommunicationWDTKick(){
	unsigned int time_from_last_gc;
	FRAM_READ_FIELD(&time_from_last_gc, lastCommTime);
	unsigned int cur_time;
	int err = Time_getUnixEpoch(&cur_time);
	unsigned int WDT_kick;
	FRAM_READ_FIELD(&WDT_kick, WDTkicktime);

	if (err == 0)
		if(time_from_last_gc + WDT_kick < cur_time)
			return TRUE;
	return FALSE;
}



