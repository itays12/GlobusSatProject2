#include "GlobalStandards.h"
#include "InitSystem.h"
#include "SubSystemModules/Communication/TRXVU.h"
#include "SubSystemModules/PowerManagment/EPS.h"
#include "utils.h"
#include <hal/Drivers/I2C.h>
#include <hal/Storage/FRAM.h>
#include <hal/Timing/Time.h>
#include <hal/errors.h>
#include "FRAM_FlightParameters.h"

// default don't argue
#define SPEED_i2cBusSpeed_Hz 100000
#define TIMEOUT_i2cBusSpeed_Hz 10

// default again
#define TIME_year 2007
#define Time_month 7
#define Time_day 30

int StartFRAM() {
  int flag = FRAM_start();
  if (flag != E_NO_SS_ERR) {
    // TODO : Telemetry log error
    //  TODO: after file system log crate
    logError(flag, "Error starting FRAM");
  }
  return flag;
}

int StartTime() {
  // Epoch
  const Time t = UNIX_DATE_JAN_D1_Y2000;
  int flag = Time_start(&t, 0);
  return flag;
}

int InitSubsystems() {
  int flag = 0;

  if (logError(StartI2C(), "could not start i2c") != E_NO_SS_ERR) {
    flag = 1;
  }
  if (logError(StartFRAM(), "Could not start FRAM") != E_NO_SS_ERR) {
    flag = 1;
  }
  if (logError(StartTime(), "Could not start Time") != E_NO_SS_ERR) {
    flag = 1;
  }
  if (logError(EPS_Init(), "Could not start EPS") != E_NO_SS_ERR) {
    flag = 1;
  }
  if (logError(InitTrxvu(), "Could not start TRXVU") != E_NO_SS_ERR) {
    flag = 1;
  }

  return flag;
}

int StartI2C() {
  int flag = I2C_start(SPEED_i2cBusSpeed_Hz, TIMEOUT_i2cBusSpeed_Hz);
  return flag;
}

int DeploySystem(){
	WriteDefaultValuesToFRAM();

	return 0;
}

int WriteDefaultValuesToFRAM() {
	PROPEGATE_ERROR(RestoreDefaultThresholdVoltages(), "WriteDefaultValues");
	time_unix WDT_kick_time;
	PROPEGATE_ERROR(Time_getUnixEpoch(&WDT_kick_time), "Time_getUnixEpoch");
	PROPEGATE_ERROR(FRAM_WRITE_FIELD(WDT_kick_time, WDTkicktime), "WriteWDTkicktime");


  time_unix beaconInterval = 60;
	PROPEGATE_ERROR(FRAM_WRITE_FIELD(beaconInterval, beaconInterval), "WriteWDTkicktime");


}
