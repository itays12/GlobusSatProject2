#include <hal/Storage/FRAM.h>
#include <hal/errors.h>
#include <hal/Drivers/I2C.h>
#include <hal/Timing/Time.h>
#include "GlobalStandards.h"
#include "InitSystem.h"
#include "SubSystemModules/PowerManagment/EPS.h"
#include "SubSystemModules/Communication/TRXVU.h"
#include "utils.h"

//default don't argue
#define SPEED_i2cBusSpeed_Hz 100000
#define TIMEOUT_i2cBusSpeed_Hz 10

//default again
#define TIME_year 2007
#define Time_month 7
#define Time_day 30


int StartFRAM()
{
	int flag = FRAM_start();
	if(flag != E_NO_SS_ERR)
	{
		//TODO : Telemetry log error
		// TODO: after file system log crate
		logError(flag, "Error starting FRAM");
	}
	return flag;
}

int StartTime()
{
	//Epoch
	const Time t = UNIX_DATE_JAN_D1_Y2000;
	int flag = Time_start(&t, 0);
	return flag;
}

int InitSubsystem()
{
	int flag;
	flag = StartI2C();
	if(flag != E_NO_SS_ERR)
	{
		return 1;
	}
	flag = StartFRAM();
	if(flag != E_NO_SS_ERR)
	{
		return 1;
	}
	flag = StartTime();
	if(flag != E_NO_SS_ERR)
	{
		return 1;
	}
	flag = EPS_Init();
	if(flag != E_NO_SS_ERR){
		return 1;
	}
	flag = EPS_Init();
	if(flag != E_NO_SS_ERR){
		return 1;
	}
	flag = IsisTRXVUdemoInit();
	if(flag != E_NO_SS_ERR){
		return 1;
	}

	return 0;

}



int StartI2C()
{
	int flag = I2C_start(SPEED_i2cBusSpeed_Hz,TIMEOUT_i2cBusSpeed_Hz);
	return flag;
}
