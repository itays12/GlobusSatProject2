#include "GlobalStandards.h"
#include "InitSystem.h"
#include "SubSystemModules/Communication/AckHandler.h"
#include "SubSystemModules/Communication/SPL.h"
#include "SubSystemModules/Communication/SatCommandHandler.h"
#include "SubSystemModules/Communication/TRXVU.h"
#include "SubSystemModules/PowerManagment/EPS.h"
#include "utils.h"
#include <hal/Drivers/I2C.h>
#include <hal/Storage/FRAM.h>
#include <hal/Timing/Time.h>
#include <hal/errors.h>
#include "FRAM_FlightParameters.h"
#include <hal/Drivers/SPI.h>
#include <satellite-subsystems/isis_ants.h>
#include <satellite-subsystems/isismepsv2_ivid7_piu.h>
#include "TLM_management.h"

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
	Time t;
	FRAM_READ_FIELD(&t, mustUpdatedTime);

	int flag = Time_start(&t, 0);
	return flag;
}

int StartSPI() {
	return SPI_start(bus1_spi, slave0_spi);
}

#define _PIN_RESET PIN_GPIO08
#define _PIN_INT   PIN_GPIO00

int InitSubsystems() {
	int err = 0;

	logError(StartI2C(), "could not start i2c");
	logError(StartFRAM(), "Could not start FRAM");
	logError(StartTime(), "Could not start Time");
	logError(EPS_Init(), "Could not start EPS");
	logError(InitializeFS(), "Could not initlize FS");
	logError(StartSPI(), "Could not start SPI");
	Pin solarpanelv2_pins[2] = { _PIN_RESET, _PIN_INT };

	logError(
			IsisSolarPanelv2_initialize(slave0_spi, &solarpanelv2_pins[0],
					&solarpanelv2_pins[1]), "Could not start solar panels");
	ISIS_ANTS_t ants[] = { { ANTS_I2C_SIDE_A_ADDR }, { ANTS_I2C_SIDE_B_ADDR } };
	isismepsv2_ivid7_piu__replyheader_t response;
	logError(
			isismepsv2_ivid7_piu__outputbuschannelon(0,
					isismepsv2_ivid7_piu__imeps_channel__channel_5v_sw2,
					&response),
			"InitTrxvuAndAnts - isismepsv2_ivid7_piu__outputbuschannelon");
	logError(ISIS_ANTS_Init(ants, 2), "Could not start ants");
	logError(InitTrxvu(), "Could not start TRXVU");
	unsigned int startupCount = 0;

	FRAM_READ_FIELD(&startupCount, startupCount);
	startupCount += 1;
	FRAM_WRITE_FIELD(&startupCount, startupCount);
	printf("startup count: %d\n", startupCount);

	SendAckPacket(ACK_RESET_WAKEUP, 0, NULL, 0);

	return err;
}

int StartI2C() {
	int flag = I2C_start(SPEED_i2cBusSpeed_Hz, TIMEOUT_i2cBusSpeed_Hz);
	return flag;
}

int DeploySystem() {
	WriteDefaultValuesToFRAM();

	return 0;
}

int WriteDefaultValuesToFRAM() {
	EpsModeVolts epsModes;
	epsModes.full_mode_up_tend = DEFFAULT_FULL_MODE_UP_TEND;
	epsModes.full_mode_down_tend = DEFFAULT_FULL_MODE_DOWN_TEND;

	epsModes.safe_mode_up_tend = DEFFAULT_SAFE_MODE_UP_TEND;
	epsModes.safe_mode_down_tend = DEFFAULT_SAFE_MODE_DOWN_TEND;

	epsModes.critical_mode_up_tend = DEFFAULT_CIRTICAL_MODE_UP_TEND;
	epsModes.critical_mode_down_tend = DEFFAULT_CIRTICAL_MODE_DOWN_TEND;
	FramFlightParams fram = { .trxMute = FALSE, .epsModes = epsModes,
			.WDTkicktime = 0, .beaconInterval = 10, .beaconSendTime = 0,
			.trxMuteTime = 0, .RSSI = 40, .mustUpdatedTime =
					UNIX_DATE_JAN_D1_Y2000, .startupCount = 0 };

	PROPEGATE_ERROR(FRAM_write((unsigned char* )&fram, 0, sizeof(fram)),
			"WriteDefaultValues");

	return 0;
}
