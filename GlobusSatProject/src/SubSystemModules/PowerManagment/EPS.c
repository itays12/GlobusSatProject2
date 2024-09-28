#include "EPS.h"
#include <satellite-subsystems/GomEPS.h>
#include "utils.h"
#include "GlobalStandards.h"

voltage_t last_voltage;
int voltage_tend;

int EPS_Init(void) {
	int err;

	err = GomEpsInitialize(&gom_i2c_address, 1);
	if (err != E_NO_SS_ERR && err != E_IS_INITIALIZED) {
		logError(err, "GomEpsInitialize() failed");
		return -1;
	}

	return 0;
}

int EPS_Conditioning() {
	int ret = 0;
	voltage_t t;
	voltage_tend = getFilteredVolt(t);
	ret = logError(GetBatteryVoltage(&t), "GetBatteryVoltage");
	last_voltage = t;

	EpsModeVolts epsModes = GetThresholdVoltages();
	if (ret != 0) {
		return ret;
	}

	if (voltage_tend == -1) {
		if (t >= epsModes.full_mode_down_tend) {
			return EnterFullMode();
		} else if (t >= epsModes.safe_mode_down_tend) {
			return EnterSafeMode();
		} else{
			return EnterCriticalMode();
		}
	} else {
		if (t >= epsModes.full_mode_up_tend) {
			return EnterFullMode();
		} else if (t >= epsModes.safe_mode_up_tend) {
			return EnterSafeMode();
		} else{
			return EnterCriticalMode();
		}
	}
}

EpsModeVolts GetThresholdVoltages() {
	EpsModeVolts epsModes;
	FRAM_READ_FIELD(&epsModes, epsModes);

	return epsModes;
}

int UpdateThresholdVoltages(EpsModeVolts epsModes) {
	FRAM_WRITE_FIELD(&epsModes, epsModes);

	return 0;
}

int RestoreDefaultThresholdVoltages(){
	EpsModeVolts epsModes;
	epsModes.full_mode_up_tend = DEFFAULT_FULL_MODE_UP_TEND;
	epsModes.full_mode_down_tend = DEFFAULT_FULL_MODE_DOWN_TEND;


	epsModes.safe_mode_up_tend = DEFFAULT_SAFE_MODE_UP_TEND;
	epsModes.safe_mode_down_tend = DEFFAULT_SAFE_MODE_DOWN_TEND;

	epsModes.critical_mode_up_tend = DEFFAULT_CIRTICAL_MODE_UP_TEND;
	epsModes.critical_mode_down_tend = DEFFAULT_CIRTICAL_MODE_DOWN_TEND;

	return logError(UpdateThresholdVoltages(epsModes), "RestoreDefaultThresholdVoltages");
}

int GetBatteryVoltage(voltage_t *vbat) {
	gom_eps_hk_t response;
	int err = (GomEpsGetHkData_general(0, &response));
	if (err != E_NO_SS_ERR && err != E_IS_INITIALIZED) {
		logError(err, "failed to get battery voltage");
		return err;
	}

	*vbat = response.fields.vbatt;
	return 0;
}

int getFilteredVolt(voltage_t t) {
	if (t >= last_voltage) {
		return 1;
	}
	return -1;
}
