#include "Maintenance.h"
#include "FRAM_FlightParameters.h"
#include "SysI2CAddr.h"
#include "hal/Timing/Time.h"
#include "hcc/api_fat.h"
#include "satellite-subsystems/isismepsv2_ivid7_piu.h"
#include "utils.h"
#include <hal/boolean.h>
#include <stdio.h>
#include "TLM_management.h"


int HardResetMCU(){
  isismepsv2_ivid7_piu__replyheader_t replay;
  PROPEGATE_ERROR(isismepsv2_ivid7_piu__reset(EPS_I2C_ADDR, &replay), "RestartMcu");
  return 0;
}

void Maintenance() {
  DeleteOldFiles(0x1000);
  IsFS_Corrupted();
  if (IsGroundCommunicationWDTKick()) {
    ; // needs to reset
  }
}

Boolean CheckExecutionTime(time_unix prev_time, time_unix period) {
  unsigned int cur_time = 0;
  int err = Time_getUnixEpoch(&cur_time);

  logError(err, "error in CheckExecutionTime Time_g");

  if (cur_time - prev_time >= period)
    return TRUE;
  else
    return FALSE;
}

void DeleteOldFiles(unsigned long minFreeSpace) {
  FN_SPACE freeSpace;
  fm_getfreespace(0, &freeSpace);
  while (freeSpace.free < minFreeSpace) {
    deleteOldestFile();
    fm_getfreespace(0, &freeSpace);
  }
}

Boolean IsFS_Corrupted() {
  FN_SPACE freeSpace;
  fm_getfreespace(0, &freeSpace);
  if (freeSpace.bad > 0)
    return TRUE;
  return FALSE;
}

void ResetGroundCommWDT() {
  unsigned int time_from_last_gc;
  Time_getUnixEpoch(&time_from_last_gc);
  FRAM_WRITE_FIELD(&time_from_last_gc, lastCommTime);
  // put in trx logic
}

Boolean IsGroundCommunicationWDTKick() {
  unsigned int time_from_last_gc;
  FRAM_READ_FIELD(&time_from_last_gc, lastCommTime);
  unsigned int cur_time;
  int err = Time_getUnixEpoch(&cur_time);
  unsigned int WDT_kick;
  FRAM_READ_FIELD(&WDT_kick, WDTkicktime);

  if (err == 0)
    if (time_from_last_gc + WDT_kick < cur_time)
      return TRUE;
  return FALSE;
}
