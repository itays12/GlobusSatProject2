#include "EPS.h"
#include <satellite-subsystems/GomEPS.h>
#include <stdio.h>
#include "utils.h"

voltage_t last_voltage;
int voltage_tend;

int EPS_Init(void)
{
    unsigned char i2c_address = 0x02;
    int err;

	err = GomEpsInitialize(&i2c_address, 1);
	if(err != E_NO_SS_ERR && err != E_IS_INITIALIZED)
	{
		// we have a problem. Indicate the error. But we'll gracefully exit to the higher menu instead of
		// hanging the code
		logError(err,"GomEpsInitialize() failed");
		return FALSE;
	}

	return err;
}

int EPS_Conditioning()
{
	int ret=0;
	voltage_t t;
	voltage_tend = getFilteredVolt(t);
	ret = GetBatteryVoltage(&t);
	last_voltage = t;
	if(ret!=0)
	{
		return -1;
	}
	if(voltage_tend==-1)
	{
		if(t>=FULL_MODE_DOWN_TEND)
		{
			return EnterFullMode();
		}

		if(t>=CRUISE_MODE_DOWN_TEND)
		{
			return EnterCruiseMode();
		}
		if(t>=SAFE_MODE_DOWN_TEND)
		{
			return EnterSafeMode();
		}
		else
		{
			return EnterCriticalMode();
		}
	}
	else
	{
		if(t>=FULL_MODE_UP_TEND)
		{
			return EnterFullMode();
		}

		if(t>=CRUISE_MODE_UP_TEND)
		{
			return EnterCruiseMode();
		}
		if(t>=SAFE_MODE_UP_TEND)
		{
			return EnterSafeMode();
		}
		else
		{
			return EnterCriticalMode();
		}
	}
}

int GetBatteryVoltage(voltage_t *vbat)
{
	gom_eps_hk_t response;
	int err = (GomEpsGetHkData_general(0,&response));
	logError(err, "failed to get battery voltage");
	*vbat = response.fields.vbatt;
	return err;
}

int getFilteredVolt(voltage_t t)
{
	if(t>=last_voltage)
	{
		return 1;
	}
	return -1;
}

